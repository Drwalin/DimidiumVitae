
//	This file is part of The Drwalin Engine project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef RESOURCE_CPP
#define RESOURCE_CPP

#include "../css/Resource.h"

#include <ctime>

Resource::~Resource() {
}

const std::string& Resource::GetName() const {
	return name;
}

Resource::Resource(const std::string &name) :
	name(name) {
}

#endif
