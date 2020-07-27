
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <streambuf>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include <StlStreamExtension.h>
#include <JSON.h>

class IFile : public std::istream {
public:
	
	IFile() = delete;
	IFile(IFile &other);
	IFile(const IFile &other) = delete;
	IFile(std::shared_ptr<std::istream> stream);
	
private:
	
	std::shared_ptr<std::istream> stream;
};

class OFile : public std::ostream {
public:
	
	OFile() = delete;
	OFile(OFile &other);
	OFile(const OFile &other) = delete;
	OFile(std::shared_ptr<std::ostream> stream);
	
	void flush() const;
	
private:
	
	std::shared_ptr<std::ostream> stream;
};

class FileSystem {
public:
	
	FileSystem(class Engine *engine);
	~FileSystem();
	
	IFile ReadFile(std::string name);
	OFile WriteFile(std::string name);
	JSON ReadJSON(std::string name);
	void WriteJSON(JSON json, std::string name);
	
private:
	
	class Engine* engine;
};

#endif
