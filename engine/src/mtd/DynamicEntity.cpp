
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DYNAMIC_ENTITY_CPP
#define DYNAMIC_ENTITY_CPP

#include "..\css\CollisionObjectManager.h"
#include "..\css\DynamicEntity.h"
#include "..\css\Engine.h"

#include "..\lib\Debug.h"
#include "..\lib\Math.hpp"

#include <ctime>

void DynamicEntity::Tick(const float deltaTime) {
	Entity::Tick(deltaTime);
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
	if(rigidBody) {
		btVector3 currentLinearVelocity = rigidBody->getLinearVelocity();
		btVector3 currentAngularVelocity = rigidBody->getAngularVelocity();
		float dv = (this->previousLinearVelocity-currentLinearVelocity).length() + (this->previousAngularVelocity-currentAngularVelocity).length();
		if(dv > 15.0f) {
			float volume = (dv-1/.0f)*0.1f;
			if(this->hitSoundSource->IsPlaying() == false) {
				this->hitSoundSource->SetLocation(this->GetLocation());
				this->hitSoundSource->Play();
				this->hitSoundSource->SetVolume(volume);
			}
		}
		this->previousLinearVelocity = currentLinearVelocity;
		this->previousAngularVelocity = currentAngularVelocity;
	}
}

void DynamicEntity::ApplyDamage(const float damage, btVector3 point, btVector3 normal) {
	Entity::ApplyDamage(damage, point, normal);
}

void DynamicEntity::Load(std::istream &stream) {
	Entity::Load(stream);
}

void DynamicEntity::Save(std::ostream &stream) const {
	Entity::Save(stream);
}

void DynamicEntity::Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform) {
	Entity::Spawn(self, name, shape, transform);
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateRigidBody(shape, transform, 1.0f);
	std::shared_ptr<btRigidBody> rigidBody = std::dynamic_pointer_cast<btRigidBody>(collisionObject);
	
	rigidBody->setDamping(0.2, 0.2);
	rigidBody->setFriction(0.75);
	
	this->SetBody(collisionObject, shape);
	
	this->hitSoundSource = new SoundSource(this->engine->GetSoundsManager()->GetSoundSampler("wood1"));
}

void DynamicEntity::Despawn() {
	if(this->hitSoundSource)
		delete this->hitSoundSource;
	this->hitSoundSource = NULL;
	Entity::Despawn();
}

void DynamicEntity::Destroy() {
	if(this->hitSoundSource)
		delete this->hitSoundSource;
	this->hitSoundSource = NULL;
	Entity::Destroy();
}

extern "C" std::shared_ptr<Entity> GetDynamicEntityInstantiator() { static std::shared_ptr<Entity> instantiator(new DynamicEntity(), [](Entity *ptr) {delete ptr;}); return instantiator; }
int DynamicEntity::GetTypeSize() const{ return sizeof(DynamicEntity); }
void DynamicEntity::Free() { delete this; }
std::shared_ptr<Entity> DynamicEntity::New() const{ return std::dynamic_pointer_cast<Entity>(std::shared_ptr<DynamicEntity>(new DynamicEntity(), [](Entity *ptr) {delete ptr;})); }
std::string DynamicEntity::GetClassName() const{ return "DynamicEntity"; }

DynamicEntity::DynamicEntity() :
	Entity() {
	this->hitSoundSource = NULL;
}

DynamicEntity::~DynamicEntity() {
	this->Destroy();
}

#endif
