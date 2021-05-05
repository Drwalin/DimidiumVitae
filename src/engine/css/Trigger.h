
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_H
#define TRIGGER_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <string>
#include <set>
#include <memory>

#include <Entity.h>

#include <Debug.h>

class Engine;

class Trigger : public Entity {
public:
	
	Trigger(const JSON& json);
	virtual ~Trigger() override;
	
	virtual void NextOverlappingFrame();
	
	virtual void Tick(float deltaTime) override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(Trigger)
	
protected:
	
	virtual void ProcessOverlappingEntity(Entity* entity,
			btCollisionObject* collisionObject);
};

#endif

