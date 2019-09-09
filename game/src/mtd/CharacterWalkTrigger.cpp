
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

CharacterWalkTrigger::CharacterWalkTrigger( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
	Trigger( engine, name, body, collisionShape, mass_ )
{
	isAnyInside = false;
}

CharacterWalkTrigger::CharacterWalkTrigger()
{
	isAnyInside = false;
}

CharacterWalkTrigger::~CharacterWalkTrigger()
{
	isAnyInside = false;
}

#endif

