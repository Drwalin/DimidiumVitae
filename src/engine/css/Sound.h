
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_H
#define SOUND_H

#include <string>

#include "Resource.h"

class Sound : public Resource {
public:

	Sound() = delete;
	Sound(const Sound &other) = delete;
	Sound(const std::string &name);
	~Sound();
	Sound &operator = (const Sound &other) = delete;
	
	unsigned GetBuffer() const;
	float GetDuration() const;
	
	operator bool() const;
	
	virtual Resource::ResourceType GetResourceType() const override;
	
private:
	
	void LoadFromWAV(const std::string &wavFileName);
	void LoadFromOGG(const std::string &oggFileName);
	void LoadFromFile(const std::string &fileName);
	
	void Destroy();
	
private:
	
	unsigned bufferID;
};

#endif
