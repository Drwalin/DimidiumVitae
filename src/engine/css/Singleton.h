
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_SINGLETON_H
#define ENGINE_SINGLETON_H

#ifndef ENGINE_SINGLETON_CPP

#include "GUI.h"

namespace irr {
	class IrrlichtDevice;
	namespace video {
		class IVideoRenderer;
	}
	namespace scene {
		class ISceneManager;
	}
	namespace gui {
		class IGUIEnvironment;
	}
}
	
class FileSystem;
class SoundEngine;
class ResourceManager;
class Engine;
class World;
class Window;

namespace sing {
	extern class Engine *engine;
	extern class World *world;
	extern class Window *window;
	
	extern irr::IrrlichtDevice *device;
	extern irr::video::IVideoDriver *videoDriver;
	extern irr::scene::ISceneManager *sceneManager;
	extern irr::gui::IGUIEnvironment *igui;
	
	extern class FileSystem *fileSystem;
	extern class SoundEngine *soundEngine;
	extern class ResourceManager *resourceManager;
	extern GUI gui;
}

#endif

#endif
