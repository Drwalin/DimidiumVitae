
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_H
#define WINDOW_H

#include "Camera.h"
#include "GUI.h"
#include "BasicWindow.h"
#include "EventResponser.h"

#include "..\lib\Debug.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <string>

#include <memory>

class Window : public BasicWindow
{
private:
	
	class Engine * engine;
	
public:
	
	virtual std::shared_ptr<Camera> GetCameraPointer() override;
	
	std::shared_ptr<Camera> camera;
	GUI gui;
	
	virtual void DrawGUI() override;
	
	void Init( Engine * engine, const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen = false );
	
	virtual void ParallelToDrawTick( const float deltaTime ) override;
	
	virtual void Tick( const float deltaTime ) override;
	float GetSmoothFps();
	
	void Destroy();
	
	Window();
	~Window();
};

#endif

