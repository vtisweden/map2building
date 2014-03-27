#include "building.h"

#include <osg/Notify>
#include <osg/Geometry>
#include <osgUtil/Tessellator>
#include <osgUtil/SmoothingVisitor>

void Building::load(pugi::xml_node buildingNode) {
	
	std::string type = buildingNode.attribute("type").as_string("");

	if (pugi::xml_node basementNode = buildingNode.child("basement")) {
		m_basementHeight = basementNode.attribute("height").as_double(0.0);
	}

	if (pugi::xml_node roofNode = buildingNode.child("roof")) {
		m_roofHeight = roofNode.attribute("height").as_double(0.0);	
	}

	if (pugi::xml_node windowNode = buildingNode.child("window")) {
		m_windowOffset = windowNode.attribute("offset").as_double(0.0);	
	}

	if (pugi::xml_node materialsNode = buildingNode.child("materials")) {
		// Load material sets
		for (pugi::xml_node materialNode = materialsNode.child("material"); materialNode; materialNode = materialNode.next_sibling("material")){
			MaterialSet materialSet;
			materialSet.wallMaterialId = materialNode.attribute("wall").as_uint();
			materialSet.windowMaterialId = materialNode.attribute("window").as_uint();
			materialSet.roofMaterialId = materialNode.attribute("roof").as_uint();
			materialSet.baseMaterialId = materialNode.attribute("basement").as_uint();
			m_materials.push_back(materialSet);
		}
	}

	osg::notify(osg::DEBUG_INFO) << "Building Type:     " << type << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Basement height:   " << m_basementHeight << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Roof height:       " << m_roofHeight << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Window offset:     " << m_windowOffset << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Material sets:     " << m_materials.size() << std::endl;
	osg::notify(osg::DEBUG_INFO) << std::endl;
}

osg::ref_ptr<osg::Group> Building::createFromPolygon(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate) {
	osg::ref_ptr<osg::Group> building = new osg::Group;
	// Build basement
	building->addChild(buildBasement(polygon, baseCoordinate));
	// Build walls
	building->addChild(buildWalls(polygon, baseCoordinate));
	// Build windows
	
	
	return building;
}

osg::ref_ptr<osg::Geode> Building::buildBasement(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate) {
	osg::ref_ptr<osg::Geode> basementGeode = new osg::Geode;
	osg::Vec4 basementColor = osg::Vec4(1.0, 1.0, 1.0, 1.0);
	osg::ref_ptr<osg::Geometry> basementGeometry = buildWall(polygon, baseCoordinate, -m_basementHeight, basementColor);
	basementGeode->addDrawable(basementGeometry);
	return basementGeode;
}

osg::ref_ptr<osg::Geode> Building::buildWalls(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate) {
	osg::ref_ptr<osg::Geode> wallGeode = new osg::Geode;
	osg::Vec4 wallColor = osg::Vec4(1.0, 0.2, 0.2, 1.0);
	osg::ref_ptr<osg::Geometry> wallGeometry = buildWall(polygon, baseCoordinate, m_roofHeight, wallColor);
	wallGeode->addDrawable(wallGeometry);
	return wallGeode;
}

osg::ref_ptr<osg::Geometry> Building::buildWall(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, double height, osg::Vec4 color) {
	// Collect the verticies
	osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array;
	osg::Vec2Array::iterator it;
	
	// Height from low to high
	double h1 = osg::minimum(polygon->height(), polygon->height() + height);
	double h2 = osg::maximum(polygon->height(), polygon->height() + height);

	// Add vertex and normals from points
	size_t numberOfPoints = polygon->points()->size();
	for (size_t p = 0; p < (numberOfPoints - 1); ++p) {
		osg::Vec2 point1 = polygon->points()->at(p) - baseCoordinate;
		osg::Vec2 point2 = polygon->points()->at(p+1) - baseCoordinate;
		createVertexAndNormal(point1, point2, h1, h2, vertexArray, normalArray);
	}

	// Add first point to close loop
	osg::Vec2 point1 = polygon->points()->at(numberOfPoints-1) - baseCoordinate;
	osg::Vec2 point2 = polygon->points()->at(0) - baseCoordinate;
	createVertexAndNormal(point1, point2, h1, h2, vertexArray, normalArray);

	osg::ref_ptr<osg::Geometry> wallGeometry = new osg::Geometry;
	wallGeometry->setVertexArray(vertexArray);
	wallGeometry->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, vertexArray->size()) );

	wallGeometry->setNormalArray(normalArray);
	wallGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX); 
	

	osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array;
	colorArray->push_back(color);
	wallGeometry->setColorArray(colorArray);
	wallGeometry->setColorBinding(osg::Geometry::BIND_OVERALL); 
	
	return wallGeometry;
}

void Building::createVertexAndNormal(osg::Vec2 point1, osg::Vec2 point2, double h1, double h2, osg::ref_ptr<osg::Vec3Array> vertexArray, osg::ref_ptr<osg::Vec3Array> normalArray) {
	osg::Vec3 vertex1 = osg::Vec3(point1.x(), h1, point1.y());
	osg::Vec3 vertex2 = osg::Vec3(point1.x(), h2, point1.y());
	osg::Vec3 vertex3 = osg::Vec3(point2.x(), h1, point2.y());
	osg::Vec3 vertex4 = osg::Vec3(point2.x(), h2, point2.y());	
	// Vertexs
	vertexArray->push_back(vertex1);
	vertexArray->push_back(vertex2);
	vertexArray->push_back(vertex4);
	vertexArray->push_back(vertex3);
	// Normals
	osg::Vec3 normal = (vertex3 - vertex1)^(vertex2 - vertex1);
	normal.normalize();
	normalArray->push_back(normal);
	normalArray->push_back(normal);
	normalArray->push_back(normal);
	normalArray->push_back(normal);
}


