
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_H
#define WINDOW_H

#include "TimeCounter.h"
#include "Camera.h"
#include "GUI.h"
#include "Menu.h"

#include "../lib/Debug.h"

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>

#include <irrlicht/irrlicht.h>

#include <string>
#include <atomic>
#include <thread>
#include <memory>

void ParallelThreadFunctionToDraw(class Window *window);

class Window {
public:
	
	Window();
	~Window();
	
	void Init(class Engine *engine, const std::string &windowName, const std::string &iconFile, int width, int height, class EventResponser *eventResponser, bool fullscreen = false);
	void BeginLoop();
	
	irr::IrrlichtDevice *GetDevice();
	irr::video::IVideoDriver *GetVideoDriver();
	irr::scene::ISceneManager *GetSceneManager();
	irr::gui::IGUIEnvironment *GetIgui();
	
	GUI &GetGUI();
	
	void SetCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> GetCamera();
	template<typename Menu_t, typename... Args>
	Menu_t* StartMenu(Args... args) {
		Menu_t *menu = new Menu_t(engine, args...);
		nextMenu = std::shared_ptr<Menu>(menu);
		return menu;
	}
	void StopMenu();
	std::shared_ptr<Menu> GetCurrentMenu();
	
	const TimeCounter& GetEventGenerationTime() const;
	const TimeCounter& GetWholeDrawTime() const;
	const TimeCounter& GetSkippedTime() const;
	const TimeCounter& GetEngineTickTime() const;
	const TimeCounter& GetAsynchronousTickTime() const;
	
	class StringToEnter *GetStringToEnterObject();
	class EventResponser *GetEventResponser();
	
	bool IsMouseLocked();
	void LockMouse();
	void UnlockMouse();
	void HideMouse();
	void ShowMouse();
	
	float GetDeltaTime();
	void SetFpsLimit(float fps);
	
	unsigned GetWidth();
	unsigned GetHeight();
	
	void QueueQuit();
	
	void Destroy();
	
	void ParallelToDrawTick();
	
private:
	
	void OneLoopFullTick();
	float GetSmoothFps();
	void GenerateEvents();
	void Tick();
	void UpdateDeltaTime();
	
	void Draw(bool drawEnvironment);
	void DrawGUI();
	
	
	void UseParallelThreadToDraw();
	void ShutDownParallelThreadToDraw();
	bool IsParallelToDrawTickInUse();
	
private:

	class Engine *engine;
	
	irr::IrrlichtDevice *device;
	irr::video::IVideoDriver *videoDriver;
	irr::scene::ISceneManager *sceneManager;
	irr::gui::IGUIEnvironment *igui;
	
	bool quitWhenPossible;
	
	float deltaTime;
	
	TimeCounter eventsTime;
	TimeCounter wholeDrawTime;
	TimeCounter engineTickTime;
	TimeCounter asynchronousTickTime;
	TimeCounter skippedTime;
	
	bool lockMouse;
	
	std::atomic<bool> parallelThreadToDrawContinue;
	std::thread parallelThreadToDraw;
	std::atomic<bool> useParallelThreadToDraw;
	
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Menu> currentMenu;
	std::shared_ptr<Menu> nextMenu;
	
	class EventReceiverIrrlicht *eventIrrlichtReceiver;
	class EventResponser *eventResponser;
	class StringToEnter *stringToEnter;
	GUI gui;
	
	float fpsLimit;
	
	TimePoint beginTime;
};

#endif

