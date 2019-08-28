
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CAMERA_H
#define CAMERA_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <Debug.h>
#include <memory>

#include <Object.h>

class Camera
{
private:
	
	irr::video::ITexture * target;
	irr::scene::ICameraSceneNode * sceneNode;
	
	btVector3 pos;
	btVector3 rot;
	btTransform parentTransformation;
	btVector3 locationScale;
	
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
	
	btVector3 GetFlatRightVector() const;
	btVector3 GetRightVector() const;
	btVector3 GetFlatForwardVector() const;
	btVector3 GetForwardVector() const;
	btVector3 GetUpVector() const;
	
	btVector3 GetLocation() const;
	
	btVector3 GetPos() const;
	btVector3 GetRot() const;
	
	void SetPos( btVector3 src );
	void SetRotation( btVector3 src );
	void Move( btVector3 src );
	void Rotate( btVector3 src );
	void SetCameraTransform( btTransform transform );
	
	Camera( Engine * engine, bool textured, unsigned w, unsigned h, irr::scene::ICameraSceneNode * cameraNode );
	~Camera();
};

#endif

