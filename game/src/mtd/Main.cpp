
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <dll\DllImporter.h>

#include <Debug.h>

int main( int argc, char ** argv )
{
	Dll eventModule( "game-core.dll" );
	int (*Init)( int argc, char ** argv );
	Init = eventModule.Get<int(*)(int,char**)>( "Init" );
	if( Init )
	{
		int ret = Init( argc, argv );
		return ret;
	}
	return -1;
}



