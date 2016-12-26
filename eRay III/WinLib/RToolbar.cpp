#include "RToolBar.h"

using namespace RClasses;

RToolBar::RToolBar()
{
	ClassName = TEXT("RClasses::RToolBar");
}

RToolBar::~RToolBar()
{
	//
}
bool RToolBar::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, TOOLBARCLASSNAME, ClassName.c_str(), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_TOP | TBSTYLE_TOOLTIPS, WS_EX_TRANSPARENT, p_wndParent, uiID, x, y, width, height);
}

void RToolBar::AutoSize()
{
	SendMessage(m_hHandle,TB_AUTOSIZE,0,0);
}

void RToolBar::BindingImageList(RImageList &ImageList)
{
	SendMessage(m_hHandle, TB_SETIMAGELIST, 0, (LPARAM)ImageList.GetNativeImageList());
}

void RToolBar::SetButtonEnable(UINT btnID, bool bEnable)
{
	SendMessage(m_hHandle,TB_ENABLEBUTTON,btnID,bEnable);
}

int RToolBar::BtnCount()
{
	return SendMessage(m_hHandle,TB_BUTTONCOUNT,0,0);
}

int RToolBar::GetBtnWidth()
{
	return LOWORD(SendMessage(m_hHandle,TB_GETBUTTONSIZE,0,0));
}

int RToolBar::GetBtnHeight()
{
	return HIWORD(SendMessage(m_hHandle,TB_GETBUTTONSIZE,0,0));
}

bool RToolBar::GetItemRect(int index, LPRECT lpRect)
{
	return SendMessage(m_hHandle,TB_GETITEMRECT,index,(LPARAM)lpRect) == TRUE ? true : false;
}

int RToolBar::GetItemWidth()
{
	RECT tmpRect;
	GetItemRect(BtnCount() - 1,&tmpRect);
	return tmpRect.right;
}
