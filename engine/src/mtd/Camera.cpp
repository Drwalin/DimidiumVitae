
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CAMERA_CPP
#define CAMERA_CPP

#include "../css/Camera.h"
#include "../css/Engine.h"

#include "../lib/Math.hpp"

void Camera::UseTarget() {
	if(this->target) {
		this->engine->GetWindow()->GetVideoDriver()->setRenderTarget(this->target, true, true, 0);
	} else {
		this->engine->GetWindow()->GetVideoDriver()->setRenderTarget(0, true, true, 0);
	}
}

irr::video::ITexture *Camera::GetTexture() {
	return this->target;
}

bool Camera::IsMainTarget() {
	if(this->target)
		return false;
	return true;
}

irr::scene::ICameraSceneNode *Camera::GetCameraNode() {
	return this->sceneNode;
}

btTransform Camera::GetTransform() const
{
	return btTransform(this->GetRotation(), this->GetWorldPosition());
}

btQuaternion Camera::GetRotation() const
{
	return this->parentTransformation.getRotation() *Math::MakeQuaternionFromEuler(this->rotation);
}

btQuaternion Camera::GetFlatRotation() const
{
	return this->parentTransformation.getRotation() *btQuaternion(btVector3(0, 1, 0), -this->rotation.y());
}

btVector3 Camera::GetUpVector() const
{
	return btTransform(this->GetRotation()) *btVector3(0, 1, 0);
}

btVector3 Camera::GetFlatRightVector() const
{
	return btTransform(this->GetFlatRotation()) *btVector3(-1, 0, 0);
}

btVector3 Camera::GetRightVector() const
{
	return btTransform(this->GetRotation()) *btVector3(-1, 0, 0);
}

btVector3 Camera::GetFlatLeftVector() const
{
	return btTransform(this->GetFlatRotation()) *btVector3(1, 0, 0);
}

btVector3 Camera::GetLeftVector() const
{
	return btTransform(this->GetRotation()) *btVector3(1, 0, 0);
}

btVector3 Camera::GetFlatForwardVector() const
{
	return btTransform(this->GetFlatRotation()) *btVector3(0, 0, 1);
}

btVector3 Camera::GetForwardVector() const
{
	return btTransform(this->GetRotation()) *btVector3(0, 0, 1);
}

btVector3 Camera::GetPosition() const
{
	return this->position;
}

btVector3 Camera::GetEulerRotation() const
{
	return this->rotation;
}

btVector3 Camera::GetWorldPosition() const
{
	return this->parentTransformation *this->position;
}

void Camera::UpdateCameraView() {
	if(this->rotation.m_floats[0] > Math::PI/2.0)
		this->rotation.m_floats[0] = Math::PI/2.0;
	else if(this->rotation.m_floats[0] < -Math::PI/2.0)
		this->rotation.m_floats[0] = -Math::PI/2.0;
	
	if(this->rotation.m_floats[1] > Math::PI*2.0)
		this->rotation.m_floats[1] -= Math::PI*2.0;
	else if(this->rotation.m_floats[1] < -Math::PI*2.0)
		this->rotation.m_floats[1] += Math::PI*2.0;
	
	if(this->rotation.m_floats[2] > Math::PI*2.0)
		this->rotation.m_floats[2] -= Math::PI*2.0;
	else if(this->rotation.m_floats[2] < -Math::PI*2.0)
		this->rotation.m_floats[2] += Math::PI*2.0;
	
	this->sceneNode->setTarget(Math::GetIrrVec(this->GetWorldPosition() + this->GetForwardVector()));
	this->sceneNode->setUpVector(Math::GetIrrVec(this->GetUpVector()));
	this->sceneNode->setPosition(Math::GetIrrVec(this->GetWorldPosition()));
}

void Camera::RotateCameraToLookAtPoint(const btVector3 &worldPoint, bool smooth) {
	btVector3 dstCameraRotation(0,0,0);
	{
		btVector3 dstLookingDirection = (worldPoint - this->GetWorldPosition()).normalized();
		btVector3 forwardVector = this->GetForwardVector().normalized();
		btVector3 flatForward = forwardVector;
		flatForward.m_floats[1] = 0.0f;
		flatForward.normalize();
		btVector3 flatDstLookingDirection = dstLookingDirection;
		flatDstLookingDirection.m_floats[1] = 0.0f;
		flatDstLookingDirection.normalize();
		
		float dot = flatForward.normalized().dot(flatDstLookingDirection.normalized());
		dstCameraRotation.m_floats[1] = ((dot<=-1 || dot>=1) ? 0.001f : acos(dot))*(flatDstLookingDirection.dot(this->GetFlatLeftVector()) > 0.0f ? -1.0f : 1.0f);
		dot = forwardVector.normalized().dot((Math::MakeTransformFromEuler(btVector3(0, -dstCameraRotation.y(), 0))*dstLookingDirection).normalized());
		dstCameraRotation.m_floats[0] = ((dot<=-1 || dot>=1) ? 0.001f : acos(dot))*(dstLookingDirection.y() > forwardVector.y() ? -1.0f : 1.0f);
	}
	
	if(smooth)
		this->Rotate(dstCameraRotation*this->engine->GetDeltaTime()*3.11f*Math::PI);
	else
		this->Rotate(dstCameraRotation);
}

void Camera::SetRelativePosition(btVector3 src) {
	this->position = src;
	this->UpdateCameraView();
}

void Camera::SetRotation(btVector3 src) {
	this->rotation = src;
	this->UpdateCameraView();
}

void Camera::Move(btVector3 src) {
	this->position += src;
	this->UpdateCameraView();
}

void Camera::Rotate(btVector3 src) {
	this->rotation += src;
	this->UpdateCameraView();
}

void Camera::SetCameraParentTransform(btTransform transform) {
	this->parentTransformation = transform;
	this->UpdateCameraView();
}

Camera::Camera(Engine *engine, bool textured, unsigned w, unsigned h, irr::scene::ICameraSceneNode *cameraNode) {
	this->engine = engine;
	
	this->sceneNode = cameraNode;
	if(textured)
		this->target = this->engine->GetWindow()->GetVideoDriver()->addRenderTargetTexture(irr::core::dimension2d<unsigned>(w, h), "RTT1");
	else
		this->target = NULL;
	
	this->position = btVector3(0, 0, 0);
	this->rotation = btVector3(0, 0, 0);
	this->parentTransformation = btTransform(btQuaternion(btVector3(0, 1, 0), 0), btVector3(0, 0, 0));
}

Camera::~Camera() {
	this->position = btVector3(0, 0, 0);
	this->rotation = btVector3(0, 0, 0);
	this->parentTransformation = btTransform(btQuaternion(btVector3(0, 1, 0), 0), btVector3(0, 0, 0));
}

#endif
