#ifndef _WORLDBUILDER_H_
#define _WORLDBUILDER_H_

#include "buildinglibrary.h"
#include "texturelibrary.h"
#include "heighttile.h"
#include "shapeworld.h"

class WorldBuilder : public osg::Referenced {
public:
	WorldBuilder(std::string filename) : m_filename(filename), m_buildingLibrary(0), m_textureLibrary(0) {};
	void loadConfiguration();
	void buildWorld();
protected:
	~WorldBuilder() {}

	void addPolygonsToHeight(PolygonVector polygons);
	
	std::string m_filename;

	osg::ref_ptr<BuildingLibrary> m_buildingLibrary;
	osg::ref_ptr<TextureLibrary> m_textureLibrary;
	std::vector<std::string> m_shapeFilenames;
	std::vector<std::string> m_heigthFilenames;

	HeightTileVector m_heightTiles;
	ShapeWorldVector m_shapeTiles;
	
};

#endif
