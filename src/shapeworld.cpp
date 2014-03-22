#include "shapeworld.h"

#include <osg/Notify>

bool ShapeWorld::loadShapeFile(const std::string& filename) {
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
	osg::notify(osg::ALWAYS) << "Number of buildings in shape file: " << numberOfBuildings << std::endl;
	
	//Find boundaries of layer features
	findWorldBounds(ogrLayer);

	//Reset before classifying
	ogrLayer->ResetReading();

	return true;
}

void ShapeWorld::findWorldBounds(OGRLayer* layer)
{
	OGRFeature* poFeature;
	while( ( poFeature = layer->GetNextFeature()) != NULL ) {
		OGRGeometry* poGeometry = poFeature->GetGeometryRef();

		if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon )	{
			OGRPolygon* poPolygon = static_cast<OGRPolygon *>(poGeometry);					
			OGRLinearRing* poLinearRing = poPolygon->getExteriorRing();

			double centerX = 0;
			double centerY = 0;

			findPolygonCenter(poLinearRing, centerX, centerY);
			

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
	}
	osg::notify(osg::ALWAYS) << "Found bound: X: " << m_minX <<"\t" << m_maxX << std::endl;
	osg::notify(osg::ALWAYS) << "             Y: " << m_minY <<"\t" << m_maxY << std::endl;
}

void ShapeWorld::findPolygonCenter(OGRLinearRing* linearRing, double& centerX, double& centerY) {
	// Get initial coordinates
	OGRPoint* poPoint = static_cast<OGRPoint*>(OGRGeometryFactory::createGeometry( wkbPoint ));
	linearRing->getPoint(0,poPoint);
	double minX = poPoint->getX();
	double maxX = minX;

	double minY = poPoint->getY();
	double maxY = minY;

	// Get bounding points of building
	for( int i=0; i < linearRing->getNumPoints(); ++i ) {
		linearRing->getPoint(i,poPoint);	

		double pointX = poPoint->getX();
		double pointY = poPoint->getY();

		if ( pointX > maxX )
		{
			maxX = pointX;
		}
		else if ( pointX < minX )
		{
			minX = pointX;
		}

		if ( pointY > maxY )
		{
			maxY = pointY;
		}
		else if ( pointY < minY )
		{
			minY = pointY;
		}
	}
	
	OGRGeometryFactory::destroyGeometry(poPoint);

	// Calculate center point of building
	centerX = ( minX + maxX ) / 2.0;
	centerY = ( minY + maxY ) / 2.0;
}