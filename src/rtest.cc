#include <cstring>
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../HWInterface/RegManager.h"
#include "../Utils/argvparser.h"
#include "../System/SystemController.h"
#include "../HWInterface/MPAGlibFWInterface.h"
#include "../HWInterface/GlibFpgaConfig.h"
#include <uhal/uhal.hpp>

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

using namespace CommandLineProcessing;


int main( int argc, char* argv[] )
{
	ArgvParser cmd;

	// init
	cmd.setIntroductoryDescription( "CMS Ph2_ACF  RUTGERS ONLY TEST 2" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );

	cmd.defineOption( "file", "Hw Description File . Default value: settings/HWDescription_MAPSA.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );

	cmd.defineOption( "output", "Output Directory . Default value: Results", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "output", "o" );

	int result = cmd.parse( argc, argv );
	if ( result != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result );
		exit( 1 );
	}

	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "settings/HWDescription_MAPSA.xml";
	std::string cDirectory = ( cmd.foundOption( "output" ) ) ? cmd.optionValue( "output" ) : "Results/";


	SystemController mysyscontroller;
	mysyscontroller.InitializeHw( cHWFile );
	//  mysyscontroller.ConfigureHw(std::cout); 
	BeBoard* pBoard = mysyscontroller.fBoardVector.at( 0 );
	std::cout << "\nExecuting POWER ON...";
	mysyscontroller.fBeBoardInterface->PowerOn(pBoard);
	std::cout << "\nFirmware version: "; 
	mysyscontroller.fBeBoardInterface->ReadVer(pBoard);
	std::chrono::milliseconds cWait( 10 );

	for(int i=1;i<=6;i++)
	{

		std::this_thread::sleep_for( cWait );
		std::vector< uint32_t > conf_upload1 = mysyscontroller.fBeBoardInterface->readconfig(pBoard,"testing", i, 1);
		std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod1({"THDAC"},{60});
		std::vector< uint32_t > conf_upload11 = mysyscontroller.fBeBoardInterface->modifyperif(pBoard,mod1,conf_upload1);
		for (int j=1;j<=24;j++)
			{
			std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod2({"PMR"},{0});
			conf_upload11 =  mysyscontroller.fBeBoardInterface->modifypix(pBoard,mod2,conf_upload11,j);
			}
		mysyscontroller.fBeBoardInterface->upload(pBoard,conf_upload11, i);
	}
	std::chrono::milliseconds cWait1( 100 );//



	for(int q=1;q<=250;q++)
	{
	mysyscontroller.fBeBoardInterface->SequencerInit(pBoard,0,300000,1,0);
	//std::this_thread::sleep_for( cWait );


	//std::pair<std::vector<uint32_t>, std::vector<std::string>>  formdata = mysyscontroller.fBeBoardInterface->FormatData(pBoard,returndata);
	//std::vector<std::string> intmem = formdata.second;
	//std::pair<std::vector<uint32_t>, std::vector<std::vector<uint64_t>>>  readdata = mysyscontroller.fBeBoardInterface->ReadMemory(pBoard,intmem, 3);
	//for(int i=1;i<=6;i++)
	//{

	//	std::pair<std::vector<uint32_t>, std::vector<uint32_t>>  returndata = mysyscontroller.fBeBoardInterface->ReadData(pBoard, 1,i);
	//	std::cout<<"AR"<<std::endl;
	//	std::cout<<returndata.first[5]<<std::endl;
	//	std::cout<<returndata.first[6]<<std::endl;
	//}
	std::pair<std::vector<uint32_t>, std::vector<uint32_t>>  returndata = mysyscontroller.fBeBoardInterface->ReadData(pBoard, 1,1);
	std::cout<<returndata.first[5]<<std::endl;
	for(int i=1;i<=6;i++)
	{

		//std::this_thread::sleep_for( cWait );
		std::vector< uint32_t > conf_upload1 = mysyscontroller.fBeBoardInterface->readconfig(pBoard,"testing", i, 1);
		std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod1({"THDAC"},{q});
		std::vector< uint32_t > conf_upload11 = mysyscontroller.fBeBoardInterface->modifyperif(pBoard,mod1,conf_upload1);
		mysyscontroller.fBeBoardInterface->upload(pBoard,conf_upload11, i);
	}


	}
	/*
	for (int k=2;k<30; k++)
	{

		std::cout<<"BX"<<std::endl;
		std::cout<<readdata.first[k]<<std::endl;
		std::cout<<"PIX"<<std::endl;
		std::bitset<32> b(readdata.second[k][0]);
		std::cout<<b.to_string()<<std::endl;
		std::cout<<"AR"<<std::endl;
		std::cout<<formdata.first[k]<<std::endl<<std::endl;
	}
	*/
	//mysyscontroller.fBeBoardInterface->Pause(pBoard);

	//}
/*	std::vector<uint32_t> counts = data.first;
	std::vector<uint32_t> mems = data.second;

/*
	mysyscontroller.fBeBoardInterface->HeaderInitMPA(pBoard,2);
	mysyscontroller.fBeBoardInterface->HeaderInit(pBoard);
	mysyscontroller.fBeBoardInterface->ReadTrig(pBoard,1);
	
	mysyscontroller.fBeBoardInterface->TestbeamInit(pBoard, 0, 0);
	mysyscontroller.fBeBoardInterface->StrobeSettings(pBoard, 0,0,0,0, 1);

	mysyscontroller.fBeBoardInterface->ReadData(pBoard, 1,1,0);

	mysyscontroller.fBeBoardInterface->VDDPSTon(pBoard);
	mysyscontroller.fBeBoardInterface->VDDPSToff(pBoard);
	mysyscontroller.fBeBoardInterface->DVDDon(pBoard);
	mysyscontroller.fBeBoardInterface->DVDDoff(pBoard);
	mysyscontroller.fBeBoardInterface->PVDDon(pBoard);
	mysyscontroller.fBeBoardInterface->PVDDoff(pBoard);
	mysyscontroller.fBeBoardInterface->VBIASon(pBoard);
	mysyscontroller.fBeBoardInterface->VBIASoff(pBoard);
	mysyscontroller.fBeBoardInterface->AVDDon(pBoard);

	mysyscontroller.fBeBoardInterface->WaitSequencer(pBoard);
	mysyscontroller.fBeBoardInterface->ReadRaw(pBoard,0,0,1);


	//	uint32_t uVal = ReadVer();
		//	mysyscontroller.InitializeSettings( HWFile );
	
		//	mysyscontroller.ConfigureHw();
*/
}

