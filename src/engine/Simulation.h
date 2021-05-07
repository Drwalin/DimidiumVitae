
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SIMULATION_H
#define SIMULATION_H

#include <entities/Entity.h>

#include <util/Thread.h>

class Simulation {
public:
	
	Simulation();
	~Simulation();
	
	void Start();
	void Sync();
	
private:
	
	void Tick();
	
private:
	
	Thread thread;
};

#endif

