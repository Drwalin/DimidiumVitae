
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef HEADER_H
#define HEADER_H

#include <Model.h>
#include <Engine.h>

#include <memory>

#include <string>

void LoadMeshes( Engine * engine, const std::string & meshesListFileName );
void LoadModules( Engine * engine, const std::string & modulesListFileName );
void LoadShapes( Engine * engine, const std::string & shapesListFileName );
void LoadSounds( Engine * engine, const std::string & soundsListFileName );

std::string GetVersionString();
std::string GetVersionReleaseDateString();

#endif

