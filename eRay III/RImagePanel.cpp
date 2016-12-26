
#include "RImagePanel.h"

RImagePanel::RImagePanel()
{
	hCloudPenSelector = CreatePen(PS_SOLID,1,RGB(0xff,0xff,0x00));
	hCloudPenBalloon = CreatePen(PS_SOLID,1,RGB(0xc0,0xc0,0xc0));
	hCloudBrushBalloon = CreateSolidBrush(RGB(0xc0,0xc0,0xc0));

	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfHeight = 14;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = DEFAULT_QUALITY;
	hCloudFontBalloon = CreateFontIndirect(&lf);

	PointX = -1;
	PointY = -1;
	AddMessage(WM_CREATE,MSG_EVENT_CAST(RImagePanel::OnCreate),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RImagePanel::OnSize),this);
	AddMessage(WM_ERASEBKGND,MSG_EVENT_CAST(RImagePanel::OnErsBckg),this);
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RImagePanel::OnPaint),this);
	AddMessage(WM_HSCROLL,MSG_EVENT_CAST(RImagePanel::OnHScroll),this);
	AddMessage(WM_VSCROLL,MSG_EVENT_CAST(RImagePanel::OnVScroll),this);
	AddMessage(WM_RBUTTONDOWN,MSG_EVENT_CAST(RImagePanel::OnRButtonDown),this);
	AddMessage(WM_LBUTTONDOWN,MSG_EVENT_CAST(RImagePanel::OnClick),this);
	AddMessage(WM_LBUTTONDBLCLK,MSG_EVENT_CAST(RImagePanel::OnDblClick),this);
	AddMessage(WM_KEYDOWN,MSG_EVENT_CAST(RImagePanel::OnKeyDown),this);
}

RImagePanel::~RImagePanel()
{

}

LRESULT RImagePanel::OnCreate(WPARAM wParam, LPARAM lParam)
{
	// Initialize the horizontal scrolling variables. 
    xMinScroll = 0; 
    xCurrentScroll = 0; 
    xMaxScroll = 0; 
 
    // Initialize the vertical scrolling variables. 
    yMinScroll = 0; 
    yCurrentScroll = 0; 
    yMaxScroll = 0; 

	fBlt = FALSE;
    fScroll = FALSE;
    fSize = FALSE;
	
	return 0;
}

LRESULT RImagePanel::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void RImagePanel::UpDateScroll(int NewClientWidth, int NewClientHeight)
{	
//	RECT WindowRect;
//	GetWindowRect(WindowRect);
//	int WindowWidth(WindowRect.right - WindowRect.left);
//	int WindowHeight(WindowRect.bottom - WindowRect.top);
	
	int WindowWidth(NewClientWidth);
	int WindowHeight(NewClientHeight);

	int ImageWidth = GraphView.GetImageWidth();
	int ImageHeight = GraphView.GetImageHeight();
	
	xMaxScroll = max(ImageWidth - WindowWidth, 0);
    xCurrentScroll = min(xCurrentScroll, xMaxScroll);
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin   = xMinScroll;
    si.nMax   = ImageWidth != NewClientWidth ? ImageWidth : 0;
	si.nPage  = ImageWidth != NewClientWidth ? WindowWidth : 0;
    si.nPos   = xCurrentScroll;
	SetScrollInfo(m_hHandle, SB_HORZ, &si, TRUE);
 
	yMaxScroll = max(ImageHeight - WindowHeight, 0);
    yCurrentScroll = min(yCurrentScroll, yMaxScroll);
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin   = yMinScroll;
    si.nMax   = ImageHeight != NewClientHeight ? ImageHeight : 0;
	si.nPage  = ImageHeight != NewClientHeight ? WindowHeight : 0;
    si.nPos   = yCurrentScroll;
    SetScrollInfo(m_hHandle, SB_VERT, &si, TRUE);
}

void RImagePanel::ClearScroll()
{
	// Initialize the horizontal scrolling variables. 
    xMinScroll = 0; 
    xCurrentScroll = 0; 
    xMaxScroll = 0; 
 
    // Initialize the vertical scrolling variables. 
    yMinScroll = 0; 
    yCurrentScroll = 0; 
    yMaxScroll = 0; 
}

