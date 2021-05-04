
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#include <mutex>
#include <list>
#include <atomic>
#include <unordered_map>
#include <thread>

#ifndef COMMAND_INTERPRETER_CPP
namespace moodycamel {
	template<typename T>
	class ConcurrentQueue;
}
#else
#include <concurrentqueue.h>
#endif

#include <JSON.hpp>

#include <Script.h>
#include <DllImporter.h>

class CommandInterpreter {
public:
	
	CommandInterpreter();
	~CommandInterpreter();
	
	void InvokeCommand(const JSON& cmd);
	void InvokeCommand(const std::string& cmd);
	
	void ExecuteSyncCommands(size_t count = 1);
	
	void AddNativeScript(const std::string& name,
			std::shared_ptr<ScriptBase> script);
	void AddNativeScript(const std::string& name, std::shared_ptr<Dll> dll);
	void AddScript(const std::string& name, const std::string& file);
	
private:
	
	std::shared_ptr<ScriptBase> GetScript(const std::string& name);
	
	void InterpreterLoop();
	
	void Interprete(const JSON& json, bool isSynced);
	
private:
	
	std::map<std::string, std::shared_ptr<ScriptBase>> scripts;
	
	moodycamel::ConcurrentQueue<JSON>* commands;
	moodycamel::ConcurrentQueue<JSON>* syncCommands;
	
	std::mutex scriptMutex;
	
	std::atomic<bool> end, ended;
	std::thread interpreterThread;
};

class ScriptLoader : public ScriptBase {
public:
	
	ScriptLoader() = default;
	virtual ~ScriptLoader() override = default;
	
	virtual void Run(const JSON& args) const override;
};

#endif

