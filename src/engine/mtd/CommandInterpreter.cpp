
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COMMAND_INTERPRETER_CPP
#define COMMAND_INTERPRETER_CPP

#include <functional>

#include <CommandInterpreter.h>
#include <Singleton.h>
#include <TimeCounter.h>

#include <Debug.h>

CommandInterpreter::CommandInterpreter() {
	commands = new moodycamel::ConcurrentQueue<JSON>();
	syncCommands = new moodycamel::ConcurrentQueue<JSON>();
	end = false;
	ended = false;
	AddNativeScript("loadscript",
			std::shared_ptr<ScriptBase>(new ScriptLoader()));
	interpreterThread =
		std::thread(
				std::bind(
					&CommandInterpreter::InterpreterLoop,
					this
					)
				);
}

CommandInterpreter::~CommandInterpreter() {
	end = true;
	interpreterThread.join();
	delete commands;
	delete syncCommands;
}

void CommandInterpreter::InvokeCommand(const JSON& cmd) {
	commands->enqueue(cmd);
}

void CommandInterpreter::InvokeCommand(const std::string& cmd) {
	JSON json = ScriptBase::ConvertStringToJSON(cmd);
	commands->enqueue(json);
}


void CommandInterpreter::ExecuteSyncCommands(size_t count) {
	for(; count>0; --count) {
		JSON json;
		if(!commands->try_dequeue(json))
			return;
		Interprete(json, false);
	}
}


void CommandInterpreter::AddNativeScript(const std::string& name,
		void (*run)(const JSON&), bool needSync) {
	AddNativeScript(name,
			std::shared_ptr<ScriptBase>(new ScriptNative(run, needSync)));
}
void CommandInterpreter::AddNativeScript(const std::string& name,
		std::shared_ptr<ScriptBase> script) {
	std::lock_guard<std::mutex> lock(scriptMutex);
	scripts[name] = script;
}

void CommandInterpreter::AddNativeScript(const std::string& name,
		std::shared_ptr<Dll> dll) {
	if(dll == NULL || !dll->IsValid()) {
		MESSAGE(std::string("Trying to add script from empty shared object: \'"
					) + name + "\'");
		return;
	}
	std::string scriptGetter = "NativeScript_" + name;
	std::shared_ptr<ScriptBase> script = dll->Get<std::shared_ptr<ScriptBase>(*)()>(
			scriptGetter.c_str())();
	AddNativeScript(name, script);
}

void CommandInterpreter::AddScript(const std::string& name,
		const std::string& file) {
	IFile in = sing::fileSystem->ReadFile(file);
	std::shared_ptr<Script> script = std::make_shared<Script>(in);
	std::lock_guard<std::mutex> lock(scriptMutex);
	if(name != "")
		scripts[name] = script;
	scripts[file] = script;
}

std::shared_ptr<ScriptBase> CommandInterpreter::GetScript(
		const std::string& name) {
	std::lock_guard<std::mutex> lock(scriptMutex);
	auto it = scripts.find(name);
	if(it == scripts.end())
		return NULL;
	return it->second;
}


void CommandInterpreter::InterpreterLoop() {
	while(end == false) {
		JSON json;
		if(commands->try_dequeue(json))
			Interprete(json, false);
		else
			TimeCounter::Sleep(0.0002f);
	}
	ended = true;
}

void CommandInterpreter::Interprete(const JSON& json, bool isSynced) {
	std::string name = json[0].String();
	std::shared_ptr<ScriptBase> script = GetScript(name);
	if(script == NULL) {
		MESSAGE(std::string("Cannot find script: \'") + name + "\'");
		return;
	}
	if(script->NeedSync() && !isSynced)
		syncCommands->enqueue(json);
	else
		script->Run(json);
}



void ScriptLoader::Run(const JSON& args) const {
	size_t size = args.size();
	if(size==2 || size==3) {
		std::string fileName = args[1].String();
		std::shared_ptr<Dll> dll(new Dll(fileName.c_str()));
		std::string name = size==3 ? args[2].String() : fileName;
		if(dll && dll->IsValid())
			sing::commandInterpreter->AddNativeScript(name, dll);
		else
			sing::commandInterpreter->AddScript(name, fileName);
	} else {
		MESSAGE(std::string("Trying to load new script from file with bad"
					" number of arguments: ") + args.Write());
	}
}

#endif

