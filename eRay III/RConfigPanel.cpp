#include "RConfigPanel.h"

#include "RModelParamFrame.h"

RConfigPanel::RConfigPanel() 
	: RFrame(), id_BoundCheck(101), id_AddParams(102)
{
	TextBoxHeight = 20;
	AddMessage(WM_CREATE,MSG_EVENT_CAST(RConfigPanel::OnCreate),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RConfigPanel::OnSize),this);
	AddMessage(WM_CTLCOLORBTN,MSG_EVENT_CAST(RConfigPanel::OnCtlClrBtn),this);
	AddMessage(WM_COMMAND,MSG_EVENT_CAST(RConfigPanel::OnCommand),this);
	AddMessage(WM_SETFOCUS,MSG_EVENT_CAST(RConfigPanel::OnFocus),this);
}

RConfigPanel::~RConfigPanel()
{
	// добавь код
}

void RConfigPanel::SetExamples(int Index)
{
	switch(Index)
	{
		case 0: // кольцо
		{
			ExprY.SetText(TEXT("0.2*sin(t*c_pi/180)"));
			ExprX.SetText(TEXT("0.2*cos(t*c_pi/180)"));
			TBegin.SetText(TEXT("-180"));
			TStep.SetText(TEXT("3000"));
			TEnd.SetText(TEXT("180"));
			RayRadius.SetText(TEXT("0.1"));
			RayVoltage.SetText(TEXT("50.000"));
			ModelWidth.SetText(TEXT("1600"));
			ModelHeight.SetText(TEXT("500"));
			BoundCheck.SetCheck(true);
			MinX.SetEnable(TRUE);  MinX.SetText(TEXT("-0.9"));
			MaxX.SetEnable(TRUE);  MaxX.SetText(TEXT("0.9"));
			MinY.SetEnable(TRUE);  MinY.SetText(TEXT("-0.25"));
			MaxY.SetEnable(TRUE);  MaxY.SetText(TEXT("0.25"));
			VarList.clear();
			UpdateVarList();
			break;
		}
		case 1: // синус
		{
			ExprY.SetText(TEXT("0.23*sin(100*t)"));
			ExprX.SetText(TEXT("t"));
			TBegin.SetText(TEXT("-0.9"));
			TStep.SetText(TEXT("1000"));
			TEnd.SetText(TEXT("0.9"));
			RayRadius.SetText(TEXT("0.1"));
			RayVoltage.SetText(TEXT("50"));
			ModelWidth.SetText(TEXT("1600"));
			ModelHeight.SetText(TEXT("500"));
			BoundCheck.SetCheck(true);
			MinX.SetEnable(TRUE);  MinX.SetText(TEXT("-0.9"));
			MaxX.SetEnable(TRUE);  MaxX.SetText(TEXT("0.9"));
			MinY.SetEnable(TRUE);  MinY.SetText(TEXT("-0.25"));
			MaxY.SetEnable(TRUE);  MaxY.SetText(TEXT("0.25"));
			VarList.clear();
			UpdateVarList();
			break;
		}
		case 2: // спиралька
		{
			ExprY.SetText(TEXT("4*sin(t)/t"));
			ExprX.SetText(TEXT("4*cos(t)/t"));
			TBegin.SetText(TEXT("0"));
			TStep.SetText(TEXT("1000"));
			TEnd.SetText(TEXT("70"));
			RayRadius.SetText(TEXT("1"));
			RayVoltage.SetText(TEXT("50.000"));
			ModelWidth.SetText(TEXT("0"));
			ModelHeight.SetText(TEXT("0"));
			BoundCheck.SetCheck(true);
			MinX.SetEnable(TRUE);  MinX.SetText(TEXT("-2"));
			MaxX.SetEnable(TRUE);  MaxX.SetText(TEXT("2"));
			MinY.SetEnable(TRUE);  MinY.SetText(TEXT("-2"));
			MaxY.SetEnable(TRUE);  MaxY.SetText(TEXT("2"));
			VarList.clear();
			UpdateVarList();
			break;
		}
		case 3: // крендель
		{
			ExprY.SetText(TEXT("(2*cos(t*c_pi/180)-3*cos(2*t*c_pi/180))*0.15"));
			ExprX.SetText(TEXT("-(2*sin(t*c_pi/180)-3*sin(2*t*c_pi/180))*0.15"));
			TBegin.SetText(TEXT("0"));
			TStep.SetText(TEXT("1000"));
			TEnd.SetText(TEXT("360"));
			RayRadius.SetText(TEXT("0.4"));
			RayVoltage.SetText(TEXT("50.000"));
			ModelWidth.SetText(TEXT("0"));
			ModelHeight.SetText(TEXT("0"));
			BoundCheck.SetCheck(false);
			MinX.SetEnable(FALSE);  MinX.SetText(TEXT(""));
			MaxX.SetEnable(FALSE);  MaxX.SetText(TEXT(""));
			MinY.SetEnable(FALSE);  MinY.SetText(TEXT(""));
			MaxY.SetEnable(FALSE);  MaxY.SetText(TEXT(""));
			VarList.clear();
			UpdateVarList();
			break;
		}
		case 4: // крендель
		{
			ExprY.SetText(TEXT("(sin(w1*t) + sin(w2*t))/8"));
			ExprX.SetText(TEXT("(cos(w1*t) - cos(w2*t))/8"));
			TBegin.SetText(TEXT("0"));
			TStep.SetText(TEXT("1000"));
			TEnd.SetText(TEXT("360"));
			RayRadius.SetText(TEXT("0.1"));
			RayVoltage.SetText(TEXT("50.000"));
			ModelWidth.SetText(TEXT("1600"));
			ModelHeight.SetText(TEXT("500"));
			BoundCheck.SetCheck(true);
			MinX.SetEnable(TRUE);  MinX.SetText(TEXT("-0.9"));
			MaxX.SetEnable(TRUE);  MaxX.SetText(TEXT("0.9"));
			MinY.SetEnable(TRUE);  MinY.SetText(TEXT("-0.25"));
			MaxY.SetEnable(TRUE);  MaxY.SetText(TEXT("0.25"));
			VarList.clear();
			VarList.push_back(VarEditorUnit(TEXT("w"),TEXT("2")));
			VarList.push_back(VarEditorUnit(TEXT("w1"),TEXT("3*w")));
			VarList.push_back(VarEditorUnit(TEXT("w2"),TEXT("2*w")));
			UpdateVarList();

			break;
		}
		default:
		{
			ExprY.SetText(TEXT(""));
			ExprX.SetText(TEXT(""));
			TBegin.SetText(TEXT(""));
			TStep.SetText(TEXT(""));
			TEnd.SetText(TEXT(""));
			RayRadius.SetText(TEXT(""));
			RayVoltage.SetText(TEXT(""));
			ModelWidth.SetText(TEXT(""));
			ModelHeight.SetText(TEXT(""));
			BoundCheck.SetCheck(false);
			MinX.SetText(TEXT(""));
			MaxX.SetText(TEXT(""));
			MinY.SetText(TEXT(""));
			MaxY.SetText(TEXT(""));
			VarList.clear();
			UpdateVarList();
		}
	}
}