LRESULT RImagePanel::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (fBlt) 
		fSize = TRUE; 
	UpDateScroll(LOWORD(lParam),HIWORD(lParam));
	return 0;
}

LRESULT RImagePanel::OnErsBckg(WPARAM wParam, LPARAM lParam)
{
	RECT ClientRect;
	GetClientRect(ClientRect);
	bool bErase = ClientRect.right <= GraphView.GetImageWidth() && ClientRect.bottom <= GraphView.GetImageHeight(); 
	return bErase ? TRUE : DefWindowProc(m_hHandle,WM_ERASEBKGND, wParam, lParam);
}

LRESULT RImagePanel::OnPaint(WPARAM wParam, LPARAM lParam)
{
	int ImagePanelWidth;
	int ImagePanelHeight;
	int ScrollImageByX;
	int ScrollImageByY;
	int ViewWidth = GraphView.GetImageWidth();
	int ViewHeight = GraphView.GetImageHeight();

	// начало отрисовки
	HDC hdc = BeginPaint(m_hHandle,&m_ps);
	
	// получаем ширину и высоту панели отрисовки
	ImagePanelWidth = m_ps.rcPaint.right - m_ps.rcPaint.left;
	ImagePanelHeight = m_ps.rcPaint.bottom - m_ps.rcPaint.top;

	if (ViewWidth < ImagePanelWidth)
		ScrollImageByX = (ImagePanelWidth >> 1) - (ViewWidth >> 1);
	else
		ScrollImageByX = -xCurrentScroll;

	if (ViewHeight < ImagePanelHeight)
		ScrollImageByY = (ImagePanelHeight >> 1) - (ViewHeight >> 1);
	else
		ScrollImageByY = -yCurrentScroll;

	GraphView.DrawScene(hdc,ScrollImageByX,ScrollImageByY);
	
	DrawCommonInfo(hdc,ScrollImageByX + GraphView.GetGraphicIndent() + 5,ScrollImageByY + GraphView.GetGraphicIndent() + 5);

	DrawSight(hdc,PointX + ScrollImageByX, PointY + ScrollImageByY);

	EndPaint(m_hHandle,&m_ps);
	return 0;
}

LRESULT RImagePanel::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	int xDelta;     // xDelta = new_pos - current_pos  
    int xNewPos;    // new position 
    int yDelta = 0; 
 
    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft left of the scroll box. 
        case SB_PAGEUP: 
            xNewPos = xCurrentScroll - 50; 
            break;
        // User clicked the scroll bar shaft right of the scroll box. 
        case SB_PAGEDOWN: 
            xNewPos = xCurrentScroll + 50; 
            break; 
        // User clicked the left arrow. 
        case SB_LINEUP: 
            xNewPos = xCurrentScroll - 5; 
            break; 
        // User clicked the right arrow. 
        case SB_LINEDOWN: 
            xNewPos = xCurrentScroll + 5; 
            break; 
        // User dragged the scroll box. 
        case SB_THUMBPOSITION: 
            xNewPos = HIWORD(wParam); 
            break; 
        default: 
            xNewPos = xCurrentScroll; 
    } 
 
    // New position must be between 0 and the screen width. 
    xNewPos = max(0, xNewPos); 
    xNewPos = min(xMaxScroll, xNewPos); 
 
    // If the current position does not change, do not scroll.
    if (xNewPos == xCurrentScroll) 
        return 0; 
 
	// Set the scroll flag to TRUE. 
    fScroll = TRUE; 

    // Determine the amount scrolled (in pixels). 
    xDelta = xNewPos - xCurrentScroll; 
 
    // Reset the current scroll position. 
    xCurrentScroll = xNewPos; 
 
	InvalidateRect(m_hHandle,NULL,TRUE);
    // Reset the scroll bar. 
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = xCurrentScroll; 
    SetScrollInfo(m_hHandle, SB_HORZ, &si, TRUE); 
	return 0;
}
LRESULT RImagePanel::OnVScroll(WPARAM wParam, LPARAM lParam)
{
	int xDelta = 0; 
    int yDelta;     // yDelta = new_pos - current_pos 
    int yNewPos;    // new position 
 
    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft above the scroll box. 
        case SB_PAGEUP: 
            yNewPos = yCurrentScroll - 50; 
            break; 
 
        // User clicked the scroll bar shaft below the scroll box. 
        case SB_PAGEDOWN: 
            yNewPos = yCurrentScroll + 50; 
            break; 
 
        // User clicked the top arrow. 
        case SB_LINEUP: 
            yNewPos = yCurrentScroll - 5; 
            break; 
 
        // User clicked the bottom arrow. 
        case SB_LINEDOWN: 
            yNewPos = yCurrentScroll + 5; 
            break; 
 
        // User dragged the scroll box. 
        case SB_THUMBPOSITION: 
            yNewPos = HIWORD(wParam); 
            break; 
        default: 
            yNewPos = yCurrentScroll; 
    } 
 
    // New position must be between 0 and the screen height. 
    yNewPos = max(0, yNewPos); 
    yNewPos = min(yMaxScroll, yNewPos); 
 
    // If the current position does not change, do not scroll.
    if (yNewPos == yCurrentScroll) 
        return 0; 
 
	// Set the scroll flag to TRUE. 
    fScroll = TRUE;

    // Determine the amount scrolled (in pixels). 
    yDelta = yNewPos - yCurrentScroll; 
 
    // Reset the current scroll position. 
    yCurrentScroll = yNewPos; 
 
	InvalidateRect(m_hHandle,NULL,TRUE);
    // Reset the scroll bar. 
    si.cbSize = sizeof(si); 
    si.fMask  = SIF_POS; 
    si.nPos   = yCurrentScroll; 
    SetScrollInfo(m_hHandle, SB_VERT, &si, TRUE); 
	return 0;
}

