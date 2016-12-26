
#include "ImageBufferObjects.h"


Base_ImageBuffer_Object::Base_ImageBuffer_Object()
{
	m_hHandle = 0;
}

Base_ImageBuffer_Object::~Base_ImageBuffer_Object()
{
	Clear();
}

const HGDIOBJ Base_ImageBuffer_Object::GetNativeObject() const
{
	return m_hHandle;
}

void Base_ImageBuffer_Object::Clear()
{
	if (m_hHandle)
	{
		DeleteObject(m_hHandle);
		m_hHandle = 0;
	}
}

IB_Font::IB_Font() : Base_ImageBuffer_Object()
{
	//
}

IB_Font::~IB_Font()
{
	Clear();
}

IB_Font::IB_Font(ImageBuffer_cstring FontName, int size)
{
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfHeight = size;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = DEFAULT_QUALITY;
	if (FontName)
	{
		int FontNameLen = _tcslen(FontName);
		int FaceNameLen = sizeof(lf.lfFaceName);
		for (int i = 0; i < sizeof(lf.lfFaceName) && i < FontNameLen; i++) 
			lf.lfFaceName[i] = FontName[i];
	}
	m_hHandle = CreateFontIndirect(&lf);
}

void IB_Font::Reset(ImageBuffer_cstring FontName, int size)
{
	Clear();
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfHeight = size;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = DEFAULT_QUALITY;
	if (FontName)
	{
		int FontNameLen = _tcslen(FontName);
		int FaceNameLen = sizeof(lf.lfFaceName);
		for (int i = 0; i < sizeof(lf.lfFaceName) && i < FontNameLen; i++) 
			lf.lfFaceName[i] = FontName[i];
	}
	m_hHandle = CreateFontIndirect(&lf);
}


IB_Pen::IB_Pen() : Base_ImageBuffer_Object()
{
	//
}

IB_Pen::IB_Pen(int fnPenStyle, int nWidth, ARGBColor crColor)
{
	m_hHandle = (HPEN)CreatePen(fnPenStyle, nWidth, (COLORREF)crColor);
}

IB_Pen::~IB_Pen()
{
	Clear();
}

void IB_Pen::Reset(int fnPenStyle, int nWidth, ARGBColor crColor)
{
	Clear();
	m_hHandle = (HPEN)CreatePen(fnPenStyle, nWidth, (COLORREF)crColor);
}

IB_Brush::IB_Brush() : Base_ImageBuffer_Object()
{
	//
}

IB_Brush::IB_Brush(ARGBColor crColor)
{
	m_hHandle = (HPEN)CreateSolidBrush((COLORREF)crColor);
}

IB_Brush::~IB_Brush()
{
	Clear();
}

void IB_Brush::Reset(ARGBColor crColor)
{
	Clear();
	m_hHandle = (HPEN)CreateSolidBrush((COLORREF)crColor);
}