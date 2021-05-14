
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXT_BOX_CPP
#define TEXT_BOX_CPP

#include "TextBox.h"

#include <engine/Singleton.h>
#include <engine/Window.h>

#include <util/Debug.h>

#include <cstdio>
#include <cstring>

namespace GUI {
	
	void Line::UpdateSublines(class TextBox* textBox, bool resizedWorkspace) {
		int maxWidth = textBox->CalcWorkspaceSizeCharacters().getWidth();
		int sublineOffset = textBox->GetSublineOffset();
		sublines.clear();
		int i, j;
		if(modified || resizedWorkspace) {
			for(i=0, j=0;; ++i) {
				if((j-i)>=maxWidth || i>=str.size() || str[i]==0) {
					sublines.resize(sublines.size()+1);
					sublines.back().length = j-i;
					sublines.back().collors.emplace_back(0);
					for(int k=j; k<i; ++k) {
						if(sublines.back().collors.back() != colors[k])
							sublines.back().collors.emplace_back(k);
					}
					j=i;
				}
				if(i>=str.size())
					break;
			}
		}
		bool modified = false;
	}
	
	int Line::GetSublinesCount() {
		return std::max(1, sublines.size());
	}
	
	void Line::PutChar(char c, Color color, int pos,
			bool replacingNotInserting) {
		if(replacingNotInserting && pos<=str.size()) {
			str[pos] = c;
			colors[pos] = color;
		} else {
			modified = true;
			str.insert(str.begin()+pos, c);
			colors.insert(colors.begin()+pos, color);
		}
	}
	
	void Line::Draw(class TextBox* textBox, Font* font, int pixelX, int pixelY,
			int sublineOffset) {
		UpdateSublines(textBox);
		int w, h;
		w = sing::window->GetWidth();
		h = sing::window->GetHeight();
		int oy=pixelY;
		for(int i=0; i<sublines.size(); ++i) {
			int ox = pixelX;
			auto& sub = sublines[i];
			for(int j=0; j<sub.collors.size(); ++j) {
				irr::core::wstring str(this->str.data()+sub.collors[j],
						(j+1<sub.collors.size()) ?
							sub.collors[j+1]-sub.collors[j] :
							sub.length-sub.collors[j]
						);
				font->draw(str, {pixelX, pixelY, w, h}, colors[sub.collors[j]]);
				auto d = font->getDimension(str.c_str());
				ox = std::max(ox, pixelX+d.Width);
				oy = std::max(oy, pixelY+d.Height);
			}
			pixelY = oy;
			pixelX += sublineOffset;
		}
	}
	
	int Line::Length() {
		return str.size();
	}
	
	Line Line::SplitAndReturnLatter(int pos) {
		modified = true;
		Line ret;
		ret.modified = true;
		ret.str.insert(ret.str.begin(), str.begin()+pos, str.end());
		str.erase(str.begin()+pos, str.end());
		ret.colors.insert(ret.colors.begin(), colors.begin()+pos, colors.end());
		colors.erase(colors.begin()+pos, colors.end());
		return ret;
	}
	
	void Line::AppendLine(const Line& line) {
		modified = true;
		str.insert(str.end(), line.str.begin(), line.str.end());
		colors.insert(colors.end(), line.colors.begin(), line.colors.end());
	}
	
	void Line::EraseAt(int pos) {
		modified = true;
		str.erase(str.begin()+pos);
		colors.erase(colors.begin()+pos);
	}
	
	
	
	void TextBox::PutChar(char c) {
		if(c == '\n') {
			Line line = lines[cursor.y].Split(cursor.x);
			lines.insert(lines.begin()+cursor.y+1, line);
			cursor.x = 0;
			cursor.y++;
		} else if(c == '\t') {
			PutChar(' ');
			PutChar(' ');
			PutChar(' ');
			PutChar(' ');
		} else if(c == '\r')
			cursor.x = 0;
		else if(c == '\b') {
			if(cursor.x == 0) {
				if(cursor.y > 0) {
					cursor.x = lines[cursor.y-1].Length();
					lines[cursor.y-1].AppendLine(lines[cursor.y]);
					lines.erase(lines.begin()+cursor.y);
					cursor.y--;
				}
			} else {
				lines[cursor.y].EraseAt(cursor.x-1);
			}
		} else if(c == '\f')
			return;
		else if(c == '\v')
			return;
		else {
			lines[cursor.y].PutChar(c, color, cursor.x);
			cursor.x++;
		}
	}
	
