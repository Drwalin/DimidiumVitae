
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "..\css\Player.h"

#include <Debug.h>
#include <Math.h>


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

Player::Player( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
	Character( engine, name, body, collisionShape, mass_ )
{
}

Player::Player() :
	Character()
{
}

Player::~Player()
{
}

#endif

