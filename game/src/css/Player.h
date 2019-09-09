
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character
{
protected:
	
	
	
public:
	
	virtual void EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * perisstentManifold ) override;
	virtual void EventOnEntityEndOverlapp( Entity * other ) override;
	
	virtual void Tick( const float deltaTime ) override;
	//virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal ) override;
	//virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ) override;
	
	
	virtual void Load( std::istream & stream ) override;
	virtual void Save( std::ostream & stream ) const override;
	virtual void Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform ) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual std::shared_ptr<Entity> New() const override;
	virtual std::string GetClassName() const override;
	
	Player();
	virtual ~Player() override;
};

#endif

