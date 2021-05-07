
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_CPP
#define RESOURCE_CPP

#include "Resource.h"
#include "Singleton.h"

#include <ctime>

Resource::~Resource() {
}

const std::string& Resource::GetName() const {
	return name;
}

Resource::Resource(const JSON& json) :
	name(json.IsObject() ?
			(json.Object().count("name") ?
					json["name"].String() : ""
			) : ""
	) {
}

const std::string& Resource::GetResourceTypeString(ResourceType type) {
	const static std::string sound("Sound");
	const static std::string model("Model");
	const static std::string texture("Texture");
	const static std::string material("Material");
	const static std::string collisionShape("CollisionShape");
	switch(type) {
		case SOUND: return sound;
		case MODEL: return model;
		case TEXTURE: return texture;
		case MATERIAL: return material;
		case COLLISIONSHAPE: return collisionShape;
		default: break;
	}
	const static std::string none;
	return none;
}

Resource::ResourceType Resource::GetResourceType(const std::string &type) {
	if(type == "Sound") return SOUND;
	if(type == "Model") return MODEL;
	if(type == "Texture") return TEXTURE;
	if(type == "Material") return MATERIAL;
	if(type == "CollisionShape") return COLLISIONSHAPE;
	return NONE;
}

#endif
