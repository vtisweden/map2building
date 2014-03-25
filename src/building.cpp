#include "building.h"

#include <osg/Notify>

void Building::load(pugi::xml_node buildingNode) {
	
	std::string type = buildingNode.attribute("type").as_string("");

	if (pugi::xml_node basementNode = buildingNode.child("basement")) {
		m_basementHeight = basementNode.attribute("height").as_double(0.0);
	}

	if (pugi::xml_node roofNode = buildingNode.child("roof")) {
		m_roofHeight = roofNode.attribute("height").as_double(0.0);	
	}

	if (pugi::xml_node windowNode = buildingNode.child("window")) {
		m_windowOffset = windowNode.attribute("offset").as_double(0.0);	
	}

	if (pugi::xml_node materialsNode = buildingNode.child("materials")) {
		// Load material sets
		for (pugi::xml_node materialNode = materialsNode.child("material"); materialNode; materialNode = materialNode.next_sibling("material")){
			MaterialSet materialSet;
			materialSet.wallMaterialId = materialNode.attribute("wall").as_uint();
			materialSet.windowMaterialId = materialNode.attribute("window").as_uint();
			materialSet.roofMaterialId = materialNode.attribute("roof").as_uint();
			materialSet.baseMaterialId = materialNode.attribute("basement").as_uint();
			m_materials.push_back(materialSet);
		}
	}

	osg::notify(osg::DEBUG_INFO) << "Building Type:     " << type << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Basement height:   " << m_basementHeight << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Roof height:       " << m_roofHeight << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Window offset:     " << m_windowOffset << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Material sets:     " << m_materials.size() << std::endl;
	osg::notify(osg::DEBUG_INFO) << std::endl;
}
