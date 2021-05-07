
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <ClassFactory.h>

#include "Entity.h"

#include <JSON.hpp>

class EntityFactory {
public:
	
	EntityFactory();
	~EntityFactory();
	
	Entity* AddEntity(const JSON& json);
	Entity* AddEntity(const std::string className, uint64_t id,
			std::shared_ptr<CollisionShape> shape, btTransform transform,
			btScalar mass = 1.0f);
	Entity* AddEntity(const std::string className,
			std::shared_ptr<CollisionShape> shape, btTransform transform,
			btScalar mass = 1.0f);
	Entity* GetEntity(uint64_t entityId);
	
	bool RegisterType(const std::string &className,
			const std::string &moduleName);
	bool RegisterModule(const std::string &moduleName);
	
private:
	
	ClassFactory<Entity, const JSON&> classFactory;
	
};

#endif

