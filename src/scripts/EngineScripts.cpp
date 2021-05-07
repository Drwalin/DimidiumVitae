
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#include "Script.h"

#include <engine/Engine.h>
#include <engine/Singleton.h>

#include <JSON.hpp>

#include <util/Debug.h>

void New(const JSON& args);
void Delete(const JSON& args);
void Quit(const JSON& args);
void Clear(const JSON& args);

void Add(const std::string& name, bool needSync, void (*run)(const JSON&)) {
	sing::commandInterpreter->AddNativeScript(name, run, needSync);
}

extern "C" void AddEngineScripts() {
	Add("new", true, New);
	Add("delete", true, Delete);
	Add("exit", true, Quit);
	Add("quit", true, Quit);
	Add("shutdown", true, Quit);
	Add("clear", true, Clear);
}

