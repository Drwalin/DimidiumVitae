
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
	engine->GetCamera()->Rotate(btVector3(float(dy)/160.0, float(dx)/160.0, 0.0));
}

irr::scene::ISceneNode *lightSceneNode = 0;

void Event::KeyPressedEvent(int keyCode) {
	Entity *player = engine->GetEntity(std::string("Player"));
	Entity *temp = NULL;
	btVector3 begin, end, point, normal, euler;
	Character *character = NULL;
	Player *playerPtr = NULL;
	if(player) {
		character = dynamic_cast<Character*>(player);
		playerPtr = dynamic_cast<Player*>(player);
	}
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	Entity *bow = engine->GetEntity("Bow");
	static Animation bowDraw = bow->GetSceneNode()->GetAnimation("draw");
	static Animation bowRelease = bow->GetSceneNode()->GetAnimation("release");
	static Animation bowFull = bow->GetSceneNode()->GetAnimation("full");
	
	switch(keyCode) {
	case irr::KEY_KEY_F:
		if(!lightSceneNode) {
			lightSceneNode = engine->GetWindow()->GetSceneManager()->addLightSceneNode(0, Math::GetIrrVec(engine->GetCamera()->GetWorldPosition()), irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 60.0f);
			lightSceneNode->setPosition(Math::GetIrrVec(engine->GetCamera()->GetWorldPosition()));
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
		fprintf(stderr, "\n Number of objects: %i ", engine->GetNumberOfEntities());
		break;
	case irr::KEY_KEY_O:
		Debug::SwitchDebugState();
		break;
		
	case irr::KEY_KEY_P:
		if(engine->GetWindow()->IsMouseLocked()) {
			engine->GetWindow()->UnlockMouse();
			engine->GetWindow()->ShowMouse();
		} else {
			engine->GetWindow()->LockMouse();
			engine->GetWindow()->HideMouse();
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
		euler = engine->GetCamera()->GetEulerRotation();
		temp = engine->AddEntity(engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Crate"), engine->GetResourceManager()->GetBox(btVector3(1,1,1)), btTransform(engine->GetCamera()->GetRotation(), engine->GetCamera()->GetWorldPosition() + engine->GetCamera()->GetForwardVector()), 20.0f);
		if(temp) {
			temp->SetModel(engine->GetResourceManager()->GetModel("Models/Crate01.obj"));
			temp->SetScale(btVector3(0.5, 0.5, 0.5));
			temp->GetBody()->setFriction(0.75);
			temp->GetBody<btRigidBody>()->setLinearVelocity(engine->GetCamera()->GetForwardVector()*16.0);
			temp->GetBody<btRigidBody>()->setDamping(0.1, 0.1);
		} else
			MESSAGE("Couldn't spawn new object");
		break;
		
	case irr::KEY_RBUTTON:
		temp = engine->AddEntity(engine->GetNewEntityOfType("DynamicEntity"), engine->GetAvailableEntityName("Ball"), engine->GetResourceManager()->GetSphere(1), btTransform(btQuaternion(btVector3(1,1,1),0), engine->GetCamera()->GetWorldPosition() + engine->GetCamera()->GetForwardVector()), 20.0f);
		if(temp) {
			temp->SetModel(engine->GetResourceManager()->GetModel("Models/Sphere.obj"));
			temp->SetScale(btVector3(0.5, 0.5, 0.5));
			temp->GetBody()->setFriction(0.75);
			temp->GetBody<btRigidBody>()->setLinearVelocity(engine->GetCamera()->GetForwardVector()*16.0);
			temp->GetBody<btRigidBody>()->setDamping(0.1, 0.1);
		} else
			MESSAGE("Couldn't spawn new object");
		break;
	}
}

void Event::KeyReleasedEvent(int keyCode) {
	Entity *player = engine->GetEntity(std::string("Player"));
	Character *character = NULL;
	if(player)
		character = dynamic_cast<Character*>(player);
	std::shared_ptr<MotionController> playerMotionController = character->GetMotionController();
	
	switch(keyCode) {
	case irr::KEY_ESCAPE:
		window->StartMenu<MainMenu>();
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
	Entity *player = engine->GetEntity(std::string("Player"));
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
		engine->GetCamera()->RotateCameraToLookAtPoint(btVector3(0,0,0), true);
		break;
		
	case irr::KEY_BACK:
	case irr::KEY_DELETE:
		if(keyCode == irr::KEY_DELETE) {
			begin = engine->GetCamera()->GetWorldPosition();
			end = begin + (engine->GetCamera()->GetForwardVector()*10000.0);
		} else {
			begin = btVector3(0,11.2,0);
			end = btVector3(0,11.2,300);
		}
		ptemp = engine->RayTrace(begin, end, CollisionDefaultMaskAll, point, normal, {player});
		if(ptemp) {
			if(ptemp->GetName() != "TestMap" && ptemp->GetName() != "Box") {
				if(ptemp->GetBody())
					ptemp->GetBody()->activate();
				engine->QueueEntityToDestroy(ptemp->GetName());
			}
		}
		break;
		
	case irr::KEY_UP:
		engine->GetCamera()->Rotate(btVector3(-(window->GetDeltaTime()), 0.0, 0.0)*2.0);
		break;
	case irr::KEY_DOWN:
		engine->GetCamera()->Rotate(btVector3((window->GetDeltaTime()), 0.0, 0.0)*2.0);
		break;
	case irr::KEY_RIGHT:
		engine->GetCamera()->Rotate(btVector3(0.0, (window->GetDeltaTime()), 0.0)*2.0);
		break;
	case irr::KEY_LEFT:
		engine->GetCamera()->Rotate(btVector3(0.0, -(window->GetDeltaTime()), 0.0)*2.0);
		break;
		
	case irr::KEY_SPACE:
		if(character && playerMotionController)
			playerMotionController->Jump();
		break;
		
	case irr::KEY_KEY_W:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(engine->GetCamera()->GetFlatForwardVector());
		break;
	case irr::KEY_KEY_A:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(engine->GetCamera()->GetFlatLeftVector());
		break;
	case irr::KEY_KEY_S:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(-engine->GetCamera()->GetFlatForwardVector());
		break;
	case irr::KEY_KEY_D:
		if(character && playerMotionController)
			playerMotionController->MoveInDirection(-engine->GetCamera()->GetFlatLeftVector());
		break;
	}
}

void Event::StringToEnterEvent(std::string str) {
	fprintf(stderr, "\n Input string: \"%s\"", str.c_str());
	
	Entity *player = engine->GetEntity(std::string("Player"));
	Entity *temp;
	btVector3 begin, end, point, normal;
	Character *character = NULL;
	if(player)
		character = dynamic_cast<Character*>(player);
	
	if(str == "Rel") {
		fprintf(stderr, "\n StringToEnterEvent(%s); ", str.c_str());
		engine->GetCamera()->SetRotation(btVector3(0, 0, 0));
	}
}

Event::Event() {
}

Event::~Event() {
}

#endif
