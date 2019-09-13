
#ifndef COLLISION_OBJECT_MANAGER_CPP
#define COLLISION_OBJECT_MANAGER_CPP

#include "..\css\CollisionObjectManager.h"

#include <BulletCollision\CollisionDispatch\btGhostObject.h>

std::shared_ptr<btCollisionObject> CollisionObjectManager::CreateRigidBody( std::shared_ptr<btCollisionShape> shape, btTransform transform, float mass, btVector3 inertia )
{
	if( shape == NULL )
		return NULL;
	
	if( mass > 0.0f )
		shape->calculateLocalInertia( mass, inertia );
	
	btDefaultMotionState * motionState = new btDefaultMotionState( transform );
	return std::dynamic_pointer_cast<btCollisionObject>( std::shared_ptr<btRigidBody>( new btRigidBody( mass <= 0.0f ? 0.0f : mass, motionState, (btCollisionShape*)shape.get(), inertia ) ) );
}

std::shared_ptr<btCollisionObject> CollisionObjectManager::CreatePairCachingGhostObject()
{
	return std::dynamic_pointer_cast<btCollisionObject>( std::shared_ptr<btPairCachingGhostObject>( new btPairCachingGhostObject() ) );
}

#endif

