
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_CPP
#define MOTION_CONTROLLER_CPP

#include <Engine.h>

#include "..\css\MotionController.h"

#include <algorithm>

void MotionController::Init( std::shared_ptr<Entity> characterEntity, float stepHeight )
{
	if( characterEntity )
	{
		this->engine = characterEntity->GetEngine();
		if( characterEntity->GetCollisionShape() )
		{
			auto body = characterEntity->GetBody<btRigidBody>();
			if( body )
			{
				this->triggerHigh = std::dynamic_pointer_cast<CharacterWalkTrigger>( this->engine->AddEntity( engine->GetNewEntityOfType("CharacterWalkTrigger"), engine->GetAvailableEntityName("Trigger"), characterEntity->GetCollisionShape(), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,10,0) ), 1.0 ) );
				this->triggerHigh->SetScale( btVector3(1,1,1)*1.1f );
				this->triggerHigh->Init( characterEntity, stepHeight*1.1f );
				this->character = characterEntity;
				this->jumpCooldownLimit = 0.2f;
				this->jumpCooldown = this->jumpCooldownLimit;
			}
			else
				MESSAGE("NULL btRigidBody");
		}
		else
			MESSAGE("NULL btCollisionShape");
	}
	else
		MESSAGE("NULL Entity passed as argument");
}

void MotionController::UpdateSpeed( const float deltaTime )
{
	std::shared_ptr<btRigidBody> rigidBody = this->character->GetBody<btRigidBody>();
	if( rigidBody && this->GetCurrentSpeed() > 0.0f )
	{
		btVector3 currentVelocity = rigidBody->getLinearVelocity();
		float currentSpeed = currentVelocity.length();
		if( currentSpeed >= 0.0000001f )
		{
			float newSpeed = currentSpeed - (deltaTime * this->GetFlatDeceleration());
			if( newSpeed < 0.0f )
				newSpeed = 0.0f;
			btVector3 newVelocity = currentVelocity * (newSpeed / currentSpeed);
			newVelocity.setY(currentVelocity.y());
			rigidBody->setLinearVelocity( newVelocity );
		}
		currentVelocity = rigidBody->getLinearVelocity();
		currentSpeed = currentVelocity.length();
		
		if( this->walkingDirection.length2() > 0.0001 )
		{
			this->walkingDirection.normalize();
			btVector3 destinyVelocity = this->walkingDirection * this->GetCurrentSpeed();
			float currentDot = currentVelocity.dot(this->walkingDirection);
			if( currentVelocity.length() < 0.5f || currentDot < this->GetCurrentSpeed() )
			{
				btVector3 modifyingVelocity = this->walkingDirection * ( this->GetCurrentSpeed() - currentDot ) * 0.5f;
				rigidBody->setLinearVelocity( currentVelocity + modifyingVelocity );
			}
		}
	}
}

float MotionController::GetFlatDeceleration()
{
	float deceleration = 1.0f;
	if( this->triggerHigh->IsBottomCollision() )
		deceleration = 16.0f;
	else
		deceleration = 5.0f;
	return deceleration;
}

float MotionController::GetCurrentSpeed()
{
	State s = this->states.size() ? this->states.back() : State::WALKING;
	
	float v = 1.0f;
	
	switch( s )
	{
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
	
	if( !this->triggerHigh->IsBottomCollision() )
		v *= 0.7f;
	
	return v;
}

float MotionController::GetJumpVelocity()
{
	return 5.0f;
}

void MotionController::Tick( const float deltaTime )
{
	this->jumpCooldown += deltaTime;
	this->UpdateSpeed( deltaTime );
	this->walkingDirection = btVector3(0,0,0);
}

void MotionController::MoveInDirection( btVector3 direction )
{
	this->walkingDirection += direction;
}

void MotionController::Jump()
{
	if( this->jumpCooldown >= this->jumpCooldownLimit && this->triggerHigh->IsBottomCollision() )
	{
		this->jumpCooldown = 0.0f;
		std::shared_ptr<btRigidBody> rigidBody = this->character->GetBody<btRigidBody>();
		if( rigidBody )
		{
			rigidBody->applyCentralImpulse( btVector3(0,1,0) * this->GetJumpVelocity() / rigidBody->getInvMass() );
		}
	}
}

void MotionController::AddState( State state )
{
	this->RemoveState( state );
	this->states.resize( this->states.size() + 1 );
	this->states.back() = state;
}

void MotionController::RemoveState( State state )
{
	while( true )
	{
		auto it = std::find( this->states.begin(), this->states.end(), state );
		if( it == this->states.end() )
			break;
		this->states.erase( it );	
	}
}

bool MotionController::IsCrouching()
{
	
	
	
	
	
	return false;
}

void MotionController::StartCrouching()
{
	this->AddState( State::CROUCHING );
}

void MotionController::StopCrouching()
{
	this->RemoveState( State::CROUCHING );
}

void MotionController::StartSneaking()
{
	this->AddState( State::SNEAKING );
}

void MotionController::StopSneaking()
{
	this->RemoveState( State::SNEAKING );
}
void MotionController::StartRunning()
{
	this->AddState( State::RUNNING );
}

void MotionController::StopRunning()
{
	this->RemoveState( State::RUNNING );
}

void MotionController::Destroy()
{
	if( this->engine )
	{
		this->states.clear();
		this->engine->QueueEntityToDestroy( this->triggerHigh );
		
		this->engine = NULL;
		this->character = NULL;
		this->triggerHigh = NULL;
	}
}

MotionController::MotionController()
{
	this->engine = NULL;
}

MotionController::~MotionController()
{
	this->Destroy();
}

#endif

