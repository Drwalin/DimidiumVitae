
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "..\css\Player.h"

#include <Debug.h>
#include <Math.h>

#include <sstream>
#include <string>
#include <fstream>

#include <ctime>

void Player::EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * perisstentManifold )
{
	Character::EventOnObjectBeginOverlapp( other, perisstentManifold );
}

void Player::EventOnObjectTickOverlapp( Object * other, btPersistentManifold * perisstentManifold )
{
	Character::EventOnObjectTickOverlapp( other, perisstentManifold );
}

void Player::EventOnObjectEndOverlapp( Object * other )
{
	Character::EventOnObjectEndOverlapp( other );
}


void Player::Tick( const float deltaTime )
{
	Character::Tick( deltaTime );
}







void Player::Load( std::istream & stream )
{
	Character::Load( stream );
}

void Player::Save( std::ostream & stream ) const
{
	Character::Save( stream );
}

void Player::Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Character::Spawn( name, shape, transform );
}

void Player::Despawn()
{
	Character::Despawn();
}

void Player::Destroy()
{
	Character::Destroy();
}

extern "C" std::shared_ptr<Object> GetClassInstantiator(){ static std::shared_ptr<Object> instantiator( new Player(), [](Object * ptr){delete ptr;} ); return instantiator; }
int Player::GetTypeSize() const{ return sizeof(Player); }
void Player::Free(){ delete this; }
std::shared_ptr<Object> Player::New() const{ return std::dynamic_pointer_cast<Object>( std::shared_ptr<Player>( new Player(), [](Object * ptr){delete ptr;} ) ); }
std::string Player::GetClassName() const{ return "Player"; }

Player::Player() : Character()
{
}

Player::~Player()
{
}

#endif

