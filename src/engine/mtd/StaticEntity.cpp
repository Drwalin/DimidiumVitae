
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STATIC_ENTITY_CPP
#define STATIC_ENTITY_CPP

#include "../css/StaticEntity.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void StaticEntity::Tick(const float deltaTime) {
	Entity::Tick(deltaTime);
}

void StaticEntity::ApplyDamage(const float damage, btVector3 point, btVector3 normal) {}

void StaticEntity::Load(std::istream &stream) {
	Entity::Load(stream);
}

void StaticEntity::Save(std::ostream &stream) const {
	Entity::Save(stream);
}

void StaticEntity::Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	Entity::Spawn(name, shape, transform);
	
	btCollisionObject *collisionObject = CollisionObjectManager::CreateCollisionObject(shape, transform);
	
	collisionObject->setFriction(0.75);
	collisionObject->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
	
	SetBody(collisionObject, shape, CollisionDefaultGroupStatic, CollisionDefaultMaskStatic);
}

void StaticEntity::Despawn() {
	Entity::Despawn();
}

void StaticEntity::Destroy() {
	Entity::Destroy();
}

extern "C" std::shared_ptr<Entity> GetStaticEntityInstantiator() { static std::shared_ptr<Entity> instantiator(new StaticEntity(), [](Entity *ptr) {delete ptr;}); return instantiator; }
int StaticEntity::GetTypeSize() const{ return sizeof(StaticEntity); }
void StaticEntity::Free() { delete this; }
Entity* StaticEntity::New() const { return new StaticEntity(); }
std::string StaticEntity::GetClassName() const{ return "StaticEntity"; }

StaticEntity::StaticEntity() :
	Entity() {
}

StaticEntity::~StaticEntity() {
	Destroy();
}

#endif
