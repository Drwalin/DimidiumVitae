
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "..\css\Character.h"
#include "..\css\CharacterWalkTrigger.h"

#include <Engine.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void Character::UpdateStepUp()
{
	if( walkTriggerStep )
	{
		CharacterWalkTrigger * trigger = dynamic_cast<CharacterWalkTrigger*>( walkTriggerStep.get() );
		if( trigger->IsAnyInside() )
		{
//			Move( btVector3(0.0,0.1,0.0) );
		}
	}
	
	
	
	
}

void Character::UpdateIsInAir()
{
	isInAir = true;
	if( walkTriggerBottom )
	{
		CharacterWalkTrigger * trigger = dynamic_cast<CharacterWalkTrigger*>( walkTriggerBottom.get() );
		if( trigger )
		{
			if( trigger->IsAnyInside() )
			{
				MESSAGE( std::string("Yes: ") + std::to_string(trigger->GetLocation().y()) );
			}
			else
			{
				MESSAGE( std::string("No: ") + std::to_string(trigger->GetLocation().y()) );
			}
			
			isInAir = !trigger->IsAnyInside();
		}
		else
		{
			MESSAGE( "Unable to convert trigger" );
		}
	}
	else
	{
		MESSAGE( "walkTriggerBottom not exist" );
	}
}

void Character::UpdateWalkTriggersLocation( const float deltaTime )
{
	if( body )
	{
		float height = GetCurrentHeight();
		
		btVector3 velocity3D = body->getLinearVelocity();
		btVector3 velocity2D = velocity3D;
		velocity2D.setY( 0.0f );
		
		btVector3 step3D = velocity3D * deltaTime;
		
		btVector3 location = GetLocation();
		btVector3 bottomLocation = location;
		bottomLocation.setY( GetBottomY() );
		
		if( walkTriggerBottom )
		{
			walkTriggerBottom->SetPosition( bottomLocation );
		}
		else
		{
			MESSAGE( "No walkTriggerBottom" );
		}
		
		if( walkTriggerStep )
		{
			walkTriggerStep->SetPosition( bottomLocation + step3D + btVector3( 0.0f, height * 0.085f * 0.5f, 0.0f ) );
		}
		else
		{
			MESSAGE( "No walkTriggerStep" );
		}
		
		if( walkTriggerBody )
		{
			walkTriggerBody->SetPosition( location + step3D + btVector3( 0.0f, height * ( 0.085f + 0.915f * 0.5f ), 0.0f ) );
		}
		else
		{
			MESSAGE( "No walkTriggerBody" );
		}
	}
	else
	{
		MESSAGE( "No body" );
	}
}

void Character::SpawnWalkTriggers()
{
	if( body && engine && collisionShape )
	{
		float height = -1.0, radius;
		
		{
			btCylinderShape * cylinder = dynamic_cast<btCylinderShape*>( collisionShape.get() );
			if( cylinder )
			{
				height = cylinder->getHalfExtentsWithMargin().y() * 2.0f;
				radius = cylinder->getHalfExtentsWithMargin().x();
			}
		}
		
		if( height > 0.0f )
		{
			if( !walkTriggerBottom )
			{
				walkTriggerBottom = engine->AddTrigger<CharacterWalkTrigger>( engine->GetAvailableObjectName("CharacterWalkTriggerBottom"), engine->GetCollisionShapeManager()->GetCylinder( radius, 0.2f, engine->GetCollisionShapeManager()->GetFirstAvailableName("CharacterWalkTriggerBottom") ), btTransform() );
				if( walkTriggerBottom )
				{
					((CharacterWalkTrigger*)walkTriggerBottom.get())->SetParent( GetThis() );
				}
			}
			
			if( !walkTriggerStep )
			{
				walkTriggerStep = engine->AddTrigger<CharacterWalkTrigger>( engine->GetAvailableObjectName("CharacterWalkTriggerStep"), engine->GetCollisionShapeManager()->GetCylinder( radius, height * 0.085f, engine->GetCollisionShapeManager()->GetFirstAvailableName("CharacterWalkTriggerStep") ), btTransform() );
				if( walkTriggerStep )
				{
					((CharacterWalkTrigger*)walkTriggerStep.get())->SetParent( GetThis() );
				}
			}
			
			if( !walkTriggerBody )
			{
				walkTriggerBody = engine->AddTrigger<CharacterWalkTrigger>( engine->GetAvailableObjectName("CharacterWalkTriggerBody"), engine->GetCollisionShapeManager()->GetCylinder( radius, height * 0.915f, engine->GetCollisionShapeManager()->GetFirstAvailableName("CharacterWalkTriggerBody") ), btTransform() );
				if( walkTriggerBody )
				{
					((CharacterWalkTrigger*)walkTriggerBody.get())->SetParent( GetThis() );
				}
			}
			
			UpdateWalkTriggersLocation( 0.01f );
		}
		else
		{
			MESSAGE( std::string("Unable to get cylinder from collisionShape in Character: ") + name );
		}
	}
	else
	{
		MESSAGE( std::string("body or engine or collisionShape is nullptr in Character: ") + name );
	}
}