	TextBox &TextBox::operator << (const char *str) {
		char *ptr = (char*)str;
		for(char *ptr=(char*)str; *ptr; ++ptr) {
			PutChar(*ptr);
		}
		return *this;
	}

	TextBox &TextBox::operator << (const std::string &str) {
		return (*this) << str.c_str();
	}

	TextBox &TextBox::operator << (const bool val) {
		char str[64];
		snprintf(str, 64, val ? "true" : "false");
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const char val) {
		char str[64];
		snprintf(str, 64, "%c", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const unsigned char val) {
		char str[64];
		snprintf(str, 64, "%c", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const short val) {
		char str[64];
		snprintf(str, 64, "%d", (int)val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const unsigned short val) {
		char str[64];
		snprintf(str, 64, "%u", (unsigned)val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const int val) {
		char str[64];
		snprintf(str, 64, "%d", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const unsigned int val) {
		char str[64];
		snprintf(str, 64, "%u", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const long long int val) {
		char str[64];
		snprintf(str, 64, "%lld", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const unsigned long long int val) {
		char str[64];
		snprintf(str, 64, "%llu", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const float val) {
		char str[64];
		snprintf(str, 64, "%.3f", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (const double val) {
		char str[64];
		snprintf(str, 64, "%.3f", val);
		return (*this) << (const char*)(str);
	}

	TextBox &TextBox::operator << (Font *font) {
		currentFont = font;
		return (*this);
	}

	TextBox &TextBox::operator << (const Color color) {
		currentColor = color;
		return (*this);
	}
	
}



GUIDrawEvent &GUIDrawEvent::operator=(const GUIDrawEvent &other) {
	type = other.type;
	switch(type) {
	case GUIDrawEvent::Type::TEXT:
		text = other.text;
		break;
	case GUIDrawEvent::Type::IMAGE:
		image = other.image;
		texture = other.texture;
		break;
	default:
		break;
	}
	return (*this);
}

void GUIDrawEvent::Draw() {
	switch(type) {
	case GUIDrawEvent::Type::TEXT:
		text.font->draw(text.str, text.destiny, text.color);
		break;
	case GUIDrawEvent::Type::IMAGE:
		sing::videoDriver->draw2DImage(texture->GetITexture(), image.destiny,
				image.source, NULL, &(image.color), true);
		break;
	default:
		break;
	}
}

GUIDrawEvent::GUIDrawEvent(Font *font, Rectanglei destiny, Color color,
		char *str) {
	type = GUIDrawEvent::Type::TEXT;
	text.font = font;
	snprintf(text.str, sizeof(text.str)-1, "%s", str);
	text.destiny = destiny;
	text.color = color;
}

GUIDrawEvent::GUIDrawEvent(std::shared_ptr<Texture> texture, Rectanglei source,
		Rectanglei destiny, Color color) {
	type = GUIDrawEvent::Type::IMAGE;
	this->texture = texture;
	image.source = source;
	image.destiny = destiny;
	image.color = color;
}

GUIDrawEvent::GUIDrawEvent() {
	type = GUIDrawEvent::Type::NONE;
}

GUIDrawEvent::GUIDrawEvent(const GUIDrawEvent &other) {
	(*this) = other;
}

Font *GUI::GetDefaultFont() {
	return defaultFont;
}

void GUI::Flush() {
	for(auto it=toDraw.begin(); it!=toDraw.end(); ++it)
		it->Draw();
	toDraw.clear();
}

void GUI::Init() {
	defaultFont = sing::igui->getFont("./media/Fonts/courier.bmp");
	currentFont = defaultFont;
	
	workSpace = Rectanglei(0, 0, GetWindowSize().X, GetWindowSize().Y);
	newLinePosition = Vectori(0,0);
	currentColor = Color(255,255,255,255);
	cursorPosition = Vectori(0,0);
}

Vectori GUI::GetWindowSize() const {
	Vectori ret;
	ret.X = sing::videoDriver->getScreenSize().Width;
	ret.Y = sing::videoDriver->getScreenSize().Height;
	return ret;
}

Vectori GUI::GetWorkspaceSize() const {
	Vectori ret;
	ret.X = workSpace.LowerRightCorner.X-workSpace.UpperLeftCorner.X;
	ret.Y = workSpace.LowerRightCorner.Y-workSpace.UpperLeftCorner.Y;
	return ret;
}

Vectori GUI::GetWorkspacePosition() const {
	Vectori ret;
	ret.X = workSpace.UpperLeftCorner.X;
	ret.Y = workSpace.UpperLeftCorner.Y;
	return ret;
}

void GUI::PrintNewLine() {
	newLinePosition.X = 0;
	cursorPosition = newLinePosition;
	newLinePosition.Y += currentFont->getDimension(L"l").Height;
}

void GUI::PrintTab() {
	int tabWidth = currentFont->getDimension(L"    ").Width;
	cursorPosition.X = (int(cursorPosition.X / tabWidth) *tabWidth) + tabWidth;
	if(cursorPosition.X >= GetWorkspaceSize().X)
		PrintNewLine();
}

void GUI::PrintOneBufferLineImplicitly(char *str, int width, int height) {
	Rectanglei rectangle(
			GetWorkspacePosition().X + cursorPosition.X,
			GetWorkspacePosition().Y + cursorPosition.Y,
			1,
			1
		);
	
	toDraw.insert(toDraw.end(), GUIDrawEvent(currentFont, rectangle,
			currentColor, str));
	cursorPosition.X += width;
	if(newLinePosition.Y < cursorPosition.Y + height)
		newLinePosition.Y = cursorPosition.Y + height;
}

void GUI::PrintOneLine(char *str, int length, int width, int height) {
	char buffer[64];
	memcpy(buffer, str, length);
	buffer[length] = 0;
	PrintOneBufferLineImplicitly(buffer, width, height);
}

void GUI::PrintOneText(char *str, int length) {
	if(length == 0)
		return;
	Vectori wokrSpaceSize = GetWorkspaceSize();
	while(*str) {
		int i=0;
		int height=0, width=0;
		for(i=0; i<length && i<63 && str[i]; ++i) {
			wchar_t oneCharwstring[2] = {(wchar_t)str[i],0};
			auto size = currentFont->getDimension(oneCharwstring);
			if((int)size.Width + width < wokrSpaceSize.X)
				width += size.Width;
			else
			{
				--i;
				break;
			}
			if(height < (int)size.Height)
				height = size.Height;
		}
		PrintOneLine(str, i, width, height);
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
			PrintOneText(ptr, i);
			ptr += i+1;
			i = -1;
			PrintNewLine();
		}
		else if(ptr[i] == '\t') {
			PrintOneText(ptr, i);
			ptr += i+1;
			i = -1;
			PrintTab();
		}
		else if(ptr[i] == 0) {
			PrintOneText(ptr, i);
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
	currentFont = font;
	return (*this);
}

GUI &GUI::operator << (const Color color) {
	currentColor = color;
	return (*this);
}

GUI &GUI::operator << (const Vectori newPosition) {
	cursorPosition = newPosition;
	newLinePosition = newPosition;
	return (*this);
}

GUI &GUI::operator << (const Rectanglef newWorkspace) {
	cursorPosition = Vectori(0,0);
	newLinePosition = Vectori(0,currentFont->getDimension(L" ").Height);
	Vectori windowSize = GetWindowSize();
	workSpace = Rectanglei(windowSize.X * newWorkspace.UpperLeftCorner.X,
			windowSize.Y * newWorkspace.UpperLeftCorner.Y,
			windowSize.X * newWorkspace.LowerRightCorner.X,
			windowSize.Y * newWorkspace.LowerRightCorner.Y);
	return (*this);
}

GUI &GUI::DrawTexture(std::shared_ptr<Texture> texture, Rectanglei source,
		Rectanglei destiny, Color color) {
	toDraw.insert(toDraw.end(), GUIDrawEvent(texture, source, destiny, color));
	return (*this);
}

GUI::GUI() {
	defaultFont = NULL;
	currentFont = NULL;
}

GUI::~GUI() {
	defaultFont = NULL;
	currentFont = NULL;
}

#endif
