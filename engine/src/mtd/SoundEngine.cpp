
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_ENGINE_CPP
#define SOUND_ENGINE_CPP

#include "..\css\SoundEngine.h"

#include "..\lib\StdUtil.hpp"
#include "..\lib\Debug.h"
#include "..\lib\Wav.h"
#include "..\lib\Ogg.h"

#include <AL\al.h>
#include <AL\alc.h>

#include <vector>

#include <ctime>

void SoundSampler::LoadFromWAV( const std::string & wavFileName )
{
	this->Destroy();
	WavHeader wavHeader;
	void * bufferData = WAVLoadFromFile( &wavHeader, wavFileName.c_str() );
	if( bufferData )
	{
		ALenum format;
		switch( wavHeader.BitsPerSample )
		{
		case 8:
			switch( wavHeader.NumChannels )
			{
			case 1:
				format = AL_FORMAT_MONO8;
				break;
			case 2:
				format = AL_FORMAT_STEREO8;
				break;
			default:
				WAVFree( bufferData );
				return;
			}
			break;
		case 16:
			switch( wavHeader.NumChannels )
			{
			case 1:
				format = AL_FORMAT_MONO16;
				break;
			case 2:
				format = AL_FORMAT_STEREO16;
				break;
			default:
				WAVFree( bufferData );
				return;
			}
			break;
		default:
			WAVFree( bufferData );
			return;
		}
		alGenBuffers( 1, &(this->bufferID) );
		alBufferData( this->bufferID, format, bufferData, wavHeader.Subchunk2Size, wavHeader.SampleRate );
		WAVFree( bufferData );
		this->IncrementRefCounter();
	}
}

void SoundSampler::LoadFromOGG( const std::string & oggFileName )
{
	std::vector<char> bufferData;
	ALenum format;
	ALsizei sampleRate;
	if( OGGLoadFromFile( oggFileName.c_str(), bufferData, format, sampleRate ) )
	{
		alGenBuffers( 1, &(this->bufferID) );
		alBufferData( this->bufferID, format, &(bufferData[0]), bufferData.size(), sampleRate );
		this->IncrementRefCounter();
	}
}

void SoundSampler::LoadFromFile( const std::string & fileName )
{
	std::string extension = GetExtension( fileName );
	if( extension == "ogg" )
		this->LoadFromOGG( fileName );
	else if( extension == "wav" )
		this->LoadFromWAV( fileName );
	else
		MESSAGE( "Unrecognized sound file extension: " + fileName + " -> " + extension );
}

unsigned SoundSampler::GetBuffer() const
{
	return this->bufferID;
}

float SoundSampler::GetDuration() const
{
	if( this->bufferID )
	{
		int sampleRate = 0, bitsPerSample = 0, channels = 0, bufferSizeBytes = 0;
		alGetBufferi( this->bufferID, AL_FREQUENCY, &sampleRate );
		alGetBufferi( this->bufferID, AL_BITS, &bitsPerSample );
		alGetBufferi( this->bufferID, AL_CHANNELS, &channels );
		alGetBufferi( this->bufferID, AL_SIZE, &bufferSizeBytes );
		if( sampleRate == 0 || channels == 0 || AL_BITS == 0 )
			return 0;
		return ((float)(bufferSizeBytes)) / ((float)(sampleRate*channels*bitsPerSample/8));
	}
	return 0;
}

unsigned SoundSampler::GetRefCounterValue() const
{
	if( this->refCounter )
		return this->refCounter->load(std::memory_order_relaxed);
	return 0;
}

SoundSampler::operator bool() const
{
	return (this->bufferID && this->GetRefCounterValue());
}

void SoundSampler::IncrementRefCounter()
{
	if( this->bufferID )
	{
		if( this->refCounter )
			++(*(this->refCounter));
		else
			this->refCounter = new std::atomic<unsigned>(1);
	}
}

