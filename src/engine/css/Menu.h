
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MENU_H
#define MENU_H

#include <EventResponser.h>
#include <Texture.h>

#include <irrlicht/irrlicht.h>

#include <vector>

class Menu : public EventResponser {
public:
	
	typedef irr::gui::IGUIElement Element;
	typedef irr::gui::IGUIButton Button;
	typedef irr::gui::IGUIImage Image;
	typedef irr::gui::IGUIStaticText StaticText;
	
	Menu();
	virtual ~Menu();
	
	virtual bool RenderSceneInBackground() const =0;
	
	void OnEvent(const irr::SEvent::SGUIEvent &event);
	
	virtual void MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw)
			override;
	virtual void KeyPressedEvent(int keyCode) override;
	virtual void KeyReleasedEvent(int keyCode) override;
	virtual void KeyHoldedEvent(int keyCode) override;
	virtual void StringToEnterEvent(const std::string& str) override;
	
protected:
	
	virtual void OnOtherEvent(const irr::SEvent::SGUIEvent &event);
	virtual void OnButtonClicked(Button *button, Element *element) =0;
	
	Button* AddButton(irr::core::rect<int> rect, const std::string &text,
			std::string toolTipText="");
	Image* AddImage(std::shared_ptr<Texture> texture, int x, int y);
	StaticText* AddStaticText(irr::core::rect<int> rect,
			const std::string &text);
	
protected:
	
	std::vector<Element*> elements;
	
	irr::gui::IGUIEnvironment *igui;
};

#endif
