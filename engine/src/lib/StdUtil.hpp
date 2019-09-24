
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
	return name.substr( 0, dot );
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

#endif

