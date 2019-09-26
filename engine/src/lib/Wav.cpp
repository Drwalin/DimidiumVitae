
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WAV_CPP
#define WAV_CPP

#include "Wav.h"

#include <cstdio>
#include <cstdlib>

extern "C" float WAVGetDuration( const WavHeader * wavHeader )
{
	return ((float)(wavHeader->Subchunk2Size)) / ((float)((wavHeader->SampleRate)*(wavHeader->SampleRate)));
}

extern "C" void * WAVLoadFromFile( WavHeader * wavHeader, const char * fileName )
{
	if( wavHeader )
	{
		FILE * file = fopen( fileName, "rb" );
		if( file )
		{
			int readed = 0;
			readed = fread( (char*)wavHeader, 1, sizeof(WavHeader), file );
			if( readed != sizeof(WavHeader) )
			{
				fclose( file );
				fprintf( stderr, "\n Error while reading file: %s - couldn't read all the WavHeader", fileName );
				return NULL;
			}
			char * buffer = (char*)malloc( wavHeader->Subchunk2Size );
			if( buffer == NULL )
			{
				fclose( file );
				fprintf( stderr, "\n Error while allocating memory ( %i bytes ) for Wav buffer while reading Wav file: %s", wavHeader->Subchunk2Size, fileName );
				return NULL;
			}
			readed = fread( buffer, 1, wavHeader->Subchunk2Size, file );
			if( readed != wavHeader->Subchunk2Size )
			{
				fclose( file );
				free( buffer );
				fprintf( stderr, "\n Error while reading from file (%s) to Wav buffer", fileName );
				return NULL;
			}
			fclose( file );
			return (void*)buffer;
		}
	}
	return NULL;
}

extern "C" void WAVFree( void * buffer )
{
	free( buffer );
}

#endif

