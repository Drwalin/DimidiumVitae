
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WORLD_CPP
#define WORLD_CPP

#include "../css/World.h"

#include "../lib/Debug.h"

void World::ActivateAll() {
	++activateAll;
	if(activateAll <= 0)
		activateAll = 1;
	else if(activateAll > 2)
		activateAll = 2;
}

btDiscreteDynamicsWorld *World::GetDynamicsWorld() {
	return dynamicsWorld;
}

inline void World::UpdateObjectsActivation() {
	if(activateAll > 0) {
		auto it = object.find(currentActivator);
		if(it == object.end())
			it = object.begin();
		
		for(int i = 0; i < 113; ++i, ++it) {
			if(it == object.end())
				break;
			(*it)->activate(true);
		}
		
		if(it != object.end()) {
			currentActivator = *it;
		} else {
			--activateAll;
			currentActivator = NULL;
		}
	}
}

void World::Tick(btScalar deltaTime, int count) {
	if(count > 1)
		dynamicsWorld->stepSimulation(deltaTime, count);
	else
		dynamicsWorld->stepSimulation(deltaTime);
}

void World::UpdateColliderForObject(btCollisionObject* body) {
	dynamicsWorld->getCollisionWorld()->updateSingleAabb(body);
	body->activate();
}

btVector3 World::GetGravity() {
	return dynamicsWorld->getGravity();
}

void World::Init() {
	Destroy();
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
			collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -20, 0));
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->
			setInternalGhostPairCallback(new btGhostPairCallback());
	dynamicsWorld->setForceUpdateAllAabbs(false);
}

bool World::AddBody(btCollisionObject *body, int collisionFilterGroup,
		int collisionFilterMask) {
	if(body) {
		if(object.find(body) == object.end()) {
			btRigidBody *rigid = dynamic_cast<btRigidBody*>(body);
			if(rigid)
				dynamicsWorld->addRigidBody(rigid, collisionFilterGroup,
						collisionFilterMask);
			else
				dynamicsWorld->addCollisionObject(body, collisionFilterGroup,
						collisionFilterMask);
			object.insert(body);
			body->activate();
		}
		return true;
	}
	return false;
}

bool World::RemoveBody(btCollisionObject *body) {
	auto it = object.find(body);
	if(it != object.end()) {
		(*it)->activate();
		btRigidBody *rigid = dynamic_cast<btRigidBody*>(*it);
		if(rigid)
			dynamicsWorld->removeRigidBody(rigid);
		else
			dynamicsWorld->removeCollisionObject(*it);
		object.erase(it);
	}
	return false;
}

void World::RemoveBodys() {
	for(auto it = object.begin(); it != object.end(); ++it) {
		btRigidBody *rigid = dynamic_cast<btRigidBody*>(*it);
		if(rigid)
			dynamicsWorld->removeRigidBody(rigid);
		else
			dynamicsWorld->removeCollisionObject(*it);
	}
	object.clear();
}

void World::Destroy() {
	RemoveBodys();
	
	if(dynamicsWorld)
		delete dynamicsWorld;
	dynamicsWorld = NULL;
	if(solver)
		delete solver;
	solver = NULL;
	if(collisionConfiguration)
		delete collisionConfiguration;
	collisionConfiguration = NULL;
	if(dispatcher)
		delete dispatcher;
	dispatcher = NULL;
	if(broadphase)
		delete broadphase;
	broadphase = NULL;
}

World::World() {
	activateAll = 0;
	broadphase = NULL;
	collisionConfiguration = NULL;
	dispatcher = NULL;
	solver = NULL;
	dynamicsWorld = NULL;
}

World::~World() {
	Destroy();
}

#endif
