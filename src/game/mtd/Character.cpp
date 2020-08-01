
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "../css/Character.h"

#include <Engine.h>
#include <World.h>
#include <CollisionObjectManager.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

std::shared_ptr<MotionController> Character::GetMotionController() {
	return motionController;
}

void Character::Tick(const float deltaTime) {
	Entity::Tick(deltaTime);
	motionController->Tick(deltaTime);
	
	static TimeCounter fpsCounter = ({TimeCounter cnt; cnt.SetTimeSpan(2.0f); fpsCounter.SubscribeStart(); cnt;});
	fpsCounter.SubscribeEnd();
	sing::gui << Rectanglef(0.05,0.02,0.6,0.6) << "Character position: " << GetTransform().getOrigin();
	btRigidBody *rigidBody = GetBody<btRigidBody>();
	sing::gui << "\n Entities count: " << sing::engine->GetNumberOfEntities();
	sing::gui << "\nFPS: " << 1.0f/fpsCounter.GetSmoothTime();
	sing::gui << " " << 1.0f/fpsCounter.GetPeakTime();
	sing::gui << " " << 1.0f/fpsCounter.GetPitTime();
	sing::gui << "\nDelta Time: " << fpsCounter.GetSmoothTime()*1000.0f;
	sing::gui << " " << fpsCounter.GetPeakTime()*1000.0f;
	sing::gui << " " << fpsCounter.GetPitTime()*1000.0f;
	sing::gui << "\n Draw Time: " << sing::window->GetWholeDrawTime().GetSmoothTime()*1000.0f;
	sing::gui << " " << sing::window->GetWholeDrawTime().GetPeakTime()*1000.0f;
	sing::gui << " " << sing::window->GetWholeDrawTime().GetPitTime()*1000.0f;
	sing::gui << "\n Engine Tick Time: " << sing::window->GetEngineTickTime().GetSmoothTime()*1000.0f;
	sing::gui << " " << sing::window->GetEngineTickTime().GetPeakTime()*1000.0f;
	sing::gui << " " << sing::window->GetEngineTickTime().GetPitTime()*1000.0f;
	sing::gui << "\n Skipped Time: " << sing::window->GetSkippedTime().GetSmoothTime()*1000.0f;
	sing::gui << " " << sing::window->GetSkippedTime().GetPeakTime()*1000.0f;
	sing::gui << " " << sing::window->GetSkippedTime().GetPitTime()*1000.0f;
	fpsCounter.SubscribeStart();
}

void Character::ApplyDamage(const float damage, btVector3 point, btVector3 normal) {
	Entity::ApplyDamage(damage, point, normal);
}

void Character::Load(std::istream &stream) {
	Entity::Load(stream);
}

void Character::Save(std::ostream &stream) const
{
	Entity::Save(stream);
}

void Character::Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	Entity::Spawn(name, shape, transform);
	
	btCollisionObject *collisionObject = CollisionObjectManager::CreateRigidBody(shape, transform, 15.0f, btVector3(0,0,0));
	btRigidBody *rigidBody = dynamic_cast<btRigidBody*>(collisionObject);
	
	rigidBody->setFriction(0.2);
	rigidBody->setAngularFactor(btVector3(0, 0, 0));
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	rigidBody->setDamping(0.1, 0.1);
	
	SetBody(collisionObject, shape, CollisionDefaultGroupCharacter, CollisionDefaultMaskCharacter);
	
	motionController = std::shared_ptr<MotionController>(new MotionController());
	motionController->Init(this, 0.3f);
}

void Character::Despawn() {
	Entity::Despawn();
	if(motionController)
		motionController->Destroy();
	motionController = NULL;
}

void Character::Destroy() {
	Entity::Destroy();
	if(motionController)
		motionController->Destroy();
	motionController = NULL;
}

Character::Character() :
	Entity(), height(1.75) {
}

Character::~Character() {
	Destroy();
}

__ENTITY_DERIVED_CODE_FACTORY__(Character)

#endif

