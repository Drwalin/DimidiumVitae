
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "../css/Player.h"

#include <sstream>
#include <string>
#include <fstream>

#include <ctime>

void Player::Tick(float deltaTime) {
	Character::Tick(deltaTime);
}

Player::Player(const JSON& json) :
	Character(json) {
}

Player::~Player() {
}

__ENTITY_DERIVED_CODE_FACTORY__(Player)

#endif

