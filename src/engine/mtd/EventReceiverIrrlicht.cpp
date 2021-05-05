
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef EVENT_RECEIVER_IRRLICHT_CPP
#define EVENT_RECEIVER_IRRLICHT_CPP

#include "../css/EventReceiverIrrlicht.h"
#include "StringToEnter.h"

#include <cstdio>

void EventReceiverIrrlicht::SetCursor(int x, int y) {
	mouseX = x;
	mouseY = y;
}

void EventReceiverIrrlicht::GetCursor(int &x, int &y) {
	x = mouseX;
	y = mouseY;
}

bool EventReceiverIrrlicht::OnEvent(const irr::SEvent& event) {
	queueMutex.lock();
	eventQueue.resize(eventQueue.size() + 1);
	eventQueue.back() = event;
	queueMutex.unlock();
	if(event.EventType == irr::EET_LOG_TEXT_EVENT)
		return true;
	return false;
}

void EventReceiverIrrlicht::GenerateOneEvent(const irr::SEvent& event) {
	EventResponser *currentEventResponser = eventResponser;
	if(sing::window->GetCurrentMenu())
		currentEventResponser = sing::window->GetCurrentMenu();
	if(eventResponser == NULL)
		MESSAGE("Event responser does not exists");
	switch(event.EventType) {
	case irr::EET_KEY_INPUT_EVENT:
		if(event.KeyInput.PressedDown) {
			if(keyHolded.find(event.KeyInput.Key) == keyHolded.end()) {
				if(event.KeyInput.Char)
					sing::window->GetStringToEnterObject()->PressKey(
							event.KeyInput);
				currentEventResponser->KeyPressedEvent(event.KeyInput.Key);
				keyPressed.insert(event.KeyInput.Key);
			}
		} else {
			currentEventResponser->KeyReleasedEvent(event.KeyInput.Key);
			keyHolded.erase(event.KeyInput.Key);
			keyPressed.erase(event.KeyInput.Key);
		}
		break;
	case irr::EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event) {
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			if(keyHolded.find(irr::KEY_LBUTTON) == keyHolded.end()) {
				currentEventResponser->KeyPressedEvent(irr::KEY_LBUTTON);
				keyPressed.insert(irr::KEY_LBUTTON);
			}
			break;
		case irr::EMIE_RMOUSE_PRESSED_DOWN:
			if(keyHolded.find(irr::KEY_RBUTTON) == keyHolded.end()) {
				currentEventResponser->KeyPressedEvent(irr::KEY_RBUTTON);
				keyPressed.insert(irr::KEY_RBUTTON);
			}
			break;
		case irr::EMIE_MMOUSE_PRESSED_DOWN:
			if(keyHolded.find(irr::KEY_MBUTTON) == keyHolded.end()) {
				currentEventResponser->KeyPressedEvent(irr::KEY_MBUTTON);
				keyPressed.insert(irr::KEY_MBUTTON);
			}
			break;
			
		case irr::EMIE_LMOUSE_LEFT_UP:
			currentEventResponser->KeyReleasedEvent(irr::KEY_LBUTTON);
			keyHolded.erase(irr::KEY_LBUTTON);
			keyPressed.erase(irr::KEY_LBUTTON);
			break;
		case irr::EMIE_RMOUSE_LEFT_UP:
			currentEventResponser->KeyReleasedEvent(irr::KEY_RBUTTON);
			keyHolded.erase(irr::KEY_RBUTTON);
			keyPressed.erase(irr::KEY_RBUTTON);
			break;
		case irr::EMIE_MMOUSE_LEFT_UP:
			currentEventResponser->KeyReleasedEvent(irr::KEY_MBUTTON);
			keyHolded.erase(irr::KEY_MBUTTON);
			keyPressed.erase(irr::KEY_MBUTTON);
			break;
			
		case irr::EMIE_MOUSE_WHEEL:
		case irr::EMIE_MOUSE_MOVED:
			currentEventResponser->MouseMoveEvent(event.MouseInput.X,
					event.MouseInput.Y, mouseW + event.MouseInput.Wheel,
					event.MouseInput.X - mouseX, event.MouseInput.Y - mouseY,
					event.MouseInput.Wheel);
			mouseW += event.MouseInput.Wheel;
			mouseX = event.MouseInput.X;
			mouseY = event.MouseInput.Y;
			break;
		default:
			break;
		}
		break;
	case irr::EET_GUI_EVENT:
		if(sing::window->GetCurrentMenu())
			sing::window->GetCurrentMenu()->OnEvent(event.GUIEvent);
		break;
	default:
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
		GenerateOneEvent(eventQueue[i]);
	}
	
	eventQueue.clear();
	
	for(auto it = keyHolded.begin(); it != keyHolded.end(); ++it) {
		eventResponser->KeyHoldedEvent(*it);
	}
	
	keyHolded.insert(keyPressed.begin(), keyPressed.end());
}

EventReceiverIrrlicht::EventReceiverIrrlicht(EventResponser *event) {
	mouseW = 0;
	mouseX = 0;
	mouseY = 0;
	this->eventResponser = event;
}

EventReceiverIrrlicht::~EventReceiverIrrlicht() {
	mouseW = 0;
	mouseX = 0;
	mouseY = 0;
	eventQueue.clear();
	eventQueue.shrink_to_fit();
	eventResponser = NULL;
}

#endif
