/*
 * polygon.h
 *
 *  Created on: March 25, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <limits>

#include <osg/Referenced>
#include <osg/Vec2>
#include <osg/Array>
#include <osg/BoundingBox>
#include <osg/Group>

class Polygon : public osg::Referenced {
public:
	Polygon() : m_points(), m_center(osg::Vec2()), m_height(0.0), m_perimeter(0.0), m_dirtyPoints(true) {}
	Polygon(osg::ref_ptr<osg::Vec2Array> points) : m_points(false), m_center(osg::Vec2()), m_height(0.0), m_perimeter(0.0), m_dirtyPoints(true) {};
	void push_back(osg::Vec2 point);

	osg::Vec2 center();
	double perimeter();
	
	void setHeight(double height) { m_height = height; }
	double height() const { return m_height; }
	osg::ref_ptr<osg::Vec2Array> points() const { return m_points; }

protected:
	~Polygon() {};
	void calculateCenterAndPerimeter();
	
	osg::ref_ptr<osg::Vec2Array> m_points;
	osg::Vec2 m_center;
	double m_height;
	double m_perimeter;
	bool m_dirtyPoints;
};

typedef std::vector< osg::ref_ptr<Polygon> > PolygonVector;
typedef PolygonVector::iterator PolygonVectorIterator;


class PolygonTree : public osg::Referenced {
public: 
	PolygonTree(size_t bucketSize = 100) : m_northEast(0), m_southEast(0), m_southWest(0), m_northWest(0), 
		m_minX(std::numeric_limits<double>::max()),
		m_maxX(std::numeric_limits<double>::min()),
		m_minY(std::numeric_limits<double>::max()),
		m_maxY(std::numeric_limits<double>::min()),
		m_bucketSize(bucketSize) {};
	void addPolygon(osg::ref_ptr<Polygon> polygon);
	void setBucketSize(size_t bucketSize);
	void balance();
	osg::ref_ptr<osg::Group> createBuildingTree(osg::Vec2 parentTileOrigo = osg::Vec2(0.0, 0.0));
protected:
	~PolygonTree() {};

	osg::ref_ptr<PolygonTree> m_northEast;
	osg::ref_ptr<PolygonTree> m_southEast;
	osg::ref_ptr<PolygonTree> m_southWest;
	osg::ref_ptr<PolygonTree> m_northWest;

	double m_minX;
	double m_maxX;
	double m_minY;
	double m_maxY;

	size_t m_bucketSize;
	PolygonVector m_polygons;
};

#endif