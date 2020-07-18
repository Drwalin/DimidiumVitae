
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef GUI_CPP
#define GUI_CPP

#include "../css/GUI.h"
#include "../css/Window.h"

#include "../lib/Debug.h"

#include <cstdio>
#include <cstring>

GUIDrawEvent &GUIDrawEvent::operator=(const GUIDrawEvent &other) {
	this->type = other.type;
	switch(this->type) {
	case GUIDrawEvent::Type::TEXT:
		this->text = other.text;
		break;
	case GUIDrawEvent::Type::IMAGE:
		this->image = other.image;
		break;
	}
	return (*this);
}

void GUIDrawEvent::Draw(irr::video::IVideoDriver *videoDriver) {
	switch(this->type) {
	case GUIDrawEvent::Type::TEXT:
		this->text.font->draw(this->text.str, this->text.destiny, this->text.color);
		break;
	case GUIDrawEvent::Type::IMAGE:
		videoDriver->draw2DImage(this->image.texture, this->image.destiny, this->image.source, NULL, &(this->image.color), true);
		break;
	}
}

GUIDrawEvent::GUIDrawEvent(Font *font, Rectanglei destiny, Color color, char *str) {
	this->type = GUIDrawEvent::Type::TEXT;
	this->text.font = font;
	snprintf(this->text.str, sizeof(this->text.str), "%s", str);
	this->text.destiny = destiny;
	this->text.color = color;
}

GUIDrawEvent::GUIDrawEvent(Texture *texture, Rectanglei source, Rectanglei destiny, Color color) {
	this->type = GUIDrawEvent::Type::IMAGE;
	this->image.texture = texture;
	this->image.source = source;
	this->image.destiny = destiny;
	this->image.color = color;
}

GUIDrawEvent::GUIDrawEvent() {
	this->type = GUIDrawEvent::Type::NONE;
}

GUIDrawEvent::GUIDrawEvent(const GUIDrawEvent &other) {
	(*this) = other;
}

Font *GUI::GetDefaultFont() {
	return this->defaultFont;
}

void GUI::Flush() {
	irr::video::IVideoDriver *videoDriver = this->window->GetVideoDriver();
	for(auto it=this->toDraw.begin(); it!=this->toDraw.end(); ++it)
		it->Draw(videoDriver);
	this->toDraw.clear();
}

void GUI::Init(Window *window) {
	this->window = window;
	
	this->defaultFont = this->window->GetDevice()->getGUIEnvironment()->getFont("./media/Fonts/courier.bmp");
	this->currentFont = this->defaultFont;
	
	this->workSpace = Rectanglei(0, 0, this->GetWindowSize().X, this->GetWindowSize().Y);
	this->newLinePosition = Vectori(0,0);
	this->currentColor = Color(255,255,255,255);
	this->cursorPosition = Vectori(0,0);
}

Vectori GUI::GetWindowSize() const {
	Vectori ret;
	ret.X = this->window->GetVideoDriver()->getScreenSize().Width;
	ret.Y = this->window->GetVideoDriver()->getScreenSize().Height;
	return ret;
}

Vectori GUI::GetWorkspaceSize() const {
	Vectori ret;
	ret.X = this->workSpace.LowerRightCorner.X-this->workSpace.UpperLeftCorner.X;
	ret.Y = this->workSpace.LowerRightCorner.Y-this->workSpace.UpperLeftCorner.Y;
	return ret;
}

Vectori GUI::GetWorkspacePosition() const {
	Vectori ret;
	ret.X = this->workSpace.UpperLeftCorner.X;
	ret.Y = this->workSpace.UpperLeftCorner.Y;
	return ret;
}

void GUI::PrintNewLine() {
	this->newLinePosition.X = 0;
	this->cursorPosition = this->newLinePosition;
	this->newLinePosition.Y += this->currentFont->getDimension(L"l").Height;
}

void GUI::PrintTab() {
	int tabWidth = this->currentFont->getDimension(L"    ").Width;
	this->cursorPosition.X = (int(this->cursorPosition.X / tabWidth) *tabWidth) + tabWidth;
	if(this->cursorPosition.X >= this->GetWorkspaceSize().X)
		this->PrintNewLine();
}

