
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DYNAMIC_ENTITY_CPP
#define DYNAMIC_ENTITY_CPP

#include "../css/CollisionObjectManager.h"
#include "../css/DynamicEntity.h"
#include "../css/Engine.h"

#include "../lib/Debug.h"
#include "../lib/Math.hpp"

#include <ctime>

void DynamicEntity::Tick(const float deltaTime) {
	Entity::Tick(deltaTime);
	btRigidBody *rigidBody = GetBody<btRigidBody>();
	if(rigidBody) {
		btVector3 currentLinearVelocity = rigidBody->getLinearVelocity();
		btVector3 currentAngularVelocity = rigidBody->getAngularVelocity();
		float dv = (previousLinearVelocity-currentLinearVelocity).length() + (previousAngularVelocity-currentAngularVelocity).length();
		if(dv > 15.0f) {
			float volume = (dv-1/.0f)*0.1f;
			if(hitSoundSource->IsPlaying() == false) {
				hitSoundSource->SetLocation(GetLocation());
				hitSoundSource->Play();
				hitSoundSource->SetVolume(volume);
			}
		}
		previousLinearVelocity = currentLinearVelocity;
		previousAngularVelocity = currentAngularVelocity;
	}
}

void DynamicEntity::ApplyDamage(const float damage, btVector3 point, btVector3 normal) {
	Entity::ApplyDamage(damage, point, normal);
}

void DynamicEntity::Spawn(size_t id, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	Entity::Spawn(id, shape, transform);
	
	btCollisionObject *collisionObject = CollisionObjectManager::CreateRigidBody(shape, transform, 1.0f);
	btRigidBody *rigidBody = dynamic_cast<btRigidBody*>(collisionObject);
	
	rigidBody->setDamping(0.2, 0.2);
	rigidBody->setFriction(0.75);
	
	SetBody(collisionObject, shape, CollisionDefaultGroupDynamic, CollisionDefaultMaskDynamic);
	
	hitSoundSource = new SoundSource(sing::resourceManager->GetSound("./media/Sounds/wood1.wav"));
}

DynamicEntity::DynamicEntity() :
	Entity() {
	hitSoundSource = NULL;
}

DynamicEntity::~DynamicEntity() {
	if(hitSoundSource)
		delete hitSoundSource;
}

__ENTITY_DERIVED_CODE_FACTORY__(DynamicEntity)

#endif
