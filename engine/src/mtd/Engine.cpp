
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_CPP
#define ENGINE_CPP

#include "..\css\Engine.h"

#include "..\lib\Debug.h"
#include "..\lib\Math.hpp"

#include <cassert>

int Engine::GetNumberOfObjects() const
{
	return this->object.size();
}

std::shared_ptr<Object> Engine::GetNewObjectOfType( const std::string & name )
{
	std::shared_ptr<Object> object = this->classFactory.GetClassInstantiator( name.c_str() )->New();
	object->Init( this );
	return object;
}

bool Engine::RegisterType( const std::string & name, const std::string & modulePath )
{
	return (bool)this->classFactory.AddClass( modulePath.c_str(), name.c_str() );
}

std::shared_ptr<Object> Engine::AddObject( std::shared_ptr<Object> emptyObject, const std::string & name, std::shared_ptr<btCollisionShape> shape, btTransform transform, btScalar mass, btVector3 inertia )
{
	if( emptyObject )
	{
		auto it = this->object.find( name );
		if( it == this->object.end() )
		{
			emptyObject->Init( this );
			emptyObject->Spawn( name, shape, transform );
			emptyObject->SetMass( mass );
			this->object[name] = emptyObject;
			return emptyObject;
		}
	}
	return NULL;
}

inline void Engine::UpdateObjectOverlaps()
{
	for( auto it = this->object.begin(); it != this->object.end(); ++it )
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
					Object * a = ((Object*)(contactManifold->getBody0()->getUserPointer()));
					Object * b = ((Object*)(contactManifold->getBody1()->getUserPointer()));
					
					if( a && b )
					{
						a->OverlapWithObject( b, contactManifold );
						b->OverlapWithObject( a, contactManifold );
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

inline void Engine::UpdateObjects( const float deltaTime )
{
	while( !this->objectsQueuedToDestroy.empty() )
	{
		this->DeleteObject( this->objectsQueuedToDestroy.front() );
		this->objectsQueuedToDestroy.pop();
	}
	
	this->UpdateObjectOverlaps();
	
	for( auto it = this->object.begin(); it != this->object.end(); ++it )
		it->second->Tick( deltaTime );
	
	if( this->cameraParent )
		this->GetCamera()->SetCameraTransform( this->cameraParent->GetTransform() );
}

void Engine::QueueObjectToDestroy( std::shared_ptr<Object> ptr )
{
	if( ptr )
		this->objectsQueuedToDestroy.push( ptr->GetName() );
}

void Engine::QueueObjectToDestroy( const std::string & name )
{
	this->objectsQueuedToDestroy.push( name );
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
	
	this->UpdateObjects( deltaTime );
	
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

std::shared_ptr<Object> Engine::GetCameraParent() const
{
	return this->cameraParent;
}

std::string Engine::GetAvailableObjectName( const std::string & name )
{
	if( this->object.find( name ) == this->object.end() )
	{
		return name;
	}
	for( int i = 0;; ++i )
	{
		if( this->object.find( name+std::to_string(i) ) == this->object.end() )
		{
			return name+std::to_string(i);
		}
	}
	return name;
}

void Engine::AttachCameraToObject( const std::string & name, btVector3 location )
{
	auto it = this->object.find( name );
	if( it != this->object.end() )
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
	auto it = this->model.find( name );
	if( it == this->model.end() )
	{
		this->model[name] = mdl;
		return true;
	}
	return false;
}

std::shared_ptr<Model> Engine::LoadModel( const std::string & name )
{
	auto it = this->model.find( name );
	if( it != this->model.end() )
	{
		if( it->second )
		{
			return it->second;
		}
		else
		{
			this->model.erase( it );
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
			this->model[name] = mdl;
			return mdl;
		}
	}
	return std::shared_ptr<Model>();
}

std::shared_ptr<Model> Engine::GetModel( const std::string & name )
{
	return this->LoadModel( name );
}

std::shared_ptr<Object> Engine::GetObject( const std::string & name )
{
	auto it = this->object.find( name );
	if( it != this->object.end() )
	{
		if( it->second )
			return it->second;
		else
			this->object.erase( it );
	}
	std::shared_ptr<Object> ret;
	return ret;
}

void Engine::DeleteObject( const std::string & name )
{
	auto it = this->object.find( name );
	if( it != this->object.end() )
	{
		if( it->second )
		{
			DEBUG( std::string("Destroying object: ") + name )
			
			if( it->second == this->cameraParent )
				this->cameraParent = NULL;
			
			this->world->RemoveBody( name );
			it->second.reset();
		}
		
		this->object.erase( it );
	}
	else
	{
		MESSAGE( std::string("Trying to destroy un-existing object: ") + name )
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
	
	this->collisionShapeManager = new CollisionShapeManager( this );
	
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
	
	for( auto it = this->object.begin(); it != this->object.end(); ++it )
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
	this->object.clear();
	
	for( auto it = this->model.begin(); it != this->model.end(); ++it )
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
	this->model.clear();
	
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

