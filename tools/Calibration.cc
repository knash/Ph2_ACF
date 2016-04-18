#include "Calibration.h"

void Calibration::Initialise ( bool pAllChan )
{
    // Initialize the TestGroups
    MakeTestGroups ( pAllChan );

    // now read the settings from the map
    auto cSetting = fSettingsMap.find ( "HoleMode" );
    fHoleMode = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second : 1;
    cSetting = fSettingsMap.find ( "TargetVcth" );
    fTargetVcth = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second : 0x78;
    cSetting = fSettingsMap.find ( "TargetOffset" );
    fTargetOffset = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second : 0x50;
    cSetting = fSettingsMap.find ( "Nevents" );
    fEventsPerPoint = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second : 10;
    cSetting = fSettingsMap.find ( "TestPulseAmplitude" );
    fTestPulseAmplitude = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second : 0;
    cSetting = fSettingsMap.find ( "VerificationLoop" );
    fCheckLoop = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second : 1;
    cSetting = fSettingsMap.find ( "Epsilon" );
    fEpsilon = ( cSetting != std::end ( fSettingsMap ) ) ? cSetting->second / float (100) : 0.1;

    if ( fTestPulseAmplitude == 0 ) fTestPulse = 0;
    else fTestPulse = 1;


    // Canvases
    fVplusCanvas = new TCanvas ( "VPlus", "VPlus", 515, 0, 500, 500 );
    fOffsetCanvas = new TCanvas ( "Offset", "Offset", 10, 0, 500, 500 );
    fOccupancyCanvas = new TCanvas ( "Occupancy", "Occupancy", 10, 525, 500, 500 );

    // count FEs & CBCs
    uint32_t cCbcCount = 0;
    uint32_t cCbcIdMax = 0;
    uint32_t cFeCount = 0;

    for ( auto cBoard : fBoardVector )
    {
        uint32_t cBoardId = cBoard->getBeId();

        for ( auto cFe : cBoard->fModuleVector )
        {
            uint32_t cFeId = cFe->getFeId();
            cFeCount++;

            for ( auto cCbc : cFe->fCbcVector )
            {
                uint32_t cCbcId = cCbc->getCbcId();
                cCbcCount++;

                if ( cCbcId > cCbcIdMax ) cCbcIdMax = cCbcId;

                fVplusMap[cCbc] = RegPair (0, false);

                TString cName = Form ( "h_VplusValues_Fe%dCbc%d", cFeId, cCbcId );
                TObject* cObj = gROOT->FindObject ( cName );

                if ( cObj ) delete cObj;

                TProfile* cHist = new TProfile ( cName, Form ( "Vplus Values for Test Groups FE%d CBC%d ; Test Group; Vplus", cFeId, cCbcId ), 8, -.5, 7.5 );
                cHist->SetMarkerStyle ( 20 );
                // cHist->SetLineWidth( 2 );
                bookHistogram ( cCbc, "Vplus", cHist );

                cName = Form ( "h_Offsets_Fe%dCbc%d", cFeId, cCbcId );
                cObj = gROOT->FindObject ( cName );

                if ( cObj ) delete cObj;

                TH1F* cOffsetHist = new TH1F ( cName, Form ( "Offsets FE%d CBC%d ; Channel; Offset", cFeId, cCbcId ), 254, -.5, 253.5 );
                uint8_t cOffset = ( fHoleMode ) ? 0x00 : 0xFF;

                for ( int iBin = 0; iBin < NCHANNELS; iBin++ )
                    cOffsetHist->SetBinContent ( iBin, cOffset );

                bookHistogram ( cCbc, "Offsets", cOffsetHist );

                cName = Form ( "h_Occupancy_Fe%dCbc%d", cFeId, cCbcId );
                cObj = gROOT->FindObject ( cName );

                if ( cObj ) delete cObj;

                TH1F* cOccHist = new TH1F ( cName, Form ( "Occupancy FE%d CBC%d ; Channel; Occupancy", cFeId, cCbcId ), 254, -.5, 254.5 );
                bookHistogram ( cCbc, "Occupancy", cOccHist );
            }
        }

        fNCbc = cCbcCount;
        fNFe = cFeCount;
    }

    uint32_t cPads = ( cCbcIdMax > cCbcCount ) ? cCbcIdMax : cCbcCount;

    fVplusCanvas->DivideSquare ( cPads );
    fOffsetCanvas->DivideSquare ( cPads );
    fOccupancyCanvas->DivideSquare ( cPads );


    std::cout << "Created Object Maps and parsed settings:" << std::endl;
    std::cout << "	Hole Mode = " << fHoleMode << std::endl;
    std::cout << "	Nevents = " << fEventsPerPoint << std::endl;
    std::cout << "	TargetVcth = " << int ( fTargetVcth ) << std::endl;
    std::cout << "	TargetOffset = " << int ( fTargetOffset ) << std::endl;
    std::cout << "	TestPulseAmplitude = " << int ( fTestPulseAmplitude ) << std::endl;
    std::cout << "	Epsilon = " << float ( fEpsilon ) << std::endl;
}

