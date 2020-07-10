
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MOTION_CONTROLLER_TRIGGER_CPP
#define MOTION_CONTROLLER_TRIGGER_CPP

#include <BulletCollision\CollisionDispatch\btCollisionWorld.h>

#include "..\css\MotionControllerTrigger.h"
#include <Engine.h>

bool MotionControllerTrigger::IsTopCollision() const
{
	return this->topCollision;
}

bool MotionControllerTrigger::IsSideCollision() const
{
	return this->sideCollision;
}

bool MotionControllerTrigger::IsBottomCollision() const
{
	return this->bottomCollision;
}

void MotionControllerTrigger::Init( std::shared_ptr<Entity> character, std::shared_ptr<Entity> otherTrigger, float stepHeight )
{
	this->character = character;
	this->otherTrigger = otherTrigger;
	this->stepHeight = stepHeight;
	this->bottom = 0.0f;
	this->top = 1.75f;
}

void MotionControllerTrigger::NextOverlappingFrame()
{
	Trigger::NextOverlappingFrame();
	this->topCollision = false;
	this->sideCollision = false;
	this->bottomCollision = false;
	
	btVector3 min, max;
	collisionShape->getAabb( body->getWorldTransform(), min, max );
	this->bottom = min.y();
	this->top = max.y();
	float mid = (this->bottom + this->top)*0.5f;
	
	for(Entity* ent : overlappingInCurrentFrame)
	{
		if( ent != this->character.get() && ent != this && ent != this->otherTrigger.get() )
		{
			struct SimulationContactResultCallback : public btCollisionWorld::ContactResultCallback
			{
				MotionControllerTrigger *trigger;
				float mid;
				SimulationContactResultCallback(MotionControllerTrigger *trigger, float mid) : trigger(trigger), mid(mid)
				{}

				btScalar addSingleResult(btManifoldPoint& manifoldPoint,
					const btCollisionObjectWrapper* colObj0Wrap,
					int partId0,
					int index0,
					const btCollisionObjectWrapper* colObj1Wrap,
					int partId1,
					int index1)
				{
					btVector3 contactPoint = (manifoldPoint.m_positionWorldOnB + manifoldPoint.m_positionWorldOnA) * 0.5f;
					btVector3 normal = manifoldPoint.m_normalWorldOnB.normalized();
					if( normal.y() < 0.0f )
						normal.setY( -normal.y() );
					bool isNormalVertical = normal.dot( btVector3(0,1,0) ) > 0.8f;
					
					if( isNormalVertical )
					{
						if( trigger->bottom + trigger->stepHeight >= contactPoint.y() )
						{
							trigger->bottomCollision = true;
							trigger->engine->GetWindow()->GetGUI() << "\n bottomCollision";
						}
						if( mid <= contactPoint.y() )
						{
							trigger->topCollision = true;
							trigger->engine->GetWindow()->GetGUI() << "\n topCollision";
						}
					}
					else
					{
						trigger->sideCollision = true;
							trigger->engine->GetWindow()->GetGUI() << "\n sideCollision";
					}
					
					return 0.0f;
				}
			};
			SimulationContactResultCallback resultCallback(this, mid);
			
			engine->GetWorld()->GetDynamicsWorld()->contactPairTest( body.get(), ent->GetBody().get(), resultCallback );
		}
	}
}

void MotionControllerTrigger::EventOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	float mid = (this->bottom + this->top)*0.5f;
	if( other != this->character.get() && other != this && other != this->otherTrigger.get() )
	{
		for( int i=0; i<persisstentManifold->getNumContacts(); ++i )
		{
			btManifoldPoint manifoldPoint = persisstentManifold->getContactPoint( i );
			btVector3 contactPoint = (manifoldPoint.m_positionWorldOnB + manifoldPoint.m_positionWorldOnA) * 0.5f;
			btVector3 normal = manifoldPoint.m_normalWorldOnB.normalized();
			if( normal.y() < 0.0f )
				normal.setY( -normal.y() );
			bool isNormalVertical = normal.dot( btVector3(0,1,0) ) > 0.8f;
			
			if( isNormalVertical )
			{
				if( this->bottom + this->stepHeight >= contactPoint.y() )
					this->bottomCollision = true;
				if( mid <= contactPoint.y() )
				{
					this->topCollision = true;
				}
			}
			else
				this->sideCollision = true;
		}
	}
}

void MotionControllerTrigger::EventOnEntityBeginOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	Trigger::EventOnEntityBeginOverlapp( other, persisstentManifold );
	this->EventOverlapp( other, persisstentManifold );
}

void MotionControllerTrigger::EventOnEntityTickOverlapp( Entity * other, btPersistentManifold * persisstentManifold )
{
	Trigger::EventOnEntityTickOverlapp( other, persisstentManifold );
	this->EventOverlapp( other, persisstentManifold );
}

void MotionControllerTrigger::EventOnEntityEndOverlapp( Entity * other )
{
	Trigger::EventOnEntityEndOverlapp( other );
}

void MotionControllerTrigger::Tick( const float deltaTime )
{
	Trigger::Tick( deltaTime );
}

void MotionControllerTrigger::Load( std::istream & stream )
{
	Trigger::Load( stream );
}

void MotionControllerTrigger::Save( std::ostream & stream ) const
{
	Trigger::Save( stream );
}

void MotionControllerTrigger::Spawn( std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform )
{
	Trigger::Spawn( self, name, shape, transform );
}

void MotionControllerTrigger::Despawn()
{
	Trigger::Despawn();
}

void MotionControllerTrigger::Destroy()
{
	this->character = NULL;
	this->otherTrigger = NULL;
	Trigger::Destroy();
}

extern "C" std::shared_ptr<Entity> GetMotionControllerTriggerInstantiator(){ static std::shared_ptr<Entity> instantiator( new MotionControllerTrigger(), [](Entity * ptr){delete ptr;} ); return instantiator; }
int MotionControllerTrigger::GetTypeSize() const{ return sizeof(MotionControllerTrigger); }
void MotionControllerTrigger::Free(){ delete this; }
std::shared_ptr<Entity> MotionControllerTrigger::New() const{ return std::dynamic_pointer_cast<Entity>( std::shared_ptr<MotionControllerTrigger>( new MotionControllerTrigger(), [](Entity * ptr){delete ptr;} ) ); }
std::string MotionControllerTrigger::GetClassName() const{ return "MotionControllerTrigger"; }

MotionControllerTrigger::MotionControllerTrigger()
{
}

MotionControllerTrigger::~MotionControllerTrigger()
{
}

#endif

