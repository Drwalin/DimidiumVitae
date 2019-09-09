
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DLL_IMPORTER_CPP
#define DLL_IMPORTER_CPP

#include "DllImporter.h"

#if defined __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#if defined __WIN32__ || defined _WIN64

#include <windows.h>

void * DllLoad( const char * file )
{
	if( file )
	{
		return LoadLibrary( file );
	}
	return nullptr;
}

void DllRelease( void * handle )
{
	if( handle )
	{
		FreeLibrary( (HINSTANCE)handle );
	}
}

void * DllGetObject( void * handle, const char * objectName )
{
	if( handle && objectName )
	{
		return (void*)GetProcAddress( (HINSTANCE)handle, objectName );
	}
	return nullptr;
}

char * DllGetErrorString()
{
	const int msgSize = 1024;
	static char msg[msgSize];
	DWORD err = GetLastError();
	if( err == 0 )
		return (msg[0]=0,msg);
	
	/*
	size_t size = FormatMessageA(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									NULL,
									errorMessageID,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									(LPSTR)&msg,
									msgSize,
									NULL );
	*/
	FormatMessageA(	FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					msg, msgSize, NULL );
	
	return msg;
}

#elif defined __unix__

#include <dlfcn.h>

void * DllLoad( const char * file )
{
	if( file )
	{
		return dlopen( file, RTLD_NOW );	// RTLD_LAZY );
	}
	return nullptr;
}

void DllRelease( void * handle )
{
	if( handle )
	{
		dlclose( handle );
	}
}

void * DllGetObject( void * handle, const char * objectName )
{
	if( handle && objectName )
	{
		return dlsym( handle, objectName );
	}
	return nullptr;
}

char * DllGetErrorString()
{
	return dlerror();
}

#  warning Not yet tested

#else

#  error Unknown system, or not defined symbol ( __unix__ or __WIN32__ or _WIN64 )

#endif		// system type



#if defined __cplusplus

bool Dll::IsValid() const
{
	return this->handle != NULL;
}

void * Dll::Open( const char * dllFileName )
{
	this->Close();
	this->handle = DllLoad( dllFileName );
	if( this->handle == NULL )
		printf( "\n Dll::Open error: %s", DllGetErrorString() );
	return this->handle;
}

void Dll::Close()
{
	if( this->handle )
		DllRelease( this->handle );
	this->handle = NULL;
}

Dll::Dll()
{
	this->handle = NULL;
}

Dll::Dll( const char * dllFileName )
{
	this->handle = NULL;
	this->Open( dllFileName );
}

Dll::~Dll()
{
	this->Close();
}

#endif

#endif		// DLL_IMPORTER_CPP

