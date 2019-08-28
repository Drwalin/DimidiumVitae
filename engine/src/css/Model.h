
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_H
#define MODEL_H

#include <irrlicht\irrlicht.h>

#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cstring>
#include <cstdio>

#include <Debug.h>
#include <AR.hpp>
#include <memory>

#include <CustomCollisionData.h>

class Engine;

class Model
{
private:
	
	struct Face
	{
		unsigned a, b, c;
		btVector3 normal;
	};
	
	irr::scene::IAnimatedMesh * mesh;
	std::map < int, irr::video::SMaterial > materials;
	
	Engine * engine;
	
	std::string name;
	std::string fileName;
	
	btVector3 minAABB, maxAABB;
	
	std::shared_ptr<CustomCollisionShapeData> collisionShapeData;
	
public:
	
	class SHAPE
	{
	public:
		static const int TRIANGLE = 1;
		static const int CONVEX = 2;
	};
	
	void SetName( std::string name );
	
	void SetMaterialsToNode( irr::scene::ISceneNode * node );
	
	btVector3 GetInertia() const;
	
	irr::scene::IAnimatedMesh * GetMesh();
	
	bool LoadCustomCollisionShapeFromObj();
	
	bool LoadFromFile( Engine * engine, std::string objFileName );
	
	std::shared_ptr<btCollisionShape> GetCollisionShape( const int shape );
	void NullCustomCollisionShape();
	
	void Destroy();
	
	Model( const Model * other );
	Model();
	~Model();
};

#endif

