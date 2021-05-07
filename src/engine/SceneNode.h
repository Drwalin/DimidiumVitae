
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <irrlicht/irrlicht.h>

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include <set>
#include <memory>

#include "Model.h"
#include "Material.h"

class SceneNode {
public:
	
	SceneNode();
	~SceneNode();
	
	void Init(std::shared_ptr<Model> model,
			irr::scene::IAnimatedMeshSceneNode *iParentSceneNode = NULL);
	
	Animation GetAnimation(const std::string &name) const;
	
	static irr::scene::IAnimatedMeshSceneNode *New(
			std::shared_ptr<Model> model);
	
	irr::scene::IAnimatedMeshSceneNode *GetISceneNode();
	
	void SetTransform(btTransform transform);
	void SetScale(btVector3 scale);
	
	std::shared_ptr<SceneNode> AddChild(std::shared_ptr<Model> model,
			irr::scene::IAnimatedMeshSceneNode *iParentSceneNode);
	std::shared_ptr<SceneNode> AddChild(std::shared_ptr<Model> model);
	void DestroyChild(std::shared_ptr<SceneNode> child);
	
	void SetMaterial(std::shared_ptr<Material> material);
	
private:
	
	btTransform previousTransform;
	
	std::shared_ptr<Material> material;
	std::shared_ptr<Model> model;
	irr::scene::IAnimatedMeshSceneNode *iSceneNode;
	irr::scene::IAnimatedMeshSceneNode *iParentSceneNode;
	
	std::set<std::shared_ptr<SceneNode>> children;
};

#endif
