
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_CONSTRUCTOR_CPP
#define COLLISION_SHAPE_CONSTRUCTOR_CP

#include "..\css\CollisionShapeConstructor.h"
#include "..\css\Engine.h"

#include "..\lib\Debug.h"
#include "..\lib\Math.hpp"
#include "..\lib\StdUtil.hpp"
#include "..\lib\StlStreamExtension.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletCollisionCommon.h>

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstdlib>

PrimitiveShape::Type PrimitiveShape::GetType( char ch )
{
	switch( ch )
	{
	case 1: return PrimitiveShape::Type::BOX;
	case 2: return PrimitiveShape::Type::SPHERE;
	case 3: return PrimitiveShape::Type::CYLINDER;
	case 4: return PrimitiveShape::Type::CAPSULE;
	case 5: return PrimitiveShape::Type::CONVEX;
	case 6: return PrimitiveShape::Type::TRIMESH;
	}
	return PrimitiveShape::Type::NONE;
}

char PrimitiveShape::GetChar( PrimitiveShape::Type type )
{
	switch( type )
	{
	case PrimitiveShape::Type::BOX: return 1;
	case PrimitiveShape::Type::SPHERE: return 2;
	case PrimitiveShape::Type::CYLINDER: return 3;
	case PrimitiveShape::Type::CAPSULE: return 4;
	case PrimitiveShape::Type::CONVEX: return 5;
	case PrimitiveShape::Type::TRIMESH: return 6;
	}
	return 0;
}

PrimitiveShape::BoxInfo::BoxInfo()
{
	this->size = btVector3(1,1,1);
}

PrimitiveShape::BoxInfo::~BoxInfo()
{
}

PrimitiveShape::SphereInfo::SphereInfo()
{
	this->radius = 1;
}

PrimitiveShape::SphereInfo::~SphereInfo()
{
}

PrimitiveShape::CylindricInfo::CylindricInfo()
{
	this->height= 1;
	this->radius = 1;
}

PrimitiveShape::CylindricInfo::~CylindricInfo()
{
}

PrimitiveShape::ConvexInfo::ConvexInfo()
{
}

PrimitiveShape::ConvexInfo::~ConvexInfo()
{
}

PrimitiveShape::TrimeshInfo ::TrimeshInfo()
{
	this->triangleData = NULL;
}

PrimitiveShape::TrimeshInfo::~TrimeshInfo()
{
	if( this->triangleData )
		delete this->triangleData;
	this->triangleData = NULL;
}

void PrimitiveShape::TrimeshInfo::Done()
{
	this->triangleData = new btTriangleIndexVertexArray( this->indices.size() / 3, &(this->indices.front()), sizeof(int) * 3, this->vertices.size(), this->vertices.front().m_floats, sizeof(btVector3) );
}

btCollisionShape * PrimitiveShape::Get()
{
	switch( this->type )
	{
	case PrimitiveShape::Type::BOX:
		return new btBoxShape( this->box->size );
	case PrimitiveShape::Type::SPHERE:
		return new btSphereShape( this->sphere->radius );
	case PrimitiveShape::Type::CYLINDER:
		return new btCylinderShape( btVector3( this->cylinder->radius, this->cylinder->height, this->cylinder->radius ) );
	case PrimitiveShape::Type::CAPSULE:
		return new btCapsuleShape( this->capsule->radius, this->capsule->height - 2.0f*this->capsule->radius );
	case PrimitiveShape::Type::CONVEX:
		return new btConvexHullShape( this->convex->vertices.front().m_floats, this->convex->vertices.size(), sizeof(btVector3) );
	case PrimitiveShape::Type::TRIMESH:
		return new btBvhTriangleMeshShape( (btStridingMeshInterface*)(this->trimesh->triangleData), true, true );
	}
	return NULL;
}

PrimitiveShape::PrimitiveShape( PrimitiveShape::Type type )
{
	this->type = type;
	switch( type )
	{
	case PrimitiveShape::Type::BOX:
		this->box = new BoxInfo();
		break;
	case PrimitiveShape::Type::SPHERE:
		this->sphere = new SphereInfo();
		break;
	case PrimitiveShape::Type::CYLINDER:
		this->cylinder = new CylindricInfo();
		break;
	case PrimitiveShape::Type::CAPSULE:
		this->capsule = new CylindricInfo();
		break;
	case PrimitiveShape::Type::CONVEX:
		this->convex = new ConvexInfo();
		break;
	case PrimitiveShape::Type::TRIMESH:
		this->trimesh = new TrimeshInfo();
		break;
	default:
		this->type = NONE;
		this->box = NULL;
	}
}

