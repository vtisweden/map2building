/*
 * buildinglibrary.cpp
 *
 *  Created on: March 22, 2014
 *      Author: Bjorn Blissing
 */
#include "buildinglibrary.h"

#include <osg/notify>

osg::ref_ptr<Building> BuildingLibrary::getBuildingFromPerimeter(double perimeter)
{
	BuildingMapIterator it;
	osg::ref_ptr<Building> building = 0;

	for (it = m_buildingMap.begin(); it != m_buildingMap.end(); ++it) {
		building = (*it).second;

		if (perimeter > (*it).first) { break; }
	}

	return building;
}

void BuildingLibrary::load(pugi::xml_node buildingsNode)
{
	// Load buildings sets
	for (pugi::xml_node buildingNode = buildingsNode.child("building"); buildingNode; buildingNode = buildingNode.next_sibling("building")) {
		double perimeter = buildingNode.attribute("perimeter").as_double(0.0);
		osg::ref_ptr<Building> building = new Building;
		building->load(buildingNode);
		addBuildingWithPerimeter(perimeter, building);
	}
}

bool BuildingLibrary::addBuildingWithPerimeter(double perimeter, osg::ref_ptr<Building> building)
{
	if (m_buildingMap.find(perimeter) == m_buildingMap.end()) {
		m_buildingMap.insert(std::make_pair(perimeter, building));
		return true;
	}

	osg::notify(osg::WARN) << "Warning: Cannot add building. Perimeter already in use: " << perimeter << std::endl;
	return false;
}

osg::ref_ptr<osg::Group> BuildingLibrary::buildingFromPolygon(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate)
{
	osg::ref_ptr<Building> building = getBuildingFromPerimeter(polygon->perimeter());
	return building ? building->createFromPolygon(polygon, baseCoordinate) : 0;
}