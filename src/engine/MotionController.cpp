
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_CPP
#define MOTION_CONTROLLER_CPP

#include <engine/Engine.h>

#include "MotionController.h"

#include <algorithm>

void MotionController::UpdateSpeed(const float deltaTime) {
	btRigidBody *rigidBody = character->GetBody<btRigidBody>();
	if(rigidBody && GetCurrentSpeed() > 0.0f) {
		btVector3 currentVelocity = rigidBody->getLinearVelocity();
		float currentSpeed = currentVelocity.length();
		if(currentSpeed >= 0.0000001f) {
			float newSpeed = currentSpeed - (deltaTime * GetFlatDeceleration());
			if(newSpeed < 0.0f)
				newSpeed = 0.0f;
			btVector3 newVelocity = currentVelocity * (newSpeed / currentSpeed);
			newVelocity.setY(currentVelocity.y());
			rigidBody->setLinearVelocity(newVelocity);
		}
		currentVelocity = rigidBody->getLinearVelocity();
		currentSpeed = currentVelocity.length();
		
		if(walkingDirection.length2() > 0.0001) {
			walkingDirection.normalize();
			//btVector3 destinyVelocity = walkingDirection * GetCurrentSpeed();
			float currentDot = currentVelocity.dot(walkingDirection);
			if(currentVelocity.length() < 0.5f ||
					currentDot < GetCurrentSpeed()) {
				btVector3 modifyingVelocity = walkingDirection *
						(GetCurrentSpeed() - currentDot) *0.5f;
				rigidBody->setLinearVelocity(
						currentVelocity + modifyingVelocity);
			}
		}
	}
}

void MotionController::UpdateTriggersTransform() {
	btTransform characterTransform = character->GetTransform();
	btVector3 characterOrigin = characterTransform.getOrigin();
	btQuaternion characterRotation = characterTransform.getRotation();
	triggerLow->SetRotation(characterRotation);
	triggerHigh->SetRotation(characterRotation);
	if(IsCrouching()) {
		triggerLow->SetLocation(characterOrigin + triggerLowOffsetCrouching);
		triggerHigh->SetLocation(characterOrigin + triggerHighOffsetCrouching);
		if(sing::engine->GetCameraParent() == character)
			sing::engine->GetCamera()->SetRelativePosition(
					btVector3(0, characterCrouchingHeight *0.5f - 0.1f, 0));
	} else {
		triggerLow->SetLocation(characterOrigin + triggerLowOffsetStanding);
		triggerHigh->SetLocation(characterOrigin + triggerHighOffsetStanding);
		if(sing::engine->GetCameraParent() == character)
			sing::engine->GetCamera()->SetRelativePosition(
					btVector3(0, characterStandingHeight *0.5f - 0.1f, 0));
	}
}

float MotionController::GetFlatDeceleration() {
	float deceleration = 1.0f;
	if(triggerHigh->IsBottomCollision() || triggerLow->IsBottomCollision())
		deceleration = 16.0f;
	else
		deceleration = 0.1f;
	return deceleration;
}

MotionController::State MotionController::GetCurrentState() const {
	if(crouchingState == MotionController::CrouchingState::CROUCHING ||
			crouchingState == MotionController::CrouchingState::STANDING_UP)
		return MotionController::State::CROUCHING;
	if(states.size()) {
		return states.back();
	}
	return MotionController::State::WALKING;
}

float MotionController::GetCurrentSpeed() {
	MotionController::State s = GetCurrentState();
	
	float v = 1.0f;
	
	switch(s) {
	case State::CROUCHING:
	case State::SNEAKING:
		v = 2.0f;
		break;
	case State::RUNNING:
		v = 6.0f;
		break;
	case State::WALKING:
	default:
		v = 3.7f;
	}
	
	if(!triggerHigh->IsBottomCollision() || !triggerLow->IsBottomCollision())
		v *= 0.2f;
	
	return v;
}

float MotionController::GetJumpVelocity() {
	return 5.0f;
}

void MotionController::Tick(const float deltaTime) {
	if(crouchingState == MotionController::CrouchingState::STANDING_UP &&
				!triggerHigh->IsTopCollision()) {
		character->GetBody()->setCollisionShape(
				triggerHigh->GetBtCollisionShape());
		sing::world->UpdateColliderForObject(character->GetBody());
		crouchingState = MotionController::CrouchingState::STANDING;
	}
	
	jumpCooldown += deltaTime;
	UpdateSpeed(deltaTime);
	walkingDirection = btVector3(0, 0, 0);
	
	if(triggerHigh->IsBottomCollision() || triggerLow->IsBottomCollision())
		character->GetBody<btRigidBody>()->setFriction(0.5);
	else
		character->GetBody<btRigidBody>()->setFriction(0);
	
	UpdateTriggersTransform();
	character->GetBody<btRigidBody>()->activate();
}

