
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
#include "../css/MainMenu.h"
#include "../css/LoadingScreen.h"

#include <iostream>
#include <memory>

#include <cassert>
#include <ctime>

extern "C" int Init(int argc, char ** argv) {
	srand(time(NULL));
	
	sing::engine = new Engine;
	sing::engine->Init(new Event(), argc>1 ? argv[1] : NULL);
	
	sing::window->StartMenu<LoadingScreen>();
	sing::window->Draw(false);
	/*
	SoundSource musicSource(sing::resourceManager->GetSound("./media/Sounds/track01.ogg"));
	musicSource.Set2D();
	musicSource.SetVolume(0.07f);
	musicSource.Loop(true);
	musicSource.Play();
	*/
	
	try {
		// create animated bow
		Entity *animatedBow = sing::engine->AddEntity("DynamicEntity", sing::engine->GetResourceManager()->GetCylinder(0.3, 1), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(15,3,15)), 3.0f);
		animatedBow->SetModel(sing::resourceManager->GetModel("Models/Bow02.x"));
		animatedBow->GetBody()->setFriction(0.75);
		animatedBow->GetBody<btRigidBody>()->setDamping(0.1, 0.1);
		animatedBow->GetSceneNode()->GetISceneNode()->setAnimationSpeed(24.0f);
		animatedBow->GetSceneNode()->GetISceneNode()->setFrameLoop(0, 19);
		animatedBow->GetSceneNode()->GetISceneNode()->setLoopMode(false);
		
		// map base
		auto mapShape = sing::resourceManager->GetCollisionShape("./media/Shapes/TechDemoMap_NoStairs.shape");
		auto mapModel = sing::resourceManager->GetModel("Models/TechDemoMap.x");
		{
			Entity *map = sing::engine->AddEntity("StaticEntity", mapShape, btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(0,-100,0)), 100000000.0f);
			map->SetModel(mapModel);
			map->SetScale(btVector3(3, 3, 3));
		}
		
		// create player
		Entity *player = sing::engine->AddEntity("Player", sing::resourceManager->GetCylinder(0.3f, 1.75f), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(-34,25,14)), 75.0);
		sing::engine->AttachCameraToEntity(player->GetId(), btVector3(0, 0.7f, 0));
		
		// create map
		float mapGridScale = 0.3f;
		for(float x = -100; x<=100.1; x+=24) {
			for(float z = -100; z<=100.1; z+=24) {
				Entity *map = sing::engine->AddEntity("StaticEntity", mapShape, btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(x,0,z)), 100000000.0f);
				map->SetModel(mapModel);
				map->SetScale(btVector3(1, 1, 1)*mapGridScale);
			}
		}
		
		// create boxes
		auto boxModel = sing::resourceManager->GetModel("Models/Crate01.obj");
		for(float x = -10; x<=10.1; x+=10) {
			for(float y = 30; y<=75.1; y+=3.5f) {
				for(float z = -10; z<=10.1; z+=10) {
					Entity *box = sing::engine->AddEntity("DynamicEntity", sing::resourceManager->GetBox(btVector3(1,1,1)), btTransform(btQuaternion(btVector3(1,1,1),0), btVector3(x,y,z)), 75.0f);
					box->SetModel(boxModel);
					box->SetScale(btVector3(0.8, 2.5, 0.8)*0.5f);
				}
			}
		}
	} catch(std::string e) {
		MESSAGE("\n Excepion while creating map: " + e);
	} catch(std::exception e) {
		MESSAGE("\n Excepion while creating map: " + std::string(e.what()));
	} catch(char *e) {
		MESSAGE("\n Excepion while creating map: " + std::string(e));
	} catch(...) {
		MESSAGE("\n Unknown exception while creating map");
	}
	
	sing::window->StartMenu<MainMenu>();
	sing::engine->BeginLoop();
	
	sing::engine->Destroy();
	delete sing::engine;
	sing::engine = NULL;
	
	return 0;
}


