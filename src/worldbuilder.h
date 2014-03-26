#ifndef _WORLDBUILDER_H_
#define _WORLDBUILDER_H_

#include "buildinglibrary.h"
#include "texturelibrary.h"
#include "heighttile.h"
#include "shapeworld.h"

class WorldBuilder : public osg::Referenced {
public:
	WorldBuilder(std::string filename) : m_filename(filename) {};
	void loadConfiguration();
	void buildWorld();
protected:
	~WorldBuilder() {}

	void addPolygonsToHeight(PolygonVector polygons);
	
	std::string m_filename;

	std::vector<std::string> m_shapeFilenames;
	std::vector<std::string> m_heigthFilenames;

	HeightTileVector m_heightTiles;
	ShapeWorldVector m_shapeTiles;
	PolygonVector m_polygonsOutsideTiles;
};

#endif
