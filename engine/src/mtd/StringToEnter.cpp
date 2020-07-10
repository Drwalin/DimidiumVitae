
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STRING_TO_ENTER_CPP
#define STRING_TO_ENTER_CPP

#include "..\css\EventResponser.h"
#include "..\css\StringToEnter.h"
#include "..\css\Window.h"

void StringToEnter::SetWindow(Window *window) {
	this->window = window;
}

std::string StringToEnter::GetCurrent() {
	return this->str;
}

void StringToEnter::Clear() {
	this->str.clear();
	this->currentPosition = 0;
}

void StringToEnter::InsertCharacter(char character) {
	if(this->str.size() < this->charactersLimit) {
		this->str.insert(this->currentPosition, 1, character);
		++(this->currentPosition);
	}
}

void StringToEnter::PressKey(const irr::SEvent::SKeyInput &key) {
	switch(key.Key) {
	case irr::KEY_LEFT:
		--this->currentPosition;
		if(this->currentPosition < 0)
			this->currentPosition = this->str.size();
		break;
	case irr::KEY_RIGHT:
		++this->currentPosition;
		if(this->currentPosition > this->str.size())
			this->currentPosition = 0;
		break;
	case irr::KEY_DELETE:
		if(this->currentPosition < this->str.size()) {
			this->str.erase(this->str.begin() + this->currentPosition);
		}
		break;
	case irr::KEY_BACK:			// backspace
		if(this->currentPosition > 0) {
			--(this->currentPosition);
			this->str.erase(this->str.begin() + this->currentPosition);
		}
		break;
	case irr::KEY_RETURN:		// enter
		if(this->window->GetEventResponser()) {
			this->window->GetEventResponser()->StringToEnterEvent(this->str);
		}
		this->Clear();
		break;
	case irr::KEY_ESCAPE:
		this->Clear();
		break;
	case irr::KEY_HOME:
		this->currentPosition = 0;
		break;
	case irr::KEY_END:
		this->currentPosition = str.size();
		break;
	default:
		if(key.Char != 0) {
			this->InsertCharacter(key.Char);
		}
	}
}

void StringToEnter::SetCharactersLimit(unsigned value) {
	this->charactersLimit = value;
	if(this->charactersLimit < 1024*4)
		this->charactersLimit = 1024*4;
}

StringToEnter::StringToEnter() {
	this->charactersLimit = 1024*4;
	this->window = NULL;
	this->str = "";
	this->currentPosition = 0;
}

StringToEnter::~StringToEnter() {
	this->charactersLimit = 0;
	this->window = NULL;
	this->str = "";
	this->currentPosition = 0;
	this->cooldownToUseKey.clear();
}

#endif
