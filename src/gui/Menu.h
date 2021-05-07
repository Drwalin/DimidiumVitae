
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MENU_H
#define MENU_H

#include <engine/EventResponser.h>

#include <resources/Texture.h>

#include <irrlicht/irrlicht.h>

#include <vector>
#include <set>

class Menu : public EventResponser {
public:
	
	typedef irr::gui::IGUIElement Element;
	typedef irr::gui::IGUIButton Button;
	typedef irr::gui::IGUIImage Image;
	typedef irr::gui::IGUIStaticText StaticText;
	typedef irr::gui::IGUIScrollBar ScrollBar;
	typedef irr::gui::IGUIListBox ListBox;
	
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
	
	virtual void PutToBackground();
	virtual void RestoreFromBackground();
	
	virtual bool ReceiveEvents() const;
	virtual void TickRender();
	
protected:
	
	virtual void OnOtherEvent(const irr::SEvent::SGUIEvent &event);
	virtual void OnButtonClicked(Button *button, Element *element) =0;
	
	Button* AddButton(irr::core::rect<int> rect, const std::string &text,
			std::string toolTipText="");
	Image* AddImage(std::shared_ptr<Texture> texture, int x, int y);
	StaticText* AddStaticText(irr::core::rect<int> rect,
			const std::string &text);
	ScrollBar* AddScrollBar(irr::core::rect<int> rect);
	ListBox* AddListBox(irr::core::rect<int> rect);
	
protected:
	
	std::vector<Element*> elements;
	std::set<Element*> enabledElementsStored, visiblelementssStored;
	
	irr::gui::IGUIEnvironment *igui;
};

#endif
