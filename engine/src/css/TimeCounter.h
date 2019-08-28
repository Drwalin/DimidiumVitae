
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

#include <ctime>

#include <vector>

class TimeCounter
{
private:
	
	struct TimePair
	{
		float begin;
		float end;
	};
	
	std::vector < TimePair > array;
	
	float timeSpan;
	
public:
	
	void SetTimeSpan( float time );
	
	float GetSmoothTime() const;
	
	void SubscribeStart();
	void SubscribeEnd();
	
	TimeCounter();
	~TimeCounter();
};

#endif