void Character::QueueMove( float val )
{
	if( queueStep < val )
		queueStep = val;
}

float Character::GetCurrentHeight() const
{
	float height = this->height;
	if( collisionShape )
	{
		btCylinderShape * cylinder = dynamic_cast<btCylinderShape*>( collisionShape.get() );
		if( cylinder )
		{
			height = cylinder->getHalfExtentsWithMargin().y() * 2.0f;
		}
	}
	
	if( walkMode == Character::WalkMode::CROUCH )
		height *= 0.5f;
	
	return height;
}

void Character::SetScale( btVector3 scale )
{
	Object::SetScale( scale );
	
	if( walkTriggerBottom )
		walkTriggerBottom->SetScale( scale );
	if( walkTriggerBody )
		walkTriggerBody->SetScale( scale );
	if( walkTriggerStep )
		walkTriggerStep->SetScale( scale );
}

void Character::NextOverlappingFrame()
{
	Object::NextOverlappingFrame();
	if( isInAir )
		lastTimeInAir = float(clock())/1000.0f;
	isInAir = true;
	body->setDamping( 0.1, 0.0 );
	body->setFriction( 0.0 );
}

float Character::GetBottomY() const
{
	if( body )
	{
		btVector3 min, max, origin;
		body->getAabb( min, max );
		return min.y();
		//return (min.y()*0.85) + (GetLocation().y()*0.15);
	}
	return currentTransform.getOrigin().y();
}

btTransform Character::MakeTransformFromEuler( const btVector3 & euler )
{
	btQuaternion quat( btVector3( 0, 1, 0 ), -euler.y() );
	quat *= btQuaternion( btVector3( 1, 0, 0 ), euler.x() );
	quat *= btQuaternion( btVector3( 0, 0, 1 ), euler.z() );
	return btTransform( quat );
}

float Character::GetMovementVelocity() const
{
	float velocity = defaultVelocity;
	
	if( isInAir )
		velocity *= 0.08f;
	
	switch( walkMode )
	{
	case Character::WalkMode::CROUCH:
		velocity *= 0.45f;
		break;
	case Character::WalkMode::STRAVAGE:
		velocity *= 0.4f;
		break;
	case Character::WalkMode::WALK:
		velocity *= 1.0f;
		break;
	case Character::WalkMode::RUN:
		velocity *= 1.3f;
		break;
	}
	
	return velocity;
}

btVector3 Character::GetJumpVelocity() const
{
	if( body )
	{
		btVector3 vel = body->getLinearVelocity();
		vel.m_floats[1] = 0;
		
		
		return ( btVector3( 0, sqrt( ( ( body->getGravity().length() ) * jumpHeight * 0.5f ) + ( vel.length() * 0.07 ) ), 0 ) * 35.0 ) + ( vel * 0.04 );
	}
	MESSAGE( "Shouldn't appear" );
	return btVector3(0,2.5f,0);
}



