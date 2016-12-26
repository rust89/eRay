// Главный модуль проекта
// Содержит описание окна

// подключение библиотек для поддержки окна
#include <windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"

// подключаем библиотеку функции для работы с графикой 
#include "View3D.h"

// дополнительные библиотеки
#define _USE_MATH_DEFINES
#include <math.h>

// глобальные переменные
HINSTANCE hInstanceDLL = NULL;
#define WM_UPDATECONFIG WM_USER+1
#define WM_SETZPOS WM_USER+2

FLOAT **g_Field = 0;
INT g_FieldX = 0;
INT g_FieldY = 0;

#define EXPORT extern "C" __declspec (dllexport)
struct StartWindowItem {HWND hOwner; HANDLE hEvent; BOOL Result;};

// Прототипы функций поддержки окна
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR WINAPI ProgressBox(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI BuildVertexBuffers(LPVOID);
DWORD WINAPI WindowsThread(LPVOID);


EXPORT BOOL Run3DView(HWND hOwner, FLOAT** pXY, INT xCount, INT yCount)
{
	g_Field = pXY;
	g_FieldX = xCount;
	g_FieldY = yCount;

	StartWindowItem swi;
	swi.hOwner = hOwner;
	swi.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	return WindowsThread(&swi);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR Cursor1 = LoadCursor(hInstanceDLL,MAKEINTRESOURCE(IDC_CURSOR1)),
		Cursor2 = LoadCursor(hInstanceDLL,MAKEINTRESOURCE(IDC_CURSOR2));
	static float AngleX = 0, AngleY = 0;
	static float CamX = 0, CamY = 0, CamZ = 0;
	static bool IsMouseBtnDown = false;
	static WORD LastXPos, LastYPos;
	static BOOL ViewAsNet = FALSE;
	static RECT Borders;
	
	switch(Msg)
	{
		case WM_CREATE:
		{
			SetClassLong(hwnd,GCL_HCURSOR,(LONG) Cursor1);
			HWND hParent = GetParent(hwnd);
			if (hParent)
			{
				RECT ParentRect;
				GetWindowRect(hParent,&ParentRect);
				RECT WindowRect;
				GetWindowRect(hwnd,&WindowRect);

				SetWindowPos(hwnd,NULL,ParentRect.left + ((ParentRect.right - ParentRect.left)>>1) - ((WindowRect.right - WindowRect.left)>>1),
					ParentRect.top + ((ParentRect.bottom - ParentRect.top)>>1) - ((WindowRect.bottom - WindowRect.top)>>1),
					0,0,SWP_NOSIZE);
			}
			RECT MustRect = {0,0,640,640};
			if (AdjustWindowRect(&MustRect,GetWindowLong(hwnd,GWL_STYLE),GetMenu(hwnd) != NULL ? TRUE : FALSE))
			{
				SetWindowPos(hwnd,NULL,0,0,MustRect.right - MustRect.left,MustRect.bottom - MustRect.top,SWP_NOMOVE);
			}
			return 0;
		}
		
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_VIEW_NET:
				{
					HMENU hMenu = GetMenu(hwnd);
					if (hMenu)
					{
						HMENU hMenu = GetMenu(hwnd);
						if (hMenu)
						{
							if (!ViewAsNet)
							{
								CheckMenuItem(hMenu,ID_VIEW_NET,MF_CHECKED);
								pDirect3DDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
								ViewAsNet = TRUE;
							}
							else
							{
								CheckMenuItem(hMenu,ID_VIEW_NET,MF_UNCHECKED);
								pDirect3DDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
								ViewAsNet = FALSE;
							}
						}
					}
					break;
				}
			}
			return 0;
		}
		case WM_UPDATECONFIG:
		{
			HMENU hMenu = GetMenu(hwnd);
			return 0;
		}
		case WM_SETZPOS:
		{
			// поворот сцены
			D3DXMATRIX PosX, PosY;
			D3DXMatrixIdentity(&PosX);
			D3DXMatrixIdentity(&PosY);
			Rotate(&PosX,&PosY);			
			// установка камеры
			int xSize = wParam;
			int ySize = lParam;
			FLOAT maxHeight = FLOAT(max(xSize, ySize));
			CamX = 0;
			CamY = -maxHeight;
			CamZ = maxHeight;
			SetCam(CamX,CamY,CamZ);
			
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			IsMouseBtnDown = true;
			SetClassLong(hwnd,GCL_HCURSOR,(LONG) Cursor2);
			LastXPos = LOWORD(lParam);
			LastYPos = HIWORD(lParam);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			LastXPos = LOWORD(lParam);
			LastYPos = HIWORD(lParam);
			IsMouseBtnDown = false;
			SetClassLong(hwnd,GCL_HCURSOR,(LONG) Cursor1);
			return 0;
		}
		case WM_MOUSELEAVE:
		{
			IsMouseBtnDown = false;
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			if (IsMouseBtnDown)
			{
				WORD currx,curry;
				currx = LOWORD(lParam);
				curry = HIWORD(lParam);

				RECT ClientRect;
				GetClientRect(hwnd,&ClientRect);
				FLOAT RealYAngle = AngleY+= FLOAT(currx - LastXPos) / FLOAT(ClientRect.right) * 360;
				FLOAT RealXAngle = AngleX+= FLOAT(curry - LastYPos) / FLOAT(ClientRect.bottom) * 360;

				D3DXMATRIX PosX, PosY;
				D3DXMatrixIdentity(&PosX); D3DXMatrixIdentity(&PosY);
				
				D3DXMatrixRotationY(&PosY,(FLOAT)-M_PI*RealYAngle / 180);
				D3DXMatrixRotationX(&PosX,(FLOAT)M_PI*RealXAngle / 180);
				Rotate(&PosY,&PosX);
				
				LastXPos = currx;
				LastYPos = curry;
				
				DrawScene();
			}
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			D3DXVECTOR3 vec;
			vec.x = CamX;
			vec.y = CamY;
			vec.z = CamZ;
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				D3DXVECTOR3 Result;
				Result = vec * 1.2f;
				if (sqrt(Result.x*Result.x + Result.y*Result.y + Result.z*Result.z) > 4000)
					return 0;
				CamX = Result.x;
				CamY = Result.y;
				CamZ = Result.z;
			}
			else
			{
				D3DXVECTOR3 Result;
				Result = vec * 0.8f;
				if (sqrt(Result.x*Result.x + Result.y*Result.y + Result.z*Result.z) < 10)
				CamX = Result.x;
				CamY = Result.y;
				CamZ = Result.z;
			}
			SetCam(CamX,CamY,CamZ);
			DrawScene();
			return 0;
		}
	}
	return DefWindowProc(hwnd,Msg,wParam,lParam);
}

