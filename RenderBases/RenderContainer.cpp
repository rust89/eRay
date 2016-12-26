
#include "RenderContainer.h"

RenderContainer::RenderContainer()
{
	m_Handle = 0;
	WindowClassAtom = 0;
}

RenderContainer::~RenderContainer()
{

}

int RenderContainer::Create(HWND hOwner, LPCTSTR pWindowName)
{
	WNDCLASSEX wnd;
	wnd.lpszClassName = pWindowName;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpfnWndProc = WNDPROC(_WndProc);//Оконная процедура
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//Цвет фона окна
	wnd.hInstance = GetModuleHandle(0);//Хендл приложения
	wnd.hCursor = LoadCursor(0,IDC_ARROW);//Загружаем старндартный курсор
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.hIcon = 0;
	wnd.hIconSm = 0;
	wnd.lpszMenuName = 0;
	// регистрируем окно
	if (!(WindowClassAtom = RegisterClassEx(&wnd)))
		return false;

	if(!CreateWindowEx(0, pWindowName, NULL,
				 WS_CHILDWINDOW | WS_VISIBLE,
				 CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hOwner,NULL,
				 GetModuleHandle(0),
				 this//Передаем в оконную функцию указатель на класс нашего окна
				 ))
		return -1;
	return 0;
}

void RenderContainer::Show()
{
	ShowWindow(m_Handle, SW_SHOW); 
}

void RenderContainer::Hide()
{
	ShowWindow(m_Handle, SW_HIDE);
}

void RenderContainer::SetPosition(int x, int y, int width, int height)
{
	MoveWindow(m_Handle, x, y, width, height, TRUE);
}

LRESULT CALLBACK RenderContainer::_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{  
    RenderContainer *wnd = 0;
	//Сообщения WM_NCCREATE приходит до WM_CREATE
	//т.е при создании окна
	if(Message==WM_NCCREATE)
	{
		//Получаем указатель на экземпляр нашего окна, который мы передали в функцию CreateWindowEx
		wnd=(RenderContainer*)LPCREATESTRUCT(lParam)->lpCreateParams;
		//И сохраняем в поле GWL_USERDATA
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
		wnd->m_Handle = hwnd;
		return TRUE; // продолжаем создание окна
	}
	//Теперь получаем указатель на наш экземлпяр окна, но уже из поля GWL_USERDATA
	wnd=(RenderContainer*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
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

LRESULT RenderContainer::wmCreate(LPCREATESTRUCT lpcs)
{
	return 0;
}

LRESULT RenderContainer::wmSize(WPARAM ResizingRequest, INT NewClientWidth, INT NewClientHeight)
{
	return 0;
}