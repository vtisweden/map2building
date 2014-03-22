#include "shapeworld.h"

#include <sstream>

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

	m_tilesize = 5000.0;
	int tiles = 0;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	for (double px = m_minX; px < m_maxX; px += m_tilesize) {
		for (double py = m_minY; py < m_maxY; py += m_tilesize) {
			tiles++;
			root->addChild(buildTile(ogrLayer, px, py));
		}
	}
	osg::notify(osg::ALWAYS) << "Number of tiles: " << tiles << std::endl;

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

osg::ref_ptr<osg::Group> ShapeWorld::buildTile(OGRLayer* layer, double px, double py) {

	// Calculate center point of tile
	//double centerX = px + m_tilesize / 2.0;
	//double centerY = px + m_tilesize / 2.0;

	osg::ref_ptr<osg::Group> tileGroup = new osg::Group;	
	std::stringstream tilename;
	tilename.clear();
	tilename << "Tile: " << px << ":" << py;
	tileGroup->setName(tilename.str());

	//Reset before classifying
	layer->ResetReading();

	int numberOfBuildings = layer->GetFeatureCount();
	int buildingInTile = 0;
	for (int h = 0; h < numberOfBuildings; ++h) {
		OGRFeature* poFeature;
		poFeature = layer->GetFeature( h );

		OGRGeometry *poGeometry;			
		poGeometry = poFeature->GetGeometryRef();

		OGRPolygon *poPolygon = (OGRPolygon *) poGeometry;					
		OGRLinearRing *poLinearRing = poPolygon->getExteriorRing();

		double cx = 0.0;
		double cy = 0.0;
		findPolygonCenter(poLinearRing, cx, cy);

		//Check if found building is within the tile
		if ((cx >= px) && (cx < px + m_tilesize) && (cy >= py) && (cy < py + m_tilesize) ) {
			osg::ref_ptr<osg::Group> building = addBuilding(poLinearRing, cx, cy);
			tileGroup->addChild(building);
			buildingInTile++;
			
		} else {
			//osg::notify(osg::ALWAYS) << "\tOutside" << std::endl;
		}
		
	}
	osg::notify(osg::ALWAYS) << "Building in tile: " << buildingInTile << std::endl;

	return tileGroup;
}

osg::ref_ptr<osg::Group> ShapeWorld::addBuilding(OGRLinearRing* linearRing, double cx, double cy) {
	//Add buildings to node
	OGRPoint* point = static_cast<OGRPoint*>(OGRGeometryFactory::createGeometry( wkbPoint ));

	osg::ref_ptr<osg::Vec3Array> floorCoord = new osg::Vec3Array;

	for(int i=0; i<linearRing->getNumPoints(); ++i)
	{
		linearRing->getPoint(i, point);	

		float newX = point->getX() - cx;
		float newY = point->getY() - cy;
		floorCoord->push_back(osg::Vec3( newX, 0.0, newY )); 
	}
	
	OGRGeometryFactory::destroyGeometry(point);

	//return BuildingLibrary::createBuilding(osg::Vec3Array floorCoord);
	return 0;
}
