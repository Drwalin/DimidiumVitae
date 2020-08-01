
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "../css/Player.h"

#include <sstream>
#include <string>
#include <fstream>

#include <ctime>

void Player::Tick(const float deltaTime) {
	Character::Tick(deltaTime);
}

Player::Player(uint64_t id, std::shared_ptr<CollisionShape> shape, btTransform transform) :
	Character(id, shape, transform) {
}

Player::~Player() {
}

__ENTITY_DERIVED_CODE_FACTORY__(Player)

#endif

