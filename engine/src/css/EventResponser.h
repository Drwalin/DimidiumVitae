
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RESPONSER_H
#define EVENT_RESPONSER_H

#include <map>
#include <string>

class EventResponser
{
protected:
	
	class Window * window;
	class Engine * engine;
	
public:
	
	class Window * GetWindow();
	void SetWindow( class Window * window );
	
	void SetEngine( class Engine * engine );
	
	virtual void MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw ) = 0;
	virtual void KeyPressedEvent( int keyCode ) = 0;
	virtual void KeyReleasedEvent( int keyCode ) = 0;
	virtual void KeyHoldedEvent( int keyCode ) = 0;
	virtual void StringToEnterEvent( std::string str ) = 0;
	
	EventResponser();
	~EventResponser();
};

#endif

