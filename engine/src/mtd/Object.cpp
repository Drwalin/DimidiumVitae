
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef OBJECT_CPP
#define OBJECT_CPP

#include <Object.h>
#include <Engine.h>

#include <Math.hpp>

#include <cassert>

bool Object::IsTrigger() const
{
	return false;
}

void Object::UpdateTransformSceneNode()
{
	DEBUG( "Debug does objects updates sceneNode transform" );
	if( sceneNode )
	{
		sceneNode->setPosition( Math::GetIrrVec( currentTransform ) * irr::core::vector3d<float>(-1,1,1) );
		
		irr::core::vector3d<float> eulerRadians; 
		Math::GetIrrQuaternion( btQuaternion( currentTransform.getRotation().getAxis() * btVector3(1,-1,-1), currentTransform.getRotation().getAngle() ) ).toEuler( eulerRadians );
		sceneNode->setRotation( eulerRadians * irr::core::RADTODEG );
	}
}

void Object::SetPosition( const btVector3 & loc )
{
	currentTransform = btTransform( currentTransform.getRotation(), loc );
	previousTransform = currentTransform;
	
	if( body )
	{
		body->activate( true );
		//body->setCenterOfMassTransform( currentTransform );
		body->getMotionState()->setWorldTransform( currentTransform );
		//body->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForObject( body );
		body->activate( true );
	}
}

void Object::SetRotation( const btQuaternion & quat )
{
	currentTransform = btTransform( quat, currentTransform.getOrigin() );
	previousTransform = currentTransform;
	
	if( body )
	{
		body->activate( true );
		//body->setCenterOfMassTransform( currentTransform );
		body->getMotionState()->setWorldTransform( currentTransform );
		//body->setWorldTransform( currentTransform );
		engine->GetWorld()->UpdateColliderForObject( body );
		body->activate( true );
	}
}

void Object::Move( const btVector3 & move )
{
	SetPosition( currentTransform.getOrigin() + move );
}

void Object::Rotate( const btQuaternion & quat )
{
	SetRotation( currentTransform.getRotation() * quat );
}

void Object::NextOverlappingFrame()
{
	for( auto it = overlappingInPreviousFrame.begin(); it != overlappingInPreviousFrame.end(); ++it )
	{
		if( overlappingInCurrentFrame.find( *it ) == overlappingInCurrentFrame.end() )
		{
			EventOnObjectEndOverlapp( *it );
		}
	}
	
	overlappingInPreviousFrame = overlappingInCurrentFrame;
	overlappingInCurrentFrame.clear();
}

void Object::OverlapWithObject( Object * other, btPersistentManifold * persisstentManifold )
{
	if( other != this )
	{
		if( persisstentManifold )
		{
			if( other )
			{
				if( overlappingInPreviousFrame.find( other ) != overlappingInPreviousFrame.end() )
				{
					EventOnObjectTickOverlapp( other, persisstentManifold );
				}
				else
				{
					EventOnObjectBeginOverlapp( other, persisstentManifold );
				}
				overlappingInCurrentFrame.insert( other );
			}
			else
			{
				MESSAGE( "other = NULL" );
			}
		}
		else
		{
			MESSAGE( "perisstentManifold = NULL" );
		}
	}
	else
	{
		MESSAGE( std::string("Trying to collide with my self: ") + GetName() );
	}
}

void Object::EventOnObjectBeginOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
}

void Object::EventOnObjectTickOverlapp( Object * other, btPersistentManifold * persisstentManifold )
{
}

void Object::EventOnObjectEndOverlapp( Object * other )
{
}

std::shared_ptr<Object> Object::GetThis()
{
	if( !thisPtr )
	{
		if( engine )
		{
			thisPtr = engine->GetObject( std::string(name) );
		}
		else
		{
			DEBUG("Error creating std::shared_ptr Object::thisPtr ");
		}
	}
	return thisPtr;
}



void Object::Tick( const float deltaTime )
{
	if( body )
	{
		previousTransform = currentTransform;
		body->getMotionState()->getWorldTransform( currentTransform );
	}
	
	UpdateTransformSceneNode();
}

void Object::ApplyDamage( const float damage, btVector3 point, btVector3 normal )
{
}

