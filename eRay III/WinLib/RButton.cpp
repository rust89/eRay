
#include "RButton.h"

using namespace RClasses;

RButton::RButton()
{
	ClassName = TEXT("RClasses::RButton");
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RButton::OnPaint),this);
	AddMessage(WM_SETFOCUS,MSG_EVENT_CAST(RButton::OnSetFocus),this);
	AddMessage(WM_KILLFOCUS,MSG_EVENT_CAST(RButton::OnKillFocus),this);
}

RButton::~RButton()
{
	//
}

bool RButton::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, WC_BUTTON, ClassName.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_TEXT, 0, p_wndParent, uiID,x,y,width,height);
}

LRESULT RButton::OnPaint(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_PAINT,wParam,lParam);

	// отрисовка фокусного прямоугольника
	if (m_hHandle == GetFocus())
	{
		HDC hdc = GetDC(m_hHandle);
		RECT ClientRect;
		GetClientRect(ClientRect);

		ClientRect.left += 3;
		ClientRect.right += -3;
		ClientRect.top += 3;
		ClientRect.bottom += -3;

		DrawFocusRect(hdc,&ClientRect);
		DeleteDC(hdc);
	}

	return Result;
}

LRESULT RButton::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_SETFOCUS,wParam,lParam);

	SendMessage(m_hHandle,BM_SETSTYLE,BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD,TRUE);

	return Result;
}

LRESULT RButton::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_KILLFOCUS,wParam,lParam);

	SendMessage(m_hHandle,BM_SETSTYLE,BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,TRUE);

	return Result;
}

