/*

        FileName :                    BeBoardInterface.cc
        Content :                     User Interface to the Boards
        Programmer :                  Lorenzo BIDEGAIN, Nicolas PIERRE
        Version :                     1.0
        Date of creation :            31/07/14
        Support :                     mail to : lorenzo.bidegain@gmail.com nico.pierre@icloud.com

 */

#include "BeBoardInterface.h"

namespace Ph2_HwInterface
{

	BeBoardInterface::BeBoardInterface( const BeBoardFWMap& pBoardMap ) :
		fBoardMap( pBoardMap ),
		fBoardFW( nullptr ),
		prevBoardIdentifier( 65535 )

	{
	}



	BeBoardInterface::~BeBoardInterface()
	{

	}

	void BeBoardInterface::setBoard( uint16_t pBoardIdentifier )
	{
		if ( prevBoardIdentifier != pBoardIdentifier )
		{
			BeBoardFWMap::iterator i = fBoardMap.find( pBoardIdentifier );
			if ( i == fBoardMap.end() )
				std::cout << "The Board: " << +( pBoardIdentifier >> 8 ) << "  doesn't exist" << std::endl;
			else
			{
				fBoardFW = i->second;
				prevBoardIdentifier = pBoardIdentifier;
			}
		}
	}

	void BeBoardInterface::WriteBoardReg( BeBoard* pBoard, const std::string& pRegNode, const uint32_t& pVal )
	{
		setBoard( pBoard->getBeBoardIdentifier() );

		fBoardFW->WriteReg( pRegNode, pVal );
		pBoard->setReg( pRegNode, pVal );
	}

