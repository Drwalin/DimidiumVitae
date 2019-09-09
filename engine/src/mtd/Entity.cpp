
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "..\css\Entity.h"
#include "..\css\Engine.h"

#include "CollisionObjectManager.h"

#include "..\lib\Math.hpp"

#include <cassert>

void Entity::UpdateTransformSceneNode()
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

void Entity::SetTransform( const btTransform & transform )
{
	this->currentTransform = transform;
	if( this->body )
	{
		this->body->activate( true );
		
		std::shared_ptr<btRigidBody> rd = this->GetBody<btRigidBody>();
		if( rd )
			rd->getMotionState()->setWorldTransform( this->currentTransform );
		else
			this->body->setWorldTransform( this->currentTransform );
		
		this->engine->GetWorld()->UpdateColliderForObject( this->body );
		this->body->activate( true );
	}
}

void Entity::SetPosition( const btVector3 & loc )
{
	this->SetTransform( btTransform( this->currentTransform.getRotation(), loc ) );
}

void Entity::SetRotation( const btQuaternion & quat )
{
	this->SetTransform( btTransform( quat, this->currentTransform.getOrigin() ) );
}

void Entity::Move( const btVector3 & move )
{
	this->SetPosition( this->currentTransform.getOrigin() + move );
}

void Entity::Rotate( const btQuaternion & quat )
{
	this->SetRotation( this->currentTransform.getRotation() * quat );
}

void Entity::NextOverlappingFrame()
{
	for( auto it = this->overlappingInPreviousFrame.begin(); it != this->overlappingInPreviousFrame.end(); ++it )
	{
		if( this->overlappingInCurrentFrame.find( *it ) == this->overlappingInCurrentFrame.end() )
		{
			this->EventOnEntityEndOverlapp( *it );
		}
	}
	
	this->overlappingInPreviousFrame = this->overlappingInCurrentFrame;
	this->overlappingInCurrentFrame.clear();
}

void Entity::OverlapWithEntity( Entity * other, btPersistentManifold * persisstentManifold )
{
	if( other != this )
	{
		if( persisstentManifold )
		{
			if( other )
			{
				if( this->overlappingInPreviousFrame.find( other ) != this->overlappingInPreviousFrame.end() )
				{
					this->EventOnEntityTickOverlapp( other, persisstentManifold );
				}
				else
				{
					this->EventOnEntityBeginOverlapp( other, persisstentManifold );
				}
				this->overlappingInCurrentFrame.insert( other );
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
		MESSAGE( std::string("Trying to collide with my self: ") + this->GetName() );
	}
}

void Entity::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * persisstentManifold ){}
void Entity::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * persisstentManifold ){}
void Entity::EventOnEntityEndOverlapp( Entity * other ){}




void Entity::Tick( const float deltaTime )
{
	if( this->body )
	{
		std::shared_ptr<btRigidBody> rd = this->GetBody<btRigidBody>();
		if( rd )
			rd->getMotionState()->getWorldTransform( this->currentTransform );
		else
			this->currentTransform = this->body->getWorldTransform();
	}
	
	this->UpdateTransformSceneNode();
}

void Entity::ApplyDamage( const float damage, btVector3 point, btVector3 normal ){}

void Entity::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	if( this->body )
	{
		if( normal.dot( direction ) > 0 )
			normal *= -1;
		
		std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
		
		if( rigidBody )
		{
			rigidBody->activate( true );
			rigidBody->applyImpulse( direction.normalized() * impetus,  point - this->currentTransform.getOrigin() );
			rigidBody->activate( true );
		}
	}
}

void Entity::SetMass( float mass )
{
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
	if( rigidBody )
	{
		this->mass = mass;
		btCollisionShape * shape = rigidBody->getCollisionShape();
		if( shape )
		{
			btVector3 inertia = rigidBody->getInvInertiaDiagLocal();
			shape->calculateLocalInertia( mass, inertia );
			rigidBody->setMassProps( mass, inertia );
			if( mass > 0.0f )
			{
				rigidBody->setGravity( btVector3(0.0f,-9.81f,0.0f) );
			}
			else
			{
				
			}
		}
	}
}

Engine * Entity::GetEngine()
{
	return engine;
}

