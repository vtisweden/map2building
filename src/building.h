#ifndef _BUILDING_H_ 
#define _BUILDING_H_

#include <vector>

#include <osg/Referenced>
#include <osg/Group>
#include <osg/Geode>

#include "polygon.h"
#include "pugixml.hpp"

struct MaterialSet {
	unsigned int wallMaterialId;
	unsigned int windowMaterialId;
	unsigned int roofMaterialId;
	unsigned int baseMaterialId;
};

class Building : public osg::Referenced {
public:
	Building() : m_roofHeight(0.0), m_basementHeight(0.0), m_windowOffset(0.0) {};
	void load(pugi::xml_node buildingNode);
	osg::ref_ptr<osg::Group> createFromPolygon(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate);
protected:
	~Building() {};
	osg::ref_ptr<osg::Geode> buildBasement(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate);
	osg::ref_ptr<osg::Geometry> buildWall(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, double height);

	double m_roofHeight;
	double m_basementHeight;
	double m_windowOffset;
	std::vector<MaterialSet> m_materials;
};

typedef std::map< double, osg::ref_ptr<Building> > BuildingMap;
typedef BuildingMap::iterator BuildingMapIterator;

#endif
