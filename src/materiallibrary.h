/*
 * materiallibrary.h
 *
 *  Created on: March 28, 2014
 *      Author: Bjorn Blissing
 */
#ifndef _MATERIALLIBRARY_H_
#define _MATERIALLIBRARY_H_

#include <osg/Referenced>
#include <osg/StateSet>

#include "pugixml.hpp"
#include "material.h"

class MaterialLibrary : public osg::Referenced {
	public:
		static MaterialLibrary& instance()
		{
			static MaterialLibrary instance;
			return instance;
		}

		void load(pugi::xml_node materialsNode);
		osg::ref_ptr<osg::StateSet> materialFromId(int materialId);
	protected:
		MaterialLibrary() {};
		~MaterialLibrary() {};
		MaterialLibrary(MaterialLibrary const&); // No copy constructor allowed for singleton
		MaterialLibrary& operator=(MaterialLibrary const&); // No assignment operator allowed for singleton

		bool addMaterialWithId(int id, osg::ref_ptr<Material> material);

		MaterialMap m_materialMap;
};

#endif