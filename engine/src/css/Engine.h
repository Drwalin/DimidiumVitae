
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
#include "Object.h"
#include "World.h"
#include "Window.h"
#include "EventResponser.h"
#include "CollisionShapeManager.h"

#include "..\lib\dll\ClassFactory.h"

class Engine
{
private:
	
	ClassFactory<Object> classFactory;
	
	World * world;
	Window * window;
	EventResponser * event;
	CollisionShapeManager * collisionShapeManager;
	
	std::map < std::string, std::shared_ptr<Model> > model;
	std::map < std::string, std::shared_ptr<Object> > object;
	
	std::queue < std::string > objectsQueuedToDestroy;
	
	TimeCounter guiDrawTime;
	TimeCounter sceneDrawTime;
	TimeCounter physicsSimulationTime;
	
	std::shared_ptr<Object> cameraParent;
	
	bool pausePhysics;
	
	
	inline void UpdateObjectOverlaps();
	inline void UpdateObjects( const float deltaTime );
	
	friend class EventResponser;
	
public:
	
	static const int SHADER_TRANSFORMATION_MATRIX_LOCATION = 0;
	
	class RayTraceData
	{
	public:
		float distance;
		btVector3 begin, end;
		btVector3 point, normal;
		std::shared_ptr<Object> object;
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
	
	
	int GetNumberOfObjects() const;
	std::shared_ptr<Object> GetNewObjectOfType( const std::string & name );
	bool RegisterType( const std::string & name, const std::string & modulePath );
	
	
	void QueueObjectToDestroy( std::shared_ptr<Object> ptr );
	void QueueObjectToDestroy( const std::string & name );
	
	std::shared_ptr<Object> RayTrace( btVector3 begin, btVector3 end, int channel, btVector3 & point, btVector3 & normal, const std::vector < std::shared_ptr<Object> > & ignoreObjects );
	
	float GetDeltaTime();
	
	World * GetWorld();
	Window * GetWindow();
	
	void PauseSimulation();
	void ResumeSimulation();
	
	std::shared_ptr<Camera> GetCamera() const;
	std::shared_ptr<Object> GetCameraParent() const;
	
	CollisionShapeManager * GetCollisionShapeManager();
	
	std::shared_ptr<Object> AddObject( std::shared_ptr<Object> emptyObject, const std::string & name, std::shared_ptr<btCollisionShape> shape, btTransform transform, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );
	
	void AttachCameraToObject( const std::string & name, btVector3 location );
	bool SetCustomModelName( const std::string & name, std::shared_ptr<Model> mdl );
	
	std::shared_ptr<Model> LoadModel( const std::string & name );
	std::shared_ptr<Model> GetModel( const std::string & name );
	std::shared_ptr<Object> GetObject( const std::string & name );
	
	std::string GetAvailableObjectName( const std::string & name );
	
	void DeleteObject( const std::string & name );
	
	int CalculateNumberOfSimulationsPerFrame( const float deltaTime );
	void Tick( const float deltaTime );
	void ParallelToDrawTick( const float deltaTime );
	
	void Init( EventResponser * eventResponser, const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	void BeginLoop();
	
	void Destroy();
	
	Engine();
	~Engine();
};

#endif

