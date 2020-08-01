
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

#include <dll/ClassFactory.h>

class Engine {
public:
	
	Engine();
	~Engine();
	
	int GetNumberOfEntities() const;
	bool RegisterType(const std::string &className, const std::string &moduleName);
	bool RegisterModule(const std::string &moduleName);
	
	void QueueEntityToDestroy(Entity *ptr);
	void QueueEntityToDestroy(const std::string &name);
	void DeleteEntity(const std::string &name);
	
	std::string GetAvailableEntityName(const std::string &name);
	Entity* AddEntity(const std::string className, const std::string &name, std::shared_ptr<CollisionShape> shape, btTransform transform, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0));
	Entity* GetEntity(const std::string &name);
	
	Entity* RayTrace(btVector3 begin, btVector3 end, int channel, btVector3 &point, btVector3 &normal, const std::vector<Entity*> &ignoreEntities=std::vector<Entity*>());
	
	std::shared_ptr<Camera> GetCamera() const;
	Entity* GetCameraParent() const;
	void AttachCameraToEntity(const std::string &name, btVector3 location);
	
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
	
	Entity* GetNewEntityOfType(const std::string &name);
	inline void UpdateEntitiesOverlapp();
	inline void UpdateEntities(const float deltaTime);
	
private:
	
	ClassFactory<Entity> classFactory;
	
	World *world;
	Window *window;
	EventResponser *event;
	FileSystem *fileSystem;
	SoundEngine *soundEngine;
	ResourceManager *resourceManager;
	
	std::map<std::string, Entity*> entities;
	std::map<std::string, Trigger*> triggerEntities;
	
	std::queue<std::string> entitiesQueuedToDestroy;
	
	TimeCounter guiDrawTime;
	TimeCounter sceneDrawTime;
	TimeCounter physicsSimulationTime;
	TimeCounter entityUpdateTime;
	
	Entity *cameraParent;
	
	bool pausePhysics;
};

#endif

