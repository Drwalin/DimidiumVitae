
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef OGG_CPP
#define OGG_CPP

#include "Ogg.h"

#include <vorbis/vorbisfile.h>

#include <fstream>

#include <cstdio>

extern "C" bool OGGLoadFromFile(const char *oggFileName, std::vector<char> &bufferData, ALenum &format, ALsizei &sampleRate) {
	FILE *file = fopen(oggFileName, "rb");
	if(file == NULL)
		return false;
	
	OggVorbis_File oggFileStream;
	if(ov_open(file, &oggFileStream, NULL, 0) != 0)
		return false;
	
	vorbis_info *oggInfo;
	oggInfo = ov_info(&oggFileStream, -1);
	sampleRate = oggInfo->rate;
	if(oggInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	
	const int cacheBufferSize = 32768;
	char *cacheBuffer = (char*)malloc(cacheBufferSize);
	if(cacheBuffer == NULL) {
		ov_clear(&oggFileStream);
		return false;
	}
	
	int bitStream = 0;
	while(true) {
		long bytesReaded;
		bytesReaded = ov_read(&oggFileStream, cacheBuffer, cacheBufferSize, 0, 2, 1, &bitStream);
		if(bytesReaded < 0) {
			ov_clear(&oggFileStream);
			free(cacheBuffer);
			return false;
		}
		bufferData.insert(bufferData.end(), cacheBuffer, cacheBuffer+bytesReaded);
		if(bytesReaded == 0)
			break;
	}
	
	ov_clear(&oggFileStream);
	free(cacheBuffer);
	return true;
}

#endif

