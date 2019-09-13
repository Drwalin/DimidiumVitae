
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_CPP
#define ENGINE_CPP

#include "..\css\Engine.h"

#include "..\lib\Debug.h"
#include "..\lib\Math.hpp"

#include <cassert>

int Engine::GetNumberOfEntities() const
{
	return this->entities.size();
}

std::shared_ptr<Entity> Engine::GetNewEntityOfType( const std::string & name )
{
	std::shared_ptr<Entity> entity = this->classFactory.GetClassInstantiator( name.c_str() )->New();
	entity->Init( this );
	return entity;
}

bool Engine::RegisterType( const std::string & name, const std::string & modulePath )
{
	return (bool)this->classFactory.AddClass( modulePath.c_str(), name.c_str() );
}

std::shared_ptr<Entity> Engine::AddEntity( std::shared_ptr<Entity> emptyEntity, const std::string & name, std::shared_ptr<btCollisionShape> shape, btTransform transform, btScalar mass, btVector3 inertia )
{
	if( emptyEntity )
	{
		auto it = this->entities.find( name );
		if( it == this->entities.end() )
		{
			emptyEntity->Init( this );
			emptyEntity->Spawn( name, shape, transform );
			emptyEntity->SetMass( mass );
			this->entities[name] = emptyEntity;
			return emptyEntity;
		}
	}
	return NULL;
}

inline void Engine::UpdateEntitiesOverlapp()
{
	for( auto it = this->entities.begin(); it != this->entities.end(); ++it )
	{
		if( it->second )
		{
			it->second->NextOverlappingFrame();
		}
	}
	
	btDispatcher * dispacher = this->world->GetDynamicsWorld()->getDispatcher();
	if( dispacher )
	{
		int numberOfManifolds = dispacher->getNumManifolds();
		for( int i = 0; i < numberOfManifolds; ++i )
		{
			btPersistentManifold * contactManifold = dispacher->getManifoldByIndexInternal(i);
			if( contactManifold )
			{
				if( contactManifold->getNumContacts() > 0 )
				{
					Entity * a = ((Entity*)(contactManifold->getBody0()->getUserPointer()));
					Entity * b = ((Entity*)(contactManifold->getBody1()->getUserPointer()));
					
					if( a && b )
					{
						a->OverlapWithEntity( b, contactManifold );
						b->OverlapWithEntity( a, contactManifold );
					}
					else
						MESSAGE( "btCollisionShape->getUserPointer() = NULL" );
				}
				else
					DEBUG( "No contact manifold points" );
			}
			else
				MESSAGE( std::string( "dispacher->getManifoldByIndexInternal(") + std::to_string(i) + ") = 0 " );
		}
	}
	else
		MESSAGE( std::string( "world->GetDynamicsWorld()->getDispatcher() = 0 " ) );
}

inline void Engine::UpdateEntities( const float deltaTime )
{
	while( !this->entitiesQueuedToDestroy.empty() )
	{
		this->DeleteEntity( this->entitiesQueuedToDestroy.front() );
		this->entitiesQueuedToDestroy.pop();
	}
	
	this->UpdateEntitiesOverlapp();
	
	for( auto it = this->entities.begin(); it != this->entities.end(); ++it )
		it->second->Tick( deltaTime );
	
	if( this->cameraParent )
		this->GetCamera()->SetCameraTransform( this->cameraParent->GetTransform() );
}

void Engine::QueueEntityToDestroy( std::shared_ptr<Entity> ptr )
{
	if( ptr )
		this->entitiesQueuedToDestroy.push( ptr->GetName() );
}

void Engine::QueueEntityToDestroy( const std::string & name )
{
	this->entitiesQueuedToDestroy.push( name );
}

float Engine::GetDeltaTime()
{
	if( window )
		return window->GetDeltaTime();
	return 1.0f/60.0f;
}

CollisionShapeManager * Engine::GetCollisionShapeManager()
{
	return this->collisionShapeManager;
}

World * Engine::GetWorld()
{
	return this->world;
}

Window * Engine::GetWindow()
{
	return this->window;
}

void Engine::PauseSimulation()
{
	this->pausePhysics = true;
}

void Engine::ResumeSimulation()
{
	this->pausePhysics = false;
}

int Engine::CalculateNumberOfSimulationsPerFrame( const float deltaTime )
{
	return 1;
	float fps = 1.0 / deltaTime;
	if( fps >= 57.0 )
		return 100;
	if( fps >= 50.0 )
		return 90;
	if( fps >= 40.0 )
		return 60;
	if( fps >= 30.0 )
		return 30;
	if( fps >= 20.0 )
		return 5;
	return 1;
}

void Engine::ParallelToDrawTick( const float deltaTime )
{
	this->Tick( deltaTime );
}

void Engine::Tick( const float deltaTime )
{
	this->physicsSimulationTime.SubscribeStart();
	
	this->UpdateEntities( deltaTime );
	
	if( !this->pausePhysics )
	{
		this->world->Tick( deltaTime, this->CalculateNumberOfSimulationsPerFrame( deltaTime ) );		/////////////////////////////////////////////////////////////////////////
	}
	
	this->physicsSimulationTime.SubscribeEnd();
}

std::shared_ptr<Camera> Engine::GetCamera() const
{
	return this->window->camera;
}