PrimitiveShape::~PrimitiveShape()
{
	switch( this->type )
	{
	case PrimitiveShape::Type::BOX:
		delete this->box;
		break;
	case PrimitiveShape::Type::SPHERE:
		delete this->sphere;
		break;
	case PrimitiveShape::Type::CYLINDER:
		delete this->cylinder;
		break;
	case PrimitiveShape::Type::CAPSULE:
		delete this->capsule;
		break;
	case PrimitiveShape::Type::CONVEX:
		delete this->convex;
		break;
	case PrimitiveShape::Type::TRIMESH:
		delete this->trimesh;
		break;
	}
	this->type = NONE;
	this->box = NULL;
}



void CollisionShapeConstructor::AddBox( const btTransform & transform, const btVector3 & halfSize )
{
	std::shared_ptr<PrimitiveShape> shape = this->AddPrimitive( PrimitiveShape::Type::BOX );
	shape->transform = transform;
	shape->box->size = halfSize;
}

void CollisionShapeConstructor::AddSphere( const btTransform & transform, const float radius )
{
	std::shared_ptr<PrimitiveShape> shape = this->AddPrimitive( PrimitiveShape::Type::BOX );
	shape->transform = transform;
	shape->sphere->radius = radius;
}

void CollisionShapeConstructor::AddCylinder( const btTransform & transform, const float radius, const float height )
{
	std::shared_ptr<PrimitiveShape> shape = this->AddPrimitive( PrimitiveShape::Type::BOX );
	shape->transform = transform;
	shape->cylinder->height = height;
	shape->cylinder->radius = radius;
}

void CollisionShapeConstructor::AddCapsule( const btTransform & transform, const float radius, const float height )
{
	std::shared_ptr<PrimitiveShape> shape = this->AddPrimitive( PrimitiveShape::Type::BOX );
	shape->transform = transform;
	shape->capsule->height = height;
	shape->capsule->radius = radius;
}

std::string CollisionShapeConstructor::GetName() const
{
	return this->name;
}

std::string CollisionShapeConstructor::GetCollisionShapeFileName() const
{
	return this->collisionShapeFileName;
}

std::shared_ptr<PrimitiveShape> CollisionShapeConstructor::AddPrimitive( PrimitiveShape::Type type )
{
	std::shared_ptr<PrimitiveShape> primitive( new PrimitiveShape( type ) );
	this->primitives.insert( this->primitives.end(), primitive );
	return primitive;
}

std::shared_ptr<btCollisionShape> CollisionShapeConstructor::Get()
{
	if( this->primitives.size() == 0 )
		return NULL;
	if( this->primitives.size() == 1 )
		return std::shared_ptr<btCollisionShape>( this->primitives[0]->Get() );
	std::shared_ptr<btCompoundShape> compoundShape( new btCompoundShape( true, this->primitives.size() ) );
	for( int i=0; i<this->primitives.size(); ++i )
	{
		btCollisionShape * primitiveShape = this->primitives[i]->Get();
		if( primitiveShape )
			compoundShape->addChildShape( this->primitives[i]->transform, primitiveShape );
	}
	return std::dynamic_pointer_cast<btCollisionShape>( compoundShape );
}

std::shared_ptr<PrimitiveShape> CollisionShapeConstructor::LoadPrimitive( std::istream & stream )
{
	PrimitiveShape::Type type = PrimitiveShape::Type::NONE;
	int vertices, indices, i;
	
	char ch = 0;
	stream.read( &ch, 1 );
	type = PrimitiveShape::GetType( ch );
	
	if( stream.eof() )
		return NULL;
	
	if( type == PrimitiveShape::Type::NONE )
	{
		MESSAGE( "Cannot read PrimitiveShape::Type from file" );
		return NULL;
	}
	
	std::shared_ptr<PrimitiveShape> primitive = this->AddPrimitive( type );
	
	if( primitive == NULL )
	{
		MESSAGE( std::string("Cannot create PrimitiveShape with type: ") + std::to_string((int)type) );
		return NULL;
	}
	
	btQuaternion rot;
	btVector3 orig;
	stream.read( (char*)&(rot), sizeof(btQuaternion) );
	stream.read( (char*)&(orig), sizeof(float)*3 );
	primitive->transform = btTransform( rot, orig );
	
	switch( type )
	{
	case PrimitiveShape::Type::BOX:
		stream.read( (char*)&(primitive->box->size), sizeof(float)*3 );
		break;
	case PrimitiveShape::Type::SPHERE:
		stream.read( (char*)&(primitive->sphere->radius), sizeof(float) );
		break;
	case PrimitiveShape::Type::CYLINDER:
		stream.read( (char*)&(primitive->cylinder->height), sizeof(float) );
		stream.read( (char*)&(primitive->cylinder->radius), sizeof(float) );
		break;
	case PrimitiveShape::Type::CAPSULE:
		stream.read( (char*)&(primitive->capsule->height), sizeof(float) );
		stream.read( (char*)&(primitive->capsule->radius), sizeof(float) );
		break;
	case PrimitiveShape::Type::CONVEX:
		stream.read( (char*)&(vertices), sizeof(int) );
		primitive->convex->vertices.resize( vertices );
		for( i=0; i<vertices; ++i )
			stream.read( (char*)&(primitive->convex->vertices[i]), sizeof(float)*3 );
		break;
	case PrimitiveShape::Type::TRIMESH:
		stream.read( (char*)&(vertices), sizeof(int) );
		primitive->trimesh->vertices.resize( vertices );
		for( i=0; i<vertices; ++i )
			stream.read( (char*)&(primitive->trimesh->vertices[i]), sizeof(float)*3 );
		stream.read( (char*)&(indices), sizeof(int) );
		primitive->trimesh->indices.resize( indices );
		stream.read( (char*)&(primitive->trimesh->indices[0]), sizeof(int)*indices );
		primitive->trimesh->Done();
		break;
	}
	
	return primitive;
}

