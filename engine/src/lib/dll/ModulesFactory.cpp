
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODULES_FACTORY_CPP
#define MODULES_FACTORY_CPP

#include "ModulesFactory.h"

std::shared_ptr<Dll> ModulesFactory::GetModule( const char * moduleName )
{
	auto it = this->dlls.find( moduleName );
	if( it != this->dlls.end() )
		return it->second;
	return NULL;
}

void ModulesFactory::RemoveModule( const char * moduleName )
{
	this->dlls.erase( moduleName );
}

std::shared_ptr<Dll> ModulesFactory::AddModule( const char * modulePath, const char * moduleName )
{
	auto it = this->dlls.find( std::string(moduleName) );
	if( it != this->dlls.end() )
	{
		return it->second;
	}
	std::shared_ptr<Dll> dll( new Dll( modulePath ) );
	if( dll->IsValid() == false )
	{
		return NULL;
	}
	auto pair = std::make_pair( std::string(moduleName), dll );
	this->dlls.insert( pair );
	return dll;
}

ModulesFactory::ModulesFactory(){}
ModulesFactory::~ModulesFactory(){}

#endif

