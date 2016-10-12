/*

        FileName :                    MPAGlibFWInterface.h
        Content :                     MPAGlibFWInterface init/config of the Glib and its Cbc's
        Programmer :                  Lorenzo BIDEGAIN, Nicolas PIERRE
        Version :                     1.0
        Date of creation :            28/07/14
        Support :                     mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

 */

#include <time.h>
#include <chrono>
#include <uhal/uhal.hpp>
#include "MPAGlibFWInterface.h"
#include "GlibFpgaConfig.h"
//
namespace Ph2_HwInterface
{

	MPAGlibFWInterface::MPAGlibFWInterface( const char* puHalConfigFileName, uint32_t pBoardId ) :
		BeBoardFWInterface( puHalConfigFileName, pBoardId ),
		fpgaConfig( nullptr ),
		fData( nullptr )
	{}


	MPAGlibFWInterface::MPAGlibFWInterface( const char* puHalConfigFileName, uint32_t pBoardId, FileHandler* pFileHandler ) :
		BeBoardFWInterface( puHalConfigFileName, pBoardId ),
		fpgaConfig( nullptr ),
		fData( nullptr ),

		fFileHandler( pFileHandler )
	{
		if ( fFileHandler == nullptr ) fSaveToFile = false;
		else fSaveToFile = true;
	}

	MPAGlibFWInterface::MPAGlibFWInterface( const char* pId, const char* pUri, const char* pAddressTable ) :
		BeBoardFWInterface( pId, pUri, pAddressTable ),
		fpgaConfig( nullptr ),
		fData( nullptr )
	{}


	MPAGlibFWInterface::MPAGlibFWInterface( const char* pId, const char* pUri, const char* pAddressTable, FileHandler* pFileHandler ) :
		BeBoardFWInterface( pId, pUri, pAddressTable ),
		fpgaConfig( nullptr ),
		fData( nullptr ),

		fFileHandler( pFileHandler )
	{
		if ( fFileHandler == nullptr ) fSaveToFile = false;
		else fSaveToFile = true;
	}


	void MPAGlibFWInterface::ConfigureBoard( const BeBoard* pBoard )
	{
	}


	void MPAGlibFWInterface::Start()
	{
		TestbeamInit(0, 0);
	}

	void MPAGlibFWInterface::Stop( uint32_t pNthAcq )
	{
		WriteReg( "Control.testbeam_mode", 0 );
		WriteReg( "Control.beam_on", 0 );
	}


	void MPAGlibFWInterface::Pause()
	{


	}


	void MPAGlibFWInterface::Resume()
	{

	}

	uint32_t MPAGlibFWInterface::ReadData( BeBoard* pBoard, unsigned int pNthAcq, bool pBreakTrigger )
	{

	}



	std::pair<std::vector<uint32_t>, std::vector<uint32_t>>  MPAGlibFWInterface::ReadData(int buffer_num, int mpa)
	  {
	    std::string targ;
	    targ = "Readout.Counter.MPA" + std::to_string(mpa);
	    targ = targ + ".buffer_" + std::to_string(buffer_num);
	    std::vector<uint32_t> counterdata =  ReadBlockRegValue( targ, 25 );

	    targ = "Readout.Memory.MPA" + std::to_string(mpa);
	    targ = targ + ".buffer_" + std::to_string(buffer_num);
	    std::vector<uint32_t> memorydata =  ReadBlockRegValue( targ, 216 );

	    if (counterdata[0]!=(0xFFFFFFF0 + mpa))
		std::cout<<"Warning, header mismatch"<<std::endl;


	    std::pair<std::vector<uint32_t>, std::vector<uint32_t>>  returndata(counterdata,memorydata);
	    return returndata;

	  }


	/** compute the block size according to the number of CBC's on this board
	 * this will have to change with a more generic FW */
	uint32_t MPAGlibFWInterface::computeBlockSize( BeBoard* pBoard )
	{

	}

