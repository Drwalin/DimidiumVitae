
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

class Model;
class Engine;

class CollisionShapeManager
{
private:
	
	Engine * engine;
	
	std::map < std::vector<btScalar>, std::shared_ptr<btCollisionShape> > shapesVecPtr;
	std::map < std::shared_ptr<btCollisionShape>, std::vector<btScalar> > shapesPtrVec;
	
	std::map < std::string, std::shared_ptr<btCollisionShape> > shapesStrPtr;
	std::map < std::shared_ptr<btCollisionShape>, std::string > shapesPtrStr;
	
	
	std::shared_ptr<btCollisionShape> GetShape( std::vector < btScalar > constructionData, bool independent );
	std::shared_ptr<btCollisionShape> AddShape( std::vector < btScalar > constructionData, std::string name );
	
	friend class Engine;
	
public:
	
	std::string GetFirstAvailableName( std::string name );
	bool IsNameAvailable( std::string name );
	
	// if name == "" ->collisionShape else ->customCollisionShape
	std::shared_ptr<btCollisionShape> GetBox( btVector3 size, std::string name = "" );
	std::shared_ptr<btCollisionShape> GetBall( btScalar radius, std::string name = "" );
	std::shared_ptr<btCollisionShape> GetCapsule( btScalar radius, btScalar height, std::string name = "" );
	std::shared_ptr<btCollisionShape> GetCylinder( btScalar radius, btScalar height, std::string name = "" );
	
	std::shared_ptr<btCollisionShape> GetShape( std::string name );
	std::shared_ptr<btCollisionShape> AddCustomShape( std::string name, std::shared_ptr<btCollisionShape> shape );
	
	void RemoveShape( std::shared_ptr<btCollisionShape> shape );
	
	void Destroy();
	
	CollisionShapeManager( Engine * engine );
	~CollisionShapeManager();
};


#endif

