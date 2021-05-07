
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "EventReceiverIrrlicht.h"
#include "EventResponser.h"
#include "Window.h"
#include "Engine.h"

#include <util/StringToEnter.h>
#include <util/Debug.h>

#include <cstring>

irr::IrrlichtDevice *Window::GetDevice() {
	return device;
}

irr::video::IVideoDriver *Window::GetVideoDriver() {
	return videoDriver;
}

irr::scene::ISceneManager *Window::GetSceneManager() {
	return sceneManager;
}

irr::gui::IGUIEnvironment *Window::GetIgui() {
	return igui;
}

GUI &Window::GetGUI() {
	return gui;
}

void Window::SetCamera(std::shared_ptr<Camera> camera) {
	this->camera = camera;
}

std::shared_ptr<Camera> Window::GetCamera() {
	return camera;
}

void Window::StopMenu() {
	if(!activeMenus.empty()) {
		Menu* top = activeMenus.back();
		delete top;
		activeMenus.pop_back();
		if(!activeMenus.empty())
			activeMenus.back()->RestoreFromBackground();
	}
}

Menu* Window::GetCurrentMenu() {
	if(!activeMenus.empty()) {
		return activeMenus.back();
	}
	return NULL;
}

Menu* Window::GetPreviousMenu() {
	if(activeMenus.size() < 2)
		return NULL;
	auto it = activeMenus.rbegin();
	++it;
	return *it;
}

const TimeCounter& Window::GetEventGenerationTime() const {
	return eventsTime;
}

const TimeCounter& Window::GetWholeDrawTime() const {
	return wholeDrawTime;
}

const TimeCounter& Window::GetEngineTickTime() const {
	return engineTickTime;
}

const TimeCounter& Window::GetSkippedTime() const {
	return skippedTime;
}

StringToEnter *Window::GetStringToEnterObject() {
	return stringToEnter;
}

EventResponser *Window::GetEventResponser() {
	return eventResponser;
}

EventResponser *Window::GetActiveEventResponser() {
	return eventIrrlichtReceiver->GetActiveEventResponser();
}

bool Window::IsMouseLocked() {
	return lockMouse;
}

void Window::LockMouse() {
	lockMouse = true;
}

void Window::UnlockMouse() {
	lockMouse = false;
}

void Window::HideMouse() {
	device->getCursorControl()->setVisible(false);
}

void Window::ShowMouse() {
	device->getCursorControl()->setVisible(true);
}


unsigned Window::GetWidth() {
	return videoDriver->getScreenSize().Width;
}

unsigned Window::GetHeight() {
	return videoDriver->getScreenSize().Height;
}

void Window::GenerateEvents() {
	eventsTime.SubscribeStart();
	eventIrrlichtReceiver->GenerateEvents();
	GetCamera()->SetRenderTargetSize(GetWidth(), GetHeight());
	eventsTime.SubscribeEnd();
}

void Window::Draw(bool doNotDrawEnvironment) {
	bool drawEnvironment = GetCurrentMenu()==NULL ||
			(GetCurrentMenu()&&GetCurrentMenu()->RenderSceneInBackground());
	if(doNotDrawEnvironment)
		drawEnvironment = false;
	wholeDrawTime.SubscribeStart();
	videoDriver->beginScene(true, true, irr::video::SColor(255,16,32,64));
	GetCamera()->UseTarget();
	if(drawEnvironment) {
		sceneManager->drawAll();
	}
	igui->drawAll();
	DrawGUI();
	videoDriver->endScene();
	wholeDrawTime.SubscribeEnd();
}

void Window::DrawGUI() {
	gui.Flush();
}

void Window::Init(const std::string &windowName, const std::string &iconFile,
		int width, int height, EventResponser *eventResponser,
		bool fullscreen) {
	Destroy();
	
	this->eventResponser = eventResponser;
	eventIrrlichtReceiver = new EventReceiverIrrlicht(eventResponser);
	sing::irrlichtEventReceiver = eventIrrlichtReceiver;
	
	sing::device = device = irr::createDevice(irr::video::EDT_OPENGL,
			irr::core::dimension2du(width, height), 16, fullscreen, true, false,
			this->eventIrrlichtReceiver);
	printf(" Using Irrlicht %s\n", device->getVersion());
	device->setWindowCaption(std::wstring(windowName.c_str(),
				windowName.c_str()+windowName.size()).c_str());
	device->setResizable(true);
	
	sing::videoDriver = videoDriver = device->getVideoDriver();
	sing::sceneManager = sceneManager = device->getSceneManager();
	sing::igui = igui = device->getGUIEnvironment();
	
	sceneManager->setShadowColor(irr::video::SColor(220,32,32,32));
	
	if(!videoDriver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
		MESSAGE("videoDriver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET) "
				"failed");
	
	igui->getSkin()->setFont(device->getGUIEnvironment()->
			getFont("./media/Fonts/courier.bmp"));
	
	gui.Init();
}

void Window::Destroy() {
	while(!activeMenus.empty()) {
		Menu* top = activeMenus.back();
		delete top;
		activeMenus.pop_back();
	}
	camera.reset();
	camera = NULL;
	if(device) {
		device->closeDevice();
		device->drop();
		device = NULL;
	}
	lockMouse = false;
	eventResponser = NULL;
	sing::device = NULL;
	sing::videoDriver = NULL;
	sing::sceneManager = NULL;
	sing::igui = NULL;
}

Window::Window() : gui(sing::gui) {
	device = NULL;
	videoDriver = NULL;
	sceneManager = NULL;
	igui = NULL;
	
	lockMouse = false;
	
	eventResponser = NULL;
	stringToEnter = new StringToEnter;
}

Window::~Window() {
	Destroy();
	delete stringToEnter;
	stringToEnter = NULL;
}

#endif
