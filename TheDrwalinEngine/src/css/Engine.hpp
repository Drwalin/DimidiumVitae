
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Engine.h"

template < class T >
std::shared_ptr<Object> Engine::AddObject( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform, bool dynamic, btScalar mass, btVector3 inertia )
{
	if( shape && object.find(name) == object.end() )
	{
		if( dynamic && mass > 0 )
			shape->calculateLocalInertia( mass, inertia );
		else
			mass = 0;
		
		btDefaultMotionState* motionState = new btDefaultMotionState( transform );
		std::shared_ptr<btRigidBody> rigidBody( new btRigidBody( mass, motionState, (btCollisionShape*)shape.get(), inertia ) );
		world->AddBody( name, rigidBody );
		rigidBody->setDamping( 0.2, 0.1 );
		rigidBody->setFriction( 0.2 );
		
		std::shared_ptr<Object> obj( new T( this, name, rigidBody, shape, mass ) );
		object[name] = obj;
		
		rigidBody->setUserPointer( (void*)obj.get() );
		
		return obj;
	}
	
	if( !shape )
	{
		DEBUG( std::string("Shape = NULL : ") + name );
	}
	
	if( object.find(name) != object.end() )
	{
		DEBUG( ( std::string( "Trying to spawn object whit name that exist: " ) + name ) );
	}
	
	return std::shared_ptr<Object>();
}

template < class T >
std::shared_ptr<Object> Engine::AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass )
{
	if( object.find(name) == object.end() )
	{
		std::string shapeName = collisionShapeManager->GetFirstAvailableName( name );
		std::shared_ptr<btCollisionShape> shape = collisionShapeManager->GetCylinder( width/2.0, height, shapeName );
		std::shared_ptr<Object> obj = AddObject<T>( name, shape, transform, true, mass, btVector3(0,0,0) );
		if( obj )
		{
			obj->GetBody()->setAngularFactor( btVector3( 0, 0, 0 ) );
			obj->GetBody()->setActivationState( DISABLE_DEACTIVATION );
			obj->GetBody()->setDamping( 0.8, 0.0 );
			obj->GetBody()->setFriction( 0.8 );
		}
		else
		{
			collisionShapeManager->RemoveShape( shape );
		}
		return obj;
	}
	return std::shared_ptr<Object>();
}

template < class T >
std::shared_ptr<Object> Engine::AddTrigger( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	if( shape && object.find(name) == object.end() )
	{
		float mass = 1.0f;
		btVector3 inertia(0.0f,0.0f,0.0f);
		
		btDefaultMotionState* motionState = new btDefaultMotionState( transform );
		std::shared_ptr<btRigidBody> rigidBody( new btRigidBody( mass, motionState, (btCollisionShape*)shape.get(), inertia ) );
		world->AddBody( name, rigidBody );
		rigidBody->setDamping( 1.0, 1.0 );
		rigidBody->setFriction( 0.0 );
		rigidBody->setAngularFactor( 0.0 );
		rigidBody->setLinearFactor( btVector3( 0.0, 0.0, 0.0 ) );
		rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | btCollisionObject::CollisionFlags::CF_KINEMATIC_OBJECT );
		//body->setCollisionFlags( body->getCollisionFlags() & (~(int(btCollisionObject::CollisionFlags::CF_STATIC_OBJECT))) );
		rigidBody->setGravity( btVector3(0.0f,0.0f,0.0f) );
		
		std::shared_ptr<Object> obj( new T( this, name, rigidBody, shape, mass ) );
		object[name] = obj;
		
		rigidBody->setUserPointer( (void*)obj.get() );
		
		return obj;
	}
	
	if( !shape )
	{
		DEBUG( std::string("Shape = NULL : ") + name );
	}
	
	if( object.find(name) != object.end() )
	{
		DEBUG( ( std::string( "Trying to spawn trigger whit name that exist: " ) + name ) );
	}
	
	return std::shared_ptr<Object>();
}

#endif

