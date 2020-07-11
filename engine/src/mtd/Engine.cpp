
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_CPP
#define ENGINE_CPP

#include "../css/Engine.h"

#include "../lib/Debug.h"
#include "../lib/Math.hpp"

#include <cassert>

void Engine::RegisterEngineCoreEntityClasses() {
	this->RegisterType("StaticEntity", "engine", "./engine.dll");
	this->RegisterType("DynamicEntity", "engine", "./engine.dll");
	this->RegisterType("Trigger", "engine", "./engine.dll");
	this->RegisterType("MotionControllerTrigger", "engine", "./engine.dll");
}

int Engine::GetNumberOfEntities() const {
	return this->entities.size();
}

std::shared_ptr<Entity> Engine::GetNewEntityOfType(const std::string &name) {
	std::shared_ptr<Entity> entity = this->classFactory.GetClassInstantiator(name.c_str())->New();
	entity->Init(this);
	return entity;
}

bool Engine::RegisterType(const std::string &className, const std::string &moduleName, const std::string &modulePath) {
	this->classFactory.AddModule(moduleName.c_str(), modulePath.c_str());
	return (bool)this->classFactory.AddClass(className.c_str(), moduleName.c_str());
}

std::shared_ptr<Entity> Engine::AddEntity(std::shared_ptr<Entity> emptyEntity, const std::string &name, std::shared_ptr<btCollisionShape> shape, btTransform transform, btScalar mass, btVector3 inertia) {
	if(emptyEntity) {
		auto it = this->entities.find(name);
		if(it == this->entities.end()) {
			emptyEntity->Init(this);
			emptyEntity->Spawn(emptyEntity, name, shape, transform);
			emptyEntity->SetMass(mass);
			this->entities[name] = emptyEntity;
			{
				std::shared_ptr<Trigger> trigger = std::dynamic_pointer_cast<Trigger>(emptyEntity);
				if(trigger.get())
					this->triggerEntities[name] = trigger;
			}
			return emptyEntity;
		}
	}
	return NULL;
}

inline void Engine::UpdateEntitiesOverlapp() {
	for(auto it = this->triggerEntities.begin(); it != this->triggerEntities.end(); ++it) {
		it->second->NextOverlappingFrame();
	}
}

inline void Engine::UpdateEntities(const float deltaTime) {
	while(!this->entitiesQueuedToDestroy.empty()) {
		this->DeleteEntity(this->entitiesQueuedToDestroy.front());
		this->entitiesQueuedToDestroy.pop();
	}
	
	this->UpdateEntitiesOverlapp();
	
	for(auto it = this->entities.begin(); it != this->entities.end(); ++it)
		it->second->Tick(deltaTime);
	
	if(this->cameraParent) {
		this->GetCamera()->SetCameraParentTransform(this->cameraParent->GetTransform());
		SoundEngine::SetListenerTransform(this->GetCamera()->GetTransform());
	}
}

void Engine::QueueEntityToDestroy(std::shared_ptr<Entity> ptr) {
	if(ptr)
		this->entitiesQueuedToDestroy.push(ptr->GetName());
}

void Engine::QueueEntityToDestroy(const std::string &name) {
	this->entitiesQueuedToDestroy.push(name);
}

float Engine::GetDeltaTime() {
	if(this->window)
		return this->window->GetDeltaTime();
	return 1.0f/60.0f;
}

CollisionShapeManager *Engine::GetCollisionShapeManager() {
	return this->collisionShapeManager;
}

World *Engine::GetWorld() {
	return this->world;
}

Window *Engine::GetWindow() {
	return this->window;
}

SoundEngine *Engine::GetSoundEngine() {
	return this->soundEngine;
}

SoundsManager *Engine::GetSoundsManager() {
	return this->soundsManager;
}

void Engine::PauseSimulation() {
	this->pausePhysics = true;
}

void Engine::ResumeSimulation() {
	this->pausePhysics = false;
}

int Engine::CalculateNumberOfSimulationsPerFrame(const float deltaTime) {
	return 1;
	float fps = 1.0 / deltaTime;
	if(fps >= 57.0)
		return 100;
	if(fps >= 50.0)
		return 90;
	if(fps >= 40.0)
		return 60;
	if(fps >= 30.0)
		return 30;
	if(fps >= 20.0)
		return 5;
	return 1;
}

void Engine::AsynchronousTick(const float deltaTime) {
	this->physicsSimulationTime.SubscribeStart();
	if(!this->pausePhysics)
		this->world->Tick(deltaTime, this->CalculateNumberOfSimulationsPerFrame(deltaTime));
	this->physicsSimulationTime.SubscribeEnd();
}

void Engine::SynchronousTick(const float deltaTime) {
	this->entityUpdateTime.SubscribeStart();
	this->UpdateEntities(deltaTime);
	this->entityUpdateTime.SubscribeEnd();
	
	this->GetWindow()->GetGUI() << "\n Entity Update Time: " << this->entityUpdateTime.GetSmoothTime()*1000.0f;
	this->GetWindow()->GetGUI() << " " << this->entityUpdateTime.GetPeakTime()*1000.0f;
	this->GetWindow()->GetGUI() << " " << this->entityUpdateTime.GetPitTime()*1000.0f;
	
	this->GetWindow()->GetGUI() << "\n Physics Simulation Time: " << this->physicsSimulationTime.GetSmoothTime()*1000.0f;
	this->GetWindow()->GetGUI() << " " << this->physicsSimulationTime.GetPeakTime()*1000.0f;
	this->GetWindow()->GetGUI() << " " << this->physicsSimulationTime.GetPitTime()*1000.0f;
}

