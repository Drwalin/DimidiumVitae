
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_MANAGER_CPP
#define COLLISION_SHAPE_MANAGER_CPP

#include "../css/CollisionShapeManager.h"

#include "../lib/Debug.h"

std::shared_ptr<btCollisionShape> CollisionShapeManager::Clone(const std::shared_ptr<btCollisionShape> &shape) {
	btCapsuleShape *capsule = dynamic_cast<btCapsuleShape*>(shape.get());
	if(capsule) {
		float radius = capsule->getRadius();
		float height = capsule->getHalfHeight()*2.0f;
		return std::shared_ptr<btCollisionShape>(new btCapsuleShape(radius, height));
	}
	
	btConeShape *cone = dynamic_cast<btConeShape*>(shape.get());
	if(cone) {
		float radius = cone->getRadius();
		float height = cone->getHeight()*2.0f;
		return std::shared_ptr<btCollisionShape>(new btConeShape(radius, height));
	}
	
	btCylinderShape *cylinder = dynamic_cast<btCylinderShape*>(shape.get());
	if(cylinder) {
		btVector3 halfExtents = cylinder->getHalfExtentsWithoutMargin();
		halfExtents.setY(halfExtents.y() *2.0f);
		return std::shared_ptr<btCollisionShape>(new btCylinderShape(halfExtents));
	}
	
	btSphereShape *sphere = dynamic_cast<btSphereShape*>(shape.get());
	if(sphere) {
		float radius = sphere->getRadius();
		return std::shared_ptr<btCollisionShape>(new btSphereShape(radius));
	}
	
	btBoxShape *box = dynamic_cast<btBoxShape*>(shape.get());
	if(box) {
		btVector3 halfExtents = box->getHalfExtentsWithoutMargin();
		return std::shared_ptr<btCollisionShape>(new btBoxShape(halfExtents));
	}
	
	btConvexHullShape *convexHull = dynamic_cast<btConvexHullShape*>(shape.get());
	if(convexHull) {
		btVector3 *vertices = convexHull->getUnscaledPoints();
		int numberOfVertices = convexHull->getNumPoints();
		return std::shared_ptr<btCollisionShape>(new btConvexHullShape(vertices->m_floats, numberOfVertices, sizeof(btVector3)));
	}
	
	btBvhTriangleMeshShape *triangleMesh = dynamic_cast<btBvhTriangleMeshShape*>(shape.get());
	if(triangleMesh) {
		btStridingMeshInterface *trianglesData = triangleMesh->getMeshInterface();
		return std::shared_ptr<btCollisionShape>(new btBvhTriangleMeshShape(trianglesData, true, true));
	}
	
	MESSAGE(std::string("Not supported collision shape type to clone:") + std::to_string((unsigned long long)shape.get()) + " shape type id: " + std::to_string(shape->getShapeType()));
	return NULL;
}

bool CollisionShapeManager::IsNameAvailable(const std::string &name) {
	if(name == "")
		return false;
	return bool(this->shapeConstructors.find(name) == this->shapeConstructors.end());
}

std::string CollisionShapeManager::GetFirstAvailableName(const std::string &name) {
	if(this->IsNameAvailable(name))
		return name;
	for(int i = 0;; ++i) {
		if(this->IsNameAvailable(name + std::to_string(i)))
			return name + std::to_string(i);
	}
	return name;
}

void CollisionShapeManager::DestroyShapeConstructionInfo(const std::string &name) {
	auto it = this->shapeConstructors.find(name);
	if(it != this->shapeConstructors.end()) {
		it->second = NULL;
		this->shapeConstructors.erase(it);
	}
}

void CollisionShapeManager::DestroyShape(std::shared_ptr<btCollisionShape> &shape) {
	btCompoundShape *compoundShape = dynamic_cast<btCompoundShape*>(shape.get());
	if(compoundShape) {
		int shapes = compoundShape->getNumChildShapes();
		if(shapes > 0) {
			for(int i=shapes-1; i>=0; --i) {
				auto shape = compoundShape->getChildShape(i);
				compoundShape->removeChildShapeByIndex(i);
				delete shape;
			}
		}
	}
}

