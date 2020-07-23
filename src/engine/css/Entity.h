
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

#include "Camera.h"
#include "SceneNode.h"

class Entity {
public:

	Entity();
	virtual ~Entity();
	
	void SetCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> GetCamera();
	
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
	
	std::shared_ptr<btCollisionShape> GetCollisionShape();
	
	void SetMass(float mass);
	
	class Engine *GetEngine();
	
	const std::string &GetName() const;
	
	template <typename T = btCollisionObject >
	inline std::shared_ptr<T> GetBody() { return std::dynamic_pointer_cast<T>(this->body); }
	
	virtual void Tick(const float deltaTime);
	virtual void ApplyDamage(const float damage, btVector3 point, btVector3 normal);
	
	void SetModel(std::shared_ptr<Model> model);
	void SetBody(std::shared_ptr<btCollisionObject> body, std::shared_ptr<btCollisionShape> shape, int collisionFilterGroup=btBroadphaseProxy::DefaultFilter, int collisionFilterMask=btBroadphaseProxy::AllFilter);
	
	void Init(class Engine *engine);
	
	virtual void Load(std::istream &stream);
	virtual void Save(std::ostream &stream) const;
	virtual void Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<btCollisionShape> shape, btTransform transform);
	virtual void Despawn();
	
	virtual void Destroy();
	void DestroyBody();
	
	virtual int GetTypeSize() const = 0;								// return size of type in bytes
	virtual void Free() = 0;											// calls type destructor and frees an Entity
	virtual std::shared_ptr<Entity> New() const = 0;					// allocate memory and calls type constructor
	virtual std::string GetClassName() const = 0;						// returns type name
	
	bool HasCommon(int group, int mask) const;
	
protected:
	
	class Engine *engine;
	
	std::string name;
	
	btTransform currentTransform;
	
	std::shared_ptr<SceneNode> sceneNode;
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btCollisionObject> body;
	
	btVector3 scale;
	
	int collisionGroup;
	int collisionMask;
	
	float mass;
	
	std::shared_ptr<Camera> camera;
};

#endif

