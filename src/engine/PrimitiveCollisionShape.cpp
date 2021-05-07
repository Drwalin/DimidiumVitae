
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef PRIMITIVE_COLLISION_SHAPE_CPP
#define PRIMITIVE_COLLISION_SHAPE_CPP

#include "PrimitiveCollisionShape.h"

#include <util/Debug.h>
#include <util/Math.hpp>

#include <cstring>


CollisionShapeInfo::Info::~Info() {
}

void PrimitiveCollisionShape::Init(const std::string &type) {
	Destroy();
	if(type == "box")			info = new CollisionShapeInfo::Box;
	else if(type == "sphere")	info = new CollisionShapeInfo::Sphere;
	else if(type == "cylinder")	info = new CollisionShapeInfo::Cylinder;
	else if(type == "capsule")	info = new CollisionShapeInfo::Capsule;
	else if(type == "convex")	info = new CollisionShapeInfo::Convex;
	else if(type == "trimesh")	info = new CollisionShapeInfo::Trimesh;
}

btCollisionShape* CollisionShapeInfo::Box::Get() const {
	return new btBoxShape(size);
}

const std::string& CollisionShapeInfo::Box::GetName() const {
	const static std::string name = "box";
	return name;
}

JSON CollisionShapeInfo::Box::GetJSON() const {
	JSON json;
	json["size"] <<= size;
	return json;
}

void CollisionShapeInfo::Box::MakeFromJSON(const JSON& json) {
	size <<= json["size"];
}

CollisionShapeInfo::Box::Box() {
	size = btVector3(1,1,1);
}

CollisionShapeInfo::Box::~Box() {
}

btCollisionShape* CollisionShapeInfo::Sphere::Get() const {
	return new btSphereShape(radius);
}

const std::string& CollisionShapeInfo::Sphere::GetName() const {
	const static std::string name = "sphere";
	return name;
}

JSON CollisionShapeInfo::Sphere::GetJSON() const {
	JSON json;
	json["radius"] = radius;
	return json;
}

void CollisionShapeInfo::Sphere::MakeFromJSON(const JSON& json) {
	radius = json["radius"];
}

CollisionShapeInfo::Sphere::Sphere() {
	radius = 1;
}

CollisionShapeInfo::Sphere::~Sphere() {
}

btCollisionShape* CollisionShapeInfo::Cylinder::Get() const {
	return new btCylinderShape(btVector3(radius, height*0.5f, radius));
}

const std::string& CollisionShapeInfo::Cylinder::GetName() const {
	const static std::string name = "cylinder";
	return name;
}

JSON CollisionShapeInfo::Cylinder::GetJSON() const {
	JSON json;
	json["radius"] = radius;
	json["height"] = height;
	return json;
}

void CollisionShapeInfo::Cylinder::MakeFromJSON(const JSON& json) {
	radius = json["radius"];
	height = json["height"];
}

CollisionShapeInfo::Cylinder::Cylinder() {
	height = 1;
	radius = 1;
}

CollisionShapeInfo::Cylinder::~Cylinder() {
}

btCollisionShape* CollisionShapeInfo::Capsule::Get() const {
	return new btCapsuleShape(radius, height - 2.0f*radius);
}

const std::string& CollisionShapeInfo::Capsule::GetName() const {
	const static std::string name = "capsule";
	return name;
}

JSON CollisionShapeInfo::Capsule::GetJSON() const {
	JSON json;
	json["radius"] = radius;
	json["height"] = height;
	return json;
}

void CollisionShapeInfo::Capsule::MakeFromJSON(const JSON& json) {
	radius = json["radius"];
	height = json["height"];
}

CollisionShapeInfo::Capsule::Capsule() {
	height = 1;
	radius = 1;
}

CollisionShapeInfo::Capsule::~Capsule() {
}

btCollisionShape* CollisionShapeInfo::Convex::Get() const {
	return new btConvexHullShape(vertices.front().m_floats, vertices.size(),
			sizeof(btVector3));
}

const std::string& CollisionShapeInfo::Convex::GetName() const {
	const static std::string name = "convex";
	return name;
}

