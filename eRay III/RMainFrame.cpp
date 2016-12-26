
#include "RMainFrame.h"

#include "RModelParamFrame.h"

CMetalInfo g_cMetalInfo;

RMainFrame::RMainFrame(void)
{
	Option3d = e3doNorm;
	SaveBeforExitPromt = false;
	ConfigPanelWidth = 200;
	InfoTextHeight = 150;
	MinSize.x = 480;
	MinSize.y = 320;
	AddMessage(WM_CREATE,MSG_EVENT_CAST(RMainFrame::OnCreate),this);
	AddMessage(WM_CLOSE,MSG_EVENT_CAST(RMainFrame::OnClose),this);
	AddMessage(WM_DESTROY,MSG_EVENT_CAST(RMainFrame::OnDestroy),this);
	AddMessage(WM_SETFOCUS,MSG_EVENT_CAST(RMainFrame::OnFocus),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RMainFrame::OnSize),this);
	AddMessage(WM_GETMINMAXINFO,MSG_EVENT_CAST(RMainFrame::OnMinMaxInfo),this);
	AddMessage(WM_COMMAND,MSG_EVENT_CAST(RMainFrame::OnCommand),this);
	AddMessage(WM_ERASEBKGND,MSG_EVENT_CAST(RMainFrame::OnErsBckg),this);
	AddMessage(WM_MENUSELECT,MSG_EVENT_CAST(RMainFrame::OnMenuSelect),this);
	AddMessage(WM_VSCROLL,MSG_EVENT_CAST(RMainFrame::OnChangeContrast),this);
	AddMessage(WM_CHANGE_RANGING,MSG_EVENT_CAST(RMainFrame::OnChangeRanging),this);
	AddMessage(WM_GET_TRANS_PARAMS,MSG_EVENT_CAST(RMainFrame::OnGetTransParams),this);
	AddMessage(WM_SET_TRANS_PARAMS,MSG_EVENT_CAST(RMainFrame::OnSetTransParams),this);
	AddMessage(WM_VIEW_ADD_PARAMS,MSG_EVENT_CAST(RMainFrame::OnViewAddParams),this);
	AddMessage(WM_VIEW_TEMP_MODEL,MSG_EVENT_CAST(RMainFrame::OnViewTemperatureModel),this);

#pragma message ("Тут временный код! не забудь меня!")
	{
	ExpressionError ExprErr;
	g_cMetalInfo.m_cLambdaLiq.Initialization(parser_string(PARSER_TEXT("37")),ExprErr);
	//g_cMetalInfo.m_cLambdaSol.Initialization(parser_string(PARSER_TEXT("33.71-5.173*(10^(-2))*t+4.814*(10^(-5))*(t^2)-1.07*(10^(-8))*(t^3)")),ExprErr);
	g_cMetalInfo.m_cLambdaSol.Initialization(parser_string(PARSER_TEXT("33.71")),ExprErr);

	//g_cMetalInfo.m_cDensityLiq.Initialization(parser_string(PARSER_TEXT("4120-0.42*(t-1944)")),ExprErr);
	g_cMetalInfo.m_cDensityLiq.Initialization(parser_string(PARSER_TEXT("4120")),ExprErr);
	//g_cMetalInfo.m_cDensitySol.Initialization(parser_string(PARSER_TEXT("4565.5-0.1525*t")),ExprErr);
	g_cMetalInfo.m_cDensitySol.Initialization(parser_string(PARSER_TEXT("4565.5")),ExprErr);

	g_cMetalInfo.m_cTempCapLiq.Initialization(parser_string(PARSER_TEXT("940.1")),ExprErr);
	g_cMetalInfo.m_cTempCapSol.Initialization(parser_string(PARSER_TEXT("700")),ExprErr);

	g_cMetalInfo.m_fTemperatureLiq = 1936.0f;
	g_cMetalInfo.m_fTemperatureSol = 1930.0f;

	g_cMetalInfo.m_fBlackness = 0.70f;
	g_cMetalInfo.m_fFusion = 358000.0f;
	g_cMetalInfo.m_fViscosity = 1.26;

	g_cMetalInfo.m_fX = 1.80f;
	g_cMetalInfo.m_fY = 0.50f;
	g_cMetalInfo.m_fZ = 0.090f;

	g_cMetalInfo.m_fJ = 0.068f;
	g_cMetalInfo.m_fG = 0.5f;
	g_cMetalInfo.m_fEfficiency = 0.780f;

	}
}

RMainFrame::~RMainFrame(void)
{
	
}

RString RMainFrame::GetCurTime()
{
	SYSTEMTIME SysTime;
	
	GetLocalTime(&SysTime);
	const int BuffSize = 10;
	TCHAR Buff[BuffSize];
	if (_stprintf_s(Buff,BuffSize,TEXT("%.2d:%.2d:%.2d"),SysTime.wHour, SysTime.wMinute, SysTime.wSecond) > 0)
	{
		return RString(Buff);
	}
	return TEXT("");
}

bool RMainFrame::InitControlImages()
{
	// загружаем изображения и прикручиваем их к контролам
	const int BtnNum = 7;
	if (!ImageList.Create(16, 15, ILC_COLOR24 | ILC_MASK, BtnNum, 0))
		return false;

	ToolBar.BindingImageList(ImageList);
	
	TBBUTTON tbButtons[BtnNum] = 
	{
		{ImageList.AddMasked(LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(IDB_FILE_OPEN)),RGB(192,192,192)),
			FILE_OPEN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, 0},
		{ImageList.AddMasked(LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(IDB_FILE_SAVE)),RGB(192,192,192)), 
			FILE_SAVE, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, 0},
		{ImageList.AddMasked(LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(IDB_FILE_SAVEI)),RGB(192,192,192)), 
			FILE_SAVE_IMAGE, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, 0},
		{-1, 0, 0, BTNS_SEP, {0}, 0, 0},
		{ImageList.AddMasked(LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(IDB_MODEL_RUN)),RGB(192,192,192)), 
			MODEL_START, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, 0},
		{ImageList.AddMasked(LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(IDB_MODEL_RUN3D)),RGB(192,192,192)), 
			MODEL_RUN_3D, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, 0},
		{-1, 0, 0, BTNS_SEP, {0}, 0, 0}
    };

    SendMessage(ToolBar.Handle(), TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(ToolBar.Handle(), TB_ADDBUTTONS, (WPARAM)BtnNum, (LPARAM)&tbButtons);

	return true;
}

void RMainFrame::AdjustWindowsLayout(int ClientWidth, int ClientHeight)
{
	// установка позиции статус бара
	int StatusBarHeight = MainStatusBar.GetHeight();
	MainStatusBar.AutoSize();
	
	int StatusBarWidth = MainStatusBar.GetWidth();
	INT PartMas[] = {(INT)(0.6*(StatusBarWidth)),INT(0.6*(StatusBarWidth)) + (INT)(0.3*(StatusBarWidth)),-1};
	MainStatusBar.SetParts(PartMas,sizeof(PartMas)/sizeof(INT));
	
	// установка позиции тул бара
	ToolBar.AutoSize();

	int ComboHeight = ToolBar.GetBtnHeight() - 8;
	ComboSwitch.SetWindowPosition(NULL,ToolBar.GetItemWidth(),2,100,ComboHeight,SWP_NOACTIVATE);

	int ToolHeight = ToolBar.GetHeight();
	// установка позиции окна конфигурации
	ConfigPanel.SetWindowPosition(NULL,0,ToolHeight, ConfigPanelWidth, ClientHeight - StatusBarHeight - ToolHeight, SWP_NOACTIVATE);
	// установка позиции окна сообщения
	InfoList.SetWindowPosition(NULL,ConfigPanelWidth,ClientHeight - StatusBarHeight - InfoTextHeight,ClientWidth - ConfigPanelWidth,InfoTextHeight,SWP_NOACTIVATE);
	
	int GraphicOutHight = ClientHeight - StatusBarHeight - InfoTextHeight - ToolHeight;
	int RangingHeight = GraphicOutHight >> 1; // попалам от оставшейся области 
	int RangingWidth = (int)(0.4*(ClientWidth - ConfigPanelWidth));
	if (RangingWidth >= RangingHeight) RangingWidth = RangingHeight;
	RangingByX.SetWindowPosition(NULL,ClientWidth - RangingWidth, ToolHeight,RangingWidth,RangingHeight,SWP_NOACTIVATE);
	RangingByY.SetWindowPosition(NULL,ClientWidth - RangingWidth, ToolHeight + RangingHeight,RangingWidth,RangingHeight,SWP_NOACTIVATE);

	// установка позиции трек бара
	int TrackWidth = 30;
	ContrastTrack.SetWindowPosition(NULL,ClientWidth - TrackWidth - RangingWidth,ToolHeight,TrackWidth,GraphicOutHight, SWP_NOACTIVATE);

	// установка позиции окна графического вывода
	ImagePanel.SetWindowPosition(NULL,ConfigPanelWidth,ToolHeight, ClientWidth - ConfigPanelWidth - TrackWidth - RangingWidth, GraphicOutHight, SWP_NOACTIVATE);
}

