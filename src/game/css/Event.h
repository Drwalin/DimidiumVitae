
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_H
#define EVENT_H

#include <EventResponser.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <string>

#include <ctime>

#include <memory>

class Event : public EventResponser  {
public:
	
	Event();
	~Event();
	
	virtual void MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw)
			override;
	virtual void KeyPressedEvent(int keyCode) override;
	virtual void KeyReleasedEvent(int keyCode) override;
	virtual void KeyHoldedEvent(int keyCode) override;
	virtual void StringToEnterEvent(const std::string& str) override;
};

#endif
