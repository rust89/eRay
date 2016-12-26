

#include "eRay III.h"

#include <GdiPlus.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// инициализаци€ библиотеки компонентов
	INITCOMMONCONTROLSEX iccex = {sizeof(INITCOMMONCONTROLSEX),ICC_WIN95_CLASSES | ICC_COOL_CLASSES};
	InitCommonControlsEx(&iccex);

	// инициализаци€ графической библиотеки gdi+
	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input;
	if (Gdiplus::GdiplusStartup(&token, &input, NULL) != Gdiplus::Ok)
		return 0;

	//«апускаем приложение
	RApplication app;
	app.m_wndMainWindow = new RMainFrame;
	
	CmdParams params;
	app.GetCmdParams(params);
	if (params.Length() > 1)
	{
		((RMainFrame*)app.m_wndMainWindow)->OpenAfterLoad = true;
		((RMainFrame*)app.m_wndMainWindow)->ProjectFileName = params[1];
	}
	else
	{
		((RMainFrame*)app.m_wndMainWindow)->OpenAfterLoad = false;
		((RMainFrame*)app.m_wndMainWindow)->ProjectFileName = TEXT("");
	}
	LPCTSTR szWindowClassName = TEXT("eRayMainWindow");
	if (!RFrame::RegisterWndClass(szWindowClassName,CS_HREDRAW | CS_VREDRAW, NULL,LoadCursor(0,IDC_ARROW)))
	{
		return 0;
	}

	app.m_hMainAccel = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));

	app.m_wndMainWindow->Create(0,szWindowClassName,TEXT("eRay"),WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,NULL,CW_USEDEFAULT,CW_USEDEFAULT,960,600,0);
	app.Run();
	
	// освобождаем графическую библиотеку gdi+
	Gdiplus::GdiplusShutdown(token);
	return 0;
}