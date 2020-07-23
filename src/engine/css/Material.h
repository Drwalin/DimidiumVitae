
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MATERIAL_H
#define MATERIAL_H

#include <irrlicht/irrlicht.h>

#include <vector>

#include "Resource.h"

class Material : public Resource {
public:
	
	Material(class Engine *engine, const std::string &name);
	~Material();
	
	void SetTo(irr::scene::ISceneNode *iSceneNode) const;
	
	virtual Resource::ResourceType GetResourceType() const override;
	
private:
	
	std::vector<irr::video::SMaterial> materials;
};

#endif
