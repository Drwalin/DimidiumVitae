
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STL_STREAM_EXTENSION_H
#define STL_STREAM_EXTENSION_H

#include <streambuf>
#include <iostream>

class __base_ostreambuf : public std::streambuf
{
public:
	
	virtual std::streamsize xsputn( const char * buffer, std::streamsize buffersize ) = 0;
	int overflow( int ch );
};

class __base_istreambuf : public std::streambuf
{
protected:
	
	int buffersize;
	char * buffer;
	
public:
	
	virtual int __read_to_buffer() = 0;
	virtual int underflow() override;
	__base_istreambuf( const int buffersize = 4096 );
	~__base_istreambuf();
};

class ocstreambuf : public __base_ostreambuf
{
private:
	
	struct _iobuf * filefd;
	
public:
	
	virtual std::streamsize xsputn( const char * buffer, std::streamsize buffersize ) override;
	ocstreambuf( const char * filename );
	~ocstreambuf();
};

class icstreambuf : public __base_istreambuf
{
private:
	
	struct _iobuf * filefd;
	
public:
	
	virtual int __read_to_buffer() override;
	icstreambuf( const char * filename );
	~icstreambuf();
};

class ogzstreambuf : public __base_ostreambuf
{
private:
	
	struct gzFile_s * filefd;
	
public:
	
	virtual std::streamsize xsputn( const char * buffer, std::streamsize buffersize ) override;
	ogzstreambuf( const char * filename );
	~ogzstreambuf();
};

class igzstreambuf : public __base_istreambuf
{
private:
	
	struct gzFile_s * filefd;
	
public:
	
	virtual int __read_to_buffer() override;
	igzstreambuf( const char * filename );
	~igzstreambuf();
};

namespace irr
{
	namespace io
	{
		class IWriteFile;
		class IReadFile;
	};
};

class oirrstreambuf : public __base_ostreambuf
{
private:
	
	irr::io::IWriteFile * file;
	
public:
	
	virtual std::streamsize xsputn( const char * buffer, std::streamsize buffersize ) override;
	oirrstreambuf( irr::io::IWriteFile * file );
	~oirrstreambuf();
};

class iirrstreambuf : public __base_istreambuf
{
private:
	
	irr::io::IReadFile * file;
	
public:
	
	virtual int __read_to_buffer() override;
	iirrstreambuf( irr::io::IReadFile * file );
	~iirrstreambuf();
};

template < typename T, typename Tstream, typename... Args >
class __t_iostream : public Tstream
{
private:
	
	T __streambuf;
	
public:
	
	__t_iostream( Args... args ) :
		__streambuf(args...), Tstream(&__streambuf)
	{
	}
	
	~__t_iostream()
	{
	}
};

typedef __t_iostream<icstreambuf,std::istream,const char*> icfstream;
typedef __t_iostream<ocstreambuf,std::ostream,const char*> ocfstream;

typedef __t_iostream<igzstreambuf,std::istream,const char*> igzfstream;
typedef __t_iostream<ogzstreambuf,std::ostream,const char*> ogzfstream;

typedef __t_iostream<iirrstreambuf,std::istream,class irr::io::IReadFile*> iirrfstream;
typedef __t_iostream<oirrstreambuf,std::ostream,class irr::io::IWriteFile*> oirrfstream;

#endif

