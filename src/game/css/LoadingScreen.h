
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <Menu.h>

class LoadingScreen : public Menu {
public:
	
	LoadingScreen(class Engine *engine);
	~LoadingScreen();
	
	virtual bool RenderSceneInBackground() const override;
	
protected:
	
	virtual void OnButtonClicked(Menu::Button *button, Menu::Element *element) override;
	
protected:
	
	std::shared_ptr<Texture> loadingTexture;
};

#endif