bool CollisionShapeConstructor::Load( const std::string & name, const std::string & collisionShapeFileName )
{
	this->name = name;
	this->collisionShapeFileName = collisionShapeFileName;
	igzfstream in( this->collisionShapeFileName.c_str() );
	while( this->LoadPrimitive( in ) );
	return bool(this->primitives.size());
}

void CollisionShapeConstructor::LoadOBJ( std::istream & stream, std::vector<std::vector<std::vector<btVector3>>> & objects )
{
	std::vector<btVector3> vertices;
	std::string line, word;
	float ftemp1, ftemp2, ftemp3;
	int itemp1;
	btVector3 vtemp1;
	while( !stream.eof() && stream.good() )
	{
		line = "";
		word = "";
		GetLine( stream, line );
		std::stringstream s(line);
		s.str( line );
		s >> word;
		switch( word[0] )
		{
		case 'v':
			s >> ftemp1;
			s >> ftemp2;
			s >> ftemp3;
			vertices.resize( vertices.size() + 1 );
			vertices.back() = btVector3(-ftemp1,ftemp2,-ftemp3);
			break;
		case 'o':
			objects.resize( objects.size() + 1 );
			break;
		case 'f':
			objects.back().resize( objects.back().size() + 1 );
			while( true )
			{
				word = "";
				s >> word;
				if( word == "" )
					break;
				itemp1 = atoi(word.c_str())-1;
				if( itemp1 < 0 )
				{
					MESSAGE( "From OBJ readed index less than 0" );
					return;
				}
				objects.back().back().insert( objects.back().back().end(), vertices[itemp1] );
			}
			break;
		}
	}
}

