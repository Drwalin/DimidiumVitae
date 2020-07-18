
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <Entity.h>

#include <SoundEngine.h>

class DynamicEntity : public Entity {
public:
	
	DynamicEntity();
	virtual ~DynamicEntity() override;

	virtual void Tick(const float deltaTime) override;
	virtual void ApplyDamage(const float damage, btVector3 point, btVector3 normal) override;
	
	virtual void Load(std::istream &stream) override;
	virtual void Save(std::ostream &stream) const override;
	virtual void Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual std::shared_ptr<Entity> New() const override;
	virtual std::string GetClassName() const override;
	
protected:
	
	SoundSource *hitSoundSource;
	btVector3 previousLinearVelocity;
	btVector3 previousAngularVelocity;
};

#endif

