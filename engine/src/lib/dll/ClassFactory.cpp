
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CLASS_FACTORY_CPP
#define CLASS_FACTORY_CPP

#include "ClassFactory.h"

#include <cstdio>

template < typename T >
std::shared_ptr<T> ClassFactory<T>::GetNewOf( const char * className )
{
	auto it = this->uniqueObjects.find( className );
	if( it == this->uniqueObjects.end() )
		return NULL;
	return it->second->New();
}

template < typename T >
std::shared_ptr<T> ClassFactory<T>::GetClassInstantiator( const char * className )
{
	auto it = this->uniqueObjects.find( className );
	if( it == this->uniqueObjects.end() )
		return NULL;
	return it->second;
}

template < typename T >
std::shared_ptr<T> ClassFactory<T>::AddClass( const char * className, const char * moduleName )
{
	auto it = this->uniqueObjects.find( className );
	if( it != this->uniqueObjects.end() )
		return it->second;
	
	std::shared_ptr<Dll> dll = this->GetModule( moduleName );
	if( dll == NULL )
	{
		fprintf( stderr, "\n Module: \"%s\" is not registered while getting %s class instantiator", moduleName, className );
		return NULL;
	}
	
	std::string instantiatorSymbol = std::string("Get") + className + "Instantiator";
	
	std::shared_ptr<T>(*GetClassInstantiator)();
	GetClassInstantiator = dll->Get<std::shared_ptr<T>(*)(void)>( instantiatorSymbol.c_str() );
	
	if( GetClassInstantiator == NULL )
	{
		fprintf( stderr, "\n Cannot get symbol: \"%s\" from: <%s>", instantiatorSymbol.c_str(), moduleName );
		return NULL;
	}
	
	std::shared_ptr<T> instantiator = GetClassInstantiator();
	if( instantiator == NULL )
	{
		fprintf( stderr, "\n Cannot get instantiator from: %s::%s", moduleName, className );
		this->RemoveModule( moduleName );
		return NULL;
	}
	
	this->uniqueObjects.insert( std::pair(std::string(className),instantiator) );
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

