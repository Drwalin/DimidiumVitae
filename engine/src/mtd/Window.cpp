
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "..\css\EventReceiverIrrlicht.h"
#include "..\css\EventResponser.h"
#include "..\css\StringToEnter.h"
#include "..\css\Window.h"
#include "..\css\Engine.h"

#include "..\lib\Debug.h"

#include <cstring>

void ParallelThreadFunctionToDraw(Window *window) {
	if(window) {
		window->ParallelToDrawTick();
	}
}

void Window::ParallelToDrawTick() {
	while(this->IsParallelToDrawTickInUse()) {
		while(this->parallelThreadToDrawContinue.load() == false) {
			if(this->IsParallelToDrawTickInUse() == false)
				return;
			std::this_thread::yield();
		}
		if(this->engine)
			this->engine->AsynchronousTick(this->GetDeltaTime());
		this->parallelThreadToDrawContinue.store(false);
	}
}

irr::IrrlichtDevice *Window::GetDevice() {
	return this->device;
}

irr::video::IVideoDriver *Window::GetVideoDriver() {
	return this->videoDriver;
}

irr::scene::ISceneManager *Window::GetSceneManager() {
	return this->sceneManager;
}

irr::gui::IGUIEnvironment *Window::GetIgui() {
	return this->igui;
}

GUI &Window::GetGUI() {
	return this->gui;
}

void Window::SetCamera(std::shared_ptr<Camera> camera) {
	this->camera = camera;
}

std::shared_ptr<Camera> Window::GetCamera() {
	return this->camera;
}

TimeCounter Window::GetEventGenerationTime() const {
	return this->eventsTime;
}

TimeCounter Window::GetWholeDrawTime() const {
	return this->wholeDrawTime;
}

TimeCounter Window::GetEngineTickTime() const {
	return this->engineTickTime;
}

TimeCounter Window::GetAsynchronousTickTime() const {
	return this->asynchronousTickTime;
}

void Window::UseParallelThreadToDraw() {
	if(this->useParallelThreadToDraw.load() == false) {
		this->parallelThreadToDrawContinue.store(false);
		this->useParallelThreadToDraw.store(true);
		this->parallelThreadToDraw = std::thread(ParallelThreadFunctionToDraw, this);
	}
}

void Window::ShutDownParallelThreadToDraw() {
	if(this->useParallelThreadToDraw.load() == true) {
		this->useParallelThreadToDraw.store(false);
		this->parallelThreadToDraw.join();
	}
}

bool Window::IsParallelToDrawTickInUse() {
	return this->useParallelThreadToDraw.load();
}

StringToEnter *Window::GetStringToEnterObject() {
	return this->stringToEnter;
}

EventResponser *Window::GetEventResponser() {
	return this->eventResponser;
}

bool Window::IsMouseLocked() {
	return this->lockMouse;
}

void Window::LockMouse() {
	this->lockMouse = true;
}

void Window::UnlockMouse() {
	this->lockMouse = false;
}

void Window::HideMouse() {
	this->device->getCursorControl()->setVisible(false);
}

void Window::ShowMouse() {
	this->device->getCursorControl()->setVisible(true);
}

float Window::GetDeltaTime() {
	return this->deltaTime;
}

unsigned Window::GetWidth() {
	return this->screenResolution.Width;
}

unsigned Window::GetHeight() {
	return this->screenResolution.Height;
}

void Window::QueueQuit() {
	this->quitWhenPossible = true;
}

void Window::OneLoopFullTick() {
	static float beginTime = (float(clock())/1000.0f);
	
	deltaTime = (float(clock())/1000.0f) - beginTime;
	beginTime = (float(clock())/1000.0f);
	
	if(deltaTime < 0.001)
		deltaTime = 0.001;
	else if(deltaTime > 0.3)
		deltaTime = 0.3;
	
	this->Tick();
}

float Window::GetSmoothFps() {
	static float last_fps = 0;
	static float fps = 0;
	static int n = 0;
	
	if(n == 20) {
		last_fps = fps / float(n);
		n = 0;
		fps = 0;
	}
	
	fps += 1.0 / this->GetDeltaTime();
	++n;
	
	return last_fps;
}