void Calibration::MakeTestGroups ( bool pAllChan )
{
    if ( !pAllChan )
    {
        for ( int cGId = 0; cGId < 8; cGId++ )
        {
            std::vector<RegPair> tempchannelVec;
            //std::vector<uint8_t> tempchannelVec;

            for ( int idx = 0; idx < 16; idx++ )
            {
                int ctemp1 = idx * 16 + cGId * 2;
                int ctemp2 = ctemp1 + 1;

                //if ( ctemp1 < 254 ) tempchannelVec.push_back ( ctemp1);
                if ( ctemp1 < 254 ) tempchannelVec.push_back ( RegPair (ctemp1, false ) );

                //if ( ctemp2 < 254 )  tempchannelVec.push_back ( ctemp2);
                if ( ctemp2 < 254 )  tempchannelVec.push_back ( RegPair (ctemp2, false ) );
            }

            fTestGroupChanMap[cGId] = tempchannelVec;
        }
    }
    else
    {
        int cGId = -1;
        std::vector<RegPair> tempchannelVec;
        //std::vector<uint8_t> tempchannelVec;

        for ( int idx = 0; idx < 254; idx++ )
            tempchannelVec.push_back ( RegPair (idx, false ) );

        //tempchannelVec.push_back ( idx );

        fTestGroupChanMap[cGId] = tempchannelVec;
    }
}


void Calibration::FindVplus()
{
    // first, set VCth to the target value for each CBC
    CbcRegWriter cWriter ( fCbcInterface, "VCth", fTargetVcth );
    accept ( cWriter );

    // now all offsets are either off (0x00 in holes mode, 0xFF in electrons mode)
    // next a group needs to be enabled - therefore now the group loop
    std::cout << BOLDBLUE << "Extracting Vplus ..." << RESET << std::endl;

    for ( auto& cTGroup : fTestGroupChanMap )
    {
        // start with a fresh <Cbc, Vplus> map
        clearVPlusMap();

        // looping over the test groups, enable it
        std::cout << GREEN << "Enabling Test Group...." << cTGroup.first << RESET << std::endl;
        setOffset ( fTargetOffset, cTGroup.first, true ); // takes the group ID

        bitwiseVplus ( cTGroup.first );

        std::cout << RED << "Disabling Test Group...." << cTGroup.first << RESET << std::endl << std::endl;
        uint8_t cOffset = ( fHoleMode ) ? 0x00 : 0xFF;
        setOffset ( cOffset, cTGroup.first, true );

        // done looping all the bits - I should now have the Vplus value that corresponds to 50% occupancy at the desired VCth and Offset for this test group mapped against CBC
        for ( auto& cCbc : fVplusMap )
        {
            TProfile* cTmpProfile = static_cast<TProfile*> ( getHist ( cCbc.first, "Vplus" ) );
            cTmpProfile->Fill ( cTGroup.first, cCbc.second.fValue ); // fill Vplus value for each test group
        }

        updateHists ( "Offsets" );
        updateHists ( "Occupancy" );
        updateHists ( "Vplus" );
    }

    // done extracting reasonable Vplus values for all test groups, now find the mean
    // since I am lazy and do not want to iterate all boards, FEs etc, i Iterate fVplusMap
    for ( auto& cCbc : fVplusMap ) //this toggles bit i on Vplus for each
    {
        TProfile* cTmpProfile = static_cast<TProfile*> ( getHist ( cCbc.first, "Vplus" ) );
        cCbc.second.fValue = cTmpProfile->GetMean ( 2 );

        fCbcInterface->WriteCbcReg ( cCbc.first, "Vplus", cCbc.second.fValue );
        std::cout << BOLDGREEN <<  "Mean Vplus value for FE " << +cCbc.first->getFeId() << " CBC " << +cCbc.first->getCbcId() << " is " << BOLDRED << +cCbc.second.fValue << RESET << std::endl;
    }
}

