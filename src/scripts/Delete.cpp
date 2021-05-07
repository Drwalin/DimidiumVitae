
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <engine/Script.h>
#include <engine/Engine.h>
#include <engine/Singleton.h>

#include <JSON.hpp>

void Delete(const JSON& args) {
	if(args[1].String() == "entity") {
			sing::engine->DeleteEntity(args[2].Integer());
	} else if(args[1].String() == "entities") {
		std::string className = args[2].String();
		std::map<uint64_t, Entity*> entities = sing::engine->GetEntities();
		size_t deletedCount = 0;
		for(auto it : entities) {
			if(it.second->GetClassName() == className) {
				deletedCount++;
				sing::engine->DeleteEntity(it.first);
			}
		}
		MESSAGE(std::string("Deleted ") + std::to_string(deletedCount) +
				" entities");
	} else {
		MESSAGE(std::string("Invalid delete argument (1): ") + args[1].Write());
	}
}

