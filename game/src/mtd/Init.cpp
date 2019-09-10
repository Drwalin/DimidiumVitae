
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <ctime>

#include <Engine.h>

#include <Debug.h>
#include <memory>

#include "..\css\Header.h"
#include "..\css\Player.h"

#include <EventResponser.h>

#include "..\css\Character.h"

#include <cassert>

#include <conio.h>

#include <dll\DllImporter.h>

extern "C" int Init( int argc, char ** argv )
{
	DEBUG(1)
	
	ConvertMeshes( "media/meshes.list" );
	
	srand( time( NULL ) );
	
	
	Dll eventModule( "dlls/Event.dll" );
	EventResponser*(*EventConstructor)();
	EventConstructor = eventModule.Get<decltype(EventConstructor)>( "EventConstructor" );
	
	
	Engine * engine = new Engine;
	engine->Init( EventConstructor(), "Engine 0.0.2", NULL, 800, 600, false );
	
	LoadMeshes( "media/loadMeshes.list", engine );
	
	LoadModules( engine, "modules.list" );
	
	
	{
		std::shared_ptr<Model> sphere = engine->GetModel( "Sphere" );
		std::shared_ptr<Model> crate01 = engine->GetModel( "Crate01" );
		std::shared_ptr<Model> mapModel = engine->GetModel( "TestMap" );
		
		engine->GetCollisionShapeManager()->AddCustomShape( "crate", crate01->GetCollisionShape( Model::SHAPE::CONVEX ) );
		engine->GetCollisionShapeManager()->AddCustomShape( "sphere", engine->GetCollisionShapeManager()->GetBall(0.5f) );
		
		
		std::shared_ptr<Entity> map = engine->AddEntity( engine->GetNewEntityOfType("StaticEntity"), "TestMap", mapModel->GetCollisionShape( Model::SHAPE::TRIANGLE ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10,0) ), 0.0f );
		map->SetModel( mapModel, false );
		
		std::shared_ptr<Entity> player = engine->AddEntity( engine->GetNewEntityOfType("Player"), "Player", engine->GetCollisionShapeManager()->GetCapsule( 0.3f, 1.75f ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(35,10,-25) ), 15.0 );
		engine->AttachCameraToEntity( "Player", btVector3( 0, 0.8, 0 ) );
		player->GetBody<btRigidBody>()->setFriction( 0.75 );
		((Character*)(player.get()))->SetCamera( engine->GetCamera() );
		player->GetBody<btRigidBody>()->setAngularFactor( btVector3( 0, 0, 0 ) );
		player->GetBody<btRigidBody>()->setActivationState( DISABLE_DEACTIVATION );
		
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



