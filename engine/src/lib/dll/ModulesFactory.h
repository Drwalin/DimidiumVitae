
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODULES_FACTORY_H
#define MODULES_FACTORY_H

#include "DllImporter.h"

#include <map>
#include <string>
#include <memory>

class ModulesFactory
{
protected:
	
	std::map < std::string, std::shared_ptr<Dll> > dlls;
	
public:
	
	std::shared_ptr<Dll> GetModule( const char * moduleName );
	
	std::shared_ptr<Dll> AddModule( const char * modulePath, const char * moduleName );
	void RemoveModule( const char * moduleName );
	
	ModulesFactory();
	~ModulesFactory();
};


#endif

