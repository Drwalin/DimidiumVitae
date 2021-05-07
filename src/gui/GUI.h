
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef GUI_H
#define GUI_H

#include <memory>
#include <string>
#include <vector>

#include <irrlicht/irrlicht.h>

#include <resources/Texture.h>

typedef irr::video::SColor Color;
typedef irr::core::rect<irr::s32> Rectanglei;
typedef irr::core::rect<irr::f32> Rectanglef;
typedef irr::core::vector2d<irr::s32> Vectori;
typedef irr::core::vector2d<irr::f32> Vectorf;
typedef irr::gui::IGUIFont Font;

class GUIDrawEvent {
public:
	enum Type
	{
		NONE,
		TEXT,
		IMAGE
	};
	Type type;
	struct Text
	{
		Font *font;
		Rectanglei destiny;
		char str[64];
		Color color;
	};
	struct Image
	{
		Rectanglei source;
		Rectanglei destiny;
		Color color;
	};
	std::shared_ptr<Texture> texture;
	union
	{
		Text text;
		Image image;
	};
	GUIDrawEvent &operator=(const GUIDrawEvent &other);
	void Draw();
	GUIDrawEvent(Font *font, Rectanglei destiny, Color color, char *str);
	GUIDrawEvent(std::shared_ptr<Texture> texture, Rectanglei source,
			Rectanglei destiny, Color color);
	GUIDrawEvent();
	GUIDrawEvent(const GUIDrawEvent &other);
};

class GUI {
public:
	
	GUI();
	~GUI();
	
	Font *GetDefaultFont();
	
	void Flush();
	void Init();
	
	Vectori GetWindowSize() const;
	Vectori GetWorkspaceSize() const;
	Vectori GetWorkspacePosition() const;
	
	GUI &operator << (const char *str);
	GUI &operator << (const std::string &str);
	GUI &operator << (const bool val);
	GUI &operator << (const char val);
	GUI &operator << (const unsigned char val);
	GUI &operator << (const short val);
	GUI &operator << (const unsigned short val);
	GUI &operator << (const int val);
	GUI &operator << (const unsigned int val);
	GUI &operator << (const long long int val);
	GUI &operator << (const unsigned long long int val);
	GUI &operator << (const float val);
	GUI &operator << (const double val);
	
	GUI &operator << (Font *font);
	GUI &operator << (const Color color);
	GUI &operator << (const Vectori newPosition);
	GUI &operator << (const Rectanglef newWorkspace);
	
	GUI &DrawTexture(std::shared_ptr<Texture> texture, Rectanglei source,
			Rectanglei destiny, Color color);
	
private:
	
	void PrintNewLine();
	void PrintTab();
	void PrintOneBufferLineImplicitly(char *str, int width, int height);
	void PrintOneLine(char *str, int length, int width, int height);
	void PrintOneText(char *str, int length);
	
private:
	
	Font *defaultFont;
	
	Vectori newLinePosition;
	Font *currentFont;
	Color currentColor;
	Vectori cursorPosition;
	Rectanglei workSpace;
	
	std::vector<GUIDrawEvent> toDraw;
};

#endif

