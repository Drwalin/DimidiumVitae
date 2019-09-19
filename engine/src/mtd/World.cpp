
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WORLD_CPP
#define WORLD_CPP

#include "..\css\World.h"

#include "..\lib\Debug.h"

void World::ActivateAll()
{
	++this->activateAll;
	if( this->activateAll <= 0 )
		this->activateAll = 1;
	else if( this->activateAll > 2 )
		this->activateAll = 2;
}

btDiscreteDynamicsWorld * World::GetDynamicsWorld()
{
	return this->dynamicsWorld;
}

inline void World::UpdateObjectsActivation()
{
	if( this->activateAll > 0 )
	{
		auto it = this->object.find( this->currentActivator );
		if( it == this->object.end() )
			it = this->object.begin();
		
		for( int i = 0; i < 113; ++i, ++it )
		{
			if( it == this->object.end() )
				break;
			(*it)->activate( true );
		}
		
		if( it != this->object.end() )
		{
			this->currentActivator = *it;
		}
		else
		{
			--this->activateAll;
			this->currentActivator = NULL;
		}
	}
}

void World::Tick( btScalar deltaTime, int count )
{
	this->UpdateObjectsActivation();
	
	if( count > 0 )
		this->dynamicsWorld->stepSimulation( deltaTime, count );
	else
		this->dynamicsWorld->stepSimulation( deltaTime );
}

void World::UpdateColliderForObject( std::shared_ptr<btCollisionObject> body )
{
	this->dynamicsWorld->getCollisionWorld()->updateSingleAabb( body.get() );
}

btVector3 World::GetGravity()
{
	return this->dynamicsWorld->getGravity();
}

void World::Init()
{
	this->Destroy();
	this->broadphase = new btDbvtBroadphase();
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher( this->collisionConfiguration );
	this->solver = new btSequentialImpulseConstraintSolver();
	this->dynamicsWorld = new btDiscreteDynamicsWorld( this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration );
	this->dynamicsWorld->setGravity( btVector3(0, -20, 0) );
}

bool World::AddBody( std::shared_ptr<btCollisionObject> body, int collisionFilterGroup, int collisionFilterMask )
{
	if( body )
	{
		if( this->object.find(body) == this->object.end() )
		{
			std::shared_ptr<btRigidBody> rigid = std::dynamic_pointer_cast<btRigidBody>( body );
			if( rigid )
				this->dynamicsWorld->addRigidBody( rigid.get(), collisionFilterGroup, collisionFilterMask );
			else
				this->dynamicsWorld->addCollisionObject( body.get(), collisionFilterGroup, collisionFilterMask );
			this->object.insert( body );
			body->activate();
		}
		return true;
	}
	return false;
}

bool World::RemoveBody( std::shared_ptr<btCollisionObject> body )
{
	auto it = this->object.find( body );
	if( it != this->object.end() )
	{
		(*it)->activate();
		std::shared_ptr<btRigidBody> rigid = std::dynamic_pointer_cast<btRigidBody>( *it );
		if( rigid )
			this->dynamicsWorld->removeRigidBody( rigid.get() );
		else
			this->dynamicsWorld->removeCollisionObject( it->get() );
		this->object.erase( it );
	}
	return false;
}

void World::RemoveBodys()
{
	for( auto it = this->object.begin(); it != this->object.end(); ++it )
	{
		std::shared_ptr<btRigidBody> rigid = std::dynamic_pointer_cast<btRigidBody>( *it );
		if( rigid )
			this->dynamicsWorld->removeRigidBody( rigid.get() );
		else
			this->dynamicsWorld->removeCollisionObject( it->get() );
	}
	this->object.clear();
}

void World::Destroy()
{
	this->RemoveBodys();
	
	if( this->dynamicsWorld )
		delete this->dynamicsWorld;
	this->dynamicsWorld = NULL;
	if( this->solver )
		delete this->solver;
	this->solver = NULL;
	if( this->collisionConfiguration )
		delete this->collisionConfiguration;
	this->collisionConfiguration = NULL;
	if( this->dispatcher )
		delete this->dispatcher;
	this->dispatcher = NULL;
	if( this->broadphase )
		delete this->broadphase;
	this->broadphase = NULL;
}

World::World()
{
	this->activateAll = 0;
	this->broadphase = NULL;
	this->collisionConfiguration = NULL;
	this->dispatcher = NULL;
	this->solver = NULL;
	this->dynamicsWorld = NULL;
}

World::~World()
{
	this->Destroy();
}

#endif

