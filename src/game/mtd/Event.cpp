
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_CPP
#define EVENT_CPP

#include <Window.h>
#include <Engine.h>

#include "../css/Event.h"
#include "../css/Character.h"
#include "../css/Player.h"
#include "../css/MotionController.h"
#include "../css/MainMenu.h"

#include <irrlicht/irrlicht.h>

#include <Debug.h>
#include <Math.hpp>

void Event::MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw) {
	sing::engine->GetCamera()->Rotate(btVector3(float(dy)/160.0, float(dx)/160.0, 0.0));
}

irr::scene::ISceneNode *lightSceneNode = 0;

void Event::KeyPressedEvent(int keyCode) {
	Entity *player = sing::engine->GetEntity(std::string("Player"));
	Entity *temp = NULL;
	btVector3 begin, end, point, normal, euler;
	Character *character = NULL;
	Player *playerPtr = NULL;
	if(player) {
		character = dynamic_cast<Character*>(player);
		playerPtr = dynamic_cast<Player*>(player);
	}
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	Entity *bow = sing::engine->GetEntity("Bow");
	static Animation bowDraw = bow->GetSceneNode()->GetAnimation("draw");
	static Animation bowRelease = bow->GetSceneNode()->GetAnimation("release");
	static Animation bowFull = bow->GetSceneNode()->GetAnimation("full");
	
	switch(keyCode) {
	case irr::KEY_KEY_F:
		if(!lightSceneNode) {
			lightSceneNode = sing::sceneManager->addLightSceneNode(0, Math::GetIrrVec(sing::engine->GetCamera()->GetWorldPosition()), irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 60.0f);
			lightSceneNode->setPosition(Math::GetIrrVec(sing::engine->GetCamera()->GetWorldPosition()));
		} else {
			lightSceneNode->remove();
			lightSceneNode = 0;
		}
		break;
		
	case irr::KEY_KEY_1:
		bowDraw.Play(false);
		break;
	case irr::KEY_KEY_2:
		bowRelease.Play(false);
		break;
	case irr::KEY_KEY_3:
		bowDraw.Play(true);
		break;
	case irr::KEY_KEY_4:
		bowRelease.Play(true);
		break;
	case irr::KEY_KEY_5:
		bowFull.Play(false);
		break;
	case irr::KEY_KEY_6:
		bowFull.Play(true);
		break;
		
	case irr::KEY_KEY_T:
		fprintf(stderr, "\n Number of objects: %i ", sing::engine->GetNumberOfEntities());
		break;
	case irr::KEY_KEY_O:
		Debug::SwitchDebugState();
		break;
		
	case irr::KEY_KEY_P:
		if(sing::window->IsMouseLocked()) {
			sing::window->UnlockMouse();
			sing::window->ShowMouse();
		} else {
			sing::window->LockMouse();
			sing::window->HideMouse();
		}
		break;
		
	case irr::KEY_LSHIFT:
		playerMotionController->StartRunning();
		break;
	case irr::KEY_LCONTROL:
		playerMotionController->StartCrouching();
		break;
	case irr::KEY_LMENU:
		playerMotionController->StartSneaking();
		break;
		
	case irr::KEY_LBUTTON:
		euler = sing::engine->GetCamera()->GetEulerRotation();
		temp = sing::engine->AddEntity(sing::engine->GetNewEntityOfType("DynamicEntity"), sing::engine->GetAvailableEntityName("Crate"), sing::resourceManager->GetBox(btVector3(1,1,1)), btTransform(sing::engine->GetCamera()->GetRotation(), sing::engine->GetCamera()->GetWorldPosition() + sing::engine->GetCamera()->GetForwardVector()), 20.0f);
		if(temp) {
			temp->SetModel(sing::resourceManager->GetModel("Models/Crate01.obj"));
			temp->SetScale(btVector3(0.5, 0.5, 0.5));
			temp->GetBody()->setFriction(0.75);
			temp->GetBody<btRigidBody>()->setLinearVelocity(sing::engine->GetCamera()->GetForwardVector()*16.0);
			temp->GetBody<btRigidBody>()->setDamping(0.1, 0.1);
		} else
			MESSAGE("Couldn't spawn new object");
		break;
		
	case irr::KEY_RBUTTON:
		temp = sing::engine->AddEntity(sing::engine->GetNewEntityOfType("DynamicEntity"), sing::engine->GetAvailableEntityName("Ball"), sing::resourceManager->GetSphere(1), btTransform(btQuaternion(btVector3(1,1,1),0), sing::engine->GetCamera()->GetWorldPosition() + sing::engine->GetCamera()->GetForwardVector()), 20.0f);
		if(temp) {
			temp->SetModel(sing::resourceManager->GetModel("Models/Sphere.obj"));
			temp->SetScale(btVector3(0.5, 0.5, 0.5));
			temp->GetBody()->setFriction(0.75);
			temp->GetBody<btRigidBody>()->setLinearVelocity(sing::engine->GetCamera()->GetForwardVector()*16.0);
			temp->GetBody<btRigidBody>()->setDamping(0.1, 0.1);
		} else
			MESSAGE("Couldn't spawn new object");
		break;
	}
}