class PlaneFace
{
public:
	float distance;
	btVector3 normal;
	std::vector<btVector3>vertices;
};
void CollisionShapeConstructor::CreateNewObject( std::vector<std::vector<btVector3>> & faces )
{
	std::vector<PlaneFace> combinedFaces;
	std::vector<btVector3> vertices;
	
	btVector3 aproximatedCenter(0,0,0);
	int count = 0;
	for( int i=0; i<faces.size(); ++i )
	{
		for( int j=0; j<faces[i].size(); ++j, ++count )
			aproximatedCenter += faces[i][j];
	}
	aproximatedCenter /= float(count);
	
	for( int i=0; i<faces.size(); ++i )
	{
		std::vector<btVector3> & face = faces[i];
		if( face.size() < 3 )
			continue;
		btVector3 normal = (face[0]-face[1]).cross(face[0]-face[2]).normalized();
		float distance = normal.dot(face[0]);
		
		if( (aproximatedCenter-face[0]).dot( normal ) < 0 )
			normal *= -1.0f;
		
		int j;
		for( j=0; j<combinedFaces.size(); ++j )
		{
			if( (combinedFaces[j].distance-distance) < 0.001f && (combinedFaces[j].distance-distance) > -0.001f && (combinedFaces[j].normal-normal).length2() < 0.01f )
			{
				for( int k=0; k<face.size(); ++k )
				{
					int l;
					for( l=0; l<combinedFaces[j].vertices.size() && (combinedFaces[j].vertices[l]-face[k]).length2()>=0.01f; ++l );
					if( l >= combinedFaces[j].vertices.size() )
						combinedFaces[j].vertices.insert( combinedFaces[j].vertices.end(), face[k] );
				}
				break;
			}
		}
		if( j>=combinedFaces.size() )
		{
			combinedFaces.resize( combinedFaces.size() + 1 );
			combinedFaces.back().distance = distance;
			combinedFaces.back().normal = normal;
			combinedFaces.back().vertices = face;
		}
		
		for( j=0; j<face.size(); ++j )
		{
			int k;
			for( k=0; k<vertices.size(); ++k )
			{
				if( (vertices[k]-face[j]).length2() < 0.01 )
					break;
			}
			if( k >= vertices.size() )
				vertices.insert( vertices.end(), face[j] );
		}
	}
	
	btVector3 origin(0,0,0);
	for( int j=0; j<vertices.size(); ++j )
		origin += vertices[j];
	origin /= float(vertices.size());
	
#ifdef __COLLISION_SHAPE_CONSTUCTOR_USE_BOXES_WHEN_POSSIBLE
	bool useBoxes = true;
#else
	bool useBoxes = false;
#endif
	
	// is it box
	if( useBoxes && combinedFaces.size() == 6 && vertices.size() == 8 )
	{
		int i;
		for( i=0; i<combinedFaces.size(); ++i )
		{
			int j;
			for( j=i+1; j<combinedFaces.size(); ++j )
			{
				float dot = combinedFaces[i].normal.dot( combinedFaces[j].normal );
				if( !( dot > 0.99 || dot < -0.99 || ( dot < 0.01 && dot > -0.01 ) ) )
					break;
			}
			if( j!=combinedFaces.size() )
				break;
		}
		if( i==combinedFaces.size() )
		{
			btVector3 X, Y, Z;
			X = combinedFaces[0].normal;
			float dot;
			dot = X.dot( combinedFaces[1].normal );
			if( dot<0.01 && dot>-0.01 )
				Y = combinedFaces[1].normal;
			else
				Y = combinedFaces[2].normal;
			
			for( int j=2; j<combinedFaces.size(); ++j )
			{
				dot = X.dot( combinedFaces[j].normal );
				if( dot<0.01 && dot>-0.01 )
				{
					dot = Y.dot( combinedFaces[j].normal );
					if( dot<0.01 && dot>-0.01 )
					{
						Z = combinedFaces[j].normal;
						break;
					}
				}
			}
			
			//if( (X.cross(Y)-Z).length2() > 0.0 )
			//	Z = -Z;
			if( X.cross(Y).dot(Z) > 0.0 ) {
				Y = -Y;
				Z = -Z;
				X = -X;
			}
			
			btVector3 betweenXaxis = btVector3(1,0,0).cross(X);
			float betweenXangle = btVector3(1,0,0).angle(X);
			if( betweenXaxis.length() <= 0.01 )
			{
				betweenXaxis = btVector3(1,1,1);
				betweenXangle = 0;
			}
			betweenXaxis.normalize();
			btQuaternion rotX = btQuaternion( betweenXaxis, -betweenXangle );
			btTransform transformX( rotX, btVector3(0,0,0) );
			btVector3 betweenYaxis = (transformX*btVector3(0,1,0)).cross(Y);
			float betweenYangle = (transformX*btVector3(0,1,0)).angle(Y);
			if( betweenYaxis.length() <= 0.01 )
			{
				betweenYaxis = btVector3(1,1,1);
				betweenYangle = 0;
			}
			betweenXaxis.normalize();
			btQuaternion rotY = btQuaternion( betweenYaxis, -betweenYangle );
			btQuaternion rot = rotX * rotY;
			
			btVector3 minAxes, maxAxes;
			
			minAxes = btVector3( X.dot(vertices[0]), Y.dot(vertices[0]), Z.dot(vertices[0]) );
			maxAxes = minAxes;
			
			float temp;
			for( int j=1; j<vertices.size(); ++j )
			{
				temp = X.dot(vertices[j]);
				if( minAxes.x() > temp )
					minAxes.setX( temp );
				if( maxAxes.x() < temp )
					maxAxes.setX( temp );
				
				temp = Y.dot(vertices[j]);
				if( minAxes.y() > temp )
					minAxes.setY( temp );
				if( maxAxes.y() < temp )
					maxAxes.setY( temp );
				
				temp = Z.dot(vertices[j]);
				if( minAxes.z() > temp )
					minAxes.setZ( temp );
				if( maxAxes.z() < temp )
					maxAxes.setZ( temp );
			}
			
			btVector3 size = (minAxes-maxAxes) * 0.5f;
			if( size.x() < 0 )
				size.setX( -size.x() );
			if( size.y() < 0 )
				size.setY( -size.y() );
			if( size.z() < 0 )
				size.setZ( -size.z() );
			std::shared_ptr<PrimitiveShape> shape = this->AddPrimitive( PrimitiveShape::Type::BOX );
			shape->transform = btTransform( rot, origin );
			shape->box->size = size;
			return;
		}
	}
	
	// is it capsule
	{
	}
	
	// is it sphere
	{
	}
	
	// is it cylinder
	{
	}
	
	// convex shape
	{
		std::shared_ptr<PrimitiveShape> shape = this->AddPrimitive( PrimitiveShape::Type::CONVEX );
		shape->transform = btTransform( btQuaternion(btVector3(1,1,1),0), origin );
		shape->convex->vertices = vertices;
		for( int j=0; j<shape->convex->vertices.size(); ++j )
			shape->convex->vertices[j] -= origin;
		return;
	}
}

