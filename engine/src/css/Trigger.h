
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TRIGGER_H
#define TRIGGER_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <string>
#include <set>
#include <memory>

#include <Entity.h>

#include <Debug.h>

class Engine;

class Trigger : public Entity
{
protected:
	
	virtual void ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject);
	
public:
	
	virtual void NextOverlappingFrame();
	
	virtual void Tick( const float deltaTime ) override;
	
	virtual void Load( std::istream & stream ) override;
	virtual void Save( std::ostream & stream ) const override;
	virtual void Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform ) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual std::shared_ptr<Entity> New() const override;
	virtual std::string GetClassName() const override;
	
	Trigger();
	virtual ~Trigger() override;
};

#endif

