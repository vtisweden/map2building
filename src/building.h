#ifndef _BUILDING_H_ 
#define _BUILDING_H_

#include <vector>

#include <osg/Referenced>

#include "pugixml.hpp"

struct MaterialSet {
	unsigned int wallMaterialId;
	unsigned int windowMaterialId;
	unsigned int roofMaterialId;
	unsigned int baseMaterialId;
};

class Building : public osg::Referenced {
public:
	Building() : m_roofHeight(0.0), m_basementHeight(0.0), m_windowOffset(0.0) {};
	void load(pugi::xml_node buildingNode);
protected:
	~Building() {};
	double m_roofHeight;
	double m_basementHeight;
	double m_windowOffset;
	std::vector<MaterialSet> m_materials;
};
#endif