void CollisionShapeConstructor::CreateFromFaces( std::vector<std::vector<std::vector<btVector3>>> & objects )
{
	for( int i=0; i<objects.size(); ++i )
		this->CreateNewObject( objects[i] );
}

void CollisionShapeConstructor::SavePrimitive( std::ostream & stream, std::shared_ptr<PrimitiveShape> primitive )
{
	int vertices, indices, i;
	
	char ch = PrimitiveShape::GetChar( primitive->type );
	stream.write( &ch, 1 );
	
	btQuaternion rot = primitive->transform.getRotation();
	btVector3 orig = primitive->transform.getOrigin();
	stream.write( (char*)&(rot), sizeof(btQuaternion) );
	stream.write( (char*)&(orig), sizeof(float)*3 );
	
	switch( primitive->type )
	{
	case PrimitiveShape::Type::BOX:
		stream.write( (char*)&(primitive->box->size), sizeof(float)*3 );
		break;
	case PrimitiveShape::Type::SPHERE:
		stream.write( (char*)&(primitive->sphere->radius), sizeof(float) );
		break;
	case PrimitiveShape::Type::CYLINDER:
		stream.write( (char*)&(primitive->cylinder->height), sizeof(float) );
		stream.write( (char*)&(primitive->cylinder->radius), sizeof(float) );
		break;
	case PrimitiveShape::Type::CAPSULE:
		stream.write( (char*)&(primitive->capsule->height), sizeof(float) );
		stream.write( (char*)&(primitive->capsule->radius), sizeof(float) );
		break;
	case PrimitiveShape::Type::CONVEX:
		vertices = primitive->convex->vertices.size();
		stream.write( (char*)&(vertices), sizeof(int) );
		for( i=0; i<vertices; ++i )
			stream.write( (char*)&(primitive->convex->vertices[i]), sizeof(float)*3 );
		break;
	case PrimitiveShape::Type::TRIMESH:
		vertices = primitive->trimesh->vertices.size();
		indices = primitive->trimesh->indices.size();
		stream.write( (char*)&(vertices), sizeof(int) );
		for( i=0; i<vertices; ++i )
			stream.write( (char*)&(primitive->trimesh->vertices[i]), sizeof(float)*3 );
		stream.write( (char*)&(indices), sizeof(int) );
		stream.write( (char*)&(primitive->trimesh->indices[0]), sizeof(int)*indices );
		break;
	}
}

void CollisionShapeConstructor::SaveShapeFile( std::ostream & stream )
{
	for( int i=0; i<this->primitives.size(); ++i )
		this->SavePrimitive( stream, this->primitives[i] );
}

bool CollisionShapeConstructor::Convert( Engine * engine, const std::string & objFileName, const std::string & collisionShapeFileName )
{
	MESSAGE( "Converting: " + objFileName + " into: " + collisionShapeFileName );
	
	iirrfstream in( engine->GetWindow()->GetDevice()->getFileSystem()->createAndOpenFile( objFileName.c_str() ) );
	ogzfstream out( collisionShapeFileName.c_str() );
	if( in.good() && out.good() )
	{
		std::vector<std::vector<std::vector<btVector3>>> objects;		// objects->faces->vertices
		
		CollisionShapeConstructor constructor;
		
		constructor.LoadOBJ( in, objects );
		constructor.CreateFromFaces( objects );
		constructor.SaveShapeFile( out );
		return true;
	}
	return false;
}

CollisionShapeConstructor::CollisionShapeConstructor()
{
	this->name = "";
	this->collisionShapeFileName = "";
}

CollisionShapeConstructor::~CollisionShapeConstructor()
{
	this->primitives.clear();
	this->name = "";
	this->collisionShapeFileName = "";
}

#endif

