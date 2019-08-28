
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_WALK_TRIGGER_H
#define CHARACTER_WALK_TRIGGER_H

#include <Trigger.h>
#include "Character.h"
#include <cmath>

class CharacterWalkTrigger : public Trigger
{
protected:
	
	std::shared_ptr<Object> parent;
	bool isAnyInside;
	
	void EventOverlapp( Object * other, btPersistentManifold * persisstentManifold );
	
public:
	
	virtual void NextOverlappingFrame() override;
	
	bool IsAnyInside() const;
	void SetParent( std::shared_ptr<Object> parent );
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold ) override;
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold ) override;
	virtual void EventOnObjectEndOverlapp( Object * other ) override;
	
	virtual void Tick( const float deltaTime ) override;
	
	CharacterWalkTrigger( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ );
	CharacterWalkTrigger();
	~CharacterWalkTrigger();
};

#endif

