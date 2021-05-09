
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef SIMULATION_CPP
#define SIMULATION_CPP

#include "Simulation.h"
#include "Engine.h"

#include <functional>

Simulation::Simulation() :
		thread(std::bind(&Simulation::Tick, this)) {
}

Simulation::~Simulation() {
}

void Simulation::Start() {
	thread.RunOnce();
}

void Simulation::Sync() {
	thread.PauseBlock();
}

void Simulation::Tick() {
	sing::engine->AsynchronousTick();
}

#endif

