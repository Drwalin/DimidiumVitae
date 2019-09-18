
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "CharacterWalkTrigger.h"

#include <Entity.h>

#include <memory>

#include <vector>

class MotionController
{
protected:
	
	class Engine * engine;
	std::shared_ptr<Entity> character;
	
	//std::shared_ptr<CharacterWalkTrigger> triggerLow;
	std::shared_ptr<CharacterWalkTrigger> triggerHigh;
	float stepHeight;
	float jumpCooldownLimit;
	float jumpCooldown;
	
	enum State
	{
		WALKING,
		CROUCHING,
		RUNNING,
		SNEAKING
	};
	
	
	std::vector<State> states;
	btVector3 walkingDirection;
	
	void UpdateSpeed( const float deltaTime );
	void AddState( State state );
	void RemoveState( State state );
	
public:
	
	void Init( std::shared_ptr<Entity> characterEntity, float stepHeight = 0.3f );
	
	float GetCurrentSpeed();
	float GetJumpVelocity();
	float GetFlatDeceleration();
	
	void Tick( const float deltaTime );
	
	void MoveInDirection( btVector3 direction );
	void Jump();
	
	bool IsCrouching();
	
	void StartCrouching();
	void StopCrouching();
	void StartSneaking();
	void StopSneaking();
	void StartRunning();
	void StopRunning();
	
	void Destroy();
	
	MotionController();
	~MotionController();
};

#endif

