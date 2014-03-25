#include "worldbuilder.h"

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include "pugixml.hpp"

void WorldBuilder::loadConfiguration() {
	
	// Add path of configuration file to path list
	std::string directory = osgDB::getFilePath(m_filename);
	osgDB::FilePathList pathList = osgDB::getDataFilePathList();
	pathList.push_back(directory);
	osgDB::setDataFilePathList(pathList);

	// Read xml information
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(m_filename.c_str());

	if (pugi::xml_node configurationNode = doc.child("configuration")) {
		// Textures
		if (pugi::xml_node texturesNode = configurationNode.child("textures")) {
			m_textureLibrary = new TextureLibrary;
			m_textureLibrary->load(texturesNode);
		}

		// Buildings
		if (pugi::xml_node buildingsNode = configurationNode.child("buildings")) {
			m_buildingLibrary = new BuildingLibrary;
			m_buildingLibrary->load(buildingsNode);
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
		osg::notify(osg::WARN) << "Warning: Could not read configuration file:" << m_filename << std::endl;
	}
}