INT_PTR WINAPI ProgressBox(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_INITDIALOG:
		{	
			HWND hParent = GetParent(hwnd);
			if (hParent)
			{
				RECT ParentRect;
				GetWindowRect(hParent,&ParentRect);
				RECT WindowRect;
				GetWindowRect(hwnd,&WindowRect);

				SetWindowPos(hwnd,NULL,ParentRect.left + ((ParentRect.right - ParentRect.left)>>1) - ((WindowRect.right - WindowRect.left)>>1),
					ParentRect.top + ((ParentRect.bottom - ParentRect.top)>>1) - ((WindowRect.bottom - WindowRect.top)>>1),
					0,0,SWP_NOSIZE);
			}
			DWORD dwThreadID;
			HANDLE hThread;
			hThread = CreateThread(NULL,0,BuildVertexBuffers,hwnd,CREATE_SUSPENDED,&dwThreadID);
			if (hThread)
			{
				ResumeThread(hThread);
				CloseHandle(hThread);
			}	
			return TRUE;
		}
	}
	return FALSE;
}

DWORD WINAPI BuildVertexBuffers(LPVOID Param)
{
	HWND hwnd = (HWND)Param;
	if (!CreateVertexBuffer(g_Field, g_FieldX, g_FieldY,GetDlgItem(hwnd,IDC_PROGRESSVIEW),GetDlgItem(hwnd,IDC_STATICVIEW)))
	{
		EndDialog(hwnd,0);
		return 0;
	}
	EndDialog(hwnd,1);
	return 0;
}

DWORD WINAPI WindowsThread(LPVOID Param)
{
	StartWindowItem *swi = (StartWindowItem*)Param;	
	EnableWindow(swi->hOwner,FALSE);
	LPCTSTR szWindowClass = TEXT("View3D");
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstanceDLL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;
	if (!RegisterClassEx(&wcex))
	{
		EnableWindow(swi->hOwner,TRUE);
		swi->Result = FALSE;
		return swi->Result;
	}
	
	HWND hWnd;
	UINT Style = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_POPUP;
	RECT cl = {0,0,640,640};
	AdjustWindowRectEx(&cl,Style,0,0);
	int MainFormWidth = cl.right - cl.left, MainFormHeight = cl.bottom - cl.top;
	hWnd = CreateWindow(szWindowClass, TEXT("View3D"), Style,
		CW_USEDEFAULT, 0, MainFormWidth, MainFormHeight, swi->hOwner, NULL, hInstanceDLL, NULL);
	// проверка на создание окна
	if (!hWnd)
	{
		EnableWindow(swi->hOwner,TRUE);
		swi->Result = FALSE;
		return swi->Result;
	}

	if(!D3DInitialization(hWnd))
	{
		EnableWindow(swi->hOwner,TRUE);
		swi->Result = FALSE;
		return swi->Result;
	}

	if (!DialogBox(hInstanceDLL,MAKEINTRESOURCE(IDD_WORKER),swi->hOwner,ProgressBox))
	{
		swi->Result = FALSE;
		return swi->Result;
	}

	const TCHAR *COMMENT_BUFFER_MASK = 
#ifdef _DEBUG 		
		TEXT("DEBUG BUILD\nКоличество полигонов: %d");
#else
		TEXT("Количество полигонов: %d");
#endif

	_stprintf_s(COMMENT_BUFFER,COMMENT_BUFFER_SIZE,
	COMMENT_BUFFER_MASK,g_PrimitiveCount1 + g_PrimitiveCount2 + 
	g_PrimitiveCount3 + g_PrimitiveCount4);

	SendMessage(hWnd,WM_SETZPOS,g_FieldX,g_FieldY);
	SendMessage(hWnd,WM_UPDATECONFIG,0,0);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DrawScene();
		}
	}
	
	// освобождаем графические ресурсы
	D3DRelease();

	// разблокируем родителя
	EnableWindow(swi->hOwner,TRUE);

	DestroyWindow(hWnd);

	// отменяем регистрацию окна
	UnregisterClass(szWindowClass,hInstanceDLL);
	
	swi->Result = TRUE;
	return swi->Result;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	hInstanceDLL = hinstDLL;
	BOOL Result = TRUE;

	return Result;
}