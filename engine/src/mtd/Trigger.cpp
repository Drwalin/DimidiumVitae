
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_CPP
#define TRIGGER_CPP

#include "..\css\Trigger.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Math.hpp>

#include <cassert>

void Trigger::ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject)
{
}

void Trigger::NextOverlappingFrame()
{
	std::shared_ptr<btGhostObject> ghostObject = std::dynamic_pointer_cast<btGhostObject>( body );
	for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++)
	{
		btCollisionObject *body = ghostObject->getOverlappingObject(i);
		if( body )
		{
			Entity * objectT = (Entity*)(body->getUserPointer());
			if( objectT ) {
				this->ProcessOverlappingEntity(objectT, body);
			}
		}
	}
	this->engine->GetWindow()->GetGUI() << "\n Trigger overlapps: " << ghostObject->getNumOverlappingObjects();
}

void Trigger::Tick( const float deltaTime )
{
	if( body )
	{
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
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateGhostObject( shape, transform );
	collisionObject->setCollisionFlags( collisionObject->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT );
	
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


