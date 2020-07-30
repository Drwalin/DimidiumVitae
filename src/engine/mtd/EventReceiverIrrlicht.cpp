
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RECEIVER_IRRLICHT_CPP
#define EVENT_RECEIVER_IRRLICHT_CPP

#include "../css/EventReceiverIrrlicht.h"
#include "StringToEnter.h"

#include <cstdio>

void EventReceiverIrrlicht::SetCursor(int x, int y) {
	this->mouseX = x;
	this->mouseY = y;
}

void EventReceiverIrrlicht::GetCursor(int &x, int &y) {
	x = this->mouseX;
	y = this->mouseY;
}

bool EventReceiverIrrlicht::OnEvent(const irr::SEvent& event) {
	this->queueMutex.lock();
	this->eventQueue.resize(this->eventQueue.size() + 1);
	this->eventQueue.back() = event;
	this->queueMutex.unlock();
	if(event.EventType == irr:: EET_LOG_TEXT_EVENT)
		return true;
	return false;
}

void EventReceiverIrrlicht::GenerateOneEvent(const irr::SEvent& event) {
	EventResponser *currentEventResponser = eventResponser;
	if(window->GetCurrentMenu()) {
		currentEventResponser = window->GetCurrentMenu();
	}
	switch(event.EventType) {
	case irr::EET_KEY_INPUT_EVENT:
		if(event.KeyInput.PressedDown) {
			if(this->keyHolded.find(event.KeyInput.Key) == this->keyHolded.end()) {
				if(event.KeyInput.Char)
					this->window->GetStringToEnterObject()->PressKey(event.KeyInput);
				currentEventResponser->KeyPressedEvent(event.KeyInput.Key);
				this->keyPressed.insert(event.KeyInput.Key);
			}
		} else {
			currentEventResponser->KeyReleasedEvent(event.KeyInput.Key);
			this->keyHolded.erase(event.KeyInput.Key);
			this->keyPressed.erase(event.KeyInput.Key);
		}
		break;
	case irr::EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event) {
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			if(this->keyHolded.find(irr::KEY_LBUTTON) == this->keyHolded.end()) {
				currentEventResponser->KeyPressedEvent(irr::KEY_LBUTTON);
				this->keyPressed.insert(irr::KEY_LBUTTON);
			}
			break;
		case irr::EMIE_RMOUSE_PRESSED_DOWN:
			if(this->keyHolded.find(irr::KEY_RBUTTON) == this->keyHolded.end()) {
				currentEventResponser->KeyPressedEvent(irr::KEY_RBUTTON);
				this->keyPressed.insert(irr::KEY_RBUTTON);
			}
			break;
		case irr::EMIE_MMOUSE_PRESSED_DOWN:
			if(this->keyHolded.find(irr::KEY_MBUTTON) == this->keyHolded.end()) {
				currentEventResponser->KeyPressedEvent(irr::KEY_MBUTTON);
				this->keyPressed.insert(irr::KEY_MBUTTON);
			}
			break;
			
		case irr::EMIE_LMOUSE_LEFT_UP:
			currentEventResponser->KeyReleasedEvent(irr::KEY_LBUTTON);
			this->keyHolded.erase(irr::KEY_LBUTTON);
			this->keyPressed.erase(irr::KEY_LBUTTON);
			break;
		case irr::EMIE_RMOUSE_LEFT_UP:
			currentEventResponser->KeyReleasedEvent(irr::KEY_RBUTTON);
			this->keyHolded.erase(irr::KEY_RBUTTON);
			this->keyPressed.erase(irr::KEY_RBUTTON);
			break;
		case irr::EMIE_MMOUSE_LEFT_UP:
			currentEventResponser->KeyReleasedEvent(irr::KEY_MBUTTON);
			this->keyHolded.erase(irr::KEY_MBUTTON);
			this->keyPressed.erase(irr::KEY_MBUTTON);
			break;
			
		case irr::EMIE_MOUSE_WHEEL:
		case irr::EMIE_MOUSE_MOVED:
			currentEventResponser->MouseMoveEvent(event.MouseInput.X, event.MouseInput.Y, this->mouseW + event.MouseInput.Wheel, event.MouseInput.X - this->mouseX, event.MouseInput.Y - this->mouseY, event.MouseInput.Wheel);
			this->mouseW += event.MouseInput.Wheel;
			this->mouseX = event.MouseInput.X;
			this->mouseY = event.MouseInput.Y;
			break;
		}
		break;
	case irr::EET_GUI_EVENT:
		if(window->GetCurrentMenu())
			window->GetCurrentMenu()->OnEvent(event.GUIEvent);
		break;
	}
}

void EventReceiverIrrlicht::GenerateEvents() {
	std::vector < irr::SEvent > eventQueue;
	
	if(this->eventQueue.size() > 0) {
		this->queueMutex.lock();
		eventQueue = this->eventQueue;
		this->eventQueue.clear();
		this->queueMutex.unlock();
	}
	
	for(unsigned i = 0; i < eventQueue.size(); ++i) {
		this->GenerateOneEvent(eventQueue[i]);
	}
	
	eventQueue.clear();
	
	for(auto it = this->keyHolded.begin(); it != this->keyHolded.end(); ++it) {
		this->eventResponser->KeyHoldedEvent(*it);
	}
	
	this->keyHolded.insert(this->keyPressed.begin(), this->keyPressed.end());		// ????????????????
}

EventReceiverIrrlicht::EventReceiverIrrlicht(EventResponser *event, Window *window) {
	this->mouseW = 0;
	this->mouseX = 0;
	this->mouseY = 0;
	this->eventResponser = event;
	this->window = window;
}

EventReceiverIrrlicht::~EventReceiverIrrlicht() {
	this->mouseW = 0;
	this->mouseX = 0;
	this->mouseY = 0;
	this->eventQueue.clear();
	this->eventQueue.shrink_to_fit();
	this->eventResponser = NULL;
	this->window = NULL;
}

#endif