void RConfigPanel::AdjustWindowsLayout(int ClientWidth, int ClientHeight)
{
	int Indent = 5;
	int LabelWidth = 40;
	int TextBoxWidth = ClientWidth - LabelWidth - 3*Indent;
	int TextBoxLeft = LabelWidth + (Indent << 1);

	// конфигурируем поля выражений
	LabelExprY.Move(Indent, Indent, LabelWidth, TextBoxHeight, TRUE);
	LabelExprX.Move(Indent,(Indent << 1) + TextBoxHeight, LabelWidth, TextBoxHeight, TRUE);
	ExprY.Move(TextBoxLeft,Indent, TextBoxWidth, TextBoxHeight, TRUE);
	ExprX.Move(TextBoxLeft,(Indent << 1) + TextBoxHeight, TextBoxWidth, TextBoxHeight, TRUE);

	int MoreVariablesLeft = Indent;
	int MoreVariablesTop = (Indent << 1) + Indent + (TextBoxHeight << 1);
	int MoreVariablesWidth = ClientWidth - (Indent << 1);
	int MoreVariablesHeight = 100;
	MoreVariables.Move(MoreVariablesLeft,MoreVariablesTop,MoreVariablesWidth,MoreVariablesHeight,TRUE);

	// конфигурируем поля параметра
	int TParamBoxTop = MoreVariablesTop + MoreVariablesHeight + Indent;
	int TParamBoxLeft = Indent;
	int TParamBoxWidth = ClientWidth - (Indent << 1);
	int TParamBoxHeight = TextBoxHeight + 16 + Indent;

	int TParamsTop = 16;
	int TParamsLeft = Indent + TParamBoxLeft;
	int TParamsWidth = (TParamBoxWidth - (Indent << 2)) / 3;

	TParamsBox.Move(TParamBoxLeft,TParamBoxTop, TParamBoxWidth,TParamBoxHeight, TRUE);

	TBegin.Move(TParamsLeft, TParamBoxTop + TParamsTop, TParamsWidth,TextBoxHeight, TRUE);
	TStep.Move(TParamsLeft + Indent + TParamsWidth, TParamBoxTop + TParamsTop, TParamsWidth,TextBoxHeight, TRUE);
	TEnd.Move(TParamsLeft + (Indent << 1) + (TParamsWidth << 1), TParamBoxTop + TParamsTop, TParamsWidth,TextBoxHeight, TRUE);
	// конфигурируем поля моделирования
	int ModelBoxTop = TParamBoxTop + Indent + TParamBoxHeight;
	int ModelBoxLeft = Indent;
	int ModelBoxWidth = ClientWidth - (Indent << 1);
	int ModelBoxHeight = 3*TextBoxHeight + 16 + 3*Indent;
	int ModelBoxControlLeft = Indent + ModelBoxLeft;
	int ModelBoxControlTop = 16;
	int ModelBoxLabelWidth = 80;
	int ModelBoxTextBoxWidth = ModelBoxWidth - ModelBoxLabelWidth - 3*Indent;

	ModelBox.Move(ModelBoxLeft, ModelBoxTop, ModelBoxWidth, ModelBoxHeight, TRUE);
	
	LabelRayRadius.Move(ModelBoxControlLeft, ModelBoxTop + ModelBoxControlTop,ModelBoxLabelWidth,TextBoxHeight,TRUE);
	RayRadius.Move(ModelBoxControlLeft + ModelBoxLabelWidth + Indent, ModelBoxTop + ModelBoxControlTop, ModelBoxTextBoxWidth, TextBoxHeight, TRUE);
	LabelRayVoltage.Move(ModelBoxControlLeft, ModelBoxTop + ModelBoxControlTop + TextBoxHeight + Indent,ModelBoxLabelWidth,TextBoxHeight,TRUE);
	RayVoltage.Move(ModelBoxControlLeft + ModelBoxLabelWidth + Indent, ModelBoxTop + ModelBoxControlTop + TextBoxHeight + Indent, ModelBoxTextBoxWidth, TextBoxHeight, TRUE);
	RayAddParams.Move((Indent << 1), ModelBoxTop + ModelBoxControlTop + 2*(TextBoxHeight + Indent), ModelBoxWidth - (Indent << 1), TextBoxHeight, TRUE);
	// конфигурируем поля Качества картинки

	int ResolutionBoxTop = ModelBoxTop + Indent + ModelBoxHeight; 
	int ResolutionBoxLeft = Indent;
	int ResolutionBoxWidth = ClientWidth - (Indent << 1);
	int ResolutionBoxHeight = (TextBoxHeight << 1) + 16 + (Indent << 1);

	int ResolTop = 16;
	int ResolLeft = Indent;
	int ResolWidth = (ResolutionBoxWidth - 3*Indent) / 2;

	ResolutionBox.SetWindowPosition(HWND_BOTTOM, ResolutionBoxLeft, ResolutionBoxTop, ResolutionBoxWidth, ResolutionBoxHeight, SWP_NOACTIVATE);
	ModelWidth.SetWindowPosition(NULL, ResolutionBoxLeft + ResolLeft,ResolutionBoxTop + ResolTop, ResolWidth,TextBoxHeight,SWP_NOACTIVATE);
	ModelHeight.SetWindowPosition(NULL, ResolutionBoxLeft + ( Indent << 1) + ResolWidth, ResolutionBoxTop + ResolTop,ResolWidth,TextBoxHeight,SWP_NOACTIVATE);
	ResolutionComment.SetWindowPosition(NULL, ResolutionBoxLeft + Indent, ResolutionBoxTop + ResolTop + TextBoxHeight + Indent,ResolutionBoxWidth - (Indent << 1),TextBoxHeight,SWP_NOACTIVATE);

	int BoundBoxTop = ResolutionBoxTop + Indent + ResolutionBoxHeight;
	int BoundBoxLeft = Indent;
	int BoundBoxWidth = ClientWidth - (Indent << 1);
	int BoundBoxHeight = 20 + ((TextBoxHeight + Indent) * 3);

	int BoundBoxContentLeft = BoundBoxLeft + Indent;
	int BoundBoxContentTop = BoundBoxTop + 20;
	int BoundEditWidth = (BoundBoxWidth - (Indent << 2))/3;

	BoundBox.SetWindowPosition(HWND_BOTTOM, BoundBoxLeft, BoundBoxTop, BoundBoxWidth, BoundBoxHeight, SWP_NOACTIVATE);
	
	BoundCheck.SetWindowPosition(BoundBox.Handle(), BoundBoxContentLeft + Indent,BoundBoxTop - 2,125,TextBoxHeight,SWP_NOACTIVATE);
	MaxY.SetWindowPosition(NULL, BoundBoxContentLeft + Indent + BoundEditWidth,BoundBoxContentTop,BoundEditWidth,TextBoxHeight,SWP_NOACTIVATE | SWP_SHOWWINDOW);
	MinX.SetWindowPosition(NULL, BoundBoxContentLeft, BoundBoxContentTop + TextBoxHeight + Indent,BoundEditWidth,TextBoxHeight,SWP_NOACTIVATE);
	MaxX.SetWindowPosition(NULL, BoundBoxContentLeft + (BoundEditWidth << 1) + Indent + Indent,BoundBoxContentTop + TextBoxHeight + Indent,BoundEditWidth,TextBoxHeight,SWP_NOACTIVATE);
	MinY.SetWindowPosition(NULL, BoundBoxContentLeft + Indent + BoundEditWidth,BoundBoxContentTop + (TextBoxHeight + Indent)*2,BoundEditWidth,TextBoxHeight,SWP_NOACTIVATE);
}

