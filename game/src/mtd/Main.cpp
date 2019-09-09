
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <ctime>

#include <Engine.h>

#include <Debug.h>
#include <memory>

#include "..\css\Header.h"
#include "..\css\Player.h"
#include "..\css\Event.h"

#include "..\css\Character.h"

#include <cassert>

#include <conio.h>

int main()
{
	DEBUG(1)
	
	ConvertMeshes( "media/meshes.list" );
	
	srand( time( NULL ) );
	
	
	Engine * engine = new Engine;
	engine->Init( new Event(), "Engine 0.0.2", NULL, 800, 600, false );
	
	LoadMeshes( "media/loadMeshes.list", engine );
	
	engine->RegisterType( "Character", "game-core.dll" );
	engine->RegisterType( "Player", "dlls/Player.dll" );
	
	
	{
		std::shared_ptr<Model> sphere = engine->GetModel( "Sphere" );
		std::shared_ptr<Model> crate01 = engine->GetModel( "Crate01" );
		std::shared_ptr<Model> mapModel = engine->GetModel( "TestMap" );
		
		engine->GetCollisionShapeManager()->AddCustomShape( "crate", crate01->GetCollisionShape( Model::SHAPE::CONVEX ) );
		engine->GetCollisionShapeManager()->AddCustomShape( "sphere", engine->GetCollisionShapeManager()->GetBall(0.5f) );
		
		
		std::shared_ptr<btCollisionShape> mapShape = mapModel->GetCollisionShape( Model::SHAPE::TRIANGLE );
		std::shared_ptr<Object> map = engine->AddObject( engine->GetNewObjectOfType("Character"), "TestMap", mapShape, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10,0) ), 0.0f );
		map->GetBody<btRigidBody>()->setGravity( btVector3(0,0,0) );
		map->SetModel( mapModel, false );
		map->GetBody<btRigidBody>()->setFriction( 0.75 );
		map->GetBody<btRigidBody>()->setAngularFactor( 0.0 );
		map->GetBody<btRigidBody>()->setLinearFactor( btVector3( 0.0, 0.0, 0.0 ) );
		
		std::shared_ptr<Object> player = engine->AddObject( engine->GetNewObjectOfType("Player"), "Player", engine->GetCollisionShapeManager()->GetCapsule( 0.3f, 1.75f ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(35,10,-25) ), 15.0 );
		engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
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



