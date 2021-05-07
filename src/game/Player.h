
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character  {
public:
	
	Player(const JSON& json);
	virtual ~Player() override;
	
	virtual void Tick(float deltaTime) override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(Player)
	
protected:
	
	
};

#endif
