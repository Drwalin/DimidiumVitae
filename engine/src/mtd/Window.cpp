
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "..\css\Window.h"
#include "..\css\Engine.h"

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
	DEBUG(1)
	Destroy();
	DEBUG(2)
	BasicWindow::Init( windowName, iconFile, width, height, eventResponser, fullscreen );
	DEBUG(3)
	output->SetBasicWindow( this );
	DEBUG(4)
	output->ClearWorkspaceBorders();
	DEBUG(5)
	this->engine = engine;
}

void Window::Destroy()
{
	BasicWindow::Destroy();
	engine = NULL;
}

Window::Window()
{
	DEBUG(0)
	engine = NULL;
	DEBUG(1)
	//camera = std::shared_ptr<Camera>( new Camera );
	DEBUG(2)
	output = new TextPrinter;
	DEBUG(3)
	output->SetBasicWindow( this );
}

Window::~Window()
{
	Destroy();
	camera.reset();
	camera = NULL;
	delete output;
	output = NULL;
	
}

#endif

