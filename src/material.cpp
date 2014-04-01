/*
 * material.cpp
 *
 *  Created on: March 28, 2014
 *      Author: Bjorn Blissing
 */

#include "material.h"

#include <sstream>

#include <osg/Notify>
#include <osg/Material>
#include <osg/CullFace>

#include "texturelibrary.h"

Material::Material() : m_name("")
	, m_ambient(osg::Vec4(0.2f,0.2f,0.2f,1.0f))
	, m_diffuse(osg::Vec4(0.8f,0.8f,0.8f,1.0f))
	, m_specular(osg::Vec4(0.0f,0.0f,0.0f,1.0f))
	, m_emissive(osg::Vec4(0.0f,0.0f,0.0f,1.0f))
	, m_shininess(0.0)
	, m_textureId(0)
	, m_stateSet(0) {}


void Material::load(pugi::xml_node materialNode)
{
	m_name = materialNode.attribute("name").as_string("");

	if (pugi::xml_node diffuseNode = materialNode.child("diffuse")) {
		m_diffuse = getColorFromNode(diffuseNode);
	}

	if (pugi::xml_node ambientNode = materialNode.child("ambient")) {
		m_ambient = getColorFromNode(ambientNode);
	} else {
		// If no ambient value is defined, use same value as for diffuse
		m_ambient = m_diffuse;
	}

	if (pugi::xml_node specularNode = materialNode.child("specular")) {
		m_specular = getColorFromNode(specularNode);
	}

	if (pugi::xml_node emissiveNode = materialNode.child("emissive")) {
		m_emissive = getColorFromNode(emissiveNode);
	}

	if (pugi::xml_node shininessNode = materialNode.child("shininess")) {
		m_shininess = shininessNode.attribute("value").as_double(0.0);
	}

	if (pugi::xml_node textureNode = materialNode.child("texture")) {
		m_textureId = textureNode.attribute("id").as_uint(0);
	}

	osg::notify(osg::DEBUG_INFO) << "Material:  " << m_name << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Ambient:   " << colorAsString(m_ambient) << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Diffuse:   " << colorAsString(m_diffuse) << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Specular:  " << colorAsString(m_specular) << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Emissive:  " << colorAsString(m_emissive) << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Shininess: " << m_shininess << std::endl;
	osg::notify(osg::DEBUG_INFO) << "Texture:   " << m_textureId << std::endl;
	osg::notify(osg::DEBUG_INFO) << std::endl;
}

osg::ref_ptr<osg::StateSet> Material::getStateSet()
{
	if (!m_stateSet.valid()) {
		m_stateSet = new osg::StateSet;
		// Set basic material parameters
		osg::ref_ptr<osg::Material> material = new osg::Material();

		if (!m_name.empty()) {
			material->setName(m_name);
		}

		material->setAmbient(osg::Material::FRONT_AND_BACK, m_ambient);
		material->setDiffuse(osg::Material::FRONT_AND_BACK, m_diffuse);
		material->setEmission(osg::Material::FRONT_AND_BACK, m_emissive);
		material->setSpecular(osg::Material::FRONT_AND_BACK, m_specular);
		material->setShininess(osg::Material::FRONT_AND_BACK, m_shininess);
		// Do not use color mode, use Material instead
		material->setColorMode(osg::Material::OFF);
		m_stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
		// Cull back face
		osg::ref_ptr<osg::CullFace> cullface = new osg::CullFace();
		cullface->setMode(osg::CullFace::BACK);
		m_stateSet->setAttributeAndModes(cullface);

		// Attach texture
		if (m_textureId != 0) {
			osg::ref_ptr<osg::Texture> texture = TextureLibrary::instance().getTextureWithId(m_textureId);

			if (texture.valid()) {
				m_stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
			}
		}
	}

	return m_stateSet;
}

osg::Vec4 Material::getColorFromNode(pugi::xml_node colorNode)
{
	double r = colorNode.attribute("r").as_double(1.0);
	double g = colorNode.attribute("g").as_double(1.0);
	double b = colorNode.attribute("b").as_double(1.0);
	double a = colorNode.attribute("a").as_double(1.0);
	return osg::Vec4(r, g, b, a);
}

std::string Material::colorAsString(osg::Vec4 color)
{
	std::stringstream colorString;
	colorString << "\tR: " << color.r();
	colorString << "\tG: " << color.g();
	colorString << "\tB: " << color.b();
	colorString << "\tA: " << color.a();
	return colorString.str();
}