void Object::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	if( body )
	{
		if( normal.dot( direction ) > 0 )
			normal *= -1;
		
		body->activate( true );
		body->applyImpulse( direction.normalized() * impetus,  point - currentTransform.getOrigin() );
		body->activate( true );
	}
}

void Object::SetMass( float mass )
{
	if( IsDynamic() )
	{
		this->mass = mass;
		if( body )
		{
			btCollisionShape * shape = body->getCollisionShape();
			if( shape )
			{
				btVector3 inertia = body->getInvInertiaDiagLocal();
				shape->calculateLocalInertia( mass, inertia );
				body->setMassProps( mass, inertia );
			}
		}
	}
}

bool Object::IsDynamic() const
{
	if( body )
	{
		if( mass > 0.0f )
		{
			return true;
		}
	}
	return false;
}

Engine * Object::GetEngine()
{
	return engine;
}

void Object::SetScale( btVector3 scale )
{
	this->scale = scale;
	if( body )
	{
		body->activate( true );
		body->getCollisionShape()->setLocalScaling( scale );
		engine->GetWorld()->UpdateColliderForObject( body );
		body->activate( true );
	}
	if( sceneNode )
	{
		sceneNode->setScale( Math::GetIrrVec( scale ) );
	}
}

btVector3 Object::GetScale()
{
	return scale;
}

btTransform Object::GetTransform()
{
	btTransform transform;
	if( body )
		body->getMotionState()->getWorldTransform( transform );
	return transform;
}

std::shared_ptr<btRigidBody> Object::GetBody()
{
	return body;
}

btVector3 Object::GetLocation() const
{
	if( body )
	{
		btTransform transform;
		body->getMotionState()->getWorldTransform( transform );
		return transform.getOrigin();
	}
	return btVector3();
}

void Object::SetRayTraceChannel( int src )
{
	rayTraceChannel = src;
}

int Object::GetRayTraceChannel()
{
	return rayTraceChannel;
}

std::string Object::GetName() const
{
	return name;
}

void Object::SetModel( std::shared_ptr<Model> model, bool light )
{
	if( engine )
	{
		if( engine->GetWindow() )
		{
			if( sceneNode )
			{
				engine->GetWindow()->sceneManager->addToDeletionQueue( sceneNode );
				sceneNode = NULL;
			}
			
			this->model = NULL;
			
			if( model )
			{
				if( model->GetMesh() )
				{
					this->model = model;
					sceneNode = engine->GetWindow()->sceneManager->addAnimatedMeshSceneNode( model->GetMesh() );
					model->SetMaterialsToNode( sceneNode );
					
					sceneNode->setMaterialFlag( irr::video::EMF_NORMALIZE_NORMALS, true );
					sceneNode->setScale( Math::GetIrrVec( scale ) );
					
					if( light )
						sceneNode->addShadowVolumeSceneNode();
				}
			}
		}
	}
}

Object::Object( Engine * engine, std::string name, std::shared_ptr<btRigidBody> body, std::shared_ptr<btCollisionShape> collisionShape, float mass_ ) :
	mass(mass_)
{
	this->collisionShape = collisionShape;
	this->engine = engine;
	this->name = name;
	this->body = body;
	scale = btVector3(1,1,1);
	rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	sceneNode = NULL;
}

Object::Object() :
	mass(1.0)
{
	engine = NULL;
	name = "";
	scale = btVector3(1,1,1);
	rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	sceneNode = NULL;
}

Object::~Object()
{
	DEBUG(0)
	if( sceneNode )
	{
		if( engine )
		{
			if( engine->GetWindow() )
			{
				if( engine->GetWindow()->sceneManager )
				{
					engine->GetWindow()->sceneManager->addToDeletionQueue( sceneNode );
				}
			}
		}
		sceneNode = NULL;
	}
	
	if( collisionShape )
	{
		engine->GetCollisionShapeManager()->RemoveShape( collisionShape );
	}
	
	if( body )
	{
		if( body->getMotionState() )
		{
			assert( body->getMotionState() != NULL );
			delete body->getMotionState();
			body->setMotionState( NULL );
		}
		
		body->setCollisionShape( NULL );
		
		assert( body );
		body.reset();
	}
	
	name = "";
	scale = btVector3(0,0,0);
	mass = 0.0f;
}

#endif

