
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ANIMATION_H
#define ANIMATION_H

#include <irrlicht/irrlicht.h>

#include <JSON.hpp>

class Animation {
public:
	
	Animation();
	Animation(const JSON& json);
	Animation(const Animation &other);
	Animation(int startFrame, int endFrame, float duration);
	Animation(const Animation &other, irr::scene::IAnimatedMeshSceneNode *iSceneNode);
	
	void Play(bool loop);
	
	void SetSceneNode(irr::scene::IAnimatedMeshSceneNode *iSceneNode);
	
	void GetJSON(JSON& json) const;
	
private:
	
	irr::scene::IAnimatedMeshSceneNode *iSceneNode;
	float duration;
	int startFrame;
	int endFrame;
};

#endif
