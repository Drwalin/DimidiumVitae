
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_CPP
#define MODEL_CPP

#include "../css/Model.h"
#include "../css/Engine.h"

#include "../lib/StdUtil.hpp"
#include "../lib/StlStreamExtension.h"

void Animation::Play(bool loop) {
	if(this->iSceneNode) {
		if(this->endFrame >= this->startFrame && this->duration > 0)
			this->iSceneNode->setAnimationSpeed(float(this->endFrame - this->startFrame) / this->duration);
		else
			this->iSceneNode->setAnimationSpeed(24.0f);
		this->iSceneNode->setFrameLoop(this->startFrame, this->endFrame);
		this->iSceneNode->setLoopMode(loop);
	}
}

void Animation::SetSceneNode(irr::scene::IAnimatedMeshSceneNode *iSceneNode) {
	this->iSceneNode = iSceneNode;
}

Animation::Animation() {
	this->duration = 1.0f;
	this->iSceneNode = NULL;
	this->startFrame = 0;
	this->endFrame = 0;
}

Animation::Animation(const Animation &other) {
	this->duration = other.duration;
	this->iSceneNode = other.iSceneNode;
	this->startFrame = other.startFrame;
	this->endFrame = other.endFrame;
}

Animation::Animation(int startFrame, int endFrame, float duration) {
	this->startFrame = startFrame;
	this->endFrame = endFrame;
	this->duration = duration;
	this->iSceneNode = NULL;
}

Animation::Animation(const Animation &other, irr::scene::IAnimatedMeshSceneNode *iSceneNode) {
	this->duration = other.duration;
	this->iSceneNode = iSceneNode;
	this->startFrame = other.startFrame;
	this->endFrame = other.endFrame;
}

std::shared_ptr<irr::scene::IAnimatedMesh> Model::GetMesh() {
	return this->mesh;
}

std::shared_ptr<Material> Model::GetDefaultMaterial() const {
	return defaultMaterial;
}

Animation Model::GetAnimation(const std::string &animationName) const {
	auto it = this->animations.find(animationName);
	if(it != this->animations.end())
		return it->second;
	if(this->mesh)
		return Animation(0, this->mesh->getFrameCount()-1, 1.0f);
	return Animation(0, 0, 0);
}

void Model::LoadMesh(Engine *engine, const std::string &fileName) {
	this->Destroy();
	if(engine == NULL)
		throw (int)1;
	
	irr::scene::IAnimatedMesh *newMesh = engine->GetWindow()->GetSceneManager()->getMesh(fileName.c_str());
	if(newMesh == NULL)
		throw (int)2;
	this->mesh = std::shared_ptr<irr::scene::IAnimatedMesh>(newMesh, [engine](irr::scene::IAnimatedMesh*ptr) {engine->GetWindow()->GetSceneManager()->getMeshCache()->removeMesh(ptr);});
	
	this->engine = engine;
	
	std::string mtlFileName = GetFileWithPathWithoutExtension(fileName) + ".mtl";
	try {
		defaultMaterial = engine->GetResourceManager()->GetMaterial(mtlFileName);
	} catch(...) {
		defaultMaterial = NULL;
	}
	
	std::string animFileName = GetFileWithPathWithoutExtension(fileName) + ".anim";
	this->LoadAnimations(animFileName);
}

void Model::LoadAnimations(const std::string &animationsFileName) {
	iirrfstream file(this->engine->GetWindow()->GetDevice()->getFileSystem()->createAndOpenFile(animationsFileName.c_str()));
	if(file && file.good() && !file.eof()) {
		std::string name;
		int start, end;
		float duration;
		while(file.good() && !file.eof()) {
			name = "";
			start = end = -1;
			duration = -1.0f;
			file >> name;
			file >> start;
			file >> end;
			file >> duration;
			if(!(name == "" || start < 0 || end < 0 || duration < 0))
				this->animations[name] = Animation(start, end, duration);
		}
	}
}

void Model::Destroy() {
	this->animations.clear();
	if(this->mesh)
		this->mesh.reset();
	this->mesh = NULL;
	this->engine = NULL;
}

Model::Model(Engine *engine, const std::string &name) :
	Resource(name) {
	LoadMesh(engine, name);
}

Model::~Model() {
	this->Destroy();
}

Resource::ResourceType Model::GetResourceType() const {
	return Resource::MODEL;
}

#endif