void Window::GenerateEvents() {
	this->eventsTime.SubscribeStart();
	this->eventIrrlichtReceiver->GenerateEvents();
	this->eventsTime.SubscribeEnd();
}

void Window::Tick() {
	if(this->lockMouse) {
		this->eventIrrlichtReceiver->SetCursor(this->GetWidth()/2, this->GetHeight()/2);
		this->device->getCursorControl()->setPosition(0.5f, 0.5f);
	}
	
	this->GenerateEvents();
	
	engineTickTime.SubscribeStart();
	if(this->engine)
		this->engine->SynchronousTick(this->deltaTime);
	engineTickTime.SubscribeEnd();
	
	this->asynchronousTickTime.SubscribeStart();
	if(this->IsParallelToDrawTickInUse())
		this->parallelThreadToDrawContinue.store(true);
	this->Draw();
	if(this->IsParallelToDrawTickInUse()) {
		while(this->parallelThreadToDrawContinue.load())
			std::this_thread::yield();
	}
	this->asynchronousTickTime.SubscribeEnd();
}

void Window::Draw() {
	this->wholeDrawTime.SubscribeStart();
	this->videoDriver->beginScene(true, true, irr::video::SColor(255,16,32,64));
	this->GetCamera()->UseTarget();
	this->sceneManager->drawAll();
	this->igui->drawAll();
	this->DrawGUI();
	this->videoDriver->endScene();
	this->wholeDrawTime.SubscribeEnd();
}

void Window::DrawGUI() {
	this->gui.Flush();
}

void Window::Init(Engine *engine, const std::string &windowName, const std::string &iconFile, int width, int height, EventResponser *eventResponser, bool fullscreen) {
	this->Destroy();
	
	this->eventResponser = eventResponser;
	this->eventResponser->SetWindow(this);
	this->eventIrrlichtReceiver = new EventReceiverIrrlicht(this->eventResponser, this);
	
	this->screenResolution = irr::core::dimension2du(width, height);
	this->device = irr::createDevice(irr::video::EDT_OPENGL, this->screenResolution, 16, fullscreen, true, false, this->eventIrrlichtReceiver);
	this->device->setWindowCaption(std::wstring(windowName.c_str(),windowName.c_str()+windowName.size()).c_str());
	
	this->videoDriver = this->device->getVideoDriver();
	this->sceneManager = this->device->getSceneManager();
	this->igui = this->device->getGUIEnvironment();
	
	this->sceneManager->setShadowColor(irr::video::SColor(220,32,32,32));
	this->stringToEnter->SetWindow(this);
	
	if(!this->videoDriver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
		MESSAGE("videoDriver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET) failed");
	
	this->engine = engine;
	
	this->gui.Init(this);
	
	this->UseParallelThreadToDraw();
}

void Window::BeginLoop() {
	this->quitWhenPossible = false;
	while(!this->quitWhenPossible && this->device->run())
		this->OneLoopFullTick();	
}

void Window::Destroy() {
	this->ShutDownParallelThreadToDraw();
	if(this->device) {
		this->device->closeDevice();
		this->device->drop();
		this->device = NULL;
	}
	this->quitWhenPossible = false;
	this->deltaTime = 0.0;
	this->lockMouse = false;
	this->eventResponser = NULL;
	this->engine = NULL;
}

Window::Window() :
	useParallelThreadToDraw(false)
{
	this->device = NULL;
	this->videoDriver = NULL;
	this->sceneManager = NULL;
	this->igui = NULL;
	
	this->quitWhenPossible = false;
	
	this->deltaTime = 0.01;
	
	this->lockMouse = false;
	
	this->eventResponser = NULL;
	this->stringToEnter = new StringToEnter;
	
	this->useParallelThreadToDraw = false;
	this->engine = NULL;
}

Window::~Window() {
	this->Destroy();
	delete this->stringToEnter;
	this->stringToEnter = NULL;
	this->camera.reset();
	this->camera = NULL;
	
}

#endif