void Calibration::bitwiseVplus ( int pTGroup )
{
    // now go over the VPlus bits for each CBC, start with the MSB, flip it to one and measure the occupancy
    for ( int iBit = 7; iBit >= 0; iBit-- )
    {
        for ( auto& cCbc : fVplusMap ) //this toggles bit i on Vplus for each
        {
            //cCbc.second is a RegPair and the bit is only toggled if RegPair.fFinal is false
            toggleRegBit ( cCbc.second, iBit );

            // also, only if it is not yet the final value, will the new value be written
            if (!cCbc.second.final() ) fCbcInterface->WriteCbcReg ( cCbc.first, "Vplus", cCbc.second.fValue );
        }

        // now each CBC has the MSB Vplus Bit written
        // now take data
        measureOccupancy ( fEventsPerPoint, pTGroup );

        // done taking data, now find the occupancy per CBC
        for ( auto& cCbc : fVplusMap )
        {
            // if the occupancy is larger than 0.5 I need to flip the bit back to 0, else leave it
            float cOccupancy = findCbcOccupancy ( cCbc.first, pTGroup, fEventsPerPoint );

            //std::cout << "VPlus " << +cCbc.second.fValue << " = 0b" << std::bitset<8> ( cCbc.second.fValue ) << " on CBC " << +cCbc.first->getCbcId() << " Occupancy : " << cOccupancy << " " << cCbc.second.fOvershoot << " " << cCbc.second.fUndershoot << std::endl;

            //if the diff between occupancy and 0.5 is smaller epsilon, I am happy & it sets the final parameter to true
            if (fabs (cOccupancy - 0.5) < fEpsilon)
            {
                cCbc.second.fFinal = true;
                //std::cout << BOLDGREEN << "Occupancy sufficiently close to 50% (+-" << fEpsilon * 100 << "%) for Cbc " << +cCbc.first->getCbcId() << " at Bit " << iBit << RESET << std::endl;
                // clear the occupancy histogram for the final check
                clearOccupancyHists ( cCbc.first );
                continue;
            }

            else
            {
                if (fHoleMode)
                {
                    if (cOccupancy > 0.5)
                    {
                        toggleRegBit ( cCbc.second, iBit );
                        fCbcInterface->WriteCbcReg ( cCbc.first, "Vplus", cCbc.second.fValue );
                        cCbc.second.fOvershoot = cOccupancy;
                    }
                    else
                        cCbc.second.fUndershoot = cOccupancy;
                }
                else
                {
                    if (cOccupancy < 0.50)
                    {
                        toggleRegBit ( cCbc.second, iBit );
                        fCbcInterface->WriteCbcReg ( cCbc.first, "Vplus", cCbc.second.fValue );
                        cCbc.second.fUndershoot = cOccupancy;
                    }
                    else
                        cCbc.second.fOvershoot = cOccupancy;
                }
            }

            // clear the occupancy histogram for the next bit
            clearOccupancyHists ( cCbc.first );
        }

    }


    //now need to loop all the structures and compare overshoot and undershoot
    for (auto& cCbc : fVplusMap)
    {
        //if final, don't run the check because we already satisfy the reuqirement of being epsilon close
        if (fHoleMode)
        {
            if (!cCbc.second.final() && fabs (0.5 - cCbc.second.fUndershoot) > fabs (cCbc.second.fOvershoot - 0.5) )
                if (cCbc.second.fValue < 255) cCbc.second.fValue += 1;
        }
        else
        {
            if (!cCbc.second.final() && fabs (0.5 - cCbc.second.fUndershoot) < fabs (cCbc.second.fOvershoot - 0.5) )
                if (cCbc.second.fValue < 255) cCbc.second.fValue += 1;
        }
    }

    if ( fCheckLoop )
    {
        measureOccupancy ( fEventsPerPoint, pTGroup );

        for ( auto& cCbc : fVplusMap )
        {
            float cOccupancy = findCbcOccupancy ( cCbc.first, pTGroup, fEventsPerPoint );
            std::cout << BOLDBLUE << "Found Occupancy of " << BOLDRED << cOccupancy << BOLDBLUE << " for CBC " << +cCbc.first->getCbcId() << " , test Group " << pTGroup << " using VPlus " << BOLDRED << +cCbc.second.fValue << BOLDBLUE << " (= 0x" << std::hex << +cCbc.second.fValue << std::dec << "; 0b" << std::bitset<8> ( cCbc.second.fValue ) << ")" << RESET << std::endl;
            clearOccupancyHists ( cCbc.first );
        }
    }
}

