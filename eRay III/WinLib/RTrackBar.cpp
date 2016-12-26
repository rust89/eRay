#include "RTrackBar.h"

using namespace RClasses;

RTrackBar::RTrackBar()
{
	ClassName = TEXT("RClasses::RTrackBar");
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RTrackBar::OnPaint),this);
}

RTrackBar::~RTrackBar()
{

}

bool RTrackBar::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, TRACKBAR_CLASS, ClassName.c_str(), WS_CHILD | WS_VISIBLE, 0, p_wndParent, uiID,x,y,width,height);
}

void RTrackBar::SetRange(WORD Min, WORD Max)
{
	SendMessage(m_hHandle,TBM_SETRANGE,TRUE,MAKELPARAM(Min,Max));
}

void RTrackBar::SetTrackPos(INT Pos)
{
	SendMessage(m_hHandle,TBM_SETPOS,TRUE,(LPARAM)Pos);
}

int RTrackBar::GetTrackPos()
{
	return (int)SendMessage(m_hHandle,TBM_GETPOS,0,0);
}

LRESULT RTrackBar::OnPaint(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_PAINT,wParam,lParam);


	if (m_hHandle == GetFocus())
	{
		HDC hdc = GetDC(m_hHandle);
		RECT ClientRect;
		GetClientRect(ClientRect);
		/*
		ClientRect.left += 3;
		ClientRect.right += -3;
		ClientRect.top += 3;
		ClientRect.bottom += -3;
		*/
		DrawFocusRect(hdc,&ClientRect);
		DeleteDC(hdc);
	}

	return Result;
}