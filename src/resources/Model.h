
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_H
#define MODEL_H

#include <irrlicht/irrlicht.h>

#include <bullet/LinearMath/btVector3.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstring>
#include <cstdio>

#include <memory>

#include <util/Debug.h>
#include <util/AR.hpp>

#include "Resource.h"
#include "Material.h"
#include "Animation.h"

class Model : public Resource {
public:
	
	Model(const JSON& json);
	~Model();
	
	irr::scene::IAnimatedMesh* GetMesh();
	std::shared_ptr<Material> GetDefaultMaterial() const;
	Animation GetAnimation(const std::string &animationName) const;
	
	virtual Resource::ResourceType GetResourceType() const override;
	virtual JSON GetJSON() const override;
	
private:
	
	void LoadMesh(const std::string &objFileName);
	void LoadAnimations(const std::string &animationsFileName);
	
private:
	
	irr::scene::IAnimatedMesh *mesh;
	std::shared_ptr<Material> defaultMaterial;
	std::map<std::string, Animation> animations;
};

#endif