void RConfigPanel::SetDefaultVisualStyle(void)
{
	// установка шрифтов
	ExprY.SetFont(hEditFont);
	ExprX.SetFont(hEditFont);
	LabelExprY.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	LabelExprX.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	MoreVariables.SetStyle(MoreVariables.GetStyle() | LVS_REPORT | WS_BORDER);
	MoreVariables.CreateColumn(TEXT("Имя"),50);
	MoreVariables.CreateColumn(TEXT("значение"),120);
	MoreVariables.SetExplorerStyle();

	TParamsBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	TBegin.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	TStep.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	TEnd.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	ModelBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	LabelRayRadius.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	RayRadius.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	LabelRayVoltage.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	RayVoltage.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	RayAddParams.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));

	ResolutionBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	ModelWidth.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	ModelHeight.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	ResolutionComment.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	
	BoundBox.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	BoundCheck.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	MinX.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	MaxX.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	MinY.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	MaxY.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));	

	// установка подсказок

	ExprY.AddToolTip(TEXT("Зависимость ординаты от параметра t"));
	ExprX.AddToolTip(TEXT("Зависимость абциссы от параметра t"));

	TBegin.AddToolTip(TEXT("Начало интервала"));
	TStep.AddToolTip(TEXT("Количество шагов"));
	TEnd.AddToolTip(TEXT("Конец интервала"));

	RayRadius.AddToolTip(TEXT("Радиус электронного пучка"));

	ModelWidth.AddToolTip(TEXT("Ширина картинки в пикселях"));
	ModelHeight.AddToolTip(TEXT("Высота картинки в пикселях"));
	
	BoundCheck.AddToolTip(TEXT("Установите чтобы задать регион отриcовки"));
	MinX.AddToolTip(TEXT("Начало интервала по X"));
	MaxX.AddToolTip(TEXT("Конец интервала по X"));
	MinY.AddToolTip(TEXT("Начало интервала по Y"));
	MaxY.AddToolTip(TEXT("Конец интервала по Y"));

	// установка дополнительных стилей

	TParamsBox.SetStyle(TParamsBox.GetStyle() | WS_CLIPSIBLINGS);
}

