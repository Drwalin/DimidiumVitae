
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include "../css/Trigger.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Math.hpp>

#include <cassert>

void Trigger::ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject) {
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

void Trigger::Tick(const float deltaTime) {
	if(body) {
		body->setWorldTransform(currentTransform);
		sing::world->UpdateColliderForObject(body);
	}
}

void Trigger::Spawn(size_t id, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	Entity::Spawn(id, shape, transform);
	
	btCollisionObject *collisionObject = CollisionObjectManager::CreateGhostObject(shape, transform);
	collisionObject->setCollisionFlags(collisionObject->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT);
	
	SetBody(collisionObject, shape, CollisionDefaultGroupTrigger, CollisionDefaultMaskTrigger);
}

void Trigger::Despawn() {
	Entity::Despawn();
}

void Trigger::Destroy() {
	Entity::Destroy();
}

Trigger::Trigger() {
}

Trigger::~Trigger() {
}

__ENTITY_DERIVED_CODE_FACTORY__(Trigger)

#endif