void GUI::PrintOneBufferLineImplicitly(char *str, int width, int height) {
	Rectanglei rectangle(
			GetWorkspacePosition().X + this->cursorPosition.X,
			GetWorkspacePosition().Y + this->cursorPosition.Y,
			1,
			1
		);
	
	this->toDraw.insert(this->toDraw.end(), GUIDrawEvent(this->currentFont, rectangle, this->currentColor, str));
	this->cursorPosition.X += width;
	if(this->newLinePosition.Y < this->cursorPosition.Y + height)
		this->newLinePosition.Y = this->cursorPosition.Y + height;
}

void GUI::PrintOneLine(char *str, int length, int width, int height) {
	char buffer[64];
	memcpy(buffer, str, length);
	buffer[length] = 0;
	this->PrintOneBufferLineImplicitly(buffer, width, height);
}

void GUI::PrintOneText(char *str, int length) {
	if(length == 0)
		return;
	Vectori wokrSpaceSize = this->GetWorkspaceSize();
	while(*str) {
		int i=0;
		int height=0, width=0;
		for(i=0; i<length && i<63 && str[i]; ++i) {
			wchar_t oneCharwstring[2] = {(wchar_t)str[i],0};
			auto size = this->currentFont->getDimension(oneCharwstring);
			if(size.Width + width < wokrSpaceSize.X)
				width += size.Width;
			else
			{
				--i;
				break;
			}
			if(height < size.Height)
				height = size.Height;
		}
		this->PrintOneLine(str, i, width, height);
		str += i;
		length -= i;
		if(length <= 0)
			break;
	}
}

GUI &GUI::operator << (const char *str) {
	char *ptr = (char*)str;
	for(int i=0;; ++i) {
		if(ptr[i] == '\n') {
			this->PrintOneText(ptr, i);
			ptr += i+1;
			i = -1;
			this->PrintNewLine();
		}
		else if(ptr[i] == '\t') {
			this->PrintOneText(ptr, i);
			ptr += i+1;
			i = -1;
			this->PrintTab();
		}
		else if(ptr[i] == 0) {
			this->PrintOneText(ptr, i);
			break;
		}
	}
	return *this;
}

GUI &GUI::operator << (const std::string &str) {
	return (*this) << str.c_str();
}

GUI &GUI::operator << (const bool val) {
	char str[64];
	snprintf(str, 64, val ? "true" : "false");
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const char val) {
	char str[64];
	snprintf(str, 64, "%c", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const unsigned char val) {
	char str[64];
	snprintf(str, 64, "%c", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const short val) {
	char str[64];
	snprintf(str, 64, "%d", (int)val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const unsigned short val) {
	char str[64];
	snprintf(str, 64, "%u", (unsigned)val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const int val) {
	char str[64];
	snprintf(str, 64, "%d", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const unsigned int val) {
	char str[64];
	snprintf(str, 64, "%u", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const long long int val) {
	char str[64];
	snprintf(str, 64, "%lld", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const unsigned long long int val) {
	char str[64];
	snprintf(str, 64, "%llu", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const float val) {
	char str[64];
	snprintf(str, 64, "%.3f", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (const double val) {
	char str[64];
	snprintf(str, 64, "%.3f", val);
	return (*this) << (const char*)(str);
}

GUI &GUI::operator << (Font *font) {
	this->currentFont = font;
	return (*this);
}

GUI &GUI::operator << (const Color color) {
	this->currentColor = color;
	return (*this);
}

GUI &GUI::operator << (const Vectori newPosition) {
	this->cursorPosition = newPosition;
	this->newLinePosition = newPosition;
	return (*this);
}

GUI &GUI::operator << (const Rectanglef newWorkspace) {
	this->cursorPosition = Vectori(0,0);
	this->newLinePosition = Vectori(0,this->currentFont->getDimension(L" ").Height);
	Vectori windowSize = this->GetWindowSize();
	this->workSpace = Rectanglei(windowSize.X *newWorkspace.UpperLeftCorner.X, windowSize.Y *newWorkspace.UpperLeftCorner.Y, windowSize.X *newWorkspace.LowerRightCorner.X, windowSize.Y *newWorkspace.LowerRightCorner.Y);
	return (*this);
}

GUI &GUI::DrawTexture(Texture *texture, Rectanglei source, Rectanglei destiny, Color color) {
	this->toDraw.insert(this->toDraw.end(), GUIDrawEvent(texture, source, destiny, color));
	return (*this);
}

GUI::GUI() {
	this->defaultFont = NULL;
	this->currentFont = NULL;
	this->window = NULL;
}

GUI::~GUI() {
	this->defaultFont = NULL;
	this->currentFont = NULL;
	this->window = NULL;
}

#endif
