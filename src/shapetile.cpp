/*
 * shapetile.cpp
 *
 *  Created on: March 22, 2014
 *      Author: Bjorn Blissing
 */
#include "shapetile.h"

#include <sstream>

#include <osg/Notify>

bool ShapeTile::load(const std::string& filename) {
	// Register all format drivers needed
	OGRRegisterAll();

	OGRDataSource* ogrData = OGRSFDriverRegistrar::Open( filename.c_str(), FALSE );

	if (ogrData == NULL) {
		osg::notify(osg::WARN) << "Warning: Unable to load shape file: " << filename << std::endl;
		return false;
	}

	// Get Layer 0
	OGRLayer* ogrLayer = ogrData->GetLayer(0);
	
	// Reset before calculate bounds
	ogrLayer->ResetReading();	

	int	numberOfBuildings = ogrLayer->GetFeatureCount();
	osg::notify(osg::DEBUG_INFO) << "Number of buildings in shape file: " << numberOfBuildings << std::endl;
	
	// Load polygons
	loadPolygons(ogrLayer);

	//Find boundaries of layer features
	findWorldBounds();

	return true;
}

void ShapeTile::findWorldBounds()
{
	PolygonVectorIterator it;
	for (it = m_polygons.begin(); it != m_polygons.end(); ++it) {
		osg::ref_ptr<Polygon> polygon = (*it);
		double centerX = polygon->center().x();
		double centerY = polygon->center().y();

		// Find max and min of X-coordinate
		if ( centerX > m_maxX )	{
			m_maxX = centerX;
		} else if ( centerX < m_minX ) {
			m_minX = centerX;
		}

		// Find max and min of Y-coordinate
		if ( centerY > m_maxY )	{
			m_maxY = centerY;
		} else if ( centerY < m_minY ) {
			m_minY = centerY;
		}
	}

	osg::notify(osg::DEBUG_INFO) << "Found bound: X: " << m_minX <<"\t" << m_maxX << std::endl;
	osg::notify(osg::DEBUG_INFO) << "             Y: " << m_minY <<"\t" << m_maxY << std::endl;
}

void ShapeTile::loadPolygons(OGRLayer* layer) {
	//Reset before classifying
	layer->ResetReading();

	int numberOfBuildings = layer->GetFeatureCount();
	for (int h = 0; h < numberOfBuildings; ++h) {
		OGRFeature* ogrFeature;
		ogrFeature = layer->GetFeature( h );

		OGRGeometry* ogrGeometry;			
		ogrGeometry = ogrFeature->GetGeometryRef();

		OGRPolygon* ogrPolygon = (OGRPolygon *) ogrGeometry;					
		OGRLinearRing* ogrLinearRing = ogrPolygon->getExteriorRing();

		osg::ref_ptr<Polygon> polygon = buildPolygon(ogrLinearRing);
		
		m_polygons.push_back(polygon);
	}
}

osg::ref_ptr<Polygon> ShapeTile::buildPolygon(OGRLinearRing* linearRing) {
	osg::ref_ptr<Polygon> polygon = new Polygon;
	//Add buildings to node
	OGRPoint* point = static_cast<OGRPoint*>(OGRGeometryFactory::createGeometry( wkbPoint ));

	for(int i=0; i<linearRing->getNumPoints(); ++i)
	{
		linearRing->getPoint(i, point);	
		polygon->push_back(osg::Vec2( point->getX(), point->getY())); 
	}

	OGRGeometryFactory::destroyGeometry(point);

	return polygon;
}