void Event::KeyReleasedEvent(int keyCode) {
	Entity *player = sing::engine->GetEntity(std::string("Player"));
	Character *character = NULL;
	if(player)
		character = dynamic_cast<Character*>(player);
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	switch(keyCode) {
	case irr::KEY_ESCAPE:
		sing::window->StartMenu<MainMenu>();
		break;
		
	case irr::KEY_LSHIFT:
		playerMotionController->StopRunning();
		break;
	case irr::KEY_LCONTROL:
		playerMotionController->StopCrouching();
		break;
	case irr::KEY_LMENU:
		playerMotionController->StopSneaking();
		break;
	}
}

void Event::KeyHoldedEvent(int keyCode) {
	Entity *player = sing::engine->GetEntity(std::string("Player"));
	Entity *temp;
	btVector3 begin, end, point, normal, euler;
	Character *character = NULL;
	Player *playerPtr = NULL;
	Entity* ptemp=NULL;
	if(player) {
		character = dynamic_cast<Character*>(player);
		playerPtr = dynamic_cast<Player*>(player);
	}
	
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	btVector3 vector;
	
	switch(keyCode) {
	case irr::KEY_MBUTTON:
		sing::engine->GetCamera()->RotateCameraToLookAtPoint(btVector3(0,0,0), true);
		break;
		
	case irr::KEY_BACK:
	case irr::KEY_DELETE:
		if(keyCode == irr::KEY_DELETE) {
			begin = sing::engine->GetCamera()->GetWorldPosition();
			end = begin + (sing::engine->GetCamera()->GetForwardVector()*10000.0);
		} else {
			begin = btVector3(0,11.2,0);
			end = btVector3(0,11.2,300);
		}
		ptemp = sing::engine->RayTrace(begin, end, CollisionGroupVisible | CollisionGroupSight, point, normal, {player});
		if(ptemp) {
			if(ptemp->GetName() != "TestMap" && ptemp->GetName() != "Box") {
				if(ptemp->GetBody())
					ptemp->GetBody()->activate();
				sing::engine->QueueEntityToDestroy(ptemp->GetName());
			}
		}
		break;
		
	case irr::KEY_UP:
		sing::engine->GetCamera()->Rotate(btVector3(-(sing::window->GetDeltaTime()), 0.0, 0.0)*2.0);
		break;
	case irr::KEY_DOWN:
		sing::engine->GetCamera()->Rotate(btVector3((sing::window->GetDeltaTime()), 0.0, 0.0)*2.0);
		break;
	case irr::KEY_RIGHT:
		sing::engine->GetCamera()->Rotate(btVector3(0.0, (sing::window->GetDeltaTime()), 0.0)*2.0);
		break;
	case irr::KEY_LEFT:
		sing::engine->GetCamera()->Rotate(btVector3(0.0, -(sing::window->GetDeltaTime()), 0.0)*2.0);
		break;
		
	case irr::KEY_SPACE:
		if(character && playerMotionController)
			playerMotionController->Jump();
		break;
		
	case irr::KEY_KEY_W:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(sing::engine->GetCamera()->GetFlatForwardVector());
		break;
	case irr::KEY_KEY_A:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(sing::engine->GetCamera()->GetFlatLeftVector());
		break;
	case irr::KEY_KEY_S:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(-sing::engine->GetCamera()->GetFlatForwardVector());
		break;
	case irr::KEY_KEY_D:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(-sing::engine->GetCamera()->GetFlatLeftVector());
		break;
	}
}

void Event::StringToEnterEvent(std::string str) {
	fprintf(stderr, "\n Input string: \"%s\"", str.c_str());
	
	Entity *player = sing::engine->GetEntity(std::string("Player"));
	Entity *temp;
	btVector3 begin, end, point, normal;
	Character *character = NULL;
	if(player)
		character = dynamic_cast<Character*>(player);
	
	if(str == "Rel") {
		fprintf(stderr, "\n StringToEnterEvent(%s); ", str.c_str());
		sing::engine->GetCamera()->SetRotation(btVector3(0, 0, 0));
	}
}

Event::Event() {
}

Event::~Event() {
}

#endif