void Calibration::FindOffsets()
{
    // do a binary search for the correct offset value


    // just to be sure, configure the correct VCth and VPlus values
    CbcRegWriter cWriter ( fCbcInterface, "VCth", fTargetVcth );
    accept ( cWriter );
    // ok, done, all the offsets are at the starting value, VCth & Vplus are written

    // now loop over test groups
    for ( auto& cTGroup : fTestGroupChanMap )
    {
        std::cout << GREEN << "Tuning Test Group...." << cTGroup.first << RESET << std::endl;

        //enabling a test group is not explicitly necessary in the bitwise bisection algorithm
        bitwiseOffset ( cTGroup.first );

        if ( fCheckLoop )
        {
            clearTGOccupancy (cTGroup.first);
            // now all the bits are toggled or not, I still want to verify that the occupancy is ok
            int cMultiple = 3;
            std::cout << "Verifying Occupancy with final offsets by taking " << fEventsPerPoint* cMultiple << " Triggers!" << std::endl;
            measureOccupancy ( fEventsPerPoint  * cMultiple, cTGroup.first );
            // now find the occupancy for each channel and update the TProfile
            updateHists ( "Occupancy" );
        }

        //here if have to explicitly disable the test group again
        uint8_t cOffset = ( fHoleMode ) ? 0x00 : 0xFF;
        setOffset ( cOffset, cTGroup.first );
        std::cout << RED << "Disabling Test Group...." << cTGroup.first << RESET << std::endl << std::endl;
    }

    setRegValues();
}


void Calibration::bitwiseOffset ( int pTGroup )
{
    // loop over the bits
    for ( int iBit = 7; iBit >= 0; iBit-- )
    {
        std::cout << "Searching for the correct offsets by flipping bit " << iBit << std::endl;
        // now, for all the channels in the group and for each cbc, toggle the next bit of the offset from the map
        toggleOffset ( pTGroup, iBit, true );


        // now the offset for the current group is changed
        // now take data
        measureOccupancy ( fEventsPerPoint, pTGroup );


        // now call toggleOffset again with pBegin = false; this method checks the occupancy and flips a bit back if necessary
        toggleOffset ( pTGroup, iBit, false );
    }

    updateHists ( "Offsets" );
    updateHists ( "Occupancy" );
}


