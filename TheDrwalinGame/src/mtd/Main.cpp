
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <ctime>

#include <Engine.h>

#include <Debug.h>
#include <memory>

#include "..\css\Header.h"
#include "..\css\Player.h"
#include "..\css\Event.h"

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
	
	
{
	std::shared_ptr<Model> sphere = engine->GetModel( "Sphere" );
	std::shared_ptr<Model> crate01 = engine->GetModel( "Crate01" );
	std::shared_ptr<Model> mapModel = engine->GetModel( /*"Plane" );*/"TestMap" );
	assert( mapModel );
	
	
	
	std::shared_ptr<btCollisionShape> mapShape = mapModel->GetCollisionShape( Model::SHAPE::TRIANGLE );//engine->GetCollisionShapeManager()->CreateCustomShape( "Map triangle collision mesh", mapModel, CollisionShapeManager::SHAPE_TYPE_TRIANGLE );
	assert( mapShape );
	
	
	
	std::shared_ptr<Object> map = engine->AddObject<Object>( "TestMap", mapShape, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10,0) ), false );
	map->SetModel( mapModel, false );
	map->GetBody()->setFriction( 0.65 );
//	map->SetScale( btVector3(5,-5,5) );
//	map->SetScale( btVector3(0.023,0.023,0.023) );
//	map->SetPosition( btVector3( -20, -20, -20 ) );
//	map->SetRotation( btQuaternion( btVector3(0,1,0), 0 ) );
	
	if( false )
	{
		auto o = engine->AddObject<Object>( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetBox( btVector3(1.5,1.5,1.5) )/*engine->GetCollisionShapeManager()->GetCustomShape( "crate01shape" )*/, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10.3,0) ), false );
		o->SetModel( crate01 );
		o->SetScale( btVector3( 30, 3, 30 ) );
	}
	
	
//	engine->AddObject<Object>( engine->GetAvailableObjectName("Brick"), engine->GetCollisionShapeManager()->AddCustomShape( engine->GetCollisionShapeManager()->GetFirstAvailableName("Brick"), engine->GetModel("Brick")->GetCollisionShape( Model::SHAPE::CONVEX ) ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,30,0) ), true, 10 )->SetModel( engine->GetModel("Brick") );
//	engine->AddObject<Object>( engine->GetAvailableObjectName("ConcreetBrick"), engine->GetCollisionShapeManager()->AddCustomShape( engine->GetCollisionShapeManager()->GetFirstAvailableName("ConcreetBrick"), engine->GetModel("ConcreetBrick")->GetCollisionShape( Model::SHAPE::CONVEX ) ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,15,0) ), true, 20 )->SetModel( engine->GetModel("ConcreetBrick") );
//	engine->AddObject<Object>( engine->GetAvailableObjectName("m4a1"), engine->GetCollisionShapeManager()->AddCustomShape( engine->GetCollisionShapeManager()->GetFirstAvailableName("m4a1"), engine->GetModel("m4a1")->GetCollisionShape( Model::SHAPE::CONVEX ) ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,40,0) ), true, 10, engine->GetModel("m4a1")->GetInertia() /* btVector3( 1, 1, 1 ) * 0.023f*/ )->SetModel( engine->GetModel("m4a1") );
//	engine->GetObject( "m4a1" )->SetScale( btVector3( 1, 1, 1 ) * 0.023f );
	
	
	engine->GetCollisionShapeManager()->AddCustomShape( "crate01shape__1331_", crate01->GetCollisionShape( Model::SHAPE::CONVEX ) );
	
	for( int i = 0; i < 0; ++i )
	{
		auto o = engine->AddObject<Object>( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetShape( "crate01shape__1331_" ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,10.3,0) + btVector3(0,(i%12)*0.3,((i/12))+(float(i%2)/2.0)) ), true, 50.0 );
		o->SetModel( crate01 );
		o->SetScale( btVector3( 0.6, 0.3, 1 ) );
		o->GetBody()->setFriction( 0.9 );
	}
	
	
	
	std::shared_ptr<Object> player = engine->AddCharacter<Player>( "Player", 0.6, 1.75, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(35,10,-25) ), 15.0 );
	engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
	((Character*)(player.get()))->SetCamera( engine->GetCamera() );
	
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