void SoundSampler::DecrementRefCounter()
{
	if( this->refCounter )
	{
		if( --(*(this->refCounter)) )
		{
			this->refCounter = NULL;
			this->bufferID = 0;
			return;
		}
		delete (this->refCounter);
		this->refCounter = NULL;
	}
	if( this->bufferID )
	{
		alDeleteBuffers( 1, &(this->bufferID) );
		this->bufferID = 0;
	}
}

SoundSampler & SoundSampler::operator = ( const SoundSampler & other )
{
	this->Destroy();
	this->bufferID = other.bufferID;
	this->refCounter = other.refCounter;
	this->IncrementRefCounter();
	return *this;
}

void SoundSampler::Destroy()
{
	this->DecrementRefCounter();
}

SoundSampler::SoundSampler()
{
	this->bufferID = 0;
	this->refCounter = NULL;
}

SoundSampler::SoundSampler( const SoundSampler & other )
{
	this->bufferID = other.bufferID;
	this->refCounter = other.refCounter;
	this->IncrementRefCounter();
}

SoundSampler::SoundSampler( const std::string & wavFile )
{
	this->bufferID = 0;
	this->refCounter = NULL;
	this->LoadFromFile( wavFile );
}

SoundSampler::~SoundSampler()
{
	this->Destroy();
}