std::shared_ptr<Camera> Engine::GetCamera() const {
	return this->window->GetCamera();
}

std::shared_ptr<Entity> Engine::GetCameraParent() const {
	return this->cameraParent;
}

std::string Engine::GetAvailableEntityName(const std::string &name) {
	if(this->entities.find(name) == this->entities.end()) {
		return name;
	}
	for(int i = 0;; ++i) {
		int id = i^rand();
		if(this->entities.find(name+std::to_string(id)) == this->entities.end()) {
			return name+std::to_string(id);
		}
	}
	return name;
}

void Engine::AttachCameraToEntity(const std::string &name, btVector3 location) {
	auto it = this->entities.find(name);
	if(it != this->entities.end()) {
		this->cameraParent = it->second;
	}
	else
		this->cameraParent = NULL;
	this->GetCamera()->SetRelativePosition(location);
}

bool Engine::SetCustomModelName(const std::string &name, std::shared_ptr<Model> mdl) {
	auto it = this->models.find(name);
	if(it == this->models.end()) {
		this->models[name] = mdl;
		return true;
	}
	return false;
}

std::shared_ptr<Model> Engine::LoadModel(const std::string &name) {
	auto it = this->models.find(name);
	if(it != this->models.end()) {
		if(it->second) {
			return it->second;
		}
		else {
			this->models.erase(it);
		}
	}
	else {
		std::shared_ptr<Model> mdl(new Model);
		if(mdl->LoadMesh(this, name) == false) {
			mdl.reset();
			return std::shared_ptr<Model>();
		}
		else {
			this->models[name] = mdl;
			return mdl;
		}
	}
	return std::shared_ptr<Model>();
}

std::shared_ptr<Model> Engine::GetModel(const std::string &name) {
	return this->LoadModel(name);
}

std::shared_ptr<Entity> Engine::GetEntity(const std::string &name) {
	auto it = this->entities.find(name);
	if(it != this->entities.end()) {
		if(it->second)
			return it->second;
		else
			this->entities.erase(it);
	}
	std::shared_ptr<Entity> ret;
	return ret;
}

void Engine::DeleteEntity(const std::string &name) {
	auto it = this->entities.find(name);
	if(it != this->entities.end()) {
		if(it->second) {
			if(dynamic_cast<Trigger*>(it->second.get())) {
				this->triggerEntities.erase(name);
			}
			
			if(it->second == this->cameraParent)
				this->cameraParent = NULL;
			
			it->second->Destroy();
			it->second.reset();
		}
		
		this->entities.erase(it);
	}
}

void Engine::BeginLoop() {
	this->pausePhysics = false;
	this->window->BeginLoop();
}

void Engine::Init(EventResponser *eventResponser, const std::string &windowName, const std::string &iconFile, int width, int height, bool fullscreen) {
	this->Destroy();
	this->event = eventResponser;
	this->event->SetEngine(this);
	this->world = new World;
	this->world->Init();
	this->window = new Window;
	this->window->Init(this, windowName, iconFile, width, height, this->event, fullscreen);
	this->soundEngine = new SoundEngine();
	this->soundsManager = new SoundsManager();
	
	this->window->HideMouse();
	this->window->LockMouse();
	
	this->collisionShapeManager = new CollisionShapeManager(this);
	
	if(this->GetCamera() == NULL) {
		this->window->SetCamera(std::shared_ptr<Camera>(new Camera(this, false, width, height, this->window->GetSceneManager()->addCameraSceneNode())));
		this->window->GetCamera()->GetCameraNode()->setFOV(70.0f *Math::PI / 180.0f);
	}
	
	this->RegisterEngineCoreEntityClasses();
}

void Engine::Destroy() {
	this->cameraParent = NULL;
	
	for(auto it = this->entities.begin(); it != this->entities.end(); ++it) {
		if(it->second) {
			it->second->Destroy();
			it->second.reset();
		}
		else
			DEBUG("It shouldn't appear");
	}
	this->entities.clear();
	
	for(auto it = this->models.begin(); it != this->models.end(); ++it) {
		if(it->second)
			it->second.reset();
		else
			DEBUG("It shouldn't appear");
	}
	this->models.clear();
	
	if(this->window) {
		this->window->Destroy();
		delete this->window;
		this->window = NULL;
	}
	
	if(this->world) {
		this->world->Destroy();
		delete this->world;
		this->world = NULL;
	}
	
	if(this->event) {
		delete this->event;
		this->event = NULL;
	}
	
	if(this->collisionShapeManager) {
		this->collisionShapeManager->Destroy();
		delete this->collisionShapeManager;
		this->collisionShapeManager = NULL;
	}
	
	if(this->soundsManager) {
		delete this->soundsManager;
		this->soundsManager = NULL;
	}
	
	if(this->soundEngine) {
		delete this->soundEngine;
		this->soundEngine = NULL;
	}
	
	this->pausePhysics = false;
}

Engine::Engine() {
	this->event = NULL;
	this->world = NULL;
	this->window = NULL;
	this->pausePhysics = true;
	this->collisionShapeManager = NULL;
	this->soundsManager = NULL;
	this->soundEngine = NULL;
}

Engine::~Engine() {
	this->Destroy();
}

#include "../lib/dll\ClassFactory.cpp"

#endif
