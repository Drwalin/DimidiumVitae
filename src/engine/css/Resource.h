
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <string>

class Resource {
public:
	
	enum ResourceType {
		SOUND,
		MODEL,
		TEXTURE,
		COLLISIONSHAPE
	};
	
	~Resource();
	
	const std::string& GetName() const;
	virtual ResourceType GetResourceType() const =0;
	
protected:
	
	Resource(const std::string &name);
	
private:
	
	const std::string name;
};

#endif
