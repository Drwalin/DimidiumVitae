
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef HUD_H
#define HUD_H

#include "Menu.h"

class HUD : public Menu {
public:
	
	HUD();
	~HUD();
	
	virtual bool RenderSceneInBackground() const override;
	
	virtual void PutToBackground() override;
	virtual void RestoreFromBackground() override;
	
	virtual bool ReceiveEvents() const override;
	virtual void TickRender() override;
	
private:
	
};

#endif

