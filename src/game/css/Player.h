
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character  {
public:
	
	Player();
	virtual ~Player() override;
	
	virtual void Tick(const float deltaTime) override;
	
	virtual void Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(Player);
	
protected:
	
	
};

#endif
