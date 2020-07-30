
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_CPP
#define ENGINE_CPP

#include "../css/Engine.h"

#include <JSON.h>

#include <Debug.h>
#include <Math.hpp>

#include <cassert>

int Engine::GetNumberOfEntities() const {
	return entities.size();
}

std::shared_ptr<Entity> Engine::GetNewEntityOfType(const std::string &name) {
	std::shared_ptr<Entity> entity = classFactory.GetClassInstantiator(name.c_str())->New();
	entity->Init(this);
	return entity;
}

bool Engine::RegisterType(const std::string &className, const std::string &moduleName) {
	if((bool)classFactory.AddClass(className.c_str(), moduleName.c_str())) {
		return true;
	}
	MESSAGE(std::string("Cannot register type: ")+className+" from module: "+moduleName);
	return false;
}

bool Engine::RegisterModule(const std::string &modulePath) {
	if((bool)classFactory.AddModule(modulePath.c_str())) {
		return true;
	}
	MESSAGE(std::string("Cannot register module: ")+modulePath);
	return false;
}

std::shared_ptr<Entity> Engine::AddEntity(std::shared_ptr<Entity> emptyEntity, const std::string &name, std::shared_ptr<CollisionShape> shape, btTransform transform, btScalar mass, btVector3 inertia) {
	if(emptyEntity) {
		auto it = entities.find(name);
		if(it == entities.end()) {
			emptyEntity->Init(this);
			emptyEntity->Spawn(emptyEntity, name, shape, transform);
			emptyEntity->SetMass(mass);
			entities[name] = emptyEntity;
			{
				std::shared_ptr<Trigger> trigger = std::dynamic_pointer_cast<Trigger>(emptyEntity);
				if(trigger.get())
					triggerEntities[name] = trigger;
			}
			return emptyEntity;
		}
	}
	return NULL;
}

inline void Engine::UpdateEntitiesOverlapp() {
	for(auto it = triggerEntities.begin(); it != triggerEntities.end(); ++it) {
		it->second->NextOverlappingFrame();
	}
}

inline void Engine::UpdateEntities(const float deltaTime) {
	while(!entitiesQueuedToDestroy.empty()) {
		DeleteEntity(entitiesQueuedToDestroy.front());
		entitiesQueuedToDestroy.pop();
	}
	
	UpdateEntitiesOverlapp();
	
	for(auto it = entities.begin(); it != entities.end(); ++it)
		it->second->Tick(deltaTime);
	
	if(cameraParent) {
		GetCamera()->SetCameraParentTransform(cameraParent->GetTransform());
		SoundEngine::SetListenerTransform(GetCamera()->GetTransform());
	}
}

void Engine::QueueEntityToDestroy(std::shared_ptr<Entity> ptr) {
	if(ptr)
		entitiesQueuedToDestroy.push(ptr->GetName());
}

void Engine::QueueEntityToDestroy(const std::string &name) {
	entitiesQueuedToDestroy.push(name);
}

float Engine::GetDeltaTime() {
	if(window)
		return window->GetDeltaTime();
	return 1.0f/60.0f;
}

World *Engine::GetWorld() {
	return world;
}

Window *Engine::GetWindow() {
	return window;
}

FileSystem* Engine::GetFileSystem() {
	return fileSystem;
}

SoundEngine *Engine::GetSoundEngine() {
	return soundEngine;
}

void Engine::PauseSimulation() {
	pausePhysics = true;
}

void Engine::ResumeSimulation() {
	pausePhysics = false;
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
	physicsSimulationTime.SubscribeStart();
	if(!pausePhysics)
		world->Tick(deltaTime, CalculateNumberOfSimulationsPerFrame(deltaTime));
	physicsSimulationTime.SubscribeEnd();
}

void Engine::SynchronousTick(const float deltaTime) {
	entityUpdateTime.SubscribeStart();
	UpdateEntities(deltaTime);
	resourceManager->ResourceFreeingCycle(16);
	entityUpdateTime.SubscribeEnd();
	
	window->GetGUI() << "\n Entity Update Time: " << entityUpdateTime.GetSmoothTime()*1000.0f;
	window->GetGUI() << " " << entityUpdateTime.GetPeakTime()*1000.0f;
	window->GetGUI() << " " << entityUpdateTime.GetPitTime()*1000.0f;
	
	window->GetGUI() << "\n Physics Simulation Time: " << physicsSimulationTime.GetSmoothTime()*1000.0f;
	window->GetGUI() << " " << physicsSimulationTime.GetPeakTime()*1000.0f;
	window->GetGUI() << " " << physicsSimulationTime.GetPitTime()*1000.0f;
}

std::shared_ptr<Camera> Engine::GetCamera() const {
	return window->GetCamera();
}

std::shared_ptr<Entity> Engine::GetCameraParent() const {
	return cameraParent;
}

std::string Engine::GetAvailableEntityName(const std::string &name) {
	if(entities.find(name) == entities.end()) {
		return name;
	}
	for(int i = 0;; ++i) {
		int id = i^rand();
		if(entities.find(name+std::to_string(id)) == entities.end()) {
			return name+std::to_string(id);
		}
	}
	return name;
}

void Engine::AttachCameraToEntity(const std::string &name, btVector3 location) {
	auto it = entities.find(name);
	if(it != entities.end()) {
		cameraParent = it->second;
	}
	else
		cameraParent = NULL;
	GetCamera()->SetRelativePosition(location);
}

ResourceManager* Engine::GetResourceManager() {
	return resourceManager;
}

std::shared_ptr<Entity> Engine::GetEntity(const std::string &name) {
	auto it = entities.find(name);
	if(it != entities.end()) {
		if(it->second)
			return it->second;
		else
			entities.erase(it);
	}
	std::shared_ptr<Entity> ret;
	return ret;
}

