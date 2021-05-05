
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include "../css/Trigger.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Math.hpp>

#include <cassert>

void Trigger::ProcessOverlappingEntity(Entity* entity,
		btCollisionObject* collisionObject) {
}

void Trigger::NextOverlappingFrame() {
	btGhostObject *ghostObject = dynamic_cast<btGhostObject*>(body);
	for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++) {
		btCollisionObject *body = ghostObject->getOverlappingObject(i);
		if(body) {
			Entity *objectT = (Entity*)(body->getUserPointer());
			if(objectT) {
				ProcessOverlappingEntity(objectT, body);
			}
		}
	}
}

void Trigger::Tick(float deltaTime) {
	if(body) {
		body->setWorldTransform(currentTransform);
		sing::world->UpdateColliderForObject(body);
	}
}

Trigger::Trigger(const JSON& json) :
	Entity(json) {
	
	btCollisionObject *collisionObject =
			CollisionObjectManager::CreateGhostObject(collisionShape,
					currentTransform);
	collisionObject->setCollisionFlags(collisionObject->getCollisionFlags() |
			btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE |
			btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT);
	
	SetBody(collisionObject, collisionShape, CollisionDefaultGroupTrigger,
			CollisionDefaultMaskTrigger);
}

Trigger::~Trigger() {
}

__ENTITY_DERIVED_CODE_FACTORY__(Trigger)

#endif
