
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_WALK_TRIGGER_CPP
#define CHARACTER_WALK_TRIGGER_CPP

#include "..\css\CharacterWalkTrigger.h"

bool CharacterWalkTrigger::IsTopCollision() const
{
	return this->topCollision;
}

bool CharacterWalkTrigger::IsSideCollision() const
{
	return this->sideCollision;
}

bool CharacterWalkTrigger::IsBottomCollision() const
{
	return this->bottomCollision;
}

void CharacterWalkTrigger::Init( std::shared_ptr<Entity> parent, float stepHeight )
{
	this->parent = parent;
	this->stepHeight = stepHeight;
}

void CharacterWalkTrigger::NextOverlappingFrame()
{
	Trigger::NextOverlappingFrame();
	this->topCollision = false;
	this->sideCollision = false;
	this->bottomCollision = false;
	
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
	if( rigidBody )
	{
		btVector3 min, max;
		rigidBody->getAabb( min, max );
		this->bottom = min.y();
		this->top = max.y();
	}
	else
	{
		this->bottom = this->top = this->GetTransform().getOrigin().y();
		MESSAGE( std::string("CharacterWalkTrigger::body is not btRigidBody (") + this->name + ")" );
	}
}

void CharacterWalkTrigger::EventOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	if( other != parent.get() )
	{
		for( int i=0; i<persisstentManifold->getNumContacts(); ++i )
		{
			btManifoldPoint manifoldPoint = persisstentManifold->getContactPoint( i );
			btVector3 contactPoint = (manifoldPoint.m_positionWorldOnB + manifoldPoint.m_positionWorldOnA) * 0.5f;
			btVector3 normal = manifoldPoint.m_normalWorldOnB.normalized();
			if( normal.y() < 0.0f )
				normal.setY( -normal.y() );
			bool isNormalVertical = normal.dot( btVector3(0,1,0) ) > 0.8f;
			
			if( isNormalVertical )
			{
				if( this->bottom + this->stepHeight >= contactPoint.y() )
					this->bottomCollision = true;
				if( this->top - this->stepHeight <= contactPoint.y() )
					this->topCollision = true;
			}
			else
				this->sideCollision = true;
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
	this->SetTransform( this->parent->GetTransform() );
}


void CharacterWalkTrigger::Load( std::istream & stream )
{
	Trigger::Load( stream );
}

void CharacterWalkTrigger::Save( std::ostream & stream ) const
{
	Trigger::Save( stream );
}

void CharacterWalkTrigger::Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Trigger::Spawn( self, name, shape, transform );
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
}

CharacterWalkTrigger::~CharacterWalkTrigger()
{
}

#endif

