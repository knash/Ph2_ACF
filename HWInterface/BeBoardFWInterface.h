


/*!
        \file                BeBoardFWInterface.h
        \brief                           BeBoardFWInterface base class of all type of boards
        \author                          Lorenzo BIDEGAIN, Nicolas PIERRE
        \version             1.0
        \date                            28/07/14
        Support :                        mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

 */

#ifndef __BEBOARDFWINTERFACE_H__
#define __BEBOARDFWINTERFACE_H__

#include <boost/thread.hpp>
#include <uhal/uhal.hpp>
#include "RegManager.h"
#include "../Utils/Event.h"
#include "../Utils/FileHandler.h"
#include "../Utils/Data.h"
#include "../Utils/Utilities.h"
#include "../Utils/Exception.h"
#include "../Utils/FileHandler.h"
#include "../HWDescription/BeBoard.h"
#include "../HWDescription/Definition.h"
#include "../HWDescription/MPARegItem.h"
#include "../HWDescription/MPA.h"

#include "../HWDescription/CbcRegItem.h"
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include <iostream>
#include <fstream>

using namespace Ph2_HwDescription;

/*!
 * \namespace Ph2_HwInterface
 * \brief Namespace regrouping all the interfaces to the hardware
 */
class FileHandler;

namespace Ph2_HwInterface
{
class FpgaConfig;

/*!
 * \class BeBoardFWInterface
 * \brief Class separating board system FW interface from uHal wrapper
 */
class BeBoardFWInterface : public RegManager
{

public:
    unsigned int fNTotalAcq;

    FileHandler* fFileHandler;
    bool fSaveToFile;

    static const uint32_t cMask1 = 0xff;
    static const uint32_t cMask2 = 0xff00;
    static const uint32_t cMask3 = 0xff0000;
    static const uint32_t cMask4 = 0xff000000;
    static const uint32_t cMask5 = 0x1e0000;
    static const uint32_t cMask6 = 0x10000;
    static const uint32_t cMask7 = 0x200000;
public:

    /*!
    * \brief Constructor of the BeBoardFWInterface class
    * \param puHalConfigFileName : path of the uHal Config File
    * \param pFileHandler : pointer to file handler for saving Raw Data*/
    BeBoardFWInterface( const char* puHalConfigFileName, uint32_t pBoardId );
    BeBoardFWInterface( const char* pId, const char* pUri, const char* pAddressTable );
    /*!
    * \brief Destructor of the BeBoardFWInterface class
    */
    virtual ~BeBoardFWInterface() {}
    /*!
    * \brief Get the board type
    */
    virtual std::string getBoardType();
    /*!
    * \brief Get the board infos
    */
    virtual void getBoardInfo();

    /*! \brief Upload a configuration in a board FPGA */
    virtual void FlashProm( const std::string& strConfig, const char* pstrFile ) {}
    /*! \brief Jump to an FPGA configuration */
    virtual void JumpToFpgaConfig( const std::string& strConfig ) {}

    virtual void DownloadFpgaConfig( const std::string& strConfig, const std::string& strDest ) {}
    /*! \brief Current FPGA configuration*/
    virtual const FpgaConfig* getConfiguringFpga()
    {
        return nullptr;
    }
    virtual void ProgramCdce() {}
    /*! \brief Get the list of available FPGA configuration (or firmware images)*/
    virtual std::vector<std::string> getFpgaConfigList( )
    {
        return std::vector<std::string>();
    }
    /*! \brief Delete one Fpga configuration (or firmware image)*/
    virtual void DeleteFpgaConfig( const std::string& strId ) {}

    //Encode/Decode Cbc values
    /*!
    * \brief Encode a/several word(s) readable for a Cbc
    * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to write
    * \param pCbcId : Id of the Cbc to work with
    * \param pVecReq : Vector to stack the encoded words
    */
    virtual void EncodeReg( const CbcRegItem& pRegItem, uint8_t pCbcId, std::vector<uint32_t>& pVecReq ); /*!< Encode a/several word(s) readable for a Cbc*/
    /*!
    * \brief Encode a/several word(s) readable for a Cbc
    * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to write
    * \param pCbcId : Id of the Cbc to work with
    * \param pVecReq : Vector to stack the encoded words
    */
    virtual void EncodeReg( const CbcRegItem& pRegItem, uint8_t pFeId, uint8_t pCbcId, std::vector<uint32_t>& pVecReq ); /*!< Encode a/several word(s) readable for a Cbc*/
    /*!
    * \brief Decode a word from a read of a register of the Cbc
    * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to read
    * \param pCbcId : Id of the Cbc to work with
    * \param pWord : variable to put the decoded word
    */
    virtual void DecodeReg( CbcRegItem& pRegItem, uint8_t pCbcId, uint32_t pWord ); /*!< Decode a word from a read of a register of the Cbc*/


