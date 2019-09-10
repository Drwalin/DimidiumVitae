
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Trigger.h"

#include <Entity.h>
#include <Camera.h>

#include <cmath>

class Character : public Entity
{
protected:
	
	btVector3 cameraRotation;
	btVector3 cameraLocation;
	
	std::shared_ptr<Camera> camera;
	
	float defaultVelocity, height;
	
	
	float GetMovementVelocity() const;
	btVector3 GetJumpVelocity() const;
	
	void CorrectCameraRotation();
	
	
	float queueStep;
	void QueueMove( float val );
	
public:
	
	virtual void NextOverlappingFrame() override;
	
	float GetBottomY();
	
	static btTransform MakeTransformFromEuler( const btVector3 & euler );
	
	void SetCamera( std::shared_ptr<Camera> camera );
	
	
	virtual void EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * persisstentManifold ) override;
	virtual void EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * persisstentManifold ) override;
	virtual void EventOnEntityEndOverlapp( Entity * other ) override;
	
	void EventJump();
	void EventCrouch();
	void EventStandUp();
	
	void EventMoveInDirection( const btVector3 & direction, bool flat );
	
	void EventRotateCameraBy( const btVector3 & rotation );
	void EventRotateCameraToLookAtPoint( const btVector3 & worldPoint, bool smooth );/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	void SetCameraLocation( const btVector3 & location );
	void SetCameraRotation( const btVector3 & rotation );
	
	btVector3 GetCameraLocation() const;
	btVector3 GetForwardVector() const;
	btVector3 GetFlatForwardVector() const;
	btVector3 GetLeftVector() const;
	btVector3 GetFlatLeftVector() const;
	
	
	virtual void Tick( const float deltaTime ) override;
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal ) override;
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal ) override;
	
	
	
	
	
	
	
	
	virtual void Load( std::istream & stream ) override;
	virtual void Save( std::ostream & stream ) const override;
	virtual void Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform ) override;
	virtual void Despawn() override;
	
	virtual void Destroy() override;
	
	virtual int GetTypeSize() const override;
	virtual void Free() override;
	virtual std::shared_ptr<Entity> New() const override;
	virtual std::string GetClassName() const override;
	
	Character();
	virtual ~Character() override;
};

#endif

