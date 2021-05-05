
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TIME_COUNTER_CPP
#define TIME_COUNTER_CPP

#include <thread>

#include "../css/TimeCounter.h"

#include "../lib/Debug.h"

void TimeCounter::Sleep(float seconds) {
	std::this_thread::sleep_for(TimeDuration(seconds));
}

TimePoint TimeCounter::GetCurrentTime() {
	return std::chrono::high_resolution_clock::now();
}

float TimeCounter::GetDurationSeconds(TimePoint begin, TimePoint end) {
	return TimeCounter::GetDuration(begin, end).count();
}

TimeDuration TimeCounter::GetDuration(TimePoint begin, TimePoint end) {
	return std::chrono::duration_cast<TimeDuration>(end - begin);
}

void TimeCounter::SetTimeSpan(float time) {
	timeSpan = time;
}

float TimeCounter::GetPeakTime() const {
	if(array.size()) {
		float peak = TimeCounter::GetDurationSeconds(array.front().begin,
				array.front().end);
		float temp;
		for(auto it=array.begin(); it!=array.end(); ++it) {
			temp = TimeCounter::GetDurationSeconds(it->begin, it->end);
			if(peak < temp)
				peak = temp;
		}
		return peak;
	}
	return 0.00001f;
}

float TimeCounter::GetPitTime() const {
	if(array.size()) {
		float pit = TimeCounter::GetDurationSeconds(array.front().begin,
				array.back().end);
		float temp;
		for(auto it=array.begin(); it!=array.end(); ++it) {
			temp = TimeCounter::GetDurationSeconds(it->begin, it->end);
			if(pit > temp)
				pit = temp;
		}
		return pit;
	}
	return 0.00001f;
}

float TimeCounter::GetSmoothTime() const {
	float ret = 0.000001f;
	for(size_t i = 0; i < array.size(); ++i)
		ret += TimeCounter::GetDurationSeconds(array[i].begin, array[i].end);
	if(array.size()) {
		ret -= 0.000001f;
		ret /= float(array.size());
	}
	return ret;
}

void TimeCounter::SubscribeStart() {
	array.resize(array.size() + 1);
	array.back().begin = TimeCounter::GetCurrentTime();
	array.back().end = array.back().begin;
}

void TimeCounter::SubscribeEnd() {
	if(array.size() > 0) {
		TimePoint currentTime = TimeCounter::GetCurrentTime();
		array.back().end = currentTime;
		
		for(int i=array.size()-1; i>=0; --i) {
			if(TimeCounter::GetDurationSeconds(array[i].end, currentTime) >
					timeSpan) {
				array.erase(array.begin(), array.begin() + i);
				break;
			}
		}
	}
}

TimeCounter::TimeCounter() {
	array.reserve(50);
	timeSpan = 0.5f;
}

TimeCounter::~TimeCounter() {
	array.clear();
}

#endif
