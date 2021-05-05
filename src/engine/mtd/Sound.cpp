
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUND_CPP
#define SOUND_CPP

#include "../css/Sound.h"
#include "../css/Singleton.h"

#include "../lib/StdUtil.hpp"
#include "../lib/Debug.h"
#include "../lib/Wav.h"
#include "../lib/Ogg.h"

#include <AL/al.h>
#include <AL/alc.h>

void Sound::LoadFromWAV(const std::string &wavFileName) {
	WavHeader wavHeader;
	void *bufferData = WAVLoadFromFile(&wavHeader, wavFileName.c_str());
	if(bufferData) {
		ALenum format;
		switch(wavHeader.BitsPerSample) {
		case 8:
			switch(wavHeader.NumChannels) {
			case 1:
				format = AL_FORMAT_MONO8;
				break;
			case 2:
				format = AL_FORMAT_STEREO8;
				break;
			default:
				WAVFree(bufferData);
				return;
			}
			break;
		case 16:
			switch(wavHeader.NumChannels) {
			case 1:
				format = AL_FORMAT_MONO16;
				break;
			case 2:
				format = AL_FORMAT_STEREO16;
				break;
			default:
				WAVFree(bufferData);
				return;
			}
			break;
		default:
			WAVFree(bufferData);
			return;
		}
		alGenBuffers(1, &bufferID);
		alBufferData(bufferID, format, bufferData, wavHeader.Subchunk2Size,
				wavHeader.SampleRate);
		WAVFree(bufferData);
	} else
		throw std::string(std::string("Cannot open sound file: ")+wavFileName);
}

void Sound::LoadFromOGG(const std::string &oggFileName) {
	std::vector<char> bufferData;
	ALenum format;
	ALsizei sampleRate;
	if(OGGLoadFromFile(oggFileName.c_str(), bufferData, format, sampleRate)) {
		alGenBuffers(1, &bufferID);
		alBufferData(bufferID, format, &(bufferData[0]), bufferData.size(),
				sampleRate);
	} else
		throw std::string(std::string("Cannot open sound file: ")+oggFileName);
}

void Sound::LoadFromFile(const std::string &fileName) {
	std::string extension = GetExtension(fileName);
	if(extension == "ogg")
		LoadFromOGG(fileName);
	else if(extension == "wav")
		LoadFromWAV(fileName);
	else
		throw std::string(std::string("Unrecognized sound file extension: ") +
				fileName + " -> " + extension);
}

unsigned Sound::GetBuffer() const {
	return bufferID;
}

float Sound::GetDuration() const {
	if(bufferID) {
		int sampleRate = 0, bitsPerSample = 0, channels = 0,
				bufferSizeBytes = 0;
		alGetBufferi(bufferID, AL_FREQUENCY, &sampleRate);
		alGetBufferi(bufferID, AL_BITS, &bitsPerSample);
		alGetBufferi(bufferID, AL_CHANNELS, &channels);
		alGetBufferi(bufferID, AL_SIZE, &bufferSizeBytes);
		if(sampleRate == 0 || channels == 0 || AL_BITS == 0)
			return 0;
		return ((float)(bufferSizeBytes)) /
				((float)(sampleRate*channels*bitsPerSample/8));
	}
	return 0.0f;
}

Sound::operator bool() const {
	return bufferID;
}
Sound::Sound(const JSON& json) :
	Resource(json), bufferID(0) {
	LoadFromFile(name);
}

Sound::~Sound() {
	if(bufferID) {
		alDeleteBuffers(1, &bufferID);
		bufferID = 0;
	}
}

Resource::ResourceType Sound::GetResourceType() const {
	return Resource::SOUND;
}

JSON Sound::GetJSON() const {
	JSON json;
	json.InitObject();
	if(name != "") {
		json["class"] = "Sound";
		json["name"] = name;
	}
	return json;
}

#endif