LRESULT RConfigPanel::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (!CreateEditFont())
	{
		return -1;
	}
	RString Label1(TEXT("Y ( t ) :"));
	RString Label2(TEXT("X ( t ) :"));
	RString Label3(TEXT("Параметр t"));
	RString Label4(TEXT("Параметры модели"));
	RString Label5(TEXT("Разрешение картинки"));
	RString Label6(TEXT("Радиус пятна :"));
	RString Label7(TEXT("*0 - автоматически"));
	RString Label8(TEXT("Область построения"));
	RString Label9(TEXT("Напряжение :"));
	RString LabelA(TEXT("Доп. параметры"));

	// создаём поля для ввода выражений
	if (!LabelExprY.Create(Label1.c_str(),this,1))
		return -1;
	if (!LabelExprX.Create(Label2.c_str(),this,1))
		return -1;
	
	if (!ExprY.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_AUTOHSCROLL,this,1))
		return -1;
	if (!ExprX.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_AUTOHSCROLL,this,1))
		return -1;

	ExprY.SetBackgroundText(TEXT("Зависимость y(t)"));
	ExprX.SetBackgroundText(TEXT("Зависимость x(t)"));

	// создаём таблицу дополнительных переменных
	if (!MoreVariables.Create(this,1))
		return -1;

	MoreVariables.SetExListViewStyle(LVS_EX_FULLROWSELECT);

	// создаём поля для ввода Т параметра
	if (!TParamsBox.Create(Label3.c_str(),this,1))
		return -1;
	if (!TBegin.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!TStep.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!TEnd.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	
	TBegin.SetBackgroundText(TEXT("Начало"));
	TStep.SetBackgroundText(TEXT("Кол-во"));
	TEnd.SetBackgroundText(TEXT("Конец"));

	TBegin.SetFilter(RTextBoxEx::RealFilter);
	TStep.SetFilter(RTextBoxEx::IntegerFilter);
	TEnd.SetFilter(RTextBoxEx::RealFilter);

	TBegin.BringWindowToTop();
	TStep.BringWindowToTop();
	TEnd.BringWindowToTop();

	if (!ModelBox.Create(Label4.c_str(),this,1))
		return -1;
	if (!RayRadius.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!LabelRayRadius.Create(Label6.c_str(),this,1))
		return -1;
	if (!RayVoltage.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!LabelRayVoltage.Create(Label9.c_str(),this,1))
		return -1;
	if (!RayAddParams.Create(LabelA.c_str(),this,id_AddParams))
		return -1;

	RayRadius.SetBackgroundText(TEXT("R, м"));
	RayVoltage.SetBackgroundText(TEXT("U, кВ"));

	RayRadius.SetFilter(RTextBoxEx::RealFilter);
	RayRadius.SetSignInput(false);
	RayVoltage.SetFilter(RTextBoxEx::RealFilter);
	RayVoltage.SetSignInput(false);

	RayRadius.BringWindowToTop();
	LabelRayRadius.BringWindowToTop();
	RayVoltage.BringWindowToTop();
	LabelRayVoltage.BringWindowToTop();
	RayAddParams.BringWindowToTop();

	if (!ResolutionBox.Create(Label5.c_str(),this,1))
		return -1;
	if (!ModelWidth.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!ModelHeight.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!ResolutionComment.Create(Label7.c_str(),this,1))
		return -1;

	ModelWidth.SetBackgroundText(TEXT("Ширина"));
	ModelHeight.SetBackgroundText(TEXT("Высота"));

	ModelWidth.SetFilter(RTextBoxEx::IntegerFilter);
	ModelHeight.SetFilter(RTextBoxEx::IntegerFilter);
	ModelWidth.SetSignInput(false);
	ModelHeight.SetSignInput(false);

	ModelWidth.BringWindowToTop();
	ModelHeight.BringWindowToTop();
	ResolutionComment.BringWindowToTop();

	if (!BoundBox.Create(TEXT(""),this,1))
		return -1;
	if (!BoundCheck.Create(Label8.c_str(),this,id_BoundCheck))
		return -1;
	if (!MinX.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!MaxX.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!MinY.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;
	if (!MaxY.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | ES_CENTER,this,1))
		return -1;

	MinX.SetBackgroundText(TEXT("min x"));
	MaxX.SetBackgroundText(TEXT("max x"));
	MinY.SetBackgroundText(TEXT("min y"));
	MaxY.SetBackgroundText(TEXT("max y"));

	MinX.SetFilter(RTextBoxEx::RealFilter);
	MaxX.SetFilter(RTextBoxEx::RealFilter);
	MinY.SetFilter(RTextBoxEx::RealFilter);
	MaxY.SetFilter(RTextBoxEx::RealFilter);

	BoundCheck.BringWindowToTop();
	MinX.BringWindowToTop();
	MaxX.BringWindowToTop();
	MinY.BringWindowToTop();
	MaxY.BringWindowToTop();
	
	BoundCheck.SetCheck(BST_UNCHECKED);
	MinX.SetEnable(FALSE);
	MinY.SetEnable(FALSE);
	MaxX.SetEnable(FALSE);
	MaxY.SetEnable(FALSE);

	SetDefaultVisualStyle();
	
	TabList.Append(&ExprY);
	TabList.Append(&ExprX);
	TabList.Append(&MoreVariables);
	TabList.Append(&TBegin);
	TabList.Append(&TStep);
	TabList.Append(&TEnd);
	TabList.Append(&RayRadius);
	TabList.Append(&RayVoltage);
	TabList.Append(&RayAddParams);
	TabList.Append(&ModelWidth);
	TabList.Append(&ModelHeight);
	TabList.Append(&BoundCheck);
	TabList.Append(&MinX);
	TabList.Append(&MaxX);
	TabList.Append(&MinY);
	TabList.Append(&MaxY);

	/*
	LoadDefaultParam();
	*/
	return 0;
}

