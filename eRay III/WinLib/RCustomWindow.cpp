#include "RCustomWindow.h"

using namespace RClasses;

RCustomWindow::RCustomWindow()
	: RObject()
{
	m_hHandle = NULL;
}

RCustomWindow::~RCustomWindow()
{
	//
}

const HWND RCustomWindow::Handle(void) const
{
	return m_hHandle;
}

bool RCustomWindow::AddMessage(UINT Message, CustomEvent pEvent, RCustomWindow* pEventOwner)
{
	if(!pEvent) return false;
	this->m_mMsgMap.AddMessage(Message,pEvent,pEventOwner);
	return true;
}


RString RCustomWindow::GetText(void)
{
	if (m_hHandle)
	{
		int TextLen = GetWindowTextLength(m_hHandle) + 1;
		LPTSTR TextBuffer = new TCHAR[TextLen];
		GetWindowText(m_hHandle,TextBuffer,TextLen);
		RString Result(TextBuffer);
		delete [] TextBuffer;
		return Result;
	}
	return RString();
}

LONG RCustomWindow::GetStyle(void)
{
	return m_hHandle ? GetWindowLong(m_hHandle,GWL_STYLE) : NULL;
}

LONG RCustomWindow::GetExStyle()
{
	return m_hHandle ? GetWindowLong(m_hHandle,GWL_EXSTYLE) : NULL;
}

LONG RCustomWindow::GetWidth()
{
	RECT WindowRect;
	if (::GetWindowRect(m_hHandle,&WindowRect))
		return WindowRect.right - WindowRect.left;
	return -1; 
}
LONG RCustomWindow::GetHeight()
{
	RECT WindowRect;
	if (::GetWindowRect(m_hHandle,&WindowRect))
		return WindowRect.bottom - WindowRect.top;
	return -1; 
}

LONG RCustomWindow::GetClientWidth()
{
	RECT ClientRect;
	if (::GetWindowRect(m_hHandle,&ClientRect))
		return ClientRect.right - ClientRect.left;
	return -1; 
}

LONG RCustomWindow::GetClientHeight()
{
	RECT ClientRect;
	if (::GetWindowRect(m_hHandle,&ClientRect))
		return ClientRect.bottom - ClientRect.top;
	return -1; 
}

BOOL RCustomWindow::GetClientRect(RECT& Rect)
{
	return ::GetClientRect(m_hHandle,&Rect);
}

BOOL RCustomWindow::GetWindowRect(RECT& Rect)
{
	return ::GetWindowRect(m_hHandle,&Rect);
}

RCustomWindow* RCustomWindow::GetParent()
{
	return (RCustomWindow*)GetWindowLongPtr(::GetParent(m_hHandle),GWLP_USERDATA);
}

LONG_PTR RCustomWindow::GetClassLong(int Index)
{
	return ::GetClassLongPtr(m_hHandle, Index);
}

void RCustomWindow::SetText(LPCTSTR Text)
{
	if (m_hHandle)
	{
		SetWindowText(m_hHandle,Text);
	}
}

void RCustomWindow::SetStyle(LONG Style)
{
	if (m_hHandle)
	{
		SetWindowLong(m_hHandle,GWL_STYLE,Style);
	}
}

void RCustomWindow::SetExStyle(LONG ExStyle)
{
	if (m_hHandle)
	{
		SetWindowLong(m_hHandle,GWL_EXSTYLE, ExStyle);
	}
}

void RCustomWindow::SetClassLong(int Index, LONG_PTR dwNewLong)
{
	::SetClassLongPtr(m_hHandle,Index,dwNewLong);
}

void RCustomWindow::SetWindowPosition(HWND hWndInsertAfter, int x, int y, int width, int height, UINT uFlags)
{
	SetWindowPos(m_hHandle, hWndInsertAfter, x, y, width, height, uFlags);
}

void RCustomWindow::BringWindowToTop()
{
	::BringWindowToTop(m_hHandle);
}

void RCustomWindow::SetParent(RCustomWindow *p_wndParent)
{
	::SetParent(m_hHandle, p_wndParent->m_hHandle);
}

void RCustomWindow::SetEnable(BOOL State)
{
	EnableWindow(m_hHandle,State);
}

void RCustomWindow::Move(int x, int y, int width, int height, BOOL bRepaint)
{
	MoveWindow(m_hHandle, x, y, width, height, bRepaint);
}

void RCustomWindow::Refresh()
{
	InvalidateRect(m_hHandle,NULL,TRUE);
}

void RCustomWindow::AddToolTip(LPTSTR pText)
{
	UINT STYLE =  WS_POPUP |TTS_ALWAYSTIP;

    // Create the tooltip. 
    HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
							  STYLE,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
							  m_hHandle, NULL, 
							  GetModuleHandle(NULL), NULL);
    
	if (!m_hHandle || !hwndTip)
	{
		return;
	}                              

    // Associate the tooltip with the tool.
	TOOLINFO toolInfo = {0};
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = m_hHandle;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)m_hHandle;
    toolInfo.lpszText = pText;

    BOOL Result = SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	// activating tooltip
	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
}

const RString RCustomWindow::GetClassName() const
{
	return m_WindowClassName;
}
void RCustomWindow::DestroyWindow()
{
	::DestroyWindow(m_hHandle);
	m_hHandle = NULL;
}

void RCustomWindow::TabManager(RCustomWindow *Sender)
{
	//
}

void RCustomWindow::SetFocus()
{
	::SetFocus(m_hHandle);
}

bool RCustomWindow::IsEnabled()
{
	return ::IsWindowEnabled(m_hHandle) == TRUE;
}

LRESULT RCustomWindow::SndMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(m_hHandle, Msg, wParam, lParam);
}