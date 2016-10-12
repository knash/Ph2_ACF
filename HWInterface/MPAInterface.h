/*!

        \file                                            MPAInterface.h
        \brief                                           User Interface to the MPAs
        \author                                          Lorenzo BIDEGAIN, Nicolas PIERRE
        \version                                         1.0
        \date                        31/07/14
        Support :                    mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

 */

#ifndef __MPAINTERFACE_H__
#define __MPAINTERFACE_H__

#include <vector>
#include "../HWInterface/GlibFWInterface.h"
#include "../HWInterface/CtaFWInterface.h"
#include "../HWInterface/MPAGlibFWInterface.h"

using namespace Ph2_HwDescription;

/*!
 * \namespace Ph2_HwInterface
 * \brief Namespace regrouping all the interfaces to the hardware
 */
namespace Ph2_HwInterface
{

	using BeBoardFWMap = std::map<uint16_t, BeBoardFWInterface*>;    /*!< Map of Board connected */

	/*!
	 * \class MPAInterface
	 * \brief Class representing the User Interface to the MPA on different boards
	 */
	class MPAInterface
	{

	  private:
		BeBoardFWMap fBoardMap;                     /*!< Map of Board connected */
		BeBoardFWInterface* fBoardFW;                     /*!< Board loaded */
		MPAGlibFWInterface* fMPAFW;                     /*!< Board loaded */
		uint16_t prevBoardIdentifier;                     /*!< Id of the previous board */

		uint16_t fRegisterCount;                                /*!< Counter for the number of Registers written */
		uint16_t fTransactionCount;         /*!< Counter for the number of Transactions */


	  private:
		/*!
		 * \brief Set the board to talk with
		 * \param pBoardId
		 */
		void setBoard( uint16_t pBoardIdentifier );

	public:
		/*!
		* \brief Constructor of the MPAInterface Class
		* \param pBoardMap
		*/
		MPAInterface( const BeBoardFWMap& pBoardMap );
		/*!
		* \brief Destructor of the MPAInterface Class
		*/
		~MPAInterface();

		void ConfigureMPA(std::vector< uint32_t >* conf_upload, int nmpa);
		void HeaderInitMPA(int nmpa);
    		void modifyperif(std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod , std::vector< uint32_t >* conf_upload);
    		void modifypix(std::pair < std::vector< std::string > ,std::vector< uint32_t >> mod , std::vector< uint32_t >* conf_upload, uint32_t  pixnum );
 		std::pair<std::vector<uint32_t>, std::vector<std::string>> FormatData(std::pair<std::vector<uint32_t>, std::vector<uint32_t>> data);
    		std::pair<std::vector<uint32_t>, std::vector<uint64_t>> ReadMemory(std::vector<std::string> intmemory, int mode);
    		std::vector< uint32_t > readconfig(const std::string& pFilename, int nmpa, int conf);
		void SequencerInit(int smode,int sdur,int mem,int ibuff);
	};
}

#endif
