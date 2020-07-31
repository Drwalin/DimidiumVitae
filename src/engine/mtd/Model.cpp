
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_CPP
#define MODEL_CPP

#include "../css/Model.h"
#include "../css/Engine.h"

#include "../lib/StdUtil.hpp"
#include "../lib/StlStreamExtension.h"

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

void Model::LoadMesh(const std::string &fileName) {
	Destroy();
	
	irr::scene::IAnimatedMesh *newMesh = sing::sceneManager->getMesh(fileName.c_str());
	if(newMesh == NULL)
		throw (int)2;
	mesh = std::shared_ptr<irr::scene::IAnimatedMesh>(newMesh, [](irr::scene::IAnimatedMesh*ptr) {sing::sceneManager->getMeshCache()->removeMesh(ptr);});
	
	std::string mtlFileName = GetFileWithPathWithoutExtension(fileName) + ".mtl";
	try {
		defaultMaterial = sing::resourceManager->GetMaterial(mtlFileName);
	} catch(...) {
		defaultMaterial = NULL;
	}
	
	std::string animFileName = GetFileWithPathWithoutExtension(fileName) + ".anim";
	LoadAnimations(animFileName);
}

void Model::LoadAnimations(const std::string &animationsFileName) {
	JSON json = sing::fileSystem->ReadJSON(animationsFileName);
	if(json.IsObject()) {
		for(auto it : json) {
			animations[it.Name()] = Animation(it.Value());
		}
	}
}

void Model::Destroy() {
	animations.clear();
	if(mesh)
		mesh.reset();
	mesh = NULL;
}

Model::Model(const std::string &name) :
	Resource(name) {
	LoadMesh(name);
}

Model::Model(JSON json) :
	Resource(json["name"]) {
	LoadMesh(name);
}

Model::~Model() {
	Destroy();
}

Resource::ResourceType Model::GetResourceType() const {
	return Resource::MODEL;
}

void Model::GetJSON(JSON json) const {
	json.InitObject();
	json["name"] = name;
}

#endif
