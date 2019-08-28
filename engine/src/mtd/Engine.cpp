
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <Engine.h>

#include <Debug.h>
#include <Math.hpp>

#include <cassert>

int Engine::GetNumberOfObjects() const
{
	return object.size();
}

inline void Engine::UpdateObjectOverlaps()
{
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			it->second->NextOverlappingFrame();
		}
	}
	
	btDispatcher * dispacher = world->GetDynamicsWorld()->getDispatcher();
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
						if( a->IsTrigger() && b->IsTrigger() )
						{
							DEBUG( "Colliding Trigger with Trigger" );
						}
						else if( a->IsTrigger() )
						{
							a->OverlapWithObject( b, contactManifold );
						}
						else if( b->IsTrigger() )
						{
							b->OverlapWithObject( a, contactManifold );
						}
						else
						{
							if( a->IsDynamic() )
								a->OverlapWithObject( b, contactManifold );
							
							if( b->IsDynamic() )
								b->OverlapWithObject( a, contactManifold );
						}
					}
					else
					{
						MESSAGE( "btCollisionShape->getUserPointer() = NULL" );
					}
				}
				else
				{
					DEBUG( "No contact manifold points" );
				}
			}
			else
			{
				MESSAGE( std::string( "dispacher->getManifoldByIndexInternal(") + std::to_string(i) + ") = 0 " );
			}
		}
	}
	else
	{
		MESSAGE( std::string( "world->GetDynamicsWorld()->getDispatcher() = 0 " ) );
	}
}

inline void Engine::UpdateObjects( const float deltaTime )
{
	while( !objectsQueuedToDestroy.empty() )
	{
		DeleteObject( objectsQueuedToDestroy.front() );
		objectsQueuedToDestroy.pop();
	}
	
	UpdateObjectOverlaps();
	
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			if( it->second->IsTrigger() )
			{
				it->second->Tick( deltaTime );
			}
		}
	}
	
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			if( !it->second->IsTrigger() )
			{
				it->second->Tick( deltaTime );
			}
		}
	}
	
	if( cameraParent )
		GetCamera()->SetCameraTransform( cameraParent->GetTransform() );
}

void Engine::QueueObjectToDestroy( std::shared_ptr<Object> ptr )
{
	if( ptr )
		objectsQueuedToDestroy.push( ptr->GetName() );
}

void Engine::QueueObjectToDestroy( const std::string & name )
{
	objectsQueuedToDestroy.push( name );
}

float Engine::GetDeltaTime()
{
	if( window )
	{
		return window->GetDeltaTime();
	}
	return 1.0f/60.0f;
}

CollisionShapeManager * Engine::GetCollisionShapeManager()
{
	return collisionShapeManager;
}

World * Engine::GetWorld()
{
	return world;
}

Window * Engine::GetWindow()
{
	return window;
}

void Engine::PauseSimulation()
{
	pausePhysics = true;
}

void Engine::ResumeSimulation()
{
	pausePhysics = false;
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
	Tick( deltaTime );
}

void Engine::Tick( const float deltaTime )
{
	physicsSimulationTime.SubscribeStart();
	
	UpdateObjects( deltaTime );
	
	if( !pausePhysics )
	{
		world->Tick( deltaTime, CalculateNumberOfSimulationsPerFrame( deltaTime ) );		/////////////////////////////////////////////////////////////////////////
	}
	
	physicsSimulationTime.SubscribeEnd();
}

std::shared_ptr<Camera> Engine::GetCamera() const
{
	return window->camera;
}

std::shared_ptr<Object> Engine::GetCameraParent() const
{
	return cameraParent;
}

std::string Engine::GetAvailableObjectName( std::string name )
{
	if( object.find( name ) == object.end() )
	{
		return name;
	}
	for( int i = 0;; ++i )
	{
		if( object.find( name+std::to_string(i) ) == object.end() )
		{
			return name+std::to_string(i);
		}
	}
	return name;
}

void Engine::AttachCameraToObject( std::string name, btVector3 location )
{
	auto it = object.find( name );
	if( it != object.end() )
	{
		cameraParent = it->second;
	}
	else
	{
		cameraParent = NULL;
	}
	GetCamera()->SetPos( location );
}

bool Engine::SetCustomModelName( std::string name, std::shared_ptr<Model> mdl )
{
	auto it = model.find( name );
	if( it == model.end() )
	{
		model[name] = mdl;
		return true;
	}
	return false;
}

std::shared_ptr<Model> Engine::LoadModel( std::string name )
{
	auto it = model.find( name );
	if( it != model.end() )
	{
		if( it->second )
		{
			return it->second;
		}
		else
		{
			model.erase( it );
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
			model[name] = mdl;
			return mdl;
		}
	}
	return std::shared_ptr<Model>();
}

