
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CONSOLE_CPP
#define CONSOLE_CPP

#include "../css/Console.h"

#include <Engine.h>
#include <Window.h>
#include <StringToEnter.h>

#include <StdUtil.hpp>

Console::Console() :
	Menu() {
	state = sing::engine->StoreSimulationExecution();
	sing::engine->PauseSimulation();
	
	log = AddStaticText({0, 0, 0, 0}, "");
	log->setRelativePositionProportional({0.0f, 0.0f, 1.0f, 0.9f});
	log->setBackgroundColor({200, 0, 0, 0});
	log->setOverrideColor({255, 255, 255, 255});
	
	input = AddStaticText({0, 0, 0, 0}, "");
	input->setRelativePositionProportional({0.0f, 0.9f, 1.0f, 1.0f});
	input->setBackgroundColor({200, 0, 0, 0});
	input->setOverrideColor({255, 255, 255, 255});
	
	stringGetter = sing::window->GetStringToEnterObject();
	stringGetter->Clear();
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
	Update();
}

void Console::StringToEnterEvent(const std::string& str) {
	sing::commandInterpreter->InvokeCommand(str);
	Update();
}

void Console::OnOtherEvent(const irr::SEvent::SGUIEvent &event) {
	Update();
}

void Console::OnButtonClicked(Menu::Button *button, Menu::Element *element) {
	Update();
}

void Console::Update() {
	input->setText(std::to_wstring(
				sing::window->GetStringToEnterObject()->GetCurrent()).c_str());
	log->setText(GetLogsText(0).c_str());
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

namespace Debug {
	void PushMessage(const std::string& str) {
		Console::AddLog(str);
	}
}

#endif