void Engine::DeleteEntity(const std::string &name) {
	auto it = entities.find(name);
	if(it != entities.end()) {
		if(it->second) {
			if(dynamic_cast<Trigger*>(it->second.get())) {
				triggerEntities.erase(name);
			}
			
			if(it->second == cameraParent)
				cameraParent = NULL;
			
			it->second->Destroy();
			it->second.reset();
		}
		
		entities.erase(it);
	}
}

void Engine::BeginLoop() {
	window->BeginLoop();
}

void Engine::Init(EventResponser *eventResponser, const char *jsonConfigFile) {
	Destroy();
	try {
		fileSystem = new FileSystem(this);
		
		JSON json = fileSystem->ReadJSON(jsonConfigFile ? jsonConfigFile : "defaultEngineConfig.json");
		
		event = eventResponser;
		event->SetEngine(this);
		world = new World;
		world->Init();
		soundEngine = new SoundEngine();
		window = new Window;
		window->Init(this, json["windowName"], json.HasKey("iconFile")?json["iconFile"].GetString():"", json["width"], json["height"], event, json.HasKey("fullscreen")?json["fullscreen"]:false);
		resourceManager = new ResourceManager(this, json.HasKey("resourcePersistencyTime")?json["resourcePersistencyTime"]:60.0f);
		
		if(json.HasKey("lockMouse") ? json["lockMouse"] : true) {
			window->HideMouse();
			window->LockMouse();
		}
		
		if(json.HasKey("fpsLimit")) {
			window->SetFpsLimit(json["fpsLimit"]);
		}
		
		if(GetCamera() == NULL) {
			window->SetCamera(std::shared_ptr<Camera>(new Camera(this, false, json["width"], json["height"], window->GetSceneManager()->addCameraSceneNode())));
			window->GetCamera()->SetFOV(json.HasKey("fov")?json["fov"]:60.0f);
		}
		
		for(auto entry : json["modules"])
			RegisterModule(entry.Value().GetString());
		for(auto entry : json["types"])
			RegisterType(entry.Value()["name"], entry.Value()["module"]);
		if(json.HasKey("fileArchives")) {
			irr::io::IFileSystem *fileSystem = window->GetDevice()->getFileSystem();
			for(auto entry : json["fileArchives"])
				fileSystem->addFileArchive(entry.Value().GetString().c_str(), false, false);
		}
	} catch(const std::string &e) {
		MESSAGE("Exception while initialising engine: " + e);
	} catch(const std::exception &e) {
		MESSAGE(std::string("Exception while initialising engine: ") + e.what());
	} catch(int e) {
		MESSAGE(std::string("Exception while initialising engine: ") + std::to_string(e));
	} catch(...) {
		MESSAGE("Unknown exception while initialising engine");
	}
}

void Engine::Destroy() {
	cameraParent = NULL;
	
	triggerEntities.clear();
	for(auto it = entities.begin(); it != entities.end(); ++it) {
		if(it->second) {
			it->second->Destroy();
			it->second.reset();
		}
		else
			DEBUG("It shouldn't appear");
	}
	entities.clear();
	
	if(world) {
		world->Destroy();
		delete world;
		world = NULL;
	}
	
	if(event) {
		delete event;
		event = NULL;
	}
	
	if(resourceManager) {
		delete resourceManager;
		resourceManager = NULL;
	}
	
	if(soundEngine) {
		delete soundEngine;
		soundEngine = NULL;
	}
	
	if(window) {
		window->Destroy();
		delete window;
		window = NULL;
	}
	
	pausePhysics = false;
	
	if(fileSystem) {
		delete fileSystem;
		fileSystem = NULL;
	}
}

Engine::Engine() {
	resourceManager = NULL;
	event = NULL;
	world = NULL;
	window = NULL;
	fileSystem = NULL;
	pausePhysics = true;
	soundEngine = NULL;
}

Engine::~Engine() {
	Destroy();
}

Entity* Engine::RayTrace(btVector3 begin, btVector3 end, int channel, btVector3 &point, btVector3 &normal, const std::vector<Entity*> &ignoreEntities) {
	point = normal = btVector3(0,0,0);
	
	struct EngineRayResultCallback : public btCollisionWorld::ClosestRayResultCallback {
		std::set<Entity*> ignoreEntities;
		int channel;
		EngineRayResultCallback(btVector3 begin, btVector3 end, const std::vector<Entity*> &ignore, int channel) :
			ignoreEntities(ignore.begin(), ignore.end()), channel(channel), btCollisionWorld::ClosestRayResultCallback(begin, end) {
			m_collisionFilterGroup = channel;
			m_collisionFilterMask = channel;
		}
		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) {
			Entity* ent = (Entity*)rayResult.m_collisionObject->getUserPointer();
			if(ent) {
				if(ent->HasCommon(channel, channel) == 0) {
					return 1.0f;
				}
				if(ignoreEntities.count(ent) > 0) {
					return 1.0f;
				}
				return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
			}
			return 1.0f;
		}
	};
	
	EngineRayResultCallback rayTraceResult(begin, end, ignoreEntities, channel);
	world->GetDynamicsWorld()->rayTest(begin, end, rayTraceResult);
	
	if(rayTraceResult.hasHit()) {
		point = rayTraceResult.m_hitPointWorld;
		normal = rayTraceResult.m_hitNormalWorld;
		return (Entity*)rayTraceResult.m_collisionObject->getUserPointer();
	}
	return NULL;
}

#include "../lib/dll/ClassFactory.cpp"

#endif
