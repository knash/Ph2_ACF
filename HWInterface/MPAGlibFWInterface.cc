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

	}

	void MPAGlibFWInterface::Stop( uint32_t pNthAcq )
	{

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

	
	    /*

	    std::cout<<"   Counter data (25)\n";

	    for (auto i: counts)
			{
		    	std::bitset<32> y(i);
		    	std::cout<<y<<' ';
			}
	    std::cout<<"   (done)\n";

	    std::cout<<"   Memory data (216)\n";
	    for (auto i: mems)
			{
		    	std::bitset<32> y(i);
		    	std::cout<<y<<' ';
			}
	
	    std::cout<<"   (done)\n";
	    */
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
	}


	void MPAGlibFWInterface::ReadVer()
	{
	        std::cout<<"\nReading GLIB firmware version:";
		std::cout<<ReadReg( "Control.firm_ver" )<<std::endl;
        	WriteReg("Control.MPA_clock_enable", 1);

		//	        std::cout<<"\nNow reading GLIB memory testing addresing:";
		//		std::cout<<ReadReg( "Readout.Counter.MPA1" )<<std::endl;
	}










	int MPAGlibFWInterface::WaitSequencer()
	  {
	    int i=0;
	    
	    uhal::ValWord<uint32_t> busyseq;
	    std::chrono::milliseconds cWait( 1 );
	    busyseq = fBoardFW->ReadReg("Control.Sequencer.busy");
	    while (busyseq == 1)
	      {
		busyseq = fBoardFW->ReadReg("Control.Sequencer.busy");
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
	    total_trigs = fBoardFW->ReadReg("Control.total_triggers");

	    targ = "Control.trigger_counter.buffer_" + std::to_string(buffer_num);  
	    trigger_counter = fBoardFW->ReadReg(targ);

	    targ = "Control.trigger_counter.buffer_" + std::to_string(buffer_num);  
	    trigger_total_counter = fBoardFW->ReadReg(targ);

	    targ = "Control.trigger_offset_BEAM.buffer_" + std::to_string(buffer_num); 
	    std::vector<uint32_t> rData =  fBoardFW->ReadBlockRegValue( targ, 255 );

	    targ = "Control.trigger_offset_MPA.buffer_" + std::to_string(buffer_num);  
	    std::vector<uint32_t> rData2 =  fBoardFW->ReadBlockRegValue( targ, 255 );

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

	void MPAGlibFWInterface::HeaderInit()
	  {
	    std::cout<<"\nWriting MPA1-6\n";
	    for (int i=1;i<=6; i++) {
	      HeaderInitMPA(i);
	    }
	  }
	    

	void MPAGlibFWInterface::HeaderInitMPA(int nmpa)
	  {
	    switch (nmpa) {
	    case 1:
	      fBoardFW->WriteReg( "Readout.Header.MPA1", 0xFFFFFFFF1 );
	      break;
	    case 2:
	      fBoardFW->WriteReg( "Readout.Header.MPA2", 0xFFFFFFFF2 );
	      break;
	    case 3:
	      fBoardFW->WriteReg( "Readout.Header.MPA3", 0xFFFFFFFF3 );
	      break;
	    case 4:
	      fBoardFW->WriteReg( "Readout.Header.MPA4", 0xFFFFFFFF4 );
	      break;
	    case 5:
	      fBoardFW->WriteReg( "Readout.Header.MPA5", 0xFFFFFFFF5 );
	      break;
	    case 6:
	      fBoardFW->WriteReg( "Readout.Header.MPA6", 0xFFFFFFFF6 );
	      break;
	    default:
	      std::cout<<"\nError in MPA selection!";
	      break;
	    }

	  }

	#define CLOCKGLIB (0)
	#define CLOCKTESTBEAM (1)
	void MPAGlibFWInterface::TestbeamInit(int clock, int phase)
	  {
	    std::cout<<"\nTestbeamInit\n";
	    int clkset = 0;
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
	    fBoardFW->WriteReg("Control.testbeam_clock", clkset);
	    fBoardFW->WriteReg("Control.testbeam_mode", 1);
	    
	    fBoardFW->WriteReg("Control.beam_on", 1);
	    fBoardFW->WriteReg("Control.shutter_delay", phase);
	  }










	std::pair<std::vector<uint32_t>, std::vector<std::string>> MPAGlibFWInterface::FormatData(std::pair<std::vector<uint32_t>, std::vector<uint32_t>> data)
	  {
		std::vector<uint32_t> counter_data = data.first;
		std::vector<uint32_t> memory_data = data.second;
		std::vector<uint32_t> bitpix(50);
		std::vector<std::string> mem(96);

	    	for (int x=0;x<25; x++) 
			{
				
				int shift1 = 0;
				int shift2 = 16;

				std::bitset<32> a(counter_data[x]);
				uint32_t l((counter_data[x] >> shift1) & 0xffff);
				uint32_t r((counter_data[x] >> shift2) & 0xffff);
				bitpix[2*x] = l;
				bitpix[2*x+1] = r;
	
			}

		std::bitset<32> y(memory_data[216]);
		std::string memory_string = y.to_string();
		for (int x=1;x<216; x++)
			{
		    		std::bitset<32> y(memory_data[215 - x]);
				memory_string = memory_string + y.to_string();
			}
		std::cout<<"parsing string"<<std::endl;
		for (int x=0;x<96; x++)
			{
				mem[x] = memory_string.substr(x*72, 72);

			}
	    	std::pair<std::vector<uint32_t>, std::vector<std::string>>  returndata(bitpix,mem);
		return 	returndata;



	  }






	std::pair<std::vector<uint32_t>, std::vector<std::vector<uint64_t>>> MPAGlibFWInterface::ReadMemory(std::vector<std::string> intmemory, int mode)

			{
			std::string memory; 
			std::vector<uint32_t> BX(96);
			std::vector<uint64_t> hit(96);
			std::vector<int> row(96);
			std::vector<int> col(96);
			std::vector<std::vector<uint64_t>> data(96);
			std::vector<int> bend(96);
			std::string header; 
			/*
			if (mode == 1)
				{
				for (int x=0;x<96; x++)
					{

		    			std::bitset<32> y(intmemory[x]);

					memory = y.to_string();
					header = memory.substr(0, 8) ;
					if (header == "00000000") break;

					BX[x] = std::stoi(memory.substr(0, 16));
				
					nrow = {std::stoi(memory.substr(16, 5)), std::stoi(memory.substr(23, 5)), std::stoi(memory.substr(30, 5)), std::stoi(memory.substr(37, 5)), std::stoi(memory.substr(44, 5)), std::stoi(memory.substr(51, 5)), std::stoi(memory.substr(58, 5)), std::stoi(memory.substr(65, 5))};
					ncol = {std::stoi(memory.substr(21, 2)), std::stoi(memory.substr(28, 2)), std::stoi(memory.substr(35, 2)), std::stoi(memory.substr(42, 2)), std::stoi(memory.substr(49, 2)), std::stoi(memory.substr(56, 2)), std::stoi(memory.substr(63, 2)), std::stoi(memory.substr(70, 2))};
					}
	    			data[x].push_back(row);
	    			data[x].push_back(col);
				}
		



			if (mode == 2)
				{
				for (int x=0;x<96; x++)
					{
		    			std::bitset<32> y(intmemory[x]);

					memory = y.to_string();
					if (memory.substr(0, 8) == '00000000') break;

					BX[x] = std::stoi(memory.substr(0, 16));
				
					nrow = {std::stoi(memory.substr(16, 5)), std::stoi(memory.substr(23, 5)), std::stoi(memory.substr(30, 5)), std::stoi(memory.substr(37, 5)), std::stoi(memory.substr(44, 5)), std::stoi(memory.substr(51, 5)), std::stoi(memory.substr(58, 5)), std::stoi(memory.substr(65, 5))};
					ncol = {std::stoi(memory.substr(21, 2)), std::stoi(memory.substr(28, 2)), std::stoi(memory.substr(35, 2)), std::stoi(memory.substr(42, 2)), std::stoi(memory.substr(49, 2)), std::stoi(memory.substr(56, 2)), std::stoi(memory.substr(63, 2)), std::stoi(memory.substr(70, 2))};
					}
	    			data[x].push_back(row);
	    			data[x].push_back(col);
				}

		

			if (mode == 0)
				{
				for (int x=0;x<96; x++)
					{
		    			std::bitset<32> y(intmemory[x]);

					memory = y.to_string();
					if (memory.substr(0, 8) == '00000000') break;

					BX[x] = std::stoi(memory.substr(4, 16));
				
					nrow = {std::stoi(memory.substr(20, 6)), std::stoi(memory.substr(33, 6)), std::stoi(memory.substr(46, 6)), std::stoi(memory.substr(59, 6))};
					nbend = {std::stoi(memory.substr(26, 5)), std::stoi(memory.substr(39, 5)), std::stoi(memory.substr(52, 5)), std::stoi(memory.substr(65, 5))};
					ncol = {std::stoi(memory.substr(31, 2)), std::stoi(memory.substr(44, 2)), std::stoi(memory.substr(57, 2)), std::stoi(memory.substr(70, 2))};
					}
	    			data[x].push_back(row);
	    			data[x].push_back(bend);
	    			data[x].push_back(col);
				}


			*/
			if (mode == 3)
				{
				for (int x=0;x<96; x++)
					{

		
					std::cout<<intmemory[x]<<std::endl;

					std::cout<<"header"<<std::endl;
					std::cout<<intmemory[x].substr(0, 8)<<std::endl;

					header = intmemory[x].substr(0, 8);
					if (header == "00000000") break;

					std::cout<<"BX"<<std::endl;
					std::cout<<intmemory[x].substr(8, 16)<<std::endl;

					std::cout<<"PIX"<<std::endl;
					std::cout<<intmemory[x].substr(24, 48)<<std::endl;

		    			std::bitset<16> b(intmemory[x].substr(8, 15));
					BX[x] = b.to_ulong();

	


		    			std::bitset<48> p(intmemory[x].substr(24, 47));
					hit = {p.to_ulong()};
					uint64_t temp = p.to_ulong();
					std::cout<<temp<<std::endl<<std::endl;

	    				data[x]=hit;
					}

				}
		
			std::pair<std::vector<uint32_t>, std::vector<std::vector<uint64_t>>> returndata(BX, data);	
			return returndata;
		
	}









	void MPAGlibFWInterface::StrobeSettings(int snum, int sdel, int slen, 
					       int sdist, int cal)
	  {
	    std::cout<<"\nStrobeSettings\n";
	    fBoardFW->WriteReg("Shutter.Strobe.number", snum);
	    fBoardFW->WriteReg("Shutter.Strobe.delay", sdel);
	    fBoardFW->WriteReg("Shutter.Strobe.length", slen);
	    fBoardFW->WriteReg("Shutter.Strobe.distance", sdist);
	    
	    fBoardFW->WriteReg("Control.calibration", cal);
	  }


	void MPAGlibFWInterface::SequencerInit(int smode,int sdur,int mem,int ibuff)
	  {
	    std::cout<<"\nSequencerInit\n";
	    fBoardFW->WriteReg("Shutter.time", sdur);
	    fBoardFW->WriteReg("Control.testbeam_mode", 0x0);
	    fBoardFW->WriteReg("Control.readout", mem);
	    fBoardFW->WriteReg("Control.Sequencer.datataking_continuous", smode);
	    fBoardFW->WriteReg("Control.Sequencer.buffers_index", ibuff);
	  }


	void MPAGlibFWInterface::upload(const std::string& pFilename, int nmpa, int conf)
	  {
	   	        pugi::xml_document doc;
			std::string fullname = "settings/Conf_"+pFilename+"_MPA"+std::to_string(nmpa)+"_config"+std::to_string(conf)+".xml";
	    		pugi::xml_parse_result result = doc.load_file( fullname.c_str() );
	    		if ( !result )
	    		{
				std::cout << "ERROR :\n Unable to open the file : " << pFilename << std::endl;
				std::cout << "Error description : " << result.description() << std::endl;
				return;
	    		}


			std::vector< uint32_t > conf_upload(25);
			int perif = -1;
	    		for ( pugi::xml_node cBeBoardNode = doc.child( "CONF" ).child( "periphery" ).first_child(); cBeBoardNode; cBeBoardNode = cBeBoardNode.next_sibling() )
				{
				if (static_cast<std::string>(cBeBoardNode.name())=="OM") perif = convertAnyInt(cBeBoardNode.child_value());
				if (static_cast<std::string>(cBeBoardNode.name())=="RT") perif |= ((convertAnyInt(cBeBoardNode.child_value())& 3)   << 2 );
				if (static_cast<std::string>(cBeBoardNode.name())=="SCW") perif |= ((convertAnyInt(cBeBoardNode.child_value())& 15)   << 4 );
				if (static_cast<std::string>(cBeBoardNode.name())=="SH2") perif |= ((convertAnyInt(cBeBoardNode.child_value())& 15)  << 8 );
				if (static_cast<std::string>(cBeBoardNode.name())=="SH1") perif |= ((convertAnyInt(cBeBoardNode.child_value())& 15)  << 12);
				if (static_cast<std::string>(cBeBoardNode.name())=="CALDAC") perif |= ((convertAnyInt(cBeBoardNode.child_value())& 255) << 16);
				if (static_cast<std::string>(cBeBoardNode.name())=="THDAC") perif |= ((convertAnyInt(cBeBoardNode.child_value())& 255) << 24);
				}
			conf_upload[0] = perif;
	    		for ( pugi::xml_node cBeBoardNode = doc.child( "CONF" ).first_child(); cBeBoardNode; cBeBoardNode = cBeBoardNode.next_sibling() )
				{
				int pix = -1;
				if (static_cast<std::string>(cBeBoardNode.name())=="pixel")
					{
					int pixnum = convertAnyInt(cBeBoardNode.attribute("n").value());

					if (pixnum<17 and pixnum>8)
						{
							for ( pugi::xml_node cBeBoardNode1 = cBeBoardNode.first_child(); cBeBoardNode1; cBeBoardNode1 = cBeBoardNode1.next_sibling() )
							{
								if (static_cast<std::string>(cBeBoardNode1.name())=="PMR") pix = convertAnyInt(cBeBoardNode1.child_value());
			
								if (static_cast<std::string>(cBeBoardNode1.name())=="ARR") pix |= ((convertAnyInt(cBeBoardNode.child_value()) & 1)	<< 1 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="TRIMDACL")  pix |= ((convertAnyInt(cBeBoardNode.child_value())& 31)	<< 2 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="CER")  pix |= ((convertAnyInt(cBeBoardNode.child_value())& 1)	<< 7 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="SP")  pix |= ((convertAnyInt(cBeBoardNode.child_value()) & 1)	<< 8 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="SR")  pix |= ((convertAnyInt(cBeBoardNode.child_value())& 1)	<< 9 ) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="PML")  pix |= ((convertAnyInt(cBeBoardNode.child_value())& 1)	<< 10);
								if (static_cast<std::string>(cBeBoardNode1.name())=="ARL")  pix |= ((convertAnyInt(cBeBoardNode.child_value()) & 1)	<< 11) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="TRIMDACR")  pix |= ((convertAnyInt(cBeBoardNode.child_value())& 31)	<< 12) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="CEL")  pix |= ((convertAnyInt(cBeBoardNode.child_value()) & 1)	<< 17);
								if (static_cast<std::string>(cBeBoardNode1.name())=="CW")  pix |= ((convertAnyInt(cBeBoardNode.child_value()) & 2)	<< 18);
							}
						}
					if (pixnum<25 and pixnum>0)
						{
							for ( pugi::xml_node cBeBoardNode1 = cBeBoardNode.first_child(); cBeBoardNode1; cBeBoardNode1 = cBeBoardNode1.next_sibling() )
							{
								if (static_cast<std::string>(cBeBoardNode1.name())=="PML") pix = convertAnyInt(cBeBoardNode1.child_value());
								if (static_cast<std::string>(cBeBoardNode1.name())=="ARL") pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 1 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="TRIMDACL")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 31)	<< 2 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="CEL")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 1)	<< 7 ) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="CW")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 3)	<< 8  );
								if (static_cast<std::string>(cBeBoardNode1.name())=="PMR")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 1)	<< 10) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="ARR")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 1)	<< 11);
								if (static_cast<std::string>(cBeBoardNode1.name())=="TRIMDACR")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 31)	<< 12) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="CER")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 17) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="SP")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 18);
								if (static_cast<std::string>(cBeBoardNode1.name())=="SR")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 19);
							}
						}


					conf_upload[pixnum] = pix;
					}

				}
	    std::cout<<"Writing to Configuration.Memory_DataConf.MPA"+std::to_string(nmpa)+".config_1 " <<std::endl;
	    fBoardFW->WriteBlockReg( "Configuration.Memory_DataConf.MPA"+std::to_string(nmpa)+".config_1", conf_upload);
	    fBoardFW->WriteReg( "Configuration.mode",0x5);
	    fBoardFW->WriteBlockReg( "Configuration.Memory_DataConf.MPA"+std::to_string(nmpa)+".config_1", conf_upload);
	    fBoardFW->WriteReg( "Configuration.mode",0x5);
	    }












}
