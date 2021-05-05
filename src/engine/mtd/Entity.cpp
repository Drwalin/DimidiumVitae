
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "../css/Entity.h"
#include "../css/Engine.h"

#include "../lib/Math.hpp"

#include <cassert>

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

void Entity::Tick(float deltaTime) {
	if(body) {
		btRigidBody *rigidBody = GetBody<btRigidBody>();
		if(rigidBody)
			rigidBody->getMotionState()->getWorldTransform(currentTransform);
		else
			currentTransform =
					GetBody<btCollisionObject>()->getWorldTransform();
	}
	
	if(sceneNode)
		sceneNode->SetTransform(currentTransform);
}

void Entity::ApplyDamage(float damage, btVector3 point, btVector3 normal) {}

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

size_t Entity::GetId() const {
	return id;
}

void Entity::SetModel(std::shared_ptr<Model> model) {
	sceneNode = NULL;
	sceneNode = std::shared_ptr<SceneNode>(new SceneNode);
	sceneNode->Init(model);
}

void Entity::SetBody(btCollisionObject* body,
		std::shared_ptr<CollisionShape> shape, int collisionFilterGroup,
		int collisionFilterMask) {
	DestroyBody();
	this->body = body;
	collisionShape = shape;
	sing::world->AddBody(body, collisionFilterGroup, collisionFilterMask);
	body->setUserPointer(this);
	collisionGroup = collisionFilterGroup;
	collisionMask = collisionFilterMask;
}

void Entity::DestroyBody() {
	if(body) {
		btCollisionShape *_btCollisionShape = body->getCollisionShape();
		sing::world->RemoveBody(body);
		
		btRigidBody *rigidBody = GetBody<btRigidBody>();
		if(rigidBody) {
			auto motionState = rigidBody->getMotionState();
			if(motionState) {
				rigidBody->setMotionState(NULL);
				delete motionState;
			}
		}
		
		delete body;
		body = NULL;
		
		if(_btCollisionShape) {
			CollisionShape::DestroyBtCollisionShape(_btCollisionShape);
		}
	}
	
	collisionShape = NULL;
}

Entity::Entity(const JSON& json) {
	mass = 0.0f;
	if(json.Object().count("shape"))
		collisionShape =
				sing::resourceManager->GetCollisionShape(json["shape"]);
	this->id = json["id"].Integer();
	scale = btVector3(1,1,1);
	currentTransform <<= json["transform"];
	JSON t;
	t <<= currentTransform;
	body = NULL;
	collisionGroup = 0;
	collisionMask = 0;
}

Entity::~Entity() {
	sceneNode = NULL;
	DestroyBody();
	id = 0;
	scale = btVector3(0,0,0);
	mass = 0.0f;
}

bool Entity::HasCommon(int group, int mask) const {
	return (collisionGroup&mask) | (collisionMask&group);
}

#endif
