
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CLASS_FACTORY_CPP
#define CLASS_FACTORY_CPP

#include "ClassFactory.h"

#include <cstdio>

template < typename T >
T* ClassFactory<T>::GetNewOf(const char *className) {
	auto it = uniqueObjects.find(className);
	if(it == uniqueObjects.end())
		return NULL;
	return it->second->New();
}

template < typename T >
T* ClassFactory<T>::GetClassInstantiator(const char *className) {
	auto it = uniqueObjects.find(className);
	if(it == uniqueObjects.end())
		return NULL;
	return it->second.get();
}

template < typename T >
T* ClassFactory<T>::AddClass(const char *className, const char *moduleName) {
	auto it = uniqueObjects.find(className);
	if(it != uniqueObjects.end())
		return it->second.get();
	
	std::shared_ptr<Dll> dll = GetModule(moduleName);
	if(dll == NULL) {
		fprintf(stderr, "\n Module: \"%s\" is not registered while getting %s class instantiator", moduleName, className);
		return NULL;
	}
	
	std::string instantiatorSymbol = std::string("Get") + className + "Instantiator";
	
	std::shared_ptr<T>(*GetClassInstantiator)();
	GetClassInstantiator = dll->Get<std::shared_ptr<T>(*)(void)>(instantiatorSymbol.c_str());
	
	if(GetClassInstantiator == NULL) {
		fprintf(stderr, "\n Cannot get symbol: \"%s\" from: <%s>", instantiatorSymbol.c_str(), moduleName);
		return NULL;
	}
	
	std::shared_ptr<T> instantiator = GetClassInstantiator();
	if(instantiator == NULL) {
		fprintf(stderr, "\n Cannot get instantiator from: %s::%s", moduleName, className);
		RemoveModule(moduleName);
		return NULL;
	}
	
	uniqueObjects.insert(std::pair(std::string(className),instantiator));
	return instantiator.get();
}

template < typename T >
void ClassFactory<T>::RemoveClass(const char *moduleName) {
	uniqueObjects.erase(moduleName);
	RemoveModule(moduleName);
}

template < typename T >
ClassFactory<T>::ClassFactory() {}

template < typename T >
ClassFactory<T>::~ClassFactory() {
	uniqueObjects.clear();
}

#endif

