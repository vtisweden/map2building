/*
 * building.h
 *
 *  Created on: March 22, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _BUILDING_H_
#define _BUILDING_H_

#include <vector>

#include <osg/Referenced>
#include <osg/Group>
#include <osg/Geode>
#include <osg/PolygonOffset>


#include "polygon.h"
#include "pugixml.hpp"

class MaterialSet : public osg::Referenced {
	public:
		MaterialSet() : m_wallMaterialId(0), m_windowMaterialId(0), m_roofMaterialId(0), m_baseMaterialId(0) {};
		void setWallMaterialId(unsigned int id) { m_wallMaterialId = id; }
		void setWindowMaterialId(unsigned int id) { m_windowMaterialId = id; }
		void setRoofMaterialId(unsigned int id) { m_roofMaterialId = id; }
		void setBaseMaterialId(unsigned int id) { m_baseMaterialId = id; }

		unsigned int wallMaterialId() const { return m_wallMaterialId; }
		unsigned int windowMaterialId() const { return m_windowMaterialId; }
		unsigned int roofMaterialId() const { return m_roofMaterialId; }
		unsigned int baseMaterialId() const { return m_baseMaterialId; }
	protected:
		~MaterialSet() {};
		unsigned int m_wallMaterialId;
		unsigned int m_windowMaterialId;
		unsigned int m_roofMaterialId;
		unsigned int m_baseMaterialId;
};

class Building : public osg::Referenced {
	public:
		Building();
		void load(pugi::xml_node buildingNode);
		osg::ref_ptr<osg::Group> createFromPolygon(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate);
		std::string type() const { return m_type; }
	protected:
		~Building() {};
		osg::ref_ptr<osg::Geode> buildBasement(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, osg::ref_ptr<MaterialSet> materialSet);
		osg::ref_ptr<osg::Geode> buildWalls(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, osg::ref_ptr<MaterialSet> materialSet);
		osg::ref_ptr<osg::Geode> buildWindows(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, osg::ref_ptr<MaterialSet> materialSet);
		osg::ref_ptr<osg::Geode> buildRoof(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, osg::ref_ptr<MaterialSet> materialSet);
		osg::ref_ptr<osg::Geometry> extrudePolygon(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, double height, osg::Vec4 color);
		void createVertexAndNormal(osg::Vec2 point1, osg::Vec2 point2, 
			double h1, double h2, 
			osg::ref_ptr<osg::Vec3Array> vertexArray, 
			osg::ref_ptr<osg::Vec3Array> normalArray, 
			osg::ref_ptr<osg::Vec2Array> textureArray,
			double& startU);
		void buildWindowQuad(osg::Vec2 point1, osg::Vec2 point2,
			double startHeight,
			osg::ref_ptr<osg::Vec3Array> vertexArray, 
			osg::ref_ptr<osg::Vec3Array> normalArray, 
			osg::ref_ptr<osg::Vec2Array> textureArray);
		osg::ref_ptr<MaterialSet> getRandomMaterialSet(osg::Vec2 point);

		double m_basementHeight;
		double m_floorHeight;
		unsigned int m_numberOfFloors;
		double m_windowHorizontalSpacing;
		double m_windowVerticalSpacing;
		double m_windowHeight;
		double m_windowWidth;
		std::string m_type;
		std::vector< osg::ref_ptr<MaterialSet> > m_materials;

		static osg::ref_ptr<osg::PolygonOffset> m_polygonOffset;
};

typedef std::map< double, osg::ref_ptr<Building> > BuildingMap;
typedef BuildingMap::iterator BuildingMapIterator;

#endif
