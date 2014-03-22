#ifndef _SHAPEWORLD_H_
#define _SHAPEWORLD_H_

#include <string>

#include <osg/Referenced>
#include <osg/Group>

#include <gdal/ogrsf_frmts.h>


class ShapeWorld : public osg::Referenced {
public:
	ShapeWorld() : m_minX(std::numeric_limits<double>::max()), m_maxX(std::numeric_limits<double>::min()), m_minY(std::numeric_limits<double>::max()), m_maxY(std::numeric_limits<double>::min()) {};
	bool loadShapeFile(const std::string& filename);
protected:
	~ShapeWorld() {};
	void findWorldBounds(OGRLayer* inLayer);
	void findPolygonCenter(OGRLinearRing* linearRing, double& x, double& y);
	osg::ref_ptr<osg::Group> addBuilding(OGRLinearRing* linearRing, double cx, double cy);
	osg::ref_ptr<osg::Group> buildTile(OGRLayer* layer, double px, double py);
	
	double m_minX;
	double m_maxX;
	double m_minY;
	double m_maxY;

	double m_tilesize;
};
#endif