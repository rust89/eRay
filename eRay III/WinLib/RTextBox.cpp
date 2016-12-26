
#include "RTextBox.h"

using namespace RClasses;

RTextBox::RTextBox()
	: RControl()
{
	m_hBackgoundFont = NULL;
	ClassName = TEXT("RClasses::RTextBox");
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RTextBox::OnPaint),this);
	AddMessage(WM_SETFONT,MSG_EVENT_CAST(RTextBox::OnSetFont),this);
	AddMessage(WM_SETFOCUS,MSG_EVENT_CAST(RTextBox::OnSetFocus),this);
}

RTextBox::~RTextBox()
{

}

bool RTextBox::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width,int height)
{
	return RControl::Create(Text, WC_EDIT, ClassName.c_str(), 
							WS_CHILD | WS_VISIBLE | ES_MULTILINE, 
							WS_EX_CLIENTEDGE, 
							p_wndParent, uiID,x, y, width, height);
}

bool RTextBox::Create(UINT Style, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width,int height)
{
	return RControl::Create(TEXT(""), WC_EDIT, ClassName.c_str(), 
							Style, WS_EX_CLIENTEDGE, 
							p_wndParent, uiID,x, y, width, height);
}

bool RTextBox::IsMultiLine()
{
	if ((GetStyle() & ES_MULTILINE) == ES_MULTILINE)
		return true;
	return false;
}

LRESULT RTextBox::GetLineCount()
{
	return SndMsg(EM_GETLINECOUNT,0,0);
}

LRESULT RTextBox::GetLineLength(LRESULT LineIndex)
{
	return SndMsg(EM_LINELENGTH,LineIndex,0);
}

void RTextBox::AddLine(RString Line)
{
	RString Buffer = GetText();
	if (Buffer.length())
		Buffer += TEXT("\r\n");
	Buffer += Line;
	SetText(Buffer.c_str());
}

void RTextBox::AddText(RString Text)
{
	RString Buffer = GetText();
	Buffer+= Text;
	SetText(Buffer.c_str());
}

void RTextBox::SetLimit(UINT Count)
{
	SndMsg(EM_SETLIMITTEXT,(WPARAM)Count,0);
}

void RTextBox::Clear()
{
	SetWindowText(m_hHandle,NULL);
}

void RTextBox::ScrollTextEnd()
{
	SndMsg(EM_LINESCROLL,0,SendMessage(m_hHandle,EM_GETLINECOUNT,0,0));
}

LRESULT RTextBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_PAINT,wParam,lParam);


	if (m_hHandle != GetFocus())
	{
		if (!GetWindowTextLength(m_hHandle))
		{
			if (!m_szBackgroundText.empty())
			{
				HDC hdc = GetDC(m_hHandle);
				RECT ClientRect;
				GetClientRect(ClientRect);
				
				SetTextColor(hdc,GetSysColor(COLOR_GRAYTEXT));
				SelectObject(hdc,m_hBackgoundFont);
				
				IsEnabled() ? SetBkColor(hdc,GetSysColor(COLOR_WINDOW)) : SetBkColor(hdc,GetSysColor(COLOR_3DFACE));

				DrawText(hdc,m_szBackgroundText.c_str(),m_szBackgroundText.size(),&ClientRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER); 

				DeleteDC(hdc);
			}
		}
	}

	return Result;
}

void RTextBox::SetBackgroundText(LPCTSTR Str)
{
	m_szBackgroundText.assign(Str);
}

LRESULT RTextBox::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_SETFONT,wParam,lParam);

	HFONT hFont = (HFONT)wParam;

	LOGFONT fontAttributes = { 0 };
    ::GetObject(hFont, sizeof(fontAttributes), &fontAttributes);
	if (fontAttributes.lfHeight > 1) fontAttributes.lfHeight -= 2;
	fontAttributes.lfItalic = TRUE;

	m_hBackgoundFont = CreateFontIndirect(&fontAttributes);

	return Result;
}

LRESULT RTextBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = CallWindowProc(ControlBaseProc, m_hHandle,WM_SETFOCUS,wParam,lParam);

	SetCaretPos(GetWindowTextLength(m_hHandle));

	return Result;
}

int RTextBox::GetCaretIndex()
{
	POINT CaretPos;
	if (!GetCaretPos(&CaretPos))
		return -1;
	return LOWORD(SndMsg(EM_CHARFROMPOS,0,MAKELPARAM(CaretPos.x,CaretPos.y)));
}

void RTextBox::SetCaretPos(int CaretIndex)
{
	SndMsg(EM_SETSEL,CaretIndex,CaretIndex);
}