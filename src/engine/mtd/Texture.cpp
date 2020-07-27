
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <Texture.h>
#include <Engine.h>

Texture::Texture(class Engine *engine, const std::string &name) :
	Resource(name) {
	this->engine = engine;
	texture = engine->GetWindow()->GetVideoDriver()->getTexture(name.c_str());
}

Texture::~Texture() {
	engine->GetWindow()->GetVideoDriver()->removeTexture(texture);
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

#endif
