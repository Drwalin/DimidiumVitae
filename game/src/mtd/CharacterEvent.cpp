
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
	/*
	int numberOfContacts = perisstentManifold->getNumContacts();
	
	float highestBottom = GetBottomY()-1.0f;
	float height = GetCurrentHeight();
	
    for( int i = 0; i < numberOfContacts; i++ )
    {
        btManifoldPoint & pt = perisstentManifold->getContactPoint( i );
        
        const btVector3 & ptA = pt.getPositionWorldOnA();
        const btVector3 & ptB = pt.getPositionWorldOnB();
        
        btVector3 point = ( ptA + ptB ) * 0.5f;
        
        btVector3 normal = pt.m_normalWorldOnB;
        normal.normalized();
        if( normal.y() < 0.0 )
        	normal.m_floats[1] = -(normal.m_floats[1]);
        
    	if( point.y() > highestBottom )		// check if it's on bottom not on top
    	{
    		highestBottom = point.y();
    	}
    	
        if( acos( normal.dot( btVector3(0,1,0) ) ) < Math::PI * 40.0f / 180.0f )
        {
        	if( point.y() < GetBottomY() )
        	{
        		isInAir = false;
        		if( (float(clock())/1000.0f) - lastTimeInAir > engine->GetDeltaTime()*2.0f )
        		{
					body->setDamping( 0.8, 0.0 );
					body->setFriction( 0.8 );
				}
        	}
        }
    }
    
    // move by step:
    if( highestBottom < GetBottomY() + ( height * ( 0.3f / 1.75f ) ) && highestBottom > GetBottomY() )
    {
    	QueueMove( ( highestBottom - GetBottomY() ) * 1.0f + 0.16f );
    }
    */
}

void Character::EventOnObjectEndOverlapp( Object * other )
{
	Object::EventOnObjectEndOverlapp( other );
}

void Character::EventJump()
{
	static float lastJumpedMoment = 0.0f;
	if( !isInAir )
	{
		if( body )
		{
			if( (float(clock())/1000.0f) - lastJumpedMoment > 0.08f )
			{
				lastJumpedMoment = (float(clock())/1000.0f);
				body->applyCentralImpulse( GetJumpVelocity() );
			}
		}
	}
}

void Character::EventCrouch()
{
	if( walkMode != Character::WalkMode::CROUCH )
	{
		previousWalkMode = walkMode;
		walkMode = Character::WalkMode::CROUCH;
		SetScale( btVector3( 1.0, 0.5, 1.0 ) );
		SetCameraLocation( btVector3( 0.0, GetCurrentHeight() * 0.5 * 0.9, 0.0 ) );
	}
}

void Character::EventStandUp()
{
	if( walkMode == Character::WalkMode::CROUCH )
	{
		walkMode = previousWalkMode;
		previousWalkMode = Character::WalkMode::WALK;
		SetScale( btVector3( 1.0, 1.0, 1.0 ) );
		SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
	}
}

void Character::EventBeginRun()
{
	switch( walkMode )
	{
	case Character::WalkMode::WALK:
		previousWalkMode = Character::WalkMode::WALK;
		walkMode = Character::WalkMode::RUN;
		break;
	case Character::WalkMode::STRAVAGE:
		previousWalkMode = Character::WalkMode::STRAVAGE;
		walkMode = Character::WalkMode::RUN;
		break;
	}
}

void Character::EventStopRun()
{
	if( walkMode == Character::WalkMode::RUN )
	{
		walkMode = previousWalkMode;
		previousWalkMode = Character::WalkMode::WALK;
	}
}

void Character::EventBeginStravage()
{
	switch( walkMode )
	{
	case Character::WalkMode::WALK:
		previousWalkMode = Character::WalkMode::WALK;
		walkMode = Character::WalkMode::STRAVAGE;
		break;
	case Character::WalkMode::RUN:
		previousWalkMode = Character::WalkMode::RUN;
		walkMode = Character::WalkMode::STRAVAGE;
		break;
	}
}

void Character::EventStopStravage()
{
	if( walkMode == Character::WalkMode::STRAVAGE )
	{
		walkMode = previousWalkMode;
		previousWalkMode = Character::WalkMode::WALK;
	}
}

void Character::EventMoveInDirection( const btVector3 & direction, bool flat )
{
	btVector3 dir = direction;
	
	if( flat )
		dir.m_floats[1] = 0.0f;
	
	dir.normalize();
	
	if( body )
	{
		float velocity = GetMovementVelocity();
		if( body->getLinearVelocity().dot( dir ) < velocity )
		{
			body->applyCentralForce( dir * 60.0f * mass );
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

