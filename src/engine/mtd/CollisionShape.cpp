
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef COLLISION_SHAPE_CPP
#define COLLISION_SHAPE_CPP

#include "../css/CollisionShape.h"
#include "../css/Singleton.h"

#include <Debug.h>
#include <Math.hpp>

CollisionShape::CollisionShape(const JSON json) :
	Resource(json) {
	try {
		JSON shape;
		if(name != "")
			shape = sing::fileSystem->ReadJSON(name);
		else
			shape = json;
		
		primitives.resize(shape["primitives"].Size());
		int i = 0;
		for(auto it : shape["primitives"]) {
			primitives[i].MakeFromJSON(it.Value());
			++i;
		}
		return;
	} catch(std::string e) {
		MESSAGE("\n Excepion while creating Collision shape: " + e + " ; for JSON: " + json.Write());
	} catch(std::exception e) {
		MESSAGE("\n Excepion while creating Collision shape: " + std::string(e.what()) + " ; for JSON: " + json.Write());
	} catch(char *e) {
		MESSAGE("\n Excepion while creating Collision shape: " + std::string(e) + " ; for JSON: " + json.Write());
	} catch(...) {
		MESSAGE(std::string("\n Unknown exception while creating Collision shape") + " ; for JSON: " + json.Write());
	}
	primitives.clear();
}

CollisionShape::~CollisionShape() {
}

btCollisionShape* CollisionShape::GetNewBtCollisionShape() const {
	if(primitives.size() == 1) {
		return primitives.back().Get();
	} else if(primitives.size() > 1) {
		btCompoundShape *compoundShape = new btCompoundShape(true, primitives.size());
		for(int i=0; i<primitives.size(); ++i) {
			compoundShape->addChildShape(primitives[i].GetTransform(), primitives[i].Get());
		}
		return compoundShape;
	}
	return NULL;
}

void CollisionShape::DestroyBtCollisionShape(btCollisionShape *shape) {
	if(shape) {
		btCompoundShape *compoundShape = dynamic_cast<btCompoundShape*>(shape);
		if(compoundShape) {
			int shapes = compoundShape->getNumChildShapes();
			if(shapes > 0) {
				for(int i=shapes-1; i>=0; --i) {
					auto primitiveShape = compoundShape->getChildShape(i);
					compoundShape->removeChildShapeByIndex(i);
					delete primitiveShape;
				}
			}
		}
		delete shape;
	}
}

Resource::ResourceType CollisionShape::GetResourceType() const {
	return Resource::COLLISIONSHAPE;
}

void CollisionShape::GetJSON(JSON json) const {
	json.InitObject();
	json["class"] = "CollisionShape";
	if(name != "") {
		json["name"] = name;
	} else {
		json["primitives"].InitArray();
		json["primitives"].Resize(primitives.size());
		int i=0;
		for(auto& it : primitives) {
			it.GetJSON(json["primitives"][i]);
			++i;
		}
	}
}

#endif
