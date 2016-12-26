#include "RModelParamFrame.h"

#define button_id_ok 1
#define button_id_cancel 2

RModelParamFrame::RModelParamFrame()
	: RFrame()
{
	AddMessage(WM_CREATE,MSG_EVENT_CAST(RModelParamFrame::OnCreate),this);
	AddMessage(WM_CLOSE,MSG_EVENT_CAST(RModelParamFrame::OnClose),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RModelParamFrame::OnSize),this);
	AddMessage(WM_DESTROY,MSG_EVENT_CAST(RModelParamFrame::OnDestroy),this);
	AddMessage(WM_COMMAND,MSG_EVENT_CAST(RModelParamFrame::OnCommand),this);
	AddMessage(WM_SETFOCUS,MSG_EVENT_CAST(RModelParamFrame::OnFocus),this);
}

RModelParamFrame::~RModelParamFrame()
{

}

void RModelParamFrame::VisualStyles()
{
	HFONT hDefaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	m_wndMetalBox.SetFont(hDefaultFont);
	m_wndStaticMetalLambdaL.SetFont(hDefaultFont);
	m_wndTextBoxMetalLambdaL.SetFont(hDefaultFont);
	m_wndStaticMetalLambdaLabL.SetFont(hDefaultFont);
	m_wndStaticMetalDensityL.SetFont(hDefaultFont);
	m_wndTextBoxMetalDensityL.SetFont(hDefaultFont);
	m_wndStaticMetalDensityLabL.SetFont(hDefaultFont);
	m_wndStaticMetalThermalCapL.SetFont(hDefaultFont);
	m_wndTextBoxMetalThermalCapL.SetFont(hDefaultFont);
	m_wndStaticMetalThermalCapLabL.SetFont(hDefaultFont);
	m_wndStaticMetalTempLiq.SetFont(hDefaultFont);
	m_wndTextBoxMetalTempLiq.SetFont(hDefaultFont);
	m_wndStaticMetalTempLiqLab.SetFont(hDefaultFont);
	m_wndStaticMetalBlackness.SetFont(hDefaultFont);
	m_wndTextBoxMetalBlackness.SetFont(hDefaultFont);
	m_wndStaticMetalBlacknessLab.SetFont(hDefaultFont);

	m_wndStaticMetalViscosity.SetFont(hDefaultFont);
	m_wndTextBoxMetalViscosity.SetFont(hDefaultFont);
	m_wndStaticMetalViscosityLab.SetFont(hDefaultFont);

	m_wndStaticMetalLambdaH.SetFont(hDefaultFont);
	m_wndTextBoxMetalLambdaH.SetFont(hDefaultFont);
	m_wndStaticMetalLambdaLabH.SetFont(hDefaultFont);
	m_wndStaticMetalDensityH.SetFont(hDefaultFont);
	m_wndTextBoxMetalDensityH.SetFont(hDefaultFont);
	m_wndStaticMetalDensityLabH.SetFont(hDefaultFont);
	m_wndStaticMetalThermalCapH.SetFont(hDefaultFont);
	m_wndTextBoxMetalThermalCapH.SetFont(hDefaultFont);
	m_wndStaticMetalThermalCapLabH.SetFont(hDefaultFont);
	m_wndStaticMetalTempSol.SetFont(hDefaultFont);
	m_wndTextBoxMetalTempSol.SetFont(hDefaultFont);
	m_wndStaticMetalTempSolLab.SetFont(hDefaultFont);
	m_wndStaticMetalFusion.SetFont(hDefaultFont);
	m_wndTextBoxMetalFusion.SetFont(hDefaultFont);
	m_wndStaticMetalFusionLab.SetFont(hDefaultFont);

	m_wndOvenBox.SetFont(hDefaultFont);
	m_wndStaticOvenX.SetFont(hDefaultFont);
	m_wndTextBoxOvenX.SetFont(hDefaultFont);
	m_wndStaticOvenXLab.SetFont(hDefaultFont);
	m_wndStaticOvenY.SetFont(hDefaultFont);
	m_wndTextBoxOvenY.SetFont(hDefaultFont);
	m_wndStaticOvenYLab.SetFont(hDefaultFont);
	m_wndStaticOvenZ.SetFont(hDefaultFont);
	m_wndTextBoxOvenZ.SetFont(hDefaultFont);
	m_wndStaticOvenZLab.SetFont(hDefaultFont);
	m_wndStaticOvenAmpTraffic.SetFont(hDefaultFont);
	m_wndTextBoxOvenAmpTraffic.SetFont(hDefaultFont);
	m_wndStaticOvenAmpTrafficLab.SetFont(hDefaultFont);
	m_wndStaticOvenG.SetFont(hDefaultFont);
	m_wndTextBoxOvenG.SetFont(hDefaultFont);
	m_wndStaticOvenGLab.SetFont(hDefaultFont);

	m_wndStaticOvenEfficiency.SetFont(hDefaultFont);
	m_wndTextBoxOvenEfficiency.SetFont(hDefaultFont);
	m_wndStaticOvenEfficiencyLab.SetFont(hDefaultFont);

	m_wndButtonOk.SetFont(hDefaultFont);
	m_wndButtonCancel.SetFont(hDefaultFont);

	// установка подсказок
	m_wndTextBoxMetalLambdaL.AddToolTip(TEXT("Коэффициент теплопроводности жидкого металла"));
	m_wndTextBoxMetalDensityL.AddToolTip(TEXT("Плотность жидкого металла"));
	m_wndTextBoxMetalThermalCapL.AddToolTip(TEXT("Теплоемкость жидкого металла"));
	m_wndTextBoxMetalTempLiq.AddToolTip(TEXT("Температура плавления"));
	m_wndTextBoxMetalBlackness.AddToolTip(TEXT("Степень черноты"));
	m_wndTextBoxMetalLambdaH.AddToolTip(TEXT("Коэффициент теплопроводности твердого металла"));
	m_wndTextBoxMetalViscosity.AddToolTip(TEXT("Кинематическая вязкость титана"));
	m_wndTextBoxMetalDensityH.AddToolTip(TEXT("Плотность твердого металла"));
	m_wndTextBoxMetalThermalCapH.AddToolTip(TEXT("Теплоемкость твердого металла"));
	m_wndTextBoxMetalTempSol.AddToolTip(TEXT("Температура затвердевания"));
	m_wndTextBoxMetalFusion.AddToolTip(TEXT("Теплота плавления"));
	m_wndTextBoxOvenX.AddToolTip(TEXT("Длина ванны промежуточной емкости"));
	m_wndTextBoxOvenY.AddToolTip(TEXT("Ширина ванны промежуточной емкости"));
	m_wndTextBoxOvenZ.AddToolTip(TEXT("Высота ванны промежуточной емкости"));
	m_wndTextBoxOvenAmpTraffic.AddToolTip(TEXT("Полная проводимость ЭЛ пушки"));
	m_wndTextBoxOvenG.AddToolTip(TEXT("Максимальная скорость поступления шихты"));
	m_wndTextBoxOvenEfficiency.AddToolTip(TEXT("КПД электронного луча"));


	
	m_wndTextBoxMetalTempLiq.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxMetalTempSol.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxMetalViscosity.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxMetalFusion.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxOvenX.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxOvenY.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxOvenZ.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxOvenAmpTraffic.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxOvenG.SetFilter(RTextBoxEx::RealFilter);
	m_wndTextBoxOvenEfficiency.SetFilter(RTextBoxEx::RealFilter);

	m_wndTextBoxMetalTempLiq.SetSignInput(false);
	m_wndTextBoxMetalTempSol.SetSignInput(false);
	m_wndTextBoxMetalViscosity.SetSignInput(false);
	m_wndTextBoxMetalFusion.SetSignInput(false);
	m_wndTextBoxOvenX.SetSignInput(false);
	m_wndTextBoxOvenY.SetSignInput(false);
	m_wndTextBoxOvenZ.SetSignInput(false);
	m_wndTextBoxOvenAmpTraffic.SetSignInput(false);
	m_wndTextBoxOvenG.SetSignInput(false);
	m_wndTextBoxOvenEfficiency.SetSignInput(false);
}


