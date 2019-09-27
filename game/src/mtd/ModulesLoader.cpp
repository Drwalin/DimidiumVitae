
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODULES_LOADER_CPP
#define MODULES_LOADER_CPP

#include "..\css\Header.h"

#include <StdUtil.hpp>

void LoadModules( Engine * engine, const std::string & modulesListFileName )
{
	std::ifstream file( modulesListFileName );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string  moduleFile(""), moduleName(""), className("");
			GetLine( file, className );
			GetLine( file, moduleName );
			GetLine( file, moduleFile );
			if( file.eof() )
				break;
			if( moduleName == "" || moduleFile == "" )
				continue;
			
			if( !engine->RegisterType( className, moduleName, moduleFile ) )
			{
				MESSAGE( std::string("Couldn't load module: \"") + moduleName + "\" -> \"" + moduleFile + "\" : " + className );
			}
		}
	}
}

#endif

