
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

#include "..\css\Character.h"

#include <cassert>

#include <conio.h>

#include <dll\DllImporter.h>

#include <Math.hpp>

extern "C" int Init( int argc, char ** argv )
{
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
		
		auto mapShape = engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap");
		
		// create player
		std::shared_ptr<Entity> player = engine->AddEntity( engine->GetNewEntityOfType("Player"), "Player", engine->GetCollisionShapeManager()->GetCapsule( 0.3f, 1.75f ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,10,0) ), 15.0 );
		engine->AttachCameraToEntity( "Player", btVector3( 0, 0.8, 0 ) );
		((Character*)(player.get()))->SetCamera( engine->GetCamera() );
		
		// create test balls
		{
			for( float x = -40; x < 41; x += 10 )
			{
				for( float y = -40; y < 41; y += 10 )
				{
					auto temp = engine->AddEntity( engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Ball"), engine->GetCollisionShapeManager()->GetSphere( 1 ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(x,30,y) ), 300.0f );
					if( temp )
					{
						temp->SetModel( sphere, false );
						temp->SetScale( btVector3( 2, 2, 2 ) );
						temp->GetBody()->setFriction( 0.75 );
						temp->GetBody()->setDamping( 0.1, 0.1 );
					}
				}
			}
		}
		
		// create map
		std::shared_ptr<Entity> map = engine->AddEntity( engine->GetNewEntityOfType("StaticEntity"), "TestMap", engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap")/*mapModel->GetCollisionShape( Model::SHAPE::TRIANGLE )*/, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) ), 100000000.0f );
		map->SetModel( mapModel, false );
	}
	
	
	DEBUG( std::string("Loading cpu time: ") + std::to_string(float(clock())/float(CLOCKS_PER_SEC)) );
	DEBUG( std::string("Loading time: ") + std::to_string(float(clock())/1000.0f) );
	
	engine->BeginLoop();
	
	DEBUG("End")
	
	engine->Destroy();
	DEBUG("1")
	delete engine;
	DEBUG("2")
	engine = NULL;
	
	return 0;
}



