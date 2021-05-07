
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <engine/Script.h>
#include <engine/Engine.h>
#include <engine/Singleton.h>

#include <JSON.hpp>
#include <game/Console.h>

void Clear(const JSON& args) {
	Console::ClearLog();
}