	void BeBoardInterface::WriteBlockBoardReg( BeBoard* pBoard, const std::string& pRegNode, const std::vector<uint32_t>& pValVec )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->WriteBlockReg( pRegNode, pValVec );
	}


	void BeBoardInterface::WriteBoardMultReg( BeBoard* pBoard, const std::vector < std::pair< std::string , uint32_t > >& pRegVec )
	{
		setBoard( pBoard->getBeBoardIdentifier() );

		fBoardFW->WriteStackReg( pRegVec );

		for ( const auto& cReg : pRegVec )
		{
			// fBoardFW->WriteReg( cReg.first, cReg.second );
			pBoard->setReg( cReg.first, cReg.second );
		}
	}


	uint32_t BeBoardInterface::ReadBoardReg( BeBoard* pBoard, const std::string& pRegNode )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		uint32_t cRegValue = static_cast<uint32_t>( fBoardFW->ReadReg( pRegNode ) );
		pBoard->setReg( pRegNode,  cRegValue );
		return cRegValue;
	}

	void BeBoardInterface::ReadBoardMultReg( BeBoard* pBoard, std::vector < std::pair< std::string , uint32_t > >& pRegVec )
	{
		setBoard( pBoard->getBeBoardIdentifier() );

		for ( auto& cReg : pRegVec )
		{
			cReg.second = static_cast<uint32_t>( fBoardFW->ReadReg( cReg.first ) );
			pBoard->setReg( cReg.first, cReg.second );
		}
	}

	std::vector<uint32_t> BeBoardInterface::ReadBlockBoardReg( BeBoard* pBoard, const std::string& pRegNode, uint32_t pSize )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->ReadBlockRegValue( pRegNode, pSize );
	}

	void BeBoardInterface::getBoardInfo( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->getBoardInfo();
	}

	void BeBoardInterface::ConfigureBoard( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeId() );
		fBoardFW->ConfigureBoard( pBoard );
	}


	void BeBoardInterface::StartThread( BeBoard* pBoard , uint32_t uNbAcq, HwInterfaceVisitor* visitor )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->StartThread( pBoard, uNbAcq, visitor );
	}

	void BeBoardInterface::StopThread( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->StopThread();
	}

	int BeBoardInterface::getNumAcqThread( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->getNumAcqThread();
	}

	bool BeBoardInterface::isRunningThread( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->isRunningThread();
	}

	void BeBoardInterface::Start( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Start();
	}


	void BeBoardInterface::Stop( BeBoard* pBoard, uint32_t pNthAcq )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Stop( pNthAcq );
	}


	void BeBoardInterface::Pause( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Pause();
	}


	void BeBoardInterface::Resume( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->Resume();
	}


	uint32_t BeBoardInterface::ReadData( BeBoard* pBoard, uint32_t pNthAcq, bool pBreakTrigger )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->ReadData( pBoard, pNthAcq, pBreakTrigger );
	}

	const Event* BeBoardInterface::GetNextEvent( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->GetNextEvent( pBoard );
	}

	const Event* BeBoardInterface::GetEvent( const BeBoard* pBoard, int i )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->GetEvent( pBoard, i );
	}
	const std::vector<Event*>& BeBoardInterface::GetEvents( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->GetEvents( pBoard );
	}

	const uhal::Node& BeBoardInterface::getUhalNode( const BeBoard* pBoard, const std::string& pStrPath )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->getUhalNode( pStrPath );
	}

	uhal::HwInterface* BeBoardInterface::getHardwareInterface( const BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->getHardwareInterface();
	}


	void BeBoardInterface::FlashProm( BeBoard* pBoard, const std::string& strConfig, const char* pstrFile )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->FlashProm( strConfig, pstrFile );
	}

	void BeBoardInterface::JumpToFpgaConfig( BeBoard* pBoard, const std::string& strConfig)
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->JumpToFpgaConfig( strConfig );
	}

	void BeBoardInterface::DownloadFpgaConfig( BeBoard* pBoard, const std::string& strConfig, const std::string& strDest)
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->DownloadFpgaConfig( strConfig, strDest );
	}

	const FpgaConfig* BeBoardInterface::getConfiguringFpga( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->getConfiguringFpga();
	}

	std::vector<std::string> BeBoardInterface::getFpgaConfigList( BeBoard* pBoard)
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		return fBoardFW->getFpgaConfigList();
	}

	void BeBoardInterface::DeleteFpgaConfig(BeBoard* pBoard, const std::string& strId)
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->DeleteFpgaConfig( strId );
	}




	void BeBoardInterface::PowerOn( BeBoard* pBoard )
	{
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->PowerOn();
	}
	void BeBoardInterface::ReadVer( BeBoard* pBoard )
	{
	//  std::cout << "Exe ReadVer from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->ReadVer();
	}
  void BeBoardInterface::TestbeamInit( BeBoard* pBoard, int clock, int phase)
	{
	//  std::cout << "Exe TestbeamInit from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->TestbeamInit(clock, phase);
		// overrides this anyway?
	}
  void BeBoardInterface::StrobeSettings( BeBoard* pBoard, 
					 int snum, int sdel, int slen, 
					 int sdist, int cal)
	{
	//  std::cout << "StrobeSettings from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->StrobeSettings(snum,sdel,slen,sdist,cal);
		// overrides this anyway?
	}
  std::pair<std::vector<uint32_t>, std::vector<uint32_t>>  BeBoardInterface::ReadData( BeBoard* pBoard, 
					 int buffernum, int mpa)
	{
	//  std::cout << "ReadData from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		std::pair<std::vector<uint32_t>, std::vector<uint32_t>> rval = fBoardFW->ReadData(buffernum,mpa);

		return rval;	
	// overrides this anyway?
	}


    std::pair<std::vector<uint32_t>, std::vector<std::string>> BeBoardInterface::FormatData(BeBoard* pBoard,std::pair<std::vector<uint32_t>, std::vector<uint32_t>> data)
	{
	//  std::cout << "Exe FormatData from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		std::pair<std::vector<uint32_t>,std::vector<std::string>>  rval = fBoardFW->FormatData(data);
		return rval;
	// overrides this anyway?
	}
    std::pair<std::vector<uint32_t>, std::vector<std::vector<uint64_t>>> BeBoardInterface::ReadMemory(BeBoard* pBoard, std::vector<std::string> intmemory, int mode)
	{
	//  std::cout << "Exe ReadMemory from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		std::pair<std::vector<uint32_t>, std::vector<std::vector<uint64_t>>>  rval = fBoardFW->ReadMemory(intmemory,mode);
		return rval;
	// overrides this anyway?
	}
        void BeBoardInterface::SequencerInit( BeBoard* pBoard,int smode,int sdur,int mem,int ibuff)
	{
	//  std::cout << "Exe SequencerInit from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->SequencerInit(smode,sdur,mem,ibuff);
		// overrides this anyway?
	}


        void BeBoardInterface::upload( BeBoard* pBoard,std::vector< uint32_t > conf_upload, int nmpa)
	{
	//  std::cout << "Exe upload from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->upload(conf_upload,nmpa);
		// overrides this anyway?
	}



        void BeBoardInterface::HeaderInit( BeBoard* pBoard)
	{
	//  std::cout << "Exe HeaderInit from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->HeaderInit();
		// overrides this anyway?
	}
        void BeBoardInterface::HeaderInitMPA( BeBoard* pBoard, int nmpa )
	{
	//  std::cout << "Exe HeaderInitMPA from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->HeaderInitMPA(nmpa);
		// overrides this anyway?
	}
        void BeBoardInterface::ReadTrig( BeBoard* pBoard, int buffer_num )
	{
	 // std::cout << "Exe ReadTrig from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		fBoardFW->ReadTrig(buffer_num);
		// overrides this anyway?
	}


        int BeBoardInterface::WaitSequencer( BeBoard* pBoard )
	{
	 // std::cout << "Exe WaitSequencer from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		int rval = fBoardFW->WaitSequencer();
		// overrides this anyway?
	}





	std::vector< uint32_t > BeBoardInterface::readconfig(BeBoard* pBoard,const std::string& pFilename, int nmpa, int conf)
	  {

	 // std::cout << "Exe readconfig from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		std::vector< uint32_t >  rval = fBoardFW->readconfig(pFilename, nmpa, conf);
		return rval;

	  }

	std::vector< uint32_t > BeBoardInterface::modifyperif(BeBoard* pBoard,std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod , std::vector< uint32_t > conf_upload)
	  {
	//  std::cout << "Exe modifyperif from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		std::vector< uint32_t >  rval = fBoardFW->modifyperif(mod,conf_upload);
		return rval;

	  }
	std::vector< uint32_t > BeBoardInterface::modifypix(BeBoard* pBoard,std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod , std::vector< uint32_t > conf_upload, uint32_t  pixnum )
	  {
	 // std::cout << "Exe modifypix from BeBoardInterface" << std::endl;
		setBoard( pBoard->getBeBoardIdentifier() );
		std::vector< uint32_t >  rval = fBoardFW->modifypix(mod,conf_upload,pixnum);
		return rval;
	  }













}
