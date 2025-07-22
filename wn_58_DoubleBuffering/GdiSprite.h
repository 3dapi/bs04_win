#pragma once
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

class GdiSprite
{
protected:
	HDC					m_memDC;
	HBITMAP				m_bmpStored;
public:
	SIZE				m_size;
public:
	int  init(const char* fileName);
	void destroy();
	void Draw(HDC hdc);
	void Draw(HDC hdc, const POINT& destPosition, float scale=1.0F, uint8_t alpha= 255);
	void Draw(HDC hdc, const POINT& srcPosition, const SIZE& srcSize, const POINT& destPosition, float scale, uint8_t alpha= 255);
};

