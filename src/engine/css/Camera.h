
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CAMERA_H
#define CAMERA_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>

#include <irrlicht/irrlicht.h>

#include "Singleton.h"

#include "../lib/Debug.h"
#include <memory>

class Camera {
public:
	
	Camera(bool textured, unsigned w, unsigned h,
			irr::scene::ICameraSceneNode *cameraNode);
	~Camera();
	
	void UseTarget();
	irr::video::ITexture *GetTexture();
	void RenderToThis();
	bool IsMainTarget();
	
	irr::scene::ICameraSceneNode *GetCameraNode();
	
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
	
	void RotateCameraToLookAtPoint(const btVector3 &worldPoint, bool smooth);
	void SetRelativePosition(btVector3 src);
	void SetRotation(btVector3 src);
	void Move(btVector3 src);
	void Rotate(btVector3 src);
	void SetCameraParentTransform(btTransform transform);
	
	void SetFOV(float fovDegrees);
	void SetRenderTargetSize(unsigned width, unsigned height);
	
private:
	
	void UpdateCameraView();
	
private:
	
	irr::video::ITexture *target;
	irr::scene::ICameraSceneNode *sceneNode;
	
	btVector3 position;
	btVector3 rotation;
	btTransform parentTransformation;
	
	btVector3 currentLocation;
	
	float fovDegrees;
};

#endif

