
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CONVERTER_CPP
#define CONVERTER_CPP

#include "..\css\Header.h"

bool Convert( std::string objFile, std::string meshFile, bool scale, btVector3 size )
{
	return true;
}

void ConvertMeshes( std::string convertMeshesListFile )
{
	std::ifstream file( convertMeshesListFile );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string objFile(""), meshFile(""), scale("");
			std::getline( file, objFile );
			std::getline( file, meshFile );
			std::getline( file, scale );
			if( file.eof() )
				break;
			
			if( objFile.size() )
			{
				if( objFile[objFile.size()-1] == 13 )
					objFile.resize( objFile.size() - 1 );
			}
			if( meshFile.size() )
			{
				if( meshFile[meshFile.size()-1] == 13 )
					meshFile.resize( meshFile.size() - 1 );
			}
			if( scale.size() )
			{
				if( scale[scale.size()-1] == 13 )
					scale.resize( scale.size() - 1 );
			}
			
			std::stringstream ss( scale );
			scale = "";
			ss >> scale;
			for( int i = 0; i < scale.size(); ++i )
				if( scale[i] <= 'Z' && scale[i] >= 'A' )
					scale[i] += ('a' - 'A');
			if( scale == "true" )
			{
				btVector3 size;
				ss >> size.m_floats[0];
				ss >> size.m_floats[1];
				ss >> size.m_floats[2];
				
				Convert( objFile, meshFile, true, size );
			}
			else
			{
				Convert( objFile, meshFile );
			}
		}
	}
}

#endif

