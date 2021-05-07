
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENTITY_FACTORY_CPP
#define ENTITY_FACTORY_CPP

#include "EntityFactory.h"

#include <engine/Singleton.h>
#include <engine/Engine.h>

#include <util/Math.hpp>

EntityFactory::EntityFactory() :
	classFactory("__Constructor_%_Function") {
}

EntityFactory::~EntityFactory() {
}

bool EntityFactory::RegisterType(const std::string &className,
		const std::string &moduleName) {
	if((bool)classFactory.AddClass(className.c_str(), moduleName.c_str())) {
		return true;
	}
	MESSAGE(std::string("Cannot register type: ")+className+" from module: "+
			moduleName);
	return false;
}

bool EntityFactory::RegisterModule(const std::string &modulePath) {
	if((bool)classFactory.AddModule(modulePath.c_str())) {
		return true;
	}
	MESSAGE(std::string("Cannot register module: ")+modulePath);
	return false;
}

Entity* EntityFactory::AddEntity(const JSON& json) {
	if(!json.IsObject())
		return NULL;
	
	if(!classFactory.HasClass(json["class"].String()))
		return NULL;
	
	uint64_t id = 0;
	if(json.Object().count("id"))
		id = json["id"].Integer();
	
	JSON const* newJson;
	JSON c;
	if(id == 0) {
		id = sing::engine->GetAvailableEntityId();
		c = json;
		c["id"] = (int64_t)id;
		newJson = &c;
	} else {
		newJson = &json;
	}
	
	Entity *entity = classFactory.GetNew(json["class"].String(), *newJson);
	if(json.Object().count("mass"))
		entity->SetMass(json["mass"].Real());
	else
		entity->SetMass(0.0f);
	
	if(json.Object().count("model")) {
		entity->SetModel(
				sing::resourceManager->GetModel(
					json["model"].String()));
	}
	
	if(json.Object().count("scale")) {
		btVector3 s;
		entity->SetScale(s<<=json["scale"]);
	}
	
	return entity;
}

Entity* EntityFactory::AddEntity(const std::string className, uint64_t id,
		std::shared_ptr<CollisionShape> shape, btTransform transform,
		btScalar mass) {
	JSON json;
	json["class"] = className;
	if(shape)
		json["shape"] = shape->GetJSON();
	json["mass"] = mass;
	json["id"] = id;
	json["transform"] <<= transform;
	return AddEntity(json);
}

Entity* EntityFactory::AddEntity(const std::string className,
		std::shared_ptr<CollisionShape> shape, btTransform transform,
		btScalar mass) {
	return AddEntity(className, 0, shape, transform, mass);
}

#endif

