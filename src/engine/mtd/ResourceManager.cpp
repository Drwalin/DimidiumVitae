
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_MANAGER_CPP
#define RESOURCE_MANAGER_CPP

#include <ResourceManager.h>
#include <Engine.h>

#include <StdUtil.hpp>
#include <Math.hpp>

#include <set>

#include <ctime>

std::shared_ptr<Resource> ResourceManager::GetResource(const JSON& json) {
	std::string name;
	if(json.Object().count("name")) {
		name = json["name"].GetString();
	} else {
		return std::shared_ptr<Resource>(LoadResource(json));
	}
	auto it = resources.find(name);
	if(it != resources.end())
		return it->second.first;
	Resource* resource = LoadResource(json);
	if(resource==NULL)
		return std::shared_ptr<Resource>(NULL);
	std::shared_ptr<Resource> ref(resource);
	if(ref->GetName() != "")
		resources[name] = std::pair(ref, -1);
	return ref;
}

std::shared_ptr<Resource> ResourceManager::GetResource(
		const std::string &name) {
	JSON json;
	json.InitObject();
	json["name"] = name;
	return GetResource(json);
}

std::shared_ptr<Resource> ResourceManager::GetResource(const char* name) {
	return GetResource((std::string)name);
}

std::shared_ptr<Sound> ResourceManager::GetSound(const JSON& json) {
	return std::dynamic_pointer_cast<Sound>(GetResource(json));
}

std::shared_ptr<Sound> ResourceManager::GetSound(const std::string &name) {
	return std::dynamic_pointer_cast<Sound>(GetResource(name));
}

std::shared_ptr<Sound> ResourceManager::GetSound(const char* name) {
	return GetSound((std::string)name);
}

std::shared_ptr<Model> ResourceManager::GetModel(const JSON& json) {
	return std::dynamic_pointer_cast<Model>(GetResource(json));
}

std::shared_ptr<Model> ResourceManager::GetModel(const std::string &name) {
	return std::dynamic_pointer_cast<Model>(GetResource(name));
}

std::shared_ptr<Model> ResourceManager::GetModel(const char* name) {
	return GetModel((std::string)name);
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const JSON& json) {
	return std::dynamic_pointer_cast<Material>(GetResource(json));
}

std::shared_ptr<Material> ResourceManager::GetMaterial(
		const std::string &name) {
	return std::dynamic_pointer_cast<Material>(GetResource(name));
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const char* name) {
	return GetMaterial((std::string)name);
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const JSON& json) {
	return std::dynamic_pointer_cast<Texture>(GetResource(json));
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string &name) {
	return std::dynamic_pointer_cast<Texture>(GetResource(name));
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const char* name) {
	return GetTexture((std::string)name);
}

