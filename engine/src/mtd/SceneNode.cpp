
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SCENE_NODE_CPP
#define SCENE_NODE_CPP

#include "..\css\SceneNode.h"
#include "..\css\Engine.h"
#include "..\lib\Math.hpp"

Animation SceneNode::GetAnimation( const std::string & name ) const
{
	if( this->model )
		return Animation( this->model->GetAnimation(name), this->iSceneNode );
	return Animation( 0, 0, 1 );
}

irr::scene::IAnimatedMeshSceneNode * SceneNode::New( class Engine * engine, std::shared_ptr<Model> model )
{
	if( engine )
	{
		if( engine->GetWindow() )
		{
			if( model )
			{
				if( model->GetMesh() )
				{
					irr::scene::IAnimatedMeshSceneNode * iSceneNode = engine->GetWindow()->GetSceneManager()->addAnimatedMeshSceneNode( model->GetMesh().get() );
					model->SetMaterialsToNode( iSceneNode );
					return iSceneNode;
				}
			}
		}
	}
	return NULL;
}

irr::scene::IAnimatedMeshSceneNode * SceneNode::GetISceneNode()
{
	return this->iSceneNode;
}

void SceneNode::SetTransform( btTransform transform )
{
	iSceneNode->setPosition( Math::GetIrrVec( transform.getOrigin() ) );
	irr::core::vector3d<float> eulerRadians;
	btQuaternion quat( transform.getRotation().getAxis() * btVector3(1,-1,-1), transform.getRotation().getAngle() );
    irr::core::quaternion q;
    q.fromAngleAxis( quat.getAngle(), irr::core::vector3d<float>(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z()) );
	q.toEuler( eulerRadians );
	iSceneNode->setRotation( eulerRadians * irr::core::RADTODEG );
}

void SceneNode::SetScale( btVector3 scale )
{
	this->iSceneNode->setScale( irr::core::vector3d<float>(scale.x(),scale.y(),scale.z()) );
}

std::shared_ptr<SceneNode> SceneNode::AddChild( std::shared_ptr<Model> model, irr::scene::IAnimatedMeshSceneNode * iSceneNode )
{
	std::shared_ptr<SceneNode> child( new SceneNode );
	child->Init( this->engine, model );
	iSceneNode->addChild( child->iSceneNode );
	this->children.insert( child );
	return child;
}

std::shared_ptr<SceneNode> SceneNode::AddChild( std::shared_ptr<Model> model )
{
	return this->AddChild( model, this->iSceneNode );
}

void SceneNode::DestroyChild( std::shared_ptr<SceneNode> child )
{
	this->iSceneNode->removeChild( child->iSceneNode );
	child->Destroy();
	this->children.erase( child );
}

void SceneNode::Init( class Engine * engine, std::shared_ptr<Model> model, irr::scene::IAnimatedMeshSceneNode * iParentSceneNode )
{
	this->engine = engine;
	this->model = model;
	this->iSceneNode = SceneNode::New( this->engine, this->model );
	this->iParentSceneNode = iParentSceneNode;
	if( this->iParentSceneNode )
		this->iParentSceneNode->addChild( this->iSceneNode );
}

void SceneNode::Destroy()
{
	if( this->iParentSceneNode )
	{
		this->iParentSceneNode->removeChild( this->iSceneNode );
		this->iParentSceneNode = NULL;
	}
	
	for( auto it=this->children.begin(); it!=this->children.end(); ++it )
		(*it)->Destroy();
	this->children.clear();
	
	if( this->iSceneNode )
	{
		this->iSceneNode->remove();
		this->iSceneNode = NULL;
	}
	
	this->model = NULL;
	this->engine = engine;
}

SceneNode::SceneNode()
{
	this->engine = NULL;
	this->iSceneNode = NULL;
}

SceneNode::~SceneNode()
{
}

#endif

