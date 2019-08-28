
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CUSTOM_COLLISION_SHAPE_DATA_H
#define CUSTOM_COLLISION_SHAPE_DATA_H

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vector>

#include <memory>

class CustomCollisionShapeData
{
private:
	
	std::vector < btVector3 > vertices;
	std::vector < int > indices;
	
	std::shared_ptr<btCollisionShape> convexShape;
	
	std::shared_ptr<btTriangleIndexVertexArray> triangleData;
	std::shared_ptr<btCollisionShape> triangleShape;
	
	float friction;
	float restitution;
	
	friend class Model;
	
public:
	
	std::shared_ptr<btCollisionShape> GetTriangleShape();
	std::shared_ptr<btCollisionShape> GetConvexShape();
	
	void DestroyTriangleShape();
	void DestroyConvexShape();
	
	CustomCollisionShapeData();
	~CustomCollisionShapeData();
};

#endif

