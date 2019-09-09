
#ifndef COLLISION_OBJECT_MANAGER_CPP
#define COLLISION_OBJECT_MANAGER_CPP

#include "..\css\CollisionObjectManager.h"

std::shared_ptr<btCollisionObject> CollisionObjectManager::CreateRigidBody( std::shared_ptr<btCollisionShape> shape, btTransform transform, float mass, btVector3 inertia )
{
	if( shape == NULL )
		return NULL;
	
	if( mass > 0.0f )
		shape->calculateLocalInertia( mass, inertia );
	
	btDefaultMotionState * motionState = new btDefaultMotionState( transform );
	std::shared_ptr<btCollisionObject> body = std::dynamic_pointer_cast<btCollisionObject>( std::shared_ptr<btRigidBody>( new btRigidBody( mass <= 0.0f ? 0.0f : mass, motionState, (btCollisionShape*)shape.get(), inertia ) ) );
	
	return body;
}

#endif

