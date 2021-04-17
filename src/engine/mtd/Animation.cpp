
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ANIMATION_CPP
#define ANIMATION_CPP

#include "../css/Animation.h"
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

Animation::Animation(const JSON& json) {
	duration = json["duration"].Real();
	startFrame = json["start"];
	endFrame = json["end"];
	iSceneNode = NULL;
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

void Animation::GetJSON(JSON& json) const {
	json.InitObject();
	json["duration"] = duration;
	json["start"] = startFrame;
	json["end"] = endFrame;
}

#endif
