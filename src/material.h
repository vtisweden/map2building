/*
 * material.h
 *
 *  Created on: March 28, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <map>

#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Vec4>
#include <osg/StateSet>

#include "pugixml.hpp"

class Material : public osg::Referenced {
	public:
		Material();
		void load(pugi::xml_node materialNode);
		osg::ref_ptr<osg::StateSet> getStateSet();
	protected:
		~Material() {};

		osg::Vec4 getColorFromNode(pugi::xml_node colorNode);
		std::string colorAsString(osg::Vec4 color);

		std::string m_name;

		osg::Vec4 m_ambient;
		osg::Vec4 m_diffuse;
		osg::Vec4 m_specular;
		osg::Vec4 m_emissive;
		double m_shininess;
		unsigned int m_textureId;

		osg::ref_ptr<osg::StateSet> m_stateSet;
};

typedef std::map<unsigned int, osg::ref_ptr<Material> > MaterialMap;
typedef MaterialMap::iterator MaterialMapIterator;

#endif