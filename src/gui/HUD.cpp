
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef HUD_CPP
#define HUD_CPP

#include "GUI.h"

#include <engine/Engine.h>

HUD::HUD() :
	Menu() {
	sing::window->UnlockMouse();
	sing::window->ShowMouse();
	sing::engine->PauseSimulation();
}

HUD::~HUD() {
	sing::window->LockMouse();
	sing::window->HideMouse();
	sing::engine->ResumeSimulation();
}

bool HUD::RenderSceneInBackground() const {
	return true;
}

void HUD::PutToBackground() {
	Menu::PutToBackground();
	sing::engine->ResumeSimulation();
}

void HUD::RestoreFromBackground() {
	Menu::RestoreFromBackground();
	sing::window->UnlockMouse();
	sing::window->ShowMouse();
	sing::engine->PauseSimulation();
}

bool HUD::ReceiveEvents() const {
	return false;
}

void HUD::TickRender() {

}

#endif

