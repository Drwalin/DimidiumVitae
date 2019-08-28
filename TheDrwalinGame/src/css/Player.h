
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character
{
protected:
	
	
	
public:
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnObjectEndOverlapp( Object * other ) override;
	
	virtual void Tick( const float deltaTime ) override;
	//virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal ) override;
	//virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ) override;
	
	Player( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ );
	Player();
	~Player();
};

#endif

