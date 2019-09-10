
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_WALK_TRIGGER_CPP
#define CHARACTER_WALK_TRIGGER_CPP

#include "..\css\CharacterWalkTrigger.h"

bool CharacterWalkTrigger::IsAnyInside() const
{
	return isAnyInside;
}

void CharacterWalkTrigger::SetParent( std::shared_ptr<Entity> parent )
{
	this->parent = parent;
}

void CharacterWalkTrigger::NextOverlappingFrame()
{
	Trigger::NextOverlappingFrame();
	isAnyInside = false;
}

void CharacterWalkTrigger::EventOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	if( other != parent.get() )
	{
		if( persisstentManifold->getNumContacts() > 0 )
		{
			isAnyInside = true;
		}
	}
}

void CharacterWalkTrigger::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	Trigger::EventOnEntityBeginOverlapp( other, persisstentManifold );
	this->EventOverlapp( other, persisstentManifold );
}

void CharacterWalkTrigger::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	Trigger::EventOnEntityTickOverlapp( other, persisstentManifold );
	this->EventOverlapp( other, persisstentManifold );
}

void CharacterWalkTrigger::EventOnEntityEndOverlapp( Entity * other )
{
	Trigger::EventOnEntityEndOverlapp( other );
}

void CharacterWalkTrigger::Tick( const float deltaTime )
{
	Trigger::Tick( deltaTime );
}


void CharacterWalkTrigger::Load( std::istream & stream )
{
	Trigger::Load( stream );
}

void CharacterWalkTrigger::Save( std::ostream & stream ) const
{
	Trigger::Save( stream );
}

void CharacterWalkTrigger::Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Trigger::Spawn( name, shape, transform );
}

void CharacterWalkTrigger::Despawn()
{
	Trigger::Despawn();
}

void CharacterWalkTrigger::Destroy()
{
	Trigger::Destroy();
}

extern "C" std::shared_ptr<Entity> GetClassInstantiator(){ static std::shared_ptr<Entity> instantiator( new CharacterWalkTrigger(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int CharacterWalkTrigger::GetTypeSize() const{ return sizeof(CharacterWalkTrigger); }
void CharacterWalkTrigger::Free(){ delete this; }
std::shared_ptr<Entity> CharacterWalkTrigger::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<CharacterWalkTrigger>( new CharacterWalkTrigger(), [](Entity * ptr){delete ptr;} ) ); }
std::string CharacterWalkTrigger::GetClassName() const{ return "CharacterWalkTrigger"; }

CharacterWalkTrigger::CharacterWalkTrigger()
{
	isAnyInside = false;
}

CharacterWalkTrigger::~CharacterWalkTrigger()
{
	isAnyInside = false;
}

#endif

