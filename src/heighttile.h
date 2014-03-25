#ifndef _HEIGHTTILE_H_
#define _HEIGHTTILE_H_

#include <string>

#include <gdal/gdal_priv.h>

#include "polygon.h"

class HeightTile : public osg::Referenced {
public:
	HeightTile(std::string filename);
	bool addPolygon(osg::ref_ptr<Polygon> polygon);
	std::vector < osg::ref_ptr<Polygon> > getPolygonList() const { return m_polygons; }
	void updatePolygonHeight();
protected:
	~HeightTile() {};
	void getExtents();
	
	double getInterpolatedValue(GDALRasterBand* band, double x, double y);


	std::string m_filename;
	double m_minX;
	double m_maxX;
	double m_minY;
	double m_maxY;
	double m_geotransform[6];
	double m_invgeotransform[6];
	bool m_validExtents;

	std::vector< osg::ref_ptr<Polygon> > m_polygons;
};

#endif
