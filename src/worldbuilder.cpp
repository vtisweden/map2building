/*
 * worldbuilder.cpp
 *
 *  Created on: March 25, 2014
 *      Author: Bjorn Blissing
 */
#include "worldbuilder.h"

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/WriteFile>

#include "pugixml.hpp"
#include "heighttile.h"

void WorldBuilder::loadConfiguration() {
	
	// Add path of configuration file to path list
	std::string directory = osgDB::getFilePath(m_configFilename);
	osgDB::FilePathList pathList = osgDB::getDataFilePathList();
	pathList.push_back(directory);
	osgDB::setDataFilePathList(pathList);

	// Read xml information
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(m_configFilename.c_str());

	if (pugi::xml_node configurationNode = doc.child("configuration")) {
		// Textures
		if (pugi::xml_node texturesNode = configurationNode.child("textures")) {
			TextureLibrary::instance().load(texturesNode);
		}

		// Buildings
		if (pugi::xml_node buildingsNode = configurationNode.child("buildings")) {
			BuildingLibrary::instance().load(buildingsNode);
		}

		// Shapes
		if (pugi::xml_node shapesNode = configurationNode.child("shapes")) {
			for (pugi::xml_node shapeNode = shapesNode.child("shape"); shapeNode; shapeNode = shapeNode.next_sibling("shape")){
				std::string shapeFilename = shapeNode.attribute("filename").as_string("");
				if (!shapeFilename.empty()) {
					m_shapeFilenames.push_back(shapeFilename);
				}
			}
			osg::notify(osg::DEBUG_INFO) << "Number of shape files: " << m_shapeFilenames.size() << std::endl;
		}

		// Heights
		if (pugi::xml_node heightsNode = configurationNode.child("heights")) {
			for (pugi::xml_node heightNode = heightsNode.child("height"); heightNode; heightNode = heightNode.next_sibling("height")){
				std::string heightFilename = heightNode.attribute("filename").as_string("");
				if (!heightFilename.empty()) {
					m_heigthFilenames.push_back(heightFilename);
				}
			}
			osg::notify(osg::DEBUG_INFO) << "Number of height files: " << m_heigthFilenames.size() << std::endl;
		}
	} else {
		osg::notify(osg::WARN) << "Warning: Could not read configuration file:" << m_configFilename << std::endl;
	}
}

