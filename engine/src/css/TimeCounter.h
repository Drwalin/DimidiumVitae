
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

#include <ctime>

#include <chrono>
#include <ratio>
#include <vector>

typedef std::chrono::high_resolution_clock::time_point TimePoint;
typedef std::chrono::duration<float> TimeDuration;

class TimeCounter
{
private:
	
	struct TimePair
	{
		TimePoint begin;
		TimePoint end;
	};
	
	std::vector < TimePair > array;
	
	float timeSpan;
	
public:
	
	static TimePoint GetCurrentTime();
	static TimeDuration GetDuration( TimePoint begin, TimePoint end );
	static float GetDurationSeconds( TimePoint begin, TimePoint end );
	
	void SetTimeSpan( float time );
	
	float GetPeakTime() const;
	float GetPitTime() const;
	float GetSmoothTime() const;
	
	void SubscribeStart();
	void SubscribeEnd();
	
	TimeCounter();
	~TimeCounter();
};

#endif

