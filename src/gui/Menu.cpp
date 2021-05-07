
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MENU_CPP
#define MENU_CPP

#include <engine/Engine.h>

#include "Menu.h"

#include <util/StdUtil.hpp>

Menu::Menu() {
}

Menu::~Menu() {
	for(auto element : elements)
		element->remove();
	elements.clear();
}

void Menu::OnEvent(const irr::SEvent::SGUIEvent &event) {
	switch(event.EventType) {
		case irr::gui::EGET_BUTTON_CLICKED:
			OnButtonClicked((Button*)event.Caller, event.Element);
			break;
		default:
			OnOtherEvent(event);
	}
}

void Menu::MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw) {}
void Menu::KeyPressedEvent(int keyCode) {}
void Menu::KeyReleasedEvent(int keyCode) {}
void Menu::KeyHoldedEvent(int keyCode) {}
void Menu::StringToEnterEvent(const std::string& str) {}


void Menu::PutToBackground() {
	enabledElementsStored.clear();
	visiblelementssStored.clear();
	for(auto it : elements) {
		if(it->isEnabled()) {
			enabledElementsStored.insert(it);
			it->setEnabled(false);
		}
		if(it->isVisible()) {
			visiblelementssStored.insert(it);
			it->setVisible(false);
		}
	}
}

void Menu::RestoreFromBackground() {
	for(auto it : enabledElementsStored)
		it->setEnabled(true);
	enabledElementsStored.clear();
	for(auto it : visiblelementssStored)
		it->setVisible(true);
	visiblelementssStored.clear();
}

bool Menu::ReceiveEvents() const {
	return true;
}


void Menu::TickRender() {
}


void Menu::OnOtherEvent(const irr::SEvent::SGUIEvent &event) {
}


Menu::Button* Menu::AddButton(irr::core::rect<int> rect,
		const std::string &text, std::string toolTipText) {
	Menu::Button *button = sing::igui->addButton(rect, NULL, -1,
			std::to_wstring(text).c_str(),
			std::to_wstring(toolTipText).c_str());
	button->setEnabled(true);
	elements.emplace_back(button);
	return button;
}

Menu::Image* Menu::AddImage(std::shared_ptr<Texture> texture, int x, int y) {
	Menu::Image *image = sing::igui->addImage(texture->GetITexture(), {x, y},
			true);
	image->setEnabled(true);
	elements.emplace_back(image);
	return image;
}

Menu::StaticText* Menu::AddStaticText(irr::core::rect<int> rect,
		const std::string &text) {
	Menu::StaticText *staticText = sing::igui->addStaticText(
			std::to_wstring(text).c_str(), rect);
	staticText->setEnabled(true);
	elements.emplace_back(staticText);
	return staticText;
}

Menu::ScrollBar* Menu::AddScrollBar(irr::core::rect<int> rect) {
	Menu::ScrollBar *elem = sing::igui->addScrollBar(false, rect);
	elem->setEnabled(true);
	elements.emplace_back(elem);
	return elem;
}

Menu::ListBox* Menu::AddListBox(irr::core::rect<int> rect) {
	Menu::ListBox *elem = sing::igui->addListBox(rect);
	elem->setEnabled(true);
	elements.emplace_back(elem);
	return elem;
	
}

#endif