	std::vector<uint32_t> MPAGlibFWInterface::ReadBlockRegValue( const std::string& pRegNode, const uint32_t& pBlocksize )
	{
		uhal::ValVector<uint32_t> valBlock = ReadBlockReg( pRegNode, pBlocksize );
		std::vector<uint32_t> vBlock = valBlock.value();

		// To avoid the IPBUS bug
		// need to convert uHal::ValVector to vector<uint32_t> so we can replace the 256th word
		if ( pBlocksize > 255 )
		{
			std::string fSram_256 = pRegNode + "_256";
			uhal::ValWord<uint32_t> cWord = ReadReg( fSram_256 );
			vBlock[255] = cWord.value();
		}
		return vBlock;
	}

	bool MPAGlibFWInterface::WriteBlockReg( const std::string& pRegNode, const std::vector< uint32_t >& pValues )
	{
		bool cWriteCorr = RegManager::WriteBlockReg( pRegNode, pValues );

		if ( pValues.size() > 255 )
			WriteReg( pRegNode + "_256", pValues[255] );
		return cWriteCorr;
	}

	void MPAGlibFWInterface::SelectDaqSRAM( uint32_t pNthAcq )
	{

	}



	//Methods for Cbc's:

	void MPAGlibFWInterface::StartThread( BeBoard* pBoard, uint32_t uNbAcq, HwInterfaceVisitor* visitor )
	{

	}

	void MPAGlibFWInterface::threadAcquisitionLoop( BeBoard* pBoard, HwInterfaceVisitor* visitor )
	{

	};

	bool MPAGlibFWInterface::I2cCmdAckWait( uint32_t pAckVal, uint8_t pNcount )
	{

	}

	void MPAGlibFWInterface::WriteI2C( std::vector<uint32_t>& pVecReq, bool pWrite )
	{

	}

	void MPAGlibFWInterface::ReadI2C( std::vector<uint32_t>& pVecReq )
	{

	}


	void MPAGlibFWInterface::WriteCbcBlockReg( uint8_t pFeId, std::vector<uint32_t>& pVecReq )
	{

	}

	void MPAGlibFWInterface::ReadCbcBlockReg( uint8_t pFeId, std::vector<uint32_t>& pVecReq )
	{

	}

	void MPAGlibFWInterface::FlashProm( const std::string& strConfig, const char* pstrFile )
	{

	}

	void MPAGlibFWInterface::JumpToFpgaConfig( const std::string& strConfig )
	{

	}



	//Methods for Mpa's:


