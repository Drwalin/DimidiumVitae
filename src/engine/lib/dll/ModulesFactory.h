
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODULES_FACTORY_H
#define MODULES_FACTORY_H

#include "DllImporter.h"

#include <map>
#include <string>
#include <memory>

class ModulesFactory {
public:

	ModulesFactory();
	~ModulesFactory();
	
	std::shared_ptr<Dll> GetModule(const char *name);
	
	std::shared_ptr<Dll> AddModule(const char *path);
	void RemoveModule(const char *name);
	
protected:
	
	std::map<std::string, std::shared_ptr<Dll> > dlls;
	
};

#endif
