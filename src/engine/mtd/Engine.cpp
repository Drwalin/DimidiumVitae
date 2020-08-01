
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

Entity* Engine::GetNewEntityOfType(const std::string &name) {
	return classFactory.GetNew(name);
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

Entity* Engine::AddEntity(const std::string className, std::shared_ptr<CollisionShape> shape, btTransform transform, btScalar mass, btVector3 inertia) {
	Entity *emptyEntity = GetNewEntityOfType(className);
	if(emptyEntity) {
		uint64_t id = GetAvailableEntityId();
		if(id != 0) {
			emptyEntity->Spawn(id, shape, transform);
			emptyEntity->SetMass(mass);
			entities[id] = emptyEntity;
			{
				Trigger *trigger = dynamic_cast<Trigger*>(emptyEntity);
				if(trigger)
					triggerEntities[id] = trigger;
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

void Engine::QueueEntityToDestroy(Entity *ptr) {
	if(ptr)
		entitiesQueuedToDestroy.push(ptr->GetId());
}

void Engine::QueueEntityToDestroy(uint64_t entityId) {
	if(entityId)
		entitiesQueuedToDestroy.push(entityId);
}

float Engine::GetDeltaTime() {
		return window->GetDeltaTime();
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
	
	sing::gui << "\n Entity Update Time: " << entityUpdateTime.GetSmoothTime()*1000.0f;
	sing::gui << " " << entityUpdateTime.GetPeakTime()*1000.0f;
	sing::gui << " " << entityUpdateTime.GetPitTime()*1000.0f;
	
	sing::gui << "\n Physics Simulation Time: " << physicsSimulationTime.GetSmoothTime()*1000.0f;
	sing::gui << " " << physicsSimulationTime.GetPeakTime()*1000.0f;
	sing::gui << " " << physicsSimulationTime.GetPitTime()*1000.0f;
}

std::shared_ptr<Camera> Engine::GetCamera() const {
	return window->GetCamera();
}

Entity* Engine::GetCameraParent() const {
	return cameraParent;
}

uint64_t Engine::GetAvailableEntityId() const {
	uint64_t id = entities.size()+1;
	while(true) {
		if(id!=0 && entities.find(id)==entities.end())
			return id;
		id += rand();
	}
	return 0;
}

void Engine::AttachCameraToEntity(uint64_t id, btVector3 location) {
	auto it = entities.find(id);
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

Entity* Engine::GetEntity(uint64_t id) {
	auto it = entities.find(id);
	if(it != entities.end()) {
		if(it->second)
			return it->second;
	}
	return NULL;
}

void Engine::DeleteEntity(uint64_t id) {
	auto it = entities.find(id);
	if(it != entities.end()) {
		if(it->second) {
			if(dynamic_cast<Trigger*>(it->second))
				triggerEntities.erase(id);
			
			if(it->second == cameraParent)
				cameraParent = NULL;
			
			delete it->second;
		}
		
		entities.erase(it);
	}
}

void Engine::BeginLoop() {
	window->BeginLoop();
}

void Engine::Init(EventResponser *eventResponser, const char *jsonConfigFile) {
	sing::engine = this;
	try {
		sing::fileSystem = fileSystem = new FileSystem();
		
		JSON json = fileSystem->ReadJSON(jsonConfigFile ? jsonConfigFile : "defaultEngineConfig.json");
		
		event = eventResponser;
		sing::world = world = new World;
		world->Init();
		sing::soundEngine = soundEngine = new SoundEngine();
		sing::window = window = new Window;
		window->Init(json["windowName"], json.HasKey("iconFile")?json["iconFile"].GetString():"", json["width"], json["height"], event, json.HasKey("fullscreen")?json["fullscreen"]:false);
		sing::resourceManager = resourceManager = new ResourceManager(json.HasKey("resourcePersistencyTime")?json["resourcePersistencyTime"]:60.0f);
		
		if(json.HasKey("lockMouse") ? json["lockMouse"] : true) {
			window->HideMouse();
			window->LockMouse();
		}
		
		if(json.HasKey("fpsLimit")) {
			window->SetFpsLimit(json["fpsLimit"]);
		}
		
		if(GetCamera() == NULL) {
			window->SetCamera(std::shared_ptr<Camera>(new Camera(false, json["width"], json["height"], sing::sceneManager->addCameraSceneNode())));
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
		if(it->second)
			delete it->second;
		else
			DEBUG("It shouldn't appear");
	}
	entities.clear();
	
	if(world) {
		world->Destroy();
		delete world;
		sing::world = world = NULL;
	}
	
	if(event) {
		delete event;
		event = NULL;
	}
	
	if(resourceManager) {
		delete resourceManager;
		sing::resourceManager = resourceManager = NULL;
	}
	
	if(soundEngine) {
		delete soundEngine;
		sing::soundEngine = soundEngine = NULL;
	}
	
	if(window) {
		window->Destroy();
		delete window;
		sing::window = window = NULL;
	}
	
	if(fileSystem) {
		delete fileSystem;
		sing::fileSystem = fileSystem = NULL;
	}
}

Engine::Engine() :
	classFactory("__Constructor_%_Function"){
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

#endif
