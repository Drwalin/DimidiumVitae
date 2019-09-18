
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_CPP
#define EVENT_CPP

#include <Window.h>
#include <Engine.h>

#include "..\css\Event.h"
#include "..\css\Character.h"
#include "..\css\Player.h"
#include "..\css\MotionController.h"

#include <irrlicht/irrlicht.h>

#include <Debug.h>
#include <Math.hpp>

void Event::MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw )
{
	std::shared_ptr<Entity> player = engine->GetEntity( std::string("Player") );
	if( player )
	{
		Character * character = dynamic_cast < Character* > ( (Entity*)(player.get()) );
		if( character )
		{
			character->EventRotateCameraBy( btVector3( float(dy)/160.0, float(dx)/160.0, 0.0 ) );
		}
	}
}

irr::scene::ISceneNode * lightSceneNode = 0;

void Event::KeyPressedEvent( int keyCode )
{
	std::shared_ptr<Entity> player = engine->GetEntity( std::string("Player") );
	std::shared_ptr<Entity> temp;
	btVector3 begin, end, point, normal, euler;
	Character * character = NULL;
	Player * playerPtr = NULL;
	if( player )
	{
		character = dynamic_cast < Character* > ( ((Entity*)(player.get())) );
		playerPtr = dynamic_cast < Player* > ( ((Entity*)(player.get())) );
	}
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	switch( keyCode )
	{
	case irr::KEY_ESCAPE:
		window->QueueQuit();
		break;
		
	case irr::KEY_KEY_F:
		if( !lightSceneNode )
		{
			MESSAGE(1)
			lightSceneNode = engine->GetWindow()->sceneManager->addLightSceneNode( 0, Math::GetIrrVec( window->camera->GetLocation() ), irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 60.0f );
			lightSceneNode->setPosition( Math::GetIrrVec( window->camera->GetLocation() ) );
		}
		else
		{
			MESSAGE(2)
			lightSceneNode->remove();
			lightSceneNode = 0;
		}
		break;
		
	case irr::KEY_KEY_T:
		fprintf( stderr, "\n Number of objects: %i ", engine->GetNumberOfEntities() );
		break;
	case irr::KEY_KEY_O:
		Debug::SwitchDebugState();
		break;
		
	case irr::KEY_KEY_P:
		if( engine->GetWindow()->IsMouseLocked() )
			engine->GetWindow()->UnlockMouse();
		else
			engine->GetWindow()->LockMouse();
		break;
		
		
	case irr::KEY_LSHIFT:
		playerMotionController->StartRunning();
		break;
	case irr::KEY_LCONTROL:
		break;
	case irr::KEY_LMENU:
		playerMotionController->StartSneaking();
		break;
		
	case irr::KEY_LBUTTON:
		euler = window->camera->GetRot();
		temp = engine->AddEntity( engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Crate"), engine->GetCollisionShapeManager()->GetBox( btVector3(1,1,1) ), btTransform( btQuaternion(-euler.y(),-euler.x(),euler.z()), window->camera->GetLocation() + character->GetForwardVector() ), 20.0f );
		if( temp )
		{
			temp->SetModel( engine->GetModel( "Crate01" ), false );
			temp->SetScale( btVector3( 0.5, 0.5, 0.5 ) );
			temp->GetBody()->setFriction( 0.75 );
			temp->GetBody()->setLinearVelocity( character->GetForwardVector() * 16.0 );
			temp->GetBody()->setDamping( 0.1, 0.1 );
		}
		else
		{
			MESSAGE("Couldn't spawn new object");
		}
		break;
		
	case irr::KEY_RBUTTON:
		temp = engine->AddEntity( engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Ball"), engine->GetCollisionShapeManager()->GetSphere( 1 ), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + character->GetForwardVector() ), 20.0f );
		if( temp )
		{
			temp->SetModel( engine->GetModel( "Sphere" ), false );
			temp->SetScale( btVector3( 0.5, 0.5, 0.5 ) );
			temp->GetBody()->setFriction( 0.75 );
			temp->GetBody()->setLinearVelocity( character->GetForwardVector() * 16.0 );
			temp->GetBody()->setDamping( 0.1, 0.1 );
		}
		else
		{
			MESSAGE("Couldn't spawn new object");
		}
		break;
	}
}

void Event::KeyReleasedEvent( int keyCode )
{
	std::shared_ptr<Entity> player = engine->GetEntity( std::string("Player") );
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Entity*)(player.get()) );
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	switch( keyCode )
	{
	case irr::KEY_ESCAPE:
		window->QueueQuit();
		break;
		
		
	case irr::KEY_LSHIFT:
		playerMotionController->StopRunning();
		break;
	case irr::KEY_LCONTROL:
		break;
	case irr::KEY_LMENU:
		playerMotionController->StopSneaking();
		break;
	}
	
	KeyHoldedEvent( keyCode );
}

