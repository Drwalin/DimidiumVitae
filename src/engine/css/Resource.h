
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_H
#define RESOURCE_H

#include <atomic>

class Resource {
public:
	
	~Resource();
	
	enum ResourceType {
		SOUND,
		TEXTURE,
		GRAPHICMESH,
		COLLISIONSHAPE
	};
	
	int IncrementCounter();
	int DecrementCounter();
	int GetCounter() const;
	
	float LastUsedSecondsAgo() const;
	void UpdateLastUsage();
	
protected:
	
	Resource(ResourceType type);
	
private:
	
	const ResourceType type;
	
private:
	
	std::atomic<int> references;
	int lastTimeUsedStamp;
};

class ResourceRef {
public:
	
	ResourceRef();
	ResourceRef(Resource *resource);
	ResourceRef(ResourceRef &other);
	ResourceRef(ResourceRef &&other);
	ResourceRef(const ResourceRef &other);
	ResourceRef &operator=(ResourceRef &other);
	
	Resource *Get();
	
	~ResourceRef();
	
private:
	
	Resource *resource;
};

#endif
