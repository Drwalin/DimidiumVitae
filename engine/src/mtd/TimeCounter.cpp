
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

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

void TimeCounter::SetTimeSpan( float time )
{
	this->timeSpan = time;
}

float TimeCounter::GetPeakTime() const
{
	if( this->array.size() )
	{
		float peak = TimeCounter::GetDurationSeconds( this->array.front().begin, this->array.front().end );
		float temp;
		for( auto it=this->array.begin(); it!=this->array.end(); ++it )
		{
			temp = TimeCounter::GetDurationSeconds( it->begin, it->end );
			if( peak < temp )
				peak = temp;
		}
		return peak;
	}
	return 0.00001f;
}

float TimeCounter::GetPitTime() const
{
	if( this->array.size() )
	{
		float pit = TimeCounter::GetDurationSeconds( this->array.front().begin, this->array.back().end );
		float temp;
		for( auto it=this->array.begin(); it!=this->array.end(); ++it )
		{
			temp = TimeCounter::GetDurationSeconds( it->begin, it->end );
			if( pit > temp )
				pit = temp;
		}
		return pit;
	}
	return 0.00001f;
}

float TimeCounter::GetSmoothTime() const
{
	float ret = 0.000001f;
	for( int i = 0; i < this->array.size(); ++i )
		ret += TimeCounter::GetDurationSeconds( this->array[i].begin, this->array[i].end );
	if( this->array.size() )
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
	if( this->array.size() > 0 )
	{
		TimePoint currentTime = TimeCounter::GetCurrentTime();
		this->array.back().end = currentTime;
		
		for( int i=this->array.size()-1; i>=0; --i )
		{
			if( TimeCounter::GetDurationSeconds( this->array[i].end, currentTime ) > this->timeSpan )
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
	this->timeSpan = 0.5f;
}

TimeCounter::~TimeCounter()
{
	this->array.clear();
}

#endif