std::shared_ptr<CollisionShapeConstructor> CollisionShapeManager::GetBoxConstructor(btVector3 size) {
	std::shared_ptr<CollisionShapeConstructor> shapeConstructor(new CollisionShapeConstructor);
	shapeConstructor->AddBox(btTransform(btQuaternion(btVector3(0,1,0),0)), size);
	return shapeConstructor;
}

std::shared_ptr<CollisionShapeConstructor> CollisionShapeManager::GetSphereConstructor(btScalar radius) {
	std::shared_ptr<CollisionShapeConstructor> shapeConstructor(new CollisionShapeConstructor);
	shapeConstructor->AddSphere(btTransform(btQuaternion(btVector3(0,1,0),0)), radius);
	return shapeConstructor;
}

std::shared_ptr<CollisionShapeConstructor> CollisionShapeManager::GetCapsuleConstructor(btScalar radius, btScalar height) {
	std::shared_ptr<CollisionShapeConstructor> shapeConstructor(new CollisionShapeConstructor);
	shapeConstructor->AddCapsule(btTransform(btQuaternion(btVector3(0,1,0),0)), radius, height);
	return shapeConstructor;
}

std::shared_ptr<CollisionShapeConstructor> CollisionShapeManager::GetCylinderConstructor(btScalar radius, btScalar height) {
	std::shared_ptr<CollisionShapeConstructor> shapeConstructor(new CollisionShapeConstructor);
	shapeConstructor->AddCylinder(btTransform(btQuaternion(btVector3(0,1,0),0)), radius, height);
	return shapeConstructor;
}

std::shared_ptr<CollisionShapeConstructor> CollisionShapeManager::GetCollisionShapeConstructor(const std::string &name) {
	auto it = this->shapeConstructors.find(name);
	if(it != this->shapeConstructors.end()) {
		return it->second;
	}
	return NULL;
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetBox(btVector3 size) {
	return std::shared_ptr<btCollisionShape>(new btBoxShape(size));
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetSphere(btScalar radius) {
	return std::shared_ptr<btCollisionShape>(new btSphereShape(radius));
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCapsule(btScalar radius, btScalar height) {
	return std::shared_ptr<btCollisionShape>(new btCapsuleShape(radius, height - 2.0f*radius));
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCylinder(btScalar radius, btScalar height) {
	return std::shared_ptr<btCollisionShape>(new btCylinderShape(btVector3(radius, height, radius)));
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCustomShape(const std::string &name) {
	auto it = this->shapeConstructors.find(name);
	if(it != this->shapeConstructors.end()) {
		return it->second->Get();
	}
	return NULL;
}

bool CollisionShapeManager::RegisterCustomShape(const std::string &name, const std::string &collisionShapeFileName) {
	auto it = this->shapeConstructors.find(name);
	if(it == this->shapeConstructors.end()) {
		std::shared_ptr<CollisionShapeConstructor> shape(new CollisionShapeConstructor());
		if(shape) {
			if(shape->Load(name, collisionShapeFileName)) {
				this->shapeConstructors[name] = shape;
				return true;
			}
			shape = NULL;
		}
	} else if(it->second->GetCollisionShapeFileName() == collisionShapeFileName)
		return true;
	return false;
}

bool CollisionShapeManager::ConvertObjToCustomShape(const std::string &objFileName, const std::string &collisionShapeFileName) {
	return CollisionShapeConstructor::Convert(this->engine, objFileName, collisionShapeFileName);
}

void CollisionShapeManager::Destroy() {
	for(auto it=this->shapeConstructors.begin(); it!=this->shapeConstructors.end(); ++it)
		it->second = NULL;
	this->shapeConstructors.clear();
	this->engine = NULL;
}

CollisionShapeManager::CollisionShapeManager(class Engine *engine) {
	this->engine = engine;
}

CollisionShapeManager::~CollisionShapeManager() {
	this->Destroy();
}

#endif
