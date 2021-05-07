
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_H
#define ENGINE_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <memory>

#include <resources/Model.h>
#include <resources/ResourceManager.h>
#include <resources/SoundEngine.h>

#include <entities/Entity.h>
#include <entities/Trigger.h>
#include <entities/EntityFactory.h>

#include "Simulation.h"
#include "World.h"
#include "Window.h"
#include "EventResponser.h"
#include "FileSystem.h"
#include "CommandInterpreter.h"

class Engine {
public:
	
	Engine();
	~Engine();
	
	int GetNumberOfEntities() const;
	
	void QueueEntityToDestroy(Entity *ptr);
	void QueueEntityToDestroy(uint64_t id);
	void DeleteEntity(uint64_t entityId);
	
	uint64_t GetAvailableEntityId() const;
	
	Entity* AddEntity(Entity* entity);
	Entity* GetEntity(uint64_t entityId);
	const std::map<uint64_t, Entity*>& GetEntities() const;
	
	
	Entity* RayTrace(btVector3 begin, btVector3 end, int channel,
			btVector3 &point, btVector3 &normal,
			const std::vector<Entity*> &ignoreEntities=std::vector<Entity*>());
	
	std::shared_ptr<Camera> GetCamera() const;
	Entity* GetCameraParent() const;
	void AttachCameraToEntity(uint64_t id, btVector3 location);
	
	float GetDeltaTime();
	
	World* GetWorld();
	Window* GetWindow();
	FileSystem* GetFileSystem();
	SoundEngine* GetSoundEngine();
	ResourceManager* GetResourceManager();
	
	void PauseSimulation();
	void ResumeSimulation();
	int StoreSimulationExecution();
	void RestoreSimulationExecution(int state);
	
	void SynchronousTick();
	void AsynchronousTick();
	
	void Init(EventResponser *eventResponser, const char *jsonConfigFile);
	void BeginLoop();
	
	void Destroy();
	
	friend class EventResponser;
	
	void SetFpsLimit(float fps);
	float GetSmoothFps();
	
	void QueueQuit();
	
private:
	
	void UpdateDeltaTime();
	
	inline void UpdateEntitiesOverlapp();
	inline void UpdateEntities(const float deltaTime);
	
private:
	
	std::shared_ptr<Dll> scriptsDll;
	
	World *world;
	Window *window;
	EventResponser *event;
	FileSystem *fileSystem;
	SoundEngine *soundEngine;
	ResourceManager *resourceManager;
	CommandInterpreter *commandInterpreter;
	EntityFactory entityFactory;
	Simulation simulation;
	
	std::map<uint64_t, Entity*> entities;
	std::map<uint64_t, Trigger*> triggerEntities;
	
	std::queue<uint64_t> entitiesQueuedToDestroy;
	
	TimeCounter guiDrawTime;
	TimeCounter sceneDrawTime;
	TimeCounter physicsSimulationTime;
	TimeCounter entityUpdateTime;
	
	Entity *cameraParent;
	
	bool pausePhysics;
	
	TimePoint beginTime;
	float deltaTime;
	
	float fpsLimit;
	
	std::atomic<bool> quitWhenPossible;
};

#endif

