
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include "../css/Singleton.h"

#include <Texture.h>
#include <Engine.h>

Texture::Texture(const JSON& json) :
	Resource(json) {
	texture = sing::videoDriver->getTexture(name.c_str());
}

Texture::~Texture() {
	sing::videoDriver->removeTexture(texture);
	texture = NULL;
}

irr::video::ITexture* Texture::GetITexture() const {
	return texture;
}

Texture::operator irr::video::ITexture*() const {
	return texture;
}

Texture::operator bool() const  {
	return (bool)texture;
}

Resource::ResourceType Texture::GetResourceType() const {
	return Resource::TEXTURE;
}

JSON Texture::GetJSON() const {
	JSON json;
	json.InitObject();
	if(name != "") {
		json["class"] = "Texture";
		json["name"] = name;
	}
	return json;
}

#endif
