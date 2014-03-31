/*
 * texturelibrary.cpp
 *
 *  Created on: March 22, 2014
 *      Author: Bjorn Blissing
 */
#include "texturelibrary.h"

#include <osg/Texture2D>
#include <osg/Image>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>


osg::ref_ptr<osg::Texture> TextureLibrary::getTextureWithId(unsigned int id) const
{
	std::map<unsigned int, osg::ref_ptr<osg::Texture> >::const_iterator it = m_textureMap.find(id);
	osg::ref_ptr<osg::Texture> texture = 0;

	if (it != m_textureMap.end()) {
		texture = (*it).second;
	} else {
		osg::notify(osg::WARN) << "Warning: Texture with id: " << id << " does not exist." << std::endl;
	}

	return texture;
}

void TextureLibrary::load(pugi::xml_node texturesNode)
{
	// Load material sets
	for (pugi::xml_node textureNode = texturesNode.child("texture"); textureNode; textureNode = textureNode.next_sibling("texture")) {
		unsigned int id = textureNode.attribute("id").as_uint(0);
		std::string filename = osgDB::findDataFile(textureNode.attribute("filename").as_string(""));
		osg::ref_ptr<osg::Image> textureImage = osgDB::readImageFile(filename);
		// Strip path from filename
		textureImage->setFileName(osgDB::getSimpleFileName(filename));

		if (textureImage) {
			osg::notify(osg::DEBUG_INFO) << "Texture: Id: " << id << "\tFilename: " << osgDB::getSimpleFileName(filename) << std::endl;
			osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
			texture->setImage(textureImage);
			texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
			texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
			texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR_MIPMAP_LINEAR);
			texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
			texture->setUnRefImageDataAfterApply(true);
			addTextureWithId(id, texture);
		} else {
			osg::notify(osg::WARN) << "Warning: Unable to read texture:" << std::endl;
			osg::notify(osg::WARN) << "\tId:       " << id << std::endl;
			osg::notify(osg::WARN) << "\tFilename: " << filename << std::endl;
		}
	}
}


bool TextureLibrary::addTextureWithId(unsigned int id, osg::ref_ptr<osg::Texture> texture)
{
	if (m_textureMap.find(id) == m_textureMap.end()) {
		m_textureMap.insert(std::make_pair(id, texture));
		return true;
	}

	osg::notify(osg::WARN) << "Warning: Cannot add texture. ID already in use: " << id << std::endl;
	return false;
}
