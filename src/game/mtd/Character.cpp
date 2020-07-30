
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
	engine->GetWindow()->GetGUI() << Rectanglef(0.05,0.02,0.6,0.6) << "Character position: " << GetTransform().getOrigin();
	std::shared_ptr<btRigidBody> rigidBody = GetBody<btRigidBody>();
	engine->GetWindow()->GetGUI() << "\n Entities count: " << engine->GetNumberOfEntities();
	engine->GetWindow()->GetGUI() << "\nFPS: " << 1.0f/fpsCounter.GetSmoothTime();
	engine->GetWindow()->GetGUI() << " " << 1.0f/fpsCounter.GetPeakTime();
	engine->GetWindow()->GetGUI() << " " << 1.0f/fpsCounter.GetPitTime();
	engine->GetWindow()->GetGUI() << "\nDelta Time: " << fpsCounter.GetSmoothTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << fpsCounter.GetPeakTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << fpsCounter.GetPitTime()*1000.0f;
	engine->GetWindow()->GetGUI() << "\n Draw Time: " << engine->GetWindow()->GetWholeDrawTime().GetSmoothTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << engine->GetWindow()->GetWholeDrawTime().GetPeakTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << engine->GetWindow()->GetWholeDrawTime().GetPitTime()*1000.0f;
	engine->GetWindow()->GetGUI() << "\n Engine Tick Time: " << engine->GetWindow()->GetEngineTickTime().GetSmoothTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << engine->GetWindow()->GetEngineTickTime().GetPeakTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << engine->GetWindow()->GetEngineTickTime().GetPitTime()*1000.0f;
	engine->GetWindow()->GetGUI() << "\n Skipped Time: " << engine->GetWindow()->GetSkippedTime().GetSmoothTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << engine->GetWindow()->GetSkippedTime().GetPeakTime()*1000.0f;
	engine->GetWindow()->GetGUI() << " " << engine->GetWindow()->GetSkippedTime().GetPitTime()*1000.0f;
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

void Character::Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	Entity::Spawn(self, name, shape, transform);
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateRigidBody(shape, transform, 15.0f, btVector3(0,0,0));
	std::shared_ptr<btRigidBody> rigidBody = std::dynamic_pointer_cast<btRigidBody>(collisionObject);
	
	rigidBody->setFriction(0.2);
	rigidBody->setAngularFactor(btVector3(0, 0, 0));
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	rigidBody->setDamping(0.1, 0.1);
	
	SetBody(collisionObject, shape, CollisionDefaultGroupCharacter, CollisionDefaultMaskCharacter);
	
	motionController = std::shared_ptr<MotionController>(new MotionController());
	motionController->Init(engine, self, 0.3f);
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

extern "C" std::shared_ptr<Entity> GetCharacterInstantiator() { static std::shared_ptr<Entity> instantiator(new Character(), [](Entity *ptr) {delete ptr;}); return instantiator; }
int Character::GetTypeSize() const{ return sizeof(Character); }
void Character::Free() { delete this; }
std::shared_ptr<Entity> Character::New() const{ return std::dynamic_pointer_cast<Entity>(std::shared_ptr<Character>(new Character(), [](Entity *ptr) {delete ptr;})); }
std::string Character::GetClassName() const{ return "Character"; }

Character::Character() :
	Entity(), height(1.75) {
}

Character::~Character() {
	Destroy();
}

#endif

