
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_H
#define CHARACTER_H

#include <Object.h>
#include <Trigger.h>
#include <Camera.h>
#include <cmath>

class Character : public Object
{
protected:
	
	enum WalkMode
	{
		CROUCH,
		STRAVAGE,
		WALK,
		RUN
	};
	
	btVector3 cameraRotation;
	btVector3 cameraLocation;
	
	btVector3 previousVelocity;
	
	float height;
	
	float defaultVelocity;
	float jumpHeight;
	
	bool isInAir;
	WalkMode walkMode;
	WalkMode previousWalkMode;
	
	std::shared_ptr<Camera> camera;
	
	float lastTimeInAir;
	
	float GetMovementVelocity() const;
	btVector3 GetJumpVelocity() const;
	
	void CorrectCameraRotation();
	
	
	std::shared_ptr<Object> walkTriggerBottom;		// directly under feets ; this->height = 0.2
	std::shared_ptr<Object> walkTriggerBody;		// a little bit in front of face, not including feets ; this->height = parent->height * 0.915
	std::shared_ptr<Object> walkTriggerStep;		// a little bit in front of faec, only feets ; this->height = parent->height * 0.085
	
	void SpawnWalkTriggers();
	void UpdateStepUp();
	void UpdateIsInAir();
	void UpdateWalkTriggersLocation( const float deltaTime );
	
	
	float queueStep;
	void QueueMove( float val );
	
public:
	
	virtual void SetScale( btVector3 scale ) override;
	
	float GetCurrentHeight() const;
	
	virtual void NextOverlappingFrame() override;
	
	float GetBottomY() const;
	
	static btTransform MakeTransformFromEuler( const btVector3 & euler );
	
	void SetCamera( std::shared_ptr<Camera> camera );
	
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold ) override;
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold ) override;
	virtual void EventOnObjectEndOverlapp( Object * other ) override;
	
	void EventJump();
	void EventCrouch();
	void EventStandUp();
	
	void EventBeginRun();
	void EventStopRun();
	void EventBeginStravage();
	void EventStopStravage();
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
	
	Character( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ );
	Character();
	~Character();
};

#endif

