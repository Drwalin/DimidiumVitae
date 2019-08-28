
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef TEXT_PRINTER_CPP
#define TEXT_PRINTER_CPP

#include <TextPrinter.h>
#include <BasicWindow.h>
#include <Debug.h>

Color::Color()
{
	r = 0;
	g = 0;
	b = 0;
	a = 255;
}

Color::Color( unsigned char r, unsigned char g, unsigned char b )
{
	this->r = r;
	this->g = g;
	this->b = b;
	a = 255;
}

Color::Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void TextPrinter::Flush()
{
	
	
}

void TextPrinter::Init()
{
	
	
}

void TextPrinter::AddCharacter( const float x, const float y, const unsigned char sign )
{
	struct Temp__d
	{
		float pos[2];
		float tex[2];
		unsigned char color[4];
	} temp;
	
	//vbo->SetVertexSize( sizeof(Temp__d) );
	
	
	*((Color*)&temp.color) = currentTextColor;
	//al_unmap_rgba( currentTextColor, temp.color, temp.color+1, temp.color+2, temp.color+3 );
	
	
	float a, b;
	a = float(sign & 31) / 32.0f;
	b = float(sign >> 5) / 8.0f;
	
	temp.pos[0] = x;
	temp.pos[1] = y;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a;
	temp.tex[1] = b;
	
	//vbo->AddVertex( &temp );
	
	
	temp.pos[0] = x+1;
	temp.pos[1] = y;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a+(1.0f/32.0f);
	temp.tex[1] = b;
	
	//vbo->AddVertex( &temp );
	
	
	temp.pos[0] = x;
	temp.pos[1] = y+1;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a;
	temp.tex[1] = b+(1.0f/8.0f);
	
	//vbo->AddVertex( &temp );
	
	
	
	temp.pos[0] = x+1;
	temp.pos[1] = y;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a+(1.0f/32.0f);
	temp.tex[1] = b;
	
	//vbo->AddVertex( &temp );
	
	
	temp.pos[0] = x;
	temp.pos[1] = y+1;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a;
	temp.tex[1] = b+(1.0f/8.0f);
	
	//vbo->AddVertex( &temp );
	
	temp.pos[0] = x+1;
	temp.pos[1] = y+1;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a+(1.0f/32.0f);
	temp.tex[1] = b+(1.0f/8.0f);
	
	//vbo->AddVertex( &temp );
}

void TextPrinter::SetBasicWindow( BasicWindow * basicWindow )
{
	this->basicWindow = basicWindow;
}

int TextPrinter::GetMaxBorderWidth()
{
	return 80;
}

int TextPrinter::GetMaxBorderHeight()
{
	return 25;
}

void TextPrinter::CorrectCursorPosition()
{
	if( currentCursorX < leftWorkspaceBorder )
	{
		currentCursorX = leftWorkspaceBorder;
	}
	
	if( currentCursorY < upWorkspaceBorder )
	{
		currentCursorY = upWorkspaceBorder;
	}
	
	if( currentCursorX > rightWorkspaceBorder )
	{
		currentCursorX = leftWorkspaceBorder;
		++currentCursorY;
	}
	
	if( currentCursorY > downWorkspaceBorder )
	{
		currentCursorY = downWorkspaceBorder;
	}
}

void TextPrinter::CorrectWorkspaceBorder()
{
	if( leftWorkspaceBorder < 0 )
	{
		leftWorkspaceBorder = 0;
	}
	
	if( upWorkspaceBorder < 0 )
	{
		upWorkspaceBorder = 0;
	}
	
	if( leftWorkspaceBorder > GetMaxBorderWidth()-2 )
	{
		leftWorkspaceBorder = GetMaxBorderWidth()-2;
	}
	
	if( upWorkspaceBorder > GetMaxBorderHeight()-2 )
	{
		upWorkspaceBorder = GetMaxBorderHeight()-2;
	}
	
	if( rightWorkspaceBorder < leftWorkspaceBorder+2 )
	{
		rightWorkspaceBorder = leftWorkspaceBorder+2;
	}
	
	if( downWorkspaceBorder < upWorkspaceBorder+2 )
	{
		downWorkspaceBorder = upWorkspaceBorder+2;
	}
}

