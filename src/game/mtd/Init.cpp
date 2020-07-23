
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF


#include <dll/DllImporter.h>
#include <Math.hpp>
#include <Debug.h>
#include <StdUtil.hpp>

#include <EventResponser.h>
#include <Engine.h>

#include "../css/Header.h"
#include "../css/Player.h"
#include "../css/Event.h"

#include <iostream>
#include <memory>

#include <cassert>
#include <ctime>

extern "C" int Init(int argc, char ** argv) {
	srand(time(NULL));
	
	Engine *engine = new Engine;
	engine->Init(new Event(), "Engine "+GetVersionString(), "", 800, 600, false);
	
	irr::io::IFileSystem *fileSystem = engine->GetWindow()->GetDevice()->getFileSystem();
	fileSystem->addFileArchive("media/Textures.zip", false, false);
	fileSystem->addFileArchive("media/Models.zip", false, false);
	fileSystem->addFileArchive("media/Shapes.zip", false, false);
	
	LoadModules(engine, "modules.list");
	
	LoadShapes(engine, "media/shapes.list");
	
	SoundSource musicSource(engine->GetResourceManager()->GetSound("./media/Sounds/track01.ogg"));
	musicSource.Set2D();
	musicSource.SetVolume(0.07f);
	musicSource.Loop(true);
	musicSource.Play();
	
	{
		// create animated bow
		std::shared_ptr<Entity> animatedBow = engine->AddEntity(engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Bow"), engine->GetCollisionShapeManager()->GetCylinder(0.3, 1), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(15,3,15)), 3.0f);
		animatedBow->SetModel(engine->GetResourceManager()->GetModel("Models/Bow02.x"));
		animatedBow->GetBody()->setFriction(0.75);
		animatedBow->GetBody<btRigidBody>()->setDamping(0.1, 0.1);
		animatedBow->GetSceneNode()->GetISceneNode()->setAnimationSpeed(24.0f);
		animatedBow->GetSceneNode()->GetISceneNode()->setFrameLoop(0, 19);
		animatedBow->GetSceneNode()->GetISceneNode()->setLoopMode(false);
		
		// create player
		std::shared_ptr<Entity> player = engine->AddEntity(engine->GetNewEntityOfType("Player"), "Player", engine->GetCollisionShapeManager()->GetCapsule(0.3f, 1.75f), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(-34,25,14)), 75.0);
		engine->AttachCameraToEntity("Player", btVector3(0, 0.7f, 0));
		player->SetCamera(engine->GetCamera());
		
		// create map
		float mapGridScale = 0.3f;
		for(float x = -100; x<=100.1; x+=24) {
			for(float z = -100; z<=100.1; z+=24) {
				std::shared_ptr<Entity> map = engine->AddEntity(engine->GetNewEntityOfType("StaticEntity"), "TestMap" + std::to_string(x) + "_" + std::to_string(z), engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap"), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(x,0,z)), 100000000.0f);
				map->SetModel(engine->GetResourceManager()->GetModel("Models/TechDemoMap.x"));
				map->SetScale(btVector3(1, 1, 1)*mapGridScale);
			}
		}/*
		std::shared_ptr<Entity> map = engine->AddEntity(engine->GetNewEntityOfType("StaticEntity"), "TestMap", engine->GetCollisionShapeManager()->GetCustomShape("TechDemoMap"), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(0,-100,0)), 100000000.0f);
		map->SetModel(engine->GetResourceManager()->GetModel("Models/TechDemoMap.x"));
		map->SetScale(btVector3(3, 3, 3));
		*/
		for(float x = -10; x<=10.1; x+=10) {
			for(float y = 30; y<=75.1; y+=3.5f) {
				for(float z = -10; z<=10.1; z+=10) {
					std::shared_ptr<Entity> box = engine->AddEntity(engine->GetNewEntityOfType("DynamicEntity"), std::to_string(x)+"_"+std::to_string(y)+"_"+std::to_string(z)+"box", engine->GetCollisionShapeManager()->GetBox(btVector3(1,1,1)), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(x,y,z)), 75.0f);
					box->SetModel(engine->GetResourceManager()->GetModel("Models/Crate01.obj"));
					box->SetScale(btVector3(0.8, 2.5, 0.8)*0.5f);
				}
			}
		}
	}
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	engine = NULL;
	
	return 0;
}



