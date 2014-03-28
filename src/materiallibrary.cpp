/*
 * materiallibrary.h
 *
 *  Created on: March 28, 2014
 *      Author: Bjorn Blissing
 */

#include "materiallibrary.h"


void MaterialLibrary::load(pugi::xml_node materialsNode)
{
	// Load buildings sets
	for (pugi::xml_node materialNode = materialsNode.child("material"); materialNode; materialNode = materialNode.next_sibling("material")) {
		int materialId = materialNode.attribute("id").as_int(0);
		osg::ref_ptr<Material> material = new Material;
		material->load(materialNode);
		addMaterialWithId(materialId, material);
	}
}

bool MaterialLibrary::addMaterialWithId(int id, osg::ref_ptr<Material> material)
{
	if (id == 0) {
		osg::notify(osg::WARN) << "Warning: Material with id 0 is reserved for default material." << id << std::endl;
		return false;
	}

	if (m_materialMap.find(id) == m_materialMap.end()) {
		m_materialMap.insert(std::make_pair(id, material));
		return true;
	}

	osg::notify(osg::WARN) << "Warning: Library already contains material with id: " << id << std::endl;
	return false;
}