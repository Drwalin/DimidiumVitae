
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_CONSTRUCTOR_H
#define COLLISION_SHAPE_CONSTRUCTOR_H

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <btBulletCollisionCommon.h>

#include <memory>
#include <string>
#include <vector>

class PrimitiveShape
{
public:
	
	enum Type
	{
		NONE,
		BOX,
		SPHERE,
		CYLINDER,
		CAPSULE,
		CONVEX,
		TRIMESH
	};
	
	class BoxInfo{public:
		btVector3 size;
		BoxInfo();
		~BoxInfo();
	};
	class SphereInfo{public:
		float radius;
		SphereInfo();
		~SphereInfo();
	};
	class CylindricInfo{public:
		float height;
		float radius;
		CylindricInfo();
		~CylindricInfo();
	};
	class ConvexInfo{public:
		std::vector<btVector3> vertices;
		ConvexInfo();
		~ConvexInfo();
	};
	class TrimeshInfo{public:
		std::vector<btVector3> vertices;
		std::vector<int> indices;
		btTriangleIndexVertexArray * triangleData;
		TrimeshInfo();
		~TrimeshInfo();
		void Done();
	};
	
	btTransform transform;
	PrimitiveShape::Type type;
	union
	{
		BoxInfo * box;
		SphereInfo * sphere;
		CylindricInfo * cylinder;
		CylindricInfo * capsule;
		ConvexInfo * convex;
		TrimeshInfo * trimesh;
	};
	
	btCollisionShape * Get();
	
	PrimitiveShape( PrimitiveShape::Type type );
	~PrimitiveShape();
};

class CollisionShapeConstructor
{
private:
	
	std::string name;
	std::string collisionShapeFileName;
	
	std::vector < std::shared_ptr<PrimitiveShape> > primitives;
	
	
	std::shared_ptr<PrimitiveShape> AddPrimitive( PrimitiveShape::Type type );
	
	void LoadOBJ( std::istream & stream, std::vector<std::vector<std::vector<btVector3>>> & objects );
	void CreateNewObject( std::vector<std::vector<btVector3>> & faces );
	void CreateFromFaces( std::vector<std::vector<std::vector<btVector3>>> & objects );
	
	void SavePrimitive( std::ostream & stream, std::shared_ptr<PrimitiveShape> primitive );
	void SaveShapeFile( std::ostream & stream );
	
	std::shared_ptr<PrimitiveShape> LoadPrimitive( std::istream & stream );
	
public:
	
	std::string GetName() const;
	std::string GetCollisionShapeFileName() const;
	
	std::shared_ptr<btCollisionShape> Get();
	bool Load( const std::string & name, const std::string & collisionShapeFileName );
	static bool Convert( const std::string & objFileName, const std::string & collisionShapeFileName );
	
	CollisionShapeConstructor();
	~CollisionShapeConstructor();
};

#endif