void Entity::SetScale( btVector3 scale )
{
	this->scale = scale;
	if( this->body )
	{
		this->body->activate( true );
		this->body->getCollisionShape()->setLocalScaling( this->scale );
		this->engine->GetWorld()->UpdateColliderForObject( this->body );
		this->body->activate( true );
	}
	if( this->sceneNode )
	{
		this->sceneNode->setScale( Math::GetIrrVec( this->scale ) );
	}
}

btVector3 Entity::GetScale()
{
	return scale;
}

btTransform Entity::GetTransform()
{
	return this->currentTransform;
}

btVector3 Entity::GetLocation() const
{
	return this->currentTransform.getOrigin();
}

void Entity::SetRayTraceChannel( int src )
{
	rayTraceChannel = src;
}

int Entity::GetRayTraceChannel()
{
	return rayTraceChannel;
}

const std::string & Entity::GetName() const
{
	return name;
}

void Entity::SetModel( std::shared_ptr<Model> model, bool light )
{
	if( this->engine )
	{
		if( this->engine->GetWindow() )
		{
			if( this->sceneNode )
			{
				this->engine->GetWindow()->sceneManager->addToDeletionQueue( this->sceneNode );
				this->sceneNode = NULL;
			}
			
			this->model = NULL;
			
			if( model )
			{
				if( model->GetMesh() )
				{
					this->model = model;
					this->sceneNode = this->engine->GetWindow()->sceneManager->addAnimatedMeshSceneNode( model->GetMesh() );
					this->model->SetMaterialsToNode( this->sceneNode );
					
					this->sceneNode->setMaterialFlag( irr::video::EMF_NORMALIZE_NORMALS, true );
					this->sceneNode->setScale( Math::GetIrrVec( scale ) );
					
					if( light )
						this->sceneNode->addShadowVolumeSceneNode();
				}
			}
		}
	}
}

void Entity::SetBody( std::shared_ptr<btCollisionObject> body, std::shared_ptr<btCollisionShape> shape )
{
	this->DestroyBody();
	this->body = body;
	this->collisionShape = shape;
	this->engine->GetWorld()->AddBody( this->GetName(), this->body );
	this->body->setUserPointer( this );
}

void Entity::DestroyBody()
{
	if( this->body )
	{
		std::shared_ptr<btRigidBody> rigid = this->GetBody<btRigidBody>();
		if( rigid )
		{
			auto motionState = rigid->getMotionState();
			if( motionState )
			{
				rigid->setMotionState( NULL );
				delete motionState;
			}
		}
		
		this->body->setCollisionShape( NULL );
		
		assert( this->body );
		this->body.reset();
		
		this->body = NULL;
	}
	
	if( this->collisionShape )
	{
		this->engine->GetCollisionShapeManager()->RemoveShape( this->collisionShape );
	}
}

void Entity::DestroySceneNode()
{
	if( this->sceneNode )
	{
		if( this->engine )
		{
			if( this->engine->GetWindow() )
			{
				if( this->engine->GetWindow()->sceneManager )
				{
					this->engine->GetWindow()->sceneManager->addToDeletionQueue( this->sceneNode );
				}
			}
		}
		this->sceneNode = NULL;
	}
}

void Entity::Destroy()
{
	this->DestroySceneNode();
	
	this->DestroyBody();
	
	this->name = "";
	this->scale = btVector3(0,0,0);
	this->mass = 0.0f;
}

void Entity::Despawn()
{
	this->Destroy();
}

void Entity::Load( std::istream & stream )
{
}

void Entity::Save( std::ostream & stream ) const
{
}

void Entity::Init( Engine * engine )
{
	this->Destroy();
	this->engine = engine;
}

void Entity::Spawn( std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	this->Destroy();
	this->mass = -1.0f;
	this->collisionShape = collisionShape;
	this->name = name;
	this->scale = btVector3(1,1,1);
	this->rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	this->currentTransform = transform;
	this->SetBody( CollisionObjectManager::CreateRigidBody( shape, transform, 1.0f ), shape );
}

Entity::Entity()
{
	this->mass = 1.0f;
	this->engine = NULL;
	this->name = "";
	this->scale = btVector3(1,1,1);
	this->rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	this->sceneNode = NULL;
}

Entity::~Entity()
{
	this->Destroy();
}

#endif

