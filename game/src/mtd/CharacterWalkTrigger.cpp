
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_WALK_TRIGGER_CPP
#define CHARACTER_WALK_TRIGGER_CPP

#include "..\css\CharacterWalkTrigger.h"

bool CharacterWalkTrigger::IsAnyInside() const
{
	return isAnyInside;
}

void CharacterWalkTrigger::SetParent( std::shared_ptr<Object> parent )
{
	this->parent = parent;
}

void CharacterWalkTrigger::NextOverlappingFrame()
{
	Trigger::NextOverlappingFrame();
	isAnyInside = false;
}

void CharacterWalkTrigger::EventOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
	if( other != parent.get() )
	{
		if( persisstentManifold->getNumContacts() > 0 )
		{
			isAnyInside = true;
		}
	}
}

void CharacterWalkTrigger::EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
	Trigger::EventOnObjectBeginOverlapp( other, persisstentManifold );
	this->EventOverlapp( other, persisstentManifold );
}

void CharacterWalkTrigger::EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
	Trigger::EventOnObjectTickOverlapp( other, persisstentManifold );
	this->EventOverlapp( other, persisstentManifold );
}

void CharacterWalkTrigger::EventOnObjectEndOverlapp( Object * other )
{
	Trigger::EventOnObjectEndOverlapp( other );
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

