
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef MESH_LOADER_CPP
#define MESH_LOADER_CPP

#include "..\css\Header.h"

#include <StdUtil.hpp>

#include <cassert>

void LoadMeshes(Engine *engine, const std::string &meshesListFileName) {
	std::ifstream file(meshesListFileName);
	if(file.good()) {
		while(!file.eof()) {
			std::string  meshFile(""), customName("");
			GetLine(file, meshFile);
			GetLine(file, customName);
			if(file.eof())
				break;
			if(meshFile == "" || customName == "")
				continue;
			
			std::shared_ptr<Model> ptr = engine->LoadModel(meshFile);
			assert((bool)ptr);
			if(ptr) {
				if(engine->SetCustomModelName(customName, ptr) == false) {
					MESSAGE(std::string("Couldn't set custom mesh: \"") + customName + "\"");
				}
			}
			else
			{
				MESSAGE(std::string("Couldn't load mesh: \"") + meshFile + "\"");
			}
		}
	}
}

#endif
