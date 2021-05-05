
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_OBJECT_MANAGER_CPP
#define COLLISION_OBJECT_MANAGER_CPP

#include "../css/CollisionObjectManager.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

btCollisionObject* CollisionObjectManager::CreateCollisionObject(
		std::shared_ptr<CollisionShape> shape, btTransform transform) {
	if(shape == NULL)
		return NULL;
	
	btCollisionObject* object(new btCollisionObject());
	object->setCollisionShape(shape->GetNewBtCollisionShape());
	object->setWorldTransform(transform);
	return object;
}

btCollisionObject* CollisionObjectManager::CreateRigidBody(
		std::shared_ptr<CollisionShape> shape, btTransform transform,
		float mass, btVector3 inertia) {
	if(shape == NULL)
		return NULL;
	btCollisionShape *btShape = shape->GetNewBtCollisionShape();
	if(mass > 0.0f)
		btShape->calculateLocalInertia(mass, inertia);
	
	btDefaultMotionState *motionState = new btDefaultMotionState(transform);
	return new btRigidBody(mass <= 0.0f ? 0.0f : mass, motionState, btShape, inertia);
}

btCollisionObject* CollisionObjectManager::CreatePairCachingGhostObject() {
	return new btPairCachingGhostObject();
}

btCollisionObject* CollisionObjectManager::CreateGhostObject(
		std::shared_ptr<CollisionShape> shape, btTransform transform) {
	btGhostObject *ghost(new btGhostObject());
	ghost->setCollisionShape(shape->GetNewBtCollisionShape());
	ghost->setWorldTransform(transform);
	return ghost;
}

#endif