LRESULT RMainFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
	HICON hIcon = LoadIcon(ModuleHandle(),MAKEINTRESOURCE(IDI_LOGO));
	if (hIcon)
	{
		SetBigIcon(hIcon);
		SetSmallIcon(hIcon);
	}
	
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINMENU));
	if (hMenu) SetMenu(m_hHandle,hMenu);
	
	// Статус бар
	if (!MainStatusBar.Create(TEXT(""),this,1))
		return -1;
	MainStatusBar.SetSimple(false);

	// тул бар
	if (!ToolBar.Create(TEXT(""),this,1))
		return -1;
	
	// трек бар
	if (!ContrastTrack.Create(TEXT(""),this,id_Track))
		return -1;
	ContrastTrack.SetStyle(ContrastTrack.GetStyle() | TBS_VERT | TBS_AUTOTICKS);
	ContrastTrack.SetRange(0,25);
	ContrastTrack.SetTrackPos(25);

	// переключатель видов
	if (!ComboSwitch.Create(TEXT(""),this,id_Combo))
		return -1;
	ComboSwitch.SetParent(&ToolBar);
	ComboSwitch.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	ComboSwitch.SetSelectedIndex(ComboSwitch.AddString(TEXT("Градиент"),(LPVOID)ImagePanel.GraphView.HotGrad));
	ComboSwitch.AddString(TEXT("Радуга"),(LPVOID)ImagePanel.GraphView.HotNonGrad);
	ComboSwitch.AddString(TEXT("Классик"),(LPVOID)ImagePanel.GraphView.Classic);

	// проекции
	LPCTSTR szRangingClassName = TEXT("RangingByX");
	if (!RFrame::RegisterWndClass(szRangingClassName,CS_HREDRAW | CS_VREDRAW, NULL,LoadCursor(0,IDC_ARROW)))
		return -1;
	if (!RangingByX.Create(0,szRangingClassName,TEXT(""),WS_VISIBLE | WS_CHILD | WS_BORDER,this,0,0,0,0,0))
		return -1;
	RangingByX.SetLegendText(TEXT("Срез на X-Z"));
	RangingByX.SetHorText(TEXT("X"));
	if (!RangingByY.Create(0,szRangingClassName,TEXT(""),WS_VISIBLE | WS_CHILD | WS_BORDER,this,0,0,0,0,0))
		return -1;
	RangingByY.SetLegendText(TEXT("Срез на Y-Z"));
	RangingByY.SetHorText(TEXT("Y"));

	// создание панели конфигурации
	LPCTSTR szConfigClassName = TEXT("ConfigPanel");
	if (!RFrame::RegisterWndClass(szConfigClassName,CS_HREDRAW | CS_VREDRAW, CreateSolidBrush(GetSysColor(COLOR_BTNFACE)),LoadCursor(0,IDC_ARROW)))
		return -1;
	if (!ConfigPanel.Create(0,szConfigClassName,TEXT(""),WS_VISIBLE | WS_CHILD,this,0,0,0,0,0))
		return -1;

	// создание панели отрисовки
	LPCTSTR szImageClassName = TEXT("ImagePanel");
	if (!RFrame::RegisterWndClass(szImageClassName,CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, CreateSolidBrush(GetSysColor(COLOR_BTNFACE)),LoadCursor(0,IDC_ARROW)))
		return -1;
	if (!ImagePanel.Create(0,szImageClassName,TEXT(""),WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL,this,0,0,0,0,0))
		return -1;


	if (!InfoList.Create(this,0,0,0,0,0))
		return -1;
	InfoList.SetStyle(InfoList.GetStyle() | WS_BORDER);
	InfoList.SetListViewState(RListView::lvsReport);
	InfoList.SetFullRowSelect(TRUE);

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if (osvi.dwMajorVersion > 5) // начиная с NT 6.0 поддержка групп 
	{	
		InfoList.SetGroupEnable(TRUE);
	}
	InfoList.CreateColumn(TEXT("Время"),80);
	InfoList.CreateColumn(TEXT("Сообщение"),400);
	InfoList.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	InfoList.SetExplorerStyle();

	InitControlImages();

	if (OpenAfterLoad)
		if (!LoadProject(ProjectFileName.c_str())) ProjectFileName.clear(); 

	return 0;
}


LRESULT RMainFrame::OnClose(WPARAM wParam, LPARAM lParam)
{
	if (!SaveBeforExitPromt) return OnDestroy(0,0);
	int MsgBoxCode = MessageBox(this->Handle(),TEXT("Сохранить текущий проект?"),TEXT("Завершение работы"),MB_YESNOCANCEL | MB_ICONQUESTION);
	switch(MsgBoxCode)
	{
		case IDYES:
			if (ProjectFileName.length())
			{
				SaveProject(ProjectFileName.c_str());
			}
			else
			{
				this->OnMenuSaveAs(0,0,0);
			}
			OnDestroy(0,0);
			break;
		case IDNO:
			OnDestroy(0,0);
			break;
		case IDCANCEL:
			break;
	}
	return 0;
}

LRESULT RMainFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return DefWindowProc(m_hHandle,WM_DESTROY,wParam,lParam);
}

LRESULT RMainFrame::OnSize(WPARAM wParam, LPARAM lParam)
{
	AdjustWindowsLayout(LOWORD(lParam),HIWORD(lParam));
	return 0;
}

LRESULT RMainFrame::OnErsBckg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT RMainFrame::OnMenuSelect(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
		case FILE_OPEN:
			MainStatusBar.SetPanelText(0,TEXT("Открыть ранее сохранённый проект"));
			break;
		case FILE_SAVE:
			MainStatusBar.SetPanelText(0,TEXT("Сохранить текущий проект"));
			break;
		case FILE_SAVE_AS:
			MainStatusBar.SetPanelText(0,TEXT("Сохранить проект под новым именем"));
			break;
		case FILE_SAVE_IMAGE:
			MainStatusBar.SetPanelText(0,TEXT("Сохранить изображение в формате *.bmp"));
			break;
		case FILE_CLOSE_APP:
			MainStatusBar.SetPanelText(0,TEXT("Закрыть приложение"));
			break;
		case MODEL_VAR_EDIT:
			MainStatusBar.SetPanelText(0,TEXT("Добавить новые или редактировать уже имеющиеся переменные"));
			break;
		case MODEL_START:
			MainStatusBar.SetPanelText(0,TEXT("Запустить моделирование ситуации"));
			break;
		case MODEL_RUN_3D:
			MainStatusBar.SetPanelText(0,TEXT("Запустить трёхмерную модель"));
			break;
		case MODEL_CONFIG:
			MainStatusBar.SetPanelText(0,TEXT("Конфигурация параметров отображения"));
			break;
		case HELP_ABOUT:
			MainStatusBar.SetPanelText(0,TEXT("Просмотр сведений о программе и её компонентах"));
			break;
		case HELP_GETHELP:
			MainStatusBar.SetPanelText(0,TEXT("Получение информации о принципах работы с программой"));
			break;
		default:
			MainStatusBar.SetPanelText(0,TEXT(""));
	}
	return (LRESULT) 0;
}

LRESULT RMainFrame::OnMinMaxInfo(WPARAM wParam, LPARAM lParam)
{
	((LPMINMAXINFO)lParam)->ptMinTrackSize = MinSize;
	return 0;
}

