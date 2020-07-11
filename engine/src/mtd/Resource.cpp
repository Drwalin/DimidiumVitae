
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_CPP
#define RESOURCE_CPP

#include "../css/Resource.h"

#include <ctime>

Resource::~Resource() {
}

int Resource::IncrementCounter() {
	return references.fetch_add(1)+1;
}

int Resource::DecrementCounter() {
	return references.fetch_add(-1)-1;
}

int Resource::GetCounter() const {
	return references.load();
}

Resource::Resource(ResourceType type) :
	type(type), references(0), lastTimeUsedStamp(0) {
}

float Resource::LastUsedSecondsAgo() const {
	if(GetCounter() <= 1) {
		return ((float)(clock() - lastTimeUsedStamp))/((float)CLOCKS_PER_SEC);
	}
	return 0.0f;
}

void Resource::UpdateLastUsage() {
	lastTimeUsedStamp = clock();
}



ResourceRef::ResourceRef() {
	this->resource = NULL;
}

ResourceRef::ResourceRef(Resource *resource) {
	this->resource = resource;
	if(resource) {
		resource->IncrementCounter();
	}
}

ResourceRef::ResourceRef(ResourceRef &other) {
	resource = other.resource;
	if(resource) {
		resource->IncrementCounter();
	}
}

ResourceRef::ResourceRef(ResourceRef &&other) {
	resource = other.resource;
}

ResourceRef::ResourceRef(const ResourceRef &other) {
	resource = (Resource*)other.resource;
	if(resource) {
		resource->IncrementCounter();
	}
}

ResourceRef &ResourceRef::operator=(ResourceRef &other) {
	resource = other.resource;
	if(resource) {
		resource->IncrementCounter();
	}
	return *this;
}

Resource *ResourceRef::Get() {
	return resource;
}

ResourceRef::~ResourceRef() {
	if(resource) {
		if(resource->DecrementCounter() <= 1) {
			resource->UpdateLastUsage();
		}
		if(resource->GetCounter() == 0) {
			delete this;
		}
	}
}

#endif
