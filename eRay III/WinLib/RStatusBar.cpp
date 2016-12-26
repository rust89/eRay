
#include "RStatusBar.h"

using namespace RClasses;

RStatusBar::RStatusBar()
{
	ClassName = TEXT("RClasses::RStatusBar");
}

RStatusBar::~RStatusBar()
{
	//
}

bool RStatusBar::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, STATUSCLASSNAME, ClassName.c_str(), WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, p_wndParent, uiID, x, y, width, height);
}

bool RStatusBar::Create(LPCTSTR Text, RCustomWindow *p_wndParent, bool SizeGrip, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, STATUSCLASSNAME, ClassName.c_str(), WS_CHILD | WS_VISIBLE | (SizeGrip ? SBARS_SIZEGRIP : 0L), 0, p_wndParent, uiID, x, y, width, height);
}


void RStatusBar::SetSimple(bool Simple)
{
	SendMessage(m_hHandle,SB_SIMPLE,Simple ? TRUE : FALSE,0);
}

bool RStatusBar::IsSimple()
{
	return SendMessage(m_hHandle,SB_ISSIMPLE,0,0) != 0;
}

void RStatusBar::SetParts(const LPINT Parts, BYTE Size)
{
	SendMessage(m_hHandle,SB_SETPARTS,(WPARAM)Size,(LPARAM)Parts);
}

void RStatusBar::SetPanelText(BYTE Index, LPCTSTR Text)
{
	SendMessage(m_hHandle,SB_SETTEXT,(WPARAM)MAKEWPARAM(Index,0),(LPARAM) Text);
}

void RStatusBar::AutoSize()
{
	HWND hOwner = ::GetParent(m_hHandle);
	if (hOwner)
	{
		// получаем размер клиентской области владельца
		RECT ClientRect;
		::GetClientRect(hOwner,&ClientRect);
		// получаем сведения о размер статус бара
		RECT StatusBarRect;
		::GetWindowRect(m_hHandle,&StatusBarRect);
		LONG StatusBarHeight = StatusBarRect.bottom - StatusBarRect.top;

		SetWindowPos(m_hHandle,NULL,0,ClientRect.bottom - StatusBarHeight,
									ClientRect.right,StatusBarHeight,SWP_NOACTIVATE);
	}
}