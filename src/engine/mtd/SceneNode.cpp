
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SCENE_NODE_CPP
#define SCENE_NODE_CPP

#include "../css/SceneNode.h"
#include "../css/Engine.h"
#include "../lib/Math.hpp"

Animation SceneNode::GetAnimation(const std::string &name) const {
	if(model)
		return Animation(model->GetAnimation(name), iSceneNode);
	return Animation(0, 0, 1);
}

irr::scene::IAnimatedMeshSceneNode *SceneNode::New(std::shared_ptr<Model> model) {
	if(model) {
		if(model->GetMesh()) {
			irr::scene::IAnimatedMeshSceneNode *iSceneNode = sing::sceneManager->addAnimatedMeshSceneNode(model->GetMesh());
			if(model->GetDefaultMaterial())
				Material::SetTo(model->GetDefaultMaterial(), iSceneNode);
			return iSceneNode;
		}
	}
	return NULL;
}

irr::scene::IAnimatedMeshSceneNode *SceneNode::GetISceneNode() {
	return iSceneNode;
}

void SceneNode::SetTransform(btTransform transform) {
	if(previousTransform.getRotation()!=transform.getRotation() || previousTransform.getOrigin()!=transform.getOrigin()) {
		iSceneNode->setPosition(Math::GetIrrVec(transform.getOrigin()));
		irr::core::vector3d<float> eulerRadians;
		btQuaternion quat(transform.getRotation().getAxis() *btVector3(1,-1,-1), transform.getRotation().getAngle());
		irr::core::quaternion q;
		q.fromAngleAxis(quat.getAngle(), irr::core::vector3d<float>(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z()));
		q.toEuler(eulerRadians);
		iSceneNode->setRotation(eulerRadians *irr::core::RADTODEG);
		previousTransform = transform;
	}
}

void SceneNode::SetScale(btVector3 scale) {
	iSceneNode->setScale(irr::core::vector3d<float>(scale.x(),scale.y(),scale.z()));
}

std::shared_ptr<SceneNode> SceneNode::AddChild(std::shared_ptr<Model> model, irr::scene::IAnimatedMeshSceneNode *iSceneNode) {
	std::shared_ptr<SceneNode> child(new SceneNode);
	child->Init(model);
	iSceneNode->addChild(child->iSceneNode);
	children.insert(child);
	return child;
}

std::shared_ptr<SceneNode> SceneNode::AddChild(std::shared_ptr<Model> model) {
	return AddChild(model, iSceneNode);
}

void SceneNode::DestroyChild(std::shared_ptr<SceneNode> child) {
	iSceneNode->removeChild(child->iSceneNode);
	children.erase(child);
}

void SceneNode::SetMaterial(std::shared_ptr<Material> material) {
	Material::SetTo(material, iSceneNode);
	this->material = material;
}

void SceneNode::Init(std::shared_ptr<Model> model, irr::scene::IAnimatedMeshSceneNode *iParentSceneNode) {
	this->model = model;
	this->iSceneNode = SceneNode::New(this->model);
	this->iParentSceneNode = iParentSceneNode;
	if(this->iParentSceneNode)
		this->iParentSceneNode->addChild(this->iSceneNode);
}

SceneNode::SceneNode() {
	iSceneNode = NULL;
}

SceneNode::~SceneNode() {
	if(iParentSceneNode) {
		iParentSceneNode->removeChild(iSceneNode);
		iParentSceneNode = NULL;
	}
	
	children.clear();
	
	SetMaterial(NULL);
	if(iSceneNode) {
		iSceneNode->remove();
		iSceneNode = NULL;
	}
	
	model = NULL;
}

#endif
