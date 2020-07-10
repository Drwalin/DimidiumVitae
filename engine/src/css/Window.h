
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_H
#define WINDOW_H

#include "TimeCounter.h"
#include "Camera.h"
#include "GUI.h"

#include "..\lib\Debug.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <irrlicht\irrlicht.h>

#include <string>
#include <atomic>
#include <thread>
#include <memory>

void ParallelThreadFunctionToDraw( class Window * window );

class Window
{
private:
	
	class Engine * engine;
	
	irr::IrrlichtDevice *device;
	irr::video::IVideoDriver *videoDriver;
	irr::scene::ISceneManager *sceneManager;
	irr::gui::IGUIEnvironment *igui;
	
	irr::core::dimension2du screenResolution;
	
	bool quitWhenPossible;
	
	float deltaTime;
	
	TimeCounter skippedTime;
	TimeCounter eventsTime;
	TimeCounter wholeDrawTime;
	TimeCounter engineTickTime;
	
	bool lockMouse;
	
	std::atomic<bool> parallelThreadToDrawContinue;
	std::thread parallelThreadToDraw;
	std::atomic<bool> useParallelThreadToDraw;
	
	std::shared_ptr<Camera> camera;
	
	class EventReceiverIrrlicht * eventIrrlichtReceiver;
	class EventResponser * eventResponser;
	class StringToEnter * stringToEnter;
	GUI gui;
	
	friend void ParallelThreadFunctionToDraw( Window* );
	
public:
	
	irr::IrrlichtDevice * GetDevice();
	irr::video::IVideoDriver * GetVideoDriver();
	irr::scene::ISceneManager * GetSceneManager();
	irr::gui::IGUIEnvironment * GetIgui();
	
	GUI & GetGUI();
	
	void SetCamera( std::shared_ptr<Camera> camera );
	std::shared_ptr<Camera> GetCamera();
	
	TimeCounter GetEventGenerationTime() const;
	TimeCounter GetWholeDrawTime() const;
	TimeCounter GetSkippedTime() const;
	TimeCounter GetEngineTickTime() const;
	void ParallelToDrawTick( const float deltaTime );
	
	void UseParallelThreadToDraw();
	void ShutDownParallelThreadToDraw();
	bool IsParallelToDrawTickInUse();
	
	class StringToEnter * GetStringToEnterObject();
	class EventResponser * GetEventResponser();
	
	bool IsMouseLocked();
	void LockMouse();
	void UnlockMouse();
	void HideMouse();
	void ShowMouse();
	
	float GetDeltaTime();
	
	unsigned GetWidth();
	unsigned GetHeight();
	
	void QueueQuit();
	
	void AlTick();
	void OneLoopFullTick();
	float GetSmoothFps();
	void GenerateEvents();
	void Tick( const float deltaTime );
	
	void Draw();
	void DrawGUI();
	
	void Init( Engine * engine, const std::string & windowName, const std::string & iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen = false );
	void BeginLoop();
	
	void Destroy();
	
	Window();
	~Window();
};

#endif

