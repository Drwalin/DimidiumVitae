
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>
#include <iostream>

#include <JSON.hpp>

#include <engine/FileSystem.h>

class ScriptBase {
public:
	
	ScriptBase();
	virtual ~ScriptBase();
	
	virtual void Run(const JSON& args) const = 0;
	virtual bool IsNative() const;
	virtual bool NeedSync() const;
	
	static JSON ConvertStreamToJSON(std::istream& in);
	static JSON ConvertStringToJSON(const std::string& str);
};

class Script : public ScriptBase {
public:
	
	Script(IFile& file);
	virtual ~Script() override;
	
	virtual void Run(const JSON& args) const override;
	virtual bool IsNative() const override;
	virtual bool NeedSync() const override;
	
private:
	
	std::vector<JSON> commands;
};

class ScriptNative : public ScriptBase {
public:
	
	ScriptNative(void (*run)(const JSON&), bool needSync);
	virtual ~ScriptNative() override = default;
	
	virtual void Run(const JSON& args) const override;
	virtual bool NeedSync() const override;
	
private:
	
	void (*_Run)(const JSON&);
	const bool needSync;
};

#endif

