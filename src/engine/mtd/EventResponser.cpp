
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RESPONSER_CPP
#define EVENT_RESPONSER_CPP

#include "../css/EventResponser.h"

class Window *EventResponser::GetWindow() {
	return window;
}

void EventResponser::SetEngine(class Engine *engine) {
	this->engine = engine;
}

void EventResponser::SetWindow(Window *window) {
	this->window = window;
}

EventResponser::EventResponser() {
	window = NULL;
}

EventResponser::~EventResponser() {
	window = NULL;
}

#endif
