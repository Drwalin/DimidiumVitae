
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STATIC_ENTITY_CPP
#define STATIC_ENTITY_CPP

#include "..\css\StaticEntity.h"

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

void StaticEntity::Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform) {
	Entity::Spawn(self, name, shape, transform);
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateCollisionObject(shape, transform);
	
	collisionObject->setFriction(0.75);
	collisionObject->setCollisionFlags(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
	
	this->rayTraceChannel = Engine::RayTraceChannel::NONE;
	
	this->SetBody(collisionObject, shape, CollisionDefaultGroupStatic, CollisionDefaultMaskStatic);
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
std::shared_ptr<Entity> StaticEntity::New() const{ return std::dynamic_pointer_cast<Entity>(std::shared_ptr<StaticEntity>(new StaticEntity(), [](Entity *ptr) {delete ptr;})); }
std::string StaticEntity::GetClassName() const{ return "StaticEntity"; }

StaticEntity::StaticEntity() :
	Entity() {
}

StaticEntity::~StaticEntity() {
	this->Destroy();
}

#endif