void MotionController::MoveInDirection(btVector3 direction) {
	walkingDirection += direction;
}

void MotionController::Jump() {
	if(jumpCooldown >= jumpCooldownLimit && triggerHigh->IsBottomCollision()) {
		jumpCooldown = 0.0f;
		btRigidBody *rigidBody = character->GetBody<btRigidBody>();
		if(rigidBody) {
			rigidBody->applyCentralImpulse(
					btVector3(0, 1, 0) * GetJumpVelocity() /
					rigidBody->getInvMass());
		}
	}
}

void MotionController::AddState(State state) {
	RemoveState(state);
	states.resize(states.size() + 1);
	states.back() = state;
}

void MotionController::RemoveState(State state) {
	while(true) {
		auto it = std::find(states.begin(), states.end(), state);
		if(it == states.end())
			break;
		states.erase(it);	
	}
}

bool MotionController::IsCrouching() {
	return crouchingState == MotionController::CrouchingState::CROUCHING ||
			crouchingState == MotionController::CrouchingState::STANDING_UP;
}

void MotionController::StartCrouching() {
	if(!IsCrouching()) {
		character->GetBody()->setCollisionShape(
				triggerLow->GetBtCollisionShape());
		sing::world->UpdateColliderForObject(character->GetBody());
	}
	AddState(State::CROUCHING);
	crouchingState = MotionController::CrouchingState::CROUCHING;
}

void MotionController::StopCrouching() {
	if(IsCrouching())
		crouchingState = MotionController::CrouchingState::STANDING_UP;
	RemoveState(State::CROUCHING);
}

void MotionController::StartSneaking() {
	AddState(State::SNEAKING);
}

void MotionController::StopSneaking() {
	RemoveState(State::SNEAKING);
}
void MotionController::StartRunning() {
	AddState(State::RUNNING);
}

void MotionController::StopRunning() {
	RemoveState(State::RUNNING);
}

MotionController::MotionController(Entity *characterEntity, float stepHeight) {
	character = NULL;
	triggerLow = NULL;
	if(characterEntity) {
		if(characterEntity->GetCollisionShape()) {
			crouchingScale = 0.6f;
			this->stepHeight = stepHeight;
			this->character = characterEntity;
			
			btVector3 aabbMin, aabbMax;
			character->GetBtCollisionShape()->getAabb(
					btTransform(
							btQuaternion(btVector3(0,1,0),0),
							btVector3(0,0,0)), aabbMin, aabbMax);
			characterStandingHeight = aabbMax.y() - aabbMin.y();
			characterCrouchingHeight = characterStandingHeight *crouchingScale;
			characterRadius = (aabbMax.x()-aabbMin.x()) *0.5f;
			
			triggerHigh = dynamic_cast<MotionControllerTrigger*>(
					sing::engine->AddEntity(sing::entityFactory->AddEntity("MotionControllerTrigger",
					sing::resourceManager->GetCapsule(characterRadius,
							characterStandingHeight),
					btTransform(btQuaternion(btVector3(1,1,1),0),
							btVector3(0,10,0)), 75.0)));
			triggerLow = dynamic_cast<MotionControllerTrigger*>(
					sing::engine->AddEntity(sing::entityFactory->AddEntity("MotionControllerTrigger",
					sing::resourceManager->GetCapsule(characterRadius,
							characterStandingHeight),
					btTransform(btQuaternion(btVector3(1,1,1),0),
							btVector3(0,10,0)), 75.0)));
			triggerHigh->Init(characterEntity, triggerLow, stepHeight);
			triggerLow->Init(characterEntity, triggerHigh, stepHeight);
			
			jumpCooldownLimit = 0.03f;
			jumpCooldown = jumpCooldownLimit;
			
			float centerHeightsDifference =
					(characterStandingHeight - characterCrouchingHeight) * 0.5f;
			triggerLowOffsetCrouching = btVector3(0,0,0);
			triggerLowOffsetStanding = btVector3(0,-centerHeightsDifference,0);
			triggerHighOffsetCrouching = btVector3(0,centerHeightsDifference,0);
			triggerHighOffsetStanding = btVector3(0,0,0);
			
			crouchingState = MotionController::CrouchingState::STANDING;
		}
		else
			MESSAGE("NULL btCollisionShape");
	}
	else
		MESSAGE("NULL Entity passed as argument");
}

MotionController::~MotionController() {
	states.clear();
	sing::engine->QueueEntityToDestroy(triggerLow);
	sing::engine->QueueEntityToDestroy(triggerHigh);
	
	character = NULL;
	triggerLow = NULL;
	triggerHigh = NULL;
}

#endif
