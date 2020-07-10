
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF


#include <dll\DllImporter.h>
#include <Math.hpp>
#include <Debug.h>
#include <StdUtil.hpp>

#include <EventResponser.h>
#include <Engine.h>

#include "..\css\Header.h"
#include "..\css\Player.h"
#include "..\css\Event.h"

#include <iostream>
#include <memory>

#include <cassert>
#include <ctime>

extern "C" int Init( int argc, char ** argv )
{
	srand( time( NULL ) );
	
	Engine * engine = new Engine;
	engine->Init( new Event(), "Engine "+GetVersionString(), "", 800, 600, false );
	
	irr::io::IFileSystem * fileSystem = engine->GetWindow()->GetDevice()->getFileSystem();
	fileSystem->addFileArchive( "media/Textures.zip", false, false );
	fileSystem->addFileArchive( "media/Models.zip", false, false );
	fileSystem->addFileArchive( "media/Shapes.zip", false, false );
	
	LoadModules( engine, "modules.list" );
	
	LoadMeshes( engine, "media/models.list" );
	LoadShapes( engine, "media/shapes.list" );
	LoadSounds( engine, "media/sounds.list" );
	
	SoundSource musicSource( engine->GetSoundsManager()->GetSoundSampler( "track01" ) );
	musicSource.Set2D();
	musicSource.SetVolume( 0.07f );
	musicSource.Loop( true );
	musicSource.Play();
	
	{
		// create animated bow
		std::shared_ptr<Entity> animatedBow = engine->AddEntity( engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Bow"), engine->GetCollisionShapeManager()->GetCylinder( 0.3, 1 ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(15,3,15) ), 3.0f );
		animatedBow->SetModel( engine->GetModel( "AnimatedBow" ) );
		animatedBow->GetBody()->setFriction( 0.75 );
		animatedBow->GetBody<btRigidBody>()->setDamping( 0.1, 0.1 );
		animatedBow->GetSceneNode()->GetISceneNode()->setAnimationSpeed( 24.0f );
		animatedBow->GetSceneNode()->GetISceneNode()->setFrameLoop( 0, 19 );
		animatedBow->GetSceneNode()->GetISceneNode()->setLoopMode( false );
		
		// create player
		std::shared_ptr<Entity> player = engine->AddEntity( engine->GetNewEntityOfType("Player"), "Player", engine->GetCollisionShapeManager()->GetCapsule( 0.3f, 1.75f ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,15,0) ), 75.0 );
		engine->AttachCameraToEntity( "Player", btVector3( 0, 0.7f, 0 ) );
		player->SetCamera( engine->GetCamera() );
		
		// create map
		std::shared_ptr<Entity> map = engine->AddEntity( engine->GetNewEntityOfType("StaticEntity"), "TestMap", engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap"), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) ), 100000000.0f );		map->SetModel( engine->GetModel( "TestMap" ) );
	}
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	engine = NULL;
	
	return 0;
}



