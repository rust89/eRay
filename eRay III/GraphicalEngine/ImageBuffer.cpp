
#include "ImageBuffer.h"

ImageBuffer::ImageBuffer()
{
	m_hDc = 0;

	m_hNewBitmap = 0;
	m_hOldBitmap = 0;
}

ImageBuffer::~ImageBuffer()
{
	Clear();
}

bool ImageBuffer::Copy(const ImageBuffer& Copy)
{
	Clear();
	m_vWidth = Copy.m_vWidth;
	m_vHeight = Copy.m_vHeight;
	m_hDc = CreateCompatibleDC(Copy.m_hDc);
	if (!m_hDc) return false;
	m_hNewBitmap = CreateCompatibleBitmap(Copy.m_hDc,m_vWidth,m_vHeight); // создаЄм совместимую пам€ть
	if (!m_hNewBitmap) return false;
	if ((m_hOldBitmap = (HBITMAP)SelectObject(m_hDc,m_hNewBitmap)) == NULL) return false;
	if (!BitBlt(m_hDc,0,0,m_vWidth,m_vHeight,Copy.m_hDc,0,0,SRCCOPY)) return false;
	return true;
}

void ImageBuffer::Clear()
{
	if (m_hDc)
	{
		DeleteObject(m_hNewBitmap);
		m_hNewBitmap = 0;

		SelectObject(m_hDc,m_hOldBitmap);
		m_hOldBitmap = 0;

		DeleteDC(m_hDc);
		m_hDc = 0;
	}
	m_vWidth = 0;
	m_vHeight = 0;
}

HDC ImageBuffer::GetDC() const
{
	return m_hDc;
}

bool ImageBuffer::CreateCompatible(HDC hdc, long Width, long Height)
{
	Clear();
	m_vWidth = Width;
	m_vHeight = Height;
	m_hDc = CreateCompatibleDC(hdc);
	if (!m_hDc) return false;
	m_hNewBitmap = CreateCompatibleBitmap(hdc,Width,Height); // создаЄм совместимую пам€ть
	if (!m_hNewBitmap) return false;
	if ((m_hOldBitmap = (HBITMAP)SelectObject(m_hDc,m_hNewBitmap)) == NULL) return false;
	return true;
}

bool ImageBuffer::CreateCompatible(HDC hdc, HBITMAP hBitMap)
{
	Clear();
	m_hDc = CreateCompatibleDC(hdc);
	if (!m_hDc) return false;
	BITMAP bm;
	if (!GetObject(hBitMap,sizeof(BITMAP),&bm))
		return false;
	m_vWidth = bm.bmWidth;
	m_vHeight = bm.bmHeight;
	m_hNewBitmap = hBitMap;
	if ((m_hOldBitmap = (HBITMAP)SelectObject(m_hDc,m_hNewBitmap)) == NULL) return false;
	return true;
}

bool ImageBuffer::CreateCompatible(ImageBuffer &ib, long Width, long Height)
{
	Clear();
	m_vWidth = Width;
	m_vHeight = Height;
	m_hDc = CreateCompatibleDC(ib.m_hDc);
	if (!m_hDc) return false;
	m_hNewBitmap = CreateCompatibleBitmap(ib.m_hDc,Width,Height); // создаЄм совместимую пам€ть
	if (!m_hNewBitmap) return false;
	if ((m_hOldBitmap = (HBITMAP)SelectObject(m_hDc,m_hNewBitmap)) == NULL) return false;	
	return true;
}

void ImageBuffer::SetPixel(int x, int y, ARGBColor Color)
{
	::SetPixel(m_hDc,x,y,Color);
}

bool ImageBuffer::MoveTo(int x, int y)
{
	return ::MoveToEx(m_hDc,x,y,NULL) == TRUE;
}

bool ImageBuffer::LineTo(int x, int y)
{
	return ::LineTo(m_hDc, x, y) == TRUE;
}

bool ImageBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	if (!::MoveToEx(m_hDc,x1,y1,NULL)) return false;
	if (!::LineTo(m_hDc,x2,y2)) return false;
	return true;
}

bool ImageBuffer::DrawText(int x, int y, LPCTSTR Text)
{
	return TextOut(m_hDc,x,y,Text,_tcslen(Text)) == TRUE;
}

bool ImageBuffer::FillRect(LPRECT Rect, IB_Brush *Brush)
{
	return ::FillRect(m_hDc, Rect, (HBRUSH)Brush->GetNativeObject()) != 0;
}

int ImageBuffer::SetBkMode(int Mode)
{
	return ::SetBkMode(m_hDc,Mode);
}

