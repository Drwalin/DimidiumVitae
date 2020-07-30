
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STRING_TO_ENTER_CPP
#define STRING_TO_ENTER_CPP

#include "../css/EventResponser.h"
#include "../css/StringToEnter.h"
#include "../css/Window.h"

void StringToEnter::SetWindow(Window *window) {
	this->window = window;
}

std::string StringToEnter::GetCurrent() {
	return str;
}

void StringToEnter::Clear() {
	str.clear();
	currentPosition = 0;
}

void StringToEnter::InsertCharacter(char character) {
	if(str.size() < charactersLimit) {
		str.insert(currentPosition, 1, character);
		++(currentPosition);
	}
}

void StringToEnter::PressKey(const irr::SEvent::SKeyInput &key) {
	switch(key.Key) {
	case irr::KEY_LEFT:
		--currentPosition;
		if(currentPosition < 0)
			currentPosition = str.size();
		break;
	case irr::KEY_RIGHT:
		++currentPosition;
		if(currentPosition > str.size())
			currentPosition = 0;
		break;
	case irr::KEY_DELETE:
		if(currentPosition < str.size()) {
			str.erase(str.begin() + currentPosition);
		}
		break;
	case irr::KEY_BACK:			// backspace
		if(currentPosition > 0) {
			--(currentPosition);
			str.erase(str.begin() + currentPosition);
		}
		break;
	case irr::KEY_RETURN:		// enter
		if(window->GetEventResponser()) {
			window->GetEventResponser()->StringToEnterEvent(str);
		}
		Clear();
		break;
	case irr::KEY_ESCAPE:
		Clear();
		break;
	case irr::KEY_HOME:
		currentPosition = 0;
		break;
	case irr::KEY_END:
		currentPosition = str.size();
		break;
	default:
		if(key.Char != 0) {
			InsertCharacter(key.Char);
		}
	}
}

void StringToEnter::SetCharactersLimit(unsigned value) {
	charactersLimit = value;
	if(charactersLimit < 1024*4)
		charactersLimit = 1024*4;
}

StringToEnter::StringToEnter() {
	charactersLimit = 1024*4;
	window = NULL;
	str = "";
	currentPosition = 0;
}

StringToEnter::~StringToEnter() {
	charactersLimit = 0;
	window = NULL;
	str = "";
	currentPosition = 0;
	cooldownToUseKey.clear();
}

#endif
