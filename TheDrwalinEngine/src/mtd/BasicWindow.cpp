
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef BASIC_WINDOW_CPP
#define BASIC_WINDOW_CPP

#include <BasicWindow.h>
#include <Debug.h>
#include <StringToEnter.h>
#include <EventReceiverIrrlicht.h>

void ParallelThreadFunctionToDraw( BasicWindow * window )
{
	if( window )
	{
		while( window->IsParallelToDrawTickInUse() )
		{
			while( window->parallelThreadToDrawContinue.load() == false )
			{
				if( window->IsParallelToDrawTickInUse() == false )
					return;
				//al_rest( 0.001 );
			}
			
			window->ParallelToDrawTick( window->GetDeltaTime() );
			
			window->parallelThreadToDrawContinue.store( false );
		}
	}
}

TimeCounter BasicWindow::GetEventGenerationTime() const
{
	return eventsTime;
}

TimeCounter BasicWindow::GetWholeDrawTime() const
{
	return wholeDrawTime;
}

TimeCounter BasicWindow::GetSkippedTime() const
{
	return skippedTime;
}

bool BasicWindow::IsParallelToDrawTickInUse()
{
	return useParallelThreadToDraw.load();
}

void BasicWindow::UseParallelThreadToDraw()
{
	if( useParallelThreadToDraw.load() == false )
	{
		parallelThreadToDrawContinue.store( false );
		useParallelThreadToDraw.store( true );
		parallelThreadToDraw = std::thread( ParallelThreadFunctionToDraw, this );
	}
}

void BasicWindow::ShutDownParallelThreadToDraw()
{
	if( useParallelThreadToDraw.load() == true )
	{
		useParallelThreadToDraw.store( false );
		parallelThreadToDraw.join();
	}
}

unsigned BasicWindow::GetWidth()
{
	return screenResolution.Width;
}

unsigned BasicWindow::GetHeight()
{
	return screenResolution.Height;
}

bool BasicWindow::IsMouseLocked()
{
	return lockMouse;
}

void BasicWindow::LockMouse()
{
	lockMouse = true;
}

void BasicWindow::UnlockMouse()
{
	lockMouse = false;
}

void BasicWindow::HideMouse()
{
	device->getCursorControl()->setVisible( false );
}

void BasicWindow::ShowMouse()
{
	device->getCursorControl()->setVisible( true );
}

float BasicWindow::GetDeltaTime()
{
	return deltaTime;
}

bool BasicWindow::Init( const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen )
{
	Destroy();
	
	this->eventResponser = eventResponser;
	this->eventResponser->SetBasicWindow( this );
	eventIrrlichtReceiver = new EventReceiverIrrlicht( eventResponser, this );
	
	screenResolution = irr::core::dimension2du( width, height );
	device = irr::createDevice( irr::video::EDT_OPENGL, screenResolution, 16, fullscreen, true, false, eventIrrlichtReceiver );
	
	videoDriver = device->getVideoDriver();
	sceneManager = device->getSceneManager();
	gui = device->getGUIEnvironment();
	
	sceneManager->setShadowColor( irr::video::SColor(220,32,32,32) );
	
	stringToEnter->SetBasicWindow( this );
	
	if( !videoDriver->queryFeature( irr::video::EVDF_RENDER_TO_TARGET ) )
	{
		return false;
	}
	
	return true;
}

void BasicWindow::Destroy()
{
	DEBUG(1)
	ShutDownParallelThreadToDraw();
	
	DEBUG(2)
	if( device )
	{
		device->closeDevice();
		device->drop();
		device = NULL;
	}
	
	DEBUG(3)
	quitWhenPossible = false;
	
	DEBUG(4)
	deltaTime = 0.0;
	
	DEBUG(5)
	lockMouse = false;
	
	DEBUG(6)
	eventResponser = NULL;
	DEBUG(7)
}

void BasicWindow::OneLoopFullTick()
{
	static float beginTime = (float(clock())/1000.0f);
	
	deltaTime = (float(clock())/1000.0f) - beginTime;
	beginTime = (float(clock())/1000.0f);
	
	if( deltaTime < 0.0001 )
		deltaTime = 0.0001;
	else if( deltaTime > 0.3 )
		deltaTime = 0.3;
	
	AlTick();
	
	/*
	if( al_get_time() - beginTime < 1.0/60.0 )
	{
		skippedTime.SubscribeStart();
		al_rest( skippedTime );
		skippedTime.SubscribeEnd();
	}
	*/
}

void BasicWindow::BeginLoop()
{
	quitWhenPossible = false;
	while( !quitWhenPossible && device->run() )
		OneLoopFullTick();	
}

void BasicWindow::QueueQuit()
{
	quitWhenPossible = true;
}

void BasicWindow::Draw()
{
	wholeDrawTime.SubscribeStart();
	
	if( IsParallelToDrawTickInUse() )
	{
		DEBUG(0)
		parallelThreadToDrawContinue.store( true );
	}
	
	videoDriver->beginScene( true, true, irr::video::SColor(255,16,32,64) );
	GetCameraPointer()->UseTarget();
	sceneManager->drawAll();
	gui->drawAll();
	videoDriver->endScene();
	
	if( IsParallelToDrawTickInUse() )
	{
		DEBUG(1)
		while( parallelThreadToDrawContinue.load() )
		{
		//	al_rest( 0.001 );
		}
	}
	
	wholeDrawTime.SubscribeStart();
}


void BasicWindow::AlTick()
{
	if( lockMouse )
	{
		eventIrrlichtReceiver->SetCursor( GetWidth() / 2, GetHeight() / 2 );
		device->getCursorControl()->setPosition( 0.5f, 0.5f );//float(GetWidth())/2.0f, float(GetHeight())/2.0f );
	}
	
	GenerateEvents();
	
	Tick( deltaTime );
	
	Draw();
}

void BasicWindow::GenerateEvents()
{
	eventsTime.SubscribeStart();
	
	eventIrrlichtReceiver->GenerateEvents();
	
	eventsTime.SubscribeEnd();
}

EventResponser * BasicWindow::GetEventResponser()
{
	return eventResponser;
}

StringToEnter * BasicWindow::GetStringToEnterObject()
{
	return stringToEnter;
}

BasicWindow::BasicWindow() :
	useParallelThreadToDraw(false)
{
	device = NULL;
	videoDriver = NULL;
	sceneManager = NULL;
	gui = NULL;
	
	quitWhenPossible = false;
	
	deltaTime = 0.01;
	
	lockMouse = false;
	
	eventResponser = NULL;
	stringToEnter = new StringToEnter;
	
	useParallelThreadToDraw = false;
}

BasicWindow::~BasicWindow()
{
	Destroy();
	delete stringToEnter;
	stringToEnter = NULL;
}

#endif

