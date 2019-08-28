
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WORLD_CPP
#define WORLD_CPP

#include <World.h>
#include <Debug.h>

void World::ActivateAll()
{
	++activateAll;
	if( activateAll <= 0 )
		activateAll = 1;
	else if( activateAll > 2 )
		activateAll = 2;
}

btDiscreteDynamicsWorld * World::GetDynamicsWorld()
{
	return dynamicsWorld;
}

inline void World::UpdateObjectsActivation()
{
	if( activateAll > 0 )
	{
		auto it = object.find( currentActivator );
		if( it == object.end() )
			it = object.begin();
		
		for( int i = 0; i < 113; ++i, ++it )
		{
			if( it == object.end() )
				break;
			it->second->activate( true );
		}
		
		if( it != object.end() )
		{
			currentActivator = it->first;
		}
		else
		{
			--activateAll;
			currentActivator = "";
		}
	}
}

void World::Tick( btScalar deltaTime, int count )
{
	UpdateObjectsActivation();
	
	if( count > 0 )
		dynamicsWorld->stepSimulation( deltaTime, count );
	else
		dynamicsWorld->stepSimulation( deltaTime );
}

void World::UpdateColliderForObject( std::shared_ptr<btRigidBody> body )
{
	dynamicsWorld->getCollisionWorld()->updateSingleAabb( (btCollisionObject*)body.get() );
}

void World::UpdateColliderForTrigger( std::shared_ptr<btPairCachingGhostObject> body )
{
	dynamicsWorld->getCollisionWorld()->updateSingleAabb( (btCollisionObject*)body.get() );
}

btVector3 World::GetGravity()
{
	return dynamicsWorld->getGravity();
}

void World::Init()
{
	Destroy();
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher( collisionConfiguration );
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfiguration );
	dynamicsWorld->setGravity( btVector3(0, -14.81, 0) );
}

bool World::AddBody( std::string name, std::shared_ptr<btRigidBody> body )
{
	if( body )
	{
		if( object.find(name) != object.end() )
		{
			return false;
			//DeleteObject( name );			////////////////////////////////////////////////////////////////////////
		}
		else
		{
			dynamicsWorld->addRigidBody( (btRigidBody*)body.get() );
			object[name] = body;
			return true;
		}
	}
	return false;
}

bool World::AddTrigger( std::string name, std::shared_ptr<btPairCachingGhostObject> body )
{
	if( body )
	{
		if( trigger.find(name) != trigger.end() )
		{
			return false;
			//DeleteTrigger( name );			////////////////////////////////////////////////////////////////////////
		}
		else
		{
			dynamicsWorld->addCollisionObject( (btCollisionObject*)body.get() );
			trigger[name] = body;
			return true;
		}
	}
	return false;
}

void World::RemoveBody( std::string name )
{
	auto it = object.find(name);
	if( it != object.end() )
	{
		if( it->second )
		{
			ActivateAll();
			it->second->activate( true );
			dynamicsWorld->removeRigidBody( (btRigidBody*)(it->second.get()) );
		}
		object.erase( it );
	}
}

void World::RemoveTrigger( std::string name )
{
	auto it = trigger.find(name);
	if( it != trigger.end() )
	{
		if( it->second )
		{
			dynamicsWorld->removeCollisionObject( (btCollisionObject*)(it->second.get()) );
		}
		trigger.erase( it );
	}
}

void World::RemoveBodys()
{
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			dynamicsWorld->removeRigidBody( (btRigidBody*)(it->second.get()) );
		}
	}
	object.clear();
	
	for( auto it = trigger.begin(); it != trigger.end(); ++it )
	{
		if( it->second )
		{
			dynamicsWorld->removeCollisionObject( (btCollisionObject*)(it->second.get()) );
		}
	}
	trigger.clear();
}

void World::Destroy()
{
	RemoveBodys();
	
	if( dynamicsWorld )
		delete dynamicsWorld;
	dynamicsWorld = NULL;
	if( solver )
		delete solver;
	solver = NULL;
	if( collisionConfiguration )
		delete collisionConfiguration;
	collisionConfiguration = NULL;
	if( dispatcher )
		delete dispatcher;
	dispatcher = NULL;
	if( broadphase )
		delete broadphase;
	broadphase = NULL;
}

World::World() :
	activateAll(0)
{
	broadphase = NULL;
	collisionConfiguration = NULL;
	dispatcher = NULL;
	solver = NULL;
	dynamicsWorld = NULL;
}

World::~World()
{
	Destroy();
}

#endif

