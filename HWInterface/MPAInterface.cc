/*

        FileName :                     MPAInterface.cc
        Content :                      User Interface to the MPAs
        Programmer :                   Lorenzo BIDEGAIN, Nicolas PIERRE, Georg AUZINGER
        Version :                      1.0
        Date of creation :             10/07/14
        Support :                      mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

 */

#include "MPAInterface.h"
#include "../Utils/ConsoleColor.h"
#include <typeinfo>
#define DEV_FLAG 0
// #define COUNT_FLAG 0

namespace Ph2_HwInterface
{

MPAInterface::MPAInterface( const BeBoardFWMap& pBoardMap ) :
    fBoardMap( pBoardMap ),
    fBoardFW( nullptr ),
    prevBoardIdentifier( 65535 ),
    fRegisterCount( 0 ),
    fTransactionCount( 0 )
{
#ifdef COUNT_FLAG
    std::cout << "Counting number of Transactions!" << std::endl;
#endif
}

MPAInterface::~MPAInterface()
{
}

void MPAInterface::setBoard( uint16_t pBoardIdentifier )
{
    if ( prevBoardIdentifier != pBoardIdentifier )
    {
        BeBoardFWMap::iterator i = fBoardMap.find( pBoardIdentifier );

        if ( i == fBoardMap.end() )
            std::cout << "The Board: " << +( pBoardIdentifier >> 8 ) << "  doesn't exist" << std::endl;
        else
        {
            fBoardFW = i->second;
	    fMPAFW = dynamic_cast<MPAGlibFWInterface*>(fBoardFW);
            prevBoardIdentifier = pBoardIdentifier;
        }
    }
}










void MPAInterface::HeaderInitMPA(int nmpa)
{
		setBoard(0);
		fMPAFW->HeaderInitMPA( nmpa );
}



void MPAInterface::ConfigureMPA(std::vector< uint32_t >* conf_upload, int nmpa)
{
	setBoard(0);
	fMPAFW->upload( conf_upload, nmpa );
}


void MPAInterface::SequencerInit(int smode,int sdur,int mem,int ibuff)
{
	setBoard(0);
	fMPAFW->SequencerInit(smode,sdur,mem,ibuff);
}












std::pair<std::vector<uint32_t>, std::vector<std::string>> MPAInterface::FormatData(std::pair<std::vector<uint32_t>, std::vector<uint32_t>> data)
{
		std::vector<uint32_t> counter_data = data.first;
		std::vector<uint32_t> memory_data = data.second;
		std::vector<uint32_t> bitpix(50);
		std::vector<std::string> mem(96);
		/*
		std::bitset<32> a(counter_data[0]);
		std::cout<<a.to_string()<<std::endl;

		std::bitset<32> b(counter_data[1]);
		std::cout<<b.to_string()<<std::endl;

		std::bitset<32> c(counter_data[2]);
		std::cout<<c.to_string()<<std::endl;
		*/
	    	for (int x=0;x<24; x++) 
			{
				//std::cout<<counter_data[x]<<std::endl;
				int shift1 = 0;
				int shift2 = 16;



				uint32_t l((counter_data[x+1] >> shift1) & 0xFFFF);
				uint32_t r((counter_data[x+1] >> shift2) & 0xFFFF);
				bitpix[2*x] = l;
				bitpix[2*x+1] = r;
				/*
		    		std::bitset<32> p(counter_data[x+1]);
				std::cout<<p.to_string()<<std::endl;

		    		std::bitset<16> pl(l);
				std::cout<<pl.to_string()<<std::endl;

		    		std::bitset<16> pr(r);
				std::cout<<pr.to_string()<<std::endl;
				*/
	
			}

		std::bitset<32> y(memory_data[215]);
		std::string memory_string = y.to_string();
		for (int x=1;x<216; x++)
			{
		    		std::bitset<32> y(memory_data[215 - x]);
				memory_string = memory_string + y.to_string();
			}
		for (int x=0;x<96; x++)
			{
				mem[x] = memory_string.substr(x*72, 72);

			}
	    	std::pair<std::vector<uint32_t>, std::vector<std::string>>  returndata(bitpix,mem);
		return 	returndata;
}

std::pair<std::vector<uint32_t>, std::vector<uint64_t>> MPAInterface::ReadMemory(std::vector<std::string> intmemory, int mode)
{
			std::string memory; 
			std::vector<uint32_t> BX(96);
			uint64_t hit;
			std::vector<int> row(96);
			std::vector<int> col(96);
			std::vector<uint64_t> data(96);
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

		
					//std::cout<<intmemory[x]<<std::endl;
					//std::cout<<"header"<<std::endl;
					//std::cout<<intmemory[x].substr(0, 8)<<std::endl;

					header = intmemory[x].substr(0, 8);
					if (header != "11111111") break;

					//std::cout<<"BX"<<std::endl;
					//std::cout<<intmemory[x].substr(8, 16)<<std::endl;

					//std::cout<<"PIX"<<std::endl;
					//std::cout<<intmemory[x].substr(24, 48)<<std::endl;

		    			std::bitset<16> b(intmemory[x].substr(8, 16));
					BX[x] = b.to_ulong();

	


		    			std::bitset<48> p(intmemory[x].substr(24, 48));
					hit = p.to_ulong();
					uint64_t temp = p.to_ulong();
					//std::cout<<temp<<std::endl<<std::endl;

	    				data[x]=hit;
					}

				}
		
			std::pair<std::vector<uint32_t>, std::vector<uint64_t>> returndata(BX, data);	
			return returndata;
		
}