std::shared_ptr<Model> Engine::GetModel( std::string name )
{
	return LoadModel( name );
}

std::shared_ptr<Object> Engine::GetObject( std::string name )
{
	auto it = object.find( name );
	if( it != object.end() )
	{
		if( it->second )
		{
			return it->second;
		}
		else
		{
			object.erase( it );
		}
	}
	std::shared_ptr<Object> ret;
	return ret;
}

void Engine::DeleteObject( std::string name )
{
	auto it = object.find( name );
	if( it != object.end() )
	{
		if( it->second )
		{
			DEBUG( std::string("Destroying object: ") + name )
			
			if( it->second == cameraParent )
			{
				cameraParent = NULL;
			}
			
			world->RemoveBody( name );
			it->second.reset();
		}
		
		object.erase( it );
	}
	else
	{
		MESSAGE( std::string("Trying to destroy un-existing object: ") + name )
	}
}


/*
void Engine::Draw2D()
{
	guiDrawTime.SubscribeStart();
	
	window->output->SetWorkSpace( 5, 10, 80, 80 );
	window->output->Goto( 5, 12 );
	window->output->SetColor( al_map_rgb( 0, 255, 255 ) );
	window->output->Print( "DeltaTime: " );
	window->output->Print( window->GetDeltaTime() * 1000.0f );
	window->output->Print( "ms" );
	
	
	//window->output->SetWorkSpace( 5, 10, 80, 80 );
	//window->output->Goto( 5, 10 );
	//window->output->SetColor( al_map_rgb( 0, 255, 255 ) );
	window->output->Print( "\nFPS: " );
	window->output->Print( window->GetSmoothFps() );
	
	window->output->Print( "\nObjects: " );
	window->output->Print( int(object.size()) );
	
	window->output->Print( "\nPlayer velocity: " );
	window->output->Print( GetObject("Player")->GetBody()->getLinearVelocity().length() );
	
	window->output->Print( "\nPlayer velocity 2D: " );
	window->output->Print( btVector3( GetObject("Player")->GetBody()->getLinearVelocity().x(), 0, GetObject("Player")->GetBody()->getLinearVelocity().z() ).length() );
	
	//if( false )
	{
		window->output->Print( "\n\nPointing at object: " );
		std::shared_ptr<Object> player = this->GetObject("Player");
		btVector3 begin, end, point, normal;
		
		begin = this->GetCamera()->GetLocation();
		end = begin + ( this->GetCamera()->GetForwardVector() * 100.0 );
		
		std::shared_ptr<Object> temp = this->RayTrace( begin, end, Engine::RayTraceChannel::COLLIDING, point, normal, { player } );
		
		if( temp )
		{
			window->output->Print( temp->GetName() );
		}
		else
		{
			window->output->Print( "(NULL)" );
		}
	}
	
	//if( false )
	{
		window->output->Print( "\ncollisionShape: " );
		window->output->Print( int(collisionShapeManager->collisionShape.size()) );
		window->output->Print( "\ncollisionShapeRev: " );
		window->output->Print( int(collisionShapeManager->collisionShapeRev.size()) );
		window->output->Print( "\nnumberOfReferencesToShape: " );
		window->output->Print( int(collisionShapeManager->numberOfReferencesToShape.size()) );
		
		window->output->Print( "\n\nmodelPointerCustomCollisionData: " );
		window->output->Print( int(collisionShapeManager->modelPointerCustomCollisionData.size()) );
		window->output->Print( "\nmodelCustomCollisionData: " );
		window->output->Print( int(collisionShapeManager->modelCustomCollisionData.size()) );
		window->output->Print( "\ncustomCollisionShapeData: " );
		window->output->Print( int(collisionShapeManager->customCollisionShapeData.size()) );
		window->output->Print( "\ncustomCollisionShape: " );
		window->output->Print( int(collisionShapeManager->customCollisionShape.size()) );
		window->output->Print( "\ncustomCollisionShapeName: " );
		window->output->Print( int(collisionShapeManager->customCollisionShapeName.size()) );
	}
	
	//if( false )
	{
		window->output->SetWorkSpace( 5, 2, 80, 80 );
		
		float wholeDrawTime = window->GetWholeDrawTime().GetSmoothTime();
		float skippedTime = window->GetSkippedTime().GetSmoothTime();
		float eventTime = window->GetEventGenerationTime().GetSmoothTime();
		float flipDisplayTime = window->GetFlipDisplayTime().GetSmoothTime();
		float sumTime = window->GetDeltaTime();
		float otherTime = sumTime - ( flipDisplayTime + guiDrawTime.GetSmoothTime() + sceneDrawTime.GetSmoothTime() + physicsSimulationTime.GetSmoothTime() + skippedTime + eventTime );
		float step = sumTime / 40;
		float t;
		
		
		
		
		window->output->Goto( 5, 2 );
		
		window->output->SetColor( al_map_rgb( 0, 255, 255 ) );
		window->output->Print( "flipDisplayTime...: " );
		window->output->Print( flipDisplayTime * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 2 );
		for( t = 0.0f; t < flipDisplayTime; t += step )
			window->output->Print( "#" );
		
		window->output->SetColor( al_map_rgb( 255, 0, 0 ) );
		window->output->Print( "\nguiDrawTime: " );
		window->output->Print( guiDrawTime.GetSmoothTime() * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 3 );
		for( t = 0.0f; t < guiDrawTime.GetSmoothTime(); t += step )
			window->output->Print( "#" );
		
		window->output->SetColor( al_map_rgb( 0, 255, 0 ) );
		window->output->Print( "\nsceneDrawTime: " );
		window->output->Print( sceneDrawTime.GetSmoothTime() * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 4 );
		for( t = 0.0f; t < sceneDrawTime.GetSmoothTime(); t += step )
			window->output->Print( "#" );
		
		window->output->SetColor( al_map_rgb( 0, 0, 255 ) );
		window->output->Print( "\nphysicsSimulationTime: " );
		window->output->Print( physicsSimulationTime.GetSmoothTime() * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 5 );
		for( t = 0.0f; t < physicsSimulationTime.GetSmoothTime(); t += step )
			window->output->Print( "#" );
		
		window->output->SetColor( al_map_rgb( 255, 255, 255 ) );
		window->output->Print( "\nskippedTime: " );
		window->output->Print( skippedTime * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 6 );
		for( t = 0.0f; t < skippedTime; t += step )
			window->output->Print( "#" );
		
		window->output->SetColor( al_map_rgb( 255, 0, 255 ) );
		window->output->Print( "\neventGenerationTime: " );
		window->output->Print( eventTime * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 7 );
		for( t = 0.0f; t < eventTime; t += step )
			window->output->Print( "#" );
		
		window->output->SetColor( al_map_rgb( 128, 128, 128 ) );
		window->output->Print( "\notherTime: " );
		window->output->Print( otherTime * 1000.0f );
		window->output->Print( "ms" );
		window->output->Goto( 42, 8 );
		for( t = 0.0f; t < otherTime; t += step )
			window->output->Print( "#" );
	}
	
//	DrawCrosshair();
	
	window->output->Flush();
	
	guiDrawTime.SubscribeEnd();
}

*/

