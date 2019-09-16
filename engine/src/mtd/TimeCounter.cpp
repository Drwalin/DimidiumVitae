
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TIME_COUNTER_CPP
#define TIME_COUNTER_CPP

#include "..\css\TimeCounter.h"

#include "..\lib\Debug.h"

TimePoint TimeCounter::GetCurrentTime()
{
	return std::chrono::high_resolution_clock::now();
}

float TimeCounter::GetDurationSeconds( TimePoint begin, TimePoint end )
{
	return TimeCounter::GetDuration( begin, end ).count();
}

TimeDuration TimeCounter::GetDuration( TimePoint begin, TimePoint end )
{
	return std::chrono::duration_cast<TimeDuration>( end - begin );
}

/*
void TimeCounter::SetTimeSpan( TimeDuration time )
{
	this->timeSpan = time;
}
*/

float TimeCounter::GetSmoothTime() const
{
	float ret = 0.000001f;
	for( int i = 0; i < this->array.size(); ++i )
		ret += TimeCounter::GetDurationSeconds( this->array[i].end, this->array[i].begin );
	if( array.size() )
	{
		ret -= 0.000001f;
		ret /= float(this->array.size());
	}
	return ret;
}

void TimeCounter::SubscribeStart()
{
	this->array.resize( this->array.size() + 1 );
	this->array.back().begin = TimeCounter::GetCurrentTime();
	this->array.back().end = this->array.back().begin;
}

void TimeCounter::SubscribeEnd()
{
	TimePoint curretnTime = TimeCounter::GetCurrentTime();
	this->array.back().end = curretnTime;
	
	if( this->array.size() )
	{
		for( int i=this->array.size()-1; i>=0; --i )
		{
			if( TimeCounter::GetDuration( curretnTime, this->array[i].end ) > this->timeSpan )
			{
				this->array.erase( this->array.begin(), this->array.begin() + i );
				break;
			}
		}
	}
}

TimeCounter::TimeCounter()
{
	this->array.reserve(50);
	this->timeSpan = TimeDuration(0.5f);
}

TimeCounter::~TimeCounter()
{
	this->array.clear();
}

#endif

