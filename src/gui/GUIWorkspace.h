
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef GUI_H
#define GUI_H

#include <memory>
#include <string>
#include <vector>

#include <irrlicht/irrlicht.h>

#include "GUI.h"

#include <vector>
#include <string>

class GUIWorkspace {
public:
	
	class Line {
	public:
		Line();
		~Line();
		
		void PutChar(char c);
		void PutString(const std::string& str);
		void SetColor(Color color);
		void SetAllToColor(Color color);
		void Clear();
		
		void SetLineWidth();
		int GetHeightChars() const;
		
		void Flush(Font* font, );
		
	private:
		int lineWidth;
		Color currentColor;
		
		std::vector<int> subLines;
		std::vector<char> text;
		std::vector<Color> color;
	};
	
public:
	
	GUIWorkspace();
	~GUIWorkspace();
	
	
	
	void SetWorkspace(Rectanglef destiny) {
		
	}
	
private:
	
	
	
	
	Rectanglef space;
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
	
	GUI &operator << (const GUIWorkSpace& workspace);
	GUI &operator << (const GUIWorkSpace* workspace);
	
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