void SoundSource::Set2D()
{
	alSourcei( this->sourceID, AL_SOURCE_RELATIVE, AL_TRUE );
	alSource3f( this->sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f );
	alSource3f( this->sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
}

void SoundSource::Set3D()
{
	alSourcei( this->sourceID, AL_SOURCE_RELATIVE, AL_FALSE );
	alSource3f( this->sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f );
	alSource3f( this->sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
}

void SoundSource::Play()
{
	alSourcePlay( this->sourceID );
}

void SoundSource::Pause()
{
	alSourcePause( this->sourceID );
}

void SoundSource::Rewind()
{
	alSourcef( this->sourceID, AL_SEC_OFFSET, 0.0f );
}

void SoundSource::SetCurretnMoment( const float time )
{
	alSourcef( this->sourceID, AL_SEC_OFFSET, time );
}

bool SoundSource::IsPlaying() const
{
	int state;
	alGetSourcei( this->sourceID, AL_SOURCE_STATE, &state );
	return (state == AL_PLAYING);
}

void SoundSource::Loop( const bool loop )
{
	alSourcei( this->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE );
}

float SoundSource::GetCurrentSecond() const
{
	float value = 0.0f;
	alGetSourcef( this->sourceID, AL_SEC_OFFSET, &value );
	return value;
}

float SoundSource::GetDuration() const
{
	return this->sampler.GetDuration();
}

void SoundSource::SetLocation( const btVector3 & location )
{
	alSourcefv( this->sourceID, AL_POSITION, location.m_floats );
}

void SoundSource::SetVelocity( const btVector3 & velocity )
{
	alSourcefv( this->sourceID, AL_VELOCITY, velocity.m_floats );
}

void SoundSource::SetDirectionAngle( const btQuaternion & rotation, const float innerAngle, const float outerAngle )
{
	this->SetDirectionAngle( btTransform(rotation)*btVector3(0,0,1), innerAngle, outerAngle );
}

void SoundSource::SetDirectionAngle( const btVector3 & direction, const float innerAngle, const float outerAngle )
{
	alSourcefv( this->sourceID, AL_DIRECTION, direction.m_floats );
	alSourcef( this->sourceID, AL_CONE_INNER_ANGLE, innerAngle );
	alSourcef( this->sourceID, AL_CONE_OUTER_ANGLE, outerAngle );
}
void SoundSource::SetListenerTransform( const btTransform & transform )
{
	float innerAngle=360.0f, outerAngle=360.0f;
	alGetSourcef( this->sourceID, AL_CONE_INNER_ANGLE, &innerAngle );
	alGetSourcef( this->sourceID, AL_CONE_OUTER_ANGLE, &outerAngle );
	this->SetListenerTransformAngle( transform, innerAngle, outerAngle );
}

void SoundSource::SetListenerTransformAngle( const btTransform & transform, const float innerAngle, const float outerAngle )
{
	this->SetLocation( transform.getOrigin() );
	this->SetDirectionAngle( transform.getRotation(), innerAngle, outerAngle );
}

void SoundSource::SetVolume( const float volume )
{
	alSourcef( this->sourceID, AL_GAIN, volume );
}

void SoundSource::SetSampler( const SoundSampler & sampler )
{
	this->sampler = sampler;
	alSourcei( this->sourceID, AL_BUFFER, this->sampler.GetBuffer() );
}

SoundSource::SoundSource( const SoundSampler & sampler )
{
	alGenSources( 1, &(this->sourceID) );
	alSourcef( this->sourceID, AL_PITCH, 1.0f );
	alSourcef( this->sourceID, AL_GAIN, 1.0f );
	alSource3f( this->sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f );
	alSource3f( this->sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	alSourcei( this->sourceID, AL_LOOPING, AL_FALSE );
	alSourcei( this->sourceID, AL_SOURCE_RELATIVE, AL_FALSE );
	this->SetSampler( sampler );
}

SoundSource::SoundSource()
{
	alGenSources( 1, &(this->sourceID) );
	alSourcef( this->sourceID, AL_PITCH, 1.0f );
	alSourcef( this->sourceID, AL_GAIN, 1.0f );
	alSource3f( this->sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f );
	alSource3f( this->sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	alSourcei( this->sourceID, AL_LOOPING, AL_FALSE );
	alSourcei( this->sourceID, AL_SOURCE_RELATIVE, AL_FALSE );
}

SoundSource::~SoundSource()
{
	if( this->sourceID )
		alDeleteSources( 1, &(this->sourceID) );
	this->sourceID = 0;
	this->sampler.Destroy();
}



SoundEngine::operator bool() const
{
	return (this->device) && (this->context);
}

void SoundEngine::PrintError()
{
	fprintf( stderr, "\n OpenAL error: %i", alcGetError( (ALCdevice*)(this->device) ) );
}

void SoundEngine::SetListenerLocation( const btVector3 & location )
{
	alListenerfv( AL_POSITION, location.m_floats );
}

void SoundEngine::SetListenerVelocity( const btVector3 & velocity )
{
	alListenerfv( AL_VELOCITY, velocity.m_floats );
}

void SoundEngine::SetListenerOrientation( const btVector3 & forward, const btVector3 & up )
{
	float ar[6] = { forward.x(), forward.y(), forward.z(), up.x(), up.y(), up.z() };
	alListenerfv( AL_ORIENTATION, ar );
}

void SoundEngine::SetListenerOrientation( const btQuaternion & rotation )
{
	btTransform transform( rotation );
	SoundEngine::SetListenerOrientation( transform*btVector3(0,0,1), transform*btVector3(0,1,0) );
}

void SoundEngine::SetListenerTransform( const btTransform & transform )
{
	SoundEngine::SetListenerLocation( transform.getOrigin() );
	SoundEngine::SetListenerOrientation( transform.getRotation() );
}

SoundEngine::SoundEngine()
{
	this->device = NULL;
	this->context = NULL;
    this->device = (ALCdevice*)alcOpenDevice( NULL );
    if( this->device == NULL )
    {
    	this->PrintError();
        return;
    }
    this->context = (ALCcontext*)alcCreateContext( (ALCdevice*)(this->device), NULL );
    if( this->context == NULL )
    {
    	this->PrintError();
    	return;
    }
    alcMakeContextCurrent( (ALCcontext*)(this->context) );
}

SoundEngine::~SoundEngine()
{
	if( this->device )
	{
		if( this->context )
	    	alcDestroyContext( (ALCcontext*)(this->context) );
	    alcCloseDevice( (ALCdevice*)(this->device) );
	}
	this->context = NULL;
	this->device = NULL;
}

SoundSampler SoundsManager::GetSoundSampler( const std::string & soundName )
{
	auto it = this->nids.find( soundName );
	if( it != this->nids.end() )
	{
		return this->GetSoundSampler( it->second );
	}
	return SoundSampler();
}

SoundSampler SoundsManager::GetSoundSampler( const unsigned soundID )
{
	this->times.erase( soundID );
	auto it = this->samplers.find( soundID );
	if( it != this->samplers.end() )
	{
		return it->second;
	}
	auto it2 = this->files.find( soundID );
	if( it2 == this->files.end() )
	{
		return SoundSampler();
	}
	SoundSampler sampler( it2->second );
	this->samplers[soundID] = sampler;
	return sampler;
}

unsigned SoundsManager::GetSoundID( const std::string & soundName ) const
{
	auto it = this->nids.find( soundName );
	if( it != this->nids.end() )
		return it->second;
	return 0;
}

unsigned SoundsManager::RegisterSound( const std::string & soundName, const std::string & soundfilePath )
{
	auto it0 = this->nids.find( soundName );
	if( it0 != this->nids.end() )
	{
		return 0;
	}
	unsigned soundID = ++(this->lastRegisteredID);
	this->nids[soundName] = soundID;
	this->idsn[soundID] = soundName;
	this->files[soundID] = soundfilePath;
	return soundID;
}

void SoundsManager::RemoveSound( const unsigned soundID )
{
	auto it = this->idsn.find( soundID );
	if( it == this->idsn.end() )
		return;
	std::string soundName = it->second;
	this->idsn.erase( it );
	this->nids.erase( soundName );
	this->files.erase( soundID );
	this->samplers.erase( soundID );
	this->times.erase( soundID );
}

void SoundsManager::RemoveSound( const std::string & soundName )
{
	auto it = this->nids.find( soundName );
	if( it != this->nids.end() )
		this->RemoveSound( it->second );
}

void SoundsManager::ImplicitFreeMemory( const unsigned soundID )
{
	auto it = this->samplers.find( soundID );
	if( it == this->samplers.end() )
		return;
	
	this->samplers.erase( soundID );
	this->times[soundID] = clock();
}

void SoundsManager::RemoveUnusedSamplersFromMemory()
{
	std::vector < unsigned > idsToErase;
	
	for( auto it = this->samplers.begin(); it != this->samplers.end(); ++it )
	{
		if( it->second.GetRefCounterValue() == 1 )
		{
			auto it1 = this->times.find( it->first );
			if( it1 == this->times.end() )
				idsToErase.insert( idsToErase.end(), it->first );
		}
		else
			this->times.erase( it->first );
	}
	
	for( auto it = this->times.begin(); it != this->times.end(); ++it )
	{
		auto it1 = this->samplers.find( it->first );
		if( it1 != this->samplers.end() )
		{
			if( it->second + this->idleMilisecondsTillRemove < clock() )
				idsToErase.insert( idsToErase.end(), it->first );
		}
		else
			idsToErase.insert( idsToErase.end(), it->first );
	}
	
	for( auto it = idsToErase.begin(); it != idsToErase.end(); ++it )
	{
		this->times.erase( *it );
		this->samplers.erase( *it );
	}
}

void SoundsManager::SetIdleMilisecondsTillRemove( unsigned miliseconds )
{
	this->idleMilisecondsTillRemove = miliseconds;
}

void SoundsManager::Clear()
{
	this->nids.clear();
	this->files.clear();
	this->idsn.clear();
	this->samplers.clear();
	this->times.clear();
	this->idleMilisecondsTillRemove = 30*1000;
	this->lastRegisteredID = 1;
}

SoundsManager::SoundsManager()
{
	this->idleMilisecondsTillRemove = 30*1000;
	this->lastRegisteredID = 1;
}

SoundsManager::~SoundsManager()
{
	this->nids.clear();
	this->files.clear();
	this->idsn.clear();
	this->samplers.clear();
	this->times.clear();
	this->idleMilisecondsTillRemove = 0;
	this->lastRegisteredID = 0;
}

#endif

