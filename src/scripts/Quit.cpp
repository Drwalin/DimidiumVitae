
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#include "Script.h"

#include <engine/Engine.h>
#include <engine/Singleton.h>

#include <JSON.hpp>

void Quit(const JSON& args) {
	sing::engine->QueueQuit();
}

