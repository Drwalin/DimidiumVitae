
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_MANAGER_CPP
#define RESOURCE_MANAGER_CPP

#include "../css/ResourceManager.h"

#include "../lib/StdUtil.hpp"

#include <set>

ResourceRef ResourceManager::GetResource(const std::string &path) {
	auto it = resources.find(path);
	if(it != resources.end())
		return it->second;
	Resource *resource = NULL;
	switch(GetResourceTypeByPath(path)) {
	case Resource::COLLISIONSHAPE:
		break;
	case Resource::GRAPHICMESH:
		break;
	case Resource::TEXTURE:
		break;
	case Resource::SOUND:
		break;
	}
	if(resource==NULL)
		return ResourceRef(NULL);
	ResourceRef ref(resource);
	resources[path] = ref;
	return ref;
}

ResourceManager::ResourceManager(float resourcePersistencyTime) :
	resourcePersistencyTime(resourcePersistencyTime), lastIteratedPath("") {
}

void ResourceManager::ResourceFreeingCycle(int iterations) {
	std::vector<std::string> toRemove;
	auto it=resources.lower_bound(lastIteratedPath);
	for(int i=0; i<iterations; ++i, ++it) {
		if(it == resources.end()) {
			it = resources.begin();
		}
		if(it != resources.end()) {
			if(it->second.Get()->LastUsedSecondsAgo() >= resourcePersistencyTime) {
				toRemove.emplace_back(it->first);
			}
		} else {
			break;
		}
	}
	if(it != resources.end())
		lastIteratedPath = it->first;
	else
		lastIteratedPath = "";
	Remove(toRemove);
}

void ResourceManager::FreeAllUnused() {
	std::vector<std::string> toRemove;
	for(auto resource : resources) {
		if(resource.second.Get()->LastUsedSecondsAgo() > 0.001f) {
			toRemove.emplace_back(resource.first);
		}
	}
	Remove(toRemove);
}

void ResourceManager::Remove(const std::vector<std::string> &toRemove) {
	for(const std::string &path : toRemove) {
		resources.erase(path);
	}
}

Resource::ResourceType ResourceManager::GetResourceTypeByPath(const std::string &path) {
	std::string extension = GetExtension(path);
	const static std::set<std::string> textureExtensions({"png", "jpg", "tga", "bmp"});
	const static std::set<std::string> graphicMeshExtensions({"x", "dae", "obj"});
	const static std::set<std::string> collisionMeshExtensions({"shape"});
	const static std::set<std::string> soundExtensions({"wav", "ogg"});
	if(collisionMeshExtensions.count(extension) > 0)
		return Resource::COLLISIONSHAPE;
	if(graphicMeshExtensions.count(extension) > 0)
		return Resource::GRAPHICMESH;
	if(textureExtensions.count(extension) > 0)
		return Resource::TEXTURE;
	if(soundExtensions.count(extension) > 0)
		return Resource::SOUND;
	return Resource::GRAPHICMESH;
}

#endif
