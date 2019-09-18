
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STATIC_ENTITY_CPP
#define STATIC_ENTITY_CPP

#include "..\css\StaticEntity.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void StaticEntity::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * persisstentManifold ){}
void StaticEntity::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * persisstentManifold ){}
void StaticEntity::EventOnEntityEndOverlapp( Entity * other ){}

void StaticEntity::Tick( const float deltaTime )
{
	Entity::Tick( deltaTime );
	/*
	if( clock()-c > 10000)
	{
		btTransform t;
		this->GetBody<btRigidBody>()->getMotionState()->getWorldTransform( t );
		std::cout << "\n Transform: " << t << "   scale: " << this->GetScale();
		c = clock();
	}
	*/
}

void StaticEntity::ApplyDamage( const float damage, btVector3 point, btVector3 normal ){}
void StaticEntity::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ){}

void StaticEntity::Load( std::istream & stream )
{
	Entity::Load( stream );
}

void StaticEntity::Save( std::ostream & stream ) const
{
	Entity::Save( stream );
}

void StaticEntity::Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Entity::Spawn( self, name, shape, transform );
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateRigidBody( shape, transform, 1.0f );
	std::shared_ptr<btRigidBody> rigidBody = std::dynamic_pointer_cast<btRigidBody>( collisionObject );
	
	rigidBody->setDamping( 1.0, 1.0 );
	rigidBody->setFriction( 0.75 );
	rigidBody->setAngularFactor( 0.0 );
	rigidBody->setLinearFactor( btVector3( 0.0, 0.0, 0.0 ) );
	//rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT );
	rigidBody->setGravity( btVector3(0.0f,0.0f,0.0f) );
	
	this->rayTraceChannel = Engine::RayTraceChannel::NONE;
	
	this->SetBody( collisionObject, shape );
}

void StaticEntity::Despawn()
{
	Entity::Despawn();
}

void StaticEntity::Destroy()
{
	Entity::Destroy();
}

extern "C" std::shared_ptr<Entity> GetClassInstantiator(){ static std::shared_ptr<Entity> instantiator( new StaticEntity(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int StaticEntity::GetTypeSize() const{ return sizeof(StaticEntity); }
void StaticEntity::Free(){ delete this; }
std::shared_ptr<Entity> StaticEntity::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<StaticEntity>( new StaticEntity(), [](Entity * ptr){delete ptr;} ) ); }
std::string StaticEntity::GetClassName() const{ return "StaticEntity"; }

StaticEntity::StaticEntity() :
	Entity()
{
	c = clock();
}

StaticEntity::~StaticEntity()
{
	this->Destroy();
}

#endif

