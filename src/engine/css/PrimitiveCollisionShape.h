
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PRIMITIVE_COLLISION_SHAPE_H
#define PRIMITIVE_COLLISION_SHAPE_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletCollisionCommon.h>

#include <memory>
#include <string>
#include <vector>

#include <JSON.hpp>

namespace CollisionShapeInfo {
	class Info {
	public:
		virtual ~Info();
		virtual btCollisionShape* Get() const =0;
		virtual const std::string& GetName() const =0;
		virtual void GetJSON(JSON& json) const =0;
		virtual void MakeFromJSON(const JSON& json) =0;
	};
	
	class Box : public Info {
	public:
		btVector3 size;
		
		virtual btCollisionShape* Get() const override;
		virtual const std::string& GetName() const override;
		virtual void GetJSON(JSON& json) const override;
		virtual void MakeFromJSON(const JSON& json) override;
		Box();
		~Box();
	};
	class Sphere : public Info {
	public:
		float radius;
		
		virtual btCollisionShape* Get() const override;
		virtual const std::string& GetName() const override;
		virtual void GetJSON(JSON& json) const override;
		virtual void MakeFromJSON(const JSON& json) override;
		Sphere();
		~Sphere();
	};
	class Cylinder : public Info {
	public:
		float height;
		float radius;
		
		virtual btCollisionShape* Get() const override;
		virtual const std::string& GetName() const override;
		virtual void GetJSON(JSON& json) const override;
		virtual void MakeFromJSON(const JSON& json) override;
		Cylinder();
		~Cylinder();
	};
	class Capsule : public Info {
	public:
		float height;
		float radius;
		
		virtual btCollisionShape* Get() const override;
		virtual const std::string& GetName() const override;
		virtual void GetJSON(JSON& json) const override;
		virtual void MakeFromJSON(const JSON& json) override;
		Capsule();
		~Capsule();
	};
	class Convex : public Info {
	public:
		std::vector<btVector3> vertices;
		
		virtual btCollisionShape* Get() const override;
		virtual const std::string& GetName() const override;
		virtual void GetJSON(JSON& json) const override;
		virtual void MakeFromJSON(const JSON& json) override;
		Convex();
		~Convex();
	};
	class Trimesh : public Info {
	public:
		std::vector<btVector3> vertices;
		std::vector<int> indices;
		btTriangleIndexVertexArray *triangleData;
		
		virtual btCollisionShape* Get() const override;
		virtual const std::string& GetName() const override;
		virtual void GetJSON(JSON& json) const override;
		virtual void MakeFromJSON(const JSON& json) override;
		Trimesh();
		~Trimesh();
		void Done();
	};
}

class PrimitiveCollisionShape {
public:
	
	PrimitiveCollisionShape();
	PrimitiveCollisionShape(const PrimitiveCollisionShape &other) = delete;
	PrimitiveCollisionShape(PrimitiveCollisionShape &&other);
	PrimitiveCollisionShape(const std::string &type);
	~PrimitiveCollisionShape();
	
	PrimitiveCollisionShape& operator=(const PrimitiveCollisionShape &) =delete;
	
	
	void Init(const std::string &type);
	
	const std::string& GetType() const;
	
	void GetJSON(JSON& json) const;
	void MakeFromJSON(const JSON& json);
	
	void Destroy();
	
	btTransform GetTransform() const;
	btCollisionShape *Get() const;
	
	friend class CollisionShape;
	
private:
	
	btTransform transform;
	CollisionShapeInfo::Info *info;
};

#endif
