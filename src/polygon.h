#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <osg/Referenced>
#include <osg/Vec2>
#include <osg/Array>
#include <osg/BoundingBox>

class Polygon : public osg::Referenced {
public:
	Polygon() : m_points(), m_center(osg::Vec2()), m_perimeter(0.0), m_dirtyPoints(true) {}
	Polygon(osg::ref_ptr<osg::Vec2Array> points) : m_points(false), m_center(osg::Vec2()), m_perimeter(0.0), m_dirtyPoints(true) {};
	void push_back(osg::Vec2 point);

	osg::Vec2 center();
	double perimeter();
	
	void setHeight(double height) { m_height = height; }
	double height() const { return m_height; }
protected:
	~Polygon() {};
	void calculateCenterAndPerimeter();
	
	osg::ref_ptr<osg::Vec2Array> m_points;
	osg::Vec2 m_center;
	double m_height;
	double m_perimeter;
	bool m_dirtyPoints;
};


#endif