
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

#include <dll/ClassFactory.h>

class Engine {
public:
	
	Engine();
	~Engine();
	
	int GetNumberOfEntities() const;
	std::shared_ptr<Entity> GetNewEntityOfType(const std::string &name);
	bool RegisterType(const std::string &className, const std::string &moduleName);
	bool RegisterModule(const std::string &moduleName);
	
	void QueueEntityToDestroy(std::shared_ptr<Entity> ptr);
	void QueueEntityToDestroy(const std::string &name);
	void DeleteEntity(const std::string &name);
	
	std::string GetAvailableEntityName(const std::string &name);
	std::shared_ptr<Entity> AddEntity(std::shared_ptr<Entity> emptyEntity, const std::string &name, std::shared_ptr<CollisionShape> shape, btTransform transform, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0));
	std::shared_ptr<Entity> GetEntity(const std::string &name);
	
	Entity* RayTrace(btVector3 begin, btVector3 end, int channel, btVector3 &point, btVector3 &normal, const std::vector<Entity*> &ignoreEntities=std::vector<Entity*>());
	
	
	std::shared_ptr<Camera> GetCamera() const;
	std::shared_ptr<Entity> GetCameraParent() const;
	void AttachCameraToEntity(const std::string &name, btVector3 location);
	
	float GetDeltaTime();
	
	World* GetWorld();
	Window* GetWindow();
	SoundEngine* GetSoundEngine();
	ResourceManager* GetResourceManager();
	
	void PauseSimulation();
	void ResumeSimulation();
	
	int CalculateNumberOfSimulationsPerFrame(const float deltaTime);
	void SynchronousTick(const float deltaTime);
	void AsynchronousTick(const float deltaTime);
	
	void Init(EventResponser *eventResponser, const char *jsonConfigFile);
	
	//void Init(EventResponser *eventResponser, const std::string &windowName, const std::string &iconFile, int width, int height, bool fullscreen = false);
	void BeginLoop();
	
	void Destroy();
	
	friend class EventResponser;
	
private:
	
	inline void UpdateEntitiesOverlapp();
	inline void UpdateEntities(const float deltaTime);
	
private:
	
	ClassFactory<Entity> classFactory;
	
	World *world;
	Window *window;
	EventResponser *event;
	SoundEngine *soundEngine;
	ResourceManager *resourceManager;
	
	std::map<std::string, std::shared_ptr<Model> > models;
	std::map<std::string, std::shared_ptr<Entity> > entities;
	std::map<std::string, std::shared_ptr<Trigger> > triggerEntities;
	
	std::queue<std::string> entitiesQueuedToDestroy;
	
	TimeCounter guiDrawTime;
	TimeCounter sceneDrawTime;
	TimeCounter physicsSimulationTime;
	TimeCounter entityUpdateTime;
	
	std::shared_ptr<Entity> cameraParent;
	
	bool pausePhysics;
};

#endif

