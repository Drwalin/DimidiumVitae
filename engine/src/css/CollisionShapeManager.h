
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_MANAGER_H
#define COLLISION_SHAPE_MANAGER_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <vector>
#include <string>
#include <map>

#include <memory>

#include "CollisionShapeConstructor.h"

class CollisionShapeManager {
public:
	
	CollisionShapeManager(class Engine *engine);
	~CollisionShapeManager();
	
	std::shared_ptr<btCollisionShape> Clone(const std::shared_ptr<btCollisionShape> &shape);
	void SerializeCollisionShape(std::ostream &stream, std::shared_ptr<btCollisionShape> &shape);
	std::shared_ptr<btCollisionShape> DeserializeCollisionShape(std::istream &stream);
	
	bool IsNameAvailable(const std::string &name);
	std::string GetFirstAvailableName(const std::string &name);
	
	void DestroyShapeConstructionInfo(const std::string &name);
	void DestroyShape(std::shared_ptr<btCollisionShape> &shape);
	
	std::shared_ptr<CollisionShapeConstructor> GetBoxConstructor(btVector3 size);
	std::shared_ptr<CollisionShapeConstructor> GetSphereConstructor(btScalar radius);
	std::shared_ptr<CollisionShapeConstructor> GetCapsuleConstructor(btScalar radius, btScalar height);
	std::shared_ptr<CollisionShapeConstructor> GetCylinderConstructor(btScalar radius, btScalar height);
	std::shared_ptr<CollisionShapeConstructor> GetCollisionShapeConstructor(const std::string &name);
	
	std::shared_ptr<btCollisionShape> GetBox(btVector3 size);
	std::shared_ptr<btCollisionShape> GetSphere(btScalar radius);
	std::shared_ptr<btCollisionShape> GetCapsule(btScalar radius, btScalar height);
	std::shared_ptr<btCollisionShape> GetCylinder(btScalar radius, btScalar height);
	std::shared_ptr<btCollisionShape> GetCustomShape(const std::string &name);
	
	bool RegisterCustomShape(const std::string &name, const std::string &collisionShapeFileName);
	bool ConvertObjToCustomShape(const std::string &objFileName, const std::string &collisionShapeFileName);
	
	void Destroy();
	
private:
	
	class Engine *engine;
	std::map < std::string, std::shared_ptr<CollisionShapeConstructor> > shapeConstructors;
};

#endif
