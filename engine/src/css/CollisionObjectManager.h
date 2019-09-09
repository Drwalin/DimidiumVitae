
#ifndef COLLISION_OBJECT_MANAGER_H
#define COLLISION_OBJECT_MANAGER_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <memory>
#include <string>

class CollisionObjectManager
{
public:
	
	static std::shared_ptr<btCollisionObject> CreateRigidBody( std::shared_ptr<btCollisionShape> shape, btTransform transform, float mass, btVector3 inertia = btVector3(0.0f,0.0f,0.0f) );
};

#endif

