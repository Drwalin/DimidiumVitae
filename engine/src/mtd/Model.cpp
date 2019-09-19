
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MODEL_CPP
#define MODEL_CPP

#include "..\css\Model.h"
#include "..\css\Engine.h"

void Model::SetMaterialsToNode( irr::scene::ISceneNode * node )
{
	node->setMaterialFlag( irr::video::EMF_LIGHTING, false );
}

std::shared_ptr<irr::scene::IAnimatedMesh> Model::GetMesh()
{
	return this->mesh;
}

void Model::SetName( std::string name )
{
	this->name = name;
}

bool Model::LoadFromFile( Engine * engine, std::string fileName )
{
	this->Destroy();
	if( engine == NULL )
		return false;
	
	irr::scene::IAnimatedMesh * newMesh = engine->GetWindow()->sceneManager->getMesh( fileName.c_str() );
	if( newMesh == NULL )
		return false;
	this->mesh = std::shared_ptr<irr::scene::IAnimatedMesh>( newMesh, [](irr::scene::IAnimatedMesh*ptr){MESSAGE("Should use: \"ptr->drop();\" here")} );
	MESSAGE( std::string("Model loaded: ") + fileName );
	
	this->fileName = fileName;
	this->engine = engine;
	
	return true;
}

void Model::Destroy()
{
	this->mesh = NULL;
	this->engine = NULL;
	this->name = "";
	this->fileName = "";
}

Model::Model( Model * other )
{
	if( this->mesh )
		this->mesh.reset();
	if( other )
	{
		this->mesh = other->mesh;
		this->materials = other->materials;
		this->engine = other->engine;
		this->name = other->name;
		this->fileName = other->fileName;
	}
}

Model::Model()
{
	this->engine = NULL;
}

Model::~Model()
{
	this->Destroy();
}

#endif
 
