
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WAV_H
#define WAV_H

#include <cinttypes>

typedef struct
{
	uint32_t ChunkID;		// "RIFF"
	uint32_t ChunkSize;		// 36 + Subchunk2Size
	uint32_t Format;		// "WAVE"

	uint32_t Subchunk1Id;	// "fmt "
	uint32_t Subchunk1Size;	// 16
	uint16_t AudioFormat;	// 1
	uint16_t NumChannels;	// 1, 2
	uint32_t SampleRate;	// 8000, 11025, 22050, 44100, ...
	uint32_t ByteRate;		// SampleRate * NumChannels * BitsPerSample/8
	uint16_t BlockAlign;	// NumChannels * BitsPerSample/8
	uint16_t BitsPerSample;	// 8, 16

	uint32_t Subchunk2Id;	// "data"
	uint32_t Subchunk2Size;
	// buffer data...
} WavHeader;

extern "C" float WAVGetDuration( const WavHeader * wavHeader );
extern "C" void * WAVLoadFromFile( WavHeader * wavHeader, const char * fileName );
extern "C" void WAVFree( void * buffer );

#endif

