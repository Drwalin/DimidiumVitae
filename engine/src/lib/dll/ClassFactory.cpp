
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CLASS_FACTORY_CPP
#define CLASS_FACTORY_CPP

#include "ClassFactory.h"

#include <cstdio>

template < typename T >
std::shared_ptr<T> ClassFactory<T>::GetNewOf( const char * moduleName )
{
	auto it = this->uniqueObjects.find( moduleName );
	if( it == this->uniqueObjects.end() )
		return NULL;
	return it->second->New();
}

template < typename T >
std::shared_ptr<T> ClassFactory<T>::GetClassInstantiator( const char * moduleName )
{
	auto it = this->uniqueObjects.find( moduleName );
	if( it == this->uniqueObjects.end() )
		return NULL;
	return it->second;
}

template < typename T >
std::shared_ptr<T> ClassFactory<T>::AddClass( const char * modulePath, const char * moduleName )
{
	auto it = this->uniqueObjects.find( moduleName );
	if( it != this->uniqueObjects.end() )
		return it->second;
	std::shared_ptr<Dll> dll = this->AddModule( modulePath, moduleName );
	if( dll == NULL )
	{
		fprintf( stderr, "\n Cannot get module: <%s>", modulePath );
		return NULL;
	}
	
	std::shared_ptr<T>(*GetClassInstantiator)();
	GetClassInstantiator = dll->Get<decltype(GetClassInstantiator)>( "GetClassInstantiator" );
	if( GetClassInstantiator == NULL )
	{
		fprintf( stderr, "\n Cannot get symbol: \"GetClassInstantiator\" from: <%s>", modulePath );
		this->RemoveModule( moduleName );
		return NULL;
	}
	
	std::shared_ptr<T> instantiator = GetClassInstantiator();
	if( instantiator == NULL )
	{
		fprintf( stderr, "\n Cannot get instantiator from: <%s>", moduleName );
		this->RemoveModule( moduleName );
		return NULL;
	}
	
	this->uniqueObjects.insert( std::pair(std::string(moduleName),instantiator) );
	return instantiator;
}

template < typename T >
void ClassFactory<T>::RemoveClass( const char * moduleName )
{
	this->uniqueObjects.erase( moduleName );
	this->RemoveModule( moduleName );
}

template < typename T >
ClassFactory<T>::ClassFactory(){}

template < typename T >
ClassFactory<T>::~ClassFactory()
{
	this->uniqueObjects.clear();
}

#endif

