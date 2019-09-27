
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CLASS_FACTORY_H
#define CLASS_FACTORY_H

#include "ModulesFactory.h"

template < typename T >
class ClassFactory : public ModulesFactory
{
protected:
	
	std::map < std::string, std::shared_ptr<T> > uniqueObjects;
	
public:
	
	std::shared_ptr<T> GetNewOf( const char * className );
	
	std::shared_ptr<T> GetClassInstantiator( const char * className );
	std::shared_ptr<T> AddClass( const char * className, const char * moduleName );
	void RemoveClass( const char * moduleName );
	
	ClassFactory();
	~ClassFactory();
};

#endif

