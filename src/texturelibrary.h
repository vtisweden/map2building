#ifndef _TEXTURE_LIBRARY_H_
#define _TEXTURE_LIBRARY_H_

#include <map>

#include <osg/Referenced>
#include <osg/Texture>

#include "pugixml.hpp"

class TextureLibrary : public osg::Referenced {
public:
	TextureLibrary() {};
	osg::ref_ptr<osg::Texture> getTextureWithId(unsigned int id) const;
	void load(pugi::xml_node texturesNode);
protected:
	~TextureLibrary() {};
	bool addTextureWithId(unsigned int id, osg::ref_ptr<osg::Texture> texture);

	std::map<unsigned int, osg::ref_ptr<osg::Texture> > m_textureMap;
};

#endif
