
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef LOADING_SCREEN_CPP
#define LOADING_SCREEN_CPP

#include "../css/LoadingScreen.h"
#include <Engine.h>

LoadingScreen::LoadingScreen(Engine *engine) :
	Menu(engine) {
	loadingTexture = engine->GetResourceManager()->GetTexture("Textures/PoweredBy-inverted.png");
	AddImage(loadingTexture, 0, 0);
}
LoadingScreen::~LoadingScreen() {
	loadingTexture = NULL;
}

bool LoadingScreen::RenderSceneInBackground() const {
	return false;
}
void LoadingScreen::OnButtonClicked(Menu::Button *button, Menu::Element *element) {
}

#endif
