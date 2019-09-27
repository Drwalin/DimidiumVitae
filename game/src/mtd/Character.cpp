
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CHARACTER_CPP
#define CHARACTER_CPP

#include "..\css\Character.h"

#include <Engine.h>
#include <CollisionObjectManager.h>

#include <Debug.h>
#include <Math.hpp>

#include <ctime>

std::shared_ptr<MotionController> Character::GetMotionController()
{
	return this->motionController;
}

void Character::NextOverlappingFrame()
{
	Entity::NextOverlappingFrame();
}

void Character::Tick( const float deltaTime )
{
	static TimeCounter fpsCounter;
	fpsCounter.SetTimeSpan( 2.0f );
	Entity::Tick( deltaTime );
	this->motionController->Tick( deltaTime );
	this->engine->GetWindow()->GetGUI() << Rectanglef(0.05,0.02,0.6,0.6) << "Character position: " << this->GetTransform().getOrigin();
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
	this->engine->GetWindow()->GetGUI() << "\nCharacter velocity: " << (btVector3(rigidBody->getLinearVelocity().x(),0,rigidBody->getLinearVelocity().z()).length());
	this->engine->GetWindow()->GetGUI() << "\nFPS: " << (int)(1.0f/fpsCounter.GetSmoothTime());
	this->engine->GetWindow()->GetGUI() << "\nFrame seconds peak: " << fpsCounter.GetPeakTime();
	this->engine->GetWindow()->GetGUI() << "\nFrame seconds pit: " << fpsCounter.GetPitTime();
	fpsCounter.SubscribeEnd();
	fpsCounter.SubscribeStart();
}

void Character::ApplyDamage( const float damage, btVector3 point, btVector3 normal )
{
	Entity::ApplyDamage( damage, point, normal );
}

void Character::ApplyImpactDamage( const float damage, const float impetus, btVector3 direction, btVector3 point, btVector3 normal )
{
	Entity::ApplyImpactDamage( damage, impetus, direction, point, normal );
	this->ApplyDamage( damage, point, normal );
}

void Character::Load( std::istream & stream )
{
	Entity::Load( stream );
}

void Character::Save( std::ostream & stream ) const
{
	Entity::Save( stream );
}

void Character::Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Entity::Spawn( self, name, shape, transform );
	
	std::shared_ptr<btCollisionObject> collisionObject = CollisionObjectManager::CreateRigidBody( shape, transform, 15.0f, btVector3(0,0,0) );
	std::shared_ptr<btRigidBody> rigidBody = std::dynamic_pointer_cast<btRigidBody>( collisionObject );
	
	rigidBody->setFriction( 0.2 );
	rigidBody->setAngularFactor( btVector3( 0, 0, 0 ) );
	rigidBody->setActivationState( DISABLE_DEACTIVATION );
	rigidBody->setDamping( 0.1, 0.1 );
	
	this->rayTraceChannel = Engine::RayTraceChannel::COLLIDING | Engine::RayTraceChannel::NOT_TRANSPARENT;
	this->SetBody( collisionObject, shape );
	
	this->motionController = std::shared_ptr<MotionController>( new MotionController() );
	this->motionController->Init( this->engine, self, 0.3f );
}

void Character::Despawn()
{
	Entity::Despawn();
	if( this->motionController )
		this->motionController->Destroy();
	this->motionController = NULL;
}

void Character::Destroy()
{
	Entity::Destroy();
	if( this->motionController )
		this->motionController->Destroy();
	this->motionController = NULL;
}

extern "C" std::shared_ptr<Entity> GetCharacterInstantiator(){ static std::shared_ptr<Entity> instantiator( new Character(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int Character::GetTypeSize() const{ return sizeof(Character); }
void Character::Free(){ delete this; }
std::shared_ptr<Entity> Character::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<Character>( new Character(), [](Entity * ptr){delete ptr;} ) ); }
std::string Character::GetClassName() const{ return "Character"; }

Character::Character() :
	Entity(), height(1.75)
{
}

Character::~Character()
{
	this->Destroy();
}

#endif

