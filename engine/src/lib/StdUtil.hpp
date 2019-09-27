
#ifndef STD_UTIL_HPP
#define STD_UTIL_HPP

#include <string>
#include <iostream>

inline std::istream & GetLine( std::istream & stream, std::string & line )
{
	std::getline( stream, line );
	if( line.size() && line[line.size()-1] == 13 )
		line.resize( line.size() - 1 );
	return stream;
}

inline std::string GetCoreName( const std::string & name )
{
	int dot = name.rfind( "." );
	if( dot == std::string::npos )
		return name;
	int dot2 = name.rfind( "/" );
	int dot3 = name.rfind( "\\" );
	if( dot2 != std::string::npos && dot2 > dot )
		return name;
	if( dot3 != std::string::npos && dot3 > dot )
		return name;
	return name.substr( 0, dot );
}

inline std::string GetExtension( const std::string & name )
{
	int dot = name.rfind( "." );
	if( dot == std::string::npos )
		return "";
	int dot2 = name.rfind( "/" );
	int dot3 = name.rfind( "\\" );
	if( dot2 != std::string::npos && dot2 > dot )
		return "";
	if( dot3 != std::string::npos && dot3 > dot )
		return "";
	return std::string( name.c_str() + dot + 1 );
}

inline std::string GetPathWithoutSlash( const std::string & name )
{
	int dot = name.rfind( "/" );
	int dot2 = name.rfind( "\\" );
	if( dot < dot2 )
		dot = dot2;
	if( dot == std::string::npos )
		return name;
	return name.substr( 0, dot );
}

inline std::string operator+( const char * cstring, const std::string & string )
{
	return std::string(cstring) + string;
}

#endif