void ImageBuffer::Contrast(int Value)
{
	char RedValue;
	char GreenValue;
	char BlueValue;
	double Pixel;
	double ContrastValue = (100.0 + Value) / 100.0; //¬ычисл€ем общее значение контраста
	ContrastValue *= ContrastValue;

	for (long i = 0; i < m_vWidth; i++)
	{
		for (long j = 0; j < m_vHeight; j++)
		{
			COLORREF PixelColor = GetPixel(m_hDc,i,j);
			
			// расчЄт красной составл€юдщей
			Pixel = GetRValue(PixelColor) / 255.0;
			Pixel -= 0.5;
			Pixel *= ContrastValue;
			Pixel += 0.5;
			Pixel *= 255;
			if (Pixel < 0) Pixel = 0;
			if (Pixel > 255) Pixel = 255;
			RedValue = (char)Pixel;

			// расчЄт зелЄной составл€ющей
			Pixel = GetGValue(PixelColor) / 255.0;
			Pixel -= 0.5;
			Pixel *= ContrastValue;
			Pixel += 0.5;
			Pixel *= 255;
			if (Pixel < 0) Pixel = 0;
			if (Pixel > 255) Pixel = 255;
			GreenValue = (char)Pixel;

			// расчЄт голубой составл€ющей
			Pixel = GetBValue(PixelColor) / 255.0;
			Pixel -= 0.5;
			Pixel *= ContrastValue;
			Pixel += 0.5;
			Pixel *= 255;
			if (Pixel < 0) Pixel = 0;
			if (Pixel > 255) Pixel = 255;
			BlueValue = (char)Pixel;

			::SetPixel(m_hDc,i,j,RGB(RedValue,GreenValue,BlueValue));
		}
	}
}

void ImageBuffer::SetFont(IB_Font *Font)
{
	SelectObject(m_hDc,Font->GetNativeObject());
}

void ImageBuffer::SetPen(IB_Pen *Pen)
{
	SelectObject(m_hDc,Pen->GetNativeObject());
}

void ImageBuffer::SetBrush(IB_Brush *Brush)
{
	SelectObject(m_hDc,Brush->GetNativeObject());
}

void ImageBuffer::SetTextColor(ARGBColor Color)
{
	::SetTextColor(m_hDc,(COLORREF)Color);
}

long ImageBuffer::GetWidth(void)
{
	return m_vWidth;
}

long ImageBuffer::GetHeight(void)
{
	return m_vHeight;
}

void ImageBuffer::Blacken()
{
	PatBlt(m_hDc,0,0,m_vWidth,m_vHeight,BLACKNESS);
}

bool ImageBuffer::SaveToFile(ImageBuffer_cstring FileName)
{
	// create file 
	HANDLE hFile = CreateFile(FileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	// save bitmap file headers 
	BITMAPFILEHEADER fileHeader; 
	BITMAPINFOHEADER infoHeader;
	fileHeader.bfType      = 0x4d42; 
	fileHeader.bfSize      = 0; 
	fileHeader.bfReserved1 = 0; 
	fileHeader.bfReserved2 = 0; 
	fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
	infoHeader.biSize = sizeof(infoHeader); 
	infoHeader.biWidth = m_vWidth; 
	infoHeader.biHeight = m_vHeight; 
	infoHeader.biPlanes = 1; 
	infoHeader.biBitCount = 24; 
	infoHeader.biCompression = BI_RGB; 
	infoHeader.biSizeImage = 0; 
	infoHeader.biXPelsPerMeter = 0; 
	infoHeader.biYPelsPerMeter = 0; 
	infoHeader.biClrUsed  = 0; 
	infoHeader.biClrImportant  = 0; 
	
	DWORD dwBytesWritten;
	BOOL bWriteResult;
	bWriteResult = WriteFile(hFile,&fileHeader,sizeof(fileHeader),&dwBytesWritten,NULL);
	if (!bWriteResult) 
	{
		return false;
	}
	bWriteResult = WriteFile(hFile,&infoHeader,sizeof(infoHeader),&dwBytesWritten,NULL);
	if (!bWriteResult) 
	{
		return false;
	}
	BITMAPINFO info; info.bmiHeader = infoHeader;  
	HDC memDC = CreateCompatibleDC(m_hDc); 
	BYTE* memory = 0; 
	HBITMAP bitmap = CreateDIBSection(m_hDc, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
	SelectObject(memDC, bitmap); 
	BitBlt(memDC, 0, 0, m_vWidth, m_vHeight, m_hDc, 0, 0, SRCCOPY); 
	DeleteDC(memDC); 
	int bytes = (((24*m_vWidth + 31) & (~31))/8)*m_vHeight; 
	bWriteResult = WriteFile(hFile,memory,bytes,&dwBytesWritten,NULL);
	if (!bWriteResult)
	{
		return false;
	}
	DeleteObject(bitmap);
	CloseHandle(hFile);
	return true;
}