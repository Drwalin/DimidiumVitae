
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_SOURCE_CPP
#define SOUND_SOURCE_CPP

#include "../css/SoundSource.h"

#include "../lib/StdUtil.hpp"
#include "../lib/Debug.h"

#include <AL/al.h>
#include <AL/alc.h>

void SoundSource::Set2D() {
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}

void SoundSource::Set3D() {
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_FALSE);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}

void SoundSource::Play() {
	alSourcePlay(sourceID);
}

void SoundSource::Pause() {
	alSourcePause(sourceID);
}

void SoundSource::Rewind() {
	alSourcef(sourceID, AL_SEC_OFFSET, 0.0f);
}

void SoundSource::SetCurretnMoment(const float time) {
	alSourcef(sourceID, AL_SEC_OFFSET, time);
}

bool SoundSource::IsPlaying() const {
	int state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void SoundSource::Loop(const bool loop) {
	alSourcei(sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

float SoundSource::GetCurrentSecond() const {
	float value = 0.0f;
	alGetSourcef(sourceID, AL_SEC_OFFSET, &value);
	return value;
}

float SoundSource::GetDuration() const {
	if(sampler)
		return sampler->GetDuration();
	return 0.0f;
}

void SoundSource::SetLocation(const btVector3 &location) {
	alSourcefv(sourceID, AL_POSITION, location.m_floats);
}

void SoundSource::SetVelocity(const btVector3 &velocity) {
	alSourcefv(sourceID, AL_VELOCITY, velocity.m_floats);
}

void SoundSource::SetDirectionAngle(const btQuaternion &rotation, const float innerAngle, const float outerAngle) {
	SetDirectionAngle(btTransform(rotation)*btVector3(0,0,1), innerAngle, outerAngle);
}

void SoundSource::SetDirectionAngle(const btVector3 &direction, const float innerAngle, const float outerAngle) {
	alSourcefv(sourceID, AL_DIRECTION, direction.m_floats);
	alSourcef(sourceID, AL_CONE_INNER_ANGLE, innerAngle);
	alSourcef(sourceID, AL_CONE_OUTER_ANGLE, outerAngle);
}
void SoundSource::SetListenerTransform(const btTransform &transform) {
	float innerAngle=360.0f, outerAngle=360.0f;
	alGetSourcef(sourceID, AL_CONE_INNER_ANGLE, &innerAngle);
	alGetSourcef(sourceID, AL_CONE_OUTER_ANGLE, &outerAngle);
	SetListenerTransformAngle(transform, innerAngle, outerAngle);
}

void SoundSource::SetListenerTransformAngle(const btTransform &transform, const float innerAngle, const float outerAngle) {
	SetLocation(transform.getOrigin());
	SetDirectionAngle(transform.getRotation(), innerAngle, outerAngle);
}

void SoundSource::SetVolume(const float volume) {
	alSourcef(sourceID, AL_GAIN, volume);
}

void SoundSource::SetSampler(std::shared_ptr<Sound> sampler) {
	this->sampler = sampler;
	alSourcei(sourceID, AL_BUFFER, sampler->GetBuffer());
}

SoundSource::SoundSource(std::shared_ptr<Sound> sampler) {
	alGenSources(1, &(sourceID));
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(sourceID, AL_LOOPING, AL_FALSE);
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_FALSE);
	this->SetSampler(sampler);
}

SoundSource::SoundSource() {
	alGenSources(1, &(this->sourceID));
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(sourceID, AL_LOOPING, AL_FALSE);
	alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_FALSE);
}

SoundSource::~SoundSource() {
	if(sourceID)
		alDeleteSources(1, &(sourceID));
	sourceID = 0;
}

#endif
