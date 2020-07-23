
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_ENGINE_CPP
#define SOUND_ENGINE_CPP

#include "../css/SoundEngine.h"

#include "../lib/StdUtil.hpp"
#include "../lib/Debug.h"
#include "../lib/Wav.h"
#include "../lib/Ogg.h"

#include <AL/al.h>
#include <AL/alc.h>

SoundEngine::operator bool() const {
	return (this->device) && (this->context);
}

int SoundEngine::PrintError() {
	int error = alcGetError((ALCdevice*)(this->device));
	fprintf(stderr, "\n OpenAL error: %i", error);
	return error;
}

void SoundEngine::SetListenerLocation(const btVector3 &location) {
	alListenerfv(AL_POSITION, location.m_floats);
}

void SoundEngine::SetListenerVelocity(const btVector3 &velocity) {
	alListenerfv(AL_VELOCITY, velocity.m_floats);
}

void SoundEngine::SetListenerOrientation(const btVector3 &forward, const btVector3 &up) {
	float ar[6] = { forward.x(), forward.y(), forward.z(), up.x(), up.y(), up.z() };
	alListenerfv(AL_ORIENTATION, ar);
}

void SoundEngine::SetListenerOrientation(const btQuaternion &rotation) {
	btTransform transform(rotation);
	SoundEngine::SetListenerOrientation(transform*btVector3(0,0,1), transform*btVector3(0,1,0));
}

void SoundEngine::SetListenerTransform(const btTransform &transform) {
	SoundEngine::SetListenerLocation(transform.getOrigin());
	SoundEngine::SetListenerOrientation(transform.getRotation());
}

SoundEngine::SoundEngine() {
	this->device = NULL;
	this->context = NULL;
    this->device = (ALCdevice*)alcOpenDevice(NULL);
    if(this->device == NULL) {
    	MESSAGE("OpenAL error: " + std::to_string(this->PrintError()) + " - You need to install OpenAL11 core sdk to enable sounds");
        return;
    }
    this->context = (ALCcontext*)alcCreateContext((ALCdevice*)(this->device), NULL);
    if(this->context == NULL) {
    	MESSAGE("OpenAL error: " + std::to_string(this->PrintError()) + " - You need to install OpenAL11 core sdk to enable sounds");
    	return;
    }
    alcMakeContextCurrent((ALCcontext*)(this->context));
}

SoundEngine::~SoundEngine() {
	if(this->device) {
		if(this->context)
	    	alcDestroyContext((ALCcontext*)(this->context));
	    alcCloseDevice((ALCdevice*)(this->device));
	}
	this->context = NULL;
	this->device = NULL;
}

#endif
