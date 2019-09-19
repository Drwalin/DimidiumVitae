
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SHAPE_LOADER_CPP
#define SHAPE_LOADER_CPP

#include "..\css\Header.h"

#include <fstream>

#include <cassert>

void LoadShapes( std::string loadShapesListFile, Engine * engine )
{
	std::ifstream file( loadShapesListFile );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string objFile(""), shapeFile(""), customName("");
			std::getline( file, objFile );
			std::getline( file, shapeFile );
			std::getline( file, customName );
			if( file.eof() )
				break;
			
			if( objFile.size() )
			{
				if( objFile[objFile.size()-1] == 13 )
					objFile.resize( objFile.size() - 1 );
			}
			if( shapeFile.size() )
			{
				if( shapeFile[shapeFile.size()-1] == 13 )
					shapeFile.resize( shapeFile.size() - 1 );
			}
			if( customName.size() )
			{
				if( customName[customName.size()-1] == 13 )
					customName.resize( customName.size() - 1 );
			}
			
			if( !std::ifstream(shapeFile).good() )
			{
				if( engine->GetCollisionShapeManager()->ConvertObjToCustomShape( objFile, shapeFile ) )
				{
					MESSAGE( std::string("Shape converted: ") + customName + " = " + objFile + " -> " + shapeFile );
				}
				else
				{
					MESSAGE( std::string("Couldn't convert shape: ") + customName + " = " + objFile + " -> " + shapeFile );
				}
			}
			
			if( !engine->GetCollisionShapeManager()->RegisterCustomShape( customName, shapeFile ) )
			{
				MESSAGE( std::string("Couldn't register shape: ") + customName + " = " + objFile + " -> " + shapeFile );
			}
		}
	}
}

#endif