std::shared_ptr<CollisionShape> ResourceManager::GetCollisionShape(
		const JSON& json) {
	if(json.Object().count("name"))
		return GetCollisionShape(json["name"].GetString());
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCollisionShape(
		const std::string &name) {
	return std::dynamic_pointer_cast<CollisionShape>(GetResource(name));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCollisionShape(
		const char* name) {
	return GetCollisionShape((std::string)name);
}

std::shared_ptr<CollisionShape> ResourceManager::GetSphere(float radius) {
	JSON json;
	json.InitObject();
	json["primitives"].InitArray();
	json["primitives"].Array().resize(1);
	json["primitives"][0].InitObject();
	json["primitives"][0]["type"] = "sphere";
	json["primitives"][0]["transform"] <<= Math::EmptyTransform;
	json["primitives"][0]["radius"] = radius;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetBox(const btVector3 &size) {
	JSON json;
	json.InitObject();
	json["primitives"].InitArray();
	json["primitives"].Array().resize(1);
	json["primitives"][0].InitObject();
	json["primitives"][0]["type"] = "box";
	json["primitives"][0]["transform"] <<= Math::EmptyTransform;
	json["primitives"][0]["size"] <<= size;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCapsule(float radius,
		float height) {
	JSON json;
	json.InitObject();
	json["primitives"].InitArray();
	json["primitives"].Array().resize(1);
	json["primitives"][0].InitObject();
	json["primitives"][0]["type"] = "capsule";
	json["primitives"][0]["transform"] <<= Math::EmptyTransform;
	json["primitives"][0]["radius"] = radius;
	json["primitives"][0]["height"] = height;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCylinder(float radius,
		float height) {
	JSON json;
	json.InitObject();
	json["primitives"].InitArray();
	json["primitives"].Array().resize(1);
	json["primitives"][0].InitObject();
	json["primitives"][0]["type"] = "cylinder";
	json["primitives"][0]["transform"] <<= Math::EmptyTransform;
	json["primitives"][0]["radius"] = radius;
	json["primitives"][0]["height"] = height;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
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
					it->second.second = clock() +
						(int)(resourcePersistencyTime*((float)CLOCKS_PER_SEC));
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
	for(auto  &resource : resources) {
		if(resource.second.first.unique()) {
			toRemove.emplace_back(resource.first);
		}
	}
	Remove(toRemove);
}

Resource* ResourceManager::LoadResource(const JSON& json) {
	Resource *resource = NULL;
	try {
		Resource::ResourceType resourceType = Resource::NONE;
		if(json.Object().count("class")) {
			resourceType = Resource::GetResourceType(json["class"].String());
		} else if(json.Object().count("name")) {
			resourceType = GetResourceTypeByPath(json["name"].String());
		}
		switch(resourceType) {
		case Resource::MODEL:
			resource = new Model(json);
			break;
		case Resource::SOUND:
			resource = new Sound(json);
			break;
		case Resource::TEXTURE:
			resource = new Texture(json);
			break;
		case Resource::MATERIAL:
			resource = new Material(json);
			break;
		case Resource::COLLISIONSHAPE:
			resource = new CollisionShape(json);
			break;
		default:
			break;
		}
		return resource;
	} catch(std::string e) {
		MESSAGE("\n Excepion while loading Resource: " + e + " for json: " +
				json.Write());
	} catch(const std::exception& e) {
		MESSAGE("\n Excepion while loading Resource: " + std::string(e.what()) +
				" for json: " + json.Write());
	} catch(char *e) {
		MESSAGE("\n Excepion while loading Resource: " + std::string(e) +
				" for json: " + json.Write());
	} catch(...) {
		MESSAGE("\n Unknown exception while loading Resource for json: " +
				json.Write());
	}
	if(resource)
		delete resource;
	
	return NULL;
}

void ResourceManager::Remove(const std::vector<std::string> &toRemove) {
	for(const std::string &path : toRemove) {
		resources[path].first.reset();
		resources.erase(path);
	}
}

Resource::ResourceType ResourceManager::GetResourceTypeByPath(
		const std::string &name) {
	std::string extension = GetExtension(name);
	const static std::set<std::string> textureExtensions(
			{"png", "jpg", "tga", "bmp", "jpeg", "bmpx"});
	const static std::set<std::string> graphicMeshExtensions(
			{"x", "dae", "obj"});
	const static std::set<std::string> collisionMeshExtensions(
			{"shape"});
	const static std::set<std::string> soundExtensions(
			{"wav", "ogg"});
	const static std::set<std::string> materialExtensions(
			{"mtl"});
	if(collisionMeshExtensions.count(extension) > 0) {
		return Resource::COLLISIONSHAPE;
	} else if(graphicMeshExtensions.count(extension) > 0) {
		return Resource::MODEL;
	} else if(materialExtensions.count(extension) > 0) {
		return Resource::MATERIAL;
	} else if(textureExtensions.count(extension) > 0) {
		return Resource::TEXTURE;
	} else if(soundExtensions.count(extension) > 0) {
		return Resource::SOUND;
	}
	return Resource::MODEL;
}

ResourceManager::ResourceManager(float resourcePersistencyTime) :
	resourcePersistencyTime(resourcePersistencyTime), lastIteratedName("") {
}

ResourceManager::~ResourceManager() {
}

#endif
