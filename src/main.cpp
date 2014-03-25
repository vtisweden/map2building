/*
 * main.cpp
 *
 *  Created on: March 21, 2014
 *      Author: Bjorn Blissing
 */

#include <osg/ArgumentParser>
#include "worldbuilder.h"

int main( int argc, char** argv )
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);

	

	// Load configuration file
	arguments.getApplicationUsage()->addCommandLineOption("-config","Path for configuration file");
	std::string configFile;
	arguments.read("-config",configFile);
	if (!configFile.empty()) {
		osg::ref_ptr<WorldBuilder> worldBuilder = new WorldBuilder(configFile);
		worldBuilder->loadConfiguration();
		worldBuilder->buildWorld();
	}

	return 0;
}
