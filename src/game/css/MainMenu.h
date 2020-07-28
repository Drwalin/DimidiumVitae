
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <Menu.h>

class MainMenu : public Menu {
public:
	
	MainMenu(class Engine *engine);
	~MainMenu();
	
	virtual void KeyReleasedEvent(int keyCode) override;
	
protected:
	
	virtual void OnOtherEvent(const irr::SEvent::SGUIEvent &event) override;
	virtual void OnButtonClicked(Menu::Button *button, Menu::Element *element) override;
	
protected:
	
	Menu::Button *startNewGame;
	Menu::Button *continueGame;
	Menu::Button *exitGame;
	Menu::Button *options;
};

#endif