void Engine::BeginLoop()
{
	pausePhysics = false;
	window->BeginLoop();
}

void Engine::Init( EventResponser * eventResponser, const char * windowName, const char * iconFile, int width, int height, bool fullscreen )
{
	Destroy();
	event = eventResponser;
	world = new World;
	window = new Window;
	world->Init();
	window->Init( this, windowName, iconFile, width, height, event, fullscreen );
	event->Init();
	event->SetEngine( this );
	
	window->HideMouse();
	window->LockMouse();
	
	collisionShapeManager = new CollisionShapeManager( this );
	
	window->output->Init();
	
	if( GetCamera() == NULL )
	{
		DEBUG( "Creating camera" );
		window->camera = std::shared_ptr<Camera>( new Camera( this, false, width, height, window->sceneManager->addCameraSceneNode() ) );
		window->camera->GetCameraNode()->setFOV( 60.0f * Math::PI / 180.0f );
	}
	
	//window->UseParallelThreadToDraw();
}

void Engine::Destroy()
{
	cameraParent = NULL;
	
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			assert( it->second );
			world->RemoveBody( it->first );
			it->second.reset();
		}
		else
		{
			MESSAGE("It shouldn't appear: ERR=3116661");
		}
	}
	object.clear();
	
	for( auto it = model.begin(); it != model.end(); ++it )
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
	model.clear();
	
	if( window )
	{
		assert( window != NULL );
		window->Destroy();
		delete window;
		window = NULL;
	}
	
	if( world )
	{
		assert( world != NULL );
		world->Destroy();
		delete world;
		world = NULL;
	}
	
	if( event )
	{
		assert( event != NULL );
		delete event;
		event = NULL;
	}
	
	if( collisionShapeManager )
	{
		assert( collisionShapeManager != NULL );
		collisionShapeManager->Destroy();
		delete collisionShapeManager;
		collisionShapeManager = NULL;
	}
	
	pausePhysics = false;
}

Engine::Engine()
{
	event = NULL;
	world = NULL;
	window = NULL;
	pausePhysics = true;
	collisionShapeManager = NULL;
}

Engine::~Engine()
{
	Destroy();
}

#endif

