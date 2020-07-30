
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_CPP
#define MODEL_CPP

#include "../css/Model.h"
#include "../css/Engine.h"

#include "../lib/StdUtil.hpp"
#include "../lib/StlStreamExtension.h"

void Animation::Play(bool loop) {
	if(iSceneNode) {
		if(endFrame >= startFrame && duration > 0)
			iSceneNode->setAnimationSpeed(float(endFrame - startFrame) / duration);
		else
			iSceneNode->setAnimationSpeed(24.0f);
		iSceneNode->setFrameLoop(startFrame, endFrame);
		iSceneNode->setLoopMode(loop);
	}
}

void Animation::SetSceneNode(irr::scene::IAnimatedMeshSceneNode *iSceneNode) {
	this->iSceneNode = iSceneNode;
}

Animation::Animation() {
	duration = 1.0f;
	iSceneNode = NULL;
	startFrame = 0;
	endFrame = 0;
}

Animation::Animation(const Animation &other) {
	duration = other.duration;
	iSceneNode = other.iSceneNode;
	startFrame = other.startFrame;
	endFrame = other.endFrame;
}

Animation::Animation(int startFrame, int endFrame, float duration) {
	this->startFrame = startFrame;
	this->endFrame = endFrame;
	this->duration = duration;
	iSceneNode = NULL;
}

Animation::Animation(const Animation &other, irr::scene::IAnimatedMeshSceneNode *iSceneNode) {
	duration = other.duration;
	this->iSceneNode = iSceneNode;
	startFrame = other.startFrame;
	endFrame = other.endFrame;
}

std::shared_ptr<irr::scene::IAnimatedMesh> Model::GetMesh() {
	return mesh;
}

std::shared_ptr<Material> Model::GetDefaultMaterial() const {
	return defaultMaterial;
}

Animation Model::GetAnimation(const std::string &animationName) const {
	auto it = animations.find(animationName);
	if(it != animations.end())
		return it->second;
	if(mesh)
		return Animation(0, mesh->getFrameCount()-1, 1.0f);
	return Animation(0, 0, 0);
}

void Model::LoadMesh(Engine *engine, const std::string &fileName) {
	Destroy();
	if(engine == NULL)
		throw (int)1;
	
	irr::scene::IAnimatedMesh *newMesh = engine->GetWindow()->GetSceneManager()->getMesh(fileName.c_str());
	if(newMesh == NULL)
		throw (int)2;
	mesh = std::shared_ptr<irr::scene::IAnimatedMesh>(newMesh, [engine](irr::scene::IAnimatedMesh*ptr) {engine->GetWindow()->GetSceneManager()->getMeshCache()->removeMesh(ptr);});
	
	this->engine = engine;
	
	std::string mtlFileName = GetFileWithPathWithoutExtension(fileName) + ".mtl";
	try {
		defaultMaterial = engine->GetResourceManager()->GetMaterial(mtlFileName);
	} catch(...) {
		defaultMaterial = NULL;
	}
	
	std::string animFileName = GetFileWithPathWithoutExtension(fileName) + ".anim";
	LoadAnimations(animFileName);
}

void Model::LoadAnimations(const std::string &animationsFileName) {
	JSON json = engine->GetFileSystem()->ReadJSON(animationsFileName);
	if(json.IsObject()) {
		for(auto it : json) {
			animations[it.Name()] = Animation(it.Value()["start"], it.Value()["end"], it.Value()["duration"]);
		}
	}
}

void Model::Destroy() {
	animations.clear();
	if(mesh)
		mesh.reset();
	mesh = NULL;
	engine = NULL;
}

Model::Model(Engine *engine, const std::string &name) :
	Resource(name) {
	LoadMesh(engine, name);
}

Model::~Model() {
	Destroy();
}

Resource::ResourceType Model::GetResourceType() const {
	return Resource::MODEL;
}

#endif

