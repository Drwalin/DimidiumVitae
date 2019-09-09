
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RESPONSER_CPP
#define EVENT_RESPONSER_CPP

#include "..\css\EventResponser.h"

void EventResponser::SetEngine( class Engine * engine )
{
	this->engine = engine;
}

void EventResponser::Init()
{
	window = (Window*)( GetBasicWindow() );
}

BasicWindow * EventResponser::GetBasicWindow()
{
	return basicWindow;
}

void EventResponser::SetBasicWindow( BasicWindow * basicWindow )
{
	this->basicWindow = basicWindow;
}

EventResponser::EventResponser()
{
	basicWindow = NULL;
}

EventResponser::~EventResponser()
{
	basicWindow = NULL;
}

#endif