LRESULT RImagePanel::OnClick(WPARAM wParam, LPARAM lParam) 
{
	SetFocus();
	if (!GraphView.CanDraw()) return 0;
	PointX = LOWORD(lParam);
	PointY = HIWORD(lParam);
	RECT Rect;
	GetClientRect(Rect);
	int ImageWidth = GraphView.GetImageWidth();
	int ImageHeight = GraphView.GetImageHeight();
	int ViewBeginX = ImageWidth < Rect.right ? (Rect.right - ImageWidth)/2 : -xCurrentScroll;
	int ViewBeginY = ImageHeight < Rect.bottom ? (Rect.bottom - ImageHeight)/2 : -yCurrentScroll;

	if (PointX > ViewBeginX &&
		PointY > ViewBeginY &&
		PointX < ViewBeginX + ImageWidth &&
		PointY < ViewBeginY + ImageHeight
		)
	{
		if (GraphView.GraphType != GraphicManager::HotGrad)
		{
			return 0;
		}
		PointX = PointX - (ViewBeginX < 0 ? 0 : ViewBeginX) + xCurrentScroll;
		PointY = PointY - (ViewBeginY < 0 ? 0 : ViewBeginY) + yCurrentScroll;

		int QuadIndexByX = (int)(PointX / GraphView.CloudGr.PixelInOneStep);
		int QuadIndexByY = (int)(PointY / GraphView.CloudGr.PixelInOneStep);

		CreateClickMsg(&GraphView.CloudGr.ThreadAccum[QuadIndexByX][QuadIndexByY]);

		SendMessage(GetParent()->Handle(),WM_CHANGE_RANGING,0,0);
	}
	else
	{
		PointX = -1;
		PointY = -1;
	}
	Refresh();
	return 0;
}

LRESULT RImagePanel::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	if (!(PointX != -1 && PointY != -1 && GraphView.GraphType == GraphicManager::HotGrad))
	{
		return 0;
	}

	int deltaX(0), deltaY(0);
	switch(wParam)
	{
		case VK_UP:
			deltaY--;
			break;
		case VK_DOWN:
			deltaY++;
			break;
		case VK_LEFT:
			deltaX--;
			break;
		case VK_RIGHT:
			deltaX++;
			break;
	}

	// получаем клиентский пр€моугольник 
	RECT Rect;
	GetClientRect(Rect);
	// получаем размеры картинки
	int ImageWidth = GraphView.GetImageWidth();
	int ImageHeight = GraphView.GetImageHeight();

	// получаем точку верхнего левого угла картинки на форме
	int ViewBeginX = ImageWidth < Rect.right ? (Rect.right - ImageWidth)/2 : -xCurrentScroll;
	int ViewBeginY = ImageHeight < Rect.bottom ? (Rect.bottom - ImageHeight)/2 : -yCurrentScroll;

	if (PointX + deltaX > 0 &&
		PointY + deltaY > 0 &&
		PointX + deltaX < ImageWidth &&
		PointY + deltaY < ImageHeight
		)
	{
		PointX += deltaX;
		PointY += deltaY;
		
		int QuadIndexByX = (int)(PointX / GraphView.CloudGr.PixelInOneStep);
		int QuadIndexByY = (int)(PointY / GraphView.CloudGr.PixelInOneStep);

		GridNodeInfo tmp = GraphView.CloudGr.ThreadAccum[QuadIndexByX][QuadIndexByY];
		CreateClickMsg(&tmp);

		SendMessage(GetParent()->Handle(),WM_CHANGE_RANGING,0,0);
		Refresh();
	}
	return 0;
}

