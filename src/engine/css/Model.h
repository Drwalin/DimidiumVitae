
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

#include "../lib/Debug.h"
#include "../lib/AR.hpp"

#include "Resource.h"
#include "Material.h"

class Animation {
public:
	
	Animation();
	Animation(const Animation &other);
	Animation(int startFrame, int endFrame, float duration);
	Animation(const Animation &other, irr::scene::IAnimatedMeshSceneNode *iSceneNode);
	
	void Play(bool loop);
	
	void SetSceneNode(irr::scene::IAnimatedMeshSceneNode *iSceneNode);
	
private:
	
	irr::scene::IAnimatedMeshSceneNode *iSceneNode;
	float duration;
	int startFrame;
	int endFrame;
};

class Model : public Resource {
public:
	
	Model(const std::string &name);
	~Model();
	
	std::shared_ptr<irr::scene::IAnimatedMesh> GetMesh();
	std::shared_ptr<Material> GetDefaultMaterial() const;
	Animation GetAnimation(const std::string &animationName) const;
	
	void LoadMesh(const std::string &objFileName);
	void LoadAnimations(const std::string &animationsFileName);
	
	void Destroy();
	
	virtual Resource::ResourceType GetResourceType() const override;
	
private:
	
	std::shared_ptr<irr::scene::IAnimatedMesh> mesh;
	std::shared_ptr<Material> defaultMaterial;
	std::map < std::string, Animation > animations;
};

#endif

