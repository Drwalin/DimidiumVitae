
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <Entity.h>

#include <SoundEngine.h>

class DynamicEntity : public Entity {
public:
	
	DynamicEntity(JSON json);
	virtual ~DynamicEntity() override;

	virtual void Tick(const float deltaTime) override;
	virtual void ApplyDamage(const float damage, btVector3 point, btVector3 normal) override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(DynamicEntity);
	
protected:
	
	SoundSource *hitSoundSource;
	btVector3 previousLinearVelocity;
	btVector3 previousAngularVelocity;
};

#endif

