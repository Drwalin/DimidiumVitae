
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WORLD_H
#define WORLD_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <cstdio>

#include <set>

enum CollisionGroups {
	CollisionGroupStatic = 1<<6,
	CollisionGroupDynamic = 1<<7,
	CollisionGroupIgnore = 1<<8,
	CollisionGroupVisible = 1<<9,
	CollisionGroupLiquid = 1<<10,
	CollisionGroupTrigger = 1<<11,
	CollisionGroupSight = 1<<12
};

enum CollisionDefaultGroups {
	CollisionDefaultGroup = CollisionGroupDynamic | CollisionGroupVisible,
	CollisionDefaultGroupDynamic = CollisionGroupDynamic | CollisionGroupVisible,
	CollisionDefaultGroupStatic = CollisionGroupStatic | CollisionGroupVisible,
	CollisionDefaultGroupTrigger = CollisionGroupTrigger,
	CollisionDefaultGroupCharacter = CollisionGroupDynamic | CollisionGroupVisible,
	CollisionDefaultGroupLiquid = CollisionGroupLiquid
};

enum CollisionDefaultMasks {
	CollisionDefaultMaskAll = ~0,
	CollisionDefaultMaskDynamic = CollisionGroupDynamic | CollisionGroupStatic | CollisionGroupLiquid | CollisionGroupTrigger | CollisionGroupSight,
	CollisionDefaultMaskStatic = CollisionGroupDynamic | CollisionGroupTrigger | CollisionGroupSight,
	CollisionDefaultMaskTrigger = CollisionGroupDynamic | CollisionGroupStatic,
	CollisionDefaultMaskCharacter = CollisionGroupDynamic | CollisionGroupStatic | CollisionGroupLiquid | CollisionGroupTrigger | CollisionGroupSight,
	CollisionDefaultMaskLiquid = CollisionGroupDynamic | CollisionGroupTrigger,
	CollisionDefaultMaskDynamicNotinteractive = CollisionGroupStatic | CollisionGroupLiquid | CollisionGroupTrigger
};

class World {
public:
	
	World();
	~World();
	
	void Init();
	
	void ActivateAll();
	
	btDiscreteDynamicsWorld *GetDynamicsWorld();
	
	void UpdateColliderForObject(btCollisionObject* body);
	
	btVector3 GetGravity();
	
	void Tick(btScalar deltaTime, int count = 0);
	
	bool AddBody(btCollisionObject* body, int collisionFilterGroup=btBroadphaseProxy::DefaultFilter, int collisionFilterMask=btBroadphaseProxy::AllFilter);
	bool RemoveBody(btCollisionObject* body);
	void RemoveBodys();
	
	void Destroy();
	
private:

	inline void UpdateObjectsActivation();
	
private:
	
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;
	btDiscreteDynamicsWorld *dynamicsWorld;
	
	std::set<btCollisionObject*> object;
	
	btCollisionObject *currentActivator;
	int activateAll;
};

#endif

