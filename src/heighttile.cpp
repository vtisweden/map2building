/*
 * heighttile.cpp
 *
 *  Created on: March 25, 2014
 *      Author: Bjorn Blissing
 */
#include "heighttile.h"

HeightTile::HeightTile() : m_filename(""),
	m_rasterSizeX(0),
	m_rasterSizeY(0),
	m_minX(0.0),
	m_maxX(0.0),
	m_minY(0.0),
	m_maxY(0.0),
	m_geotransform(),
	m_invgeotransform(),
	m_validExtents(false) {}

void HeightTile::load(std::string filename)
{
	m_filename = filename;
	getExtents();
}

bool HeightTile::addPolygon(osg::ref_ptr<Polygon> polygon)
{
	if (!m_validExtents) {
		return false;
	}

	// Check if polygon center is withing tile extents
	osg::Vec2 center = polygon->center();

	if (center.x() >= m_minX && center.x() < m_maxX && center.y() >= m_minY && center.y() < m_maxY) {
		// if it is add to list and return true
		m_polygons.push_back(polygon);
		return true;
	}

	// Otherwise return false
	return false;
}

void HeightTile::getExtents()
{
	// Try to open file
	GDALAllRegister();
	GDALDataset* dataset = (GDALDataset*) GDALOpen( m_filename.c_str(), GA_ReadOnly );

	if ( dataset == NULL ) {
		osg::notify(osg::WARN) << "Warning: Unable to open file: " << m_filename << std::endl;
		m_validExtents = false;
		return;
	}

	m_rasterSizeX = dataset->GetRasterXSize();
	m_rasterSizeY = dataset->GetRasterYSize();
	double width = (double) m_rasterSizeX;
	double height = (double) m_rasterSizeY;
	dataset->GetGeoTransform(m_geotransform);
	GDALInvGeoTransform(m_geotransform, m_invgeotransform);
	// Get extents and set extents to valid
	m_minX = m_geotransform[0];
	m_maxX = m_geotransform[0] + width*m_geotransform[1] + height*m_geotransform[2];
	m_minY = m_geotransform[3] + width*m_geotransform[4] + height*m_geotransform[5];
	m_maxY = m_geotransform[3];
	osg::notify(osg::DEBUG_INFO) << "Extents: X-min: " << m_minX <<"\tX-max: " << m_maxX << std::endl;
	osg::notify(osg::DEBUG_INFO) << "         Y-min: " << m_minY <<"\tY-max: " << m_maxY << std::endl;
	// Close file
	GDALClose(dataset);
	m_validExtents = true;
}

void HeightTile::updatePolygonHeight()
{
	// If there is no polygons there is no need to open any files
	if (m_polygons.empty()) { return; }

	// Try to open file
	GDALAllRegister();
	GDALDataset* dataset = (GDALDataset*) GDALOpen( m_filename.c_str(), GA_ReadOnly );

	if ( dataset == NULL ) {
		osg::notify(osg::WARN) << "Warning: Unable to open file: " << m_filename << std::endl;
		return;
	}

	// Get raster band containing height
	GDALRasterBand* rasterBand = dataset->GetRasterBand( 1 );
	// Get height for every polygon in list
	PolygonVectorIterator it;

	for (it = m_polygons.begin(); it != m_polygons.end(); ++it) {
		osg::ref_ptr<Polygon> polygon = (*it);
		// Get height at center pos;
		double height = getInterpolatedValue(rasterBand, polygon->center().x(), polygon->center().y());
		polygon->setHeight(height);
	}

	// Close file
	GDALClose(dataset);
}

double HeightTile::getInterpolatedValue(GDALRasterBand* band, double x, double y)
{
	double row = 0.0;
	double column = 0.0;
	// Transform from lat & long to row & column.
	GDALApplyGeoTransform(m_invgeotransform, x, y, &column, &row);
	double result = 0.0;
	int column_int = (int)osg::round(column);
	int row_int = (int)osg::round(row);

	// Clamp to edge
	if (column_int >= m_rasterSizeX) { column_int = (m_rasterSizeX - 1); }

	if (row_int >= m_rasterSizeY) { row_int = (m_rasterSizeY - 1); }

	if (column_int < 0) { column_int = 0; }

	if (row_int < 0) { row_int = 0; }

	band->RasterIO(GF_Read, column_int, row_int, 1, 1, &result,1, 1, GDT_Float64, 0, 0);
	// Assume that we always get valid values
	// It would probably be good to add some error handling
	return result;
}
