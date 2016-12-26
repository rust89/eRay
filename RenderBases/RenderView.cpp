
#include "RenderView.h"


RenderView::RenderView()
{
	m_Handle = 0;
	WindowClassAtom = 0;
}

RenderView::~RenderView()
{

}

int RenderView::Create(HWND hOwner, LPCTSTR pWindowName)
{
	WNDCLASSEX wnd;
	wnd.lpszClassName = pWindowName;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpfnWndProc = WNDPROC(_WndProc);//������� ���������
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//���� ���� ����
	wnd.hInstance = GetModuleHandle(0);//����� ����������
	wnd.hCursor = LoadCursor(0,IDC_ARROW);//��������� ������������ ������
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.hIcon = 0;
	wnd.hIconSm = 0;
	wnd.lpszMenuName = 0;
	// ������������ ����
	if (!(WindowClassAtom = RegisterClassEx(&wnd)))
		return false;

	if(!CreateWindowEx(0, pWindowName, NULL,
				 WS_CHILDWINDOW | WS_VISIBLE,
				 CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hOwner,NULL,
				 GetModuleHandle(0),
				 this//�������� � ������� ������� ��������� �� ����� ������ ����
				 ))
		return -1;
	return 0;
}

void RenderView::Show()
{
	ShowWindow(m_Handle, SW_SHOW); 
}

void RenderView::Hide()
{
	ShowWindow(m_Handle, SW_HIDE);
}

void RenderView::SetPosition(int x, int y, int width, int height)
{
	MoveWindow(m_Handle, x, y, width, height, TRUE);
}

LRESULT CALLBACK RenderView::_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{  
    RenderView *wnd = 0;
	//��������� WM_NCCREATE �������� �� WM_CREATE
	//�.� ��� �������� ����
	if(Message==WM_NCCREATE)
	{
		//�������� ��������� �� ��������� ������ ����, ������� �� �������� � ������� CreateWindowEx
		wnd=(RenderView*)LPCREATESTRUCT(lParam)->lpCreateParams;
		//� ��������� � ���� GWL_USERDATA
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
		wnd->m_Handle = hwnd;
		return TRUE; // ���������� �������� ����
	}
	//������ �������� ��������� �� ��� ��������� ����, �� ��� �� ���� GWL_USERDATA
	wnd=(RenderView*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
	if(wnd)
	{	  
		switch(Message)
		{
			case WM_CREATE:
			{
				return wnd->wmCreate((LPCREATESTRUCT)lParam);
			}
			case WM_SIZE:
			{
				return wnd->wmSize(wParam,LOWORD(lParam),HIWORD(lParam));
			}
		}
	}
	return DefWindowProc(hwnd,Message,wParam,lParam);
}

LRESULT RenderView::wmCreate(LPCREATESTRUCT lpcs)
{
	return 0;
}

LRESULT RenderView::wmSize(WPARAM ResizingRequest, INT NewClientWidth, INT NewClientHeight)
{
	return 0;
}