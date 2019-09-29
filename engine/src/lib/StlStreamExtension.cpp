
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef STL_STREAM_EXTENSION_CPP
#define STL_STREAM_EXTENSION_CPP

#include "StlStreamExtension.h"

#include <streambuf>

#include <cstdlib>
#include <cstdio>

#include <zlib.h>

#include <irrlicht/irrlicht.h>

int __base_ostreambuf::overflow( int ch )
{
	return 1;
}

int __base_istreambuf::underflow()
{
	if( this->gptr() == this->egptr() )
	{
		int readed = this->__read_to_buffer();
		if( readed > 0 )
			this->setg( this->buffer, this->buffer, this->buffer + readed );
	}
	if( this->gptr() == this->egptr() )
		return std::char_traits<char>::eof();
	return std::char_traits<char>::to_int_type( *this->gptr() );
}

__base_istreambuf::__base_istreambuf( const int buffersize )
{
	if( buffersize > 16 )
		this->buffersize = buffersize;
	else
		this->buffersize = 4096;
	this->buffer = (char*)malloc( this->buffersize );
}

__base_istreambuf::~__base_istreambuf()
{
	if( this->buffer )
	{
		free( this->buffer );
		this->buffer = NULL;
	}
}

std::streamsize ocstreambuf::xsputn( const char * buffer, std::streamsize buffersize )
{
	if( this->filefd )
		return fwrite( buffer, 1, buffersize, this->filefd );
	return 0;
}

ocstreambuf::ocstreambuf( const char * filename )
{
	this->filefd = fopen( filename, "wb" );
}

ocstreambuf::~ocstreambuf()
{
	if( this->filefd )
	{
		fclose( this->filefd );
		this->filefd = NULL;
	}
}

int icstreambuf::__read_to_buffer()
{
	if( this->filefd )
		return fread( this->buffer, 1, this->buffersize, this->filefd );
	return 0;
}

icstreambuf::icstreambuf( const char * filename )
{
	this->filefd = fopen( filename, "rb" );
}

icstreambuf::~icstreambuf()
{
	if( this->filefd )
	{
		fclose( this->filefd );
		this->filefd = NULL;
	}
}

std::streamsize ogzstreambuf::xsputn( const char * buffer, std::streamsize buffersize )
{
	if( this->filefd )
		return gzwrite( this->filefd, buffer, buffersize );
	return 0;
}

ogzstreambuf::ogzstreambuf( const char * filename )
{
	this->filefd = gzopen( filename, "wb" );
}

ogzstreambuf::~ogzstreambuf()
{
	if( this->filefd )
	{
		gzclose( this->filefd );
		this->filefd = NULL;
	}
}

int igzstreambuf::__read_to_buffer()
{
	if( this->filefd )
		return gzread( this->filefd, this->buffer, this->buffersize );
	return 0;
}

igzstreambuf::igzstreambuf( const char * filename )
{
	this->filefd = gzopen( filename, "rb" );
}

igzstreambuf::~igzstreambuf()
{
	if( this->filefd )
	{
		gzclose( this->filefd );
		this->filefd = NULL;
	}
}

std::streamsize oirrstreambuf::xsputn( const char * buffer, std::streamsize buffersize )
{
	if( this->file )
		return this->file->write( buffer, buffersize );
	return 0;
}

oirrstreambuf::oirrstreambuf( irr::io::IWriteFile * file )
{
	this->file = file;
}

oirrstreambuf::~oirrstreambuf()
{
	if( this->file )
	{
		this->file->drop();
		this->file = NULL;
	}
}

int iirrstreambuf::__read_to_buffer()
{
	if( this->file )
		return this->file->read( this->buffer, this->buffersize );
	return 0;
}

iirrstreambuf::iirrstreambuf( irr::io::IReadFile * file )
{
	this->file = file;
}

iirrstreambuf::~iirrstreambuf()
{
	if( this->file )
	{
		this->file->drop();
		this->file = NULL;
	}
}

#endif

