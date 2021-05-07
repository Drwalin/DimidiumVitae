
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>

#include <string>

#include "Sound.h"
#include "SoundSource.h"

class SoundEngine {
public:
	
	SoundEngine();
	~SoundEngine();
	
	operator bool() const;
	
	int PrintError();
	
	static void SetListenerLocation(const btVector3 &location);
	static void SetListenerVelocity(const btVector3 &velocity);
	static void SetListenerOrientation(const btVector3 &forward,
			const btVector3 &up);
	static void SetListenerOrientation(const btQuaternion &rotation);
	static void SetListenerTransform(const btTransform &transform);
	
private:
	
	void *device;
	void *context;
};

#endif

