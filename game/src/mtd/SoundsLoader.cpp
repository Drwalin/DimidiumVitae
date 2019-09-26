
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SOUNDS_LOADER_CPP
#define SOUNDS_LOADER_CPP

#include "..\css\Header.h"

#include <StdUtil.hpp>

void LoadSounds( Engine * engine, const std::string & soundsListFileName )
{
	std::ifstream file( soundsListFileName );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string  soundFile(""), soundName("");
			GetLine( file, soundName );
			GetLine( file, soundFile );
			if( file.eof() )
				break;
			if( soundName == "" || soundFile == "" )
				continue;
			
			if( engine->GetSoundsManager()->RegisterSound( soundName, soundFile ) == 0 )
			{
				MESSAGE( std::string("Couldn't load sound file: \"") + soundName + "\" -> \"" + soundFile + "\"" );
			}
		}
	}
}

#endif

