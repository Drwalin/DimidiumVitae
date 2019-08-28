
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

namespace Math
{
	const float PI = 3.14159265359f;
	
	inline irr::core::vector3d<float> GetIrrVec( const btVector3 & vec )
	{
		return irr::core::vector3d<float>( vec.x(), vec.y(), vec.z() );
	}
	
	template < class T >
	inline btVector3 GetBtVec( const irr::core::vector3d<T> & vec )
	{
		return btVector3( vec.X, vec.Y, vec.Z );
	}
	
	inline irr::core::quaternion GetIrrQuaternion( const btQuaternion & quat )
	{
	    irr::core::quaternion q;
	    q.fromAngleAxis( quat.getAngle(), Math::GetIrrVec( quat.getAxis() ) );
		return q;
	}
	
	inline irr::core::quaternion GetIrrQuaternion( const btTransform & trans )
	{
	    irr::core::quaternion q;
	    q.fromAngleAxis( trans.getRotation().getAngle(), Math::GetIrrVec( trans.getRotation().getAxis() ) );
		return q;
	}
	
	inline irr::core::vector3d<float> GetIrrVec( const btTransform & trans )
	{
	    return Math::GetIrrVec( trans.getOrigin() );
	}
	
};

#endif

