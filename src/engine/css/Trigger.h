
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
	
	Trigger();
	virtual ~Trigger() override;
	
	virtual void NextOverlappingFrame();
	
	virtual void Tick(const float deltaTime) override;
	
	virtual void Spawn(size_t id, std::shared_ptr<CollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(Trigger);
	
protected:
	
	virtual void ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject);
};

#endif

