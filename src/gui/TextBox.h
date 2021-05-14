
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <irrlicht/irrlicht.h>

#include "GUI.h"

#include <vector>
#include <string>
#include <cinttypes>

namespace GUI {
	
	struct Subline {
		std::vector<int> collors;
		int length;
	};
	
	struct Margin {
		int top, left, right, bottom;
	};
	
	class Line {
	public:
		Line() = default;
		~Line() = default;
		
		void UpdateSublines(class TextBox* textBox, bool resizedWorkspace);
		int GetSublinesCount();
		
		void PutChar(char c, Color color, int pos,
				bool replacingNotInserting=false);
		
		void Draw(class TextBox* textBox, Font* font, int pixelX, int pixelY,
				int sublineOffset);
		
		int Length();
		Line SplitAndReturnLatter(int pos);
		void AppendLine(const Line& line);
		void EraseAt(int pos);
		
	private:
		
		std::vector<Subline> sublines;
		
		std::vector<char> str;
		std::vector<Color> colors;
		bool modified;
	};
	
	class TextBox {
	public:
		TextBox() = default;
		~TextBox() = default;
		
		void SetRelativeSize(const Rectanglef& relativeSize);
		Rectanglef GetRelativeSize() const;
		void SetMargin(Margin margin);
		Margin GetMargin() const;
		
		void Clear();
		void Flush();
		void UpdateWorkspace();
		Rectanglei CalcWorkspaceSizeCharacters();
		int GetSublineOffset() const;
		
		int GetLinesCount() const;
		int GetSublinesCount();
		void SetLinesOffset(int offset);
		
		void PutChar(char c);
		
		TextBox &operator << (const char *str);
		TextBox &operator << (const std::string &str);
		TextBox &operator << (const bool val);
		TextBox &operator << (const char val);
		TextBox &operator << (const unsigned char val);
		TextBox &operator << (const short val);
		TextBox &operator << (const unsigned short val);
		TextBox &operator << (const int val);
		TextBox &operator << (const unsigned int val);
		TextBox &operator << (const long long int val);
		TextBox &operator << (const unsigned long long int val);
		TextBox &operator << (const float val);
		TextBox &operator << (const double val);
		
		TextBox &operator << (Font *font);
		TextBox &operator << (const Color color);
		
		void SetColor(const Color color);
		void SetFont(Font* font);
		void Goto(int line, int column);
		void SetDrawCursor(bool drawCursor);
		
	private:
		void PrintNewLine();
		void PrintTab();
		void PrintOneBufferLineImplicitly(char *str, int width, int height);
		void PrintOneLine(char *str, int length, int width, int height);
		void PrintOneText(char *str, int length);
		
	private:
		
		int linesOffset;
		struct {int x, y; bool draw;} cursor;
		
		Font* font;
		Color currentColor;
		
		Rectanglef relativeSize;
		Margin margin;
		Vectori currentWorkspaceSizeCharacters;
		Vectori currentWorkspaceSizePixels;
		
		int breakLineCharactersOffset;
		std::vector<Line> lines;
	};
}

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

