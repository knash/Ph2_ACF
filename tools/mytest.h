
/*!

        \file                  mytest.h
        \brief                 class for testing a custom calibration routing for the Ph2 Workshop
        \author              Andrea Massetti & Ali Imran
        \version                1.0
        \date                   20/01/15
        Support :               mail to : georg.auzinger@cern.ch

 */

#ifndef MYTEST_H__
#define MYTEST_H__

#include "Tool.h"
//#include "Channel.h"
#include "../Utils/Visitor.h"
#include "../Utils/Utilities.h"
#include "../Utils/CommonVisitors.h"
#include "TFile.h"
#include "TString.h"
//#include "TMultiGraph.h"
//#include "TGraph.h"
//#include "TCanvas.h"
#include "TH1F.h"
//#include "TF1.h"
//#include "TH2F.h"
#include <math.h>

using namespace Ph2_System;
using namespace Ph2_HwInterface;
using namespace Ph2_HwDescription;

/*!
 * \class mytest
 * \brief Class to reconstruct pulse shape
 */
//typedef std::map<Cbc*, std::vector<Channel*> > ChannelMap;

class mytest : public Tool
{
  private:
    bool fHoleMode;
    uint8_t fTPAmplitude;
    TCanvas* fCanvas;
  public:
    /*!
    * \Initialize the Canvas
    */
    void Initialise();

    /*!
    * \scan the Vcth
    */
    TH1F* ScanVcth (uint32_t pNEvents, uint32_t pChannel);

    /*!
     * \brief: Save results, if Histograms are mapped vs CBCs & the provided maps from Tool are used, this is not necessary
     */
    void SaveResults();

    // The following section is provided and not necessarily useful for the exercise!
  private:

    void setTestPulse (uint8_t pTestGroup, uint8_t pTPAmplitude)
    {
        if ( fTPAmplitude != 0x00 && fTPAmplitude != 0xFF )
        {
            BeBoardRegWriter cBeBoardWriter ( fBeBoardInterface, "COMMISSIONNING_MODE_RQ", 1 );
            this->accept ( cBeBoardWriter );
            cBeBoardWriter.setRegister ( "COMMISSIONNING_MODE_CBC_TEST_PULSE_VALID", 1 );
            this->accept ( cBeBoardWriter );

            // now select the channel group we want to look at
            std::vector<std::pair<std::string, uint8_t>> cRegVec;
            uint8_t cRegValue =  to_reg ( 0, pTestGroup );

            cRegVec.push_back ( std::make_pair ( "SelTestPulseDel&ChanGroup",  cRegValue ) );

            //set the value of test pulsepot registrer and MiscTestPulseCtrl&AnalogMux register
            if ( fHoleMode )
                cRegVec.push_back ( std::make_pair ( "MiscTestPulseCtrl&AnalogMux", 0xD1 ) );
            else
                cRegVec.push_back ( std::make_pair ( "MiscTestPulseCtrl&AnalogMux", 0x61 ) );

            cRegVec.push_back ( std::make_pair ( "TestPulsePot", pTPAmplitude ) );
            // cRegVec.push_back( std::make_pair( "Vplus",  fVplus ) );
            CbcMultiRegWriter cWriter ( fCbcInterface, cRegVec );
            this->accept ( cWriter );
        }
    }
    
    uint8_t reverse ( uint8_t n )
    {
        // Reverse the top and bottom nibble then swap them.
        return ( fLookup[n & 0b1111] << 4 ) | fLookup[n >> 4];
    }

    /*!
    * \brief reverse the endianess before writing in to the register
    * \param pDelay: the actual delay
    * \param pGroup: the actual group number
    * \return the reversed endianness
    */
    uint8_t to_reg ( uint8_t pDelay, uint8_t pGroup )
    {

        uint8_t cValue = ( ( reverse ( pDelay ) ) & 0xF8 ) |
                         ( ( reverse ( pGroup ) ) >> 5 );

        //std::cout << std::bitset<8>( cValue ) << " cGroup " << +pGroup << " " << std::bitset<8>( pGroup ) << " pDelay " << +pDelay << " " << std::bitset<8>( pDelay ) << std::endl;
        return cValue;
    }

    unsigned char fLookup[16] =
    {
        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,
    }; /*!< Lookup table for reverse the endianness */
};


#endif
