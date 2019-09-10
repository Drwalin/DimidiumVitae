
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DYNAMIC_ENTITY_CPP
#define DYNAMIC_ENTITY_CPP

#include "..\css\DynamicEntity.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void DynamicEntity::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	Entity::EventOnEntityBeginOverlapp( other, persisstentManifold );
}

void DynamicEntity::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	Entity::EventOnEntityTickOverlapp( other, persisstentManifold );
}

void DynamicEntity::EventOnEntityEndOverlapp( Entity * other )
{
	Entity::EventOnEntityEndOverlapp( other );
}

void DynamicEntity::Tick( const float deltaTime )
{
	Entity::Tick( deltaTime );
}

void DynamicEntity::ApplyDamage( const float damage, btVector3 point, btVector3 normal )
{
	Entity::ApplyDamage( damage, point, normal );
}

void DynamicEntity::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	Entity::ApplyImpactDamage( damage, impetus, direction, point, normal );
}

void DynamicEntity::Load( std::istream & stream )
{
	Entity::Load( stream );
}

void DynamicEntity::Save( std::ostream & stream ) const
{
	Entity::Save( stream );
}

void DynamicEntity::Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Entity::Spawn( name, shape, transform );
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateRigidBody( shape, transform, 1.0f );
	std::shared_ptr<btRigidBody> rigidBody = std::dynamic_pointer_cast<btRigidBody>( collisionObject );
	
	rigidBody->setDamping( 0.2, 0.2 );
	rigidBody->setFriction( 0.75 );
	
	this->SetBody( collisionObject, shape );
}

void DynamicEntity::Despawn()
{
	Entity::Despawn();
}

void DynamicEntity::Destroy()
{
	Entity::Destroy();
}

extern "C" std::shared_ptr<Entity> GetClassInstantiator(){ static std::shared_ptr<Entity> instantiator( new DynamicEntity(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int DynamicEntity::GetTypeSize() const{ return sizeof(DynamicEntity); }
void DynamicEntity::Free(){ delete this; }
std::shared_ptr<Entity> DynamicEntity::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<DynamicEntity>( new DynamicEntity(), [](Entity * ptr){delete ptr;} ) ); }
std::string DynamicEntity::GetClassName() const{ return "DynamicEntity"; }

DynamicEntity::DynamicEntity() :
	Entity()
{
}

DynamicEntity::~DynamicEntity()
{
	this->Destroy();
}

#endif

