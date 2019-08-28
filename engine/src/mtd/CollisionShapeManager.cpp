
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_MANAGER_CPP
#define COLLISION_SHAPE_MANAGER_CPP

#include <CollisionShapeManager.h>
#include <Model.h>
#include <Object.h>
#include <Engine.h>

bool CollisionShapeManager::IsNameAvailable( std::string name )
{
	if( name == "" )
		return false;
	return bool( shapesStrPtr.find( name ) == shapesStrPtr.end() );
}

std::string CollisionShapeManager::GetFirstAvailableName( std::string name )
{
	if( IsNameAvailable( name ) )
		return name;
	for( int i = 0;; ++i )
	{
		if( IsNameAvailable( name + std::to_string(i) ) )
			return name + std::to_string(i);
	}
	return name;
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetShape( std::vector < btScalar > constructionData, bool independent )
{
	auto it = shapesVecPtr.find( constructionData );
	if( !independent && it != shapesVecPtr.end() )
	{
		return it->second;
	}
	else
	{
		std::shared_ptr<btCollisionShape> shape;
		switch( constructionData.size() )
		{
		case 1:
			shape = std::shared_ptr<btCollisionShape>( new btSphereShape( constructionData[0] ) );
			break;
		case 2:
			shape = std::shared_ptr<btCollisionShape>( new btCapsuleShape( constructionData[0], constructionData[1]/2.0 ) );
			break;
		case 3:
			shape = std::shared_ptr<btCollisionShape>( new btBoxShape( btVector3( constructionData[0], constructionData[1], constructionData[2] ) ) );
			break;
		case 4:
			shape = std::shared_ptr<btCollisionShape>( new btCylinderShape( btVector3( constructionData[0], constructionData[1]/2.0, constructionData[2] ) ) );
			break;
		}
		
		if( !independent && shape )
		{
			shapesVecPtr[ constructionData ] = shape;
			shapesPtrVec[ shape ] = constructionData;
		}
		return shape;
	}
	std::shared_ptr<btCollisionShape> ret;
	return ret;
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::AddShape( std::vector < btScalar > constructionData, std::string name )
{
	std::shared_ptr<btCollisionShape> shape;
	if( name == "" )
	{
		shape = GetShape( constructionData, false );
	}
	else if( IsNameAvailable( name ) )
	{
		shape = GetShape( constructionData, true );
		if( shape )
		{
			shapesStrPtr[ name ] = shape;
			shapesPtrStr[ shape ] = name;
		}
	}
	return shape;
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetBox( btVector3 size, std::string name )
{
	std::vector < btScalar > constructionData = { size.x(), size.y(), size.z() };
	return AddShape( constructionData, name );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetBall( btScalar radius, std::string name )
{
	std::vector < btScalar > constructionData = { radius };
	return AddShape( constructionData, name );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCapsule( btScalar radius, btScalar height, std::string name )
{
	std::vector < btScalar > constructionData = { radius, height };
	return AddShape( constructionData, name );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCylinder( btScalar radius, btScalar height, std::string name )
{
	std::vector < btScalar > constructionData = { radius, height, radius, height };
	return AddShape( constructionData, name );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetShape( std::string name )
{
	auto it = shapesStrPtr.find( name );
	if( it != shapesStrPtr.end() )
	{
		return it->second;
	}
	return std::shared_ptr<btCollisionShape>();
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::AddCustomShape( std::string name, std::shared_ptr<btCollisionShape> shape )
{
	if( shapesPtrVec.find( shape ) == shapesPtrVec.end() && shapesPtrStr.find( shape ) == shapesPtrStr.end() )
	{
		if( IsNameAvailable( name ) )
		{
			shapesStrPtr[ name ] = shape;
			shapesPtrStr[ shape ] = name;
			return shape;
		}
	}
	return std::shared_ptr<btCollisionShape>();
}

void CollisionShapeManager::RemoveShape( std::shared_ptr<btCollisionShape> shape )
{
	{
		auto it = shapesPtrVec.find( shape );
		if( it != shapesPtrVec.end() )
		{
			DEBUG( std::string("Pointer use count: ") + std::to_string( shape.use_count() ) );
			/*
			if( shape.use_count() <= 1 )
			{
				shapesVecPtr.erase( it->second );
				shapesPtrVec.erase( it );
			}
			*/
			return;
		}
	}
	
	{
		auto it = shapesPtrStr.find( shape );
		if( it != shapesPtrStr.end() )
		{
			DEBUG( std::string("Pointer use count: ") + std::to_string( shape.use_count() ) );
			/*
			if( shape.use_count() <= 1 )
			{
				shapesStrPtr.erase( it->second );
				shapesPtrStr.erase( it );
			}
			*/
			return;
		}
	}
}

void CollisionShapeManager::Destroy()
{
	shapesStrPtr.clear();
	shapesPtrStr.clear();
	shapesPtrVec.clear();
	shapesVecPtr.clear();
}

CollisionShapeManager::CollisionShapeManager( Engine * engine )
{
	this->engine = engine;
}

CollisionShapeManager::~CollisionShapeManager()
{
	Destroy();
}

#endif

