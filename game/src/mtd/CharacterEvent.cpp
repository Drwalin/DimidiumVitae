
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_EVENT_CPP
#define CHARACTER_EVENT_CPP

#include "..\css\Character.h"

#include <Engine.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

void Character::EventRotateCameraBy( const btVector3 & rotation )
{
	this->cameraRotation += rotation;
	if( this->camera )
	{
		this->CorrectCameraRotation();
		this->camera->SetRotation( this->cameraRotation );
	}
}

void Character::EventRotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth )
{
	this->CorrectCameraRotation();
	
	btVector3 dstCameraRotation(0,0,0);
	{
		btVector3 dstLookingDirection = (worldPoint - GetCameraLocation()).normalized();
		btVector3 forwardVector = GetForwardVector().normalized();
		btVector3 flatForward = forwardVector;
		flatForward.m_floats[1] = 0.0f;
		flatForward.normalize();
		btVector3 flatDstLookingDirection = dstLookingDirection;
		flatDstLookingDirection.m_floats[1] = 0.0f;
		flatDstLookingDirection.normalize();
		
		float dot = flatForward.normalized().dot( flatDstLookingDirection.normalized() );
		dstCameraRotation.m_floats[1] = ( ( dot <= -1 || dot >= 1 ) ? 0.001f : acos( dot ) ) * ( flatDstLookingDirection.dot( GetFlatLeftVector() ) > 0.0f ? -1.0f : 1.0f );
		dot = forwardVector.normalized().dot( ( Math::MakeTransformFromEuler( btVector3( 0, -dstCameraRotation.y(), 0 ) ) * dstLookingDirection ).normalized() );
		dstCameraRotation.m_floats[0] = ( ( dot <= -1 || dot >= 1 ) ? 0.001f : acos( dot ) ) * ( dstLookingDirection.y() > forwardVector.y() ? -1.0f : 1.0f );
	}
	
	if( smooth )
	{
		this->cameraRotation += dstCameraRotation * this->engine->GetDeltaTime() * 3.11f * Math::PI;
	}
	else
	{
		this->cameraRotation += dstCameraRotation;
	}
	
	this->CorrectCameraRotation();
	
	if( this->camera )
	{
		this->camera->SetRotation( cameraRotation );
	}
}

#endif