void Calibration::measureOccupancy ( uint32_t pNEvents, int pTGroup )
{
    for ( BeBoard* pBoard : fBoardVector )
    {

        uint32_t cN = 0;
        uint32_t cNthAcq = 0;

        fBeBoardInterface->ReadNEvents (pBoard, pNEvents);
        std::vector<Event*> events = fBeBoardInterface->GetEvents ( pBoard );

        // if this is for channelwise offset tuning, iterate the events and fill the occupancy histogram
        for ( auto& cEvent : events )
        {
            for ( auto cFe : pBoard->fModuleVector )
            {
                for ( auto cCbc : cFe->fCbcVector )
                    fillOccupancyHist ( cCbc, pTGroup, cEvent );
            }

            cN++;
        }

        cNthAcq++;
    }
}


float Calibration::findCbcOccupancy ( Cbc* pCbc, int pTGroup, int pEventsPerPoint )
{
    TH1F* cOccHist = static_cast<TH1F*> ( getHist ( pCbc, "Occupancy" ) );
    float cOccupancy = cOccHist->GetEntries();
    // return the hitcount divided by the the number of channels and events
    return cOccupancy / ( static_cast<float> ( fTestGroupChanMap[pTGroup].size() * pEventsPerPoint ) );
}

void Calibration::fillOccupancyHist ( Cbc* pCbc, int pTGroup, const Event* pEvent )
{
    // Find the Occupancy histogram for the current Cbc
    TH1F* cOccHist = static_cast<TH1F*> ( getHist ( pCbc, "Occupancy" ) );
    // iterate the channels in current group
    //int cHits = 0;

    for ( auto& cChanId : fTestGroupChanMap[pTGroup] )
    {
        // I am filling the occupancy profile for each CBC for the current test group
        // since the value in the testgroupchannel map is a struct (RegPair), I need to explicitly use fValue
        // ///////////////////////////////////////////////////////////////////////////////////////////
        // ///////////////////////////////////////////////////////////////////////////////////////////
        if ( pEvent->DataBit ( pCbc->getFeId(), pCbc->getCbcId(), cChanId.fValue ) && !cChanId.final() )
        {
            cOccHist->Fill ( cChanId.fValue );
            //cHits++;
        }
    }
}

void Calibration::clearOccupancyHists ( Cbc* pCbc )
{
    TH1F* cOccHist = static_cast<TH1F*> ( getHist ( pCbc, "Occupancy" ) );
    cOccHist->Reset ( "ICESM" );
}

void Calibration::clearTGOccupancy (uint8_t pGroup)
{
    for ( auto cBoard : fBoardVector )
    {
        for ( auto cFe : cBoard->fModuleVector )
        {
            uint32_t cFeId = cFe->getFeId();

            for ( auto cCbc : cFe->fCbcVector )
            {
                uint32_t cCbcId = cCbc->getCbcId();

                // find the TProfile for occupancy measurment of current channel
                TH1F* cOccHist = static_cast<TH1F*> ( getHist ( cCbc, "Occupancy" ) );

                // loop the channels of the current group and toggle bit i in the global map
                for ( auto& cChannel : fTestGroupChanMap[pGroup] )
                {
                    int iBin = cOccHist->GetXaxis()->FindBin ( cChannel.fValue );
                    cOccHist->SetBinContent ( iBin, 0 );
                    //set fFinal to false again so that the histogram will actually be filled
                    cChannel.fFinal = false;
                }
            }
        }
    }
}

void Calibration::clearVPlusMap()
{
    for ( auto cBoard : fBoardVector )
    {
        for ( auto cFe : cBoard->fModuleVector )
        {
            for ( auto cCbc : cFe->fCbcVector )
                fVplusMap[cCbc] = RegPair (0, false);
        }
    }
}

