#include "RWindow.h"

using namespace RClasses;

RWindow::RWindow()
	: RCustomWindow()
{

}
RWindow::~RWindow()
{

}

void RWindow::SetBigIcon(HICON hIcon)
{
	SendMessage(m_hHandle,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
}

void RWindow::SetSmallIcon(HICON hIcon)
{
	SendMessage(m_hHandle,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
}

int RWindow::ShowModal()
{
	HWND hLastFocus = ::GetFocus();
	RCustomWindow *Parent = GetParent();
	
	if (Parent)
		Parent->SetEnable(FALSE);

	ShowWindow(m_hHandle,SW_SHOW);
	UpdateWindow(m_hHandle);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != WM_NULL) // отработка сообщений
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//SetForegroundWindow(Parent->Handle());
	
	if (Parent)
		Parent->SetEnable(TRUE);
	
	//Parent->SetFocus();

	DestroyWindow();

	::SetFocus(hLastFocus);

	return msg.wParam;
}