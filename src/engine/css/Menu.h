
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MENU_H
#define MENU_H

#include <EventResponser.h>

#include <irrlicht/irrlicht.h>

#include <vector>

class Menu : public EventResponser {
public:
	
	typedef irr::gui::IGUIElement Element;
	typedef irr::gui::IGUIButton Button;
	
	Menu(class Engine *engine);
	~Menu();
	
	void OnEvent(const irr::SEvent::SGUIEvent &event);
	
	virtual void MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw) override;
	virtual void KeyPressedEvent(int keyCode) override;
	virtual void KeyReleasedEvent(int keyCode) override;
	virtual void KeyHoldedEvent(int keyCode) override;
	virtual void StringToEnterEvent(std::string str) override;
	
protected:
	
	virtual void OnOtherEvent(const irr::SEvent::SGUIEvent &event);
	virtual void OnButtonClicked(Button *button, Element *element) =0;
	
	Button* AddButton(irr::core::rect<int> rect, const std::string &text, std::string toolTipText="");
	
protected:
	
	std::vector<Element*> elements;
	
	irr::gui::IGUIEnvironment *igui;
};

#endif
