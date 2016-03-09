
#include <cstring>
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../Utils/argvparser.h"
#include "TROOT.h"
#include "TApplication.h"
#include "../Utils/Timer.h"



using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

using namespace CommandLineProcessing;


int main ( int argc, char* argv[] )
{
    // the following section greatly simplifies the command line parsing of parameters
    ArgvParser cmd;

    // init
    cmd.setIntroductoryDescription ( "Simple testbed for DAQ workshop commissioning Exercise!" );
    // error codes
    cmd.addErrorCode ( 0, "Success" );
    cmd.addErrorCode ( 1, "Error" );
    // options
    cmd.setHelpOption ( "h", "help", "Print this help page" );

    cmd.defineOption ( "file", "Hw Description File . Default value: settings/Calibration8CBC.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
    cmd.defineOptionAlternative ( "file", "f" );

    cmd.defineOption ( "output", "Output Directory . Default value: Results", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
    cmd.defineOptionAlternative ( "output", "o" );

    // parse
    int result = cmd.parse ( argc, argv );

    if ( result != ArgvParser::NoParserError )
    {
        std::cout << cmd.parseErrorDescription ( result );
        exit ( 1 );
    }

    // now query the parsing results
    std::string cHWFile = ( cmd.foundOption ( "file" ) ) ? cmd.optionValue ( "file" ) : "settings/Calibration8CBC.xml";
    std::string cDirectory = ( cmd.foundOption ( "output" ) ) ? cmd.optionValue ( "output" ) : "Results/";
    cDirectory += "Test";

    // TApplication helps to visualize the results via TCanvas while the Application is running!
    TApplication cApp ( "Root Application", &argc, argv );

    if ( batchMode ) gROOT->SetBatch ( true );
    else TQObject::Connect ( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );

    // A little timer so we can benchmark our Code!
    Timer t;
    t.start();

    // instantiating a tool inheriting from System Controller, parsing the HW Description & Settings, creating output Directory
    Tool cTool;
    cTool.InitializeHw ( cHWFile );
    cTool.InitializeSettings ( cHWFile );
    cTool.CreateResultDirectory ( cDirectory );
    cTool.InitResultFile ( "Testdata" );
    //cTool.StartHttpServer();

    cTool.ConfigureHw();

    // now it is time to use our own class, we can use the copy constructor inherited from SystemController or Tool
    // here goes our code!
    
    
    
    // stop the timer!
    t.stop();
    t.show ( "Time to Calibrate the system: " );



    if ( !batchMode ) cApp.Run();

    return 0;
}
