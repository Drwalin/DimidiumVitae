
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MATERIAL_H
#define MATERIAL_H

#include <irrlicht/irrlicht.h>

#include <vector>
#include <memory>

#include "Resource.h"
#include "Texture.h"

class Material : public Resource {
public:
	
	Material(class Engine *engine, const std::string &name);
	~Material();
	
	static void SetTo(std::shared_ptr<Material> material, irr::scene::ISceneNode *iSceneNode);
	
	virtual Resource::ResourceType GetResourceType() const override;
	
private:
	
	std::vector<irr::video::SMaterial> materials;
	std::vector<std::shared_ptr<Texture>> textures;
};

#endif
