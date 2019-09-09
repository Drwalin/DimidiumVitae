
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_EVENT_CPP
#define CHARACTER_EVENT_CPP

#include "..\css\Character.h"

#include <Engine.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void Character::EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
	Object::EventOnObjectBeginOverlapp( other, persisstentManifold );
}

void Character::EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
	Object::EventOnObjectTickOverlapp( other, persisstentManifold );
}

void Character::EventOnObjectEndOverlapp( Object * other )
{
	Object::EventOnObjectEndOverlapp( other );
}

void Character::EventJump()
{
	static float lastJumpedMoment = 0.0f;
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
	if( rigidBody )
	{
		if( (float(clock())/1000.0f) - lastJumpedMoment > 0.2f )
		{
			lastJumpedMoment = (float(clock())/1000.0f);
			rigidBody->applyCentralImpulse( this->GetJumpVelocity() );
		}
	}
}

void Character::EventMoveInDirection( const btVector3 & direction, bool flat )
{
	btVector3 dir = direction;
	
	if( flat )
		dir.m_floats[1] = 0.0f;
	
	dir.normalize();
	
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
	
	if( rigidBody )
	{
		float velocity = GetMovementVelocity();
		if( rigidBody->getLinearVelocity().dot( dir ) < velocity )
		{
			rigidBody->applyCentralForce( dir * 60.0f * mass );
		}
	}
}

void Character::EventRotateCameraBy( const btVector3 & rotation )
{
	cameraRotation += rotation;
	
	CorrectCameraRotation();
	
	if( camera )
	{
		camera->SetRotation( cameraRotation );
	}
}

void Character::EventRotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth )
{
	CorrectCameraRotation();
	
	btVector3 dstCameraRotation(0,0,0);
	{
		btVector3 dstLookingDirection = (worldPoint - GetCameraLocation()).normalized();
		btVector3 forwardVector = GetForwardVector().normalized();
		btVector3 flatForward = forwardVector;
		flatForward.m_floats[1] = 0.0f;
		flatForward.normalize();
		btVector3 flatDstLookingDirection = dstLookingDirection;
		flatDstLookingDirection.m_floats[1] = 0.0f;
		flatDstLookingDirection.normalize();
		
		float dot = flatForward.normalized().dot( flatDstLookingDirection.normalized() );
		dstCameraRotation.m_floats[1] = ( ( dot <= -1 || dot >= 1 ) ? 0.001f : acos( dot ) ) * ( flatDstLookingDirection.dot( GetFlatLeftVector() ) > 0.0f ? -1.0f : 1.0f );
		dot = forwardVector.normalized().dot( ( MakeTransformFromEuler( btVector3( 0, -dstCameraRotation.y(), 0 ) ) * dstLookingDirection ).normalized() );
		dstCameraRotation.m_floats[0] = ( ( dot <= -1 || dot >= 1 ) ? 0.001f : acos( dot ) ) * ( dstLookingDirection.y() > forwardVector.y() ? -1.0f : 1.0f );
	}
	
	if( smooth )
	{
		cameraRotation += dstCameraRotation * engine->GetDeltaTime() * 3.11f * Math::PI;
	}
	else
	{
		cameraRotation += dstCameraRotation;
	}
	
	CorrectCameraRotation();
	
	if( camera )
	{
		camera->SetRotation( cameraRotation );
	}
}

#endif

