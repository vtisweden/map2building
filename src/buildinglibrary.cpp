#include "buildinglibrary.h"

#include <osg/notify>

osg::ref_ptr<Building> BuildingLibrary::getBuildingFromPerimeter(double perimeter) const {
	std::map<double, osg::ref_ptr<Building> >::const_iterator it;
	osg::ref_ptr<Building> building = 0;
	for (it != m_buildingMap.begin(); it != m_buildingMap.end(); ++it) {
		if (perimeter > (*it).first)
			break;
		building = (*it).second;
	}
	
	return building;
}

void BuildingLibrary::load(pugi::xml_node buildingsNode) {
	// Load buildings sets
	for (pugi::xml_node buildingNode = buildingsNode.child("building"); buildingNode; buildingNode = buildingNode.next_sibling("building")){
		double perimeter = buildingNode.attribute("perimeter").as_double(0.0);
		osg::ref_ptr<Building> building = new Building;
		building->load(buildingNode);
		addBuildingWithPerimeter(perimeter, building);
	}
}

bool BuildingLibrary::addBuildingWithPerimeter(double perimeter, osg::ref_ptr<Building> building) {
	if (m_buildingMap.find(perimeter) == m_buildingMap.end()) {
		m_buildingMap.insert(std::make_pair(perimeter, building));
		return true;
	}
	osg::notify(osg::WARN) << "Warning: Cannot add building. Perimeter already in use: " << perimeter << std::endl;
	return false;
}