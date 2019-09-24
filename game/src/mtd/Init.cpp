
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <ctime>

#include <Engine.h>

#include <Debug.h>

#include <memory>
#include <iostream>

#include "..\css\Header.h"
#include "..\css\Player.h"

#include <EventResponser.h>

#include <cassert>

#include <conio.h>

#include <dll\DllImporter.h>

#include <Math.hpp>

extern "C" int Init( int argc, char ** argv )
{
	Dll triggerModule( "dlls/Trigger.dll" );
	Dll characterWalkTriggerModule( "dlls/CharacterWalkTrigger.dll" );
	Dll characterModule( "dlls/Character.dll" );
	Dll eventModule( "dlls/Event.dll" );
	
	srand( time( NULL ) );
	
	EventResponser * eventResponser = eventModule.Get<EventResponser*(*)(void)>( "EventConstructor" )();
	
	Engine * engine = new Engine;
	engine->Init( eventResponser, "Engine 0.0.2", NULL, 800, 600, false );
	
	LoadShapes( "media/shapes.list", engine );
	LoadMeshes( "media/meshes.list", engine );
	LoadModules( engine, "modules.list" );
	
	{
		std::shared_ptr<Model> sphere = engine->GetModel( "Sphere" );
		std::shared_ptr<Model> crate = engine->GetModel( "Crate01" );
		std::shared_ptr<Model> mapModel = engine->GetModel( "TestMap" );
		
		
		
		// create animated bow
		std::shared_ptr<Entity> animatedBow = engine->AddEntity( engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Bow"), engine->GetCollisionShapeManager()->GetCylinder( 0.3, 1 ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(15,3,15) ), 3.0f );
		animatedBow->SetModel( engine->GetModel( "AnimatedBow" ) );
		animatedBow->GetBody()->setFriction( 0.75 );
		animatedBow->GetBody()->setDamping( 0.1, 0.1 );
		animatedBow->GetSceneNode()->GetISceneNode()->setAnimationSpeed( 24.0f );
		animatedBow->GetSceneNode()->GetISceneNode()->setFrameLoop( 0, 19 );
		animatedBow->GetSceneNode()->GetISceneNode()->setLoopMode( false );
		
		
		
		auto mapShape = engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap");
		
		// create player
		std::shared_ptr<Entity> player = engine->AddEntity( engine->GetNewEntityOfType("Player"), "Player", engine->GetCollisionShapeManager()->GetCapsule( 0.3f, 1.75f ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,15,0) ), 75.0 );
		engine->AttachCameraToEntity( "Player", btVector3( 0, 0.7f, 0 ) );
		player->SetCamera( engine->GetCamera() );
		
		
		
		// create map
		std::shared_ptr<Entity> map = engine->AddEntity( engine->GetNewEntityOfType("StaticEntity"), "TestMap", engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap"), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) ), 100000000.0f );
		map->SetModel( mapModel );
	}
	
	
	MESSAGE( std::string("Loading cpu time: ") + std::to_string(float(clock())/float(CLOCKS_PER_SEC)) );
	MESSAGE( std::string("Loading time: ") + std::to_string(float(clock())/1000.0f) );
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	engine = NULL;
	
	return 0;
}



