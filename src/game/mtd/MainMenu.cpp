
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MAIN_MENU_CPP
#define MAIN_MENU_CPP

#include "../css/MainMenu.h"
#include <Engine.h>

MainMenu::MainMenu() :
	Menu() {
	startNewGame = AddButton({100,100,300,130}, "Start New Game");
	continueGame = AddButton({100,140,300,170}, "Continue Game");
	options = AddButton({100,180,300,210}, "Options");
	exitGame = AddButton({100,220,300,250}, "Exit Game");
	sing::window->UnlockMouse();
	sing::window->ShowMouse();
	sing::engine->PauseSimulation();
}
MainMenu::~MainMenu() {
	sing::window->LockMouse();
	sing::window->HideMouse();
	sing::engine->ResumeSimulation();
}

bool MainMenu::RenderSceneInBackground() const {
	return false;
}

void MainMenu::KeyReleasedEvent(int keyCode) {
	switch(keyCode) {
	case irr::KEY_ESCAPE:
		sing::window->StopMenu();
		break;
	}
}

void MainMenu::OnOtherEvent(const irr::SEvent::SGUIEvent &event) {
}
void MainMenu::OnButtonClicked(Menu::Button *button, Menu::Element *element) {
	if(button == startNewGame) {
		sing::window->StopMenu();
	} else if(button == continueGame) {
		sing::window->StopMenu();
	} else if(button == exitGame) {
		sing::window->QueueQuit();
	} else if(button == options) {
		//sing::window->StartMenu<OptionsMenu>();
	}
}

#endif
