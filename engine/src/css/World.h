
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

#include <set>
#include <memory>

class World
{
private:
	
	btBroadphaseInterface * broadphase;
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btSequentialImpulseConstraintSolver * solver;
	btDiscreteDynamicsWorld * dynamicsWorld;
	
	std::set < std::shared_ptr<btCollisionObject> > object;
	
	std::shared_ptr<btCollisionObject> currentActivator;
	int activateAll;
	
	inline void UpdateObjectsActivation();
	
public:
	
	void ActivateAll();
	
	btDiscreteDynamicsWorld * GetDynamicsWorld();
	
	void UpdateColliderForObject( std::shared_ptr<btCollisionObject> body );
	
	btVector3 GetGravity();
	
	void Tick( btScalar deltaTime, int count = 0 );
	
	bool AddBody( std::shared_ptr<btCollisionObject> body, int collisionFilterGroup=btBroadphaseProxy::DefaultFilter, int collisionFilterMask=btBroadphaseProxy::AllFilter );
	bool RemoveBody( std::shared_ptr<btCollisionObject> body );
	void RemoveBodys();
	
	void Init();
	void Destroy();
	
	World();
	~World();
};

#endif

