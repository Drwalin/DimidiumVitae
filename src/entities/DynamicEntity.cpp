
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DYNAMIC_ENTITY_CPP
#define DYNAMIC_ENTITY_CPP

#include <resources/CollisionObjectManager.h>

#include "DynamicEntity.h"

#include <engine/Engine.h>

#include <util/Debug.h>
#include <util/Math.hpp>

#include <ctime>

void DynamicEntity::Tick(float deltaTime) {
	Entity::Tick(deltaTime);
	btRigidBody *rigidBody = GetBody<btRigidBody>();
	if(rigidBody) {
		btVector3 currentLinearVelocity = rigidBody->getLinearVelocity();
		btVector3 currentAngularVelocity = rigidBody->getAngularVelocity();
		float dv = (previousLinearVelocity-currentLinearVelocity).length() +
				(previousAngularVelocity-currentAngularVelocity).length();
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

void DynamicEntity::ApplyDamage(float damage, btVector3 point,
		btVector3 normal) {
	Entity::ApplyDamage(damage, point, normal);
}

DynamicEntity::DynamicEntity(const JSON& json) :
	Entity(json) {
	btCollisionObject *collisionObject =
			CollisionObjectManager::CreateRigidBody(collisionShape,
					currentTransform, json.Object().count("mass") ?
					json["mass"].Real() : 1.0f);
	btRigidBody *rigidBody = dynamic_cast<btRigidBody*>(collisionObject);
	
	rigidBody->setDamping(0.2, 0.2);
	rigidBody->setFriction(0.75);
	
	SetBody(collisionObject, collisionShape, CollisionDefaultGroupDynamic,
			CollisionDefaultMaskDynamic);
	
	hitSoundSource = new SoundSource(sing::resourceManager->GetSound(
				(std::string)"./media/Sounds/wood1.wav"));
}

DynamicEntity::~DynamicEntity() {
	if(hitSoundSource)
		delete hitSoundSource;
}

__ENTITY_DERIVED_CODE_FACTORY__(DynamicEntity)

#endif
	