void WorldBuilder::buildWorld() {
	// Load height files
	osg::notify(osg::ALWAYS) << "Reading height file(s)..." << std::endl;
	std::vector<std::string>::iterator it;
	int filenumber = 0;
	for (it = m_heigthFilenames.begin(); it != m_heigthFilenames.end(); ++it) {
		osg::notify(osg::DEBUG_INFO) << "Reading height file: " << ++filenumber << " of " << m_heigthFilenames.size() << std::endl;
		std::string heightFilename = osgDB::findDataFile(*it);
		if (!heightFilename.empty()) {
			osg::ref_ptr<HeightTile> heightTile = new HeightTile();
			heightTile->load(heightFilename);
			m_heightTiles.push_back(heightTile);
		} else {
			osg::notify(osg::WARN) << "Warning: Could not find height data file: " << (*it) << std::endl;
		}
	}

	// Load shape files and populate height files with polygons
	osg::notify(osg::ALWAYS) << "Reading shape file(s)..." << std::endl;
	filenumber = 0;
	for (it = m_shapeFilenames.begin(); it != m_shapeFilenames.end(); ++it) {
		osg::notify(osg::DEBUG_INFO) << "Reading shape file: " << ++filenumber << " of " << m_shapeFilenames.size() << std::endl;
		std::string shapeFilename = osgDB::findDataFile(*it);
		if (!shapeFilename.empty()) {
			osg::ref_ptr<ShapeTile> shapeWorld = new ShapeTile();
			shapeWorld->load(shapeFilename);
			m_shapeTiles.push_back(shapeWorld);
		} else {
			osg::notify(osg::WARN) << "Warning: Could not find shape file: " << (*it) << std::endl;
		}
	}

	// Extract polygons from shape tiles
	osg::notify(osg::ALWAYS) << "Extracting polygons..." << std::endl;
	int tilenumber = 0;
	ShapeWorldVectorIterator shapeIt;
	for (shapeIt = m_shapeTiles.begin(); shapeIt != m_shapeTiles.end(); ++shapeIt) {
		osg::notify(osg::DEBUG_INFO) << "Processed shape tile: " << ++tilenumber << " of " << m_shapeTiles.size() << std::endl;
		osg::ref_ptr<ShapeTile> shapeWorld = (*shapeIt);
		addPolygonsToHeight(shapeWorld->polygons());	
	}

	// Update height values for polygons
	osg::notify(osg::ALWAYS) << "Update heights in polygons..." << std::endl;
	tilenumber = 0;
	HeightTileVectorIterator heightIt;
	for (heightIt = m_heightTiles.begin(); heightIt != m_heightTiles.end(); ++heightIt) {
		osg::notify(osg::DEBUG_INFO) << "Update height in tile: " << ++tilenumber << " of " << m_heightTiles.size() << std::endl;
		osg::ref_ptr<HeightTile> heightTile = (*heightIt);
		heightTile->updatePolygonHeight();
	}

	// Use OSG binary format as default
	std::string fileExtension("osgb");
	std::string path = osgDB::getCurrentWorkingDirectory();
	std::string filename("buildings");
	if (!m_outputFilename.empty()) {
		if (osgDB::getLowerCaseFileExtension(m_outputFilename) != "osgb") {
			osg::notify(osg::WARN) << "Warning: Unknown file extension specified. Reverting to .osgb file format." << std::endl;
		}
		filename = osgDB::getStrippedName(m_outputFilename);
		path = osgDB::getFilePath(m_outputFilename);
	}

	// Build complete filename
	std::stringstream completeFilename;
	completeFilename << path;
	completeFilename << osgDB::getNativePathSeparator();
	completeFilename << filename;
	completeFilename << ".";
	completeFilename << fileExtension;

	// Build balanced tree for polygons
	osg::ref_ptr<PolygonTree> polygonTree = new PolygonTree(path, filename);
	
	osg::notify(osg::ALWAYS) << "Build polygon tree..." << std::endl;
	for (heightIt = m_heightTiles.begin(); heightIt != m_heightTiles.end(); ++heightIt) {
		osg::notify(osg::DEBUG_INFO) << "Getting polygons from height tile: " << ++tilenumber << " of " << m_heightTiles.size() << std::endl;
		osg::ref_ptr<HeightTile> heightTile = (*heightIt);
		PolygonVector polygons = heightTile->polygons();
		if (!polygons.empty()) {
			PolygonVectorIterator polygonIt;
			for (polygonIt = polygons.begin(); polygonIt != polygons.end(); ++polygonIt) {
				osg::ref_ptr<Polygon> polygon = (*polygonIt);
				polygonTree->addPolygon(polygon);
			}
		}
	}

	// Add polygons that are outside height tiles to tree
	if (!m_polygonsOutsideTiles.empty()) {
		osg::notify(osg::ALWAYS) << "Polygons outside height tiles: " << m_polygonsOutsideTiles.size() << std::endl;
		PolygonVectorIterator polygonIt;
		for (polygonIt = m_polygonsOutsideTiles.begin(); polygonIt != m_polygonsOutsideTiles.end(); ++polygonIt) {
			osg::ref_ptr<Polygon> polygon = (*polygonIt);
			polygonTree->addPolygon(polygon);
		}
	}

	// Balance polygon tree
	osg::notify(osg::ALWAYS) << "Balancing polygon tree..." << std::endl;
	polygonTree->balance();

	osg::notify(osg::ALWAYS) << "Create buildings from polygon tree..." << std::endl;
	osg::ref_ptr<osg::Group> worldGroup = polygonTree->createBuildingTree();
	
	osg::notify(osg::ALWAYS) << "Write models as output..." << std::endl;

	osgDB::writeNodeFile(*worldGroup, completeFilename.str());
}

void WorldBuilder::addPolygonsToHeight(PolygonVector polygons) {
	PolygonVectorIterator polygonIt;
	int polygonNumber = 0;
	for (polygonIt = polygons.begin(); polygonIt != polygons.end(); ++polygonIt) {
		osg::notify(osg::DEBUG_INFO) << "\tProcessed polygon: " << ++polygonNumber << " of " << polygons.size() << std::endl;
		osg::ref_ptr<Polygon> polygon = (*polygonIt);
		bool foundMatchingTile = false;
		HeightTileVectorIterator heightIt;
		for (heightIt = m_heightTiles.begin(); heightIt != m_heightTiles.end(); ++heightIt) {
			osg::ref_ptr<HeightTile> heightTile = (*heightIt);
			if (heightTile->addPolygon(polygon)) {
				foundMatchingTile = true;
				break;
			}
		}
		if (!foundMatchingTile) {
			m_polygonsOutsideTiles.push_back(polygon);
		}
	}

} 