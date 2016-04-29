/*

    FileName :                     Data.cc
    Content :                      Data handling from DAQ
    Programmer :                   Nicolas PIERRE
    Version :                      1.0
    Date of creation :             10/07/14
    Support :                      mail to : nicolas.pierre@icloud.com

 */

#include "../Utils/Data.h"
#include <iostream>

namespace Ph2_HwInterface {
    //Data Class

    // copy constructor
    Data::Data ( const Data& pD ) :

        // Initialise( pD.fNevents );
        fNevents ( pD.fNevents ),
        fCurrentEvent ( pD.fCurrentEvent ),
        fNCbc ( pD.fNCbc ),
        fEventSize ( pD.fEventSize )
    {
    }


    void Data::Set ( const BeBoard* pBoard, const std::vector<uint32_t>& pData, uint32_t pNevents, bool swapBits )
    {
        Reset();

        fNevents = static_cast<uint32_t> ( pNevents );
        fEventSize = static_cast<uint32_t> ( (pData.size() ) / fNevents );
        fNCbc = ( fEventSize - ( EVENT_HEADER_TDC_SIZE_32 ) ) / ( CBC_EVENT_SIZE_32 );

        //use a SwapIndex to decide wether to swap a word or not
        //use a WordIndex to pick events apart
        uint32_t cWordIndex = 0;
        uint32_t cSwapIndex = 0;

        // to Fill fEventList
        std::vector<uint32_t> lvec;

        for ( auto word : pData )
        {
            //if the SwapIndex is greater than 0 and a multiple of the event size in 32 bit words, reset SwapIndex to 0
            if (cSwapIndex > 0 && cSwapIndex % fEventSize == 0) cSwapIndex = 0;

            if (swapBits && is_channel_data (cSwapIndex, fNCbc) ) word = reverse_bits (word);

#ifdef __CBCDAQ_DEV__
             std::cout << std::setw(3) <<  cWordIndex << " ### " << std::bitset<32>(pData.at(cWordIndex)) << std::endl;
             if((cWordIndex+1)%fEventSize == 0 && cWordIndex >0 ) std::cout << std::endl << std::endl;
#endif

            lvec.push_back ( pData.at(cWordIndex));

            if ( cWordIndex > 0 &&  (cWordIndex+1) % fEventSize == 0 )
            { 
                std::cout << "I Happen" << std::endl;
                fEventList.push_back ( new Event ( pBoard, fNCbc, lvec ) );
                lvec.clear();
            }
            cWordIndex++;
            cSwapIndex++;
        }

#ifdef __CBCDAQ_DEV__
        std::cout << "Initializing list with " << pData.size() << " 32 bit words
                  << " containing data from "
                  << fNevents << "  Events with an eventbuffer size of " << fEventSize << " and " << fNCbc
                  << " CBCs each! " << EVENT_HEADER_TDC_SIZE_32 << " " << CBC_EVENT_SIZE_32 << std::endl;
#endif
    }

    void Data::Reset()
    {
        for ( auto& pevt : fEventList )
            delete pevt;

        fEventList.clear();
        fCurrentEvent = 0;
    }
}
