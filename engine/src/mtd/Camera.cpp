
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CAMERA_CPP
#define CAMERA_CPP

#include "..\css\Camera.h"
#include "..\css\Engine.h"

#include "..\lib\Math.hpp"

void Camera::UseTarget()
{
	if( target )
	{
		engine->GetWindow()->GetVideoDriver()->setRenderTarget( target, true, true, 0 );
	}
	else
	{
		engine->GetWindow()->GetVideoDriver()->setRenderTarget( 0, true, true, 0 );
	}
}

irr::video::ITexture * Camera::GetTexture()
{
	return target;
}

bool Camera::IsMainTarget()
{
	if( target )
		return false;
	return true;
}

irr::scene::ICameraSceneNode * Camera::GetCameraNode()
{
	return sceneNode;
}

btTransform Camera::GetTransform() const
{
	return btTransform
		(
			GetRotation(),
			GetLocation()
		);
}

btQuaternion Camera::GetRotation() const
{
	return 
			btQuaternion( btVector3(0,1,0), rot.y() ) *
			btQuaternion( btVector3(1,0,0), rot.x() ) *
			btQuaternion( btVector3(0,0,1), rot.z() ) *
			parentTransformation.getRotation();
}

btQuaternion Camera::GetFlatRotation() const
{
	return parentTransformation.getRotation() *
			btQuaternion( btVector3(0,1,0), rot.y() );
}

btVector3 Camera::GetUpVector() const
{
	return btTransform( GetRotation() ) * btVector3( 0, 1, 0 );
}

btVector3 Camera::GetFlatRightVector() const
{
	return btTransform( GetFlatRotation() ) * btVector3( -1, 0, 0 );
}

btVector3 Camera::GetRightVector() const
{
	return btTransform( GetRotation() ) * btVector3( -1, 0, 0 );
}

btVector3 Camera::GetFlatForwardVector() const
{
	return btTransform( GetFlatRotation() ) * btVector3( 0, 0, 1 );
}

btVector3 Camera::GetForwardVector() const
{
	return btTransform( GetRotation() ) * btVector3( 0, 0, 1 );
}

btVector3 Camera::GetPos() const
{
	return pos;
}

btVector3 Camera::GetRot() const
{
	return rot;
}

btVector3 Camera::GetLocation() const
{
	return parentTransformation.getOrigin() + pos;
}

void Camera::UpdateCameraView()
{
	sceneNode->setTarget( Math::GetIrrVec( ( GetLocation() * btVector3(-1,1,1) ) + ( btTransform(btQuaternion(btVector3(0,1,0),Math::PI)) * GetForwardVector() ) ) );
	sceneNode->setUpVector( Math::GetIrrVec( ( btTransform(btQuaternion(btVector3(0,1,0),Math::PI)) * GetUpVector() ) ) );
	sceneNode->setPosition( Math::GetIrrVec( GetLocation() * btVector3(-1,1,1) ) );
}

void Camera::SetPos( btVector3 src )
{
	pos = src;
	UpdateCameraView();
}

void Camera::SetRotation( btVector3 src )
{
	rot = src;
	
	if( rot.x() > Math::PI / 2.0 )
		rot.m_floats[0] = Math::PI / 2.0;
	else if( rot.x() < -Math::PI / 2.0 )
		rot.m_floats[0] = -Math::PI / 2.0;
	
	if( rot.y() > Math::PI * 2.0 )
		rot.m_floats[1] -= Math::PI * 2.0;
	else if( rot.y() < -Math::PI * 2.0 )
		rot.m_floats[1] += Math::PI * 2.0;
	
	if( rot.z() > Math::PI * 2.0 )
		rot.m_floats[2] -= Math::PI * 2.0;
	else if( rot.z() < -Math::PI * 2.0 )
		rot.m_floats[2] += Math::PI * 2.0;
	
	UpdateCameraView();
}

void Camera::Move( btVector3 src )
{
	pos += src;
	
	UpdateCameraView();
}

void Camera::Rotate( btVector3 src )
{
	rot += src;
	
	if( rot.x() > Math::PI / 2.0 )
		rot.m_floats[0] = Math::PI / 2.0;
	else if( rot.x() < -Math::PI / 2.0 )
		rot.m_floats[0] = -Math::PI / 2.0;
	
	if( rot.y() > Math::PI * 2.0 )
		rot.m_floats[1] -= Math::PI * 2.0;
	else if( rot.y() < -Math::PI * 2.0 )
		rot.m_floats[1] += Math::PI * 2.0;
	
	if( rot.z() > Math::PI * 2.0 )
		rot.m_floats[2] -= Math::PI * 2.0;
	else if( rot.z() < -Math::PI * 2.0 )
		rot.m_floats[2] += Math::PI * 2.0;
	
	UpdateCameraView();
}

void Camera::SetCameraTransform( btTransform transform )
{
	parentTransformation = transform;
	
	UpdateCameraView();
}

Camera::Camera( Engine * engine, bool textured, unsigned w, unsigned h, irr::scene::ICameraSceneNode * cameraNode )
{
	this->engine = engine;
	
	sceneNode = cameraNode;
	if( textured )
		target = engine->GetWindow()->GetVideoDriver()->addRenderTargetTexture( irr::core::dimension2d<unsigned>( w, h ), "RTT1" );
	else
		target = NULL;
	
	pos = btVector3(0,0,0);
	rot = btVector3(0,0,0);
	locationScale = btVector3(1,1,1);
	parentTransformation = btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) );
}

Camera::~Camera()
{
	pos = btVector3(0,0,0);
	rot = btVector3(0,0,0);
	locationScale = btVector3(0,0,0);
	parentTransformation = btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) );
}

#endif