std::vector< uint32_t > MPAInterface::readconfig(const std::string& pFilename, int nmpa, int conf)
{

	   	        pugi::xml_document doc;
			std::string fullname = "settings/Conf_"+pFilename+"_MPA"+std::to_string(nmpa)+"_config"+std::to_string(conf)+".xml";
	    		pugi::xml_parse_result result = doc.load_file( fullname.c_str() );
	    		if ( !result )
	    		{
				std::cout << "ERROR :\n Unable to open the file : " << pFilename << std::endl;
				std::cout << "Error description : " << result.description() << std::endl;
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
				int pix = 0;
				if (static_cast<std::string>(cBeBoardNode.name())=="pixel")
					{
					int pixnum = convertAnyInt(cBeBoardNode.attribute("n").value());

					if (pixnum<17 and pixnum>8)
						{
							for ( pugi::xml_node cBeBoardNode1 = cBeBoardNode.first_child(); cBeBoardNode1; cBeBoardNode1 = cBeBoardNode1.next_sibling() )
							{
								if (static_cast<std::string>(cBeBoardNode1.name())=="PMR") pix |= convertAnyInt(cBeBoardNode1.child_value());		
								if (static_cast<std::string>(cBeBoardNode1.name())=="ARR") pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 1 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="TRIMDACL")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 31)	<< 2 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="CER")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 1)	<< 7 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="SP")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 8 );
								if (static_cast<std::string>(cBeBoardNode1.name())=="SR")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 1)	<< 9 ) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="PML")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 1)	<< 10);
								if (static_cast<std::string>(cBeBoardNode1.name())=="ARL")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 11) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="TRIMDACR")  pix |= ((convertAnyInt(cBeBoardNode1.child_value())& 31)	<< 12) ;
								if (static_cast<std::string>(cBeBoardNode1.name())=="CEL")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 1)	<< 17);
								if (static_cast<std::string>(cBeBoardNode1.name())=="CW")  pix |= ((convertAnyInt(cBeBoardNode1.child_value()) & 2)	<< 18);
			

							}
						}
					else if (pixnum<25 and pixnum>0)
						{
							for ( pugi::xml_node cBeBoardNode1 = cBeBoardNode.first_child(); cBeBoardNode1; cBeBoardNode1 = cBeBoardNode1.next_sibling() )
							{
								if (static_cast<std::string>(cBeBoardNode1.name())=="PML") pix |= convertAnyInt(cBeBoardNode1.child_value());
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
	  return conf_upload;
}




void MPAInterface::modifyperif(std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod , std::vector< uint32_t >* conf_upload)
{
	  std::vector<std::string> vars = mod.first;
	  std::vector< uint32_t > vals = mod.second;
	  int perif = conf_upload->at(0);

	  for (int iperif=0;iperif<vars.size(); iperif++)
	  {
		if (vars[iperif]=="OM") 
			{
			perif = (perif&~3);
			perif |= (vals[iperif]);
			}
		if (vars[iperif]=="RT") 
			{
			perif = (perif&~(3<<2));
			perif |= ((vals[iperif]& 3)   << 2 );
			}
		if (vars[iperif]=="SCW") 
			{
			perif = (perif&~(15<<4));
			perif |= ((vals[iperif]& 15)   << 4 );
			}
		if (vars[iperif]=="SH2") 
			{
			perif = (perif&~(15<<8));
			perif |= ((vals[iperif]& 15)  << 8 );
			}
		if (vars[iperif]=="SH1") 
			{
			perif = (perif&~(15<<12));
			perif |= ((vals[iperif]& 15)  << 12);
			}
		if (vars[iperif]=="CALDAC") 
			{
			perif = (perif&~(255<<16));
			perif |= ((vals[iperif]& 255) << 16);
			}
		if (vars[iperif]=="THDAC") 
			{
			perif = (perif&~(255<<24));
			perif |= ((vals[iperif]& 255) << 24);
			}

	  }
	  conf_upload->at(0) = perif;

}
void MPAInterface::modifypix(std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod , std::vector< uint32_t >* conf_upload ,  uint32_t  pixnum )
{

	  	std::vector<std::string> vars = mod.first;
	  	std::vector< uint32_t > vals = mod.second;
		
		uint32_t pix = conf_upload->at(pixnum);
		if (pixnum<17 and pixnum>8)
			{
				for (int ipix=0;ipix<vars.size(); ipix++)
				{
					if (vars[ipix]=="PMR") 
					{
						pix = (pix&~1);
						pix |= (vals[ipix]);
					}
					if (vars[ipix]=="ARR") 
					{
						pix = (pix&~(1<<1));
						pix |= ((vals[ipix]& 1) << 1);
					}
					if (vars[ipix]=="TRIMDACL")  
					{
						pix = (pix&~(31<<2));
						pix |= ((vals[ipix]& 31) << 2);
					}
					if (vars[ipix]=="CER") 
					{ 
						pix = (pix&~(1<<7));
						pix |= ((vals[ipix]& 1) << 7);
					}
					if (vars[ipix]=="SP") 
					{ 
						pix = (pix&~(1<<8));
						pix |= ((vals[ipix]& 1) << 8);
					}
					if (vars[ipix]=="SR")  
					{
						pix = (pix&~(1<<9));
						pix |= ((vals[ipix]& 1) << 9);
					}
					if (vars[ipix]=="PML")  
					{
						pix = (pix&~(1<<10));
						pix |= ((vals[ipix]& 1) << 10);
					}
					if (vars[ipix]=="ARL")  
					{
						pix = (pix&~(1<<11));
						pix |= ((vals[ipix]& 1) << 11);
					}
					if (vars[ipix]=="TRIMDACR") 
					{ 
						pix = (pix&~(31<<12));
						pix |= ((vals[ipix]& 31) << 12);
					}

					if (vars[ipix]=="CEL")  
					{
						pix = (pix&~(1<<17));
						pix |= ((vals[ipix]& 1) << 17);
					}
					if (vars[ipix]=="CW")  
					{
						pix = (pix&~(2<<18));
						pix |= ((vals[ipix]& 2) << 18);
					}
				}
			}
		else if (pixnum<25 and pixnum>0)
			{
				for (int ipix=0;ipix<vars.size(); ipix++)
				{
					if (vars[ipix]=="PML") 
					{
						pix = (pix&~1);
						pix |= (vals[ipix]);
					}
					if (vars[ipix]=="ARL") 
					{
						pix = (pix&~(1<<1));
						pix |= ((vals[ipix]& 1) << 1);
					}
					if (vars[ipix]=="TRIMDACL")  
					{
						pix = (pix&~(31<<2));
						pix |= ((vals[ipix]& 31) << 2);
					}
					if (vars[ipix]=="CEL")  
					{
						pix = (pix&~(1<<7));
						pix |= ((vals[ipix]& 1) << 7);
					}
					if (vars[ipix]=="CW")  
					{
						pix = (pix&~(3<<8));
						pix |= ((vals[ipix]& 3) << 8);
					}
					if (vars[ipix]=="PMR")  
					{
						pix = (pix&~(1<<10));
						pix |= ((vals[ipix]& 1) << 10);
					}
					if (vars[ipix]=="ARR")  
					{
						pix = (pix&~(1<<11));
						pix |= ((vals[ipix]& 1) << 11);
					}
					if (vars[ipix]=="TRIMDACR")  
					{
						pix = (pix&~(31<<12));
						pix |= ((vals[ipix]& 31) << 12);
					}
					if (vars[ipix]=="CER")  
					{
						pix = (pix&~(1<<17));
						pix |= ((vals[ipix]& 1) << 17);
					}
					if (vars[ipix]=="SP")  
					{
						pix = (pix&~(1<<18));
						pix |= ((vals[ipix]& 1) << 18);
					}
					if (vars[ipix]=="SR")  
					{
						pix = (pix&~(1<<19));
						pix |= ((vals[ipix]& 1) << 19);
					}
				}
			}


		conf_upload->at(pixnum) = pix;

}


}