void RImagePanel::DrawSight(HDC hdc, int X, int Y)
{
	if (PointX != -1 && PointY != -1 && GraphView.GraphType == GraphicManager::HotGrad)
	{
		SelectObject(hdc,hCloudPenSelector);

		// получаем клиентские координаты
		RECT Rect;
		GetClientRect(Rect);
		// рисуем прицел
		MoveToEx(hdc,X,0,NULL);
		LineTo(hdc,X,Rect.bottom);
		MoveToEx(hdc,0,Y,NULL);
		LineTo(hdc,Rect.right,Y);
		
		// добавление надписи по счелчку
		
		int Indent = 3; // отступ текста в облачке
		int BallonIndent = 2; // отступ прицела от облачка
		// определ€ем размер надписи
		int BalloonPosX(0), BalloonPosY(0);
		SelectObject(hdc,hCloudFontBalloon);
		// надпись потока
		SIZE QStreamTextSize;
		if (!GetTextExtentPoint32(hdc,QStream.c_str(),QStream.size(),&QStreamTextSize))
		{
			// не рисуем в таком случае
			return;
		}
		// надпись доли
		SIZE QPercentTextSize;
		if (!GetTextExtentPoint32(hdc,QPercent.c_str(),QPercent.size(),&QPercentTextSize))
		{
			// не рисуем в таком случае
			return;
		}
		// ‘ормиуем общую высоту и шинину
		SIZE TextSize = {
		QStreamTextSize.cx > QPercentTextSize.cx ? QStreamTextSize.cx : QPercentTextSize.cx,
		QStreamTextSize.cy + QPercentTextSize.cy};	
		// получаем размер картинки
		RECT ImageRect;
		GraphView.GetDrawRegion(&ImageRect);
		long ImageWidth = ImageRect.right - ImageRect.left;
		long ImageHeight = ImageRect.bottom - ImageRect.top;
		// преобразуем клиентские координады в координаты рисунка
		if (ImageWidth < Rect.right)
		{
			Rect.left = (Rect.right - ImageWidth) >> 1;
			Rect.right = (Rect.right + ImageWidth) >> 1;
		}
		if (ImageHeight < Rect.bottom)
		{
			Rect.top = (Rect.bottom - ImageHeight) >> 1;
			Rect.bottom = (Rect.bottom + ImageHeight) >> 1;
		}

		// включаем отступ в расчЄт
		long BalloonSizeCx = TextSize.cx + (Indent << 1);
		long BalloonSizeCy = TextSize.cy + (Indent << 1);

		// определ€ем позицию надписи относительно вертикальной черты
		if (X + BalloonSizeCx + BallonIndent < Rect.right )
			BalloonPosX = X + BallonIndent;
		else
			if (X - BalloonSizeCx - BallonIndent > Rect.left )
				BalloonPosX = X - BalloonSizeCx - BallonIndent;
			else
				return;
		// определ€ем позицию надписи относительно горизонтакльной черты
		if (Y - BalloonSizeCy - BallonIndent > Rect.top )
			BalloonPosY = Y - BalloonSizeCy - BallonIndent;
		else
			if (Y + BalloonSizeCy + BallonIndent < Rect.bottom )
				BalloonPosY = Y + BallonIndent;
			else
				return;

		// формируем пр€моугольник отрисовки
		RECT BalloonRect = {BalloonPosX,BalloonPosY,BalloonPosX + BalloonSizeCx,BalloonPosY + BalloonSizeCy};
			
		// отрисовываем облачко
		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,hCloudPenBalloon);
		SelectObject(hdc,hCloudBrushBalloon);
		RoundRect(hdc,BalloonRect.left,BalloonRect.top,BalloonRect.right,BalloonRect.bottom,5,5);
			
		// отрисовка текста в облачке
		//SetTextColor(hdc,0xffffff);
		SetTextColor(hdc,0x000000);
		RString ClickMsg = QStream + TEXT("\n") + QPercent;
		BalloonRect.top += Indent;
		BalloonRect.left += Indent;
		DrawText(hdc,ClickMsg.c_str(),ClickMsg.size(),&BalloonRect,DT_LEFT | DT_VCENTER /* | DT_SINGLELINE*/);
	}
}

