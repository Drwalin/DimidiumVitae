
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <string>

#include <JSON.hpp>
#include <engine/FileSystem.h>

class Resource {
public:
	
	enum ResourceType {
		NONE,
		SOUND,
		MODEL,
		TEXTURE,
		MATERIAL,
		COLLISIONSHAPE
	};
	
	virtual ~Resource();
	
	const std::string& GetName() const;
	virtual ResourceType GetResourceType() const =0;
	
	virtual JSON GetJSON() const =0;
	
	static const std::string& GetResourceTypeString(ResourceType type);
	static ResourceType GetResourceType(const std::string &type);
	
protected:
	
	Resource(const JSON& json);
	
protected:
	
	const std::string name;
};

#endif
