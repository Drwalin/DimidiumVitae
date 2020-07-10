
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STRING_TO_ENTER_H
#define STRING_TO_ENTER_H

#include <irrlicht\irrlicht.h>

#include <string>
#include <map>

class StringToEnter
{
private:
	
	class Window * window;
	
	std::string str;
	int currentPosition;
	std::map < int, float > cooldownToUseKey;
	
	unsigned charactersLimit;
	
	
	void InsertCharacter( char character );
	
public:
	
	void SetCharactersLimit( unsigned value );
	
	void SetWindow( class Window * window );
	
	void PressKey( const irr::SEvent::SKeyInput & key );
	
	void Clear();
	std::string GetCurrent();
	
	StringToEnter();
	~StringToEnter();
};

#endif

