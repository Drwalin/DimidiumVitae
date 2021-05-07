
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CONSOLE_CPP
#define CONSOLE_CPP

#include "Console.h"

#include <engine/Engine.h>
#include <engine/Window.h>
#include <engine/StringToEnter.h>

#include <util/StdUtil.hpp>

Console::Console() :
	Menu() {
	state = sing::engine->StoreSimulationExecution();
	sing::engine->PauseSimulation();
	
	log = AddStaticText({0, 0, 0, 0}, "");
	log->setRelativePositionProportional({0.0f, 0.0f, 0.985f, 0.9f});
	log->setBackgroundColor({200, 0, 0, 0});
	log->setOverrideColor({255, 255, 255, 255});
	log->setDrawBorder(true);
	
	input = AddStaticText({0, 0, 0, 0}, "");
	input->setRelativePositionProportional({0.0f, 0.9f, 1.0f, 1.0f});
	input->setBackgroundColor({200, 0, 0, 0});
	input->setOverrideColor({255, 255, 255, 255});
	input->setDrawBorder(true);
	
	scrollBar = AddScrollBar({0, 0, 0, 0});
	scrollBar->setRelativePositionProportional({0.985f, 0.0f, 1.0f, 0.9f});
	scrollBar->setMin(0);
	
	stringGetter = sing::window->GetStringToEnterObject();
	stringGetter->Clear();
	
	scroll = 0;
	Update();
}

Console::~Console() {
	sing::engine->RestoreSimulationExecution(state);
}

bool Console::RenderSceneInBackground() const {
	return true;
}

void Console::MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw) {
	Update();
}

void Console::KeyReleasedEvent(int keyCode) {
	Update();
	switch(keyCode) {
	case irr::KEY_ESCAPE:
		sing::window->StopMenu();
		break;
	case irr::KEY_TILDE:
		sing::window->StopMenu();
		break;
	}
}

void Console::KeyHoldedEvent(int keyCode) {
	Update();
}

void Console::KeyPressedEvent(int keyCode) {
	switch(keyCode) {
	case irr::KEY_UP:
		commandsHistory++;
		if(commandsHistory > (int)CommandsHistory().size())
			commandsHistory = CommandsHistory().size();
		else if(commandsHistory > 0) {
			sing::window->GetStringToEnterObject()->SetCurrent(
					CommandsHistory()[commandsHistory-1]);
		} else {
			sing::window->GetStringToEnterObject()->SetCurrent("");
		}
		break;
	case irr::KEY_DOWN:
		commandsHistory--;
		if(commandsHistory < 0)
			commandsHistory = 0;
		else if(commandsHistory > 0) {
			sing::window->GetStringToEnterObject()->SetCurrent(
					CommandsHistory()[commandsHistory-1]);
		} else {
			sing::window->GetStringToEnterObject()->SetCurrent("");
		}
		break;
	}
	Update();
}

void Console::StringToEnterEvent(const std::string& str) {
	sing::commandInterpreter->InvokeCommand(str);
	CommandsHistory().emplace_back(str);
	commandsHistory = 0;
	Update();
}

void Console::OnOtherEvent(const irr::SEvent::SGUIEvent &event) {
	Update();
}

void Console::OnButtonClicked(Menu::Button *button, Menu::Element *element) {
	Update();
}

void Console::Update() {
	log->setRelativePositionProportional({0.0f, 0.0f, 1.0f, 0.9f});
	input->setRelativePositionProportional({0.0f, 0.9f, 1.0f, 1.0f});
	scrollBar->setRelativePositionProportional({0.985f, 0.0f, 1.0f, 0.9f});
	
	int size = AccessLog().size();
	scrollBar->setMax(size);
	scroll = scrollBar->getPos();
	if(scroll >= size)
		scroll = size-1;
	if(scroll < 0)
		scroll = 0;
	
	std::wstring first = GetLogsText(scroll);
	log->setText(first.c_str());
//	irr::core::rect<int> rect = log->getAbsolutePosition();
//	if(rect.getHeight() < log->getTextHeight()) {
//		scroll++;
//		Update();
//	}
	
	input->setText(std::to_wstring(
			sing::window->GetStringToEnterObject()->GetCurrent()).c_str());
}

std::vector<std::wstring>& Console::AccessLog() {
	static std::vector<std::wstring> logs;
	return logs;
}

std::mutex& Console::Mutex() {
	static std::mutex mutex;
	return mutex;
}

void Console::AddLog(const std::string& str) {
	std::lock_guard<std::mutex> lock(Mutex());
	AccessLog().emplace_back(std::to_wstring(str));
}

void Console::ClearLog() {
	std::lock_guard<std::mutex> lock(Mutex());
	AccessLog().clear();
}

std::wstring Console::GetLogsText(size_t offset) {
	std::vector<std::wstring> log;
	{
		std::lock_guard<std::mutex> lock(Mutex());
		log = AccessLog();
	}
	std::wstring str;
	if(offset >= log.size()) {
		offset = log.size();
		if(offset > 0)
			--offset;
	}
	for(size_t i=offset; i<log.size(); ++i) {
		if(i)
			str += L"\n";
		str += log[i];
	}
	return str;
}

std::vector<std::string>& Console::CommandsHistory() {
	static std::vector<std::string> cmd;
	return cmd;
}

namespace Debug {
	void PushMessage(const std::string& str) {
		Console::AddLog(str);
	}
}

#endif

