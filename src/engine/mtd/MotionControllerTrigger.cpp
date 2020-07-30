
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

void MotionControllerTrigger::Init(std::shared_ptr<Entity> character, std::shared_ptr<Entity> otherTrigger, float stepHeight) {
	this->character = character;
	this->otherTrigger = otherTrigger;
	this->stepHeight = stepHeight;
	bottom = 0.0f;
	top = 1.75f;
}

void MotionControllerTrigger::ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject) {
	if(entity != character.get() && entity != this && entity != otherTrigger.get()) {
		float mid = (bottom + top)*0.5f;
		
		SimulationContactResultCallback resultCallback(this, mid);
		engine->GetWorld()->GetDynamicsWorld()->contactPairTest(collisionObject, body.get(), resultCallback);
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
	float mid = (bottom + top)*0.5f;
	
	Trigger::NextOverlappingFrame();
}

void MotionControllerTrigger::Tick(const float deltaTime) {
	Trigger::Tick(deltaTime);
}

void MotionControllerTrigger::Load(std::istream &stream) {
	Trigger::Load(stream);
}

void MotionControllerTrigger::Save(std::ostream &stream) const
{
	Trigger::Save(stream);
}

void MotionControllerTrigger::Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	Trigger::Spawn(self, name, shape, transform);
}

void MotionControllerTrigger::Despawn() {
	Trigger::Despawn();
}

void MotionControllerTrigger::Destroy() {
	character = NULL;
	otherTrigger = NULL;
	Trigger::Destroy();
}

extern "C" std::shared_ptr<Entity> GetMotionControllerTriggerInstantiator() { static std::shared_ptr<Entity> instantiator(new MotionControllerTrigger(), [](Entity *ptr) {delete ptr;}); return instantiator; }
int MotionControllerTrigger::GetTypeSize() const{ return sizeof(MotionControllerTrigger); }
void MotionControllerTrigger::Free() { delete this; }
std::shared_ptr<Entity> MotionControllerTrigger::New() const{ return std::dynamic_pointer_cast<Entity>(std::shared_ptr<MotionControllerTrigger>(new MotionControllerTrigger(), [](Entity *ptr) {delete ptr;})); }
std::string MotionControllerTrigger::GetClassName() const{ return "MotionControllerTrigger"; }

MotionControllerTrigger::MotionControllerTrigger() {
}

MotionControllerTrigger::~MotionControllerTrigger() {
}

#endif
