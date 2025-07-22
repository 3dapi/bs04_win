#include "GdiSprite.h"

int GdiSprite::init(const char* fileName)
{
	auto hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	HBITMAP hBitmap = (HBITMAP)LoadImageA(hInstance, "res/mario_sprite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if(!hBitmap)
	{
		return E_FAIL;
	}

	// 1. 비트맵 정보를 가져오기
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);
	m_size.cx = bmp.bmWidth;
	m_size.cy = bmp.bmHeight;

	// 2. 메모리 DC 생성
	m_memDC = CreateCompatibleDC(nullptr);
	if(!m_memDC)
		return E_FAIL;

	// 3. 비트맵을 DC에 선택
	m_bmpStored = (HBITMAP)SelectObject(m_memDC, hBitmap);
	return S_OK;
}

void GdiSprite::destroy()
{
	SelectObject(m_memDC, m_bmpStored);
	DeleteDC(m_memDC);
}

void GdiSprite::Draw(HDC hdc)
{
	GdiSprite::Draw(hdc, POINT{0, 0}, m_size, POINT{0, 0}, 1.0F, 255);
}

void GdiSprite::Draw(HDC hdc, const POINT& destPosition, float scale, uint8_t alpha)
{
	GdiSprite::Draw(hdc, destPosition, m_size, POINT{0, 0}, scale, alpha);
}

void GdiSprite::Draw(HDC hdc, const POINT& srcPosition, const SIZE& srcSize, const POINT& destPosition, float scale, uint8_t alpha)
{
	if(255 == alpha)
	{
		GdiTransparentBlt(hdc
						, destPosition.x, destPosition.y
						, int(srcSize.cx * scale), int(srcSize.cy * scale)
						, m_memDC
						, srcPosition.x, srcPosition.y, srcSize.cx, srcSize.cy, RGB(255, 0, 255));
	}
	else
	{
		BLENDFUNCTION bf;
		bf.AlphaFormat = 0;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = alpha;
		GdiAlphaBlend(hdc
						, destPosition.x, destPosition.y
						, int(srcSize.cx * scale), int(srcSize.cy * scale)
						, m_memDC
						, srcPosition.x, srcPosition.y, srcSize.cx, srcSize.cy, bf);
	}
	
}
