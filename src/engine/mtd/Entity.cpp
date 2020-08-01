
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "../css/Entity.h"
#include "../css/Engine.h"

#include "../lib/Math.hpp"

#include <cassert>

void Entity::SetCamera(std::shared_ptr<Camera> camera) {
	this->camera = camera;
}

std::shared_ptr<Camera> Entity::GetCamera() {
	return camera;
}

std::shared_ptr<SceneNode> Entity::GetSceneNode() {
	return sceneNode;
}

std::shared_ptr<CollisionShape> Entity::GetCollisionShape() {
	return collisionShape;
}

btCollisionShape* Entity::GetBtCollisionShape() const {
	if(body)
		return body->getCollisionShape();
	return NULL;
}

void Entity::SetTransform(const btTransform &transform) {
	currentTransform = transform;
	if(body) {
		body->activate(true);
		
		btRigidBody *rigidBody = GetBody<btRigidBody>();
		if(rigidBody)
			rigidBody->getMotionState()->setWorldTransform(currentTransform);
		else
			body->setWorldTransform(currentTransform);
		
		sing::world->UpdateColliderForObject(body);
		body->activate(true);
	}
}

void Entity::SetLocation(const btVector3 &loc) {
	SetTransform(btTransform(currentTransform.getRotation(), loc));
}

void Entity::SetRotation(const btQuaternion &quat) {
	SetTransform(btTransform(quat, currentTransform.getOrigin()));
}

void Entity::Move(const btVector3 &move) {
	SetLocation(currentTransform.getOrigin() + move);
}

void Entity::Rotate(const btQuaternion &quat) {
	SetRotation(currentTransform.getRotation() *quat);
}

void Entity::Tick(const float deltaTime) {
	if(body) {
		btRigidBody *rigidBody = GetBody<btRigidBody>();
		if(rigidBody) rigidBody->getMotionState()->getWorldTransform(currentTransform);
		else currentTransform = GetBody<btCollisionObject>()->getWorldTransform();
	}
	
	if(sceneNode)
		sceneNode->SetTransform(currentTransform);
}

void Entity::ApplyDamage(const float damage, btVector3 point, btVector3 normal) {}

void Entity::SetMass(float mass) {
	btRigidBody *rigidBody = GetBody<btRigidBody>();
	if(rigidBody) {
		this->mass = mass;
		btCollisionShape *shape = rigidBody->getCollisionShape();
		if(shape) {
			btVector3 inertia = rigidBody->getInvInertiaDiagLocal();
			shape->calculateLocalInertia(mass, inertia);
			rigidBody->setMassProps(mass, inertia);
			if(mass > 0.0f) {
				rigidBody->setGravity(btVector3(0.0f,-9.81f,0.0f));
			}
		}
	}
}

void Entity::SetScale(btVector3 scale) {
	this->scale = scale;
	if(body) {
		body->activate(true);
		body->getCollisionShape()->setLocalScaling(this->scale);
		sing::world->UpdateColliderForObject(body);
		body->activate(true);
	}
	if(sceneNode)
		sceneNode->SetScale(this->scale);
}

btVector3 Entity::GetScale() {
	return scale;
}

btTransform Entity::GetTransform() {
	return currentTransform;
}

btVector3 Entity::GetLocation() const {
	return currentTransform.getOrigin();
}

const std::string &Entity::GetName() const {
	return name;
}

void Entity::SetModel(std::shared_ptr<Model> model) {
	sceneNode = NULL;
	sceneNode = std::shared_ptr<SceneNode>(new SceneNode);
	sceneNode->Init(model);
}

void Entity::SetBody(btCollisionObject* body, std::shared_ptr<CollisionShape> shape, int collisionFilterGroup, int collisionFilterMask) {
	DestroyBody();
	this->body = body;
	collisionShape = shape;
	sing::world->AddBody(this->body, collisionFilterGroup, collisionFilterMask);
	this->body->setUserPointer(this);
	collisionGroup = collisionFilterGroup;
	collisionMask = collisionFilterMask;
}

void Entity::DestroyBody() {
	if(body) {
		btCollisionShape *_btCollisionShape = body->getCollisionShape();
		sing::world->RemoveBody(GetBody());
		
		btRigidBody *rigidBody = GetBody<btRigidBody>();
		if(rigidBody) {
			auto motionState = rigidBody->getMotionState();
			if(motionState) {
				rigidBody->setMotionState(NULL);
				delete motionState;
			}
		}
		
		body->setCollisionShape(NULL);
		delete body;
		body = NULL;
		
		if(_btCollisionShape) {
			CollisionShape::DestroyBtCollisionShape(_btCollisionShape);
		}
	}
	
	collisionShape = NULL;
}

void Entity::Destroy() {
	sceneNode = NULL;
	DestroyBody();
	name = "";
	scale = btVector3(0,0,0);
	mass = 0.0f;
}

void Entity::Despawn() {
	Destroy();
}

void Entity::Spawn(std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	mass = 0.0f;
	collisionShape = shape;
	this->name = name;
	scale = btVector3(1,1,1);
	currentTransform = transform;
}

Entity::Entity() {
	body = NULL;
	mass = 1.0f;
	name = "";
	scale = btVector3(1,1,1);
	collisionGroup = 0;
	collisionMask = 0;
}

Entity::~Entity() {
	Destroy();
}

bool Entity::HasCommon(int group, int mask) const {
	return (collisionGroup&mask) | (collisionMask&group);
}

#endif
