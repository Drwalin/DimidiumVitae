
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "..\css\Window.h"
#include "..\css\Engine.h"

void Window::DrawGUI()
{
	this->gui.Flush();
}

std::shared_ptr<Camera> Window::GetCameraPointer()
{
	return camera;
}

void Window::ParallelToDrawTick( const float deltaTime )
{
	if( engine )
		engine->ParallelToDrawTick( deltaTime );
}

void Window::Tick( const float deltaTime )
{
	if( engine )
		engine->Tick( deltaTime );
}

float Window::GetSmoothFps()
{
	static float last_fps = 0;
	static float fps = 0;
	static int n = 0;
	
	if( n == 20 )
	{
		last_fps = fps / float(n);
		n = 0;
		fps = 0;
	}
	
	fps += 1.0 / this->GetDeltaTime();
	++n;
	
	return last_fps;
}

void Window::Init( Engine * engine, const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen )
{
	Destroy();
	BasicWindow::Init( windowName, iconFile, width, height, eventResponser, fullscreen );
	gui.Init( this );
	this->engine = engine;
}

void Window::Destroy()
{
	BasicWindow::Destroy();
	engine = NULL;
}

Window::Window()
{
	engine = NULL;
}

Window::~Window()
{
	Destroy();
	camera.reset();
	camera = NULL;
	
}

#endif

