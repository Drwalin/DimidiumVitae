
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

void Model::SetMaterialsToNode(irr::scene::ISceneNode *node) const {
	int merialCount = node->getMaterialCount();
	for(int i=0; i<merialCount && i<this->materials.size(); ++i)
		node->getMaterial(i) = this->materials[i];
	node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
}

Animation Model::GetAnimation(const std::string &animationName) const {
	auto it = this->animations.find(animationName);
	if(it != this->animations.end())
		return it->second;
	if(this->mesh)
		return Animation(0, this->mesh->getFrameCount()-1, 1.0f);
	return Animation(0, 0, 0);
}

void Model::SetName(std::string name) {
	this->name = name;
}

bool Model::LoadMesh(Engine *engine, const std::string &fileName) {
	this->Destroy();
	if(engine == NULL)
		return false;
	
	irr::scene::IAnimatedMesh *newMesh = engine->GetWindow()->GetSceneManager()->getMesh(fileName.c_str());
	if(newMesh == NULL)
		return false;
	this->mesh = std::shared_ptr<irr::scene::IAnimatedMesh>(newMesh, [engine](irr::scene::IAnimatedMesh*ptr) {engine->GetWindow()->GetSceneManager()->getMeshCache()->removeMesh(ptr);});
	
	this->fileName = fileName;
	this->engine = engine;
	
	std::string mtlFileName = GetCoreName(this->fileName) + ".mtl";
	this->LoadMaterials(mtlFileName);
	
	std::string animFileName = GetCoreName(this->fileName) + ".anim";
	bool anim = this->LoadAnimations(animFileName);
	printf(anim ? "\nAnimation loaded: %s" : "\nAimation not loaded: %s", animFileName.c_str());
	
	return true;
}

bool Model::LoadMaterials(const std::string &materialsFileName) {
	iirrfstream file(this->engine->GetWindow()->GetDevice()->getFileSystem()->createAndOpenFile(materialsFileName.c_str()));
	if(file && file.good() && !file.eof()) {
		std::string line;
		while(file.good() && !file.eof()) {
			line = "";
			GetLine(file, line);
			
			if(line != "") {
				if(line.find("newmtl") == 0) {
					this->materials.resize(this->materials.size() + 1);
				} else if(line.find("map_K") == 0) {
					std::string textureFileName = (line.c_str()+7);//GetPathWithoutSlash(materialsFileName) + "/" + (line.c_str()+7);
					this->materials.back().setTexture(0, this->engine->GetWindow()->GetVideoDriver()->getTexture(textureFileName.c_str()));
				} else if(line[0] == 'K') {
					float r, g, b;
					sscanf(line.c_str()+3, "%f%f%f", &r, &g, &b);
					switch(line[1]) {
					case 'a': this->materials.back().AmbientColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					case 'd': this->materials.back().DiffuseColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					case 's': this->materials.back().SpecularColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					case 'e': this->materials.back().EmissiveColor = irr::video::SColor(255, r*255.0f, g*255.0f, b*255.0f); break;
					}
				} else if(line.find("Ns") == 0) {
					float s;
					sscanf(line.c_str()+3, "%f", &s);
					this->materials.back().Shininess = s;
				} else if(line.find("illum") == 0) {
					int il;
					sscanf(line.c_str()+6, "%i", &il);
					this->materials.back().Shininess = il;
				}
			}
		}
		return true;
	}
	return false;
}

bool Model::LoadAnimations(const std::string &animationsFileName) {
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
			printf("\n '%s' '%i' '%i' '%f'", name.c_str(), start, end, duration);
			if(!(name == "" || start < 0 || end < 0 || duration < 0))
				this->animations[name] = Animation(start, end, duration);
		}
		return true;
	}
	return false;
}

void Model::Destroy() {
	this->materials.clear();
	this->animations.clear();
	if(this->mesh)
		this->mesh.reset();
	this->mesh = NULL;
	this->engine = NULL;
	this->name = "";
	this->fileName = "";
}

Model::Model(Model *other) {
	if(this->mesh)
		this->mesh.reset();
	if(other) {
		this->mesh = other->mesh;
		this->materials = other->materials;
		this->engine = other->engine;
		this->name = other->name;
		this->fileName = other->fileName;
		this->animations = other->animations;
	}
}

Model::Model() {
	this->engine = NULL;
}

Model::~Model() {
	this->Destroy();
}

#endif

