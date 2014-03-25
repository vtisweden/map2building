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

	// Build windows
	
	
	return building;
}

osg::ref_ptr<osg::Geode> Building::buildBasement(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate) {
	osg::ref_ptr<osg::Geode> basementGeode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> basementGeometry = buildWall(polygon, baseCoordinate, m_basementHeight);
	basementGeode->addDrawable(basementGeometry);
	return basementGeode;
}

osg::ref_ptr<osg::Geometry> Building::buildWall(osg::ref_ptr<Polygon> polygon, osg::Vec2 baseCoordinate, double height) {
	// Collect the verticies
	osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
	osg::Vec2Array::iterator it;
	// 
	for (it = polygon->points()->begin(); it != polygon->points()->end(); ++it) {
		osg::Vec2 p = (*it);
		vertexArray->push_back(osg::Vec3(p.x() - baseCoordinate.x(), polygon->height(), p.y()-baseCoordinate.y()));
	}
	size_t numberVerticiesInLayer = vertexArray->size();
	// Extrude
	for (it = polygon->points()->begin(); it != polygon->points()->end(); ++it) {
		osg::Vec2 p = (*it);
		vertexArray->push_back(osg::Vec3(p.x() - baseCoordinate.x(), polygon->height() + height, p.y()-baseCoordinate.y()));
	}
	osg::ref_ptr<osg::Geometry> wallGeometry = new osg::Geometry;
	wallGeometry->setVertexArray(vertexArray);
	wallGeometry->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0 ,numberVerticiesInLayer));
	wallGeometry->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, numberVerticiesInLayer ,numberVerticiesInLayer));

	osgUtil::Tessellator tesselator;
	tesselator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);
	tesselator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
	tesselator.retessellatePolygons(*wallGeometry);

	osg::ref_ptr<osg::DrawElementsUInt> sideIndices = new osg::DrawElementsUInt(GL_QUAD_STRIP);
	for (size_t i = 0; i < numberVerticiesInLayer; ++i) {
		sideIndices->push_back(i);
		sideIndices->push_back((numberVerticiesInLayer-1-i) + numberVerticiesInLayer);
	}
	sideIndices->push_back(0);
	sideIndices->push_back(numberVerticiesInLayer*2 - 1);

	wallGeometry->addPrimitiveSet(sideIndices);

	osgUtil::SmoothingVisitor::smooth(*wallGeometry);

	return wallGeometry;
}

