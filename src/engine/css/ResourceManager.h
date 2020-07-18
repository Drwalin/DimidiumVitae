
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"

#include <string>
#include <map>
#include <vector>

class ResourceManager {
public:
	
	ResourceRef GetResource(const std::string &path);
	
	ResourceManager(float resourcePersistencyTime);
	
	void ResourceFreeingCycle(int iterations = 1);
	void FreeAllUnused();
	
	static Resource::ResourceType GetResourceTypeByPath(const std::string &path);
	
private:
	
	void Remove(const std::vector<std::string> &toRemove);
	
private:
	
	float resourcePersistencyTime;
	std::string lastIteratedPath;
	std::map<std::string, ResourceRef> resources;
};

#endif