std::shared_ptr<Entity> Engine::GetCameraParent() const
{
	return this->cameraParent;
}

std::string Engine::GetAvailableEntityName( const std::string & name )
{
	if( this->entities.find( name ) == this->entities.end() )
	{
		return name;
	}
	for( int i = 0;; ++i )
	{
		if( this->entities.find( name+std::to_string(i) ) == this->entities.end() )
		{
			return name+std::to_string(i);
		}
	}
	return name;
}

void Engine::AttachCameraToEntity( const std::string & name, btVector3 location )
{
	auto it = this->entities.find( name );
	if( it != this->entities.end() )
	{
		this->cameraParent = it->second;
	}
	else
	{
		this->cameraParent = NULL;
	}
	this->GetCamera()->SetPos( location );
}

bool Engine::SetCustomModelName( const std::string & name, std::shared_ptr<Model> mdl )
{
	auto it = this->models.find( name );
	if( it == this->models.end() )
	{
		this->models[name] = mdl;
		return true;
	}
	return false;
}

std::shared_ptr<Model> Engine::LoadModel( const std::string & name )
{
	auto it = this->models.find( name );
	if( it != this->models.end() )
	{
		if( it->second )
		{
			return it->second;
		}
		else
		{
			this->models.erase( it );
		}
	}
	else
	{
		std::shared_ptr<Model> mdl( new Model );
		if( mdl->LoadFromFile( this, name ) == false )
		{
			mdl.reset();
			return std::shared_ptr<Model>();
		}
		else
		{
			this->models[name] = mdl;
			return mdl;
		}
	}
	return std::shared_ptr<Model>();
}

std::shared_ptr<Model> Engine::GetModel( const std::string & name )
{
	return this->LoadModel( name );
}

std::shared_ptr<Entity> Engine::GetEntity( const std::string & name )
{
	auto it = this->entities.find( name );
	if( it != this->entities.end() )
	{
		if( it->second )
			return it->second;
		else
			this->entities.erase( it );
	}
	std::shared_ptr<Entity> ret;
	return ret;
}

void Engine::DeleteEntity( const std::string & name )
{
	auto it = this->entities.find( name );
	if( it != this->entities.end() )
	{
		if( it->second )
		{
			DEBUG( std::string("Destroying entity: ") + name )
			
			if( it->second == this->cameraParent )
				this->cameraParent = NULL;
			
			this->world->RemoveBody( name );
			it->second.reset();
		}
		
		this->entities.erase( it );
	}
	else
	{
		MESSAGE( std::string("Trying to destroy un-existing entity: ") + name )
	}
}

void Engine::BeginLoop()
{
	this->pausePhysics = false;
	this->window->BeginLoop();
}

void Engine::Init( EventResponser * eventResponser, const char * windowName, const char * iconFile, int width, int height, bool fullscreen )
{
	this->Destroy();
	this->event = eventResponser;
	this->world = new World;
	this->window = new Window;
	this->world->Init();
	this->window->Init( this, windowName, iconFile, width, height, event, fullscreen );
	this->event->Init();
	this->event->SetEngine( this );
	
	this->window->HideMouse();
	this->window->LockMouse();
	
	this->collisionShapeManager = new CollisionShapeManager();
	
	this->window->output->Init();
	
	if( this->GetCamera() == NULL )
	{
		DEBUG( "Creating camera" );
		this->window->camera = std::shared_ptr<Camera>( new Camera( this, false, width, height, this->window->sceneManager->addCameraSceneNode() ) );
		this->window->camera->GetCameraNode()->setFOV( 60.0f * Math::PI / 180.0f );
	}
	
	//this->window->UseParallelThreadToDraw();
}

void Engine::Destroy()
{
	this->cameraParent = NULL;
	
	for( auto it = this->entities.begin(); it != this->entities.end(); ++it )
	{
		if( it->second )
		{
			assert( it->second );
			this->world->RemoveBody( it->first );
			it->second.reset();
		}
		else
		{
			MESSAGE("It shouldn't appear: ERR=3116661");
		}
	}
	this->entities.clear();
	
	for( auto it = this->models.begin(); it != this->models.end(); ++it )
	{
		if( it->second )
		{
			assert( it->second );
			it->second->Destroy();
			it->second.reset();
		}
		else
		{
			MESSAGE("It shouldn't appear: ERR=3116662");
		}
	}
	this->models.clear();
	
	if( this->window )
	{
		assert( this->window != NULL );
		this->window->Destroy();
		delete this->window;
		this->window = NULL;
	}
	
	if( this->world )
	{
		assert( this->world != NULL );
		this->world->Destroy();
		delete this->world;
		this->world = NULL;
	}
	
	if( this->event )
	{
		assert( this->event != NULL );
		delete this->event;
		this->event = NULL;
	}
	
	if( this->collisionShapeManager )
	{
		assert( this->collisionShapeManager != NULL );
		this->collisionShapeManager->Destroy();
		delete this->collisionShapeManager;
		this->collisionShapeManager = NULL;
	}
	
	this->pausePhysics = false;
}

Engine::Engine()
{
	this->event = NULL;
	this->world = NULL;
	this->window = NULL;
	this->pausePhysics = true;
	this->collisionShapeManager = NULL;
}

Engine::~Engine()
{
	this->Destroy();
}

#include "..\lib\dll\ClassFactory.cpp"

#endif