JSON CollisionShapeInfo::Convex::GetJSON() const {
	JSON json;
	json["vertices"].InitArray();
	JSON& verts = json["vertices"];
	for(auto vert : vertices) {
		verts.Array().push_back(vert.x());
		verts.Array().push_back(vert.y());
		verts.Array().push_back(vert.z());
	}
	return json;
}

void CollisionShapeInfo::Convex::MakeFromJSON(const JSON& json) {
	const JSON& verts = json["vertices"];
	vertices.resize(verts.size()/3);
	for(size_t i=0; i<vertices.size(); ++i) {
		vertices[i] = btVector3(verts[i*3].Real(), verts[i*3+1].Real(),
				verts[i*3+2].Real());
	}
}

CollisionShapeInfo::Convex::Convex() {
}

CollisionShapeInfo::Convex::~Convex() {
}

btCollisionShape* CollisionShapeInfo::Trimesh::Get() const {
	return new btBvhTriangleMeshShape((btStridingMeshInterface*)(triangleData),
			true, true);
}

const std::string& CollisionShapeInfo::Trimesh::GetName() const {
	const static std::string name = "trimesh";
	return name;
}

JSON CollisionShapeInfo::Trimesh::GetJSON() const {
	JSON json;
	json["vertices"].InitArray();
	JSON& verts = json["vertices"];
	for(auto vert : vertices) {
		verts.Array().push_back(vert.x());
		verts.Array().push_back(vert.y());
		verts.Array().push_back(vert.z());
	}
	json["indices"].InitArray();
	JSON& inds = json["indices"];
	inds.Array().resize(indices.size());
	size_t i = 0;
	for(int id : indices)
		inds[i++] = id;
	return json;
}

void CollisionShapeInfo::Trimesh::MakeFromJSON(const JSON& json) {
	if(triangleData)
		delete triangleData;
	triangleData = NULL;
	
	const JSON& verts = json["vertices"];
	vertices.resize(verts.size()/3);
	for(size_t i=0; i<vertices.size(); ++i)
		vertices[i] = btVector3(verts[i*3].Real(), verts[i*3+1].Real(),
				verts[i*3+2].Real());
	
	const JSON& inds = json["indices"];
	indices.resize(0);
	indices.reserve(inds.size());
	for(auto id : inds.Array())
		indices.emplace_back(id.Integer());
	
	Done();
}

CollisionShapeInfo::Trimesh::Trimesh() {
	triangleData = NULL;
}

CollisionShapeInfo::Trimesh::~Trimesh() {
	if(triangleData)
		delete triangleData;
	triangleData = NULL;
}

void CollisionShapeInfo::Trimesh::Done() {
	triangleData = new btTriangleIndexVertexArray(indices.size()/3,
			&(indices.front()), sizeof(int)*3, vertices.size(),
			vertices.front().m_floats, sizeof(btVector3));
}

btCollisionShape *PrimitiveCollisionShape::Get() const {
	if(info)
		return info->Get();
	return NULL;
}

PrimitiveCollisionShape::PrimitiveCollisionShape(
		PrimitiveCollisionShape &&other) {
	transform = other.transform;
	info = other.info;
	other.info = NULL;
}

PrimitiveCollisionShape::PrimitiveCollisionShape() {
	info = NULL;
}

PrimitiveCollisionShape::PrimitiveCollisionShape(const std::string &type) {
	info = NULL;
	Init(type);
}

PrimitiveCollisionShape::~PrimitiveCollisionShape() {
	Destroy();
}

void PrimitiveCollisionShape::Destroy() {
	if(info) {
		delete info;
		info = NULL;
	}
	transform = Math::EmptyTransform;
}

btTransform PrimitiveCollisionShape::GetTransform() const {
	return transform;
}

JSON PrimitiveCollisionShape::GetJSON() const {
	JSON json;
	if(info) {
		json.InitObject();
		json = info->GetJSON();
		json["transform"] <<= transform;
		json["type"] = info->GetName();
	}
	return json;
}

void PrimitiveCollisionShape::MakeFromJSON(const JSON& json) {
	Init(json["type"].String());
	if(info) {
		info->MakeFromJSON(json);
		transform <<= json["transform"];
	} else {
		throw std::string("Trying to make unknown "
				"PrimitiveCollisionShape type: " + json["type"].GetString());
	}
}

#endif
