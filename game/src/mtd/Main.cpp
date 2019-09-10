
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <dll\DllImporter.h>

#include <Debug.h>

int main( int argc, char ** argv )
{
	Dll characterModule( "dlls/Character.dll" );
	DEBUG(1)
	Dll eventModule( "game-core.dll" );
	int (*Init)( int argc, char ** argv );
	Init = eventModule.Get<decltype(Init)>( "Init" );
	if( Init )
	{
		DEBUG(2)
		return Init( argc, argv );
	}
	DEBUG( "Should not appear" );
	return -1;
}



