
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CAMERA_H
#define CAMERA_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <irrlicht\irrlicht.h>

#include "..\lib\Debug.h"
#include <memory>

class Camera
{
private:
	
	irr::video::ITexture * target;
	irr::scene::ICameraSceneNode * sceneNode;
	
	btVector3 position;
	btVector3 rotation;
	btTransform parentTransformation;
	
	btVector3 currentLocation;
	
	class Engine * engine;
	
	
	void UpdateCameraView();
	
public:
	
	void UseTarget();
	irr::video::ITexture * GetTexture();
	void RenderToThis();
	bool IsMainTarget();
	
	irr::scene::ICameraSceneNode * GetCameraNode();
	
	btTransform GetTransform() const;
	btQuaternion GetRotation() const;
	btQuaternion GetFlatRotation() const;
	
	btVector3 GetUpVector() const;
	btVector3 GetFlatRightVector() const;
	btVector3 GetRightVector() const;
	btVector3 GetFlatLeftVector() const;
	btVector3 GetLeftVector() const;
	btVector3 GetFlatForwardVector() const;
	btVector3 GetForwardVector() const;
	
	btVector3 GetPosition() const;
	btVector3 GetEulerRotation() const;
	btVector3 GetWorldPosition() const;
	
	void RotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth );
	void SetRelativePosition( btVector3 src );
	void SetRotation( btVector3 src );
	void Move( btVector3 src );
	void Rotate( btVector3 src );
	void SetCameraParentTransform( btTransform transform );
	
	Camera( Engine * engine, bool textured, unsigned w, unsigned h, irr::scene::ICameraSceneNode * cameraNode );
	~Camera();
};

#endif

