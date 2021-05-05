
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include <Entity.h>

class StaticEntity : public Entity {
public:
	
	StaticEntity(const JSON& json);
	virtual ~StaticEntity() override;
	
	virtual void Tick(float deltaTime) override;
	virtual void ApplyDamage(float damage, btVector3 point, btVector3 normal)
		override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(StaticEntity)
	
protected:
	
	
};

#endif

