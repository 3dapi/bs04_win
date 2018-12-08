
#ifdef _MSC_VER
#pragma once
#endif

#ifndef _GdiFont_H_
#define _GdiFont_H_

#include <windows.h>

class GdiFont
{
public:
	GdiFont();
	virtual ~GdiFont();

	static int   createLogFont(const char* fontFileName, LOGFONTA* logFont);
	static HFONT createHFont(float floatHeight, const char* fontFileName);
	static int   removeAllHFontResource();

	static GdiFont* createWithPixel(const char* font_file, float font_pixel_height);
	static GdiFont* createWithPoint(const char* font_file, float font_point_height);

	void	drawText(HDC hdc, const char* msg, int x, int y, UINT color=0xFFFFFF);
	void	drawTextWithShadow(HDC hdc, const char* msg, int x, int y, UINT color=0xFFFFFF, UINT color_shadow=0x000000);
	void	drawTextWithOutline(HDC hdc, const char* msg, int x, int y, UINT color=0xFFFFFF, UINT color_shadow=0x000000);
	HFONT	font() const { return m_font; }
	float	height() const { return m_height; }
protected:
	HFONT	m_font;
	float	m_height;
};

#endif
