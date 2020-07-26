
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
	return this->camera;
}

std::shared_ptr<SceneNode> Entity::GetSceneNode() {
	return this->sceneNode;
}

std::shared_ptr<CollisionShape> Entity::GetCollisionShape() {
	return this->collisionShape;
}

btCollisionShape* Entity::GetBtCollisionShape() const {
	if(body)
		return body->getCollisionShape();
	return NULL;
}

void Entity::SetTransform(const btTransform &transform) {
	this->currentTransform = transform;
	if(this->body) {
		this->body->activate(true);
		
		std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
		if(rigidBody)
			rigidBody->getMotionState()->setWorldTransform(this->currentTransform);
		else
			this->body->setWorldTransform(this->currentTransform);
		
		this->engine->GetWorld()->UpdateColliderForObject(this->body);
		this->body->activate(true);
	}
}

void Entity::SetLocation(const btVector3 &loc) {
	this->SetTransform(btTransform(this->currentTransform.getRotation(), loc));
}

void Entity::SetRotation(const btQuaternion &quat) {
	this->SetTransform(btTransform(quat, this->currentTransform.getOrigin()));
}

void Entity::Move(const btVector3 &move) {
	this->SetLocation(this->currentTransform.getOrigin() + move);
}

void Entity::Rotate(const btQuaternion &quat) {
	this->SetRotation(this->currentTransform.getRotation() *quat);
}

void Entity::Tick(const float deltaTime) {
	if(this->body) {
		std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
		if(rigidBody) rigidBody->getMotionState()->getWorldTransform(this->currentTransform);
		else this->currentTransform = this->GetBody<btCollisionObject>()->getWorldTransform();
	}
	
	if(this->sceneNode)
		this->sceneNode->SetTransform(this->currentTransform);
}

void Entity::ApplyDamage(const float damage, btVector3 point, btVector3 normal) {}

void Entity::SetMass(float mass) {
	std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
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

Engine *Entity::GetEngine() {
	return engine;
}

void Entity::SetScale(btVector3 scale) {
	this->scale = scale;
	if(this->body) {
		this->body->activate(true);
		this->body->getCollisionShape()->setLocalScaling(this->scale);
		this->engine->GetWorld()->UpdateColliderForObject(this->body);
		this->body->activate(true);
	}
	if(this->sceneNode)
		this->sceneNode->SetScale(this->scale);
}

btVector3 Entity::GetScale() {
	return scale;
}

btTransform Entity::GetTransform() {
	return this->currentTransform;
}

btVector3 Entity::GetLocation() const {
	return this->currentTransform.getOrigin();
}

const std::string &Entity::GetName() const {
	return name;
}

void Entity::SetModel(std::shared_ptr<Model> model) {
	if(this->engine) {
		if(this->engine->GetWindow()) {
			if(this->sceneNode) {
				this->sceneNode->Destroy();
				this->sceneNode = NULL;
			}
			
			this->sceneNode = std::shared_ptr<SceneNode>(new SceneNode);
			this->sceneNode->Init(this->engine, model);
		}
	}
}

void Entity::SetBody(std::shared_ptr<btCollisionObject> body, std::shared_ptr<CollisionShape> shape, int collisionFilterGroup, int collisionFilterMask) {
	this->DestroyBody();
	this->body = body;
	this->collisionShape = shape;
	this->engine->GetWorld()->AddBody(this->body, collisionFilterGroup, collisionFilterMask);
	this->body->setUserPointer(this);
	this->collisionGroup = collisionFilterGroup;
	this->collisionMask = collisionFilterMask;
}

void Entity::DestroyBody() {
	btCollisionShape *btCollisionShape = NULL;
	if(this->body) {
		btCollisionShape = this->body->getCollisionShape();
		this->engine->GetWorld()->RemoveBody(this->GetBody<btCollisionObject>());
		
		std::shared_ptr<btRigidBody> rigidBody = this->GetBody<btRigidBody>();
		if(rigidBody) {
			auto motionState = rigidBody->getMotionState();
			if(motionState) {
				rigidBody->setMotionState(NULL);
				delete motionState;
			}
		}
		
		this->body->setCollisionShape(NULL);
		
		assert(this->body);
		this->body.reset();
		
		this->body = NULL;
	}
	
	if(btCollisionShape) {
		CollisionShape::DestroyBtCollisionShape(btCollisionShape);
	}
	this->collisionShape = NULL;
}

void Entity::Destroy() {
	if(this->sceneNode)
		this->sceneNode->Destroy();
	this->sceneNode = NULL;
	this->DestroyBody();
	this->name = "";
	this->scale = btVector3(0,0,0);
	this->mass = 0.0f;
}

void Entity::Despawn() {
	this->Destroy();
}

void Entity::Load(std::istream &stream) {
}

void Entity::Save(std::ostream &stream) const {
}

void Entity::Init(Engine *engine) {
	this->engine = engine;
}

void Entity::Spawn(std::shared_ptr<Entity> self, std::string name, std::shared_ptr<CollisionShape> shape, btTransform transform) {
	this->mass = 0.0f;
	this->collisionShape = collisionShape;
	this->name = name;
	this->scale = btVector3(1,1,1);
	this->currentTransform = transform;
}

Entity::Entity() {
	this->mass = 1.0f;
	this->engine = NULL;
	this->name = "";
	this->scale = btVector3(1,1,1);
	this->sceneNode = NULL;
	this->collisionGroup = 0;
	this->collisionMask = 0;
}

Entity::~Entity() {
	this->Destroy();
}

bool Entity::HasCommon(int group, int mask) const {
	return (collisionGroup&mask) | (collisionMask&group);
}

#endif
