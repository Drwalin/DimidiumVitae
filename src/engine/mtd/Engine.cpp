
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_CPP
#define ENGINE_CPP

#include "../css/Engine.h"

#include <JSON.hpp>

#include <Debug.h>
#include <Math.hpp>

#include <cassert>

int Engine::GetNumberOfEntities() const {
	return entities.size();
}

bool Engine::RegisterType(const std::string &className,
		const std::string &moduleName) {
	if((bool)classFactory.AddClass(className.c_str(), moduleName.c_str())) {
		return true;
	}
	MESSAGE(std::string("Cannot register type: ")+className+" from module: "+
			moduleName);
	return false;
}

bool Engine::RegisterModule(const std::string &modulePath) {
	if((bool)classFactory.AddModule(modulePath.c_str())) {
		return true;
	}
	MESSAGE(std::string("Cannot register module: ")+modulePath);
	return false;
}

Entity* Engine::AddEntity(const JSON& json) {
	if(!json.IsObject())
		return NULL;
	
	if(!classFactory.HasClass(json["class"].String()))
		return NULL;
	
	uint64_t id = 0;
	if(json.Object().count("id"))
		id = json["id"].Integer();
	
	if(id == 0)
		id = GetAvailableEntityId();
	
	entities[id] = NULL;
	Entity *emptyEntity = classFactory.GetNew(json["class"].String(), json);
	entities[id] = emptyEntity;
	if(json.Object().count("mass"))
		emptyEntity->SetMass(json["mass"].Real());
	else
		emptyEntity->SetMass(0.0f);
	
	Trigger *trigger = dynamic_cast<Trigger*>(emptyEntity);
	if(trigger)
		triggerEntities[id] = trigger;
	
	if(json.Object().count("model"))
		emptyEntity->SetModel(
				sing::resourceManager->GetModel(
					json["model"].String()));
	
	if(json.Object().count("scale")) {
		btVector3 s;
		emptyEntity->SetScale(s<<=json["scale"]);
	}
	
	return emptyEntity;
}

Entity* Engine::AddEntity(const std::string className, uint64_t id,
		std::shared_ptr<CollisionShape> shape, btTransform transform,
		btScalar mass) {
	JSON json;
	json["class"] = className;
	if(shape)
		json["shape"] = shape->GetJSON();
	json["mass"] = mass;
	json["id"] = id;
	json["transform"] <<= transform;
	return AddEntity(json);
}

Entity* Engine::AddEntity(const std::string className,
		std::shared_ptr<CollisionShape> shape, btTransform transform,
		btScalar mass) {
	return AddEntity(className, GetAvailableEntityId(), shape, transform, mass);
}

const std::map<uint64_t, Entity*>& Engine::GetEntities() const {
	return entities;
}


inline void Engine::UpdateEntitiesOverlapp() {
	for(auto it = triggerEntities.begin(); it != triggerEntities.end(); ++it) {
		it->second->NextOverlappingFrame();
	}
}

