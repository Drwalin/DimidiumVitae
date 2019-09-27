
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include "..\css\Trigger.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Math.hpp>

#include <cassert>

void Trigger::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * perisstentManifold ){}
void Trigger::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * perisstentManifold ){}
void Trigger::EventOnEntityEndOverlapp( Entity * other ){}
void Trigger::ApplyDamage( const float damage, btVector3 point, btVector3 normal ){}
void Trigger::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ){}

void Trigger::Tick( const float deltaTime )
{
	if( body )
	{
		std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
		if( rigidBody )
		{
			rigidBody->setLinearVelocity( btVector3(0.0,0.0,0.0) );
			rigidBody->setAngularVelocity( btVector3(0.0,0.0,0.0) );
		}
		body->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForObject( body );
	}
}

void Trigger::Load( std::istream & stream )
{
	Entity::Load( stream );
}

void Trigger::Save( std::ostream & stream ) const
{
	Entity::Save( stream );
}

void Trigger::Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Entity::Spawn( self, name, shape, transform );
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateRigidBody( shape, transform, 1.0f );
	std::shared_ptr<btRigidBody> rigidBody = std::dynamic_pointer_cast<btRigidBody>( collisionObject );
	
	rigidBody->setDamping( 1.0, 1.0 );
	rigidBody->setFriction( 0.0 );
	rigidBody->setAngularFactor( 0.0 );
	rigidBody->setLinearFactor( btVector3( 0.0, 0.0, 0.0 ) );
	rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT );
	rigidBody->setGravity( btVector3(0.0f,0.0f,0.0f) );
	
	this->rayTraceChannel = Engine::RayTraceChannel::NONE;
	
	this->SetBody( collisionObject, shape );
}

void Trigger::Despawn()
{
	Entity::Despawn();
}

void Trigger::Destroy()
{
	Entity::Destroy();
}

extern "C" std::shared_ptr<Entity> GetTriggerInstantiator(){ static std::shared_ptr<Entity> instantiator( new Trigger(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int Trigger::GetTypeSize() const{ return sizeof(Trigger); }
void Trigger::Free(){ delete this; }
std::shared_ptr<Entity> Trigger::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<Trigger>( new Trigger(), [](Entity * ptr){delete ptr;} ) ); }
std::string Trigger::GetClassName() const{ return "Trigger"; }

Trigger::Trigger()
{
}

Trigger::~Trigger()
{
}

#endif


