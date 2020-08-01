
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_H
#define CHARACTER_H

#include "MotionController.h"

#include <Entity.h>
#include <Camera.h>

#include <cmath>

#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class Character : public Entity  {
public:
	
	Character();
	virtual ~Character() override;
	
	std::shared_ptr<MotionController> GetMotionController();
	
	virtual void Tick(const float deltaTime) override;
	virtual void ApplyDamage(const float damage, btVector3 point, btVector3 normal) override;
	
	virtual void Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(Character);
	
protected:
	
	float height;
	std::shared_ptr<MotionController> motionController;
};

#endif
