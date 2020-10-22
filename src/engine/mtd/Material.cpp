
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include <irrlicht/irrlicht.h>

#include <algorithm>
#include <vector>

#include "../lib/StdUtil.hpp"
#include "../lib/StlStreamExtension.h"

#include "../css/Material.h"
#include "../css/Engine.h"
#include "../css/Singleton.h"

Material::Material(JSON json) :
	Resource(json) {
	IFile file = sing::fileSystem->ReadFile(name);
	if(file && file.good() && !file.eof()) {
		std::string line;
		while(file.good() && !file.eof()) {
			line = "";
			GetLine(file, line);
			
			if(line != "") {
				if(line.find("newmtl") == 0) {
					materials.resize(materials.size() + 1);
				} else if(line.find("map_K") == 0) {
					std::string textureFileName = (line.c_str()+7);
					textures.emplace_back(sing::resourceManager->GetTexture(textureFileName));
					materials.back().setTexture(0, textures.back()->GetITexture());
				} else if(line[0] == 'K') {
					float r, g, b;
					sscanf(line.c_str()+3, "%f%f%f", &r, &g, &b);
					switch(line[1]) {
					case 'a': materials.back().AmbientColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					case 'd': materials.back().DiffuseColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					case 's': materials.back().SpecularColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					case 'e': materials.back().EmissiveColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					}
				} else if(line.find("Ns") == 0) {
					float s;
					sscanf(line.c_str()+3, "%f", &s);
					materials.back().Shininess = s;
				} else if(line.find("illum") == 0) {
					int il;
					sscanf(line.c_str()+6, "%i", &il);
					materials.back().Shininess = il;
				}
			}
		}
	} else
		throw std::string(std::string("Cannot load material from file: ")+name);
}

void Material::SetTo(std::shared_ptr<Material> material, irr::scene::ISceneNode *iSceneNode) {
	if(iSceneNode) {
		if(material && material->materials.size()>0) {
			int i=0;
			int materialCount = std::min<int>(iSceneNode->getMaterialCount(), material->materials.size());
			for(i=0; i<materialCount; ++i)
				iSceneNode->getMaterial(i) = material->materials[i];
			for(; i<iSceneNode->getMaterialCount(); ++i)
				iSceneNode->getMaterial(i) = material->materials.back();
		} else {
			for(int i=0; i<iSceneNode->getMaterialCount(); ++i)
				iSceneNode->getMaterial(i) = irr::video::SMaterial();
		}
		iSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	}
}

Material::~Material() {
}

Resource::ResourceType Material::GetResourceType() const {
	return Resource::MATERIAL;
}

void Material::GetJSON(JSON json) const {
	json.InitObject();
	if(name != "") {
		json["class"] = "Material";
		json["name"] = name;
	}
}

#endif
