
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef HEADER_H
#define HEADER_H

#include <Model.h>
#include <Engine.h>

#include <memory>

#include <string>

bool Convert( std::string objFile, std::string meshFile, bool scale = false, btVector3 size = btVector3(1,1,1) );
void ConvertMeshes( std::string convertMeshesListFile );

void LoadMeshes( std::string loadMeshesListFile, Engine * engine );


// between every word need to be a white sign ( ' ', '\n', '\t' )
int LoadMapPause( Engine * engine, std::string mapFile );		// return 0 -> no errors


void LoadModules( Engine * engine, const std::string & modulesList );

#endif

