#include "RFrame.h"

using namespace RClasses;

RFrame::RFrame() 
	: RWindow()
{

}

RFrame::~RFrame()
{

}

bool RFrame::Create(DWORD ExStyle, LPCTSTR ClassName, RString Caption, 
	        DWORD Style, 
			RCustomWindow *pOwner, int x, int y,
			int width,int height, HMENU hMenu)
{
	m_hHandle = CreateWindowEx(ExStyle, ClassName, Caption.c_str(),
				 Style,
				 x,y,width,height,pOwner != NULL ? pOwner->Handle() : NULL,hMenu,
				 GetModuleHandle(0),
				 this//Передаем в оконную функцию указатель на класс нашего окна
				 );
	if (!m_hHandle)
		return false;
	m_WindowClassName.assign(ClassName);
	return true;
}

bool RFrame::RegisterWndClass(LPCTSTR ClassName, UINT Style, HBRUSH hBrush, HCURSOR hCursor)
{
	WNDCLASSEX wnd;
	wnd.lpszClassName = ClassName;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpfnWndProc = WNDPROC(_WndProc);//Оконная процедура
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = hBrush;//Цвет фона окна
	wnd.hInstance = GetModuleHandle(0); //Хендл приложения
	wnd.hCursor = hCursor; //Загружаем старндартный курсор
	wnd.style = Style;
	wnd.hIcon = 0;
	wnd.hIconSm = 0;
	wnd.lpszMenuName = 0;
	if (!RegisterClassEx(&wnd))
		return false;
	return true;
}

bool RFrame::UnRegisterWndClass(LPCTSTR ClassName)
{
	return ::UnregisterClass(ClassName,GetModuleHandle(0)) != FALSE;
}


LRESULT CALLBACK RFrame::_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{  
    RFrame *wnd = 0;
	//Сообщения WM_NCCREATE приходит до WM_CREATE
	//т.е при создании окна
	if(Message==WM_NCCREATE)
	{
		//Получаем указатель на экземпляр нашего окна, который мы передали в функцию CreateWindowEx
		wnd=(RFrame*)LPCREATESTRUCT(lParam)->lpCreateParams;
		//И сохраняем в поле GWL_USERDATA
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
		wnd->m_hHandle = hwnd;      
	}
	//Теперь получаем указатель на наш экземлпяр окна, но уже из поля GWL_USERDATA
	wnd=(RFrame*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
	if(wnd)
	{  
		CustomEvent FuncPointer = wnd->m_mMsgMap.Find(Message);
		if (!FuncPointer) return DefWindowProc(hwnd,Message,wParam,lParam);
		return (wnd->*FuncPointer)(wParam,lParam);
	}
	return DefWindowProc(hwnd,Message,wParam,lParam);
}

void RFrame::TabManager(RCustomWindow *Sender)
{
	// поиск контрола в списке
	int ControlIndex = -1;
	for (int i = 0; i < TabList.Length(); i++)
	{
		if (TabList[i] == Sender)
		{
			ControlIndex = i;
			break;
		}
	}
	// выходим если контрол не задан
	if(ControlIndex == -1) 
		return;
	// ищем следующий контрол для активации
	int NextActivateControl = -1;
	for (int i = ControlIndex + 1; i < TabList.Length(); i++)
	{
		if (TabList[i]->IsEnabled())
		{
			NextActivateControl = i;
			break;
		}
	}

	// если контрол не найден в конце списка, то ищем его в начале списка
	if (NextActivateControl == -1)
	{
		for (int i = 0; i < ControlIndex; i++)
		{
			if (TabList[i]->IsEnabled())
			{
				NextActivateControl = i;
				break;
			}
		}
	}

	if (NextActivateControl == -1)
		return;

	
	
	TabList[NextActivateControl]->SetFocus();
	//TabList[NextActivateControl]->Refresh();
}