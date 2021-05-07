
//	This file is part of The Drwalin Game project
// Copyright (C) 2020-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef CONSOLE_H
#define CONSOLE_H

#include <gui/Menu.h>

#include <mutex>
#include <vector>

class Console : public Menu {
public:
	
	Console();
	~Console();
	
	virtual bool RenderSceneInBackground() const override;
	
	virtual void MouseMoveEvent(int x, int y, int w, int dx, int dy, int dw)
			override;
	virtual void KeyPressedEvent(int keyCode) override;
	virtual void KeyHoldedEvent(int keyCode) override;
	virtual void KeyReleasedEvent(int keyCode) override;
	virtual void StringToEnterEvent(const std::string& str) override;
	
	static std::vector<std::wstring>& AccessLog();
	static void AddLog(const std::string& str);
	static void ClearLog();
	
protected:
	
	static std::mutex& Mutex();
	
	virtual void OnOtherEvent(const irr::SEvent::SGUIEvent &event) override;
	virtual void OnButtonClicked(Menu::Button *button, Menu::Element *element)
			override;
	
private:
	
	void Update();
	static std::wstring GetLogsText(size_t offset);
	
	static std::vector<std::string>& CommandsHistory();
	
private:
	
	int size;
	int scroll;
	
	int state;
	int commandsHistory;
	
	Menu::StaticText *input;
	Menu::StaticText *log;
	Menu::ScrollBar *scrollBar;
	class StringToEnter* stringGetter;
};

#endif

