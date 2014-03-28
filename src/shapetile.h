/*
 * shapetile.h
 *
 *  Created on: March 22, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _SHAPEWORLD_H_
#define _SHAPEWORLD_H_

#include <string>

#include <gdal/ogrsf_frmts.h>

#include "polygon.h"

class ShapeTile : public osg::Referenced {
	public:
		ShapeTile() : m_minX(std::numeric_limits<double>::max()), m_maxX(std::numeric_limits<double>::min()), m_minY(std::numeric_limits<double>::max()), m_maxY(std::numeric_limits<double>::min()) {};
		bool load(const std::string& filename);
		PolygonVector polygons() const { return m_polygons; }
	protected:
		~ShapeTile() {};
		void findWorldBounds();
		void loadPolygons(OGRLayer* layer);
		osg::ref_ptr<Polygon> buildPolygon(OGRLinearRing* linearRing);

		double m_minX;
		double m_maxX;
		double m_minY;
		double m_maxY;

		PolygonVector m_polygons;
};

typedef std::vector< osg::ref_ptr<ShapeTile> > ShapeWorldVector;
typedef ShapeWorldVector::iterator ShapeWorldVectorIterator;

#endif