	void MPAGlibFWInterface::PowerOn()
	{
		std::chrono::milliseconds cWait( 10 );

		WriteReg( "Utility.MPA_settings.VDDPST_enable", 1 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.DVDD_enable", 1 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.AVDD_enable", 1 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.VBIAS_enable", 1 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.DAC_register", 0x12BE );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.DAC_register", 0x10BE );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.DAC_register", 0x11BE );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.PVDD_enable", 1 );
		std::this_thread::sleep_for( cWait );
        	WriteReg("Control.MPA_clock_enable", 1);
		std::this_thread::sleep_for( cWait );
	}


	void MPAGlibFWInterface::PowerOff()
	{

		std::chrono::milliseconds cWait( 10 );
		WriteReg( "Utility.MPA_settings.PVDD_enable", 0 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.DAC_register", 0x1100 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.DAC_register", 0x1000 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.DAC_register", 0x1200 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.VBIAS_enable", 0 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.AVDD_enable", 0 );
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.DVDD_enable", 0);
		std::this_thread::sleep_for( cWait );
		WriteReg( "Utility.MPA_settings.VDDPST_enable", 0 );
		std::this_thread::sleep_for( cWait );

	}



	void MPAGlibFWInterface::ReadVer()
	{
	        std::cout<<"\nReading GLIB firmware version:";
		std::cout<<ReadReg( "Control.firm_ver" )<<std::endl;

	}










	int MPAGlibFWInterface::WaitSequencer()
	  {
	    int i=0;
	    
	    uhal::ValWord<uint32_t> busyseq;
	    std::chrono::milliseconds cWait( 1 );
	    busyseq = ReadReg("Control.Sequencer.busy");
	    while (busyseq == 1)
	      {
		busyseq = ReadReg("Control.Sequencer.busy");
		std::this_thread::sleep_for( cWait );
		i++;
		if (i > 100) {
		  std::cout<<"WaitSequence Timeout\n";
		  return 0;
		}
	      }
	    return 1;
	  }







	void MPAGlibFWInterface::ReadTrig(int buffer_num)
	  {
	    int total_trigs = -1;
	    int trigger_counter = -1;
	    int trigger_total_counter = -1;
	    int Offset_BEAM = -1;
	    int Offset_MPA = -1;
	    std::string targ;
	    std::cout<<"\nRead_Trig - for now just display to stdout\n";
	    total_trigs = ReadReg("Control.total_triggers");

	    targ = "Control.trigger_counter.buffer_" + std::to_string(buffer_num);  
	    trigger_counter = ReadReg(targ);

	    targ = "Control.trigger_counter.buffer_" + std::to_string(buffer_num);  
	    trigger_total_counter = ReadReg(targ);

	    targ = "Control.trigger_offset_BEAM.buffer_" + std::to_string(buffer_num); 
	    std::vector<uint32_t> rData =  ReadBlockRegValue( targ, 255 );

	    targ = "Control.trigger_offset_MPA.buffer_" + std::to_string(buffer_num);  
	    std::vector<uint32_t> rData2 =  ReadBlockRegValue( targ, 255 );

	    std::cout<<"   total_triggers = "<<total_trigs<<"\n";
	    std::cout<<"   trigger_counter = "<<trigger_counter<<"\n";
	    std::cout<<"   trigger_total_counter = "<<trigger_total_counter<<"\n";

	    std::cout<<"   Offset_BEAM (255)\n";
	    for (auto i: rData)
	      std::cout<<i<<' ';
	    std::cout<<"   (done)\n";

	    std::cout<<"   Offset_MPA (255)\n";
	    for (auto i: rData2)
	      std::cout<<i<<' ';
	    std::cout<<"   (done)\n";
	  }


	void MPAGlibFWInterface::HeaderInitMPA(int nmpa)
	  {
	    WriteReg( "Readout.Header.MPA"+std::to_string(nmpa), 0xFFFFFFF0 + nmpa );
	  }

	#define CLOCKGLIB (0)
	#define CLOCKTESTBEAM (1)
	void MPAGlibFWInterface::TestbeamInit(int clock, int phase)
	  {
	    /* int clkset = 0;
	    switch (clock) {
	    case CLOCKGLIB:
	      clkset = 0;
	      break;
	    case CLOCKTESTBEAM:
	      clkset = 1;
	      break;
	    default:
	      clkset = 0;
	      break;
	    }
	    */
	    WriteReg("Control.testbeam_clock", clock);
	    WriteReg("Control.testbeam_mode", 1);
	    
	    WriteReg("Control.beam_on", 1);
	    WriteReg("Control.shutter_delay", phase);
	  }















	void MPAGlibFWInterface::StrobeSettings(int snum, int sdel, int slen, 
					       int sdist, int cal)
	  {
	    WriteReg("Shutter.Strobe.number", snum);
	    WriteReg("Shutter.Strobe.delay", sdel);
	    WriteReg("Shutter.Strobe.length", slen);
	    WriteReg("Shutter.Strobe.distance", sdist);
	    
	    WriteReg("Control.calibration", cal);
	  }


	void MPAGlibFWInterface::SequencerInit(int smode,int sdur,int mem,int ibuff)
	  {
	    WriteReg("Shutter.time", sdur);
	    WriteReg("Control.testbeam_mode", 0x0);
	    WriteReg("Control.readout", mem);
	    WriteReg("Control.Sequencer.datataking_continuous", smode);
	    WriteReg("Control.Sequencer.buffers_index", ibuff);
	  }


	void MPAGlibFWInterface::upload( std::vector< uint32_t > *conf_upload, int nmpa)
	  {
	    WriteBlockReg( "Configuration.Memory_DataConf.MPA"+std::to_string(nmpa)+".config_1", (*conf_upload));
	    WriteReg( "Configuration.mode",0x5);
	  }












}
