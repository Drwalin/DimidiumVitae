
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <set>
#include <memory>

#include "Model.h"

class SceneNode {
public:
	
	SceneNode();
	~SceneNode();
	
	void Init(class Engine *engine, std::shared_ptr<Model> model, irr::scene::IAnimatedMeshSceneNode *iParentSceneNode = NULL);
	
	Animation GetAnimation(const std::string &name) const;
	
	static irr::scene::IAnimatedMeshSceneNode *New(class Engine *engine, std::shared_ptr<Model> model);
	
	irr::scene::IAnimatedMeshSceneNode *GetISceneNode();
	
	void SetTransform(btTransform transform);
	void SetScale(btVector3 scale);
	
	std::shared_ptr<SceneNode> AddChild(std::shared_ptr<Model> model, irr::scene::IAnimatedMeshSceneNode *iParentSceneNode);
	std::shared_ptr<SceneNode> AddChild(std::shared_ptr<Model> model);
	void DestroyChild(std::shared_ptr<SceneNode> child);
	
	void Destroy();
	
private:
	
	class Engine *engine;
	std::shared_ptr<Model> model;
	irr::scene::IAnimatedMeshSceneNode *iSceneNode;
	irr::scene::IAnimatedMeshSceneNode *iParentSceneNode;
	
	std::set<std::shared_ptr<SceneNode>> children;
};

#endif

