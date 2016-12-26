#include "RTemperatureFrame.h"

/*
#define WM_MYMSG_STEP WM_USER + 1
*/

RTemperatureFrame *g_wndTemperatureFrame;

RTemperatureFrame::RTemperatureFrame() 
{
	hWorkThread = 0;
	CurrentModuleState = InComplete;
	g_wndTemperatureFrame = this;
	AddMessage(WM_CREATE,MSG_EVENT_CAST(RTemperatureFrame::OnCreate),this);
	AddMessage(WM_CLOSE,MSG_EVENT_CAST(RTemperatureFrame::OnClose),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RTemperatureFrame::OnSize),this);
	AddMessage(WM_DESTROY,MSG_EVENT_CAST(RTemperatureFrame::OnDestroy),this);
	AddMessage(WM_COMMAND,MSG_EVENT_CAST(RTemperatureFrame::OnCommand),this);
	AddMessage(WM_HSCROLL,MSG_EVENT_CAST(RTemperatureFrame::OnHScroll),this);
}

RTemperatureFrame::~RTemperatureFrame()
{
	//
}

void RTemperatureFrame::VisualStyles()
{
	m_wndSwitchView.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndCurrentDimLayer.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndLabel1.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndLabelTime.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStepCount.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStartButton.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndQStreamBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticQxl.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQxr.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQyl.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQyr.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQzb.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndTextBoxQxl.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxQxr.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxQyl.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxQyr.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxQzb.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticQxlLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQxrLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQylLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQyrLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticQzbLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndModelParamBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	
	m_wndStaticDeltaTau.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxDeltaTau.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticDeltaTauLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticN.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxN.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticNLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	
	m_wndStaticM.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxM.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticMLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticL.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxL.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticLLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	
	m_wndModelBeginParamBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticDropletTemp.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxDropletTemp.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticDropletTempLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticT0Liq.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxT0Liq.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticT0LiqLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticT0Sol.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxT0Sol.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticT0SolLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticSolProp.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxSolProp.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticSolPropLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	m_wndStaticTEnv.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndTextBoxTEnv.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_wndStaticTEnvLab.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));


	// ��������� ���������� �� ���������
	m_wndTextBoxQxl.SetText(TEXT("(t-303)/0.01"));
	m_wndTextBoxQxr.SetText(TEXT("(t-303)/0.01"));
	m_wndTextBoxQyl.SetText(TEXT("(t-303)/0.01"));
	m_wndTextBoxQyr.SetText(TEXT("(t-303)/0.01"));
	m_wndTextBoxQzb.SetText(TEXT("(t-303)/0.01"));
	m_wndTextBoxDeltaTau.SetText(TEXT("0.5"));
	m_wndTextBoxDropletTemp.SetText(TEXT("2000"));
	m_wndTextBoxT0Liq.SetText(TEXT("2000"));
	m_wndTextBoxT0Sol.SetText(TEXT("1900"));
	m_wndTextBoxSolProp.SetText(TEXT("0.3"));
	m_wndTextBoxTEnv.SetText(TEXT("700"));
	m_wndStepCount.SetText(TEXT("100"));

	// ��������� ����������� ���������
	m_wndTextBoxQxl.AddToolTip(TEXT("�������� ����� �� ������ {x=0,y=[0;B],z=[0;H]}"));
	m_wndTextBoxQxr.AddToolTip(TEXT("�������� ����� �� ������ {x=A,y=[0;B],z=[0;H]}"));
	m_wndTextBoxQyl.AddToolTip(TEXT("�������� ����� �� ������ {x=[0;A],y=0,z=[0;H]}"));
	m_wndTextBoxQyr.AddToolTip(TEXT("�������� ����� �� ������ {x=[0;A],y=B,z=[0;H]}"));
	m_wndTextBoxQzb.AddToolTip(TEXT("�������� ����� �� ������ {x=[0;A],y=[0;B],z=0}"));
	m_wndTextBoxDeltaTau.AddToolTip(TEXT("��� �� �������"));
	m_wndTextBoxDropletTemp.AddToolTip(TEXT("����������� ������������ � ������������� ������� �������"));
	m_wndTextBoxT0Liq.AddToolTip(TEXT("����������� ������ ����� ������� � ��������� ������ �������"));
	m_wndTextBoxT0Sol.AddToolTip(TEXT("����������� ������� ����� ������� � ��������� ������ �������"));
	m_wndTextBoxSolProp.AddToolTip(TEXT("���� ������� ����� ������� � ��������� ������ �������"));
	m_wndTextBoxTEnv.AddToolTip(TEXT("����������� ���������� �����"));
	m_wndStepCount.AddToolTip(TEXT("���������� ����� ��� �������������"));

	m_wndTextBoxN.SetEnable(FALSE);
	m_wndTextBoxM.SetEnable(FALSE);
}