void Event::KeyHoldedEvent( int keyCode )
{
	std::shared_ptr<Entity> player = engine->GetEntity( std::string("Player") );
	std::shared_ptr<Entity> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Entity*)(player.get()) );
	
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	btVector3 vector;
	
	switch( keyCode )
	{
	case irr::KEY_MBUTTON:
		if( character )
		{
			temp = engine->GetEntity( std::string("Box") );
			if( temp )
			{
				character->EventRotateCameraToLookAtPoint( temp->GetLocation(), true );
			}
		}
		break;
		
	case irr::KEY_ESCAPE:
		window->QueueQuit();
		break;
		
		
		
	case irr::KEY_BACK:
	case irr::KEY_DELETE:
		if( keyCode == irr::KEY_DELETE )
		{
			begin = engine->GetCamera()->GetLocation();
			end = begin + ( character->GetForwardVector() * 10000.0 );
		}
		else
		{
			begin = btVector3(0,11.2,0);
			end = btVector3(0,11.2,300);
		}
		temp = engine->RayTrace( begin, end, Engine::RayTraceChannel::COLLIDING, point, normal, { player } );
		if( temp )
		{
			if( temp->GetName() != "TestMap" && temp->GetName() != "Box" )
			{
				if( temp->GetBody() )
					temp->GetBody()->activate();
				engine->QueueEntityToDestroy( temp->GetName() );
			}
		}
		break;
		
	case irr::KEY_UP:
		if( character )
			character->EventRotateCameraBy( btVector3( -window->GetDeltaTime(), 0.0, 0.0 ) * 2.0 );
		break;
	case irr::KEY_DOWN:
		if( character )
			character->EventRotateCameraBy( btVector3( window->GetDeltaTime(), 0.0, 0.0 ) * 2.0 );
		break;
	case irr::KEY_RIGHT:
		if( character )
			character->EventRotateCameraBy( btVector3( 0.0, window->GetDeltaTime(), 0.0 ) * 2.0 );
		break;
	case irr::KEY_LEFT:
		if( character )
			character->EventRotateCameraBy( btVector3( 0.0, -window->GetDeltaTime(), 0.0 ) * 2.0 );
		break;
		
	case irr::KEY_SPACE:
		if( character && playerMotionController )
			playerMotionController->Jump();
		break;
		
	case irr::KEY_KEY_W:
		if( character && playerMotionController )
			playerMotionController->MoveInDirection( character->GetFlatForwardVector() );
		break;
	case irr::KEY_KEY_A:
		if( character && playerMotionController )
			playerMotionController->MoveInDirection( character->GetFlatLeftVector() );
		break;
	case irr::KEY_KEY_S:
		if( character && playerMotionController )
			playerMotionController->MoveInDirection( -character->GetFlatForwardVector() );
		break;
	case irr::KEY_KEY_D:
		if( character && playerMotionController )
			playerMotionController->MoveInDirection( -character->GetFlatLeftVector() );
		break;
	}
}

void Event::StringToEnterEvent( std::string str )
{
	fprintf( stderr, "\n Input string: \"%s\"", str.c_str() );
	
	std::shared_ptr<Entity> player = engine->GetEntity( std::string("Player") );
	std::shared_ptr<Entity> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Entity*)(player.get()) );
	
	if( str == "Rel" )
	{
		fprintf( stderr, "\n StringToEnterEvent( %s ); ", str.c_str() );
		engine->GetCamera()->SetRotation( btVector3( 0, 0, 0 ) );
		if( character )
		{
			character->SetCameraRotation( btVector3( 0, 0, 0 ) );
		}
	}
}

Event::Event()
{
	window = NULL;
}

Event::~Event()
{
	window = NULL;
}

extern "C" EventResponser * EventConstructor()
{
	return new Event();
}

#endif

