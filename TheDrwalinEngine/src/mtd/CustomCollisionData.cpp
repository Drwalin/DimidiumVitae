
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CUSTOM_COLLISION_SHAPE_DATA_CPP
#define CUSTOM_COLLISION_SHAPE_DATA_CPP

#include <CustomCollisionData.h>

#include <Debug.h>

std::shared_ptr<btCollisionShape> CustomCollisionShapeData::GetTriangleShape()
{
	if( !triangleShape )
	{
		if( !triangleData )
		{
			triangleData = std::shared_ptr<btTriangleIndexVertexArray>( new btTriangleIndexVertexArray( indices.size() / 3, &indices.front(), sizeof(int) * 3, vertices.size(), vertices.front().m_floats, sizeof(btVector3) ) );
			if( !triangleData )
			{
				return triangleShape;
			}
		}
		triangleShape = std::shared_ptr<btBvhTriangleMeshShape>( new btBvhTriangleMeshShape( (btStridingMeshInterface*)(triangleData.get()), true, true ) );
	}
	return triangleShape;
}

std::shared_ptr<btCollisionShape> CustomCollisionShapeData::GetConvexShape()
{
	DEBUG(0)
	if( !convexShape )
	{
	DEBUG(1)
		convexShape = std::shared_ptr<btConvexHullShape>( new btConvexHullShape( vertices.front().m_floats, vertices.size(), sizeof(btVector3) ) );
		if( !convexShape )
		{
			DEBUG(311)
		}
	DEBUG(2)
	}
	DEBUG(3)
	return convexShape;
}

/*
void CustomCollisionShapeData::DestroyTriangleShape()
{
	triangleShape.reset();
	triangleData.reset();
}

void CustomCollisionShapeData::DestroyConvexShape()
{
	convexShape.reset();
}
*/

CustomCollisionShapeData::CustomCollisionShapeData()
{
	friction = 0.5f;
	restitution = 0.5f;
}

CustomCollisionShapeData::~CustomCollisionShapeData()
{
	friction = 0.0f;
	restitution = 0.0f;
}

#endif

