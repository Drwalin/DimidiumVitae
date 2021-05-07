
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SCRIPT_CPP
#define SCRIPT_CPP

#include <sstream>

#include "Script.h"
#include "Singleton.h"
#include "CommandInterpreter.h"

#include <util/Debug.h>

ScriptBase::ScriptBase() {}
ScriptBase::~ScriptBase() {}

bool ScriptBase::IsNative() const {
	return true;
}

bool ScriptBase::NeedSync() const {
	return false;
}

JSON ScriptBase::ConvertStreamToJSON(std::istream& in) {
	JSON json;
	in >> std::ws;
	for(int i=0; !in.eof() || in.peek()==';'; in>>std::ws, ++i)
		in >> json[i];
	return json;
}

JSON ScriptBase::ConvertStringToJSON(const std::string& str) {
	std::istringstream ss(str);
	return ConvertStreamToJSON(ss);
}


Script::~Script() {}

Script::Script(IFile& file) {
	commands.clear();
	commands.reserve(128);
	while(!file.eof()) {
		commands.emplace_back(ConvertStreamToJSON(file));
		file >> std::ws;
		if(!file.eof() && file.peek() == ';')
			file.get();
	}
}

void Script::Run(const JSON& args) const {
	for(const JSON& it : commands) {
		JSON cmd = it;
		for(JSON& arg : cmd.Array()) {
			if(arg.IsString()) {
				std::string str = arg.GetString();
				if(str[0] == '$') {
					int id = atoi(str.c_str()+1);
					arg = args[id];
				}
			}
		}
		sing::commandInterpreter->InvokeCommand(cmd);
	}
}

bool Script::IsNative() const {
	return false;
}

bool Script::NeedSync() const {
	return false;
}



ScriptNative::ScriptNative(void (*run)(const JSON&), bool needSync) :
	_Run(run), needSync(needSync) {
}

void ScriptNative::Run(const JSON& json) const {
	_Run(json);
}

bool ScriptNative::NeedSync() const {
	return needSync;
}

#endif