void Character::CorrectCameraRotation()
{
	if( cameraRotation.m_floats[0] < -Math::PI*0.5f )
		cameraRotation.m_floats[0] = -Math::PI*0.5f;
	else if( cameraRotation.m_floats[0] > Math::PI*0.5f )
		cameraRotation.m_floats[0] = Math::PI*0.5f;
	
	while( cameraRotation.m_floats[1] >= Math::PI * 2.0f )
		cameraRotation.m_floats[1] -= Math::PI * 2.0f;
	while( cameraRotation.m_floats[1] < 0.0f )
		cameraRotation.m_floats[1] += Math::PI * 2.0f;
}

void Character::SetCamera( std::shared_ptr<Camera> camera )
{
	this->camera = camera;
}

void Character::SetCameraLocation( const btVector3 & location )
{
	cameraLocation = location;
	
	if( camera )
	{
		camera->SetPos( cameraLocation );
	}
}

void Character::SetCameraRotation( const btVector3 & rotation )
{
	cameraRotation = rotation;
	
	CorrectCameraRotation();
	
	if( camera )
	{
		camera->SetRotation( cameraRotation );
	}
}

btVector3 Character::GetCameraLocation() const
{
	return GetLocation() + cameraLocation;
}

btVector3 Character::GetForwardVector() const
{
	return ( Character::MakeTransformFromEuler( btVector3( cameraRotation.x(), cameraRotation.y(), 0.0 ) ) * btVector3(0.0,0.0,-1.0) ) * btVector3( 1, -1, 1 );
}

btVector3 Character::GetFlatForwardVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), 0.0 ) ) * btVector3(0.0,0.0,-1.0);
}

btVector3 Character::GetLeftVector() const/////////////////////////////
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), cameraRotation.z() ) ) * btVector3(-1.0,0.0,0.0);
}

btVector3 Character::GetFlatLeftVector() const
{
	return Character::MakeTransformFromEuler( btVector3( 0.0, cameraRotation.y(), 0.0 ) ) * btVector3(-1.0,0.0,0.0);
}


void Character::Tick( const float deltaTime )
{
	if( !( walkTriggerBottom && walkTriggerBody && walkTriggerStep ) )
		SpawnWalkTriggers();
	
	UpdateIsInAir();
	UpdateWalkTriggersLocation( deltaTime );
	UpdateStepUp();
	
	Object::Tick( deltaTime );
	
	/*
	if( queueStep >= 0.001f )
	{
		MESSAGE( std::string("queuedMove.y() = ") + std::to_string(queueStep) );
		Move( btVector3( 0.0f, queueStep, 0.0f ) );
		if( body )
		{
			body->setLinearVelocity( // body->getLinearVelocity() +
			 previousVelocity + btVector3( 0.0, queueStep / ( deltaTime * 103.0f ), 0.0 ) );
		}
		else
		{
			DEBUG( "No body" );
		}
		queueStep = 0.0f;
	}
	//else
	 if( body )
	{
		previousVelocity = body->getLinearVelocity();
		previousVelocity.setY( 0.0f );
	}
	else
	{
		DEBUG( "No body" );
	}
	*/
}

void Character::ApplyDamage( const float damage, btVector3 point, btVector3 normal )
{
	
	
	
}

void Character::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	Object::ApplyImpactDamage( damage, impetus, direction, point, normal );
	Character::ApplyDamage( damage, point, normal );
}




Character::Character( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
	Object( engine, name, body, collisionShape, mass_ ),
	cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(3.7), jumpHeight(1.0), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK),
	isInAir(true),
	lastTimeInAir(0.0),
	queueStep(0.0)
{
	height = GetCurrentHeight();
	SetCameraLocation( btVector3( 0.0, height * 0.5 * 0.9, 0.0 ) );
}

Character::Character() :
	Object(), cameraRotation(0,0,0), cameraLocation(0,0,0),
	defaultVelocity(3.7), jumpHeight(1.0), height(1.75),
	walkMode(Character::WalkMode::WALK), previousWalkMode(Character::WalkMode::WALK),
	isInAir(true),
	lastTimeInAir(0.0),
	queueStep(0.0)
{
	SetCameraLocation( btVector3( 0.0, GetCurrentHeight() * 0.5 * 0.9, 0.0 ) );
}

Character::~Character()
{
}

#endif

