
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_SINGLETON_CPP
#define ENGINE_SINGLETON_CPP

#include <gui/GUI.h>

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

namespace sing {
	class Engine *engine = NULL;
	class World *world = NULL;
	class Window *window = NULL;
	
	irr::IrrlichtDevice *device = NULL;
	irr::video::IVideoDriver *videoDriver = NULL;
	irr::scene::ISceneManager *sceneManager = NULL;
	irr::gui::IGUIEnvironment *igui = NULL;
	
	class FileSystem *fileSystem = NULL;
	class SoundEngine *soundEngine = NULL;
	class ResourceManager *resourceManager = NULL;
	class CommandInterpreter *commandInterpreter = NULL;
	
	GUI gui;
}

#endif
