/*
 * main.cpp
 *
 *  Created on: March 21, 2014
 *      Author: Bjorn Blissing
 */

#include <osg/ArgumentParser>
#include "worldbuilder.h"
#include <iostream>

int main( int argc, char** argv )
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);
	// Load configuration file
	arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" application is utility tools which can be used to generate buildings for geospatial terrain databases.");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
	arguments.getApplicationUsage()->addCommandLineOption("-c  or --config","Path for configuration file");
	arguments.getApplicationUsage()->addCommandLineOption("-o  or --output","Path output filename");
	arguments.getApplicationUsage()->addCommandLineOption("-h  or --help","Display this information");
	arguments.getApplicationUsage()->addCommandLineOption("--max-buildings","Maximum of buildings per file [default: 100]");

	// if user requests help write it out to cout.
	if (arguments.argc() < 2 ||arguments.read("-h") || arguments.read("--help")) {
		arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
		return 1;
	}

	std::string configFile;
	std::string outputFile;
	int maxBuildingsPerFile = 0;
	arguments.read("--config", configFile);
	arguments.read("-c", configFile);
	arguments.read("--output", outputFile);
	arguments.read("-o", outputFile);
	arguments.read("--max-buildings", maxBuildingsPerFile);
	osg::ref_ptr<WorldBuilder> worldBuilder = new WorldBuilder();

	if (!outputFile.empty()) {
		worldBuilder->setOutputFilename(outputFile);
	}

	if (maxBuildingsPerFile > 0) {
		worldBuilder->setMaximumBuildingsPerFile(maxBuildingsPerFile);
	}

	if (!configFile.empty()) {
		worldBuilder->setConfigFilename(configFile);
		worldBuilder->loadConfiguration();
		worldBuilder->buildWorld();
	}

	return 0;
}
