
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>

#include <string>
#include <memory>

#include "Sound.h"

class SoundSource {
public:
	
	SoundSource(std::shared_ptr<Sound> sampler);
	SoundSource();
	~SoundSource();
	
	void Set2D();
	void Set3D();
	
	void Play();
	void Pause();
	void Rewind();
	void SetCurretnMoment(const float time);
	bool IsPlaying() const;
	void Loop(const bool loop);
	
	float GetCurrentSecond() const;
	float GetDuration() const;
	
	void SetLocation(const btVector3 &location);
	void SetVelocity(const btVector3 &velocity);
	void SetDirectionAngle(const btQuaternion &rotation, const float innerAngle, const float outerAngle);
	void SetDirectionAngle(const btVector3 &direction, const float innerAngle, const float outerAngle);
	void SetListenerTransform(const btTransform &transform);
	void SetListenerTransformAngle(const btTransform &transform, const float innerAngle, const float outerAngle);
	
	void SetVolume(const float volume);
	void SetSampler(std::shared_ptr<Sound> sampler);
	
private:
	
	std::shared_ptr<Sound> sampler;
	unsigned sourceID;
};

#endif
