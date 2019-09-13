
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_MANAGER_H
#define COLLISION_SHAPE_MANAGER_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>
#include <string>
#include <map>

#include "CustomCollisionData.h"

#include <memory>

class CollisionShapeManager
{
private:
	
	std::map < std::string, class CollisionShapeConstructor* > shapeConstructors;
	
public:
	
	bool IsNameAvailable( const std::string & name );
	std::string GetFirstAvailableName( const std::string & name );
	
	void DestroyShapeConstructionInfo( const std::string & name );
	void DestroyShape( std::shared_ptr<btCollisionShape> & shape );
	
	std::shared_ptr<btCollisionShape> GetBox( btVector3 size );
	std::shared_ptr<btCollisionShape> GetSphere( btScalar radius );
	std::shared_ptr<btCollisionShape> GetCapsule( btScalar radius, btScalar height );
	std::shared_ptr<btCollisionShape> GetCylinder( btScalar radius, btScalar height );
	
	std::shared_ptr<btCollisionShape> GetCustomShape( const std::string & name );
	bool RegisterCustomShape( const std::string & name, const std::string & collisionShapeFileName );
	bool ConvertObjToCustomShape( const std::string & objFileName, const std::string & collisionShapeFileName );
	
	void Destroy();
	
	CollisionShapeManager();
	~CollisionShapeManager();
};


#endif