LRESULT RTemperatureFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{	
	LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;

	// ��������� ���� �� �������� ������������ ��������
	RECT OwnerRect;
	::GetWindowRect(lpcs->hwndParent,&OwnerRect);

	RECT ThisRect;
	::GetWindowRect(m_hHandle,&ThisRect);

	SetWindowPos(m_hHandle,NULL,
		OwnerRect.left + (OwnerRect.right - OwnerRect.left)/2 - (ThisRect.right - ThisRect.left)/2,
		OwnerRect.top + (OwnerRect.bottom - OwnerRect.top)/2 - (ThisRect.bottom - ThisRect.top)/2,
		0,
		0,
		SWP_NOSIZE);


	// �������� ������������� ����
	if (!m_wndSwitchView.Create(TEXT(""),this,TEMPERATURE_SWITCH_VIEW,0,0,0,0))
		return -1;

	int SwitchIndex = m_wndSwitchView.AddString(TEXT("�������� �� XY"),(LPVOID)ViewXY);
	m_wndSwitchView.AddString(TEXT("������� �������� �� XY"),(LPVOID)ViewXYPhase);
	m_wndSwitchView.AddString(TEXT("�������� �� XZ"),(LPVOID)ViewXZ);
	m_wndSwitchView.AddString(TEXT("������� �������� �� XZ"),(LPVOID)ViewXZPhase);
	m_wndSwitchView.AddString(TEXT("�������� �� YZ"),(LPVOID)ViewYZ);
	m_wndSwitchView.AddString(TEXT("������� �������� �� YZ"),(LPVOID)ViewYZPhase);
	m_wndSwitchView.AddString(TEXT("������������� �����"),(LPVOID)ViewTempLine);
	m_wndSwitchView.SetSelectedIndex(SwitchIndex);

	// �������� �������� - ������ ���� �� ���
	if (!m_wndDimLayer.Create(TEXT("0"),this,TEMPERATURE_VIEW_DIMLAYER,0,0,0,0))
		return -1;
	m_wndDimLayer.SetEnable(FALSE);
	m_wndDimLayer.SetStyle(m_wndDimLayer.GetStyle() | TBS_HORZ | TBS_AUTOTICKS);
	m_wndDimLayer.SetRange(0,0);
	m_wndDimLayer.SetTrackPos(0);

	
	if (!m_wndCurrentDimLayer.Create(TEXT("N/A"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE | WS_BORDER,this,0,0,0,0,0))
		return -1;
	// �������� �������
	LPCTSTR szImageClassName = TEXT("RTemperatureImage");
	if (!RFrame::RegisterWndClass(szImageClassName,CS_HREDRAW | CS_VREDRAW,NULL,LoadCursor(0,IDC_ARROW)))
		return -1;
	if (!m_wndImage.Create(0,szImageClassName,TEXT(""),WS_VISIBLE | WS_CHILD,this,0,0,0,0,NULL))
		return -1;

	// �������� �������
	if (!m_wndLabel1.Create(TEXT("���-�� �����:"), WS_VISIBLE | WS_CHILD,this,0,0,0,0,0))
		return -1;

	// �������� ���� ����� ����
	if (!m_wndStepCount.Create(WS_VISIBLE | WS_CHILD | ES_NUMBER | ES_CENTER,this,TEMPERATURE_STEP,0,0,0,0))
		return -1;

	// �������� ������ ������� ��������
	if (!m_wndStartButton.Create(TEXT("������"), this, TEMPERATURE_RENDER_START, 0, 0, 0, 0))
		return -1;

	if (!m_wndLabelTime.Create(TEXT("����� ������: N/A"), WS_VISIBLE | WS_CHILD,this,0,0,0,0,0))
		return -1;
	

	//m_wndStartButton.SetStyle(m_wndStartButton.GetStyle() | WS_TABSTOP);

	if(!m_wndQStreamBox.Create(TEXT("�������� ������"),this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticQxl.Create(TEXT("Qxl:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQxr.Create(TEXT("Qxr:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQyl.Create(TEXT("Qyl:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQyr.Create(TEXT("Qyr:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQzb.Create(TEXT("Qzb:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxQxl.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxQxr.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxQyl.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxQyr.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxQzb.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticQxlLab.Create(TEXT("��/�2"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQxrLab.Create(TEXT("��/�2"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQylLab.Create(TEXT("��/�2"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQyrLab.Create(TEXT("��/�2"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticQzbLab.Create(TEXT("��/�2"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;


	if(!m_wndModelParamBox.Create(TEXT("��������� ������"),this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticDeltaTau.Create(TEXT("\x0394\x03C4:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxDeltaTau.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,TEMPERATURE_DTAU,0,0,0,0))
		return -1;
	if (!m_wndStaticDeltaTauLab.Create(TEXT("���."), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticN.Create(TEXT("N:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxN.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,TEMPERATURE_N,0,0,0,0))
		return -1;
	if (!m_wndStaticNLab.Create(TEXT("��."), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticM.Create(TEXT("M:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxM.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,TEMPERATURE_M,0,0,0,0))
		return -1;
	if (!m_wndStaticMLab.Create(TEXT("��."), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticL.Create(TEXT("L:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxL.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,TEMPERATURE_L,0,0,0,0))
		return -1;
	if (!m_wndStaticLLab.Create(TEXT("��."), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticDropletTemp.Create(TEXT("T�:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxDropletTemp.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticDropletTempLab.Create(TEXT("K"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if(!m_wndModelBeginParamBox.Create(TEXT("��������� �������"),this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticT0Liq.Create(TEXT("T0 �:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxT0Liq.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticT0LiqLab.Create(TEXT("K"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticT0Sol.Create(TEXT("T0 �:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxT0Sol.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticT0SolLab.Create(TEXT("K"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticSolProp.Create(TEXT("Sp:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxSolProp.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticSolPropLab.Create(TEXT("��"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticTEnv.Create(TEXT("T ���:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;
	if (!m_wndTextBoxTEnv.Create(WS_VISIBLE | WS_CHILD | ES_CENTER,this,0,0,0,0,0))
		return -1;
	if (!m_wndStaticTEnvLab.Create(TEXT("K"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;



	// ������ ��������� ������
	if (!m_wndStatusBar.Create(TEXT(""),this,false, 0, 0, 0, 0, 0))
		return -1;
	// ������ ������� ��������� ������
	m_wndStatusBar.SetSimple(false);

	if (!m_wndProgress.Create(PBS_SMOOTH, &m_wndStatusBar, 0, 0, 0, 0, 0))
		return -1;

	// ��������� �������� �����
	m_wndTextBoxDeltaTau.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxN.SetFilter(RTextBoxEx::IntegerFilter);
	m_wndTextBoxM.SetFilter(RTextBoxEx::IntegerFilter);
	m_wndTextBoxL.SetFilter(RTextBoxEx::IntegerFilter);

	m_wndTextBoxDeltaTau.SetSignInput(false);
	m_wndTextBoxN.SetSignInput(false);
	m_wndTextBoxM.SetSignInput(false);
	m_wndTextBoxL.SetSignInput(false);

	m_wndTextBoxDropletTemp.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxT0Liq.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxT0Sol.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxSolProp.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxTEnv.SetFilter(RTextBoxEx::RealFilter);

	m_wndTextBoxDropletTemp.SetSignInput(false);
	m_wndTextBoxT0Liq.SetSignInput(false);
	m_wndTextBoxT0Sol.SetSignInput(false);
	m_wndTextBoxSolProp.SetSignInput(false);
	m_wndTextBoxTEnv.SetSignInput(false);

	// ���������� ��������� � ������� tab - ���������
	TabList.Append(&m_wndSwitchView);
	TabList.Append(&m_wndDimLayer);
	TabList.Append(&m_wndTextBoxQxl);
	TabList.Append(&m_wndTextBoxQxr);
	TabList.Append(&m_wndTextBoxQyl);
	TabList.Append(&m_wndTextBoxQyr);
	TabList.Append(&m_wndTextBoxQzb);
	TabList.Append(&m_wndTextBoxDeltaTau);
	TabList.Append(&m_wndTextBoxN);
	TabList.Append(&m_wndTextBoxM);
	TabList.Append(&m_wndTextBoxL);
	TabList.Append(&m_wndTextBoxDropletTemp);
	TabList.Append(&m_wndTextBoxT0Liq);
	TabList.Append(&m_wndTextBoxT0Sol);
	TabList.Append(&m_wndTextBoxSolProp);
	TabList.Append(&m_wndTextBoxTEnv);
	TabList.Append(&m_wndStepCount);
	TabList.Append(&m_wndStartButton);

	// ������������� �������� ��������� ������
	CurrentModuleState = InComplete;

	// ���������� ���������� ������
	VisualStyles();

	// ������������� �������
	InitRenderParams();

	// ��������� ����������� ���������� ����� ��� ������
	m_wndTextBoxN.SetText(RConverter::ToString(m_wndImage.MainRender.GetActualN()).c_str());
	m_wndTextBoxM.SetText(RConverter::ToString(m_wndImage.MainRender.GetActualM()).c_str());
	m_wndTextBoxL.SetText(RConverter::ToString(m_wndImage.MainRender.GetActualL()).c_str());

	return 0;
}

LRESULT RTemperatureFrame::OnClose(WPARAM wParam, LPARAM lParam)
{	
	if (hWorkThread)
	{
		if (MessageBox(m_hHandle,TEXT("�� �������, ��� ������ �������� ��������?"),TEXT(""),MB_YESNO | MB_ICONQUESTION)
			== IDYES)
		{
			TerminateThread(hWorkThread,0);
			hWorkThread = 0;
			PostQuitMessage(0);
		}
	}
	else
	{
		PostQuitMessage(0);
	}
	return 0;
}

LRESULT RTemperatureFrame::OnSize(WPARAM wParam, LPARAM lParam)
{
	int ControlIndent = 5;
	int StandartControlHeight = 20;
	int GroupBoxHeight = 144;

	int ProgressWidth = 100;

	// ������������ ������� ����������
	m_wndStatusBar.AutoSize();
	int ClientWidth = LOWORD(lParam);
	int ClientHeight = HIWORD(lParam) - m_wndStatusBar.GetHeight();

	INT PartMas[] = {ClientWidth - ProgressWidth,ProgressWidth,-1};
	m_wndStatusBar.SetParts(PartMas,sizeof(PartMas)/sizeof(INT));
	UpdateStatusMsg();

	int ProgressIndent = 2;
	m_wndProgress.SetWindowPosition(NULL,ClientWidth - ProgressWidth + ProgressIndent,ProgressIndent,ProgressWidth - 2*ProgressIndent,m_wndStatusBar.GetHeight() - 2*ProgressIndent,SWP_NOACTIVATE);

	// ��������� ������� ������������� ����
	int SwitchLeft = ControlIndent;
	int SwitchTop = ControlIndent;
	int SwitchWidth = 150;
	int SwitchHeight = StandartControlHeight;
	m_wndSwitchView.SetWindowPosition(NULL,SwitchLeft,SwitchTop,SwitchWidth,SwitchHeight,SWP_NOACTIVATE);

	int CurrentDimWidth = 140;
	int CurrentDimHeight = StandartControlHeight;
	int CurrentDimLeft = ClientWidth - ControlIndent - CurrentDimWidth;
	int CurrentDimTop = ControlIndent;
	m_wndCurrentDimLayer.SetWindowPosition(NULL,CurrentDimLeft,CurrentDimTop,CurrentDimWidth,CurrentDimHeight,SWP_NOACTIVATE);

	// ��������� ������� �������� ��������
	int TrackLeft = SwitchLeft + SwitchWidth + ControlIndent;
	int TrackTop = ControlIndent;
	int TrackWidth = ClientWidth - SwitchWidth - CurrentDimWidth - 4 * ControlIndent;
	int TrackHeight = StandartControlHeight;
	m_wndDimLayer.SetWindowPosition(NULL,TrackLeft,TrackTop,TrackWidth,TrackHeight,SWP_NOACTIVATE);


	// ��������� ������� �����������
	int ImageTop = SwitchTop + SwitchHeight + ControlIndent;
	int ImageLeft = ControlIndent;
	int ImageHeight = ClientHeight - (ImageTop + GroupBoxHeight + 4 * ControlIndent);
	int ImageWidth = ClientWidth - 2 * ControlIndent;
	m_wndImage.SetWindowPosition(NULL, ImageLeft, ImageTop, ImageWidth, ImageHeight, SWP_NOACTIVATE);

	int BottomControlTop = ImageTop + ImageHeight + 3 * ControlIndent;
	
	int StartButtonWidth = 80;
	m_wndStartButton.SetWindowPosition(NULL,ClientWidth - StartButtonWidth - ControlIndent,ClientHeight - StandartControlHeight - ControlIndent,StartButtonWidth,StandartControlHeight,SWP_NOACTIVATE);

	{
		int StepCountWidth = 100;
		int StepCountLeft = ClientWidth - StepCountWidth - ControlIndent;
		int LabelLeft = StepCountLeft;
		int TimeLabelTop = ImageTop + ImageHeight + ControlIndent;
		
		m_wndLabel1.SetWindowPosition(NULL,LabelLeft, TimeLabelTop,StepCountWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStepCount.SetWindowPosition(NULL,StepCountLeft, TimeLabelTop + StandartControlHeight + ControlIndent,StepCountWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndLabelTime.SetWindowPosition(NULL,LabelLeft, TimeLabelTop + 2*StandartControlHeight + 3*ControlIndent,StepCountWidth,2*StandartControlHeight,SWP_NOACTIVATE);
	}
	int QStreamBoxWidth = 200;
	int ModelBoxWidth = 250;
	int ModelBeginBoxWidth = 160;
	
	{
		int QStreamBoxLeft = ControlIndent;
		int QStreamBoxTop = BottomControlTop;
		m_wndQStreamBox.SetWindowPosition(NULL, QStreamBoxLeft, QStreamBoxTop, QStreamBoxWidth, GroupBoxHeight, SWP_NOACTIVATE);
		
		int QStreamBoxStaticWidth = 25;
		int QStreamBoxStaticWidthLab = 30;
		int QStreamBoxStaticLeft1 = QStreamBoxLeft + ControlIndent;
		int QStreamBoxControlTop = QStreamBoxTop + 16;

		int QStreamBoxTextBoxWidth = (QStreamBoxWidth - (4 * ControlIndent + QStreamBoxStaticWidth + QStreamBoxStaticWidthLab));
		int QStreamBoxTextBoxLeft1 = QStreamBoxLeft + 2 * ControlIndent + QStreamBoxStaticWidth;
		int QStreamBoxTextBoxLeft = QStreamBoxLeft + 2 * ControlIndent + QStreamBoxStaticWidth;

		int QStreamBoxControlTopTmp = QStreamBoxControlTop;
		m_wndStaticQxl.SetWindowPosition(NULL,QStreamBoxStaticLeft1,QStreamBoxControlTopTmp,QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQxr.SetWindowPosition(NULL,QStreamBoxStaticLeft1,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQyl.SetWindowPosition(NULL,QStreamBoxStaticLeft1,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQyr.SetWindowPosition(NULL,QStreamBoxStaticLeft1,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQzb.SetWindowPosition(NULL,QStreamBoxStaticLeft1,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		
		QStreamBoxControlTopTmp = QStreamBoxControlTop;

		m_wndTextBoxQxl.SetWindowPosition(NULL,QStreamBoxTextBoxLeft,QStreamBoxControlTopTmp,QStreamBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxQxr.SetWindowPosition(NULL,QStreamBoxTextBoxLeft,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxQyl.SetWindowPosition(NULL,QStreamBoxTextBoxLeft,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxQyr.SetWindowPosition(NULL,QStreamBoxTextBoxLeft,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxQzb.SetWindowPosition(NULL,QStreamBoxTextBoxLeft,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int QStreamBoxStaticLeft2 = QStreamBoxLeft + ControlIndent +QStreamBoxStaticWidth + ControlIndent + QStreamBoxTextBoxWidth + ControlIndent;

		QStreamBoxControlTopTmp = QStreamBoxControlTop;
		m_wndStaticQxlLab.SetWindowPosition(NULL,QStreamBoxStaticLeft2,QStreamBoxControlTopTmp,QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQxrLab.SetWindowPosition(NULL,QStreamBoxStaticLeft2,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQylLab.SetWindowPosition(NULL,QStreamBoxStaticLeft2,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQyrLab.SetWindowPosition(NULL,QStreamBoxStaticLeft2,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticQzbLab.SetWindowPosition(NULL,QStreamBoxStaticLeft2,QStreamBoxControlTopTmp += (StandartControlHeight + ControlIndent),QStreamBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
	}

	{
		int ModelBoxLeft = 2*ControlIndent + QStreamBoxWidth;
		int ModelBoxTop = BottomControlTop;
		m_wndModelParamBox.SetWindowPosition(NULL, ModelBoxLeft, ModelBoxTop, ModelBoxWidth, GroupBoxHeight, SWP_NOACTIVATE);
		
		int ModelBoxStaticWidth = 15;
		int ModelBoxStaticWidthLab = 120;
		int ModelBoxStaticLeft1 = ModelBoxLeft + ControlIndent;
		int ModelBoxControlTop = ModelBoxTop + 16;

		int ModelBoxTextBoxWidth = (ModelBoxWidth - (4 * ControlIndent + ModelBoxStaticWidth + ModelBoxStaticWidthLab));
		int ModelBoxTextBoxLeft1 = ModelBoxLeft + 2 * ControlIndent + ModelBoxStaticWidth;
		int ModelBoxTextBoxLeft = ModelBoxLeft + 2 * ControlIndent + ModelBoxStaticWidth;

		int ModelBoxControlTopTmp = ModelBoxControlTop;
		m_wndStaticDeltaTau.SetWindowPosition(NULL,ModelBoxStaticLeft1,ModelBoxControlTopTmp,ModelBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticN.SetWindowPosition(NULL,ModelBoxStaticLeft1,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticM.SetWindowPosition(NULL,ModelBoxStaticLeft1,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticL.SetWindowPosition(NULL,ModelBoxStaticLeft1,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		
		ModelBoxControlTopTmp = ModelBoxControlTop;

		m_wndTextBoxDeltaTau.SetWindowPosition(NULL,ModelBoxTextBoxLeft,ModelBoxControlTopTmp,ModelBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxN.SetWindowPosition(NULL,ModelBoxTextBoxLeft,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxM.SetWindowPosition(NULL,ModelBoxTextBoxLeft,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxL.SetWindowPosition(NULL,ModelBoxTextBoxLeft,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int ModelBoxStaticLeft2 = ModelBoxLeft + ControlIndent + ModelBoxStaticWidth + ControlIndent + ModelBoxTextBoxWidth + ControlIndent;

		ModelBoxControlTopTmp = ModelBoxControlTop;
		m_wndStaticDeltaTauLab.SetWindowPosition(NULL,ModelBoxStaticLeft2,ModelBoxControlTopTmp,ModelBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticNLab.SetWindowPosition(NULL,ModelBoxStaticLeft2,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMLab.SetWindowPosition(NULL,ModelBoxStaticLeft2,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticLLab.SetWindowPosition(NULL,ModelBoxStaticLeft2,ModelBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
	}


	// ��������� ������� ��������� ���������� ���������
	{
		int ModelBeginBoxLeft = 3*ControlIndent + QStreamBoxWidth + ModelBoxWidth;
		int ModelBeginBoxTop = BottomControlTop;
		m_wndModelBeginParamBox.SetWindowPosition(NULL, ModelBeginBoxLeft, ModelBeginBoxTop, ModelBeginBoxWidth, GroupBoxHeight, SWP_NOACTIVATE);
		
		int ModelBeginBoxStaticWidth = 35;
		int ModelBeginBoxStaticWidthLab = 25;
		int ModelBeginBoxStaticLeft1 = ModelBeginBoxLeft + ControlIndent;
		int ModelBeginBoxControlTop = ModelBeginBoxTop + 16;

		int ModelBeginBoxTextBoxWidth = (ModelBeginBoxWidth - (4 * ControlIndent + ModelBeginBoxStaticWidth + ModelBeginBoxStaticWidthLab));
		int ModelBeginBoxTextBoxLeft1 = ModelBeginBoxLeft + 2 * ControlIndent + ModelBeginBoxStaticWidth;
		int ModelBeginBoxTextBoxLeft = ModelBeginBoxLeft + 2 * ControlIndent + ModelBeginBoxStaticWidth;

		int ModelBeginBoxControlTopTmp = ModelBeginBoxControlTop;
		m_wndStaticDropletTemp.SetWindowPosition(NULL,ModelBeginBoxStaticLeft1,ModelBeginBoxControlTopTmp,ModelBeginBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticT0Liq.SetWindowPosition(NULL,ModelBeginBoxStaticLeft1,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticT0Sol.SetWindowPosition(NULL,ModelBeginBoxStaticLeft1,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticSolProp.SetWindowPosition(NULL,ModelBeginBoxStaticLeft1,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticTEnv.SetWindowPosition(NULL,ModelBeginBoxStaticLeft1,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		
		ModelBeginBoxControlTopTmp = ModelBeginBoxControlTop;

		m_wndTextBoxDropletTemp.SetWindowPosition(NULL,ModelBeginBoxTextBoxLeft,ModelBeginBoxControlTopTmp,ModelBeginBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxT0Liq.SetWindowPosition(NULL,ModelBeginBoxTextBoxLeft,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxT0Sol.SetWindowPosition(NULL,ModelBeginBoxTextBoxLeft,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxSolProp.SetWindowPosition(NULL,ModelBeginBoxTextBoxLeft,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxTEnv.SetWindowPosition(NULL,ModelBeginBoxTextBoxLeft,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int ModelBeginBoxStaticLeft2 = ModelBeginBoxLeft + ControlIndent + ModelBeginBoxStaticWidth + ControlIndent + ModelBeginBoxTextBoxWidth + ControlIndent;

		ModelBeginBoxControlTopTmp = ModelBeginBoxControlTop;
		m_wndStaticDropletTempLab.SetWindowPosition(NULL,ModelBeginBoxStaticLeft2,ModelBeginBoxControlTopTmp,ModelBeginBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticT0LiqLab.SetWindowPosition(NULL,ModelBeginBoxStaticLeft2,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticT0SolLab.SetWindowPosition(NULL,ModelBeginBoxStaticLeft2,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticSolPropLab.SetWindowPosition(NULL,ModelBeginBoxStaticLeft2,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticTEnvLab.SetWindowPosition(NULL,ModelBeginBoxStaticLeft2,ModelBeginBoxControlTopTmp += (StandartControlHeight + ControlIndent),ModelBeginBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
	}

	return 0;
}

LRESULT RTemperatureFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	m_wndImage.DestroyWindow();
	RFrame::UnRegisterWndClass(m_wndImage.GetClassName().c_str());
	return 0;
}

LRESULT RTemperatureFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
		case TEMPERATURE_RENDER_START:
		{
			if(HIWORD(wParam) == BN_CLICKED)
			{
				StartRender(wParam, lParam);
			}
			break;
		}
		case TEMPERATURE_SWITCH_VIEW:
		{
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				ChangeView(wParam,lParam);
			}
			break;
		}
		case TEMPERATURE_N:
		{
			N_Command(wParam,lParam);
			break;
		}
		case TEMPERATURE_M:
		{
			M_Command(wParam,lParam);
			break;
		}
		case TEMPERATURE_L:
		{
			L_Command(wParam,lParam);
			break;
		}
		case TEMPERATURE_DTAU:
		case TEMPERATURE_STEP:
		{
			TimeChange(wParam,lParam);
			break;
		}
	}
	return 0;
}

void RTemperatureFrame::StartRender(WPARAM wParam, LPARAM lParam)
{
	if (!hWorkThread)
	{
		RTemperatureFrameParam *rtfp = new RTemperatureFrameParam; 
		if (!GetModelingParams(rtfp))
		{
			return;
		}

		DWORD dwThreadID;
		hWorkThread = CreateThread(NULL, 0, StartRenderThread, rtfp, CREATE_SUSPENDED, &dwThreadID);
		if (hWorkThread)
		{
			LockInterface();

			// ������� ���������� �����������
			m_wndImage.Image.Clear();
			m_wndImage.Refresh();

			m_wndStartButton.SetText(TEXT("��������"));
			ResumeThread(hWorkThread);
		}
	}
	else
	{
		m_wndImage.MainRender.SetStopEvent(true);
		m_wndStartButton.SetText(TEXT("����������..."));
		m_wndStartButton.SetEnable(FALSE);
	}
}

eView RTemperatureFrame::GetCurrentView()
{
	return (eView)(int)(m_wndSwitchView.GetItemData(m_wndSwitchView.GetSelectedIndex()));
}

void RTemperatureFrame::ChangeView(WPARAM wParam, LPARAM lParam)
{
	if (m_wndImage.Image.GetDC() == NULL) return;
	
	eView CurrentView = GetCurrentView();
	m_wndImage.SetCurrentView(CurrentView);
	SetTrackBarState(CurrentView);

	DrawScene(CurrentView, m_wndDimLayer.GetTrackPos());

	m_wndImage.Refresh();
}

void RTemperatureFrame::ChangeDimLayer()
{
	if (m_wndImage.Image.GetDC() == NULL) return;

	eView CurrentView = GetCurrentView(); 
	DrawScene(CurrentView,m_wndDimLayer.GetTrackPos());

	UpdateDimantionLabel(CurrentView);
	
	m_wndImage.Refresh();
}

void RTemperatureFrame::UpdateDimantionLabel(eView CurrentView)
{
	int TrackPos = m_wndDimLayer.GetTrackPos();
	double Value;
	RString DimantionIndex;
	RString Dimantion;
	switch(CurrentView)
	{
		case ViewXY:
		case ViewXYPhase:
		{
			DimantionIndex = TEXT("k");
			Dimantion = TEXT("z");
			double DeltaZ = m_wndImage.MainRender.GetDeltaZ();
			Value = TrackPos * DeltaZ + 0.5f * DeltaZ;
			break;
		}
		case ViewXZ:
		case ViewXZPhase:
		{
			DimantionIndex = TEXT("j");
			Dimantion = TEXT("y");
			double DeltaY = m_wndImage.MainRender.GetDeltaY();
			Value = TrackPos * DeltaY + 0.5f * DeltaY;
			break;
		}
		case ViewYZ:
		case ViewYZPhase:
		{
			DimantionIndex = TEXT("i");
			Dimantion = TEXT("x");
			double DeltaX = m_wndImage.MainRender.GetDeltaX();
			Value = TrackPos * DeltaX + 0.5f * DeltaX;
			break;
		}
		default:
			m_wndCurrentDimLayer.SetText(TEXT("N/A"));
			return;
	}

	RString DimLayerText = DimantionIndex + TEXT(" = ") + RConverter::ToString(TrackPos) +
		TEXT("; ") + Dimantion + TEXT(" = ") + RConverter::ToString(Value*1000) + TEXT(" ��");
	m_wndCurrentDimLayer.SetText(DimLayerText.c_str());
}

LRESULT RTemperatureFrame::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	if (lParam == (LPARAM)m_wndDimLayer.Handle())
	{
		switch(LOWORD(wParam))
		{
			case TB_ENDTRACK:
			{
				ChangeDimLayer();
				break;
			}
		}
	}
	return 0;
}

void RTemperatureFrame::WorkStep(TemperatureWorkParam *twp)
{
	switch(twp->CurrentWorkEvent)
	{
		case TemperatureWorkParam::WorkStart:
		{
			g_wndTemperatureFrame->CurrentModuleState = InProgress;
			g_wndTemperatureFrame->UpdateStatusMsg();
			g_wndTemperatureFrame->m_wndProgress.SetMinMax(twp->Min, twp->Max);
			g_wndTemperatureFrame->m_wndProgress.SetPosition(twp->Current);
			break;
		}
		case TemperatureWorkParam::WorkProcess:
		{
			g_wndTemperatureFrame->m_wndProgress.SetPosition(twp->Current);
			break;
		}
		case TemperatureWorkParam::WorkStop:
		{
			g_wndTemperatureFrame->CurrentModuleState = InComplete;
			g_wndTemperatureFrame->UpdateStatusMsg();
			g_wndTemperatureFrame->m_wndProgress.SetMinMax(twp->Min, twp->Max);
			g_wndTemperatureFrame->m_wndProgress.SetPosition(twp->Current);
			break;
		}
	}
}

DWORD WINAPI RTemperatureFrame::StartRenderThread(LPVOID lpParam)
{
	RTemperatureFrameParam *rtfp = (RTemperatureFrameParam*)lpParam;

	g_wndTemperatureFrame->m_wndDimLayer.SetEnable(FALSE);
	g_wndTemperatureFrame->m_wndDimLayer.SetRange(0,0);
	g_wndTemperatureFrame->m_wndDimLayer.SetTrackPos(0);
	g_wndTemperatureFrame->m_wndCurrentDimLayer.SetText(TEXT("N/A"));

	g_wndTemperatureFrame->m_wndImage.
		MainRender.
		SetQStreams(rtfp->Qxl,
					rtfp->Qxr,
					rtfp->Qyl,
					rtfp->Qyr,
					rtfp->Qzb);
	
	g_wndTemperatureFrame->m_wndImage.MainRender.ManualSteps(rtfp->Dtau,rtfp->N,rtfp->M,rtfp->L);

	MetalParamStruct mps = {
		rtfp->m_cMetalInfo.m_cLambdaLiq,
		rtfp->m_cMetalInfo.m_cLambdaSol,
		rtfp->m_cMetalInfo.m_cDensityLiq,
		rtfp->m_cMetalInfo.m_cDensitySol,
		rtfp->m_cMetalInfo.m_cTempCapLiq,
		rtfp->m_cMetalInfo.m_cTempCapSol,
		rtfp->m_cMetalInfo.m_fTemperatureLiq,
		rtfp->m_cMetalInfo.m_fTemperatureLiq,
		rtfp->m_cMetalInfo.m_fBlackness,
		rtfp->m_cMetalInfo.m_fFusion,
		rtfp->m_cMetalInfo.m_fViscosity,
		rtfp->m_cMetalInfo.m_fG,
		rtfp->TEnv
	};

	g_wndTemperatureFrame->m_wndImage.MainRender.InitMetalParam(&mps);

	// ��������� ��������� �������
	g_wndTemperatureFrame->m_wndImage.MainRender.Initialization(
		rtfp->T0Liq, rtfp->T0Sol, rtfp->SolProp, rtfp->DropLetTemp);

	
	g_wndTemperatureFrame->m_wndImage.MainRender.SetStopEvent(false);
	if (!g_wndTemperatureFrame->m_wndImage.MainRender.Next(rtfp->TimeLayer,WorkStep))
	{
		g_wndTemperatureFrame->m_wndImage.Image.Clear();
		g_wndTemperatureFrame->m_wndImage.Refresh();
		g_wndTemperatureFrame->UnlockInterface();
		g_wndTemperatureFrame->hWorkThread = 0;
		g_wndTemperatureFrame->m_wndStartButton.SetText(TEXT("�����"));
		g_wndTemperatureFrame->m_wndStartButton.SetEnable(TRUE);
		return 0;
	}
	// �������� ����������� �������� ���������� ��� ���������
	HDC hdc = GetDC(g_wndTemperatureFrame->m_wndImage.Handle());
	// ������� ������� ����������� ����������� ��������
	if (!g_wndTemperatureFrame->
		m_wndImage.Image.CreateCompatible(hdc, 
		g_wndTemperatureFrame->m_wndImage.GetWidth(),
		g_wndTemperatureFrame->m_wndImage.GetHeight()))
	{
		MessageBox(g_wndTemperatureFrame->m_hHandle,TEXT("������ ��� �������� ����������� ����������� ������."),TEXT("������"), MB_OK | MB_ICONERROR);
		g_wndTemperatureFrame->hWorkThread = 0;
		return 0;
	}

	eView ViewMode = g_wndTemperatureFrame->GetCurrentView();
	g_wndTemperatureFrame->DrawScene(ViewMode,0);
	g_wndTemperatureFrame->SetTrackBarState(ViewMode);

	g_wndTemperatureFrame->m_wndImage.Refresh();
	g_wndTemperatureFrame->UnlockInterface();
	g_wndTemperatureFrame->hWorkThread = 0;
	g_wndTemperatureFrame->m_wndStartButton.SetText(TEXT("�����"));
	return 0;
}

void RTemperatureFrame::LockInterface()
{
	m_wndSwitchView.SetEnable(FALSE);
	m_wndDimLayer.SetEnable(FALSE);
	m_wndTextBoxQxl.SetEnable(FALSE);
	m_wndTextBoxQxr.SetEnable(FALSE);
	m_wndTextBoxQyl.SetEnable(FALSE);
	m_wndTextBoxQyr.SetEnable(FALSE);
	m_wndTextBoxQzb.SetEnable(FALSE);
	m_wndStepCount.SetEnable(FALSE);

	m_wndTextBoxDeltaTau.SetEnable(FALSE);
	/*
	m_wndTextBoxN.SetEnable(FALSE);
	m_wndTextBoxM.SetEnable(FALSE);
	*/
	m_wndTextBoxL.SetEnable(FALSE);

	m_wndTextBoxDropletTemp.SetEnable(FALSE);
	m_wndTextBoxT0Liq.SetEnable(FALSE);
	m_wndTextBoxT0Sol.SetEnable(FALSE);
	m_wndTextBoxSolProp.SetEnable(FALSE);
	m_wndTextBoxTEnv.SetEnable(FALSE);

}

void RTemperatureFrame::UnlockInterface()
{
	m_wndSwitchView.SetEnable(TRUE);
	m_wndDimLayer.SetEnable(TRUE);
	m_wndTextBoxQxl.SetEnable(TRUE);
	m_wndTextBoxQxr.SetEnable(TRUE);
	m_wndTextBoxQyl.SetEnable(TRUE);
	m_wndTextBoxQyr.SetEnable(TRUE);
	m_wndTextBoxQzb.SetEnable(TRUE);
	m_wndStepCount.SetEnable(TRUE);

	m_wndTextBoxDeltaTau.SetEnable(TRUE);
	/*
	m_wndTextBoxN.SetEnable(TRUE);
	m_wndTextBoxM.SetEnable(TRUE);
	*/
	m_wndTextBoxL.SetEnable(TRUE);

	m_wndTextBoxDropletTemp.SetEnable(TRUE);
	m_wndTextBoxT0Liq.SetEnable(TRUE);
	m_wndTextBoxT0Sol.SetEnable(TRUE);
	m_wndTextBoxSolProp.SetEnable(TRUE);
	m_wndTextBoxTEnv.SetEnable(TRUE);

}

void RTemperatureFrame::UpdateStatusMsg()
{
	switch(CurrentModuleState)
	{
		case InComplete:
			m_wndStatusBar.SetPanelText(0,TEXT("������"));
			break;
		case InProgress:
			m_wndStatusBar.SetPanelText(0,TEXT("���������� . . ."));
			break;
		default:
			m_wndStatusBar.SetPanelText(0,TEXT(""));
	}
}

void RTemperatureFrame::InitQzt(CloudField <double> &QztField, double Qzt)
{
	//MessageBox(NULL,RClasses::RConverter::ToString((double)Qzt).c_str(),TEXT("Msg"),MB_OK);
	m_cQzt = QztField;
	m_fQzt = Qzt;
}

void RTemperatureFrame::DrawScene(eView ViewMode, int TrackPos)
{	
	switch (ViewMode)
	{
		case ViewXY:
			m_wndImage.MainRender.DrawSceneByXY(&g_wndTemperatureFrame->m_wndImage.Image,TrackPos);
			m_wndImage.SetSightDrawing(false);
			break;
		case ViewXYPhase:
			m_wndImage.MainRender.DrawScenePhaseByXY(&g_wndTemperatureFrame->m_wndImage.Image,TrackPos);
			m_wndImage.SetSightDrawing(false);
			break;
		case ViewXZ:
			m_wndImage.MainRender.DrawSceneByXZ(&g_wndTemperatureFrame->m_wndImage.Image,TrackPos);
			m_wndImage.SetSightDrawing(false);
			break;
		case ViewXZPhase:
			m_wndImage.MainRender.DrawScenePhaseByXZ(&g_wndTemperatureFrame->m_wndImage.Image,TrackPos);
			m_wndImage.SetSightDrawing(false);
			break;
		case ViewYZ:
			m_wndImage.MainRender.DrawSceneByYZ(&g_wndTemperatureFrame->m_wndImage.Image,TrackPos);
			m_wndImage.SetSightDrawing(false);
			break;
		case ViewYZPhase:
			m_wndImage.MainRender.DrawScenePhaseByYZ(&g_wndTemperatureFrame->m_wndImage.Image,TrackPos);
			m_wndImage.SetSightDrawing(false);
			break;
		case ViewTempLine:
		{
			m_wndImage.MainRender.DrawSceneTemperatureLine(&g_wndTemperatureFrame->m_wndImage.Image);
			m_wndImage.SetSightDrawing(true);
			m_wndImage.m_cTemp_map.clear();
			std::vector <TemperaturePoint> *TempPoints = &(m_wndImage.MainRender.m_cTemperature_points);
			for (std::size_t i = 0, i_end = TempPoints->size(); i < i_end; i++)
			{
				TemperatureInfo ti = {TempPoints->operator[](i).Y,
					TempPoints->operator[](i).Temperature,
					TempPoints->operator[](i).Time};
				m_wndImage.m_cTemp_map.insert(std::make_pair(TempPoints->operator[](i).X,ti));
			}
			break;
		}
	}

	m_wndImage.SetCurrentView(ViewMode);
	m_wndImage.SetCurrentLayer(TrackPos);
}

void RTemperatureFrame::InitRenderParams()
{
	// ��������� ������� �����
	m_wndImage.MainRender.SetDimensions(
		m_cQzt.GetWidth() * m_cQzt.GetPixelWeight(),
		m_cQzt.GetHeight() * m_cQzt.GetPixelWeight(), 
		m_cMetalInfo.m_fZ);

	// ��������� �������� ��������� ������
	m_wndImage.MainRender.SetBaseQStream(m_cQzt,m_fQzt);
}

void RTemperatureFrame::SetTrackBarState(eView ViewMode)
{
	switch(ViewMode)
	{
		case ViewXY:
		case ViewXYPhase:
			m_wndDimLayer.SetRange(0,m_wndImage.MainRender.GetL() - 1);
			m_wndDimLayer.SetEnable(TRUE);
			break;
		case ViewXZ:
		case ViewXZPhase:
			m_wndDimLayer.SetRange(0,m_wndImage.MainRender.GetM() - 1);
			m_wndDimLayer.SetEnable(TRUE);
			break;
		case ViewYZ:
		case ViewYZPhase:
			m_wndDimLayer.SetRange(0,m_wndImage.MainRender.GetN() - 1);
			m_wndDimLayer.SetEnable(TRUE);
			break;
		case ViewTempLine:
			m_wndDimLayer.SetRange(0,0);
			m_wndDimLayer.SetEnable(FALSE);
			break;
	}
	m_wndDimLayer.SetTrackPos(0);
	UpdateDimantionLabel(ViewMode);
}

bool RTemperatureFrame::CheckExpressionForTOnly(Expression *expr, RString &msg)
{
	Variables vars; // ���������� � ����������
	expr->GetAllVariableNames(vars);
	if (!vars.empty())
	{
		if (vars.size() > 1)
		{
			msg.assign(TEXT("c������ ����� ����������.\n������������ ���������� ����������: t"));
			return false;
		}
		else
		{
			if (vars.begin()->GetVarName() != PARSER_TEXT("t"))
			{
				msg.assign(vars.begin()->GetVarName() + TEXT(" - �������������� ����������.\n������������ ���������� ����������: t"));
				return false;
			}
		}
	}
	return true;
}

bool RTemperatureFrame::GetModelingParams(RTemperatureFrameParam *Params)
{
	// ������ ���������
	ExpressionError ExprErr; // �������� ������ ��� �������� ���������
	RString ErrorMsg; // ������ ������� ��������� � ���� �������� ������
	if (!Params->Qxl.Initialization(m_wndTextBoxQxl.GetText(),ExprErr))
	{
		ErrorMsg.assign(TEXT("Qxl: ") + ExprErrorToStr(ExprErr));
		goto syn_error_quit;
	}

	if (!Params->Qxr.Initialization(m_wndTextBoxQxr.GetText(),ExprErr))
	{
		ErrorMsg.assign(TEXT("Qxr: ") + ExprErrorToStr(ExprErr));
		goto syn_error_quit;
	}

	if (!Params->Qyl.Initialization(m_wndTextBoxQyl.GetText(),ExprErr))
	{
		ErrorMsg.assign(TEXT("Qyl: ") + ExprErrorToStr(ExprErr));
		goto syn_error_quit;
	}

	if (!Params->Qyr.Initialization(m_wndTextBoxQyr.GetText(),ExprErr))
	{
		ErrorMsg.assign(TEXT("Qyr: ") + ExprErrorToStr(ExprErr));
		goto syn_error_quit;
	}

	if (!Params->Qzb.Initialization(m_wndTextBoxQzb.GetText(),ExprErr))
	{
		ErrorMsg.assign(TEXT("Qzb: ") + ExprErrorToStr(ExprErr));
		goto syn_error_quit;
	}

	// �������� �� �������������� ����������
	// �� ����������� ������������ ���������� �������� �� t
	if (!CheckExpressionForTOnly(&(Params->Qxl),ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Qxl: ") + ErrorMsg);
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(&(Params->Qxr),ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Qxr: ") + ErrorMsg);
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(&(Params->Qyl),ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Qyl: ") + ErrorMsg);
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(&(Params->Qyr),ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Qyr: ") + ErrorMsg);
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(&(Params->Qzb),ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Qzb: ") + ErrorMsg);
		goto syn_error_quit;
	}

	// �������� ��������� ����������
	try
	{
		Params->Dtau = RConverter::ToDouble(m_wndTextBoxDeltaTau.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxDeltaTau.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	try
	{
		Params->N = RConverter::ToInt(m_wndTextBoxN.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxN.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->N <= 0)
	{
		ErrorMsg.assign(TEXT("N ������ ���� ������������� ��������� ������!"));
		goto syn_error_quit;
	}

	try
	{
		Params->M = RConverter::ToInt(m_wndTextBoxM.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxM.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->M <= 0)
	{
		ErrorMsg.assign(TEXT("M ������ ���� ������������� ��������� ������!"));
		goto syn_error_quit;
	}

	try
	{
		Params->L = RConverter::ToInt(m_wndTextBoxL.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxL.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->L <= 0)
	{
		ErrorMsg.assign(TEXT("L ������ ���� ������������� ��������� ������!"));
		goto syn_error_quit;
	}

	try
	{
		Params->DropLetTemp = RConverter::ToDouble(m_wndTextBoxDropletTemp.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxDropletTemp.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->DropLetTemp <= 0)
	{
		ErrorMsg.assign(TEXT("DropTemp ������ ���� ������������� ������!"));
		goto syn_error_quit;
	}

	try
	{
		Params->T0Liq = RConverter::ToDouble(m_wndTextBoxT0Liq.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxT0Liq.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->T0Liq <= 0)
	{
		ErrorMsg.assign(TEXT("T0Liq ������ ���� ������������� ������!"));
		goto syn_error_quit;
	}

	try
	{
		Params->T0Sol = RConverter::ToDouble(m_wndTextBoxT0Sol.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxT0Sol.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->T0Sol <= 0)
	{
		ErrorMsg.assign(TEXT("T0Sol ������ ���� ������������� ������!"));
		goto syn_error_quit;
	}

	try
	{
		Params->SolProp = RConverter::ToDouble(m_wndTextBoxSolProp.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxSolProp.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	if (Params->SolProp < 0 || Params->SolProp > 1)
	{
		ErrorMsg.assign(TEXT("SolProp ������ ���� ������������� ������ � ��������� �� 0 �� 1!"));
		goto syn_error_quit;
	}

	try
	{
		Params->TEnv = RConverter::ToDouble(m_wndTextBoxTEnv.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndTextBoxTEnv.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	try
	{
		Params->TimeLayer = RConverter::ToInt(m_wndStepCount.GetText().c_str());
	}
	catch(RException ex)
	{
		ErrorMsg.assign(TEXT("���������� ������������� \"") + m_wndStepCount.GetText() + TEXT("\" � �����!"));
		goto syn_error_quit;
	}

	Params->m_cMetalInfo = m_cMetalInfo;
	return true;

syn_error_quit:
	MessageBox(m_hHandle,ErrorMsg.c_str(),GetText().c_str(), MB_OK | MB_ICONERROR);
	return false;
}

void RTemperatureFrame::InitMetalParam(CMetalInfo *pMetalInfo)
{
	if (pMetalInfo)
		m_cMetalInfo = *pMetalInfo;
}

int RTemperatureFrame::ShowModal(RCustomWindow *Owner)
{
	// ��� ������ ����
	LPCTSTR szTemperatureFrameClassName = TEXT("RTemperatureFrame");

	// ������������ ����
	if (!RFrame::RegisterWndClass(szTemperatureFrameClassName,CS_HREDRAW | CS_VREDRAW,CreateSolidBrush(GetSysColor(COLOR_BTNFACE)),LoadCursor(0,IDC_ARROW)))
			return 0;
	
	// ������ ����
	if (!Create(0,szTemperatureFrameClassName,TEXT("������������� ����"), WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_POPUP,Owner,0,0,800,600,NULL))
	{
		MessageBox(this->m_hHandle,TEXT("�� ���� ���������!"),TEXT("Msg"),MB_OK);
		RFrame::UnRegisterWndClass(szTemperatureFrameClassName);
		return 0;
	}

	int Result = RWindow::ShowModal();

	// �������� ����������� ����
	RFrame::UnRegisterWndClass(szTemperatureFrameClassName);

	return Result;
}

LRESULT RTemperatureFrame::N_Command(WPARAM wParam, LPARAM lParam)
{
	RString Text = m_wndTextBoxN.GetText();

	if (Text.empty())
	{
		m_wndStaticNLab.SetText(TEXT("��."));
		return 0;
	}

	int N;
	try
	{
		N = RConverter::ToInt(Text.c_str());
	}
	catch(...)
	{
		m_wndStaticNLab.SetText(TEXT("��."));
		return 0;
	}

	if (N == 0)
	{
		m_wndStaticNLab.SetText(TEXT("��."));
		return 0;
	}
	
	double Length = m_wndImage.MainRender.GetLength();

	RString ResultString = TEXT("��., \x0394x = ") + RConverter::ToString(Length/N*1000) + TEXT(" ��");
	m_wndStaticNLab.SetText(ResultString.c_str());

	return 0;
}

LRESULT RTemperatureFrame::M_Command(WPARAM wParam, LPARAM lParam)
{
	RString Text = m_wndTextBoxM.GetText();

	if (Text.empty())
	{
		m_wndStaticMLab.SetText(TEXT("��."));
		return 0;
	}

	int M;
	try
	{
		M = RConverter::ToInt(Text.c_str());
	}
	catch(...)
	{
		m_wndStaticMLab.SetText(TEXT("��."));
		return 0;
	}

	if (M == 0)
	{
		m_wndStaticMLab.SetText(TEXT("��."));
		return 0;
	}

	double Width = m_wndImage.MainRender.GetWidth();

	RString ResultString = TEXT("��., \x0394y = ") + RConverter::ToString(Width/M*1000) + TEXT(" ��");
	m_wndStaticMLab.SetText(ResultString.c_str());

	return 0;
}

LRESULT RTemperatureFrame::L_Command(WPARAM wParam, LPARAM lParam)
{
	RString Text = m_wndTextBoxL.GetText();

	if (Text.empty())
	{
		m_wndStaticLLab.SetText(TEXT("��."));
		return 0;
	}

	int L;
	try
	{
		L = RConverter::ToInt(Text.c_str());
	}
	catch(...)
	{
		m_wndStaticLLab.SetText(TEXT("��."));
		return 0;
	}

	if (L == 0)
	{
		m_wndStaticLLab.SetText(TEXT("��."));
		return 0;
	}

	double Height = m_wndImage.MainRender.GetHeight();

	RString ResultString = TEXT("��., \x0394z = ") + RConverter::ToString(Height/L*1000) + TEXT(" ��");
	m_wndStaticLLab.SetText(ResultString.c_str());

	return 0;
}

LRESULT RTemperatureFrame::TimeChange(WPARAM wParam, LPARAM lParam)
{
	
	RString TextDTau = m_wndTextBoxDeltaTau.GetText();
	RString TextStep = m_wndStepCount.GetText();

	if (TextDTau.empty() || TextStep.empty())
	{
		m_wndLabelTime.SetText(TEXT("����� ������: N/�"));
		return 0;
	}

	int nStep;
	try
	{
		nStep = RConverter::ToInt(TextStep.c_str());
	}
	catch(...)
	{
		m_wndLabelTime.SetText(TEXT("����� ������: N/�"));
		return 0;
	}

	double fDTau;
	try
	{
		fDTau = RConverter::ToDouble(TextDTau.c_str());
	}
	catch(...)
	{
		m_wndLabelTime.SetText(TEXT("����� ������: N/�"));
		return 0;
	}

	RString ResultString = TEXT("����� ������: ") + RConverter::ToString(fDTau*nStep) + TEXT(" ���.");
	m_wndLabelTime.SetText(ResultString.c_str());

	return 0;
}