
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_TRIGGER_CPP
#define MOTION_CONTROLLER_TRIGGER_CPP

#include "../css/MotionControllerTrigger.h"
#include <Engine.h>

SimulationContactResultCallback::SimulationContactResultCallback(MotionControllerTrigger *trigger, float mid) : trigger(trigger), mid(mid) {}

btScalar SimulationContactResultCallback::addSingleResult(btManifoldPoint& manifoldPoint,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {
	btVector3 contactPoint = (manifoldPoint.m_positionWorldOnB + manifoldPoint.m_positionWorldOnA) *0.5f;
	btVector3 normal = manifoldPoint.m_normalWorldOnB.normalized();
	if(normal.y() < 0.0f)
		normal.setY(-normal.y());
	bool isNormalVertical = normal.dot(btVector3(0,1,0)) > 0.8f;
	
	if(isNormalVertical) {
		if(trigger->bottom + trigger->stepHeight >= contactPoint.y()) {
			trigger->bottomCollision = true;
		}
		if(mid <= contactPoint.y()) {
			trigger->topCollision = true;
		}
	} else {
		trigger->sideCollision = true;
	}
	
	return 0.0f;
}

bool MotionControllerTrigger::IsTopCollision() const {
	return topCollision;
}

bool MotionControllerTrigger::IsSideCollision() const {
	return sideCollision;
}

bool MotionControllerTrigger::IsBottomCollision() const {
	return bottomCollision;
}

void MotionControllerTrigger::Init(Entity *character, Entity *otherTrigger, float stepHeight) {
	this->character = character;
	this->otherTrigger = otherTrigger;
	this->stepHeight = stepHeight;
	bottom = 0.0f;
	top = 1.75f;
}

void MotionControllerTrigger::ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject) {
	if(entity != character && entity != this && entity != otherTrigger) {
		float mid = (bottom + top)*0.5f;
		
		SimulationContactResultCallback resultCallback(this, mid);
		sing::world->GetDynamicsWorld()->contactPairTest(collisionObject, body, resultCallback);
	}
}

void MotionControllerTrigger::NextOverlappingFrame() {
	topCollision = false;
	sideCollision = false;
	bottomCollision = false;
	
	btVector3 min, max;
	GetBtCollisionShape()->getAabb(body->getWorldTransform(), min, max);
	bottom = min.y();
	top = max.y();
	//float mid = (bottom + top)*0.5f;
	
	Trigger::NextOverlappingFrame();
}

void MotionControllerTrigger::Tick(const float deltaTime) {
	Trigger::Tick(deltaTime);
}

MotionControllerTrigger::MotionControllerTrigger(const JSON& json) :
	Trigger(json) {
}

MotionControllerTrigger::~MotionControllerTrigger() {
}

__ENTITY_DERIVED_CODE_FACTORY__(MotionControllerTrigger)

#endif
