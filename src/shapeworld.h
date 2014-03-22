#ifndef _SHAPEWORLD_H_
#define _SHAPEWORLD_H_

#include <string>

#include <osg/Referenced>

#include <gdal/ogrsf_frmts.h>


class ShapeWorld : public osg::Referenced {
public:
	ShapeWorld() : m_minX(std::numeric_limits<double>::max()), m_maxX(std::numeric_limits<double>::min()), m_minY(std::numeric_limits<double>::max()), m_maxY(std::numeric_limits<double>::min()) {};
	bool loadShapeFile(const std::string& filename);
protected:
	~ShapeWorld() {};
	void findWorldBounds(OGRLayer* inLayer);
	void findPolygonCenter(OGRLinearRing* linearRing, double& x, double& y);
	
	double m_minX;
	double m_maxX;
	double m_minY;
	double m_maxY;

};
#endif