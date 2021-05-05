
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef FILE_SYSTEM_CPP
#define FILE_SYSTEM_CPP

#include <irrlicht/irrlicht.h>

#include <streambuf>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include <StlStreamExtension.h>
#include <StdUtil.hpp>

#include <Engine.h>
#include <FileSystem.h>

IFile::IFile(IFile &other) :
	std::istream(other.stream?other.stream->rdbuf():NULL) {
	stream = other.stream;
}
IFile::IFile(std::shared_ptr<std::istream> stream) :
	std::istream(stream?stream->rdbuf():NULL) {
	this->stream = stream;
}

OFile::OFile(OFile &other) :
	std::ostream(other.stream?other.stream->rdbuf():NULL) {
	stream = other.stream;
}
OFile::OFile(std::shared_ptr<std::ostream> stream) {
	this->stream = stream;
}
void OFile::flush() const {
	if(stream)
		stream->flush();
}


FileSystem::FileSystem() {
}
FileSystem::~FileSystem() {
}
IFile FileSystem::ReadFile(std::string name) {
	if(GetExtension(name) == ".gz") {
		std::shared_ptr<igzfstream> file(new igzfstream(name.c_str()));
		if(file->good() && !file->eof())
			return IFile(file);
	}
	{
		std::shared_ptr<std::ifstream> file(new std::ifstream(name.c_str(),
				std::ios::binary));
		if(file->good() && !file->eof())
			return IFile(file);
	}
	std::shared_ptr<iirrfstream> file(new iirrfstream(
			sing::device->getFileSystem()->createAndOpenFile(name.c_str())));
	if(file)
		if(file->good() && !file->eof())
			return IFile(file);
	return IFile(NULL);
}
OFile FileSystem::WriteFile(std::string name) {
	if(GetExtension(name) == ".gz") {
		std::shared_ptr<ogzfstream> file(new ogzfstream(name.c_str()));
		if(file->good())
			return OFile(file);
	}
	{
		std::shared_ptr<std::ofstream> file(new std::ofstream(name.c_str(),
				std::ios::binary));
		if(file->good())
			return OFile(file);
	}
	std::shared_ptr<oirrfstream> file(new oirrfstream(
			sing::device->getFileSystem()->createAndWriteFile(name.c_str())));
	if(file)
		if(file->good())
			return OFile(file);
	return OFile(NULL);
}
JSON FileSystem::ReadJSON(std::string name) {
	IFile file = ReadFile(name);
	if(file) {
		return JSON(file);
	}
	return JSON();
}
void FileSystem::WriteJSON(const JSON& json, std::string name) {
	OFile file = WriteFile(name);
	if(file) {
		return json.Write(file);
	}
}

#endif
