
#include "RControl.h"

using namespace RClasses;

RControl::RControl()
	: RCustomWindow(), ClassName(TEXT(""))
{
}

RControl::~RControl()
{
	//
}

bool RControl::Create(LPCTSTR Text, LPCTSTR BaseClassName, LPCTSTR ClassName, UINT Style, UINT ExStyle, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width,int height)
{
	WNDCLASS wnd;
	if (! ::GetClassInfo(ModuleHandle(),BaseClassName,&wnd))
		return false;

	// ������� ������� ���������
	ControlBaseProc = wnd.lpfnWndProc;
	wnd.lpfnWndProc = ControlProc;

	// �������������� ������
	wnd.lpszClassName = ClassName;

	if (!RegisterClass(&wnd))
	{
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
		{
			return false;
		}
	}

	m_hHandle = CreateWindowEx(ExStyle, ClassName, Text,
				Style,x,y,width,height,p_wndParent->Handle(),(HMENU)uiID,
				ModuleHandle(), this);

	return true;
}

LRESULT WINAPI RControl::ControlProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    RControl *wnd(0);
	if(Message == WM_NCCREATE)
	{
		//�������� ��������� �� ��������� ������ ����, ������� �� �������� � ������� CreateWindowEx
		wnd = (RControl*)LPCREATESTRUCT(lParam)->lpCreateParams;
		//� ��������� � ���� GWL_USERDATA
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
		wnd->m_hHandle = hwnd;
		return wnd->ControlBaseProc(hwnd,Message,wParam,lParam);
	}
	//������ �������� ��������� �� ��� ��������� ����, �� ��� �� ���� GWL_USERDATA
	if(wnd = (RControl*)GetWindowLongPtr(hwnd,GWLP_USERDATA))
	{
		if (Message == WM_CHAR)
		{
			if (wParam == VK_TAB)
			{
				RCustomWindow* parent = wnd->GetParent();
				parent->TabManager(wnd);
				return 0;
			}
		}
		CustomEvent FuncPointer;
		RCustomWindow *pWindow;
		if (wnd->m_mMsgMap.Find(Message,FuncPointer,pWindow))
		{
			return (pWindow->*FuncPointer)(wParam,lParam);
		}
	}
	return wnd->ControlBaseProc(hwnd,Message,wParam,lParam);
}

void RControl::SetFont(HFONT hFont)
{
	SendMessage(m_hHandle,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(TRUE,0));
}

LRESULT RControl::DefaultControlProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return ControlBaseProc(m_hHandle,Message,wParam,lParam);
}