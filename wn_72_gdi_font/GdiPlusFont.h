
#ifdef _MSC_VER
#pragma once
#endif

#ifndef _GdiPlusFont_H_
#define _GdiPlusFont_H_

#include <string>
#include <windows.h>

#ifndef RGBA
  #define RGBA(a,r,g,b) ( ((UINT)(r)<< 0) & 0x000000FF | \
						  ((UINT)(g)<< 8) & 0x0000FF00 | \
						  ((UINT)(b)<<16) & 0x00FF0000 | \
						  ((UINT)(a)<<24) & 0x00000000 )
#endif
#ifndef ARGB
  #define ARGB(a,r,g,b) ( ((UINT)(b)<< 0) & 0x000000FF | \
						  ((UINT)(g)<< 8) & 0x0000FF00 | \
						  ((UINT)(r)<<16) & 0x00FF0000 | \
						  ((UINT)(a)<<24) & 0xFF000000 )
#endif
#ifndef XRGB
  #define XRGB(r,g,b)   ( ((UINT)(b)<< 0) & 0x000000FF | \
						  ((UINT)(g)<< 8) & 0x0000FF00 | \
						  ((UINT)(r)<<16) & 0x00FF0000 | 0xFF000000 )
#endif

class GdiPlus
{
protected:
	unsigned long long m_token;
	bool m_bSuccess;

public:
	static GdiPlus* getInstance();

	GdiPlus();
	~GdiPlus();
	bool isSuccess() const { return m_bSuccess; }

	static void beginGraphics(HDC hdc);
	static void endGraphics();
	static void clearColor(UINT color=ARGB(0xFF, 0xFF, 0xFF, 0xFF));
};

class GdiPlusFont
{
public:
	GdiPlusFont();
	virtual ~GdiPlusFont();

	static int createFamily(const char* fontFileName);
	static int removeAllFontFamily();

	static int drawPath(std::string msg, float pos_x, float pos_y, const char* fontFileName, float font_em_size, UINT color=XRGB(0, 0, 0));
	static int drawString(std::string msg, float pos_x, float pos_y, const char* fontFileName, float font_em_size, UINT color=XRGB(0, 0, 0));

	static GdiPlusFont* create(const char* font_file, float font_pixel);

	void	drawText(const char* msg, float pos_x, float pos_y, UINT color=0xFFFFFFFF);
	void	drawPath(const char* msg, float pos_x, float pos_y, UINT color=0xFFFFFFFF);
	float	height() const { return m_height; }
protected:
	float		m_height;
	std::string	m_file;
};

#endif
