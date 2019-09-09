
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

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

#include "Entity.h"

#include "..\lib\Debug.h"

class Engine;

class Trigger : public Entity
{
protected:
	
	
	
public:
	
	virtual bool IsTrigger() const final;
	
	virtual void SetModel( std::shared_ptr<Model> model, bool light = true ) final;
	virtual void SetMass( float mass ) final;
	virtual bool IsDynamic() const final;
	
	
	virtual void EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnEntityEndOverlapp( Entity * other ) override;
	
	virtual void Tick( const float deltaTime ) override;
	
	Trigger( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ );
	Trigger();
	~Trigger();
};

#endif