void Calibration::setOffset ( uint8_t pOffset, int  pGroup, bool pVPlus )
{
    // std::cout << "Setting offsets of Test Group " << pGroup << " to 0x" << std::hex << +pOffset << std::dec << std::endl;
    for ( auto cBoard : fBoardVector )
    {
        for ( auto cFe : cBoard->fModuleVector )
        {
            uint32_t cFeId = cFe->getFeId();

            for ( auto cCbc : cFe->fCbcVector )
            {
                uint32_t cCbcId = cCbc->getCbcId();

                // first, find the offset Histogram for this CBC
                TH1F* cOffsetHist = static_cast<TH1F*> ( getHist ( cCbc, "Offsets" ) );

                RegisterVector cRegVec;   // vector of pairs for the write operation

                // loop the channels of the current group and write the offset
                // the value of the map is a RegPair
                for ( auto& cChannel : fTestGroupChanMap[pGroup] )
                {
                    TString cRegName = Form ( "Channel%03d", cChannel.fValue + 1 );
                    cRegVec.push_back ( {cRegName.Data(), pOffset} );

                    //only if this is a VPlus tuning, use the offset histogram in this global enable, disable step
                    if ( pVPlus ) cOffsetHist->SetBinContent ( cChannel.fValue, pOffset );
                }

                fCbcInterface->WriteCbcMultReg ( cCbc, cRegVec );
            }
        }
    }
}

void Calibration::toggleOffset ( uint8_t pGroup, uint8_t pBit, bool pBegin )
{
    for ( auto cBoard : fBoardVector )
    {
        for ( auto cFe : cBoard->fModuleVector )
        {
            uint32_t cFeId = cFe->getFeId();

            for ( auto cCbc : cFe->fCbcVector )
            {
                uint32_t cCbcId = cCbc->getCbcId();

                // first, find the offset Histogram for this CBC
                TH1F* cOffsetHist = static_cast<TH1F*> ( getHist ( cCbc, "Offsets" ) );
                // find the TProfile for occupancy measurment of current channel
                TH1F* cOccHist = static_cast<TH1F*> ( getHist ( cCbc, "Occupancy" ) );
                // cOccHist->Scale( 1 / double( fEventsPerPoint ) );
                RegisterVector cRegVec;   // vector of pairs for the write operation

                // loop the channels of the current group and toggle bit i in the global map
                for ( auto& cChannel : fTestGroupChanMap[pGroup] )
                {
                    TString cRegName = Form ( "Channel%03d", cChannel.fValue + 1 );

                    if (!cChannel.final() )
                    {
                        if ( pBegin )
                        {
                            //this is the part that flips any new bit before the occupancy is measured

                            // get the offset
                            uint8_t cOffset = cOffsetHist->GetBinContent ( cChannel.fValue );

                            // toggle Bit i
                            toggleRegBit ( cOffset, pBit );

                            // modify the histogram
                            cOffsetHist->SetBinContent ( cChannel.fValue, cOffset );

                            // push in a vector for CBC write transaction
                            cRegVec.push_back ( {cRegName.Data(), cOffset} );
                        }
                        else  //here it is interesting since now I will check if the occupancy is smaller or larger 50% and decide wether to toggle or not to toggle
                        {
                            //this is the part the checks the occupancy and flips bit i back if necessary

                            // if the occupancy is larger than 50%, flip the bit back, if it is smaller, don't do anything
                            // get the offset
                            uint8_t cOffset = cOffsetHist->GetBinContent ( cChannel.fValue );
                            // get the occupancy
                            int iBin = cOccHist->GetXaxis()->FindBin ( cChannel.fValue );
                            float cOccupancy = cOccHist->GetBinContent ( iBin );

                            // only if the occupancy is too high I need to flip the bit back and write, if not, I can leave it
                            if (fabs (cOccupancy - 0.5 * fEventsPerPoint) < fEpsilon * fEventsPerPoint)
                            {
                                //I've found an offset that meets the requirement
                                cChannel.fFinal = true;
                                //cOffsetHist->SetBinContent(cChannel.fValue, cOffset);
                                //std::cout << BOLDGREEN << "Found 50% at bit " << +pBit << " and offset 0x" << std::hex << +cOffset << RESET << std::endl;
                                continue;
                            }
                            else if (cOccupancy > 0.5 * fEventsPerPoint)
                            {
                                toggleRegBit ( cOffset, pBit ); // toggle the bit back that was previously flipped
                                cOffsetHist->SetBinContent ( cChannel.fValue, cOffset );
                                cRegVec.push_back ( {cRegName.Data(), cOffset} );
                            }

                            // since I extracted the info from the occupancy profile for this bit (this iteration), i need to clear the corresponding bins
                            cOccHist->SetBinContent ( iBin, 0 );
                        }
                    }
                }

                fCbcInterface->WriteCbcMultReg ( cCbc, cRegVec );
            }
        }
    }
}