inline void Engine::UpdateEntities(float deltaTime) {
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

int Engine::StoreSimulationExecution() {
	return pausePhysics;
}

void Engine::RestoreSimulationExecution(int state) {
	pausePhysics = state;
}

void Engine::AsynchronousTick(float deltaTime) {
	physicsSimulationTime.SubscribeStart();
	if(!pausePhysics) {
		world->Tick(deltaTime, 1);
	}
	physicsSimulationTime.SubscribeEnd();
}

void Engine::SynchronousTick(float deltaTime) {
	entityUpdateTime.SubscribeStart();
	UpdateEntities(deltaTime);
	resourceManager->ResourceFreeingCycle(16);
	commandInterpreter->ExecuteSyncCommands(16);
	entityUpdateTime.SubscribeEnd();
	
	sing::gui << "\n Entity Update Time: " <<
		entityUpdateTime.GetSmoothTime()*1000.0f;
	sing::gui << " " << entityUpdateTime.GetPeakTime()*1000.0f;
	sing::gui << " " << entityUpdateTime.GetPitTime()*1000.0f;
	
	sing::gui << "\n Physics Simulation Time: " <<
		physicsSimulationTime.GetSmoothTime()*1000.0f;
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
	if(it != entities.end())
		cameraParent = it->second;
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
		sing::commandInterpreter = commandInterpreter =
				new CommandInterpreter();
		
		scriptsDll = std::shared_ptr<Dll>(new Dll("./scripts"));
		scriptsDll->Get<void(*)(void)>("AddEngineScripts")();
		
		JSON json = fileSystem->ReadJSON(jsonConfigFile ? jsonConfigFile :
				"defaultEngineConfig.json");
		
		event = eventResponser;
		sing::world = world = new World;
		world->Init();
		sing::soundEngine = soundEngine = new SoundEngine();
		sing::window = window = new Window;
		window->Init(json["windowName"],
				json.Object().count("iconFile") ?
				json["iconFile"].String() : "",
				json["width"].Integer(), json["height"].Integer(), event,
				json.Object().count("fullscreen") ?
				json["fullscreen"].Boolean() : false);
		sing::resourceManager = resourceManager =
			new ResourceManager(json.Object().count("resourcePersistencyTime") ?
					json["resourcePersistencyTime"].Real() : 60.0f);
		
		if(json.Object().count("lockMouse") ?
				json["lockMouse"].Boolean() : true) {
			window->HideMouse();
			window->LockMouse();
		}
		
		if(json.Object().count("fpsLimit")) {
			window->SetFpsLimit(json["fpsLimit"].Real());
		}
		
		if(GetCamera() == NULL) {
			window->SetCamera(std::shared_ptr<Camera>(new Camera(false,
							json["width"].Integer(), json["height"].Integer(),
							sing::sceneManager->addCameraSceneNode())));
			window->GetCamera()->SetFOV(json.Object().count("fov") ?
					json["fov"].Real() : 60.0f);
		}
		
		for(auto entry : json["modules"].Array())
			RegisterModule(entry.String());
		for(auto entry : json["types"].Array())
			RegisterType(entry["name"], entry["module"]);
		if(json.Object().count("fileArchives")) {
			irr::io::IFileSystem *fileSystem =
				window->GetDevice()->getFileSystem();
			for(auto entry : json["fileArchives"].Array())
				fileSystem->addFileArchive(entry.String().c_str(), false,
						false);
		}
	} catch(const std::string &e) {
		MESSAGE("Exception while initialising engine: " + e);
	} catch(const std::exception &e) {
		MESSAGE(std::string("Exception while initialising engine: ") +
				e.what());
	} catch(int e) {
		MESSAGE(std::string("Exception while initialising engine: ") +
				std::to_string(e));
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
	
	if(commandInterpreter) {
		delete commandInterpreter;
		sing::commandInterpreter = commandInterpreter = NULL;
	}
}

Engine::Engine() :
	classFactory("__Constructor_%_Function") {
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

Entity* Engine::RayTrace(btVector3 begin, btVector3 end, int channel,
		btVector3 &point, btVector3 &normal,
		const std::vector<Entity*> &ignoreEntities) {
	point = normal = btVector3(0,0,0);
	
	struct EngineRayResultCallback :
		public btCollisionWorld::ClosestRayResultCallback {
		std::set<Entity*> ignoreEntities;
		int channel;
		EngineRayResultCallback(btVector3 begin, btVector3 end,
				const std::vector<Entity*> &ignore, int channel) :
			btCollisionWorld::ClosestRayResultCallback(begin, end),
			ignoreEntities(ignore.begin(), ignore.end()), channel(channel) {
			m_collisionFilterGroup = channel;
			m_collisionFilterMask = channel;
		}
		virtual btScalar addSingleResult(
				btCollisionWorld::LocalRayResult &rayResult,
				bool normalInWorldSpace) {
			Entity* ent =
				(Entity*)rayResult.m_collisionObject->getUserPointer();
			if(ent) {
				if(ent->HasCommon(channel, channel) == 0) {
					return 1.0f;
				}
				if(ignoreEntities.count(ent) > 0) {
					return 1.0f;
				}
				return
					btCollisionWorld::ClosestRayResultCallback::addSingleResult(
							rayResult, normalInWorldSpace);
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
