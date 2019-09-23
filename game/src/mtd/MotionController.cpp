
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_CPP
#define MOTION_CONTROLLER_CPP

#include <Engine.h>

#include "..\css\MotionController.h"

#include <algorithm>

void MotionController::Init( class Engine * engine, std::shared_ptr<Entity> characterEntity, float stepHeight )
{
	this->engine = engine;
	if( characterEntity )
	{
		this->engine = characterEntity->GetEngine();
		if( characterEntity->GetCollisionShape() )
		{
			this->crouchingScale = 0.6f;
			this->stepHeight = stepHeight;
			this->character = characterEntity;
			
			btVector3 aabbMin, aabbMax;
			this->character->GetCollisionShape()->getAabb( btTransform(btQuaternion(btVector3(0,1,0),0),btVector3(0,0,0)), aabbMin, aabbMax );
			this->characterStandingHeight = aabbMax.y() - aabbMin.y();
			this->characterCrouchingHeight = this->characterStandingHeight * this->crouchingScale;
			this->characterRadius = (aabbMax.x()-aabbMin.x()) * 0.5f;
			
			this->triggerHigh = std::dynamic_pointer_cast<CharacterWalkTrigger>(
					this->engine->AddEntity( engine->GetNewEntityOfType("CharacterWalkTrigger"),
					engine->GetAvailableEntityName("Trigger"),
					engine->GetCollisionShapeManager()->GetCapsule( this->characterRadius, this->characterStandingHeight ),
					btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,10,0) ), 1.0 ) );
			this->triggerLow = std::dynamic_pointer_cast<CharacterWalkTrigger>(
					this->engine->AddEntity( engine->GetNewEntityOfType("CharacterWalkTrigger"),
					engine->GetAvailableEntityName("Trigger"),
					engine->GetCollisionShapeManager()->GetCapsule( this->characterRadius, this->characterCrouchingHeight ),
					btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,10,0) ), 1.0 ) );
			this->triggerHigh->Init( characterEntity, this->triggerLow, this->stepHeight );
			this->triggerLow->Init( characterEntity, this->triggerHigh, this->stepHeight );
			
			this->jumpCooldownLimit = 0.2f;
			this->jumpCooldown = this->jumpCooldownLimit;
			
			float centerHeightsDifference = (this->characterStandingHeight - this->characterCrouchingHeight) * 0.5f;
			this->triggerLowOffsetCrouching = btVector3(0,0,0);
			this->triggerLowOffsetStanding = btVector3(0,-centerHeightsDifference,0);
			this->triggerHighOffsetCrouching = btVector3(0,centerHeightsDifference,0);
			this->triggerHighOffsetStanding = btVector3(0,0,0);
			
			this->crouchingState = MotionController::CrouchingState::STANDING;
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

void MotionController::UpdateTriggersTransform()
{
	btTransform characterTransform = this->character->GetTransform();
	btVector3 characterOrigin = characterTransform.getOrigin();
	btQuaternion characterRotation = characterTransform.getRotation();
	this->triggerLow->SetRotation( characterRotation );
	this->triggerHigh->SetRotation( characterRotation );
	if( this->IsCrouching() )
	{
		this->triggerLow->SetPosition( characterOrigin + this->triggerLowOffsetCrouching );
		this->triggerHigh->SetPosition( characterOrigin + this->triggerHighOffsetCrouching );
		if( this->character->GetCamera() )
			this->character->GetCamera()->SetRelativePosition( btVector3( 0, this->characterCrouchingHeight * 0.5f - 0.1f, 0 ) );
	}
	else
	{
		this->triggerLow->SetPosition( characterOrigin + this->triggerLowOffsetStanding );
		this->triggerHigh->SetPosition( characterOrigin + this->triggerHighOffsetStanding );
		if( this->character->GetCamera() )
			this->character->GetCamera()->SetRelativePosition( btVector3( 0, this->characterStandingHeight * 0.5f - 0.1f, 0 ) );
	}
}

float MotionController::GetFlatDeceleration()
{
	float deceleration = 1.0f;
	if( this->triggerHigh->IsBottomCollision() || this->triggerLow->IsBottomCollision() )
		deceleration = 16.0f;
	else
		deceleration = 0.1f;
	return deceleration;
}

MotionController::State MotionController::GetCurrentState() const
{
	if( this->crouchingState == MotionController::CrouchingState::CROUCHING || this->crouchingState == MotionController::CrouchingState::STANDING_UP )
		return MotionController::State::CROUCHING;
	if( this->states.size() )
	{
		return this->states.back();
	}
	return MotionController::State::WALKING;
}

float MotionController::GetCurrentSpeed()
{
	MotionController::State s = this->GetCurrentState();
	
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
	
	if( !this->triggerHigh->IsBottomCollision() || !this->triggerLow->IsBottomCollision() )
		v *= 0.2f;
	
	return v;
}

float MotionController::GetJumpVelocity()
{
	return 5.0f;
}

void MotionController::Tick( const float deltaTime )
{
	if( this->crouchingState == MotionController::CrouchingState::STANDING_UP && !this->triggerHigh->IsTopCollision() )
	{
		this->character->GetBody()->setCollisionShape( this->triggerHigh->GetCollisionShape().get() );
		this->engine->GetWorld()->UpdateColliderForObject( this->character->GetBody() );
		this->crouchingState = MotionController::CrouchingState::STANDING;
	}
	
	this->jumpCooldown += deltaTime;
	this->UpdateSpeed( deltaTime );
	this->walkingDirection = btVector3(0,0,0);
	
	if( this->triggerHigh->IsBottomCollision() || this->triggerLow->IsBottomCollision() )
		this->character->GetBody<btRigidBody>()->setFriction( 0.5 );
	else
		this->character->GetBody<btRigidBody>()->setFriction( 0 );
	
	this->UpdateTriggersTransform();
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
	return this->crouchingState == MotionController::CrouchingState::CROUCHING || this->crouchingState == MotionController::CrouchingState::STANDING_UP;
}

void MotionController::StartCrouching()
{
	if( !this->IsCrouching() )
	{
		this->character->GetBody()->setCollisionShape( this->triggerLow->GetCollisionShape().get() );
		this->engine->GetWorld()->UpdateColliderForObject( this->character->GetBody() );
	}
	this->AddState( State::CROUCHING );
	this->crouchingState = MotionController::CrouchingState::CROUCHING;
}

void MotionController::StopCrouching()
{
	if( this->IsCrouching() )
		this->crouchingState = MotionController::CrouchingState::STANDING_UP;
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
		this->engine->QueueEntityToDestroy( this->triggerLow );
		this->engine->QueueEntityToDestroy( this->triggerHigh );
		
		this->engine = NULL;
		this->character = NULL;
		this->triggerLow = NULL;
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

