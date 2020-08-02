
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"
#include "Sound.h"
#include "Model.h"
#include "Material.h"
#include "CollisionShape.h"
#include "Texture.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class ResourceManager {
public:
	
	ResourceManager(float resourcePersistencyTime);
	~ResourceManager();
	
	std::shared_ptr<Resource> GetResource(const std::string &name);
	std::shared_ptr<Sound> GetSound(const std::string &name);
	std::shared_ptr<Model> GetModel(const std::string &name);
	std::shared_ptr<Texture> GetTexture(const std::string &name);
	std::shared_ptr<Material> GetMaterial(const std::string &name);
	
	std::shared_ptr<CollisionShape> GetCollisionShape(const std::string &name);
	std::shared_ptr<CollisionShape> GetSphere(float radius);
	std::shared_ptr<CollisionShape> GetBox(const btVector3 &size);
	std::shared_ptr<CollisionShape> GetCapsule(float radius, float height);
	std::shared_ptr<CollisionShape> GetCylinder(float radius, float height);
	
	std::shared_ptr<CollisionShape> GetCollisionShape(JSON json);
	
	void ResourceFreeingCycle(int iterations = 16);
	void FreeAllUnused();
	
	static Resource::ResourceType GetResourceTypeByPath(const std::string &name);
	
private:
	
	Resource* LoadResource(const std::string &name);
	void Remove(const std::vector<std::string> &toRemove);
	
private:
	
	float resourcePersistencyTime;
	std::string lastIteratedName;
	std::map<std::string, std::pair<std::shared_ptr<Resource>, float>> resources;
};

#endif
