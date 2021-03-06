#include "../System/SystemController.h"
#include "../Utils/argvparser.h"


using namespace Ph2_System;
using namespace CommandLineProcessing;

int main( int argc, char** argv )
{

	ArgvParser cmd;

	// init
	cmd.setIntroductoryDescription( "CMS Ph2_ACF  system test application" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );

	cmd.defineOption( "file", "Hw Description File . Default value: settings/HWDescription_2CBC.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );

	cmd.defineOption( "configure", "Configure HW", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "configure", "c" );

	int result = cmd.parse( argc, argv );
	if ( result != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result );
		exit( 1 );
	}

	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "settings/HWDescription_2CBC.xml";
	bool cConfigure = ( cmd.foundOption( "configure" ) ) ? true : false;


	SystemController cSystemController;
	cSystemController.InitializeHw( cHWFile );
	cSystemController.InitializeSettings( cHWFile );
	if ( cConfigure ) cSystemController.ConfigureHw();

	std::cout << "*** End of the System test ***" << std::endl;
	return 0;
}
