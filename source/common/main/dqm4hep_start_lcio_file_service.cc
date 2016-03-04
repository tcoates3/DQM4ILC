  /// \file dqm4hep_start_lcio_file_service.cc
/*
 *
 * dqm4hep_start_lcio_file_service.cc main source file template automatically generated
 * Creation date : mer. nov. 5 2014
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */

#include <iostream>

// -- lcio headers
#include "EVENT/LCIO.h"
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"

// -- dqm4hep
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/DQMLogging.h"
#include "dqm4hep/DQMPluginManager.h"
#include "dqm4hep/DQMDimEventClient.h"

// -- dqm4ilc headers
#include "dqm4ilc/DQMLcioReaderListener.h"
#include "dqm4ilc/DQMLCEventStreamer.h"

// -- tclap headers
#include "tclap/CmdLine.h"
#include "tclap/Arg.h"

// -- dim headers
#include "dis.hxx"

using namespace std;
using namespace dqm4hep;

int main(int argc, char* argv[])
{
	DQM4HEP::screenSplash();

	std::string cmdLineFooter = "Please report bug to <rete@ipnl.in2p3.fr>";
	TCLAP::CmdLine *pCommandLine = new TCLAP::CmdLine(cmdLineFooter, ' ', DQM4HEP_VERSION_STR);
	std::string log4cxx_file = std::string(DQMCore_DIR) + "/conf/defaultLoggerConfig.xml";

	TCLAP::ValueArg<unsigned int> sleepTimeArg(
				  "t"
				 , "sleep-time"
				 , "The sleep time between each event (in usec)"
				 , false
				 , 100000
				 , "unsigned int");
	pCommandLine->add(sleepTimeArg);

	TCLAP::ValueArg<std::string> lcioFileNamesArg(
				  "f"
				 , "lcio-files"
				 , "The list of lcio files to process (separated by a ':' character)"
				 , true
				 , ""
				 , "string");
	pCommandLine->add(lcioFileNamesArg);

	TCLAP::ValueArg<std::string> eventCollectorNameArg(
				  "c"
				 , "collector-name"
				 , "The event collector name in which the event will be published"
				 , true
				 , ""
				 , "string");
	pCommandLine->add(eventCollectorNameArg);

	TCLAP::SwitchArg rewindArg(
				  "r"
				 , "rewind"
				 , "Whether the lcio file(s) should be rewinded"
				 , false);
	pCommandLine->add(rewindArg);

	TCLAP::ValueArg<std::string> loggerConfigArg(
				  "l"
				 , "logger-config"
				 , "The xml logger file to configure log4cxx"
				 , false
				 , log4cxx_file
				 , "string");
	pCommandLine->add(loggerConfigArg);

	std::vector<std::string> allowedLevels;
	allowedLevels.push_back("INFO");
	allowedLevels.push_back("WARN");
	allowedLevels.push_back("DEBUG");
	allowedLevels.push_back("TRACE");
	allowedLevels.push_back("ERROR");
	allowedLevels.push_back("FATAL");
	allowedLevels.push_back("OFF");
	allowedLevels.push_back("ALL");
	TCLAP::ValuesConstraint<std::string> allowedLevelsContraint( allowedLevels );

	TCLAP::ValueArg<std::string> verbosityArg(
				  "v"
				 , "verbosity"
				 , "The verbosity level used for this application"
				 , false
				 , "INFO"
				 , &allowedLevelsContraint);
	pCommandLine->add(verbosityArg);

	TCLAP::SwitchArg simulateSpillArg(
				  "s"
				 , "simulate-spill"
				 , "Whether a spill structure has to be simulated using getTimeStamp() of LCEvents"
				 , false);
	pCommandLine->add(simulateSpillArg);

	// parse command line
	pCommandLine->parse(argc, argv);

	log4cxx_file = loggerConfigArg.getValue();
	log4cxx::xml::DOMConfigurator::configure(log4cxx_file);

	if( verbosityArg.isSet() )
		dqmMainLogger->setLevel( log4cxx::Level::toLevel( verbosityArg.getValue() ) );

	bool shouldRewind = rewindArg.getValue();
	std::vector<std::string> lcioInputFiles;
	DQM4HEP::tokenize(lcioFileNamesArg.getValue(), lcioInputFiles, ":");

	// file reader
	IO::LCReader *pLCReader = IOIMPL::LCFactory::getInstance()->createLCReader(1);

	// event collector client
	DQMEventClient *pEventClient = new DQMDimEventClient();

	DQMLCEventStreamer *pEventStreamer = new DQMLCEventStreamer();
	pEventClient->setEventStreamer(pEventStreamer);

	pEventClient->setCollectorName(eventCollectorNameArg.getValue());

	// lcio file listener
	DQMLcioReaderListener *pListener = new DQMLcioReaderListener(pLCReader);
	pListener->setSimulateSpill(simulateSpillArg.getValue());
	pListener->setEventClient(pEventClient);
	pListener->setSleepTime(sleepTimeArg.getValue());

	while(1)
	{
		try
		{
		   pLCReader->open(lcioInputFiles);
		   pLCReader->readStream();
		   pLCReader->close();
		}
		catch(StatusCodeException &exception)
		{
			LOG4CXX_ERROR( dqmMainLogger , "StatusCodeException caught while reading stream : " << exception.toString() );

			pLCReader->close();
			shouldRewind = false;
			break;
		}
		catch(std::exception & exception)
		{
			LOG4CXX_ERROR( dqmMainLogger , "std::exception caught while reading stream : " << exception.what() );

			pLCReader->close();
			shouldRewind = false;
			break;
		}

		if(shouldRewind)
		{
			LOG4CXX_INFO( dqmMainLogger , "Rewinding lcio files" );
		}
		else
		{
			LOG4CXX_INFO( dqmMainLogger , "Exiting lcio file service ..." );
			break;
		}

	}

	delete pEventClient;
	delete pListener;
	delete pLCReader;
	delete pCommandLine;

	return 0;
}
