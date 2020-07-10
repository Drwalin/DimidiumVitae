
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_TRIGGER_H
#define MOTION_CONTROLLER_TRIGGER_H

#include "Trigger.h"

#include <cmath>

class MotionControllerTrigger : public Trigger
{
protected:
	
	std::shared_ptr<Entity> character, otherTrigger;
	bool topCollision;
	bool sideCollision;
	bool bottomCollision;
	float stepHeight;
	float bottom, top;
	
	friend struct SimulationContactResultCallback;
	
	virtual void ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject) override;
	
public:
	
	bool IsTopCollision() const;
	bool IsSideCollision() const;
	bool IsBottomCollision() const;
	
	virtual void NextOverlappingFrame() override;
	
	void Init( std::shared_ptr<Entity> character, std::shared_ptr<Entity> otherTrigger, float stepHeight );
	
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
	
	MotionControllerTrigger();
	virtual ~MotionControllerTrigger() override;
};

#endif

