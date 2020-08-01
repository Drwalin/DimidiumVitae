
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include <Entity.h>

class StaticEntity : public Entity {
public:
	
	StaticEntity();
	virtual ~StaticEntity() override;
	
	virtual void Tick(const float deltaTime) override;
	virtual void ApplyDamage(const float damage, btVector3 point, btVector3 normal) override;
	
	virtual void Load(std::istream &stream) override;
	virtual void Save(std::ostream &stream) const override;
	virtual void Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	__ENTITY_DERIVED_HEADER_FACTORY__(StaticEntity);
	
protected:
	
	
};

#endif

