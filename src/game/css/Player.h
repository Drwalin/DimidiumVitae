
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
	
	virtual void Load(std::istream &stream) override;
	virtual void Save(std::ostream &stream) const override;
	virtual void Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual std::shared_ptr<Entity> New() const override;
	virtual std::string GetClassName() const override;
	
protected:
	
	
};

#endif
