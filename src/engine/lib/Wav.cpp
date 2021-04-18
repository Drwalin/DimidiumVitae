
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WAV_CPP
#define WAV_CPP

#include "Debug.h"
#include "Wav.h"

#include <Singleton.h>
#include <FileSystem.h>

#include <cstdlib>

float WAVGetDuration(const WavHeader *wavHeader) {
	return ((float)(wavHeader->Subchunk2Size)) / ((float)((wavHeader->SampleRate)*(wavHeader->SampleRate)));
}

void *WAVLoadFromFile(WavHeader *wavHeader, const char *fileName) {
	if(wavHeader) {
		IFile file = sing::fileSystem->ReadFile(fileName);
		if(file) {
			file.read((char*)wavHeader,  sizeof(WavHeader));
			if(file.fail()) {
				fprintf(stderr, "\n Error while reading Wav file: \"%s\" - couldn't read all the WavHeader", fileName);
				return NULL;
			}
			char *buffer = (char*)malloc(wavHeader->Subchunk2Size);
			if(buffer == NULL) {
				fprintf(stderr, "\n Error while allocating memory (%i bytes) for Wav buffer while reading Wav file: \"%s\"", wavHeader->Subchunk2Size, fileName);
				return NULL;
			}
			file.read(buffer, wavHeader->Subchunk2Size);
			int c = file.gcount();
			if(c != (int)wavHeader->Subchunk2Size) {
				free(buffer);
				fprintf(stderr, "\n Error while reading from file \"%s\" to Wav buffer; readed: %i / %i", fileName, c, wavHeader->Subchunk2Size);
				return NULL;
			}
			return (void*)buffer;
		}
	}
	return NULL;
}

void WAVFree(void *buffer) {
	free(buffer);
}

#endif
