#ifndef _BUILDING_LIBRARY_H_
#define _BUILDING_LIBRARY_H_

#include <map>

#include <osg/Referenced>
#include <osg/Group>

#include "building.h"
#include "polygon.h"
#include "pugixml.hpp"


class BuildingLibrary : public osg::Referenced {
public:
	static BuildingLibrary& instance() {
		static BuildingLibrary instance;
		return instance;
	}
	
	void load(pugi::xml_node buildingsNode);
	osg::ref_ptr<osg::Group> buildingFromPolygon(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate);
protected:
	BuildingLibrary() {};
	~BuildingLibrary() {};
	BuildingLibrary(BuildingLibrary const&); // No copy constructor allowed for singleton
	BuildingLibrary& operator=(BuildingLibrary const&); // No assignment operator allowed for singleton

	bool addBuildingWithPerimeter(double perimeter, osg::ref_ptr<Building> building);
	osg::ref_ptr<Building> getBuildingFromPerimeter(double perimeter);
	
	BuildingMap m_buildingMap;
};
#endif