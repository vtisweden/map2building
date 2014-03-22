/*
 * main.cpp
 *
 *  Created on: March 21, 2014
 *      Author: Bjorn Blissing
 */

#include <osg/ArgumentParser>

#include "shapeworld.h"
 
int main( int argc, char** argv )
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);

	// Load configuration file

	// Load texture definitions

	// Load building definitions

	// Load shape file
	osg::ref_ptr<ShapeWorld> shapeWorld = new ShapeWorld;
	arguments.getApplicationUsage()->addCommandLineOption("-shape","Path for shape file");
	std::string shapeFile;
	arguments.read("-shape",shapeFile);
	shapeWorld->loadShapeFile(shapeFile);

	// Build houses from shape file information

	return 0;
}
