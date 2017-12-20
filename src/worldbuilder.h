/*
 * worldbuilder.h
 *
 *  Created on: March 25, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _WORLDBUILDER_H_
#define _WORLDBUILDER_H_

#include "buildinglibrary.h"
#include "texturelibrary.h"
#include "heighttile.h"
#include "shapetile.h"

class WorldBuilder : public osg::Referenced {
	public:
		WorldBuilder() : m_configFilename(""),
			m_outputFilename(""),
			m_maxBuildingsPerFile(100) {};
		void setConfigFilename(std::string configFilename) { m_configFilename = configFilename; }
		void setOutputFilename(std::string outputFilename) { m_outputFilename = outputFilename; }
		void setMaximumBuildingsPerFile(int number) { m_maxBuildingsPerFile = number; }
		void loadConfiguration();
		void buildWorld();
	protected:
		~WorldBuilder() {}

		void addPolygonsToHeight(PolygonVector polygons);

		std::string m_configFilename;
		std::string m_outputFilename;

		int m_maxBuildingsPerFile;

		std::vector<std::string> m_shapeFilenames;
		std::vector<std::string> m_heigthFilenames;

		HeightTileVector m_heightTiles;
		ShapeWorldVector m_shapeTiles;
		PolygonVector m_polygonsOutsideTiles;
};

#endif
