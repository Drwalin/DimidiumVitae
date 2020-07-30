
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_TRIGGER_H
#define MOTION_CONTROLLER_TRIGGER_H

#include "Trigger.h"

#include <cmath>

struct SimulationContactResultCallback : public btCollisionWorld::ContactResultCallback {
	class MotionControllerTrigger *trigger;
	float mid;
	SimulationContactResultCallback(MotionControllerTrigger *trigger, float mid);

	btScalar addSingleResult(btManifoldPoint& manifoldPoint,
		const btCollisionObjectWrapper* colObj0Wrap,
		int partId0,
		int index0,
		const btCollisionObjectWrapper* colObj1Wrap,
		int partId1,
		int index1);
};

class MotionControllerTrigger : public Trigger {
public:
	
	MotionControllerTrigger();
	virtual ~MotionControllerTrigger() override;
	
	bool IsTopCollision() const;
	bool IsSideCollision() const;
	bool IsBottomCollision() const;
	
	virtual void NextOverlappingFrame() override;
	
	void Init(Entity *character, Entity *otherTrigger, float stepHeight);
	
	virtual void Tick(const float deltaTime) override;
	
	virtual void Load(std::istream &stream) override;
	virtual void Save(std::ostream &stream) const override;
	virtual void Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual Entity* New() const override;
	virtual std::string GetClassName() const override;
	
	friend struct SimulationContactResultCallback;
	
protected:
	
	virtual void ProcessOverlappingEntity(Entity* entity, btCollisionObject* collisionObject) override;
	
protected:
	
	Entity *character, *otherTrigger;
	bool topCollision;
	bool sideCollision;
	bool bottomCollision;
	float stepHeight;
	float bottom, top;
};

#endif
