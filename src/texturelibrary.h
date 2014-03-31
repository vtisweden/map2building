/*
 * texturelibrary.h
 *
 *  Created on: March 22, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _TEXTURE_LIBRARY_H_
#define _TEXTURE_LIBRARY_H_

#include <map>

#include <osg/Referenced>
#include <osg/Texture>

#include "pugixml.hpp"

typedef std::map<unsigned int, osg::ref_ptr<osg::Texture> > TextureMap;
typedef TextureMap::const_iterator TextureMapIterator;

class TextureLibrary : public osg::Referenced {
	public:
		static TextureLibrary& instance()
		{
			static TextureLibrary instance;
			return instance;
		}
		osg::ref_ptr<osg::Texture> getTextureWithId(unsigned int id) const;
		void load(pugi::xml_node texturesNode);
		void exportTextures(std::string outputPath);
	protected:
		TextureLibrary() {};
		~TextureLibrary() {};
		TextureLibrary(TextureLibrary const&); // No copy constructor allowed for singleton
		TextureLibrary& operator=(TextureLibrary const&); // No assignment operator allowed for singleton
		bool addTextureWithId(unsigned int id, osg::ref_ptr<osg::Texture> texture);

		TextureMap m_textureMap;
};

#endif
