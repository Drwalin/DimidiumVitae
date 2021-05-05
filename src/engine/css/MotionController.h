
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#include "MotionControllerTrigger.h"

#include <Entity.h>

#include <memory>

#include <vector>

class MotionController {
public:
	
	enum class State {
		WALKING,
		CROUCHING,
		RUNNING,
		SNEAKING
	};
	
	enum class CrouchingState {
		CROUCHING,
		STANDING,
		STANDING_UP
	};
	
	MotionController(Entity *characterEntity, float stepHeight = 0.3f);
	~MotionController();
	
	float GetCurrentSpeed();
	float GetJumpVelocity();
	float GetFlatDeceleration();
	
	void Tick(float deltaTime);
	
	void MoveInDirection(btVector3 direction);
	void Jump();
	
	bool IsCrouching();
	
	void StartCrouching();
	void StopCrouching();
	void StartSneaking();
	void StopSneaking();
	void StartRunning();
	void StopRunning();
	
protected:
	
	MotionController::State GetCurrentState() const;
	
	void UpdateSpeed(float deltaTime);
	void AddState(State state);
	void RemoveState(State state);
	
	void UpdateTriggersTransform();
	
protected:
	
	Entity *character;
	
	btVector3 triggerLowOffsetCrouching;
	btVector3 triggerLowOffsetStanding;
	btVector3 triggerHighOffsetCrouching;
	btVector3 triggerHighOffsetStanding;
	MotionControllerTrigger *triggerLow;
	MotionControllerTrigger *triggerHigh;
	float stepHeight;
	float jumpCooldownLimit;
	float jumpCooldown;
	float characterStandingHeight;
	float characterCrouchingHeight;
	float characterRadius;
	
	float crouchingScale;
	
	std::vector<State> states;
	btVector3 walkingDirection;
	
	CrouchingState crouchingState;
};

#endif

