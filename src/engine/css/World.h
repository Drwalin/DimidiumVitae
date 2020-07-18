
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
#include <memory>

enum CollisionGroups {
	CollisionGroupStatic = 1<<6,
	CollisionGroupDynamic = 1<<7,
	CollisionGroupIgnore = 1<<8,
	CollisionGroupVisible = 1<<9,
	CollisionGroupLiquid = 1<<10,
	CollisionGroupTrigger = 1<<10
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
	CollisionDefaultMask = -1,
	CollisionDefaultMaskDynamic = CollisionGroupDynamic | CollisionGroupStatic | CollisionGroupLiquid | CollisionGroupTrigger,
	CollisionDefaultMaskStatic = CollisionGroupDynamic | CollisionGroupTrigger,
	CollisionDefaultMaskTrigger = CollisionGroupDynamic | CollisionGroupStatic,
	CollisionDefaultMaskCharacter = CollisionGroupDynamic | CollisionGroupStatic | CollisionGroupLiquid | CollisionGroupTrigger,
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
	
	void UpdateColliderForObject(std::shared_ptr<btCollisionObject> body);
	
	btVector3 GetGravity();
	
	void Tick(btScalar deltaTime, int count = 0);
	
	bool AddBody(std::shared_ptr<btCollisionObject> body, int collisionFilterGroup=btBroadphaseProxy::DefaultFilter, int collisionFilterMask=btBroadphaseProxy::AllFilter);
	bool RemoveBody(std::shared_ptr<btCollisionObject> body);
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
	
	std::set < std::shared_ptr<btCollisionObject> > object;
	
	std::shared_ptr<btCollisionObject> currentActivator;
	int activateAll;
};

#endif

