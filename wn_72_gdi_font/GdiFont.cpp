﻿
#pragma comment(lib, "Dwrite.lib")
#pragma warning(disable:4005)

#include "GdiFont.h"
#include <map>
#include <string>
#include <Dwrite.h>


static std::map<std::string, LOGFONTA*>		g_gdi_font;

GdiFont::GdiFont()
	: m_font(NULL)
, m_height(0)
{
}

GdiFont::~GdiFont()
{
}

GdiFont* GdiFont::createWithPixel(const char* font_file, float font_pixel_height)
{
	HFONT font = GdiFont::createHFont(font_pixel_height, font_file);
	if(!font)
		return NULL;
	GdiFont* ret = new GdiFont;
	ret->m_height = font_pixel_height;
	ret->m_font = font;
	return ret;
}

GdiFont* GdiFont::createWithPoint(const char* font_file, float font_point_height)
{
	font_point_height = LONG(font_point_height * 96.0/72.0); //MulDiv(floatHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72)
	return GdiFont::createWithPixel(font_file, font_point_height);
}

void GdiFont::drawText(HDC hdc, const char* msg, int pos_x, int pos_y, UINT color_txt)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, m_font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color_txt);
	TextOut(hdc, pos_x, pos_y, msg, strlen(msg));
	SelectObject(hdc, oldFont);
}

void GdiFont::drawTextWithShadow(HDC hdc, const char* msg, int pos_x, int pos_y, UINT color_txt, UINT color_shadow)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, m_font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color_shadow);
	TextOut(hdc, pos_x+1, pos_y+1, msg, strlen(msg));
	SetTextColor(hdc, color_txt);
	TextOut(hdc, pos_x, pos_y, msg, strlen(msg));
	SelectObject(hdc, oldFont);
}

void GdiFont::drawTextWithOutline(HDC hdc, const char* msg, int pos_x, int pos_y, UINT color_txt, UINT color_shadow)
{
	HFONT oldFont = (HFONT)SelectObject(hdc, m_font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color_shadow);
	for(int y=0; y<3; ++y)
	{
		for(int x=0; x<3; ++x)
		{
			TextOut(hdc, pos_x+x-1, pos_y+y-1, msg, strlen(msg));
		}
	}
	SetTextColor(hdc, color_txt);
	TextOut(hdc, pos_x, pos_y, msg, strlen(msg));
	SelectObject(hdc, oldFont);
}

