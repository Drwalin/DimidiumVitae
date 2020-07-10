
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "..\css\Player.h"

#include <sstream>
#include <string>
#include <fstream>

#include <ctime>

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

void Player::Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Character::Spawn( self, name, shape, transform );
}

void Player::Despawn()
{
	Character::Despawn();
}

void Player::Destroy()
{
	Character::Destroy();
}

extern "C" std::shared_ptr<Entity> GetPlayerInstantiator(){ static std::shared_ptr<Entity> instantiator( new Player(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int Player::GetTypeSize() const{ return sizeof(Player); }
void Player::Free(){ delete this; }
std::shared_ptr<Entity> Player::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<Player>( new Player(), [](Entity * ptr){delete ptr;} ) ); }
std::string Player::GetClassName() const{ return "Player"; }

Player::Player() : Character()
{
}

Player::~Player()
{
}

#endif

