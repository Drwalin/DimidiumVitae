
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STATIC_ENTITY_CPP
#define STATIC_ENTITY_CPP

#include "StaticEntity.h"

#include "Engine.h"
#include "CollisionObjectManager.h"

#include <util/Debug.h>
#include <util/Math.hpp>

#include <ctime>

void StaticEntity::Tick(float deltaTime) {
	Entity::Tick(deltaTime);
}

void StaticEntity::ApplyDamage(float damage, btVector3 point,
		btVector3 normal) {
}

StaticEntity::StaticEntity(const JSON& json) :
	Entity(json) {
	
	btCollisionObject *collisionObject =
			CollisionObjectManager::CreateCollisionObject(collisionShape,
					currentTransform);
	
	collisionObject->setFriction(0.75);
	collisionObject->setCollisionFlags(
			btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
	
	SetBody(collisionObject, collisionShape, CollisionDefaultGroupStatic,
			CollisionDefaultMaskStatic);
}

StaticEntity::~StaticEntity() {
}

__ENTITY_DERIVED_CODE_FACTORY__(StaticEntity)

#endif
