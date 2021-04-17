
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btTransform.h>
#include <bullet/LinearMath/btQuaternion.h>
#include <bullet/btBulletCollisionCommon.h>

#include "Resource.h"
#include "PrimitiveCollisionShape.h"
#include <JSON.hpp>

#include <vector>
#include <memory>

class CollisionShape : public Resource {
public:
	
	CollisionShape(const JSON json);
	virtual ~CollisionShape();
	
	virtual void GetJSON(JSON json) const override;
	
	btCollisionShape* GetNewBtCollisionShape() const;
	static void DestroyBtCollisionShape(btCollisionShape *shape);
	
	virtual ResourceType GetResourceType() const override;
	
protected:
	
	std::vector<PrimitiveCollisionShape> primitives;
};

#endif
