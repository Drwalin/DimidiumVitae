
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_ENGINE_CPP
#define SOUND_ENGINE_CPP

#include "SoundEngine.h"

#include <util/StdUtil.hpp>
#include <util/Debug.h>
#include <util/Wav.h>
#include <util/Ogg.h>

#include <AL/al.h>
#include <AL/alc.h>

SoundEngine::operator bool() const {
	return (device) && (context);
}

int SoundEngine::PrintError() {
	int error = alcGetError((ALCdevice*)(device));
	fprintf(stderr, "\n OpenAL error: %i", error);
	return error;
}

void SoundEngine::SetListenerLocation(const btVector3 &location) {
	alListenerfv(AL_POSITION, location.m_floats);
}

void SoundEngine::SetListenerVelocity(const btVector3 &velocity) {
	alListenerfv(AL_VELOCITY, velocity.m_floats);
}

void SoundEngine::SetListenerOrientation(const btVector3 &forward,
		const btVector3 &up) {
	float ar[6] = { forward.x(), forward.y(), forward.z(), up.x(), up.y(),
			up.z() };
	alListenerfv(AL_ORIENTATION, ar);
}

void SoundEngine::SetListenerOrientation(const btQuaternion &rotation) {
	btTransform transform(rotation);
	SoundEngine::SetListenerOrientation(transform*btVector3(0,0,1),
			transform*btVector3(0,1,0));
}

void SoundEngine::SetListenerTransform(const btTransform &transform) {
	SoundEngine::SetListenerLocation(transform.getOrigin());
	SoundEngine::SetListenerOrientation(transform.getRotation());
}

SoundEngine::SoundEngine() {
	device = NULL;
	context = NULL;
    device = (ALCdevice*)alcOpenDevice(NULL);
    if(device == NULL) {
    	MESSAGE("OpenAL error: " + std::to_string(PrintError()) +
				" - You need to install OpenAL11 core sdk to enable sounds");
        return;
    }
    context = (ALCcontext*)alcCreateContext((ALCdevice*)(device), NULL);
    if(context == NULL) {
    	MESSAGE("OpenAL error: " + std::to_string(PrintError()) +
				" - You need to install OpenAL11 core sdk to enable sounds");
    	return;
    }
    alcMakeContextCurrent((ALCcontext*)(context));
}

SoundEngine::~SoundEngine() {
	if(device) {
		if(context)
	    	alcDestroyContext((ALCcontext*)(context));
	    alcCloseDevice((ALCdevice*)(device));
	}
	context = NULL;
	device = NULL;
}

#endif