    //virtual pure methods which are defined in the proper BoardFWInterface class
    //r/w the Cbc registers
    /*!
    * \brief Write register blocks of a Cbc
    * \param pFeId : FrontEnd to work with
    * \param pVecReq : Block of words to write
    */
    virtual void WriteCbcBlockReg( uint8_t pFeId, std::vector<uint32_t>& pVecReq ) = 0;
    /*!
    * \brief Read register blocks of a Cbc
    * \param pFeId : FrontEnd to work with
    * \param pVecReq : Vector to stack the read words
    */
    virtual void ReadCbcBlockReg( uint8_t pFeId, std::vector<uint32_t>& pVecReq ) = 0;
    /*!
    * \brief Configure the board with its Config File
    * \param pBoard
    */

    virtual void WriteMPABlockReg( uint8_t pFeId, std::vector<uint32_t>& pVecReq ) = 0;
    /*!
    * \brief Read register blocks of a MPA
    * \param pFeId : FrontEnd to work with
    * \param pVecReq : Vector to stack the read words
    */
    virtual void ReadMPABlockReg( uint8_t pFeId, std::vector<uint32_t>& pVecReq ) = 0;
    /*!
    * \brief Configure the board with its Config File
    * \param pBoard
    */

    virtual void ConfigureBoard( const BeBoard* pBoard ) = 0;

    virtual void PowerOn() = 0;

    virtual void ReadVer() = 0;

    virtual void TestbeamInit(int clock, int phase) = 0;   /* j */
    virtual void StrobeSettings(int snum, int sdel, int slen, int sdist,
				int cal) = 0;   /* j */
    virtual std::pair<std::vector<uint32_t>, std::vector<uint32_t>>  ReadData(int buffernum, int mpa) = 0;   /* j */
    virtual std::pair<std::vector<uint32_t>, std::vector<std::string>> FormatData(std::pair<std::vector<uint32_t>, std::vector<uint32_t>> data)= 0;
    virtual std::pair<std::vector<uint32_t>, std::vector<std::vector<uint64_t>>> ReadMemory(std::vector<std::string> intmemory, int mode) = 0;
    virtual void SequencerInit(int smode,int sdur,int mem,int ibuff) = 0;   /* j */
    virtual void upload(const std::string& pFilename, int nmpa, int conf) = 0;
    virtual void HeaderInit() = 0;   /* j */
    virtual void HeaderInitMPA(const int nmpa) = 0;   /* j */
    virtual void ReadTrig(const int buffer_num) = 0;   /* j */
    virtual void VDDPSTon() = 0;   /* j */
    virtual void VDDPSToff() = 0;   /* j */
    virtual void DVDDon() = 0;   /* j */
    virtual void DVDDoff() = 0;   /* j */
    virtual void AVDDon() = 0;   /* j */
    virtual void PVDDon() = 0;   /* j */
    virtual void PVDDoff() = 0;   /* j */
    virtual void VBIASon() = 0;   /* j */
    virtual void VBIASoff() = 0;   /* j */


    virtual int WaitSequencer() = 0;   /* j */
    

    /*!
     * \brief Start an acquisition in a separate thread
     * \param pBoard Board running the acquisition
     * \param uNbAcq Number of acquisition iterations (each iteration will get CBC_DATA_PACKET_NUMBER + 1 events)
     * \param visitor override the visit() method of this object to process each event
     */
    virtual void StartThread( BeBoard* pBoard, uint32_t uNbAcq, HwInterfaceVisitor* visitor ) = 0;
    /*! \brief Stop a running parallel acquisition
     */
    virtual void StopThread();
    /*! \brief Get the parallel acquisition iteration number */
    int getNumAcqThread();
    /*! \brief Is a parallel acquisition running ? */
    bool isRunningThread() const
    {
        return runningAcquisition;
    }
    /*!
     * \brief Start a DAQ
     */
    virtual void Start() = 0;
    /*!
     * \brief Stop a DAQ
     * \param pNthAcq : actual number of acquisitions
     */
    virtual void Stop( uint32_t pNthAcq ) = 0;
    /*!
     * \brief Pause a DAQ
     */
    virtual void Pause() = 0;
    /*!
     * \brief Resume a DAQ
     */
    virtual void Resume() = 0;
    /*!
     * \brief Read data from DAQ
     * \param pBoard
     * \param pNthAcq : actual number of acquisitions
     * \param pBreakTrigger : if true, enable the break trigger
     * \return cNPackets: the number of packets read
     */
    virtual uint32_t ReadData( BeBoard* pBoard, uint32_t pNthAcq, bool pBreakTrigger ) = 0;
    /*!
     * \brief Get next event from data buffer
     * \return Next event
     */
    virtual const Event* GetNextEvent( const BeBoard* pBoard ) const = 0;
    virtual const Event* GetEvent( const BeBoard* pBoard, int i ) const = 0;
    virtual const std::vector<Event*>& GetEvents( const BeBoard* pBoard ) const = 0;

    virtual std::vector<uint32_t> ReadBlockRegValue( const std::string& pRegNode, const uint32_t& pBlocksize ) = 0;

protected:

    bool runningAcquisition;
    uint32_t cBlockSize, cNPackets, numAcq, nbMaxAcq;
    boost::thread thrAcq;

};
}

#endif
BeBoardFWInterface.h
Open with
Displaying BeBoardFWInterface.h.
