
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>

#include <irrlicht/irrlicht.h>

#include "Resource.h"

class Texture : public Resource {
public:

	Texture() = delete;
	Texture(const Texture &other) = delete;
	Texture(JSON json);
	~Texture();
	Texture &operator = (const Texture &other) = delete;
	
	irr::video::ITexture* GetITexture() const;
	operator irr::video::ITexture*() const;
	
	operator bool() const;
	
	virtual Resource::ResourceType GetResourceType() const override;
	virtual void GetJSON(JSON json) const override;
	
private:
	
	irr::video::ITexture* texture;
};

#endif
