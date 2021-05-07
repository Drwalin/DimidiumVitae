
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_H
#define WINDOW_H

#include "Singleton.h"

#include "Camera.h"
#include <gui/GUI.h>
#include <gui/Menu.h>

#include <util/TimeCounter.h>
#include <util/Thread.h>
#include <util/Debug.h>

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>

#include <irrlicht/irrlicht.h>

#include <string>
#include <atomic>
#include <memory>

class Window {
public:
	
	Window();
	~Window();
	
	void Init(const std::string &windowName, const std::string &iconFile,
			int width, int height, class EventResponser *eventResponser,
			bool fullscreen = false);
	
	irr::IrrlichtDevice *GetDevice();
	irr::video::IVideoDriver *GetVideoDriver();
	irr::scene::ISceneManager *GetSceneManager();
	irr::gui::IGUIEnvironment *GetIgui();
	
	GUI &GetGUI();
	
	void SetCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> GetCamera();
	template<typename Menu_t, typename... Args>
	Menu_t* StartMenu(Args... args) {
		if(!activeMenus.empty())
			activeMenus.back()->PutToBackground();
		Menu_t *menu = new Menu_t(args...);
		activeMenus.emplace_back(menu);
		return menu;
	}
	void StopMenu();
	Menu* GetCurrentMenu();
	Menu* GetPreviousMenu();
	
	const TimeCounter& GetEventGenerationTime() const;
	const TimeCounter& GetWholeDrawTime() const;
	const TimeCounter& GetSkippedTime() const;
	const TimeCounter& GetEngineTickTime() const;
	
	class StringToEnter *GetStringToEnterObject();
	class EventResponser *GetEventResponser();
	class EventResponser *GetActiveEventResponser();
	
	bool IsMouseLocked();
	void LockMouse();
	void UnlockMouse();
	void HideMouse();
	void ShowMouse();
	
	unsigned GetWidth();
	unsigned GetHeight();
	
	void Destroy();
	
	void Draw(bool doNotDrawEnvironment = false);
	
	void GenerateEvents();
	
private:
	
	void DrawGUI();
	
private:
	
	irr::IrrlichtDevice *device;
	irr::video::IVideoDriver *videoDriver;
	irr::scene::ISceneManager *sceneManager;
	irr::gui::IGUIEnvironment *igui;
	
	TimeCounter eventsTime;
	TimeCounter wholeDrawTime;
	TimeCounter engineTickTime;
	TimeCounter skippedTime;
	
	bool lockMouse;
	
	std::shared_ptr<Camera> camera;
	std::list<Menu*> activeMenus;
	
	class EventReceiverIrrlicht *eventIrrlichtReceiver;
	class EventResponser *eventResponser;
	class StringToEnter *stringToEnter;
	GUI &gui;
};

#endif

