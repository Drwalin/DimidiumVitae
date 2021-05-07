
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <atomic>
#include <memory>
#include <chrono>

class Thread {
public:
	
	template<class Fn, typename... Args>
	Thread(Fn&& func, Args... args) {
		runFor = 0;
		running = true;
		pause = true;
		paused = true;
		stop = false;
		thread = std::thread(&Thread::Loop<Fn, Args...>, this, func, args...);
		thread.detach();
	}
	
	~Thread() {
		Join();
	}
	
	Thread() = delete;
	Thread(Thread&) = delete;
	Thread(Thread&&) = delete;
	Thread(const Thread&) = delete;
	Thread& operator=(Thread&) = delete;
	Thread& operator=(Thread&&) = delete;
	Thread& operator=(const Thread&) = delete;
	
	inline void RunOnce() {
		runFor = 1;
		paused = false;
	}
	
	inline void Run() {
		pause = false;
	}
	
	inline void Join() {
		Stop();
		while(running)
			std::this_thread::sleep_for(std::chrono::duration<float>(0.0001f));
	}
	
	inline void Stop() {
		stop = true;
	}
	
	
	inline void Pause() {
		pause = true;
	}
	
	inline void PauseBlock() {
		if(running) {
			Pause();
			while(!paused)
				std::this_thread::sleep_for(std::chrono::duration<float>(0.00001f));
		}
	}
	
	inline void Continue() {
		if(running)
			pause = false;
	}
	
	
	inline bool IsRunning() const {
		return running;
	}
	
	inline bool IsPaused() const {
		return running && paused;
	}
	
private:
	
	friend class Closer;
	class Closer {
	public:
		Closer(Thread& thread) : thread(thread) {
		}
		
		~Closer() {
			thread.running = false;
		}
		
		Thread& thread;
	};
	
	template<class Fn, typename... Args>
	void Loop(Fn&& func, Args... args) {
		running = true;
		Closer closer(*this);
		while(running && !stop) {
			if(pause && runFor==0) {
				paused = true;
				while(pause && !stop && runFor==0)
					std::this_thread::sleep_for(
							std::chrono::duration<float>(0.00001f));
				if(runFor > 0)
					--runFor;
				paused = false;
				if(stop)
					break;
			}
			func(args...);
		}
	}
	
private:
	
	std::thread thread;
	std::atomic<bool> pause, running, paused, stop;
	std::atomic<int> runFor;
};

#endif

