
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

/*
	never use:
		delete $TypeName;
		new $TypeName;
	
	CLASS_FACTORY_GENERATE_NEEDED_FUNCTIONS_DEFINITION need to be before:
		CLASS_FACTORY_GENERATE_NEEDED_METHODS_DEFINITION
*/

#ifndef CLASS_FACTORY_BASE_H
#define CLASS_FACTORY_BASE_H

#include <string>
#include <memory>


#define CLASS_FACTORY_GENERATE_NEEDED_FUNCTIONS_DEFINITION( className, base ) \
	std::shared_ptr<className> staticGlobal##className##Instantiator( new className() ); \
	ClassFactoryBase::~ClassFactoryBase(){} \
	extern "C" std::shared_ptr<base> GetClassInstantiator() { return std::dynamic_pointer_cast<base>( staticGlobal##className##Instantiator ); } \
	extern "C" void Destructor( base * ptr ) { delete ((className*)(ptr)); }


#define CLASS_FACTORY_GENERATE_NEEDED_METHODS_DECLARATION( className, base ) \
	public: \
		virtual int GetTypeSize() const override; \
		virtual void Free() override; \
		virtual std::shared_ptr<base> New( void * args ) const override; \
		virtual std::shared_ptr<base> New() const override; \
		virtual std::string GetClassName() const override;

#define CLASS_FACTORY_GENERATE_NEEDED_METHODS_DEFINITION( className, base ) \
	int className::GetTypeSize() const { return sizeof(className); } \
	void className::Free() { delete this; } \
	std::shared_ptr<base> className::New( void * args ) const { return std::shared_ptr<base>( (base*)new className( args ), Destructor ); } \
	std::shared_ptr<base> className::New() const { return std::shared_ptr<base>( (base*)new className(), Destructor ); } \
	std::string className::GetClassName() const { return #className; }


#define CLASS_FACTORY_GENERATE_OPTIONAL_EMPTY_CONSTRUCTOS_DESTRUCTOR_DECLARATION( className ) \
	public: \
		className(); \
		className( void * args ); \
		virtual ~className() override;

#define CLASS_FACTORY_GENERATE_OPTIONAL_EMPTY_CONSTRUCTOS_DESTRUCTOR_DEFINITION( className ) \
	className::className() {} \
	className::className( void * args ) {} \
	className::~className() {}

class ClassFactoryBase
{
public:
	
	virtual int GetTypeSize() const = 0;										// return size of type in bytes
	virtual void Free() = 0;													// calls type destructor and frees an object
	virtual std::shared_ptr<ClassFactoryBase> New( void * args ) const = 0;		// allocate memory and calls type constructor
	virtual std::shared_ptr<ClassFactoryBase> New() const = 0;					// allocate memory and calls type constructor
	virtual std::string GetClassName() const = 0;								// returns type name
	
	virtual ~ClassFactoryBase();
};

#endif

