
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RESPONSER_H
#define EVENT_RESPONSER_H

#include <map>
#include <string>

class EventResponser
{
private:
	
	class BasicWindow * basicWindow;
	
protected:
	
	class Window * window;
	class Engine * engine;
	class BasicWindow * GetBasicWindow();
	
public:
	
	void SetEngine( class Engine * engine );
	void Init();
	
	void SetBasicWindow( class BasicWindow * basicWindow );
	
	virtual void MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw ) = 0;
	virtual void KeyPressedEvent( int keyCode ) = 0;
	virtual void KeyReleasedEvent( int keyCode ) = 0;
	virtual void KeyHoldedEvent( int keyCode ) = 0;
	virtual void StringToEnterEvent( std::string str ) = 0;
	
	EventResponser();
	~EventResponser();
};

#endif

