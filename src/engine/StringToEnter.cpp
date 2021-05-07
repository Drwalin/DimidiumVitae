
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STRING_TO_ENTER_CPP
#define STRING_TO_ENTER_CPP

#include "EventResponser.h"
#include "StringToEnter.h"
#include "Window.h"

const std::string& StringToEnter::GetCurrent() {
	return str;
}

void StringToEnter::SetCurrent(const std::string& str) {
	this->str = str;
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
		if(currentPosition > (int)str.size())
			currentPosition = 0;
		break;
	case irr::KEY_DELETE:
		if(currentPosition < (int)str.size())
			str.erase(str.begin() + currentPosition);
		break;
	case irr::KEY_BACK:			// backspace
		if(currentPosition > 0) {
			--(currentPosition);
			str.erase(str.begin() + currentPosition);
		}
		break;
	case irr::KEY_RETURN:		// enter
		if(!key.Shift) {
			EventResponser *currentEventResponser =
					sing::window->GetActiveEventResponser();
			if(currentEventResponser)
				currentEventResponser->StringToEnterEvent(str);
			Clear();
		} else {
			InsertCharacter('\n');
		}
		break;
	case irr::KEY_ESCAPE:
		Clear();
		break;
	case irr::KEY_HOME:
		currentPosition = 0;
		break;
	case irr::KEY_END:
		currentPosition = (int)str.size();
		break;
	default:
		if(key.Key == 'W' && key.Control) {
			for(; currentPosition>0;) {
				if(str[currentPosition-1]==' ' ||
						str[currentPosition-1]=='\t' ||
						str[currentPosition-1]=='\n' ||
						str[currentPosition-1]=='\r')
					break;
				--(currentPosition);
				str.erase(str.begin() + currentPosition);
			}
		} else if(key.Key == 'V' && key.Control) {
			std::string clipboard =
				sing::device->getOSOperator()->getTextFromClipboard();
			str.insert(str.begin()+currentPosition, clipboard.begin(), clipboard.end());
			currentPosition += clipboard.size();
		} else if(key.Char && key.Char!=0 && key.Char!='\n' && key.Char!=-1)
			InsertCharacter(key.Char);
	}
}

void StringToEnter::SetCharactersLimit(unsigned value) {
	charactersLimit = value;
	if(charactersLimit < 1024*4)
		charactersLimit = 1024*4;
}

StringToEnter::StringToEnter() {
	charactersLimit = 1024*256;
	str = "";
	str.reserve(charactersLimit);
	currentPosition = 0;
}

StringToEnter::~StringToEnter() {
	charactersLimit = 0;
	str = "";
	currentPosition = 0;
	cooldownToUseKey.clear();
}

#endif
