
#include "RImageList.h"

using namespace RClasses;

RImageList::RImageList()
{
	m_hImageList = 0;
}

RImageList::~RImageList()
{
	Destroy();
}

void RImageList::Destroy()
{
	if (m_hImageList)
	{
		ImageList_Destroy(m_hImageList);
		m_hImageList = 0;
	}
}

bool RImageList::Create(int cx, int cy, UINT flags, int cInitial, int cGrow)
{
	return (m_hImageList = ImageList_Create(
        cx, cy, flags, cInitial, cGrow)) == NULL ? false : true;
}

const HIMAGELIST RImageList::GetNativeImageList() const
{
	return m_hImageList;
}

int RImageList::AddMasked(HBITMAP hbmImage, COLORREF crMask)
{
	return ImageList_AddMasked(m_hImageList, hbmImage, crMask);
}

HICON RImageList::GetIcon(int i,UINT flags)
{
	return ImageList_GetIcon(m_hImageList, i, flags);
}


HBITMAP RImageList::GetBitmap(int i)//за освобождение ресурса HBITMAP отвечает юзверь
{
    //копируем битмап
    HDC hDTDC=::GetDC(NULL);
    HDC hDestDC=::CreateCompatibleDC(hDTDC);
    HBITMAP hBitmapRet;
	int cx, cy;
	ImageList_GetIconSize(m_hImageList,&cx,&cy);
    hBitmapRet = CreateCompatibleBitmap(hDTDC,cx,cy);
    hBitmapRet = (HBITMAP)SelectObject(hDestDC,hBitmapRet);
    ImageList_DrawEx(m_hImageList,i,hDestDC,0,0,0,0,CLR_NONE,CLR_DEFAULT,ILD_TRANSPARENT);
    hBitmapRet=(HBITMAP)SelectObject(hDestDC,hBitmapRet);
    //освобождаем GDI ресурсы
    ::DeleteDC(hDestDC);    
    ::ReleaseDC(NULL,hDTDC);
    return hBitmapRet;
};

const int RImageList::GetImageCount() const
{
	return ImageList_GetImageCount(m_hImageList);
}
