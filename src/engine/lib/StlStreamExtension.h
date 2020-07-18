
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STL_STREAM_EXTENSION_H
#define STL_STREAM_EXTENSION_H

#include <streambuf>
#include <iostream>

class __base_ostreambuf : public std::streambuf {
public:
	
	virtual std::streamsize xsputn(const char *buffer, std::streamsize buffersize) = 0;
	int overflow(int ch);
};

class __base_istreambuf : public std::streambuf {
public:
	
	virtual int __read_to_buffer() = 0;
	virtual int underflow() override;
	__base_istreambuf(const int buffersize = 4096);
	~__base_istreambuf();
	
protected:
	
	int buffersize;
	char *buffer;
};

class ocstreambuf : public __base_ostreambuf {
public:
	
	virtual std::streamsize xsputn(const char *buffer, std::streamsize buffersize) override;
	ocstreambuf(const char *filename);
	~ocstreambuf();
	
private:
	
	struct _iobuf *filefd;
};

class icstreambuf : public __base_istreambuf {
public:
	
	virtual int __read_to_buffer() override;
	icstreambuf(const char *filename);
	~icstreambuf();
	
private:
	
	struct _iobuf *filefd;
};

class ogzstreambuf : public __base_ostreambuf {
public:
	
	virtual std::streamsize xsputn(const char *buffer, std::streamsize buffersize) override;
	ogzstreambuf(const char *filename);
	~ogzstreambuf();
	
private:
	
	struct gzFile_s *filefd;
};

class igzstreambuf : public __base_istreambuf {
public:
	
	virtual int __read_to_buffer() override;
	igzstreambuf(const char *filename);
	~igzstreambuf();
	
private:
	
	struct gzFile_s *filefd;
};

namespace irr {
	namespace io {
		class IWriteFile;
		class IReadFile;
	};
};

class oirrstreambuf : public __base_ostreambuf {
public:
	
	virtual std::streamsize xsputn(const char *buffer, std::streamsize buffersize) override;
	oirrstreambuf(irr::io::IWriteFile *file);
	~oirrstreambuf();
	
private:
	
	irr::io::IWriteFile *file;
};

class iirrstreambuf : public __base_istreambuf {
public:
	
	virtual int __read_to_buffer() override;
	iirrstreambuf(irr::io::IReadFile *file);
	~iirrstreambuf();
	
private:
	
	irr::io::IReadFile *file;
};

template <typename T, typename Tstream, typename... Args >
class __t_iostream : public Tstream {
public:
	
	__t_iostream(Args... args) :
		__streambuf(args...), Tstream(&__streambuf) {
	}
	
	~__t_iostream() {
	}
	
private:
	
	T __streambuf;
};

typedef __t_iostream<icstreambuf,std::istream,const char*> icfstream;
typedef __t_iostream<ocstreambuf,std::ostream,const char*> ocfstream;

typedef __t_iostream<igzstreambuf,std::istream,const char*> igzfstream;
typedef __t_iostream<ogzstreambuf,std::ostream,const char*> ogzfstream;

typedef __t_iostream<iirrstreambuf,std::istream,class irr::io::IReadFile*> iirrfstream;
typedef __t_iostream<oirrstreambuf,std::ostream,class irr::io::IWriteFile*> oirrfstream;

#endif

