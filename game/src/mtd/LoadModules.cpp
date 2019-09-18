
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef LOAD_MODULES_CPP
#define LOAD_MODULES_CPP

#include "..\css\Header.h"

void LoadModules( Engine * engine, const std::string & modulesList )
{
	std::ifstream file( modulesList );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string  moduleFile(""), moduleName("");
			std::getline( file, moduleName );
			std::getline( file, moduleFile );
			if( file.eof() )
				break;
			
			if( moduleFile.size() )
			{
				if( moduleFile[moduleFile.size()-1] == 13 )
					moduleFile.resize( moduleFile.size() - 1 );
			}
			else
				continue;
			if( moduleName.size() )
			{
				if( moduleName[moduleName.size()-1] == 13 )
					moduleName.resize( moduleName.size() - 1 );
			}
			else
				continue;
			
			if( !engine->RegisterType( moduleName, moduleFile ) )
			{
				DEBUG( std::string("Couldn't load module: \"") + moduleName + "\" -> \"" + moduleFile + "\"" );
			}
		}
	}
}

#endif

