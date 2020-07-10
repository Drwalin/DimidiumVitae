
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <map>
#include <string>
#include <atomic>

class SoundSampler
{
private:
	
	std::atomic<unsigned> * refCounter;
	unsigned bufferID;
	
	void LoadFromWAV( const std::string & wavFileName );
	void LoadFromOGG( const std::string & oggFileName );
	void LoadFromFile( const std::string & fileName );
	
	void IncrementRefCounter();
	void DecrementRefCounter();
	
public:
	
	unsigned GetBuffer() const;
	
	float GetDuration() const;
	
	unsigned GetRefCounterValue() const;
	operator bool() const;
	
	SoundSampler & operator = ( const SoundSampler & other );
	
	void Destroy();
	
	SoundSampler();
	SoundSampler( const SoundSampler & other );
	SoundSampler( const std::string & wavFile );
	~SoundSampler();
};

class SoundSource
{
private:
	
	SoundSampler sampler;
	unsigned sourceID;
	
public:
	
	void Set2D();
	void Set3D();
	
	void Play();
	void Pause();
	void Rewind();
	void SetCurretnMoment( const float time );
	bool IsPlaying() const;
	void Loop( const bool loop );
	
	float GetCurrentSecond() const;
	float GetDuration() const;
	
	void SetLocation( const btVector3 & location );
	void SetVelocity( const btVector3 & velocity );
	void SetDirectionAngle( const btQuaternion & rotation, const float innerAngle, const float outerAngle );
	void SetDirectionAngle( const btVector3 & direction, const float innerAngle, const float outerAngle );
	void SetListenerTransform( const btTransform & transform );
	void SetListenerTransformAngle( const btTransform & transform, const float innerAngle, const float outerAngle );
	
	void SetVolume( const float volume );
	void SetSampler( const SoundSampler & sampler );
	
	SoundSource( const SoundSampler & sampler );
	SoundSource();
	~SoundSource();
};

class SoundEngine
{
private:
	
	void * device;
	void * context;
	
public:
	
	operator bool() const;
	
	int PrintError();
	
	static void SetListenerLocation( const btVector3 & location );
	static void SetListenerVelocity( const btVector3 & velocity );
	static void SetListenerOrientation( const btVector3 & forward, const btVector3 & up );
	static void SetListenerOrientation( const btQuaternion & rotation );
	static void SetListenerTransform( const btTransform & transform );
	
	SoundEngine();
	~SoundEngine();
};

class SoundsManager
{
private:
	
	std::map < std::string, unsigned > nids;		// < soundName, soundID >
	std::map < unsigned, std::string > files;		// < soundID, soundfilePath >
	std::map < unsigned, std::string > idsn;		// < soundID, soundName >
	std::map < unsigned, SoundSampler > samplers;	// < soundID, soundSampler >
	std::map < unsigned, unsigned > times;			// < soundID, lastUsedMilisecond >
	
	unsigned idleMilisecondsTillRemove;
	unsigned lastRegisteredID;
	
public:
	
	SoundSampler GetSoundSampler( const std::string & soundName );
	SoundSampler GetSoundSampler( const unsigned soundID );
	
	unsigned GetSoundID( const std::string & soundName ) const;
	unsigned RegisterSound( const std::string & soundName, const std::string & soundfilePath );
	void RemoveSound( const unsigned soundID );
	void RemoveSound( const std::string & soundName );
	void ImplicitFreeMemory( const unsigned soundID );
	
	void RemoveUnusedSamplersFromMemory();
	void SetIdleMilisecondsTillRemove( unsigned miliseconds );
	
	void Clear();
	
	SoundsManager();
	~SoundsManager();
};

#endif

