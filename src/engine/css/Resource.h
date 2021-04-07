
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <string>

#include "JSON.h"
#include "FileSystem.h"

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
	
	virtual void GetJSON(JSON json) const =0;
	
	static const std::string& GetResourceTypeString(ResourceType type);
	static ResourceType GetResourceType(const std::string &type);protected:
	
	Resource(JSON json);
	
protected:
	
	const std::string name;
};

#endif
