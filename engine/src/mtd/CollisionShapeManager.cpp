
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_MANAGER_CPP
#define COLLISION_SHAPE_MANAGER_CPP

#include "..\css\CollisionShapeManager.h"
#include "..\css\CollisionShapeConstructor.h"

bool CollisionShapeManager::IsNameAvailable( const std::string & name )
{
	if( name == "" )
		return false;
	return bool( this->shapeConstructors.find( name ) == this->shapeConstructors.end() );
}

std::string CollisionShapeManager::GetFirstAvailableName( const std::string & name )
{
	if( this->IsNameAvailable( name ) )
		return name;
	for( int i = 0;; ++i )
	{
		if( this->IsNameAvailable( name + std::to_string(i) ) )
			return name + std::to_string(i);
	}
	return name;
}

void CollisionShapeManager::DestroyShapeConstructionInfo( const std::string & name )
{
	auto it = this->shapeConstructors.find( name );
	if( it != this->shapeConstructors.end() )
	{
		delete it->second;
		it->second = NULL;
		this->shapeConstructors.erase( it );
	}
}

void CollisionShapeManager::DestroyShape( std::shared_ptr<btCollisionShape> & shape )
{
	btCompoundShape * compoundShape = dynamic_cast<btCompoundShape*>( shape.get() );
	if( compoundShape )
	{
		int shapes = compoundShape->getNumChildShapes();
		if( shapes > 0 )
		{
			for( int i=shapes-1; i>=0; --i )
			{
				auto shape = compoundShape->getChildShape( i );
				compoundShape->removeChildShapeByIndex( i );
				delete shape;
			}
		}
	}
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetBox( btVector3 size )
{
	return std::shared_ptr<btCollisionShape>( new btBoxShape( size ) );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetSphere( btScalar radius )
{
	return std::shared_ptr<btCollisionShape>( new btSphereShape( radius ) );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCapsule( btScalar radius, btScalar height )
{
	return std::shared_ptr<btCollisionShape>( new btCapsuleShape( radius, height ) );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCylinder( btScalar radius, btScalar height )
{
	return std::shared_ptr<btCollisionShape>( new btCylinderShape( btVector3( radius, height, radius ) ) );
}

std::shared_ptr<btCollisionShape> CollisionShapeManager::GetCustomShape( const std::string & name )
{
	auto it = this->shapeConstructors.find( name );
	if( it != this->shapeConstructors.end() )
	{
		return it->second->Get();
	}
	return NULL;
}

bool CollisionShapeManager::RegisterCustomShape( const std::string & name, const std::string & collisionShapeFileName )
{
	auto it = this->shapeConstructors.find( name );
	if( it == this->shapeConstructors.end() )
	{
		CollisionShapeConstructor * shape = new CollisionShapeConstructor();
		if( shape )
		{
			if( shape->Load( name, collisionShapeFileName ) )
			{
				this->shapeConstructors[name] = shape;
				return true;
			}
			delete shape;
		}
	}
	else if( it->second->GetCollisionShapeFileName() == collisionShapeFileName )
		return true;
	return false;
}

bool CollisionShapeManager::ConvertObjToCustomShape( const std::string & objFileName, const std::string & collisionShapeFileName )
{
	return CollisionShapeConstructor::Convert( objFileName, collisionShapeFileName );
}

void CollisionShapeManager::Destroy()
{
	for( auto it=this->shapeConstructors.begin(); it!=this->shapeConstructors.end(); ++it )
		delete it->second;
	this->shapeConstructors.clear();
}

CollisionShapeManager::CollisionShapeManager(){}

CollisionShapeManager::~CollisionShapeManager()
{
	this->Destroy();
}

#endif

