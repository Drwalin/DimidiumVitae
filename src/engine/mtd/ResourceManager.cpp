
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_MANAGER_CPP
#define RESOURCE_MANAGER_CPP

#include "../css/ResourceManager.h"
#include "../css/Model.h"

#include "../lib/StdUtil.hpp"

#include <set>

#include <ctime>

std::shared_ptr<Resource> ResourceManager::GetResource(const std::string &name) {
	auto it = resources.find(name);
	if(it != resources.end())
		return it->second.first;
	Resource* resource = LoadResource(name);
	if(resource==NULL)
		return std::shared_ptr<Resource>(NULL);
	std::shared_ptr<Resource> ref(resource);
	resources[name] = std::pair(ref, -1);
	return ref;
}

ResourceManager::ResourceManager(class Engine *engine, float resourcePersistencyTime) :
	resourcePersistencyTime(resourcePersistencyTime), lastIteratedName(""), engine(engine) {
}

void ResourceManager::ResourceFreeingCycle(int iterations) {
	std::vector<std::string> toRemove;
	auto it = resources.lower_bound(lastIteratedName);
	for(int i=0; i<iterations; ++i, ++it) {
		if(it == resources.end()) {
			it = resources.begin();
		}
		if(it != resources.end()) {
			if(it->second.first.unique()) {
				if(it->second.second == -1) {
					it->second.second = clock() + (int)(resourcePersistencyTime*((float)CLOCKS_PER_SEC));
				} else {
					if(it->second.second < clock()) {
						toRemove.emplace_back(it->first);
					}
				}
			} else {
				it->second.second = -1;
			}
		} else {
			break;
		}
	}
	if(it != resources.end())
		lastIteratedName = it->first;
	else
		lastIteratedName = "";
	Remove(toRemove);
}

void ResourceManager::FreeAllUnused() {
	std::vector<std::string> toRemove;
	for(auto resource : resources) {
		if(resource.second.first.unique()) {
			toRemove.emplace_back(resource.first);
		}
	}
	Remove(toRemove);
}

Resource* ResourceManager::LoadResource(const std::string &name) {
	Resource *resource = NULL;
	try {
		switch(GetResourceTypeByPath(name)) {
		case Resource::COLLISIONSHAPE:
			break;
		case Resource::MODEL:
			resource = new Model(engine, name);
			break;
		case Resource::TEXTURE:
			break;
		case Resource::SOUND:
			break;
		}
	} catch(...) {
		delete resource;
		return NULL;
	}
	return resource;
}

void ResourceManager::Remove(const std::vector<std::string> &toRemove) {
	for(const std::string &path : toRemove) {
		resources.erase(path);
	}
}

Resource::ResourceType ResourceManager::GetResourceTypeByPath(const std::string &name) {
	std::string extension = GetExtension(name);
	const static std::set<std::string> textureExtensions({"png", "jpg", "tga", "bmp"});
	const static std::set<std::string> graphicMeshExtensions({"x", "dae", "obj"});
	const static std::set<std::string> collisionMeshExtensions({"shape"});
	const static std::set<std::string> soundExtensions({"wav", "ogg"});
	if(collisionMeshExtensions.count(extension) > 0)
		return Resource::COLLISIONSHAPE;
	if(graphicMeshExtensions.count(extension) > 0)
		return Resource::MODEL;
	if(textureExtensions.count(extension) > 0)
		return Resource::TEXTURE;
	if(soundExtensions.count(extension) > 0)
		return Resource::SOUND;
	return Resource::MODEL;
}

#endif
