
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODULES_FACTORY_CPP
#define MODULES_FACTORY_CPP

#include "ModulesFactory.h"

#include "../StdUtil.hpp"

std::shared_ptr<Dll> ModulesFactory::GetModule(const char *name) {
	auto it = this->dlls.find(GetCoreName(name));
	if(it != this->dlls.end())
		return it->second;
	return NULL;
}

void ModulesFactory::RemoveModule(const char *name) {
	this->dlls.erase(GetCoreName(name));
}

std::shared_ptr<Dll> ModulesFactory::AddModule(const char *path) {
	std::string name = GetCoreName(path);
	auto it = this->dlls.find(name);
	if(it != this->dlls.end()) {
		return it->second;
	}
	std::shared_ptr<Dll> dll(new Dll(path));
	if(dll->IsValid() == false) {
		fprintf(stderr, "\n Cannot register module \"%s\" as \"%s\"", path, name.c_str());
		return NULL;
	}
	auto pair = std::make_pair(std::string(name), dll);
	this->dlls.insert(pair);
	return dll;
}

ModulesFactory::ModulesFactory() {}
ModulesFactory::~ModulesFactory() {
	this->dlls.clear();
}

#endif