void Calibration::updateHists ( std::string pHistname )
{
    // loop the CBCs
    for ( const auto& cCbc : fCbcHistMap )
    {
        // loop the map of string vs TObject
        auto cHist = cCbc.second.find ( pHistname );

        if ( cHist != std::end ( cCbc.second ) )
        {
            // now cHist.second is the Histogram
            if ( pHistname == "Vplus" )
            {
                fVplusCanvas->cd ( cCbc.first->getCbcId() + 1 );
                TProfile* cTmpProfile = static_cast<TProfile*> ( cHist->second );
                cTmpProfile->DrawCopy ( "H P0 E" );
                fVplusCanvas->Update();
            }

            if ( pHistname == "Offsets" )
            {
                fOffsetCanvas->cd ( cCbc.first->getCbcId() + 1 );
                TH1F* cTmpHist = static_cast<TH1F*> ( cHist->second );
                cTmpHist->DrawCopy();
                fOffsetCanvas->Update();
            }

            if ( pHistname == "Occupancy" )
            {
                fOccupancyCanvas->cd ( cCbc.first->getCbcId() + 1 );
                TProfile* cTmpProfile = static_cast<TProfile*> ( cHist->second );
                cTmpProfile->DrawCopy();
                fOccupancyCanvas->Update();
            }
        }
        else std::cout << "Error, could not find Histogram with name " << pHistname << std::endl;
    }

#ifdef __HTTP__
    fHttpServer->ProcessRequests();
#endif
}

void Calibration::setRegValues()
{
    for ( auto cBoard : fBoardVector )
    {
        for ( auto cFe : cBoard->fModuleVector )
        {
            uint32_t cFeId = cFe->getFeId();

            for ( auto cCbc : cFe->fCbcVector )
            {
                uint32_t cCbcId = cCbc->getCbcId();

                // first, find the offset Histogram for this CBC
                TH1F* cOffsetHist = static_cast<TH1F*> ( getHist ( cCbc, "Offsets" ) );

                for ( int iChan = 0; iChan < NCHANNELS; iChan++ )
                {
                    uint8_t cOffset = cOffsetHist->GetBinContent ( iChan );
                    cCbc->setReg ( Form ( "Channel%03d", iChan + 1 ), cOffset );
                    std::cout << GREEN << "Offset for CBC " << cCbcId << " Channel " << iChan << " : 0x" << std::hex << +cOffset << std::dec << RESET << std::endl;
                }

            }
        }
    }
}

void Calibration::writeGraphs()
{
    fResultFile->cd();

    // Save hist maps for CBCs
    //
    //Tool::SaveResults();

    // save canvases too
    fVplusCanvas->Write ( fVplusCanvas->GetName(), TObject::kOverwrite );
    fOffsetCanvas->Write ( fOffsetCanvas->GetName(), TObject::kOverwrite );
    fOccupancyCanvas->Write ( fOccupancyCanvas->GetName(), TObject::kOverwrite );

}
