
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_H
#define ENGINE_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <memory>

#include <EventResponser.h>
#include <World.h>
#include <Window.h>
#include <Model.h>
#include <Object.h>
#include <Trigger.h>

#include <CollisionShapeManager.h>

class Engine
{
private:
	
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
	
	int GetNumberOfObjects() const;
	int GetNumberOfTriggers() const;
	
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
	
	void DrawCrosshair();
	
public:
	
	void GetDynamicObjectsWithinTheCollisionShape( std::shared_ptr<btCollisionShape> shape, btTransform transform, std::vector < std::shared_ptr<Object> > retObjects ) const;
	
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
	
	template < class T >
	std::shared_ptr<Object> AddObject( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform, bool dynamic = false, btScalar mass = 1.0f, btVector3 inertia = btVector3(0,0,0) );
	template < class T >
	std::shared_ptr<Object> AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass );
	template < class T >
	std::shared_ptr<Object> AddTrigger( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform );
	
	void AttachCameraToObject( std::string name, btVector3 location );
	bool SetCustomModelName( std::string name, std::shared_ptr<Model> mdl );
	
	std::shared_ptr<Model> LoadModel( std::string name );
	std::shared_ptr<Model> GetModel( std::string name );
	std::shared_ptr<Object> GetObject( std::string name );
	
	std::string GetAvailableObjectName( std::string name );
	
	void DeleteObject( std::string name );
	
	int CalculateNumberOfSimulationsPerFrame( const float deltaTime );
	void Tick( const float deltaTime );
	void ParallelToDrawTick( const float deltaTime );
	
	void Init( EventResponser * eventResponser, const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	void BeginLoop();
	
	void Destroy();
	
	Engine();
	~Engine();
};

#include "Engine.hpp"

#endif