int RModelParamFrame::ShowModal(RFrame *Owner)
{
	LPCTSTR szRModelParamFrameClassName = TEXT("RModelParamFrame");

		// регистрируем окно
	if (!RFrame::RegisterWndClass(szRModelParamFrameClassName,CS_HREDRAW | CS_VREDRAW,CreateSolidBrush(GetSysColor(COLOR_BTNFACE)),LoadCursor(0,IDC_ARROW)))
			return 0;

		// создаём окно
	if (!Create(0,szRModelParamFrameClassName,TEXT("Дополнительные параметры"), WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_POPUP,Owner,0,0,480,350,NULL))
	{
		MessageBox(this->m_hHandle,TEXT("Не могу запустить!"),TEXT("Msg"),MB_OK);
		RFrame::UnRegisterWndClass(szRModelParamFrameClassName);
		return 0;
	}

	int Result = RFrame::ShowModal();

	// разрегистрируем окно
	RFrame::UnRegisterWndClass(szRModelParamFrameClassName);

	return Result;
}

void RModelParamFrame::setMetalInfo(CMetalInfo *MetalInfo)
{
	if (!MetalInfo)
		return;
	m_cMetalInfo = *MetalInfo;
}

void RModelParamFrame::getMetalInfo(CMetalInfo *MetalInfo)
{
	if (!MetalInfo)
		return;
	 *MetalInfo = m_cMetalInfo;
}

