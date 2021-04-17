
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENTITY_H
#define ENTITY_H

#include <irrlicht/irrlicht.h>

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include <iostream>
#include <string>
#include <memory>
#include <set>

#include "../lib/Debug.h"

#include "SceneNode.h"
#include "CollisionShape.h"

#include <JSON.hpp>

#define __ENTITY_DERIVED_HEADER_FACTORY__(__ENTITY_CLASS_NAME_) virtual size_t GetTypeSize() const override; virtual const std::string& GetClassName() const override;
#define __ENTITY_DERIVED_CODE_FACTORY__(__ENTITY_CLASS_NAME_) size_t __ENTITY_CLASS_NAME_::GetTypeSize() const {return sizeof(__ENTITY_CLASS_NAME_);} const std::string& __ENTITY_CLASS_NAME_::GetClassName() const {const static std::string _m_Name_c_s = #__ENTITY_CLASS_NAME_ ; return _m_Name_c_s;} extern "C" __ENTITY_CLASS_NAME_* __Constructor_##__ENTITY_CLASS_NAME_##_Function(JSON json) {return new __ENTITY_CLASS_NAME_(json);}

class Entity {
public:
	
	Entity(JSON json);
	virtual ~Entity();
	
	std::shared_ptr<SceneNode> GetSceneNode();
	
	void SetTransform(const btTransform &transform);
	void SetLocation(const btVector3 &loc);
	void SetRotation(const btQuaternion &quat);
	void Move(const btVector3 &move);
	void Rotate(const btQuaternion &quat);
	btTransform GetTransform();
	btVector3 GetLocation() const;
	virtual void SetScale(btVector3 scale);
	btVector3 GetScale();
	
	std::shared_ptr<CollisionShape> GetCollisionShape();
	btCollisionShape* GetBtCollisionShape() const;
	
	void SetMass(float mass);
	
	uint64_t GetId() const;
	
	template <typename T = btCollisionObject >
	inline T* GetBody() { return dynamic_cast<T*>(this->body); }
	
	virtual void Tick(const float deltaTime);
	virtual void ApplyDamage(const float damage, btVector3 point, btVector3 normal);
	
	void SetModel(std::shared_ptr<Model> model);
	void SetBody(btCollisionObject *body, std::shared_ptr<CollisionShape> shape, int collisionFilterGroup=btBroadphaseProxy::DefaultFilter, int collisionFilterMask=btBroadphaseProxy::AllFilter);
	
	virtual uint64_t GetTypeSize() const = 0;					// return size of type in bytes
	virtual const std::string& GetClassName() const = 0;	// returns type name
	
	bool HasCommon(int group, int mask) const;
	
protected:
	
	void DestroyBody();
	
protected:
	
	uint64_t id;
	
	btTransform currentTransform;
	
	std::shared_ptr<SceneNode> sceneNode;
	std::shared_ptr<CollisionShape> collisionShape;
	btCollisionObject *body;
	
	btVector3 scale;
	
	int collisionGroup;
	int collisionMask;
	
	float mass;
};

#endif

