
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WORLD_H
#define WORLD_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <cstdio>

#include <map>
#include <string>

#include <memory>

class World
{
private:
	
	btBroadphaseInterface * broadphase;
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btSequentialImpulseConstraintSolver * solver;
	btDiscreteDynamicsWorld * dynamicsWorld;
	
	std::map < std::string, std::shared_ptr<btRigidBody> > object;
	std::map < std::string, std::shared_ptr<btPairCachingGhostObject> > trigger;
	
	std::string currentActivator;
	int activateAll;
	
	inline void UpdateObjectsActivation();
	
public:
	
	void ActivateAll();
	
	btDiscreteDynamicsWorld * GetDynamicsWorld();
	
	void UpdateColliderForObject( std::shared_ptr<btRigidBody> body );
	void UpdateColliderForTrigger( std::shared_ptr<btPairCachingGhostObject> body );
	
	btVector3 GetGravity();
	
	void Tick( btScalar deltaTime, int count = 0 );
	
	bool AddBody( std::string name, std::shared_ptr<btRigidBody> body );
	bool AddTrigger( std::string name, std::shared_ptr<btPairCachingGhostObject> body );
	
	void RemoveTrigger( std::string name );
	void RemoveBody( std::string name );
	void RemoveBodys();
	
	void Init();
	void Destroy();
	
	World();
	~World();
};

#endif

