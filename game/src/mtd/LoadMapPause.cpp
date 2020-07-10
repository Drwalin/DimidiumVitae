
//	This file is part of The Drwalin Game project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef LOAD_MAP_PAUSE_CPP
#define LOAD_MAP_PAUSE_CPP

#include "..\css\Header.h"

#include <Debug.h>

std::string ConvertToValue(std::string str) {
	if(str.size()) {
		if(str[str.size()-1] == ';')
			str.resize(str.size() - 1);
		
		if(str.size()) {
			int first = 0, last = str.size()-1;
			for(int i = 0; i < str.size(); ++i) {
				if(str[i] == '"') {
					first = i+1;
					for(; i < str.size(); ++i) {
						if(str[i] == '"')
							last = i-1;
					}
					break;
				}
			}
			
			int size = last - first + 1;
			
			if(size > 0)
				return std::string(str.c_str()+first, size);
		}
	}
	
	return std::string();
}

int LoadOneObjectData(std::ifstream &file, std::map < std::string, std::string > &data) {
	std::string text, word, key;
	
	do
	{
		text = "";
		file >> text;
		if(text == "}") {
			break;
		}
		else if(text == "{") {
		}
		else
		{
			key = text;
			text = "";
			file >> text;
			
			if(text == "=") {
				text = "";
				std::getline(file, text, ';');
				
				data[key] = ConvertToValue(text);
			}
			else
				return 4;
		}
	}
	while(!file.eof());
	
	return 0;
}

int LoadMapPause(Engine *engine, std::string mapFile) {
	if(engine == NULL)
		return 3;
	
	std::ifstream file(mapFile);
	
	if(!file.good())
		return 1;
	
	std::string text;
	
	
	
	
	while(!file.eof()) {
		/*
		std::getline(file, line);
		
		if(file.eof())
			break;
		
		if(line.size())
			if(line[line.size()-1] == 13)
				line.resize(line.size() - 1);
		*/
		
		file >> text;
		
		if(file.eof())
			break;
		
		if(text == "new") {
			text = "";
			file >> text;
			
			std::map < std::string, std::string > data;
			
			int ret = LoadOneObjectData(file, data);
			
			if(ret == 0) {
				btVector3 location(0,0,0);
				std::string name(""), model("");
				
				if(data.find("name") != data.end()) {
					name = data["name"];
				}
				else
				{
					DEBUG(std::string("Unable to load object name from map: ") + mapFile);
					continue;
				}
				
				if(data.find("model") != data.end()) {
					model = data["model"];
				}
				
				if(data.find("location") != data.end()) {
					std::stringstream ss(data["location"]);
					ss >> location.m_floats[0];
					ss >> location.m_floats[1];
					ss >> location.m_floats[2];
				}
				else
				{
					DEBUG(std::string("Unable to load object \"") + name + "\" location from map: " + mapFile);
					continue;
				}
				
				
				
				
				
				
				if(text == "Character" || text == "Player") {
					
					
					
					
					
				}
				else if(text == "Item" || text == "Static" || text == "Entity")////////////////////
				{
					btQuaternion rotation(btVector3(1, 1, 1), 0);
					std::string collisionShapeType("");
					
					collisionShapeType = 
					
					
					
					
				}
			}
			else
			{
				return ret;
			}
		}
	}
	
	return 0;
}

#endif

