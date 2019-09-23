
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MATH_ENGINE_HPP
#define MATH_ENGINE_HPP

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <cmath>

#include <string>
#include <iostream>
#include <istream>
#include <ostream>

#include "..\css\GUI.h"

namespace Math
{
	const float PI = 3.14159265359f;
	
	inline irr::core::vector3d<float> GetIrrVec( const btVector3 & vec )
	{
		return irr::core::vector3d<float>( -vec.x(), vec.y(), vec.z() );
	}
	
	template < class T >
	inline btVector3 GetBtVec( const irr::core::vector3d<T> & vec )
	{
		return btVector3( -vec.X, vec.Y, vec.Z );
	}
	
	inline btQuaternion MakeQuaternionFromEuler( const btVector3 & euler )
	{
		btQuaternion quat( btVector3( 0, 1, 0 ), -euler.y() );
		quat *= btQuaternion( btVector3( 1, 0, 0 ), euler.x() );
		quat *= btQuaternion( btVector3( 0, 0, 1 ), euler.z() );
		return quat;
	}
	
	inline btTransform MakeTransformFromEuler( const btVector3 & euler )
	{
		return btTransform( Math::MakeQuaternionFromEuler( euler ) );
	}
};



inline std::istream & operator >> ( std::istream & stream, btVector3 & point )
{
	float x, y, z;
	stream >> x;
	stream >> y;
	stream >> z;
	point.setValue( x, y, z );
	return stream;
}

inline std::istream & operator >> ( std::istream & stream, btQuaternion & rotation )
{
	float x, y, z, w;
	stream >> x;
	stream >> y;
	stream >> z;
	stream >> w;
	rotation = btQuaternion( x, y, z, w );
	return stream;
}

inline std::istream & operator >> ( std::istream & stream, btTransform & trans )
{
	btVector3 origin;
	btQuaternion rotation;
	stream >> origin;
	stream >> rotation;
	trans = btTransform( rotation, origin );
	return stream;
}



inline std::ostream & operator << ( std::ostream & stream, const btVector3 point )
{
	stream << (point.x()) << " ";
	stream << (point.y()) << " ";
	stream << (point.z());
	return stream;
}

inline std::ostream & operator << ( std::ostream & stream, const btQuaternion rotation )
{
	stream << (rotation.x()) << " ";
	stream << (rotation.y()) << " ";
	stream << (rotation.z()) << " ";
	stream << (rotation.w());
	return stream;
}

inline std::ostream & operator << ( std::ostream & stream, const btTransform trans )
{
	stream << (trans.getOrigin()) << " ";
	stream << (trans.getRotation());
	return stream;
}



inline GUI & operator << ( GUI & gui, const btVector3 point )
{
	gui << (point.x()) << " ";
	gui << (point.y()) << " ";
	gui << (point.z());
	return gui;
}

inline GUI & operator << ( GUI & gui, const btQuaternion rotation )
{
	gui << (rotation.x()) << " ";
	gui << (rotation.y()) << " ";
	gui << (rotation.z()) << " ";
	gui << (rotation.w());
	return gui;
}

inline GUI & operator << ( GUI & gui, const btTransform trans )
{
	gui << (trans.getOrigin()) << " ";
	gui << (trans.getRotation());
	return gui;
}

#endif

