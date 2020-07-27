
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

std::shared_ptr<Resource> ResourceManager::GetResource(const std::string &name) {
	auto it = resources.find(name);
	if(it != resources.end())
		return it->second.first;
	Resource* resource = LoadResource(name);
	if(resource==NULL)
		return std::shared_ptr<Resource>(NULL);
	std::shared_ptr<Resource> ref(resource);
	if(ref->GetName() != "")
		resources[name] = std::pair(ref, -1);
	return ref;
}

std::shared_ptr<Sound> ResourceManager::GetSound(const std::string &name) {
	return std::dynamic_pointer_cast<Sound>(GetResource(name));
}

std::shared_ptr<Model> ResourceManager::GetModel(const std::string &name) {
	return std::dynamic_pointer_cast<Model>(GetResource(name));
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string &name) {
	return std::dynamic_pointer_cast<Material>(GetResource(name));
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string &name) {
	return std::dynamic_pointer_cast<Texture>(GetResource(name));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCollisionShape(const std::string &name) {
	return std::dynamic_pointer_cast<CollisionShape>(GetResource(name));
}

std::shared_ptr<CollisionShape> ResourceManager::GetSphere(float radius) {
	JSON json;
	json.InitObject();
	json["type"] = "sphere";
	json["transform"] <<= Math::EmptyTransform;
	json["radius"] = radius;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetBox(const btVector3 &size) {
	JSON json;
	json.InitObject();
	json["type"] = "box";
	json["transform"] <<= Math::EmptyTransform;
	json["size"] <<= size;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCapsule(float radius, float height) {
	JSON json;
	json.InitObject();
	json["type"] = "capsule";
	json["transform"] <<= Math::EmptyTransform;
	json["radius"] = radius;
	json["height"] = height;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
}

std::shared_ptr<CollisionShape> ResourceManager::GetCylinder(float radius, float height) {
	JSON json;
	json.InitObject();
	json["type"] = "cylinder";
	json["transform"] <<= Math::EmptyTransform;
	json["radius"] = radius;
	json["height"] = height;
	return std::shared_ptr<CollisionShape>(new CollisionShape(json));
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
		case Resource::MODEL:
			resource = new Model(engine, name);
			break;
		case Resource::SOUND:
			resource = new Sound(name);
			break;
		case Resource::TEXTURE:
			resource = new Texture(engine, name);
			break;
		case Resource::MATERIAL:
			resource = new Material(engine, name);
			break;
		case Resource::COLLISIONSHAPE:
			resource = new CollisionShape(name, engine->GetFileSystem()->ReadJSON(name));
			break;
		}
		return resource;
	} catch(std::string e) {
		MESSAGE("\n Excepion while creating Collision shape:" + e);
	} catch(std::exception e) {
		MESSAGE("\n Excepion while creating Collision shape:" + std::string(e.what()));
	} catch(char *e) {
		MESSAGE("\n Excepion while creating Collision shape:" + std::string(e));
	} catch(...) {
		MESSAGE("\n Unknown exception while creating Collision shape");
	}
	if(resource)
		delete resource;
	return NULL;
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
	const static std::set<std::string> materialExtensions({"mtl"});
	if(collisionMeshExtensions.count(extension) > 0)
		return Resource::COLLISIONSHAPE;
	if(graphicMeshExtensions.count(extension) > 0)
		return Resource::MODEL;
	if(materialExtensions.count(extension) > 0)
		return Resource::MATERIAL;
	if(textureExtensions.count(extension) > 0)
		return Resource::TEXTURE;
	if(soundExtensions.count(extension) > 0)
		return Resource::SOUND;
	return Resource::MODEL;
}

#endif
