
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXT_PRINTER_H
#define TEXT_PRINTER_H

#include <string>

#include <cstdio>

struct Color
{
	unsigned char r, g, b, a;
	Color();
	Color( unsigned char r, unsigned char g, unsigned char b );
	Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a );
};

class TextPrinter
{
private:
	
	float windowX, windowY;
	
	class BasicWindow * basicWindow;
	
	Color currentTextColor;
	int currentCursorX, currentCursorY;
	int leftWorkspaceBorder, rightWorkspaceBorder, upWorkspaceBorder, downWorkspaceBorder;
	
	void CorrectCursorPosition();
	void CorrectWorkspaceBorder();
	
	void AddCharacter( const float x, const float y, const unsigned char sign );
	
public:
	
	void Flush();
	void Init();
	
	void SetBasicWindow( BasicWindow * basicWindow );
	
	void SetColor( Color color );
	int GetMaxBorderWidth();
	int GetMaxBorderHeight();
	void Print( const std::string & str );
	void Print( const char * str );
	void Print( const char val );
	void Print( const unsigned char val );
	void Print( const short val );
	void Print( const unsigned short val );
	void Print( const int val );
	void Print( const unsigned int val );
	void Print( const long long int val );
	void Print( const unsigned long long int val );
	void Print( const float val );
	void Print( const double val );
	void Goto( int x, int y );								// in character space not pixel space
	void SetWorkSpace( int lx, int ly, int hx, int hy );		// in character space not pixel space
	void ClearWorkspaceBorders();
	
	TextPrinter();
	~TextPrinter();
};

#endif

