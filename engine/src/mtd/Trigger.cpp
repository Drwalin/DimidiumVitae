
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include "..\css\Trigger.h"
#include "..\css\Engine.h"

#include "..\lib\Math.hpp"

#include <cassert>

bool Trigger::IsTrigger() const
{
	return true;
}

void Trigger::SetModel( std::shared_ptr<Model> model, bool light )
{
}

void Trigger::SetMass( float mass )
{
}

bool Trigger::IsDynamic() const
{
	return false;
}

void Trigger::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * perisstentManifold )
{
}

void Trigger::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * perisstentManifold )
{
}

void Trigger::EventOnEntityEndOverlapp( Entity * other )
{
}

void Trigger::Tick( const float deltaTime )
{
	previousTransform = currentTransform;
	if( body )
	{
		body->setLinearVelocity( btVector3(0.0,0.0,0.0) );
		body->setAngularVelocity( btVector3(0.0,0.0,0.0) );
		body->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForObject( body );
	}
}

Trigger::Trigger( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
	Entity( engine, name, body, collisionShape, mass_ )
{
}

Trigger::Trigger()
{
}

Trigger::~Trigger()
{
}

#endif


