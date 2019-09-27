
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_H
#define ENGINE_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <memory>

#include "Model.h"
#include "Entity.h"
#include "World.h"
#include "Window.h"
#include "EventResponser.h"
#include "CollisionShapeManager.h"
#include "SoundEngine.h"

#include "..\lib\dll\ClassFactory.h"

class Engine
{
private:
	
	ClassFactory<Entity> classFactory;
	
	World * world;
	Window * window;
	EventResponser * event;
	CollisionShapeManager * collisionShapeManager;
	SoundsManager * soundsManager;
	SoundEngine * soundEngine;
	
	std::map < std::string, std::shared_ptr<Model> > models;
	std::map < std::string, std::shared_ptr<Entity> > entities;
	
	std::queue < std::string > entitiesQueuedToDestroy;
	
	TimeCounter guiDrawTime;
	TimeCounter sceneDrawTime;
	TimeCounter physicsSimulationTime;
	
	std::shared_ptr<Entity> cameraParent;
	
	bool pausePhysics;
	
	
	inline void UpdateEntitiesOverlapp();
	inline void UpdateEntities( const float deltaTime );
	
	void RegisterEngineCoreEntityClasses();
	
	friend class EventResponser;
	
public:
	
	static const int SHADER_TRANSFORMATION_MATRIX_LOCATION = 0;
	
	class RayTraceData
	{
	public:
		float distance;
		btVector3 begin, end;
		btVector3 point, normal;
		std::shared_ptr<Entity> entity;
		bool operator < ( const RayTraceData & other ) const;
		RayTraceData( btCollisionWorld::AllHitsRayResultCallback & hitData, unsigned id );
		RayTraceData();
	};
	friend RayTraceData;
	
	enum RayTraceChannel
	{
		NONE = 0,
		COLLIDING = 1,
		NOT_TRANSPARENT = 2
	};
	
	int GetNumberOfEntities() const;
	std::shared_ptr<Entity> GetNewEntityOfType( const std::string & name );
	bool RegisterType( const std::string & className, const std::string & moduleName, const std::string & modulePath );
	
	void QueueEntityToDestroy( std::shared_ptr<Entity> ptr );
	void QueueEntityToDestroy( const std::string & name );
	void DeleteEntity( const std::string & name );
	
	std::shared_ptr<Entity> RayTrace( btVector3 begin, btVector3 end, int channel, btVector3 & point, btVector3 & normal, const std::vector < std::shared_ptr<Entity> > & ignoreEntities );
	
	float GetDeltaTime();
	
	World * GetWorld();
	Window * GetWindow();
	SoundEngine * GetSoundEngine();
	SoundsManager * GetSoundsManager();
	CollisionShapeManager * GetCollisionShapeManager();
	
	void PauseSimulation();
	void ResumeSimulation();
	
	std::shared_ptr<Camera> GetCamera() const;
	std::shared_ptr<Entity> GetCameraParent() const;
	
	std::string GetAvailableEntityName( const std::string & name );
	std::shared_ptr<Entity> AddEntity( std::shared_ptr<Entity> emptyEntity, const std::string & name, std::shared_ptr<btCollisionShape> shape, btTransform transform, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );
	
	void AttachCameraToEntity( const std::string & name, btVector3 location );
	bool SetCustomModelName( const std::string & name, std::shared_ptr<Model> mdl );
	
	std::shared_ptr<Model> LoadModel( const std::string & name );
	std::shared_ptr<Model> GetModel( const std::string & name );
	std::shared_ptr<Entity> GetEntity( const std::string & name );
	
	int CalculateNumberOfSimulationsPerFrame( const float deltaTime );
	void Tick( const float deltaTime );
	void ParallelToDrawTick( const float deltaTime );
	
	void Init( EventResponser * eventResponser, const std::string & windowName, const std::string & iconFile, int width, int height, bool fullscreen = false );
	void BeginLoop();
	
	void Destroy();
	
	Engine();
	~Engine();
};

#endif

