
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef OBJECT_H
#define OBJECT_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <string>
#include <set>
#include <iostream>

#include "..\lib\Debug.h"
#include <memory>

#include "Model.h"

class Engine;

class Object
{
protected:
	
	Engine * engine;
	
	std::string name;
	
	btTransform currentTransform;
	
	std::shared_ptr<Model> model;
	irr::scene::IAnimatedMeshSceneNode * sceneNode;
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btCollisionObject> body;
	
	btVector3 scale;
	
	int rayTraceChannel;
	
	float mass;
	
	std::set< Object* > overlappingInPreviousFrame;
	std::set< Object* > overlappingInCurrentFrame;
	
	void UpdateTransformSceneNode();
	
public:
	
	virtual void NextOverlappingFrame();
	void OverlapWithObject( Object * other, btPersistentManifold * persisstentManifold );
	
	void SetTransform( const btTransform & transform );
	void SetPosition( const btVector3 & loc );
	void SetRotation( const btQuaternion & quat );
	void Move( const btVector3 & move );
	void Rotate( const btQuaternion & quat );
	
	
	void SetMass( float mass );
	
	Engine * GetEngine();
	
	const std::string & GetName() const;
	
	void SetRayTraceChannel( int src );
	int GetRayTraceChannel();
	
	virtual void SetScale( btVector3 scale );
	btVector3 GetScale();
	
	btTransform GetTransform();
	btVector3 GetLocation() const;
	
	template < typename T = btRigidBody >
	std::shared_ptr<T> GetBody() { return std::dynamic_pointer_cast<T>( body ); }
	
	virtual void EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold );
	virtual void EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold );
	virtual void EventOnObjectEndOverlapp( Object * other );
	
	virtual void Tick( const float deltaTime );
	virtual void ApplyDamage( const float damage, btVector3 point, btVector3 normal );
	virtual void ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal );
	
	void SetModel( std::shared_ptr<Model> model, bool light = true );
	void SetBody( std::shared_ptr<btCollisionObject> body, std::shared_ptr<btCollisionShape> shape );
	
	void Init( Engine * engine );
	
	virtual void Load( std::istream & stream );
	virtual void Save( std::ostream & stream ) const;
	virtual void Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform );
	virtual void Despawn();
	
	virtual void Destroy();
	void DestroyBody();
	void DestroySceneNode();
	
	virtual int GetTypeSize() const = 0;								// return size of type in bytes
	virtual void Free() = 0;											// calls type destructor and frees an object
	virtual std::shared_ptr<Object> New() const = 0;					// allocate memory and calls type constructor
	virtual std::string GetClassName() const = 0;						// returns type name
	
	Object();
	virtual ~Object();
};

#endif

