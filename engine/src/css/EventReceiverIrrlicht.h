
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RECEIVER_IRRLICHT_H
#define EVENT_RECEIVER_IRRLICHT_H

#include <irrlicht\irrlicht.h>

#include <atomic>
#include <mutex>
#include <vector>
#include <set>

#include "EventResponser.h"
#include "Window.h"

class EventReceiverIrrlicht : public irr::IEventReceiver
{
private:
	
	std::vector < irr::SEvent > eventQueue;
	std::mutex queueMutex;
	
	EventResponser * eventResponser;
	Window * window;
	
	int mouseX, mouseY, mouseW;
	
	std::set < unsigned > keyHolded;
	std::set < unsigned > keyPressed;
	
	void GenerateOneEvent( const irr::SEvent& event );
	
public:
	
	void SetCursor( int x, int y );
	void GetCursor( int & x, int & y );
	
	virtual bool OnEvent( const irr::SEvent& event ) override;
	
	void GenerateEvents();
	
	EventReceiverIrrlicht( EventResponser * event, Window * window );
	~EventReceiverIrrlicht();
};

#endif

