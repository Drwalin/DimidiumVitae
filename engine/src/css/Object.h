
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef OBJECT_H
#define OBJECT_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>

#include <string>
#include <set>

#include <Debug.h>
#include <memory>

#include <Model.h>

class Engine;

class Object
{
protected:
	
	Engine * engine;
	
	std::string name;
	
	btTransform currentTransform;
	btTransform previousTransform;
	
	std::shared_ptr<Model> model;
	irr::scene::IAnimatedMeshSceneNode *sceneNode;
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btRigidBody> body;
	
	btVector3 scale;
	
	int rayTraceChannel;
	
	float mass;
	
	std::set< Object* > overlappingInPreviousFrame;
	std::set< Object* > overlappingInCurrentFrame;
	
	std::shared_ptr<Object> thisPtr;
	
	void UpdateTransformSceneNode();
	
public:
	
	virtual bool IsTrigger() const;
	
	virtual void NextOverlappingFrame();
	void OverlapWithObject( Object * other, btPersistentManifold * persisstentManifold );
	
	void SetPosition( const btVector3 & loc );
	void SetRotation( const btQuaternion & quat );
	void Move( const btVector3 & move );
	void Rotate( const btQuaternion & quat );
	
	std::shared_ptr<Object> GetThis();
	
	
	virtual void SetMass( float mass );
	
	virtual bool IsDynamic() const;
	
	Engine * GetEngine();
	
	std::string GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	virtual void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation() const;
	
	std::shared_ptr<btRigidBody> GetBody();
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold );
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold );
	virtual void EventOnObjectEndOverlapp( Object * other );
	
	virtual void Tick( const float deltaTime );
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal );
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal );
	
	virtual void SetModel( std::shared_ptr<Model> model, bool light = true );
	
	Object( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ );
	Object();
	~Object();
};

#endif