void TextPrinter::SetColor( Color color )
{
	currentTextColor = color;
}

void TextPrinter::Print( const std::string & str )
{
	Print( str.c_str() );
}

void TextPrinter::Print( const char * str )
{
	char * ptr = (char*)str;
	char sign[2];
	sign[1] = 0;
	while( *ptr )
	{
		switch( *ptr )
		{
		case '\t':
			currentCursorX = ( ( ( ( currentCursorX - leftWorkspaceBorder ) / 4 ) + 1 ) * 4 ) + leftWorkspaceBorder;
			break;
			
		case '\n':
			++currentCursorY;
			currentCursorX = 0;
			break;
			
		default:
			AddCharacter( currentCursorX, currentCursorY, *ptr );
			++currentCursorX;
		}
		
		++ptr;
		CorrectCursorPosition();
	}
}

void TextPrinter::Print( const char val )
{
	char str[64];
	sprintf( str, "%c", val );
	Print( str );
}

void TextPrinter::Print( const unsigned char val )
{
	char str[64];
	sprintf( str, "%c", val );
	Print( str );
}

void TextPrinter::Print( const short val )
{
	char str[64];
	sprintf( str, "%i", (int)val );
	Print( str );
}

void TextPrinter::Print( const unsigned short val )
{
	char str[64];
	sprintf( str, "%u", (unsigned int)val );
	Print( str );
}

void TextPrinter::Print( const int val )
{
	char str[64];
	sprintf( str, "%i", val );
	Print( str );
}

void TextPrinter::Print( const unsigned int val )
{
	char str[64];
	sprintf( str, "%u", val );
	Print( str );
}

void TextPrinter::Print( const long long int val )
{
	char str[64];
	sprintf( str, "%lli", val );
	Print( str );
}

void TextPrinter::Print( const unsigned long long int val )
{
	char str[64];
	sprintf( str, "%llu", val );
	Print( str );
}

void TextPrinter::Print( const float val )
{
	char str[64];
	sprintf( str, "%f", val );
	Print( str );
}

void TextPrinter::Print( const double val )
{
	Print( (float)val );
}

void TextPrinter::Goto( int x, int y )
{
	currentCursorX = x;
	currentCursorY = y;
	CorrectCursorPosition();
}

void TextPrinter::SetWorkSpace( int lx, int ly, int hx, int hy )
{
	leftWorkspaceBorder = lx;
	rightWorkspaceBorder = hx;
	upWorkspaceBorder = ly;
	downWorkspaceBorder = hy;
	CorrectWorkspaceBorder();
	CorrectCursorPosition();
}

void TextPrinter::ClearWorkspaceBorders()
{
	leftWorkspaceBorder = 0;
	upWorkspaceBorder = 0;
	rightWorkspaceBorder = GetMaxBorderWidth();
	downWorkspaceBorder = GetMaxBorderHeight();
}


TextPrinter::TextPrinter()
{
	windowX = 100.0f;
	windowY = 75.0f;
	basicWindow = NULL;
	currentTextColor = Color( 255, 255, 255 );
	currentCursorX = 0;
	currentCursorY = 0;
	leftWorkspaceBorder = 0;
	rightWorkspaceBorder = 100;
	upWorkspaceBorder = 0;
	downWorkspaceBorder = 100;
}

TextPrinter::~TextPrinter()
{
	basicWindow = NULL;
	currentTextColor = Color( 255, 255, 255 );
	currentCursorX = 0;
	currentCursorY = 0;
	leftWorkspaceBorder = 0;
	rightWorkspaceBorder = 0;
	upWorkspaceBorder = 0;
	downWorkspaceBorder = 0;
}

#endif

