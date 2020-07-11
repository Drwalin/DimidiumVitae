
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_CONSTRUCTOR_H
#define COLLISION_SHAPE_CONSTRUCTOR_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletCollisionCommon.h>

#include <memory>
#include <string>
#include <vector>

class PrimitiveShape {
public:
	
	enum Type
	{
		NONE = 0,
		BOX = 1,
		SPHERE = 2,
		CYLINDER = 3,
		CAPSULE = 4,
		CONVEX = 5,
		TRIMESH = 6
	};
	
	PrimitiveShape(PrimitiveShape::Type type);
	~PrimitiveShape();
	
	static PrimitiveShape::Type GetType(char ch);
	static char GetChar(PrimitiveShape::Type type);
	
	class BoxInfo {public:
		btVector3 size;
		BoxInfo();
		~BoxInfo();
	};
	class SphereInfo {public:
		float radius;
		SphereInfo();
		~SphereInfo();
	};
	class CylindricInfo {public:
		float height;
		float radius;
		CylindricInfo();
		~CylindricInfo();
	};
	class ConvexInfo {public:
		std::vector<btVector3> vertices;
		ConvexInfo();
		~ConvexInfo();
	};
	class TrimeshInfo {public:
		std::vector<btVector3> vertices;
		std::vector<int> indices;
		btTriangleIndexVertexArray *triangleData;
		TrimeshInfo();
		~TrimeshInfo();
		void Done();
	};
	
	btTransform transform;
	PrimitiveShape::Type type;
	union
	{
		BoxInfo *box;
		SphereInfo *sphere;
		CylindricInfo *cylinder;
		CylindricInfo *capsule;
		ConvexInfo *convex;
		TrimeshInfo *trimesh;
	};
	
	btCollisionShape *Get();
};

class CollisionShapeConstructor {
public:
	
	CollisionShapeConstructor();
	~CollisionShapeConstructor();
	
	void AddBox(const btTransform &transform, const btVector3 &halfSize);
	void AddSphere(const btTransform &transform, const float radius);
	void AddCylinder(const btTransform &transform, const float radius, const float height);
	void AddCapsule(const btTransform &transform, const float radius, const float height);
	
	std::string GetName() const;
	std::string GetCollisionShapeFileName() const;
	
	std::shared_ptr<btCollisionShape> Get();
	bool Load(const std::string &name, const std::string &collisionShapeFileName);
	static bool Convert(class Engine *engine, const std::string &objFileName, const std::string &collisionShapeFileName);
	
private:
	
	std::string name;
	std::string collisionShapeFileName;
	
	std::vector < std::shared_ptr<PrimitiveShape> > primitives;
	
	
	std::shared_ptr<PrimitiveShape> AddPrimitive(PrimitiveShape::Type type);
	
	void LoadOBJ(std::istream &stream, std::vector<std::vector<std::vector<btVector3>>> &objects);
	void CreateNewObject(std::vector<std::vector<btVector3>> &faces);
	void CreateFromFaces(std::vector<std::vector<std::vector<btVector3>>> &objects);
	
	void SavePrimitive(std::ostream &stream, std::shared_ptr<PrimitiveShape> primitive);
	void SaveShapeFile(std::ostream &stream);
	
	std::shared_ptr<PrimitiveShape> LoadPrimitive(std::istream &stream);
};

#endif