LRESULT RConfigPanel::OnCtlClrBtn(WPARAM wParam, LPARAM lParam)
{
	if (lParam == (LPARAM)BoundBox.Handle())
	{
		HDC hdcStatic = (HDC) wParam;
		SetBkColor(hdcStatic,GetSysColor(COLOR_BTNFACE));
		return (LRESULT)CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	}
	return DefWindowProc(this->Handle(),WM_CTLCOLORSTATIC,wParam,lParam);
}

LRESULT RConfigPanel::OnSize(WPARAM wParam, LPARAM lParam)
{
	AdjustWindowsLayout(LOWORD(lParam),HIWORD(lParam));
	return 0;
}

LRESULT RConfigPanel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int ControlID = LOWORD(wParam);

	if (ControlID == id_BoundCheck)
	{
		return OnRegionClick((HWND)lParam,LOWORD(wParam),HIWORD(wParam));
	}
	else if (ControlID == id_AddParams)
	{
		SendMessage(GetParent()->Handle(),WM_VIEW_ADD_PARAMS,0,0);
	}
	return 0;
}

LRESULT RConfigPanel::OnRegionClick(HWND hControl, WORD cID, WORD Notify)
{
	if (BoundCheck.GetCheck())
	{
		MinX.SetEnable(TRUE);
		MaxX.SetEnable(TRUE);
		MinY.SetEnable(TRUE);
		MaxY.SetEnable(TRUE);
	}
	else
	{
		MinX.SetEnable(FALSE);
		MaxX.SetEnable(FALSE);
		MinY.SetEnable(FALSE);
		MaxY.SetEnable(FALSE);
	}
	return 0;
}

