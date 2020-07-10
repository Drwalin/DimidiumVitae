
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_H
#define MODEL_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstring>
#include <cstdio>

#include <memory>

#include "..\lib\Debug.h"
#include "..\lib\AR.hpp"

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

class Model {
public:
	
	Model(Model *other);
	Model();
	~Model();
	
	void SetName(std::string name);
	
	std::shared_ptr<irr::scene::IAnimatedMesh> GetMesh();
	void SetMaterialsToNode(irr::scene::ISceneNode *node) const;
	Animation GetAnimation(const std::string &animationName) const;
	
	bool LoadMesh(class Engine *engine, const std::string &objFileName);
	bool LoadMaterials(const std::string &materialsFileName);
	bool LoadAnimations(const std::string &animationsFileName);
	
	void Destroy();
	
private:
	
	std::shared_ptr<irr::scene::IAnimatedMesh> mesh;
	std::vector < irr::video::SMaterial > materials;
	std::map < std::string, Animation > animations;
	
	class Engine *engine;
	
	std::string name;
	std::string fileName;
};

#endif

