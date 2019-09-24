
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef LOAD_MODULES_CPP
#define LOAD_MODULES_CPP

#include "..\css\Header.h"

#include <StdUtil.hpp>

void LoadModules( Engine * engine, const std::string & modulesList )
{
	std::ifstream file( modulesList );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string  moduleFile(""), moduleName("");
			GetLine( file, moduleName );
			GetLine( file, moduleFile );
			if( file.eof() )
				break;
			if( moduleName == "" || moduleFile == "" )
				continue;
			
			if( !engine->RegisterType( moduleName, moduleFile ) )
			{
				MESSAGE( std::string("Couldn't load module: \"") + moduleName + "\" -> \"" + moduleFile + "\"" );
			}
		}
	}
}

#endif