LRESULT RModelParamFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
	LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;

	// установка окна на середину относительно родителя
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
	

	if(!m_wndMetalBox.Create(TEXT("Параметры металла"),this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalLambdaL.Create(TEXT("\x03BB ж:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalLambdaL.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalLambdaLabL.Create(TEXT("Вт/(м*К)"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalDensityL.Create(TEXT("\x03C1 ж:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalDensityL.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalDensityLabL.Create(TEXT("кг/м3"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalThermalCapL.Create(TEXT("C ж:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalThermalCapL.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalThermalCapLabL.Create(TEXT("Дж/(кг*K)"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalTempLiq.Create(TEXT("T liq:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalTempLiq.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalTempLiqLab.Create(TEXT("K"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalBlackness.Create(TEXT("\x03B5:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalBlackness.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalBlacknessLab.Create(TEXT("ед."), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;


	if (!m_wndStaticMetalLambdaH.Create(TEXT("\x03BB тв:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalLambdaH.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalLambdaLabH.Create(TEXT("Вт/(м*К)"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalDensityH.Create(TEXT("\x03C1 тв:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalDensityH.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalDensityLabH.Create(TEXT("кг/м3"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalThermalCapH.Create(TEXT("C тв:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalThermalCapH.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalThermalCapLabH.Create(TEXT("Дж/(кг*K)"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalTempSol.Create(TEXT("T sol:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalTempSol.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalTempSolLab.Create(TEXT("K"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalFusion.Create(TEXT("L:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalFusion.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalFusionLab.Create(TEXT("Дж/кг"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalViscosity.Create(TEXT("\x03BD:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxMetalViscosity.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticMetalViscosityLab.Create(TEXT("сСт"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;


	if (!m_wndOvenBox.Create(TEXT("Параметры печи"),this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenX.Create(TEXT("А:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxOvenX.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenXLab.Create(TEXT("м"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenY.Create(TEXT("B:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxOvenY.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenYLab.Create(TEXT("м"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenZ.Create(TEXT("Н:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxOvenZ.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenZLab.Create(TEXT("м"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenAmpTraffic.Create(TEXT("k:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxOvenAmpTraffic.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenAmpTrafficLab.Create(TEXT("А/kВ^(3/2)"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenG.Create(TEXT("G:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxOvenG.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenGLab.Create(TEXT("т/час"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenEfficiency.Create(TEXT("кпд:"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;

	if (!m_wndTextBoxOvenEfficiency.Create(WS_VISIBLE | WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,this,0,0,0,0,0))
		return -1;

	if (!m_wndStaticOvenEfficiencyLab.Create(TEXT("%"), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,this,0,0,0,0,0))
		return -1;


	if (!m_wndButtonOk.Create(TEXT("Ok"),this,button_id_ok))
		return -1;

	if (!m_wndButtonCancel.Create(TEXT("Отмена"),this,button_id_cancel))
		return -1;


	TabList.Append(&m_wndTextBoxMetalLambdaL);
	TabList.Append(&m_wndTextBoxMetalLambdaH);
	TabList.Append(&m_wndTextBoxMetalDensityL);
	TabList.Append(&m_wndTextBoxMetalDensityH);
	TabList.Append(&m_wndTextBoxMetalThermalCapL);
	TabList.Append(&m_wndTextBoxMetalThermalCapH);
	TabList.Append(&m_wndTextBoxMetalTempLiq);
	TabList.Append(&m_wndTextBoxMetalTempSol);
	TabList.Append(&m_wndTextBoxMetalBlackness);
	TabList.Append(&m_wndTextBoxMetalFusion);
	TabList.Append(&m_wndTextBoxMetalViscosity);

	TabList.Append(&m_wndTextBoxOvenX);
	TabList.Append(&m_wndTextBoxOvenY);
	TabList.Append(&m_wndTextBoxOvenZ);
	TabList.Append(&m_wndTextBoxOvenAmpTraffic);
	TabList.Append(&m_wndTextBoxOvenG);


	TabList.Append(&m_wndButtonOk);
	TabList.Append(&m_wndButtonCancel);

	VisualStyles();

	// переместить код
	{
		m_wndTextBoxMetalLambdaL.SetText(m_cMetalInfo.m_cLambdaLiq.ToString().c_str());
		m_wndTextBoxMetalLambdaH.SetText(m_cMetalInfo.m_cLambdaSol.ToString().c_str());

		m_wndTextBoxMetalDensityL.SetText(m_cMetalInfo.m_cDensityLiq.ToString().c_str());
		m_wndTextBoxMetalDensityH.SetText(m_cMetalInfo.m_cDensitySol.ToString().c_str());

		m_wndTextBoxMetalThermalCapL.SetText(m_cMetalInfo.m_cTempCapLiq.ToString().c_str());
		m_wndTextBoxMetalThermalCapH.SetText(m_cMetalInfo.m_cTempCapSol.ToString().c_str());

		m_wndTextBoxMetalTempLiq.SetText(RConverter::ToString(m_cMetalInfo.m_fTemperatureLiq).c_str());
		m_wndTextBoxMetalTempSol.SetText(RConverter::ToString(m_cMetalInfo.m_fTemperatureSol).c_str());

		m_wndTextBoxMetalBlackness.SetText(RConverter::ToString(m_cMetalInfo.m_fBlackness).c_str());
		m_wndTextBoxMetalFusion.SetText(RConverter::ToString(m_cMetalInfo.m_fFusion).c_str());
		m_wndTextBoxMetalViscosity.SetText(RConverter::ToString(m_cMetalInfo.m_fViscosity).c_str());

		m_wndTextBoxOvenX.SetText(RConverter::ToString(m_cMetalInfo.m_fX).c_str());
		m_wndTextBoxOvenY.SetText(RConverter::ToString(m_cMetalInfo.m_fY).c_str());
		m_wndTextBoxOvenZ.SetText(RConverter::ToString(m_cMetalInfo.m_fZ).c_str());

		m_wndTextBoxOvenAmpTraffic.SetText(RConverter::ToString(m_cMetalInfo.m_fJ).c_str());
		m_wndTextBoxOvenG.SetText(RConverter::ToString(m_cMetalInfo.m_fG).c_str());
		m_wndTextBoxOvenEfficiency.SetText(RConverter::ToString(m_cMetalInfo.m_fEfficiency*100).c_str());
	}
	return 0;
}

LRESULT RModelParamFrame::OnSize(WPARAM wParam, LPARAM lParam)
{
	int ControlIndent = 5;
	int StandartControlHeight = 20;
	int GroupBoxHeight = 170;

	int ClientWidth = LOWORD(lParam);
	int ClientHeight = HIWORD(lParam);

	int MetalBoxWidth = ClientWidth - (ControlIndent << 1);

	int ButtonHeight = 22;
	int ButtonWidth = 80;

	// изменение позиции контролов характеризующиъ металл
	{
		int MetalBoxLeft = ControlIndent;
		int MetalBoxTop = ControlIndent;
		m_wndMetalBox.SetWindowPosition(NULL, MetalBoxLeft, MetalBoxTop, MetalBoxWidth, GroupBoxHeight, SWP_NOACTIVATE);
	
		int MetalBoxStaticWidth = 25;
		int MetalBoxStaticWidthLab = 60;
		int MetalBoxStaticLeft1 = MetalBoxLeft + ControlIndent;
		int MetalBoxControlTop = MetalBoxTop + 16;

		int MetalBoxTextBoxWidth = (MetalBoxWidth - (7 * ControlIndent + 2*MetalBoxStaticWidth + 2*MetalBoxStaticWidthLab)) / 2;
		int MetalBoxTextBoxLeft1 = MetalBoxLeft + 2 * ControlIndent + MetalBoxStaticWidth;

		int MetalBoxControlTopTmp = MetalBoxControlTop;
		m_wndStaticMetalLambdaL.SetWindowPosition(NULL,MetalBoxStaticLeft1,MetalBoxControlTopTmp,MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalDensityL.SetWindowPosition(NULL,MetalBoxStaticLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalThermalCapL.SetWindowPosition(NULL,MetalBoxStaticLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalTempLiq.SetWindowPosition(NULL,MetalBoxStaticLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalBlackness.SetWindowPosition(NULL,MetalBoxStaticLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalViscosity.SetWindowPosition(NULL,MetalBoxStaticLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);

		MetalBoxControlTopTmp = MetalBoxControlTop;
		m_wndTextBoxMetalLambdaL.SetWindowPosition(NULL,MetalBoxTextBoxLeft1,MetalBoxControlTopTmp,MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalDensityL.SetWindowPosition(NULL,MetalBoxTextBoxLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalThermalCapL.SetWindowPosition(NULL,MetalBoxTextBoxLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalTempLiq.SetWindowPosition(NULL,MetalBoxTextBoxLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalBlackness.SetWindowPosition(NULL,MetalBoxTextBoxLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalViscosity.SetWindowPosition(NULL,MetalBoxTextBoxLeft1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int MetalBoxStaticLeftLab1 = MetalBoxLeft + ControlIndent + MetalBoxStaticWidth + ControlIndent + MetalBoxTextBoxWidth + ControlIndent;

		MetalBoxControlTopTmp = MetalBoxControlTop;
		m_wndStaticMetalLambdaLabL.SetWindowPosition(NULL,MetalBoxStaticLeftLab1,MetalBoxControlTopTmp,MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalDensityLabL.SetWindowPosition(NULL,MetalBoxStaticLeftLab1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalThermalCapLabL.SetWindowPosition(NULL,MetalBoxStaticLeftLab1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalTempLiqLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalBlacknessLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalViscosityLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab1,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);

		int MetalBoxStaticLeft2 = MetalBoxLeft + ControlIndent + MetalBoxStaticWidth + ControlIndent + MetalBoxTextBoxWidth + ControlIndent + MetalBoxStaticWidthLab + ControlIndent;
	
		MetalBoxControlTopTmp = MetalBoxControlTop;
		m_wndStaticMetalLambdaH.SetWindowPosition(NULL,MetalBoxStaticLeft2,MetalBoxControlTopTmp,MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalDensityH.SetWindowPosition(NULL,MetalBoxStaticLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalThermalCapH.SetWindowPosition(NULL,MetalBoxStaticLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalTempSol.SetWindowPosition(NULL,MetalBoxStaticLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalFusion.SetWindowPosition(NULL,MetalBoxStaticLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);

		int MetalBoxTextBoxLeft2 = MetalBoxLeft + ControlIndent + MetalBoxStaticWidth + ControlIndent + MetalBoxTextBoxWidth + ControlIndent + MetalBoxStaticWidthLab + ControlIndent + MetalBoxStaticWidth + ControlIndent;

		MetalBoxControlTopTmp = MetalBoxControlTop;
		m_wndTextBoxMetalLambdaH.SetWindowPosition(NULL,MetalBoxTextBoxLeft2,MetalBoxControlTopTmp,MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalDensityH.SetWindowPosition(NULL,MetalBoxTextBoxLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalThermalCapH.SetWindowPosition(NULL,MetalBoxTextBoxLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalTempSol.SetWindowPosition(NULL,MetalBoxTextBoxLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxMetalFusion.SetWindowPosition(NULL,MetalBoxTextBoxLeft2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int MetalBoxStaticLeftLab2 = MetalBoxLeft + ControlIndent + MetalBoxStaticWidth + ControlIndent + MetalBoxTextBoxWidth + ControlIndent + MetalBoxStaticWidthLab + ControlIndent + MetalBoxStaticWidth + ControlIndent + MetalBoxTextBoxWidth + ControlIndent;

		MetalBoxControlTopTmp = MetalBoxControlTop;
		m_wndStaticMetalLambdaLabH.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,MetalBoxControlTopTmp,MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalDensityLabH.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalThermalCapLabH.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalTempSolLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticMetalFusionLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,MetalBoxControlTopTmp += (StandartControlHeight + ControlIndent),MetalBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
	}

	// изменение позиции контролов характеризующиъ металл
	{
		int OvenBoxLeft = ControlIndent;
		int OvenBoxTop = GroupBoxHeight + 2*ControlIndent;
		m_wndOvenBox.SetWindowPosition(NULL, OvenBoxLeft, OvenBoxTop, MetalBoxWidth, 100, SWP_NOACTIVATE);
		
		
		int OvenBoxStaticWidth = 25;
		int OvenBoxStaticWidthLab = 60;
		int OvenBoxStaticLeft1 = OvenBoxLeft + ControlIndent;
		int OvenBoxControlTop = OvenBoxTop + 16;

		int OvenBoxTextBoxWidth = (MetalBoxWidth - (7 * ControlIndent + 2*OvenBoxStaticWidth + 2*OvenBoxStaticWidthLab)) / 2;
		int OvenBoxTextBoxLeft1 = OvenBoxLeft + 2 * ControlIndent + OvenBoxStaticWidth;

		int OvenBoxControlTopTmp = OvenBoxControlTop;
		m_wndStaticOvenX.SetWindowPosition(NULL,OvenBoxStaticLeft1,OvenBoxControlTopTmp,OvenBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenY.SetWindowPosition(NULL,OvenBoxStaticLeft1,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenZ.SetWindowPosition(NULL,OvenBoxStaticLeft1,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		
		
		OvenBoxControlTopTmp = OvenBoxControlTop;
		m_wndTextBoxOvenX.SetWindowPosition(NULL,OvenBoxTextBoxLeft1,OvenBoxControlTopTmp,OvenBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxOvenY.SetWindowPosition(NULL,OvenBoxTextBoxLeft1,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxOvenZ.SetWindowPosition(NULL,OvenBoxTextBoxLeft1,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int OvenBoxStaticLeftLab1 = OvenBoxLeft + ControlIndent + OvenBoxStaticWidth + ControlIndent + OvenBoxTextBoxWidth + ControlIndent;

		OvenBoxControlTopTmp = OvenBoxControlTop;
		m_wndStaticOvenXLab.SetWindowPosition(NULL,OvenBoxStaticLeftLab1,OvenBoxControlTopTmp,OvenBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenYLab.SetWindowPosition(NULL,OvenBoxStaticLeftLab1,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenZLab.SetWindowPosition(NULL,OvenBoxStaticLeftLab1,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		
		
		int OvenBoxStaticLeft2 = OvenBoxLeft + ControlIndent + OvenBoxStaticWidth + ControlIndent + OvenBoxTextBoxWidth + ControlIndent + OvenBoxStaticWidthLab + ControlIndent;
	
		OvenBoxControlTopTmp = OvenBoxControlTop;
		m_wndStaticOvenAmpTraffic.SetWindowPosition(NULL,OvenBoxStaticLeft2,OvenBoxControlTopTmp,OvenBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenG.SetWindowPosition(NULL,OvenBoxStaticLeft2,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenEfficiency.SetWindowPosition(NULL,OvenBoxStaticLeft2,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidth,StandartControlHeight,SWP_NOACTIVATE);

		int OvenBoxTextBoxLeft2 = OvenBoxLeft + ControlIndent + OvenBoxStaticWidth + ControlIndent + OvenBoxTextBoxWidth + ControlIndent + OvenBoxStaticWidthLab + ControlIndent + OvenBoxStaticWidth + ControlIndent;

		OvenBoxControlTopTmp = OvenBoxControlTop;
		m_wndTextBoxOvenAmpTraffic.SetWindowPosition(NULL,OvenBoxTextBoxLeft2,OvenBoxControlTopTmp,OvenBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxOvenG.SetWindowPosition(NULL,OvenBoxTextBoxLeft2,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);
		m_wndTextBoxOvenEfficiency.SetWindowPosition(NULL,OvenBoxTextBoxLeft2,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxTextBoxWidth,StandartControlHeight,SWP_NOACTIVATE);

		int MetalBoxStaticLeftLab2 = OvenBoxLeft + ControlIndent + OvenBoxStaticWidth + ControlIndent + OvenBoxTextBoxWidth + ControlIndent + OvenBoxStaticWidthLab + ControlIndent + OvenBoxStaticWidth + ControlIndent + OvenBoxTextBoxWidth + ControlIndent;

		OvenBoxControlTopTmp = OvenBoxControlTop;
		m_wndStaticOvenAmpTrafficLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,OvenBoxControlTopTmp,OvenBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenGLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
		m_wndStaticOvenEfficiencyLab.SetWindowPosition(NULL,MetalBoxStaticLeftLab2,OvenBoxControlTopTmp += (StandartControlHeight + ControlIndent),OvenBoxStaticWidthLab,StandartControlHeight,SWP_NOACTIVATE);
	}

	m_wndButtonOk.SetWindowPosition(NULL, ClientWidth - 2*ButtonWidth - 2*ControlIndent,ClientHeight - ButtonHeight - ControlIndent,ButtonWidth,ButtonHeight,SWP_NOACTIVATE); 
	m_wndButtonCancel.SetWindowPosition(NULL, ClientWidth - ButtonWidth - ControlIndent,ClientHeight - ButtonHeight - ControlIndent,ButtonWidth,ButtonHeight,SWP_NOACTIVATE); 

	return 0;
}

LRESULT RModelParamFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(m_hHandle,WM_DESTROY,wParam,lParam);
}

LRESULT RModelParamFrame::OnClose(WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT RModelParamFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int nSender_id = LOWORD(wParam);
	switch(nSender_id)
	{
		case button_id_ok:
			if (ParseParams()) PostQuitMessage(1);
				break;
		case button_id_cancel:
			PostQuitMessage(0);
	}
	return 0;
}

bool RModelParamFrame::ParseParams()
{
	// строим выражения
	ExpressionError ExprErr; // описание ошибки для парсинга выражений
	RString ErrorMsg; // строка которая принимает в себя описание ошибки
	RCustomWindow *pFocusWnd(0);
	if (!m_cMetalInfo.m_cLambdaLiq.Initialization(m_wndTextBoxMetalLambdaL.GetText(),ExprErr))
	{
		ErrorMsg.assign(ExprErrorToStr(ExprErr));
		pFocusWnd = &m_wndTextBoxMetalLambdaL;
		goto syn_error_quit;
	}

	if (!m_cMetalInfo.m_cLambdaSol.Initialization(m_wndTextBoxMetalLambdaH.GetText(),ExprErr))
	{
		ErrorMsg.assign(ExprErrorToStr(ExprErr));
		pFocusWnd = &m_wndTextBoxMetalLambdaH;
		goto syn_error_quit;
	}

	if (!m_cMetalInfo.m_cDensityLiq.Initialization(m_wndTextBoxMetalDensityL.GetText(),ExprErr))
	{
		ErrorMsg.assign(ExprErrorToStr(ExprErr));
		pFocusWnd = &m_wndTextBoxMetalDensityL;
		goto syn_error_quit;
	}

	if (!m_cMetalInfo.m_cDensitySol.Initialization(m_wndTextBoxMetalDensityH.GetText(),ExprErr))
	{
		ErrorMsg.assign(ExprErrorToStr(ExprErr));
		pFocusWnd = &m_wndTextBoxMetalDensityH;
		goto syn_error_quit;
	}

	if (!m_cMetalInfo.m_cTempCapLiq.Initialization(m_wndTextBoxMetalThermalCapL.GetText(),ExprErr))
	{
		ErrorMsg.assign(ExprErrorToStr(ExprErr));
		pFocusWnd = &m_wndTextBoxMetalThermalCapL;
		goto syn_error_quit;
	}

	if (!m_cMetalInfo.m_cTempCapSol.Initialization(m_wndTextBoxMetalThermalCapH.GetText(),ExprErr))
	{
		ErrorMsg.assign(ExprErrorToStr(ExprErr));
		pFocusWnd = &m_wndTextBoxMetalThermalCapH;
		goto syn_error_quit;
	}

	// проверка на использованные переменные
	// не допускается использовать переменные отличные от t
	if (!CheckExpressionForTOnly(m_cMetalInfo.m_cLambdaLiq,ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Lambda Liq: ") + ErrorMsg);
		pFocusWnd = &m_wndTextBoxMetalLambdaL;
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(m_cMetalInfo.m_cLambdaSol,ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Lambda Sol: ") + ErrorMsg);
		pFocusWnd = &m_wndTextBoxMetalLambdaH;
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(m_cMetalInfo.m_cDensityLiq,ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Density Liq: ") + ErrorMsg);
		pFocusWnd = &m_wndTextBoxMetalDensityL;
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(m_cMetalInfo.m_cDensitySol,ErrorMsg))
	{
		ErrorMsg.assign(TEXT("Density Sol: ") + ErrorMsg);
		pFocusWnd = &m_wndTextBoxMetalDensityH;
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(m_cMetalInfo.m_cTempCapLiq,ErrorMsg))
	{
		ErrorMsg.assign(TEXT("TempCap Liq: ") + ErrorMsg);
		pFocusWnd = &m_wndTextBoxMetalThermalCapL;
		goto syn_error_quit;
	}

	if (!CheckExpressionForTOnly(m_cMetalInfo.m_cTempCapSol,ErrorMsg))
	{
		ErrorMsg.assign(TEXT("TempCap Sol: ") + ErrorMsg);
		pFocusWnd = &m_wndTextBoxMetalThermalCapH;
		goto syn_error_quit;
	}

	// получаем численные значения
	try 
	{
		m_cMetalInfo.m_fTemperatureLiq = RConverter::ToDouble(m_wndTextBoxMetalTempLiq.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("T liq: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxMetalTempLiq;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fTemperatureLiq <= 0 || m_cMetalInfo.m_fTemperatureLiq > 5000)
	{
		ErrorMsg.assign(TEXT("T liq: температура должна лежать в промежутке от 1 до 5000"));
		pFocusWnd = &m_wndTextBoxMetalTempLiq;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fTemperatureSol = RConverter::ToDouble(m_wndTextBoxMetalTempSol.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("T sol: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxMetalTempSol;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fTemperatureSol <= 0 || m_cMetalInfo.m_fTemperatureSol > 5000)
	{
		ErrorMsg.assign(TEXT("T sol: температура должна лежать в промежутке от 1 до 5000"));
		pFocusWnd = &m_wndTextBoxMetalTempSol;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fBlackness = RConverter::ToDouble(m_wndTextBoxMetalBlackness.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("\x03B5: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxMetalBlackness;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fBlackness < 0 || m_cMetalInfo.m_fBlackness > 1)
	{
		ErrorMsg.assign(TEXT("\x03B5: Степень черноты должна лежать в промежутке от 0 до 1"));
		pFocusWnd = &m_wndTextBoxMetalBlackness;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fFusion = RConverter::ToDouble(m_wndTextBoxMetalFusion.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("L: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxMetalFusion;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fFusion < 1 || m_cMetalInfo.m_fFusion > 1000000)
	{
		ErrorMsg.assign(TEXT("L: Теплота плавления должна лежать в промежутке от 1 до 1000000"));
		pFocusWnd = &m_wndTextBoxMetalFusion;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fViscosity = RConverter::ToDouble(m_wndTextBoxMetalViscosity.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("\x03BD: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxMetalViscosity;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fViscosity <= 0 || m_cMetalInfo.m_fViscosity > 100)
	{
		ErrorMsg.assign(TEXT("\x03BD: Кинематическая вязкость должна лежать в промежутке от 0 до 100"));
		pFocusWnd = &m_wndTextBoxMetalViscosity;
		goto syn_error_quit;
	}

	try
	{
		m_cMetalInfo.m_fX = RConverter::ToDouble(m_wndTextBoxOvenX.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("A: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxOvenX;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fX <= 0 || m_cMetalInfo.m_fX > 10)
	{
		ErrorMsg.assign(TEXT("А: Длина ванны должна быть в пределах от 0 до 10"));
		pFocusWnd = &m_wndTextBoxOvenX;
		goto syn_error_quit;
	}

	try
	{
		m_cMetalInfo.m_fY = RConverter::ToDouble(m_wndTextBoxOvenY.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("B: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxOvenY;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fY <= 0 || m_cMetalInfo.m_fY > 10)
	{
		ErrorMsg.assign(TEXT("B: Ширина ванны должна быть в пределах от 0 до 10"));
		pFocusWnd = &m_wndTextBoxOvenY;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fZ = RConverter::ToDouble(m_wndTextBoxOvenZ.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("H: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxOvenZ;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fZ <= 0 || m_cMetalInfo.m_fZ > 10)
	{
		ErrorMsg.assign(TEXT("H: Высота ванны должна быть в пределах от 0 до 10"));
		pFocusWnd = &m_wndTextBoxOvenZ;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fJ = RConverter::ToDouble(m_wndTextBoxOvenAmpTraffic.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("k: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxOvenAmpTraffic;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fJ <= 0 || m_cMetalInfo.m_fJ > 1)
	{
		ErrorMsg.assign(TEXT("k: Полная проводимость должна быть в пределах от 0 до 1"));
		pFocusWnd = &m_wndTextBoxOvenAmpTraffic;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fG = RConverter::ToDouble(m_wndTextBoxOvenG.GetText().c_str());
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("G: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxOvenG;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fG <= 0 || m_cMetalInfo.m_fG > 100)
	{
		ErrorMsg.assign(TEXT("G: Скорость плавления должна быть в пределах от 0 до 100"));
		pFocusWnd = &m_wndTextBoxOvenG;
		goto syn_error_quit;
	}

	try 
	{
		m_cMetalInfo.m_fEfficiency = RConverter::ToDouble(m_wndTextBoxOvenEfficiency.GetText().c_str()) / 100;
	}
	catch (RException ex)
	{
		ErrorMsg.assign(TEXT("КПД: ") + ex.GetMessage());
		pFocusWnd = &m_wndTextBoxOvenEfficiency;
		goto syn_error_quit;
	}

	if (m_cMetalInfo.m_fEfficiency <= 0 || m_cMetalInfo.m_fEfficiency > 1)
	{
		ErrorMsg.assign(TEXT("КПД: Коэффициент полезного действия должнен быть в пределах от 0 до 100"));
		pFocusWnd = &m_wndTextBoxOvenEfficiency;
		goto syn_error_quit;
	}

	return true;

syn_error_quit:
	MessageBox(m_hHandle,ErrorMsg.c_str(),GetText().c_str(), MB_OK | MB_ICONERROR);
	if (pFocusWnd) pFocusWnd->SetFocus();
	return false;
}

bool RModelParamFrame::CheckExpressionForTOnly(Expression &expr, RString &msg)
{
	Variables vars; // переменные в выражениях
	expr.GetAllVariableNames(vars);
	if (!vars.empty())
	{
		if (vars.size() > 1)
		{
			msg.assign(TEXT("cлишком много переменных.\nЕдинственная допустимая переменная: t"));
			return false;
		}
		else
		{
			if (vars.begin()->GetVarName() != PARSER_TEXT("t"))
			{
				msg.assign(vars.begin()->GetVarName() + TEXT(" - неопределенная переменная.\nЕдинственная допустимая переменная: t"));
				return false;
			}
		}
	}
	return true;
}

LRESULT RModelParamFrame::OnFocus(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = DefWindowProc(m_hHandle,WM_SETFOCUS,wParam,lParam);
	m_wndTextBoxMetalLambdaL.SetFocus();
	return lResult;
}