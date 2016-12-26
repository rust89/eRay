
#include "RCheckBox.h"

using namespace RClasses;

RCheckBox::RCheckBox()
{
	ClassName = TEXT("RClasses::RCheckBox");
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RCheckBox::OnPaint),this);
}

RCheckBox::~RCheckBox()
{
	//
}

bool RCheckBox::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, WC_BUTTON, ClassName.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, WS_EX_LEFT, p_wndParent, uiID,x,y,width,height);
}

UINT RCheckBox::GetState()
{
	// возвращаемые значения: BST_CHECKED / BST_INDETERMINATE / BST_UNCHECKED
	return SendMessage(m_hHandle,BM_GETCHECK,0,0);
}

void RCheckBox::SetState(UINT CheckState)
{
	SendMessage(m_hHandle,BM_SETCHECK,CheckState,0);
}

bool RCheckBox::GetCheck()
{
	return SendMessage(m_hHandle,BM_GETCHECK,0,0) == BST_CHECKED;
}

void RCheckBox::SetCheck(bool Check)
{
	SendMessage(m_hHandle,BM_SETCHECK,Check ? BST_CHECKED : BST_UNCHECKED,0);
}

LRESULT RCheckBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_PAINT,wParam,lParam);

	if (m_hHandle == GetFocus())
	{
		HDC hdc = GetDC(m_hHandle);
		RECT ClientRect;
		GetClientRect(ClientRect);
		DrawFocusRect(hdc,&ClientRect);
		DeleteDC(hdc);
	}
	return Result;
}