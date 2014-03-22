#ifndef _BUILDING_LIBRARY_H_
#define _BUILDING_LIBRARY_H_

#include <map>

#include <osg/Referenced>
#include <osg/ref_ptr>

#include "building.h"
#include "pugixml.hpp"


class BuildingLibrary : public osg::Referenced {
public:
	BuildingLibrary() {};
	void load(pugi::xml_node buildingsNode);
	osg::ref_ptr<Building> getBuildingFromPerimeter(double perimeter) const;
protected:
	~BuildingLibrary() {};

	bool addBuildingWithPerimeter(double perimeter, osg::ref_ptr<Building> building);
	
	std::map<double, osg::ref_ptr<Building> > m_buildingMap;
};
#endif