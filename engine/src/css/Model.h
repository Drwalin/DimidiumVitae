
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

#include <memory>

#include "..\lib\Debug.h"
#include "..\lib\AR.hpp"

class Engine;

class Model
{
private:
	
	std::shared_ptr<irr::scene::IAnimatedMesh> mesh;
	std::map < int, irr::video::SMaterial > materials;
	
	Engine * engine;
	
	std::string name;
	std::string fileName;
	
public:
	
	void SetName( std::string name );
	
	void SetMaterialsToNode( irr::scene::ISceneNode * node );
	
	std::shared_ptr<irr::scene::IAnimatedMesh> GetMesh();
	
	bool LoadFromFile( Engine * engine, std::string objFileName );
	
	void Destroy();
	
	Model( Model * other );
	Model();
	~Model();
};

#endif

