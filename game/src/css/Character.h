
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Trigger.h"
#include "MotionController.h"

#include <Entity.h>
#include <Camera.h>

#include <cmath>

#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

class Character : public Entity
{
protected:
	
	float defaultVelocity, height;
	std::shared_ptr<MotionController> motionController;
	
public:
	
	std::shared_ptr<MotionController> GetMotionController();
	
	virtual void NextOverlappingFrame() override;
	
	void EventRotateCameraBy( const btVector3 & rotation );
	void EventRotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth );
	
	virtual void Tick( const float deltaTime ) override;
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal ) override;
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ) override;
	
	virtual void Load( std::istream & stream ) override;
	virtual void Save( std::ostream & stream ) const override;
	virtual void Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform ) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual std::shared_ptr<Entity> New() const override;
	virtual std::string GetClassName() const override;
	
	Character();
	virtual ~Character() override;
};

#endif

