
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#include <Script.h>
#include <Engine.h>
#include <JSON.hpp>
#include <Singleton.h>

#include "../../game/css/Console.h"

void Clear(const JSON& args) {
	Console::ClearLog();
}


