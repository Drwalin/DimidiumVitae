
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STL_STREAM_EXTENSION_CPP
#define STL_STREAM_EXTENSION_CPP

#include "StlStreamExtension.h"

#include <streambuf>

#include <cstdlib>
#include <cstdio>

#include <zlib.h>

#include <irrlicht/irrlicht.h>

int __base_ostreambuf::overflow(int ch) {
	return 1;
}

int __base_istreambuf::underflow() {
	if(gptr() == egptr()) {
		int readed = __read_to_buffer();
		if(readed > 0)
			setg(buffer, buffer, buffer + readed);
	}
	if(gptr() == egptr())
		return std::char_traits<char>::eof();
	return std::char_traits<char>::to_int_type(*gptr());
}
__base_istreambuf::__base_istreambuf(const int buffersize) {
	if(buffersize > 16)
		this->buffersize = buffersize;
	else
		this->buffersize = 4096;
	buffer = (char*)malloc(this->buffersize);
}
__base_istreambuf::~__base_istreambuf() {
	if(buffer) {
		free(buffer);
		buffer = NULL;
	}
}

std::streamsize ocstreambuf::xsputn(const char *buffer, std::streamsize buffersize) {
	if(filefd)
		return fwrite(buffer, 1, buffersize, (FILE*)filefd);
	return 0;
}
ocstreambuf::ocstreambuf(const char *filename) {
	filefd = (_iobuf*)fopen(filename, "wb");
}
ocstreambuf::~ocstreambuf() {
	if(filefd) {
		fclose((FILE*)filefd);
		filefd = NULL;
	}
}
void ocstreambuf::flush() const {
	if(filefd)
		fflush((FILE*)filefd);
}

int icstreambuf::__read_to_buffer() {
	if(filefd)
		return fread(buffer, 1, buffersize, (FILE*)filefd);
	return 0;
}
icstreambuf::icstreambuf(const char *filename) {
	filefd = (_iobuf*)fopen(filename, "rb");
}
icstreambuf::~icstreambuf() {
	if(filefd) {
		fclose((FILE*)filefd);
		filefd = NULL;
	}
}

std::streamsize ogzstreambuf::xsputn(const char *buffer, std::streamsize buffersize) {
	if(filefd)
		return gzwrite(filefd, buffer, buffersize);
	return 0;
}
ogzstreambuf::ogzstreambuf(const char *filename) {
	filefd = gzopen(filename, "wb");
}
ogzstreambuf::~ogzstreambuf() {
	if(filefd) {
		gzclose(filefd);
		filefd = NULL;
	}
}
void ogzstreambuf::flush() const {
	if(filefd)
		gzflush(filefd, Z_FULL_FLUSH);
}

int igzstreambuf::__read_to_buffer() {
	if(filefd)
		return gzread(filefd, buffer, buffersize);
	return 0;
}
igzstreambuf::igzstreambuf(const char *filename) {
	filefd = gzopen(filename, "rb");
}
igzstreambuf::~igzstreambuf() {
	if(filefd) {
		gzclose(filefd);
		filefd = NULL;
	}
}

std::streamsize oirrstreambuf::xsputn(const char *buffer, std::streamsize buffersize) {
	if(file)
		return file->write(buffer, buffersize);
	return 0;
}
oirrstreambuf::oirrstreambuf(irr::io::IWriteFile *file) {
	this->file = file;
}
oirrstreambuf::~oirrstreambuf() {
	if(file) {
		file->drop();
		file = NULL;
	}
}
void oirrstreambuf::flush() const {
}

int iirrstreambuf::__read_to_buffer() {
	if(file)
		return file->read(buffer, buffersize);
	return 0;
}
iirrstreambuf::iirrstreambuf(irr::io::IReadFile *file) {
	this->file = file;
}
iirrstreambuf::~iirrstreambuf() {
	if(file) {
		file->drop();
		file = NULL;
	}
}

#endif

