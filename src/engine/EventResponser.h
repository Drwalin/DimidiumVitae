
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RESPONSER_H
#define EVENT_RESPONSER_H

#include <map>
#include <string>

namespace irr {
	const int KEY_TILDE = 192;
}

class EventResponser  {
public:
	
	EventResponser();
	virtual ~EventResponser();
	
	virtual void MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw)
			= 0;
	virtual void KeyPressedEvent(int keyCode) = 0;
	virtual void KeyReleasedEvent(int keyCode) = 0;
	virtual void KeyHoldedEvent(int keyCode) = 0;
	virtual void StringToEnterEvent(const std::string& str) = 0;
};

#endif