LRESULT RMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int ControlID = LOWORD(wParam);
	switch(ControlID)
	{
		case FILE_OPEN:
			return OnMenuOpen((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case FILE_SAVE:
			return OnMenuSave((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case FILE_SAVE_AS:
			return OnMenuSaveAs((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case FILE_CLOSE_APP:
			return OnMenuClose((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_START:
			return OnMenuStart((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_CONFIG:
			return OnConfig((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case HELP_GETHELP:
			return OnMenuGetHelp((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case HELP_ABOUT:
			return OnMenuAbout((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case FILE_SAVE_IMAGE:
			return OnImageSave((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case id_Combo:
			return OnViewSwitch((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_VAR_EDIT:
			return OnVarEditor((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_RUN_3D:
			return OnRun3d((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_EX_RING:
			return OnExampleRing((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_EX_SIN:
			return OnExampleSin((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_EX_SPIR:
			return OnExampleSpir((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_EX_KREN:
			return OnExampleKren((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
		case MODEL_EX_ROME:
			return OnExampleRose((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
	}
	return 0;
}

LRESULT RMainFrame::OnMenuSave(HWND hControl, WORD cID, WORD Notify)
{
	if (ProjectFileName.length())
		SaveProject(ProjectFileName.c_str());
	else OnMenuSaveAs(hControl,0,0);
	return 0;
}

LRESULT RMainFrame::OnMenuSaveAs(HWND hControl, WORD cID, WORD Notify)
{
	OPENFILENAME ofn;
	TCHAR szFile[1024];
	LPCTSTR szFilter = TEXT("eRay project\0*.erp\0\0");
	LPCTSTR szDefExt = TEXT("erp");
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	
	szFile[0] = TEXT('\0'); 

	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = m_hHandle; 
	ofn.lpstrFilter = szFilter; 
	ofn.lpstrFile= szFile; 
	ofn.lpstrDefExt = szDefExt;
	ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile); 
	ofn.Flags = OFN_OVERWRITEPROMPT; 
 
	if (!GetSaveFileName(&ofn))
	{
		return 0;	
	}

	SaveProject(ofn.lpstrFile);
	return 0;
}

LRESULT RMainFrame::OnMenuOpen(HWND hControl, WORD cID, WORD Notify)
{
	OPENFILENAME ofn;
	TCHAR szFile[1024];
	LPCTSTR szFilter = TEXT("eRay project\0*.erp\0\0");
	LPCTSTR szDefExt = TEXT("erp");
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	
	szFile[0] = TEXT('\0'); 

	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = m_hHandle; 
	ofn.lpstrFilter = szFilter; 
	ofn.lpstrFile= szFile; 
	ofn.lpstrDefExt = szDefExt;
	ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile); 
	ofn.Flags = OFN_OVERWRITEPROMPT; 
 
	if (!GetOpenFileName(&ofn))
	{
		return 0;	
	}
	LoadProject(ofn.lpstrFile);
	return 0;
}

LRESULT RMainFrame::OnConfig(HWND hControl, WORD cID, WORD Notify)
{
	DialogBox(this->ModuleHandle(),MAKEINTRESOURCE(IDD_CONFIG),this->Handle(),ConfigBox);
	return 0;
}

LRESULT RMainFrame::OnMenuGetHelp(HWND hControl, WORD cID, WORD Notify)
{
	RString HelpMsg;
	HelpMsg += TEXT("Поддерживаемые функции:\n");
	HelpMsg += TEXT("	- sin(x) - синус числа x\n");
	HelpMsg += TEXT("	- cos(x) - косинус числа x\n");
	HelpMsg += TEXT("	- tan(x) - тангенс числа x\n");
	HelpMsg += TEXT("	- ctn(x) - катангенс числа x\n");
	HelpMsg += TEXT("	- sqrt(x) - квадратный корень числа x\n");
	HelpMsg += TEXT("	- pow(x, y) - возведение x в степень y\n");
	HelpMsg += TEXT("	- abs(x) - модуль числа x\n");
	HelpMsg += TEXT("	- exp(x) - экспонента x\n");
	HelpMsg += TEXT("	- log(x, y) - логарифм x по основанию y\n");
	HelpMsg += TEXT("	- ln(x) - натуральный логарифм\n");
	HelpMsg += TEXT("	- sinh(x) - гиперболический синус\n");
	HelpMsg += TEXT("	- cosh(x) - гиперболический косинус\n");
	HelpMsg += TEXT("	- tanh(x) - гиперболический тангенс\n");
	HelpMsg += TEXT("	- ctnh(x) - гиперболический катангенс\n");
	MessageBox(m_hHandle,HelpMsg.c_str(),TEXT("Справка"),MB_OK | MB_ICONINFORMATION);
	//ShellExecute(NULL,TEXT("open"),TEXT("help.chm"),NULL,NULL,SW_SHOWDEFAULT);
	return 0;
}

LRESULT RMainFrame::OnMenuAbout(HWND hControl, WORD cID, WORD Notify)
{
	DialogBox(ModuleHandle(),MAKEINTRESOURCE(IDD_ABOUT),this->Handle(),AboutBoxProc);
	return 0;
}

void RMainFrame::AddMenuIcon(HMENU hMenu, UINT mID, UINT bID)
{
	MENUITEMINFO mii;
	ZeroMemory(&mii,sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	if (GetMenuItemInfo(hMenu,mID,FALSE,&mii))
	{
		mii.fMask |= MIIM_BITMAP;
		mii.hbmpItem = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(bID));
		SetMenuItemInfo(hMenu,mID,FALSE,&mii);
	}
}

LRESULT RMainFrame::OnMenuClose(HWND hControl, WORD cID, WORD Notify)
{
	OnDestroy(0,0);
	return 0;
}

LRESULT RMainFrame::OnViewSwitch(HWND hControl, WORD cID, WORD Notify)
{
	if (Notify == CBN_SELCHANGE)
	{
		LPVOID Data = ComboSwitch.GetItemData(ComboSwitch.GetSelectedIndex());
		ImagePanel.GraphView.GraphType = (GraphicManager::GraphTypes)(INT)Data;
		if (ImagePanel.GraphView.GraphType != GraphicManager::HotGrad)
			ContrastTrack.SetEnable(FALSE);
		else
			ContrastTrack.SetEnable(TRUE);
		ImagePanel.Refresh();
	}	
	return 0;
}

LRESULT RMainFrame::OnChangeContrast(WPARAM wParam, LPARAM lParam)
{
	static int Contr = 0;
	if (lParam == (LPARAM)ContrastTrack.Handle())
	{
		switch(LOWORD(wParam))
		{
			case TB_ENDTRACK:
			{
				if (!ImagePanel.GraphView.CanDraw()) break;
				WORD tmp = LOWORD(wParam);
				int Pos = (25 - ContrastTrack.GetTrackPos()) << 3;
				if (Pos == Contr) return 0;
				Contr = Pos;
				ImagePanel.GraphView.AddContrast(Pos);
				ImagePanel.Refresh();
				break;
			}
		}
	}
	return 0;
}

LRESULT RMainFrame::OnImageSave(HWND hControl, WORD cID, WORD Notify)
{
	OPENFILENAME ofn;
	TCHAR szFile[1024];
	LPCTSTR szFilter = TEXT("Bitmap Files\0*.bmp\0\0");
	LPCTSTR szDefExt = TEXT("bmp");
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	
	szFile[0] = TEXT('\0'); 

	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = m_hHandle; 
	ofn.lpstrFilter = szFilter; 
	ofn.lpstrFile= szFile; 
	ofn.lpstrDefExt = szDefExt;
	ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile); 
	ofn.Flags = OFN_OVERWRITEPROMPT; 
 
	if (GetSaveFileName(&ofn))
	{
		ImagePanel.GraphView.CaptureImage(ofn.lpstrFile);
	}
	return 0;
}

LRESULT RMainFrame::OnMenuStart(HWND hControl, WORD cID, WORD Notify)
{
	DWORD dwThreadID;
	HANDLE hThread;
	hThread = CreateThread(NULL,0,BuildThread,this,CREATE_SUSPENDED,&dwThreadID);
	if (hThread)
	{
		ImagePanel.ClearImage();
		ImagePanel.Refresh();
		LockInterface();
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
	return 0;
}

bool RMainFrame::GetVariable(Variables &var, OutMsg &MsgList)
{
	// очищаем предыдущие переменные
	var.clear();

	for (std::size_t i = 0; i < ConfigPanel.VarList.size(); i++)
	{
		Expression CustomExpression;
		ExpressionError ExprErr;
		// синтаксический анализ
		if (!CustomExpression.Initialization(ConfigPanel.VarList[i].Value,ExprErr))
		{
			MsgList.push_back(ConfigPanel.VarList[i].Name + TEXT(": ") + RString((LPTSTR)ExprErrorToStr(ExprErr).c_str()));
			return false;
		}

		double Result;
		if (!CustomExpression.GetValue(Result,var,ExprErr))
		{
			MsgList.push_back(ConfigPanel.VarList[i].Name + TEXT(": ") + RString((LPTSTR)ExprErrorToStr(ExprErr).c_str()));
			return false;
		}

		var.push_back(VarItem(ConfigPanel.VarList[i].Name,Result));
	}
	return true;
}

bool RMainFrame::GetParamValue(double **Var, int *VarCount, OutMsg &MsgList)
{
	double vBegin;
	long vStepCount;
	double vEnd;
	
	// получаем значение начала интервала
	parser_string vBeginText = ConfigPanel.TBegin.GetText();
	bool Result = true;
	if (!vBeginText.size()) 
	{
		// Пустое окно
		MsgList.push_back(TEXT("Заполните поле \"Начало интервала\""));
		Result = false;
	}
	else
	{
		if (!parser_converter::try_to_double(vBeginText,vBegin))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать ")) + RString((LPTSTR)vBeginText.c_str()) + RString(TEXT(" в число!")));
			Result = false;
		}
	}
	// получаем значение количества шагов интервала
	parser_string vStepCountText = ConfigPanel.TStep.GetText();
	if (!vStepCountText.size())
	{
		// Пустое окно
		MsgList.push_back(TEXT("Заполните поле \"Количество шагов\""));
		Result = false;
	}
	else
	{
		if (!parser_converter::try_to_integer(vStepCountText,vStepCount))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать ")) + RString((LPTSTR)vStepCountText.c_str()) + RString(TEXT(" в число!")));
			Result = false;
		}
		if (vStepCount <= 0)
		{
			MsgList.push_back(TEXT("Значение поля \"Количество шагов\" должно быть целым положительным числом!"));
			Result = false;
		}
	}
	// получаем значение конца интервала
	parser_string vEndText = ConfigPanel.TEnd.GetText();
	if (!vEndText.size())
	{
		// Пустое окно
		MsgList.push_back(TEXT("Заполните поле \"Конец интервала\""));
		Result = false;
	}
	else
	{
		if (!parser_converter::try_to_double(vEndText,vEnd))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать ")) + RString((LPTSTR)vEndText.c_str()) + RString(TEXT(" в число!")));
			Result = false;
		}
	}
	if (!Result) return Result;
	// заполняем интервал
	double vStep = (vEnd - vBegin)/vStepCount;
	*Var = new double[vStepCount];
	*VarCount = vStepCount;
	for (int i = 0; i < vStepCount; i++)
	{
		(*Var)[i] = vBegin + i*vStep;
	}
	return true;
}

DWORD WINAPI RMainFrame::BuildThread(LPVOID Param)
{
	RMainFrame *This = (RMainFrame*)Param;

	This->RangingByX.ClearParams();
	This->RangingByX.Refresh();
	This->RangingByY.ClearParams();
	This->RangingByY.Refresh();

	This->ImagePanel.ResetSelector();
	This->ContrastTrack.SetTrackPos(25);

	This->InfoList.CollapseAllGroup();
	RString GroupCaption = RString(TEXT("Построение проекта (") + This->GetCurTime() + TEXT(") \"")) + 
		(This->ProjectFileName.empty() ? RString(TEXT("Без имени")) : This->ProjectFileName) + RString(TEXT("\""));
	int GroupID = This->AddEventGroup(GroupCaption.c_str());

	ModelingParams ModParams;
	OutMsg MsgList;
//	int LastIndex;
	
	bool Result = This->ParseModelingParams(&ModParams, MsgList);

	if (!Result)
	{
		RString tmpStr = RString(TEXT("При построении возникли следующие проблемы (")) + RConverter::ToString((long)MsgList.size()) + RString(TEXT("):"));
		This->AddEventMsg(true,tmpStr.c_str(),GroupID);
		for (size_t i = 0; i < MsgList.size(); i++)
		{
			tmpStr = RConverter::ToString((long)i+1) + RString(TEXT(". ")) + MsgList[i];
			This->AddEventMsg(false,tmpStr.c_str(),GroupID);
		}
		This->UnLockInterface();
		return 0;
	}

	This->AddEventMsg(true,TEXT("Генерация точек..."),GroupID);

	This->ImagePanel.GraphView.ClearXY();
	for (size_t i = 0; i < (unsigned)ModParams.TParamsCount; i++)
	{
		Variables vars;

		// включаем в переменные t-параметр
		vars.push_back(VarItem(TEXT("t"),ModParams.TParams[i]));

		// включаем в переменные дополнительные переменные
		vars.insert(vars.end(),ModParams.Vars.begin(),ModParams.Vars.end());
		double Result1, Result2;
		ExpressionError ExprErr;

		if (!ModParams.ExprY.GetValue(Result1,vars,ExprErr))
		{
			if (IsCalcError(ExprErr.ErrorCode))
			{
				This->ImagePanel.GraphView.AddHole();
				continue;	
			}
			// далее отрабатывает ошибка			
			This->AddEventMsg(false,RString(TEXT("Выражение Y : ") + ExprErrorToStr(ExprErr)).c_str(),GroupID);

			This->ImagePanel.GraphView.ClearXY();
			This->InfoList.ScrollDown();
			This->UnLockInterface();
			return 0;
		}
		if (!ModParams.ExprX.GetValue(Result2,vars,ExprErr))
		{
			if (IsCalcError(ExprErr.ErrorCode))
			{
				This->ImagePanel.GraphView.AddHole();
				continue;	
			}
			// далее отрабатывает ошибка
			This->AddEventMsg(false,RString(TEXT("Выражение X : ") + ExprErrorToStr(ExprErr)).c_str(),GroupID);

			This->ImagePanel.GraphView.ClearXY();
			This->InfoList.ScrollDown();
			This->UnLockInterface();
			return 0;
		}
		This->ImagePanel.GraphView.AddXY(Result2,Result1);
	}

	// установка параметров моделирования
	RECT DrawRect = {0,0,ModParams.XRes,ModParams.YRes};
	This->ImagePanel.GraphView.SetDrawRegion(&DrawRect);
	This->ImagePanel.GraphView.SetRayRadius(ModParams.RayRadius);
	This->ImagePanel.GraphView.SetRayVoltage(ModParams.RayVoltage);
	This->ImagePanel.GraphView.SetRayAmpTraffic(g_cMetalInfo.m_fJ);
	This->ImagePanel.GraphView.CloudGr.SetTotalEfficiency(g_cMetalInfo.m_fEfficiency);
	This->ImagePanel.GraphView.GraphType = (GraphicManager::GraphTypes)(INT)This->ComboSwitch.GetItemData(This->ComboSwitch.GetSelectedIndex());
	if (This->ImagePanel.GraphView.DrawInRegion = ModParams.UseRegion)
	{
		frectangle RectRegion( ModParams.MinX, ModParams.MinY, ModParams.MaxX, ModParams.MaxY);
		This->ImagePanel.GraphView.SetGraphRegion(&RectRegion);
	}

	This->AddEventMsg(true,TEXT("Расчёт параметров..."),GroupID);

	if (!This->ImagePanel.GraphView.CalculateParams())
	{
		This->AddEventMsg(false,TEXT("Ошибка генерации параметров модели. Сохраните проект и отправьте его разработчику"),GroupID);
		This->UnLockInterface();
		return 0;
	}

	This->AddEventMsg(true,TEXT("Построение изображения..."),GroupID);
	HDC hdc = GetDC(This->ImagePanel.Handle());
	if (!This->ImagePanel.GraphView.ReBuildAll(hdc))
	{
		This->AddEventMsg(false,TEXT("Ошибка генерации параметров модели. Сохраните проект и отправьте его разработчику"),GroupID);
		This->UnLockInterface();
		DeleteDC(hdc);
		return 0;
	}
	DeleteDC(hdc);

	This->AddEventMsg(true,TEXT("Построение проекта успешно завершено!"),GroupID);

	RString ResolutionString = RConverter::ToString(ModParams.XRes) + TEXT("x") + RConverter::ToString(ModParams.YRes);
	This->MainStatusBar.SetPanelText(1,ResolutionString.c_str());

	RECT ImagePanelRect;
	This->ImagePanel.GetClientRect(ImagePanelRect);
	This->ImagePanel.UpDateScroll(ImagePanelRect.right,ImagePanelRect.bottom);

	This->ImagePanel.Refresh();
	This->UnLockInterface();
	return 0;
}

bool RMainFrame::SaveProject(LPCTSTR FileName)
{
	// копируем y
	Document.set_expr_y(ConfigPanel.ExprY.GetText().c_str());
	// копируем x
	Document.set_expr_x(ConfigPanel.ExprX.GetText().c_str());
	// копируем переменные
	doc_var_list tmp_var_list;
	for (std::size_t i = 0; i < ConfigPanel.VarList.size(); i++)
		tmp_var_list.push_back(std::make_pair(ConfigPanel.VarList[i].Name,ConfigPanel.VarList[i].Value));
	Document.set_var_list(&tmp_var_list);
	// копируем TBegin
	Document.set_t_begin(ConfigPanel.TBegin.GetText().c_str());
	// копируем TStep
	Document.set_t_stepcount(ConfigPanel.TStep.GetText().c_str());
	// копируем TEnd
	Document.set_t_end(ConfigPanel.TEnd.GetText().c_str());
	// копируем Радиус
	Document.set_ray_radius(ConfigPanel.RayRadius.GetText().c_str());
	// копируем Напряжение
	Document.set_ray_voltage(ConfigPanel.RayVoltage.GetText().c_str());
	// копируем ResX
	Document.set_resolution_by_x(ConfigPanel.ModelWidth.GetText().c_str());
	// копируем ResY
	Document.set_resolution_by_y(ConfigPanel.ModelHeight.GetText().c_str());
	// копируем MinX
	Document.set_region_min_x(ConfigPanel.MinX.GetText().c_str());
	// копируем MaxX
	Document.set_region_max_x(ConfigPanel.MaxX.GetText().c_str());
	// копируем MaxY
	Document.set_region_max_y(ConfigPanel.MaxY.GetText().c_str());
	// копируем MinY
	Document.set_region_min_y(ConfigPanel.MinY.GetText().c_str());
	// копируем флаг использования региона
	Document.set_use_region(ConfigPanel.BoundCheck.GetCheck());
	// копируем параметры
	ConfigParams tmp;
	InitConfigParam(&tmp);
	Document.set_configuration(&tmp);
	Document.set_metalinfo(&g_cMetalInfo);
	// Сохранение
	Document.SaveToFile(FileName);

	SetText(FileName);
	ProjectFileName.assign(FileName);
	return true;
}

bool RMainFrame::LoadProject(LPCTSTR FileName)
{
	if (!Document.LoadFromFile(FileName))
	{
		//InsertMessage(TEXT("Не удалось загрузить проект: \"") + RString(FileName) + TEXT("\""));
		return false;
	}

	// загружаем выражения
	ConfigPanel.ExprX.SetText(Document.get_expr_x().c_str());
	ConfigPanel.ExprY.SetText(Document.get_expr_y().c_str());

	// загружаем переменные
	doc_var_list tmp_var_list;
	Document.get_var_list(&tmp_var_list);
	ConfigPanel.VarList.clear();
	for (std::size_t i = 0; i < tmp_var_list.size(); i++)
		ConfigPanel.VarList.push_back(VarEditorUnit(tmp_var_list[i].first,tmp_var_list[i].second));
	ConfigPanel.UpdateVarList();

	// загружаем параметры интервала
	ConfigPanel.TBegin.SetText(Document.get_t_begin().c_str());
	ConfigPanel.TStep.SetText(Document.get_t_stepcount().c_str());
	ConfigPanel.TEnd.SetText(Document.get_t_end().c_str());

	// загружаем параметры луча
	ConfigPanel.RayRadius.SetText(Document.get_ray_radius().c_str());
	ConfigPanel.RayVoltage.SetText(Document.get_ray_voltage().c_str());

	// загружаем данные о высоте картинки
	ConfigPanel.ModelWidth.SetText(Document.get_resolution_by_x().c_str());
	ConfigPanel.ModelHeight.SetText(Document.get_resolution_by_y().c_str());

	// загружаем данные о параметрах региона построения
	BOOL RegionCheck(Document.get_use_region());
	ConfigPanel.BoundCheck.SetCheck(RegionCheck ?  BST_CHECKED : BST_UNCHECKED);
	ConfigPanel.MinX.SetText(Document.get_region_min_x().c_str()); ConfigPanel.MinX.SetEnable(RegionCheck);
	ConfigPanel.MaxX.SetText(Document.get_region_max_x().c_str()); ConfigPanel.MaxX.SetEnable(RegionCheck);
	ConfigPanel.MinY.SetText(Document.get_region_min_y().c_str()); ConfigPanel.MinY.SetEnable(RegionCheck);
	ConfigPanel.MaxY.SetText(Document.get_region_max_y().c_str()); ConfigPanel.MaxY.SetEnable(RegionCheck);

	// загружаем данные о конфигурации программы
	ConfigParams tmp;
	Document.get_configuration(&tmp);
	ApplyConfigParam(&tmp);

	Document.get_metalinfo(&g_cMetalInfo);

	SetText(FileName);
	ProjectFileName.assign(FileName);
	//InsertMessage(TEXT("Загружен проект: \"") + RString(FileName) + TEXT("\""));
	return true;
}

LRESULT RMainFrame::OnChangeRanging(WPARAM wParam, LPARAM lParam)
{
	int BeginSel, EndSel;
	int Indent = ImagePanel.GraphView.GetGraphicIndent();
	// создаём передаточный массив
	RArray <double> field(ImagePanel.GraphView.GetImageWidth() - 2 * Indent);
	// заполняем передаточный массив вдоль <y>
	for (int i = 0; i < field.Length(); i++)
	{
		field[i] = ImagePanel.GraphView.CloudGr.OutRanging[i + Indent][ImagePanel.PointY];
	}
	// получаем выделенные квадрат
	int QuadIndexByX = (int)(ImagePanel.PointX / ImagePanel.GraphView.CloudGr.PixelInOneStep);
	BeginSel = QuadIndexByX*ImagePanel.GraphView.CloudGr.PixelInOneStep;
	EndSel = BeginSel + ImagePanel.GraphView.CloudGr.PixelInOneStep;
	int ImageWidth = ImagePanel.GraphView.GetImageWidth();
	if (EndSel >= ImageWidth)
		EndSel = ImageWidth;
	RangingByX.CreateImage(&field, BeginSel, EndSel);
	RangingByX.Refresh();


	field.Resize(ImagePanel.GraphView.GetImageHeight() - 2 * Indent);
	int tmp = ImagePanel.GraphView.GetImageHeight() - Indent;
	for (int i = 0; i < field.Length(); i++)
	{
		field[i] = ImagePanel.GraphView.CloudGr.OutRanging[ImagePanel.PointX][tmp - i - 1];
	}

	int QuadIndexByY = (int)(ImagePanel.PointY / ImagePanel.GraphView.CloudGr.PixelInOneStep);
	BeginSel = QuadIndexByY*ImagePanel.GraphView.CloudGr.PixelInOneStep;
	EndSel = BeginSel + ImagePanel.GraphView.CloudGr.PixelInOneStep;
	int ImageHeight = ImagePanel.GraphView.GetImageHeight();
	if (EndSel >= ImageHeight)
		EndSel = ImageHeight;

	RangingByY.CreateImage(&field, BeginSel, EndSel);
	RangingByY.Refresh();
	return 0;
}

LRESULT RMainFrame::OnExampleRing(HWND hControl, WORD cID, WORD Notify)
{
	ConfigPanel.SetExamples(0);
	return 0;
}
LRESULT RMainFrame::OnExampleSin(HWND hControl, WORD cID, WORD Notify)
{
	ConfigPanel.SetExamples(1);
	return 0;
}
LRESULT RMainFrame::OnExampleSpir(HWND hControl, WORD cID, WORD Notify)
{
	ConfigPanel.SetExamples(2);
	return 0;
}
LRESULT RMainFrame::OnExampleKren(HWND hControl, WORD cID, WORD Notify)
{
	ConfigPanel.SetExamples(3);
	return 0;
}

LRESULT RMainFrame::OnExampleRose(HWND hControl, WORD cID, WORD Notify)
{
	ConfigPanel.SetExamples(4);
	return 0;
}

LRESULT RMainFrame::OnRun3d(HWND hControl, WORD cID, WORD Notify)
{
	MessageBox(m_hHandle, TEXT("Данная возможность заблокирована в текущей версии"),TEXT("eRay"),MB_OK | MB_ICONINFORMATION);
	// Load3dView();
	return 0;
}

LRESULT RMainFrame::OnVarEditor(HWND hControl, WORD cID, WORD Notify)
{
	VarEditor_Vars = ConfigPanel.VarList; // копируем имеющиеся переменные
	if (DialogBox(ModuleHandle(),MAKEINTRESOURCE(IDD_VAR_EDIT),m_hHandle,VarEditorProc))
	{
		 ConfigPanel.VarList = VarEditor_Vars;
		 ConfigPanel.UpdateVarList();
	}
	return 0;
}

void RMainFrame::Load3dView()
{
	OutRangingFor3D(this);
	//MessageBox(m_hHandle,TEXT("На данный момент эта возможность остановлена"),TEXT("Информация"),MB_OK | MB_ICONINFORMATION);
}

DWORD WINAPI RMainFrame::OutRangingFor3D(LPVOID Param)
{
	RMainFrame *This = (RMainFrame*)Param;

	ModelingParams ModParams;
	OutMsg MsgList;
	
	int GroupID = This->InfoList.AddGroup(RString(TEXT("Построение 3х мерного представления (") + This->GetCurTime() + TEXT(")")).c_str());
	This->InfoList.ScrollDown();
	bool Result = This->ParseModelingParams(&ModParams, MsgList);
	int LastIndex;
	// если проект не построен успешно печатаем сообщения об ошибке(ах)
	if (!Result)
	{
		RString tmpStr = RString(TEXT("При построении возникли следующие проблемы (")) + RConverter::ToString((int)MsgList.size()) + RString(TEXT("):"));
		
		LastIndex = This->InfoList.AddItem(This->GetCurTime().c_str(),GroupID);
		This->InfoList.SetItemText(LastIndex,1,tmpStr.c_str());
		This->InfoList.ScrollDown();
		for (size_t i = 0; i < MsgList.size(); i++)
		{
			tmpStr = RConverter::ToString((int)i+1) + RString(TEXT(". ")) + MsgList[i];
			LastIndex = This->InfoList.AddItem(TEXT(""),GroupID);
			This->InfoList.SetItemText(LastIndex,1,tmpStr.c_str());
			This->InfoList.ScrollDown();
		}
		This->UnLockInterface();
		return 0;
	}

	float Mult;
	switch(This->Option3d)
	{
		case e3doLow:
			Mult = 100;
			break;
		case e3doNorm:
			Mult = 200;
			break;
		case e3doHigh:
			Mult = 400;
			break;
		default:
			Mult = 200;
	}
	int MaxWidth = max(ModParams.XRes, ModParams.YRes);
	float Zoomer = (float)Mult/MaxWidth;
	ModParams.XRes = long(Zoomer*ModParams.XRes);
	ModParams.YRes = long(Zoomer*ModParams.YRes);

	// сохраняем прошлую конфигурацию
	This->ImagePanel.GraphView.StoreGrInf();

	LastIndex = This->InfoList.AddItem(This->GetCurTime().c_str(),GroupID);
	This->InfoList.SetItemText(LastIndex,1,TEXT("Генерация точек..."));
	This->InfoList.ScrollDown();
	This->ImagePanel.GraphView.ClearXY();
	for (size_t i = 0; i < (unsigned)ModParams.TParamsCount; i++)
	{
		Variables vars;

		// включаем в переменные t-параметр
		vars.push_back(VarItem(TEXT("t"),ModParams.TParams[i]));

		// включаем в переменные дополнительные переменные
		vars.insert(vars.end(),ModParams.Vars.begin(),ModParams.Vars.end());
		double Result1, Result2;
		ExpressionError ExprErr;

		if (!ModParams.ExprY.GetValue(Result1,vars,ExprErr))
		{
			if (IsCalcError(ExprErr.ErrorCode))
			{
				This->ImagePanel.GraphView.AddHole();
				continue;	
			}
			// далее отрабатывает ошибка
			LastIndex = This->InfoList.AddItem(This->GetCurTime().c_str(),GroupID);
			This->InfoList.SetItemText(LastIndex,1,RString(TEXT("Выражение Y : ") + ExprErrorToStr(ExprErr)).c_str());
			This->InfoList.ScrollDown();
			This->ImagePanel.GraphView.ClearXY();
			This->UnLockInterface();
			return 0;
		}
		if (!ModParams.ExprX.GetValue(Result2,vars,ExprErr))
		{
			if (IsCalcError(ExprErr.ErrorCode))
			{
				This->ImagePanel.GraphView.AddHole();
				continue;	
			}
			// далее отрабатывает ошибка
			LastIndex = This->InfoList.AddItem(This->GetCurTime().c_str(),GroupID);
			This->InfoList.SetItemText(LastIndex,1,RString(TEXT("Выражение X : ") + ExprErrorToStr(ExprErr)).c_str());
			This->InfoList.ScrollDown();
			This->ImagePanel.GraphView.ClearXY();
			This->UnLockInterface();
			return 0;
		}
		This->ImagePanel.GraphView.AddXY(Result2,Result1);
	}
	This->InfoList.SetItemText(LastIndex,1,TEXT("Генерация точек...ok!"));

	// установка параметров моделирования
	RECT DrawRect = {0,0,ModParams.XRes,ModParams.YRes};
	This->ImagePanel.GraphView.SetDrawRegion(&DrawRect);
	This->ImagePanel.GraphView.SetRayRadius(ModParams.RayRadius);
	This->ImagePanel.GraphView.GraphType = (GraphicManager::GraphTypes)(INT)This->ComboSwitch.GetItemData(This->ComboSwitch.GetSelectedIndex());
	if (This->ImagePanel.GraphView.DrawInRegion = ModParams.UseRegion)
	{
		frectangle RectRegion(ModParams.MinX, ModParams.MinY, ModParams.MaxX, ModParams.MaxY);
		This->ImagePanel.GraphView.SetGraphRegion(&RectRegion);
	}

	LastIndex = This->InfoList.AddItem(This->GetCurTime().c_str(),GroupID);
	This->InfoList.SetItemText(LastIndex,1,TEXT("Расчёт параметров..."));
	This->InfoList.ScrollDown();
	if (!This->ImagePanel.GraphView.CalculateParams())
	{
		LastIndex = This->InfoList.AddItem(TEXT(""),GroupID);
		This->InfoList.SetItemText(LastIndex,1,TEXT("Ошибка генерации параметров модели. Сохраните проект и отправьте его разработчику"));
		This->InfoList.ScrollDown();

		This->UnLockInterface();
		return 0;
	}
	This->InfoList.SetItemText(LastIndex,1,TEXT("Расчёт параметров...ok!"));

	LastIndex = This->InfoList.AddItem(This->GetCurTime().c_str(),GroupID);
	This->InfoList.SetItemText(LastIndex,1,TEXT("Построение изображения..."));
	This->InfoList.ScrollDown();
	PointsFieldFor3D tmp_PFF3D;
	if (!This->ImagePanel.GraphView.BuildCloudOutRanging(&tmp_PFF3D))
	{
		LastIndex = This->InfoList.AddItem(TEXT(""),GroupID);
		This->InfoList.SetItemText(LastIndex,1,TEXT("Ошибка генерации параметров модели. Сохраните проект и отправьте его разработчику"));
		This->InfoList.ScrollDown();

		This->UnLockInterface();
		return 0;
	}
	This->InfoList.SetItemText(LastIndex,1,TEXT("Построение изображения...ok!"));

	LastIndex = This->InfoList.AddItem(TEXT(""),GroupID);
	This->InfoList.SetItemText(LastIndex,1,TEXT("Построение успешно завершено"));
	This->InfoList.ScrollDown();

	HMODULE heRaydx3dLib = LoadLibrary(TEXT("View3D.dll"));

	if (!heRaydx3dLib) 
	{
		MessageBox(This->m_hHandle,TEXT("Не удаётся загрузить View3D.dll"),TEXT("Ошибка"),MB_OK | MB_ICONERROR);
		This->UnLockInterface();
		return 0;
	}

	typedef BOOL (*Run3DView)(HWND,FLOAT**,INT,INT);
	// поиск функции
	Run3DView StartModuleProc = (Run3DView) GetProcAddress(heRaydx3dLib,"Run3DView");
	
	if (!StartModuleProc)
	{
		MessageBox(NULL,TEXT("Не удаётся найти процедуру в View3D.dll"),TEXT("Ошибка"),MB_OK | MB_ICONERROR);
		FreeLibrary(heRaydx3dLib);
		goto ReStore;
		return 0;
	}

	float **Field;
	int Indent = This->ImagePanel.GraphView.GetGraphicIndent();
	int xCount = tmp_PFF3D.GetWidth() - 2*Indent;
	int yCount = tmp_PFF3D.GetHeight() - 2*Indent;

	int mult = max(xCount,yCount);
	Field = new float*[xCount];
	for (int i = 0; i < xCount; i++)
	{
		Field[i] = new float[yCount]; 
		for (int j = 0; j < yCount; j++)
		{
			Field[i][j] = 0.25f*mult*((FLOAT)tmp_PFF3D.Index(i + Indent,j + Indent));
		}
	}
	
	if (!StartModuleProc(This->m_hHandle,Field,xCount,yCount))
	{
		FreeLibrary(heRaydx3dLib);
		goto ReStore;
	}
	
	for (int i = 0; i < xCount; i++)
	{
		delete [] Field[i];
	}
	delete [] Field;

	FreeLibrary(heRaydx3dLib);

	// восстановление предыдущих параметров модели
ReStore:
	This->ImagePanel.GraphView.ReStoreGrInf();
	This->UnLockInterface();
	return 0;
}

LRESULT RMainFrame::OnGetTransParams(WPARAM wParam, LPARAM lParam)
{
	ConfigParams *Params = (ConfigParams*)wParam;
	if (!Params) 
		return (LRESULT) 0;
	if (lParam != sizeof(ConfigParams))
		return (LRESULT )0;

	InitConfigParam(Params);

	return 1;
}

LRESULT RMainFrame::OnSetTransParams(WPARAM wParam, LPARAM lParam)
{
	ConfigParams *Params = (ConfigParams*)wParam;
	if (!Params) 
		return (LRESULT) 0;
	if (lParam != sizeof(ConfigParams))
		return (LRESULT )0;

	ApplyConfigParam(Params);

	return 1;
}

LRESULT RMainFrame::OnViewAddParams(WPARAM wParam, LPARAM lParam)
{
	RModelParamFrame dlg;
	dlg.setMetalInfo(&g_cMetalInfo);
	if (dlg.ShowModal(this))
	{
		dlg.getMetalInfo(&g_cMetalInfo);
	}

	return 1;
}

void RMainFrame::InitConfigParam(ConfigParams *cp)
{
	// общие настройки
	cp->СommonOptimiz = ImagePanel.GraphView.UsingMultiCore;
	cp->CommonAccRayRad = ImagePanel.GraphView.AccountRayRadByAutoScale;
	cp->Option3D = Option3d;
	// классический график
	cp->ClassicDrawAxes = ImagePanel.GraphView.ClassGr.vDrawAxes;
	cp->ClassicDrawNode = ImagePanel.GraphView.ClassGr.vDrawNode;
	cp->ClassicDrawBound = ImagePanel.GraphView.ClassGr.vDrawBoundary;

	// градиентный график
	cp->GCloudDrawAxes = ImagePanel.GraphView.CloudGr.vDrawAxesGrad;
	cp->GCloudDrawBound = ImagePanel.GraphView.CloudGr.vDrawBoundaryGrad;
	cp->GCloudDrawNodeGrid = ImagePanel.GraphView.CloudGr.vDrawStepGrid;
	cp->GCloudNodeGridStep = ImagePanel.GraphView.CloudGr.PixelInOneStep;

	// радужный график
	cp->NGCloudDrawAxes = ImagePanel.GraphView.CloudGr.vDrawAxesNoGrad;
	cp->NGCloudDrawBound = ImagePanel.GraphView.CloudGr.vDrawBoundaryNoGrad;
	cp->NGCloudeDrawScale = ImagePanel.GraphView.CloudGr.vDrawScaleNoGrad;
}

void RMainFrame::ApplyConfigParam(ConfigParams *cp)
{
	// общие настройки
	ImagePanel.GraphView.UsingMultiCore = cp->СommonOptimiz;
	ImagePanel.GraphView.AccountRayRadByAutoScale = cp->CommonAccRayRad;
	Option3d = cp->Option3D;
	// классический график
	ImagePanel.GraphView.ClassGr.vDrawAxes = cp->ClassicDrawAxes;
	ImagePanel.GraphView.ClassGr.vDrawNode = cp->ClassicDrawNode;
	ImagePanel.GraphView.ClassGr.vDrawBoundary = cp->ClassicDrawBound;

	// градиентный график
	ImagePanel.GraphView.CloudGr.vDrawAxesGrad = cp->GCloudDrawAxes;
	ImagePanel.GraphView.CloudGr.vDrawBoundaryGrad = cp->GCloudDrawBound;
	ImagePanel.GraphView.CloudGr.vDrawStepGrid = cp->GCloudDrawNodeGrid;
	ImagePanel.GraphView.CloudGr.PixelInOneStep = cp->GCloudNodeGridStep;

	// радужный график
	ImagePanel.GraphView.CloudGr.vDrawAxesNoGrad = cp->NGCloudDrawAxes;
	ImagePanel.GraphView.CloudGr.vDrawBoundaryNoGrad = cp->NGCloudDrawBound;
	ImagePanel.GraphView.CloudGr.vDrawScaleNoGrad = cp->NGCloudeDrawScale;
}

bool RMainFrame::ParseModelingParams(ModelingParams *mp, OutMsg &MsgList)
{
	bool Result = true;

	//DRAWRECT RectRegion;

	ExpressionError ExprErr;
	parser_string ExprY_text = ConfigPanel.ExprY.GetText();
	parser_string ExprX_text = ConfigPanel.ExprX.GetText();

	// получаем выражения
	if (ExprY_text.size())
	{
		if (!mp->ExprY.Initialization(ExprY_text,ExprErr))
		{
			MsgList.push_back(RString(TEXT("Выражение Y :")) + RString((LPTSTR)ExprErrorToStr(ExprErr).c_str()));
			Result = false;
		}
	}
	else
	{
		MsgList.push_back(TEXT("Задайте выражение Y"));
		Result = false;
	}

	if (ExprX_text.size())
	{
		if (!mp->ExprX.Initialization(ExprX_text,ExprErr))
		{
			MsgList.push_back(RString(TEXT("Выражение X :")) + RString((LPTSTR)ExprErrorToStr(ExprErr).c_str()));
			Result = false;
		}

	}
	else
	{
		MsgList.push_back(TEXT("Задайте выражение X"));
		Result = false;
	}

	// получаем дополнительные переменные
	if (!GetVariable(mp->Vars,MsgList))
	{
		Result = false;
	}
	
	// получаем параметры T
	if (!GetParamValue(&mp->TParams, &mp->TParamsCount, MsgList))
	{
		Result = false;
	}
	
	// получаем радиус пятна
	RString Radius = ConfigPanel.RayRadius.GetText();
	if (Radius.length())
	{
		parser_string tmp(Radius);
		if (!parser_converter::try_to_double(tmp,mp->RayRadius))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + Radius + RString(TEXT("\" в вещественное число")));
			Result = false;
		}
	}
	else
	{
		MsgList.push_back(TEXT("Задайте радиус пятна"));
		Result = false;
	}

	// получаем ускоряющее напряжение пучка
	RString Voltage = ConfigPanel.RayVoltage.GetText();
	if (Voltage.length())
	{
		parser_string tmp(Voltage);
		if (!parser_converter::try_to_double(tmp,mp->RayVoltage))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + Voltage + RString(TEXT("\" в вещественное число")));
			Result = false;
		}
	}
	else
	{
		MsgList.push_back(TEXT("Задайте ускоряющее напряжение"));
		Result = false;
	}

	// получаем разрешение картинки
	RString XRes = ConfigPanel.ModelWidth.GetText();
	if (XRes.length())
	{
		parser_string tmp(XRes);
		if (!parser_converter::try_to_integer(tmp,(mp->XRes)))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + XRes + RString(TEXT("\" в целое число")));
			Result = false;
		}
	}
	else
	{
		MsgList.push_back(TEXT("Задайте ширину изображения"));
		Result = false;
	}
	RString YRes = ConfigPanel.ModelHeight.GetText();
	if (YRes.length())
	{
		parser_string tmp(YRes);
		if (!parser_converter::try_to_integer(tmp,(mp->YRes)))
		{
			MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + YRes + RString(TEXT("\" в целое число")));
			Result = false;
		}
	}
	else
	{
		MsgList.push_back(TEXT("Задайте высоту изображения"));
		Result = false;
	}

	// проверка на автоматический режим
	if (!mp->XRes || !mp->YRes)
	{
		RECT ClientRect;
		ImagePanel.GetClientRect(ClientRect);
		if (!mp->XRes) mp->XRes = ClientRect.right;
		if (!mp->YRes) mp->YRes = ClientRect.bottom;
	}

	// проверка на использования региона
	if (mp->UseRegion = ConfigPanel.BoundCheck.GetCheck())
	{
		RString strMinX = ConfigPanel.MinX.GetText();
		if (strMinX.length())
		{
			parser_string tmp(strMinX);
			if (!parser_converter::try_to_double(tmp,mp->MinX))
			{
				MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + strMinX + RString(TEXT("\" в целое число")));
				Result = false;
			}
		}
		else
		{
			MsgList.push_back(TEXT("Задайте MinX для региона"));
			Result = false;
		}

		RString strMaxX = ConfigPanel.MaxX.GetText();
		if (strMaxX.length())
		{
			parser_string tmp(strMaxX);
			if (!parser_converter::try_to_double(tmp,mp->MaxX))
			{
				MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + strMaxX + RString(TEXT("\" в целое число")));
				Result = false;
			}
		}
		else
		{
			MsgList.push_back(TEXT("Задайте MaxX для региона"));
			Result = false;
		}

		RString strMinY = ConfigPanel.MinY.GetText();
		if (strMinY.length())
		{
			parser_string tmp(strMinY);
			if (!parser_converter::try_to_double(tmp,mp->MinY))
			{
				MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + strMinY + RString(TEXT("\" в целое число")));
				Result = false;
			}
		}
		else
		{
			MsgList.push_back(TEXT("Задайте MinY для региона"));
			Result = false;
		}

		RString strMaxY = ConfigPanel.MaxY.GetText();
		if (strMaxY.length())
		{
			parser_string tmp(strMaxY);
			if (!parser_converter::try_to_double(tmp,mp->MaxY))
			{
				MsgList.push_back(RString(TEXT("Невозможно преобразовать \"")) + strMaxY + RString(TEXT("\" в целое число")));
				Result = false;
			}
		}
		else
		{
			MsgList.push_back(TEXT("Задайте MaxY для региона"));
			Result = false;
		}
	}
	
	return Result;
}

void RMainFrame::LockInterface()
{
	// блокировка меню
	HMENU hMenu = GetMenu(m_hHandle);
	if (hMenu)
	{
		EnableMenuItem(hMenu,FILE_OPEN,TRUE);
		EnableMenuItem(hMenu,FILE_SAVE,TRUE);
		EnableMenuItem(hMenu,FILE_SAVE_AS,TRUE);
		EnableMenuItem(hMenu,FILE_SAVE_IMAGE,TRUE);

		EnableMenuItem(hMenu,MODEL_START,TRUE);
		EnableMenuItem(hMenu,MODEL_EX_RING,TRUE);
		EnableMenuItem(hMenu,MODEL_EX_SIN,TRUE);
		EnableMenuItem(hMenu,MODEL_EX_SPIR,TRUE);
		EnableMenuItem(hMenu,MODEL_EX_KREN,TRUE);
		EnableMenuItem(hMenu,MODEL_EX_ROME,TRUE);
		EnableMenuItem(hMenu,MODEL_VAR_EDIT,TRUE);
		EnableMenuItem(hMenu,MODEL_RUN_3D,TRUE);
		EnableMenuItem(hMenu,MODEL_CONFIG,TRUE);
	}

	// блокировка панели инструментов
	ToolBar.SetEnable(FALSE);

	// блокировка панели отображения
	ImagePanel.LockInterface();

	// блокировка панели конфигурации модели
	ConfigPanel.LockInterface();
}

void RMainFrame::UnLockInterface()
{
	// разблокировка меню
	HMENU hMenu = GetMenu(m_hHandle);
	if (hMenu)
	{
		EnableMenuItem(hMenu,FILE_OPEN,FALSE);
		EnableMenuItem(hMenu,FILE_SAVE,FALSE);
		EnableMenuItem(hMenu,FILE_SAVE_AS,FALSE);
		EnableMenuItem(hMenu,FILE_SAVE_IMAGE,FALSE);

		EnableMenuItem(hMenu,MODEL_START,FALSE);
		EnableMenuItem(hMenu,MODEL_EX_RING,FALSE);
		EnableMenuItem(hMenu,MODEL_EX_SIN,FALSE);
		EnableMenuItem(hMenu,MODEL_EX_SPIR,FALSE);
		EnableMenuItem(hMenu,MODEL_EX_KREN,FALSE);
		EnableMenuItem(hMenu,MODEL_EX_ROME,FALSE);
		EnableMenuItem(hMenu,MODEL_VAR_EDIT,FALSE);
		EnableMenuItem(hMenu,MODEL_RUN_3D,FALSE);
		EnableMenuItem(hMenu,MODEL_CONFIG,FALSE);
	}

	// разблокировка панели инструментов
	ToolBar.SetEnable(TRUE);

	// разблокировка панели отображения
	ImagePanel.UnLockInterface();

	// разблокировка панели конфигурации модели
	ConfigPanel.UnLockInterface();
}

LRESULT RMainFrame::OnFocus(WPARAM wParam, LPARAM lParam)
{
	ConfigPanel.SetFocus();
	return 0;
}

int RMainFrame::AddEventGroup(LPCTSTR szText)
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	int Result;

	if (osvi.dwMajorVersion > 5) // начиная с NT 6.0 поддержка групп 
	{
		Result = InfoList.AddGroup(szText);
	}
	else
	{
		InfoList.AddItem(szText);
		Result = -1;
	}

	InfoList.ScrollDown();
	return Result;
}

int RMainFrame::AddEventMsg(bool ViewTime, LPCTSTR szText, int idGroup)
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	RString CurTime = ViewTime ? GetCurTime() : TEXT("");

	int Result;

	if (osvi.dwMajorVersion > 5) // начиная с NT 6.0 поддержка групп 
	{	
		int LastIndex = InfoList.AddItem(CurTime.c_str(),idGroup);
		InfoList.SetItemText(LastIndex,1,szText);
		Result = LastIndex;
	}
	else
	{
		int LastIndex = InfoList.AddItem(CurTime.c_str());
		InfoList.SetItemText(LastIndex,1,szText);
		Result = LastIndex;
	}

	InfoList.ScrollDown();
	return Result;
}

LRESULT RMainFrame::OnViewTemperatureModel(WPARAM wParam, LPARAM lParam)
{	
	// инициализируем тепловой поток
	int Indent = ImagePanel.GraphView.GetGraphicIndent();
	tBaseCloudDump fld(ImagePanel.GraphView.CloudGr.BaseCloudDump.GetWidth() - 2 * Indent, 
		ImagePanel.GraphView.CloudGr.BaseCloudDump.GetHeight() - 2 * Indent);
	for (int i = 0, i_stop = fld.GetWidth(); i < i_stop; i++)
	{
		for (int j = 0 ,j_stop = fld.GetHeight(); j < j_stop; j++)
		{
			fld.Index(i, j) = ImagePanel.GraphView.CloudGr.BaseCloudDump.Index(i + Indent,j + Indent);
		}
	}

	fld.SetPixelWeight(ImagePanel.GraphView.CloudGr.BaseCloudDump.GetPixelWeight());
	double S = fld.GetWidth() * fld.GetHeight() * fld.GetPixelWeight() * fld.GetPixelWeight();
	double Power = pow(ImagePanel.GraphView.GrInf.RayVoltage,2.5) * 
		            ImagePanel.GraphView.GrInf.AmpTraffic * 1000;
	double QStream = (Power * 
					ImagePanel.GraphView.CloudGr.TotalEfficiency) / 
					S;
	RTemperatureFrame dlg;
	dlg.InitQzt(fld, QStream);
	dlg.InitMetalParam(&g_cMetalInfo);
	dlg.ShowModal(this);

	return 0;
}