
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

#include "Model.h"
#include "Entity.h"
#include "Trigger.h"
#include "World.h"
#include "Window.h"
#include "EventResponser.h"
#include "SoundEngine.h"
#include "ResourceManager.h"
#include "FileSystem.h"
#include "CommandInterpreter.h"

#include <ClassFactory.h>

class Engine {
public:
	
	Engine();
	~Engine();
	
	int GetNumberOfEntities() const;
	bool RegisterType(const std::string &className, const std::string &moduleName);
	bool RegisterModule(const std::string &moduleName);
	
	void QueueEntityToDestroy(Entity *ptr);
	void QueueEntityToDestroy(uint64_t id);
	void DeleteEntity(uint64_t entityId);
	
	uint64_t GetAvailableEntityId() const;
	Entity* AddEntity(const JSON& json);
	Entity* AddEntity(const std::string className, uint64_t id, std::shared_ptr<CollisionShape> shape, btTransform transform, btScalar mass = 1.0f);
	Entity* AddEntity(const std::string className, std::shared_ptr<CollisionShape> shape, btTransform transform, btScalar mass = 1.0f);
	Entity* GetEntity(uint64_t entityId);
	const std::map<uint64_t, Entity*>& GetEntities() const;
	
	
	Entity* RayTrace(btVector3 begin, btVector3 end, int channel, btVector3 &point, btVector3 &normal, const std::vector<Entity*> &ignoreEntities=std::vector<Entity*>());
	
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
	
	int CalculateNumberOfSimulationsPerFrame(const float deltaTime);
	void SynchronousTick(const float deltaTime);
	void AsynchronousTick(const float deltaTime);
	
	void Init(EventResponser *eventResponser, const char *jsonConfigFile);
	void BeginLoop();
	
	void Destroy();
	
	friend class EventResponser;
	
private:
	
	inline void UpdateEntitiesOverlapp();
	inline void UpdateEntities(const float deltaTime);
	
private:
	
	ClassFactory<Entity, const JSON&> classFactory;
	std::shared_ptr<Dll> scriptsDll;
	
	World *world;
	Window *window;
	EventResponser *event;
	FileSystem *fileSystem;
	SoundEngine *soundEngine;
	ResourceManager *resourceManager;
	CommandInterpreter *commandInterpreter;
	
	std::map<uint64_t, Entity*> entities;
	std::map<uint64_t, Trigger*> triggerEntities;
	
	std::queue<uint64_t> entitiesQueuedToDestroy;
	
	TimeCounter guiDrawTime;
	TimeCounter sceneDrawTime;
	TimeCounter physicsSimulationTime;
	TimeCounter entityUpdateTime;
	
	Entity *cameraParent;
	
	bool pausePhysics;
};

#endif

