
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef BASIC_WINDOW_H
#define BASIC_WINDOW_H

#include <irrlicht\irrlicht.h>

#include <atomic>
#include <thread>
#include <memory>

#include <EventResponser.h>
#include <Camera.h>
#include <TextPrinter.h>

#include <TimeCounter.h>

#include <Camera.h>

void ParallelThreadFunctionToDraw( class BasicWindow * window );

class BasicWindow
{
public:
	
	irr::IrrlichtDevice *device;
	irr::video::IVideoDriver *videoDriver;
	irr::scene::ISceneManager *sceneManager;
	irr::gui::IGUIEnvironment *gui;
	
	irr::core::dimension2du screenResolution;
	
private:
	
	virtual std::shared_ptr<Camera> GetCameraPointer() = 0;
	
	bool quitWhenPossible;
	
	float deltaTime;
	
	TimeCounter skippedTime;
	TimeCounter eventsTime;
	TimeCounter wholeDrawTime;
	
	bool lockMouse;
	
	std::atomic<bool> parallelThreadToDrawContinue;
	std::thread parallelThreadToDraw;
	std::atomic<bool> useParallelThreadToDraw;
	
	class EventReceiverIrrlicht * eventIrrlichtReceiver;
	EventResponser * eventResponser;
	
	friend void ParallelThreadFunctionToDraw( BasicWindow* );
	
protected:
	
	class StringToEnter * stringToEnter;
	
public:
	
	TimeCounter GetFlipDisplayTime() const;
	TimeCounter GetEventGenerationTime() const;
	TimeCounter GetWholeDrawTime() const;
	TimeCounter GetSkippedTime() const;
	
	void UseParallelThreadToDraw();
	void ShutDownParallelThreadToDraw();
	bool IsParallelToDrawTickInUse();
	
	class StringToEnter * GetStringToEnterObject();
	EventResponser * GetEventResponser();
	
	bool IsMouseLocked();
	void LockMouse();
	void UnlockMouse();
	void HideMouse();
	void ShowMouse();
	
	float GetDeltaTime();
	
	unsigned GetWidth();
	unsigned GetHeight();
	
	virtual void ParallelToDrawTick( const float deltaTime ) = 0;
	
	virtual void Tick( const float deltaTime ) = 0;
	void AlTick();
	void OneLoopFullTick();
	void BeginLoop();
	void QueueQuit();
	
	void Draw();
	
	void GenerateEvents();
	
	bool InitAllegro();
	bool InitFont();
	void InitTransforms();
	bool CreateDisplay( const char * windowName, int width, int height, bool fullscreen );
	void LoadIconFromFile( const char * iconFile );
	
	bool Init( const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen = false );
	
	void Destroy();
	
	BasicWindow();
	~BasicWindow();
};

#endif