int GdiFont::createLogFont(const char* fontFileName, LOGFONTA* inOutFont)
{
	int hr=0;

	// find
	std::map<std::string, LOGFONTA*>::iterator find_it = g_gdi_font.find(std::string(fontFileName));
	if(find_it != g_gdi_font.end())
	{
		LOGFONTA* lf = find_it->second;
		memcpy(inOutFont, lf, sizeof(LOGFONTA));
		return 0;
	}

	// load from file
	int wcs_len =
	MultiByteToWideChar(CP_ACP, 0, fontFileName, -1, NULL, NULL);
	std::wstring ret_wstr(wcs_len-1, 0);
	MultiByteToWideChar(CP_ACP, 0, fontFileName, -1, &ret_wstr[0], wcs_len);

	IDWriteFactory* dwriteFactory = NULL;
	IDWriteFontFile* fontFile = NULL;
	IDWriteFontFace* fontFace = NULL;
	IDWriteGdiInterop* gdiInterop = NULL;
	LOGFONTA* pLogFont = NULL;
	do
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)(&dwriteFactory));
		if(FAILED(hr))
		{
			std::string err = "GdiFont::createLogFont error:: DWriteCreateFactory failed";
			OutputDebugStringA(err.c_str());
			break;
		}

		hr = dwriteFactory->GetGdiInterop(&gdiInterop);
		if(FAILED(hr))
		{
			std::string err = "GdiFont::createLogFont error:: GetGdiInterop failed";
			OutputDebugStringA(err.c_str());
			break;
		}

		// Open the file and determine the font type.
		hr = dwriteFactory->CreateFontFileReference(ret_wstr.c_str(), nullptr, &fontFile);
		if(FAILED(hr))
		{
			std::string err = "GdiFont::createLogFont error:: CreateFontFileReference failed";
			OutputDebugStringA(err.c_str());
			break;
		}

		BOOL isSupportedFontType = false;
		DWRITE_FONT_FILE_TYPE fontFileType;
		DWRITE_FONT_FACE_TYPE fontFaceType;
		UINT32 numberOfFaces = 0;
		hr= fontFile->Analyze(&isSupportedFontType, &fontFileType, &fontFaceType, &numberOfFaces);
		if(FAILED(hr))
		{
			std::string err = "GdiFont::createLogFont error:: Analyze failed";
			OutputDebugStringA(err.c_str());
			break;
		}

		if(!isSupportedFontType)
		{
			std::string err = "GdiFont::createLogFont error:: OS is not supported font type";
			OutputDebugStringA(err.c_str());
			hr = DWRITE_E_FILEFORMAT;
			break;
		}

		// Set up a font face from the array of font files (just one)
		IDWriteFontFile* fontFileArray[] ={fontFile};
		hr = dwriteFactory->CreateFontFace(
											fontFaceType,
											ARRAYSIZE(fontFileArray),		// file count
											&fontFileArray[0],				// or GetAddressOf if WRL ComPtr
											0,								// faceIndex
											DWRITE_FONT_SIMULATIONS_NONE,
											&fontFace);
		if(FAILED(hr))
		{
			std::string err = "GdiFont::createLogFont error:: CreateFontFace failed";
			OutputDebugStringA(err.c_str());
			break;
		}

		// Get the necessary logical font information.
		LOGFONTW wlogFont;
		pLogFont = new LOGFONTA;
		hr = gdiInterop->ConvertFontFaceToLOGFONT(fontFace, &wlogFont);
		if(FAILED(hr))
		{
			std::string err = "GdiFont::createLogFont error:: ConvertFontFaceToLOGFONT failed";
			OutputDebugStringA(err.c_str());
			break;
		}

		int str_len =
		WideCharToMultiByte(CP_ACP, 0, wlogFont.lfFaceName, -1, NULL, 0, NULL, NULL);
		std::string strFaceName(str_len-1, 0);
		WideCharToMultiByte(CP_ACP, 0, wlogFont.lfFaceName, -1, &strFaceName[0], str_len, NULL, NULL);
		pLogFont->lfHeight         = wlogFont.lfHeight        ;
		pLogFont->lfWidth          = wlogFont.lfWidth         ;
		pLogFont->lfEscapement     = wlogFont.lfEscapement    ;
		pLogFont->lfOrientation    = wlogFont.lfOrientation   ;
		pLogFont->lfWeight         = wlogFont.lfWeight        ;
		pLogFont->lfItalic         = wlogFont.lfItalic        ;
		pLogFont->lfUnderline      = wlogFont.lfUnderline     ;
		pLogFont->lfStrikeOut      = wlogFont.lfStrikeOut     ;
		pLogFont->lfCharSet        = wlogFont.lfCharSet       ;
		pLogFont->lfOutPrecision   = wlogFont.lfOutPrecision  ;
		pLogFont->lfClipPrecision  = wlogFont.lfClipPrecision ;
		pLogFont->lfQuality        = wlogFont.lfQuality       ;
		pLogFont->lfPitchAndFamily = wlogFont.lfPitchAndFamily;
		strcpy_s(pLogFont->lfFaceName, _countof(pLogFont->lfFaceName), strFaceName.c_str());
		fontFace->Release();
		fontFile->Release();
		gdiInterop->Release();
		dwriteFactory->Release();
	} while(0);

	if(0>hr)
		return hr;

	hr = AddFontResourceExA(fontFileName, FR_PRIVATE, 0);
	if(0>hr)
	{
		std::string err = "GdiFont::createLogFont error:: AddFontResourceExA failed";
		OutputDebugStringA(err.c_str());
		return -1;
	}
	memcpy(inOutFont, pLogFont, sizeof(LOGFONTA));
	g_gdi_font.insert(std::pair<std::string, LOGFONTA*>(std::string(fontFileName), pLogFont));
	return hr;
}

HFONT GdiFont::createHFont(float floatHeight, const char* fontFileName)
{
	HFONT ret_font = 0;
	LOGFONT fontLog ={0};
	if(0> GdiFont::createLogFont(fontFileName, &fontLog))
		return NULL;

	fontLog.lfHeight         = LONG(floatHeight+0.01F);
	fontLog.lfCharSet        = DEFAULT_CHARSET;
	fontLog.lfOutPrecision   = OUT_TT_ONLY_PRECIS;
	fontLog.lfQuality        = ANTIALIASED_QUALITY;
	fontLog.lfPitchAndFamily = FF_DONTCARE;
	strcpy(fontLog.lfFaceName, fontLog.lfFaceName);
	ret_font = CreateFontIndirect(&fontLog);
	return ret_font;
}

int GdiFont::removeAllHFontResource()
{
	if(g_gdi_font.empty())
		return 0;

	for(std::map<std::string, LOGFONTA*>::iterator it = g_gdi_font.begin(); it != g_gdi_font.end(); ++it)
	{
		const std::string& fontFileName = it->first;
		LOGFONTA* pLogFont = it->second;
		int hr = ::RemoveFontResourceExA(fontFileName.c_str(), FR_PRIVATE, 0);
		if(0>hr)
		{
			// ERR::
			std::string err = "RemoveFontResourceExA error:: font:: ";
			err += fontFileName;
			OutputDebugStringA(err.c_str());
		}
		delete pLogFont;
	}
	g_gdi_font.clear();
	return 0;
}