void RImagePanel::DrawCommonInfo(HDC hdc, int X, int Y)
{
	if (!GraphView.CanDraw()) return;
}

void RImagePanel::CreateClickMsg(GridNodeInfo *gni)
{
	double Per = gni->Percent;
	double Pow = gni->Power;

	QStream = TEXT("Ёнерги€: ") + RConverter::ToString(Pow / 1000.0) + TEXT(" k¬т");
	QPercent = TEXT("ƒол€: ") + RConverter::ToString(Per*100.0) + TEXT(" %");
}

void RImagePanel::LockInterface()
{
	SetClassLong(GCL_HCURSOR,(LONG_PTR)LoadCursor(NULL,IDC_WAIT));
}

void RImagePanel::UnLockInterface()
{
	SetClassLong(GCL_HCURSOR,(LONG_PTR)LoadCursor(NULL,IDC_ARROW));
}

void RImagePanel::ClearImage()
{
	GraphView.ClearAll();
}

LRESULT RImagePanel::OnDblClick(WPARAM wParam, LPARAM lParam)
{
	if (!GraphView.CanDraw()) return 0;
	/*
	// им€ класса окна
	LPCTSTR szTemperatureFrameClassName = TEXT("RTemperatureFrame");
	RTemperatureFrame dlg;
	
	// инициализируем тепловой поток
	int Indent = GraphView.GetGraphicIndent();
	tBaseCloudDump fld(GraphView.CloudGr.BaseCloudDump.GetWidth() - 2 * Indent, 
		GraphView.CloudGr.BaseCloudDump.GetHeight() - 2 * Indent);


	for (int i = 0, i_stop = fld.GetWidth(); i < i_stop; i++)
	{
		for (int j = 0 ,j_stop = fld.GetHeight(); j < j_stop; j++)
		{
			fld.Index(i, j) = GraphView.CloudGr.BaseCloudDump.Index(i + Indent,j + Indent);
		}
	}

	fld.SetPixelWeight(GraphView.CloudGr.BaseCloudDump.GetPixelWeight());
	double S = fld.GetWidth() * fld.GetHeight() * fld.GetPixelWeight() * fld.GetPixelWeight();

	dlg.InitQzt(fld, GraphView.CloudGr.TotalEfficiency * GraphView.GrInf.RayVoltage * GraphView.GrInf.Amp / S);
	
	// регистрируем окно
	if (!RFrame::RegisterWndClass(szTemperatureFrameClassName,CS_HREDRAW | CS_VREDRAW,CreateSolidBrush(GetSysColor(COLOR_BTNFACE)),LoadCursor(0,IDC_ARROW)))
			return 0;
	
	// создаЄм окно
	if (!dlg.Create(0,szTemperatureFrameClassName,TEXT("“емпературное поле"), WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_POPUP,this->GetParent(),0,0,800,600,NULL))
	{
		MessageBox(this->m_hHandle,TEXT("Ќе могу запустить!"),TEXT("Msg"),MB_OK);
		RFrame::UnRegisterWndClass(szTemperatureFrameClassName);
		return 0;
	}
	
	dlg.ShowModal();

	// отмен€ем регистрацию окна
	RFrame::UnRegisterWndClass(szTemperatureFrameClassName);
	*/
	RCustomWindow *Owner = GetParent();
	if (Owner)
		SendMessage(GetParent()->Handle(),WM_VIEW_TEMP_MODEL,0,0);
	return 0;
}