void RConfigPanel::LoadDefaultParam()
{
	ExprY.SetText(TEXT("t^2"));
	ExprX.SetText(TEXT("t"));

	TBegin.SetText(TEXT("-4"));
	TStep.SetText(TEXT("8"));
	TEnd.SetText(TEXT("4"));

	RayRadius.SetText(TEXT("0.5"));

	ModelWidth.SetText(TEXT("0"));
	ModelHeight.SetText(TEXT("0"));

	MinX.SetText(TEXT("-2"));
	MinY.SetText(TEXT("-2"));
	MaxX.SetText(TEXT("2"));
	MaxY.SetText(TEXT("2"));
}

bool RConfigPanel::CreateEditFont()
{
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfEscapement = 0;
	//lf.lfFaceName = NULL;
	lf.lfHeight = 16;
	lf.lfItalic = FALSE;
	lf.lfOrientation = 0;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfStrikeOut = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfWeight = FW_NORMAL;
	lf.lfWidth = 0;
	hEditFont = CreateFontIndirect(&lf);
	if (!hEditFont) return FALSE;
	return TRUE;
}

void RConfigPanel::UpdateVarList()
{
	MoreVariables.ClearList();
	// загрузка уже существующих переменных
	for (std::size_t i = 0; i < VarList.size(); i++)
	{
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.pszText = (LPTSTR)(VarList[i].Name.c_str());
		lvi.iItem = i;
		int ItemIndex;
						
		ItemIndex = MoreVariables.AddItem(&lvi);
		MoreVariables.SetItemText(ItemIndex,1,(LPTSTR)VarList[i].Value.c_str());
	}
}

void RConfigPanel::LockInterface()
{
	EnableWindow(m_hHandle,FALSE);
}

void RConfigPanel::UnLockInterface()
{
	EnableWindow(m_hHandle,TRUE);
}

LRESULT RConfigPanel::OnFocus(WPARAM wParam, LPARAM lParam)
{
	ExprY.SetFocus();
	return 0;
}