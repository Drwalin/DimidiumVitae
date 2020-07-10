
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef OGG_H
#define OGG_H

#include <AL/alc.h>
#include <AL/al.h>

#include <vector>

extern "C" bool OGGLoadFromFile( const char * oggFileName, std::vector<char> & bufferData, ALenum & format, ALsizei & sampleRate );

#endif

