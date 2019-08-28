
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef ENGINE_RAY_TRACE_DATA_CPP
#define ENGINE_RAY_TRACE_DATA_CPP

#include <Engine.h>

bool Engine::RayTraceData::operator < ( const RayTraceData & other ) const
{
	return distance < other.distance;
}

Engine::RayTraceData::RayTraceData( btCollisionWorld::AllHitsRayResultCallback & hitData, unsigned id ) :
	distance(10000000000.0)
{
	if( hitData.m_collisionObjects.size() > id && hitData.m_hitNormalWorld.size() > id && hitData.m_hitPointWorld.size() > id )
	{
		const btCollisionObject * temp = hitData.m_collisionObjects.at( id );
		if( temp )
		{
			Object * objectT = (Object*)(temp->getUserPointer());
			if( objectT )
			{
				if( !objectT->IsTrigger() )
				{
					object = objectT->GetEngine()->GetObject( std::string(objectT->GetName()) );
					begin = hitData.m_rayFromWorld;
					end = hitData.m_rayToWorld;
					point = hitData.m_hitPointWorld.at( id );
					normal = hitData.m_hitNormalWorld.at( id );
					distance = begin.distance2( point );
				}
			}
			else
			{
				object = NULL;
			}
		}
	}
	else
	{
		DEBUG( "It should not appear" );
	}
}

Engine::RayTraceData::RayTraceData() :
	distance(10000000000.0)
{
}

std::shared_ptr<Object> Engine::RayTrace( btVector3 begin, btVector3 end, int channel, btVector3 & point, btVector3 & normal, const std::vector < std::shared_ptr<Object> > & ignoreObjects )
{
	point = normal = btVector3(0,0,0);
	
	btCollisionWorld::AllHitsRayResultCallback rayTraceResult( begin, end );
	world->GetDynamicsWorld()->rayTest( begin, end, rayTraceResult );
	if( rayTraceResult.hasHit() )
	{
		std::set < std::shared_ptr<Object> > ignoreObjectsSet( ignoreObjects.begin(), ignoreObjects.end() );		// does it sort it?
		std::set < RayTraceData > objects;
		
		for( int i = 0; i < rayTraceResult.m_collisionObjects.size(); ++i )
		{
			RayTraceData hitData( rayTraceResult, i );
			if( hitData.object && ( hitData.object->GetRayTraceChannel() & channel ) )
			{
				if( ignoreObjectsSet.find( hitData.object ) == ignoreObjectsSet.end() )
				{
					objects.insert( hitData );
				}
			}
		}
		
		if( objects.size() > 0 )
		{
			point = objects.begin()->point;
			normal = objects.begin()->normal;
			
			return objects.begin()->object;
		}
	}
	std::shared_ptr<Object> ret;
	return ret;
}

#endif

