
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SHAPE_LOADER_CPP
#define SHAPE_LOADER_CPP

#include "..\css\Header.h"

#include <StdUtil.hpp>

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
			GetLine( file, objFile );
			GetLine( file, shapeFile );
			GetLine( file, customName );
			if( file.eof() )
				break;
			
			if( objFile == "" || shapeFile == "" || customName == "" )
				continue;
			
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

