
#include "RRangingFrame.h"

RRangingFrame::RRangingFrame(void)
{
	m_hDc = 0;
	AddMessage(WM_CREATE,MSG_EVENT_CAST(RRangingFrame::OnCreate),this);
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RRangingFrame::OnPaint),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RRangingFrame::OnSize),this);
}

RRangingFrame::~RRangingFrame(void)
{

}

void RRangingFrame::SetLegendText(LPCTSTR Text)
{
	m_strLegend.assign(Text);
}

void RRangingFrame::SetHorText(LPCTSTR Text)
{
	m_strHorText.assign(Text);
}

void RRangingFrame::ClearParams(void)
{
	m_cGraph.Clear();
	CreateDrawScene();
}

LRESULT RRangingFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
	m_strLegend.assign(TEXT("Текст легенды"));
	return 0;
}

LRESULT RRangingFrame::OnSize(WPARAM wParam, LPARAM lParam)
{
	CreateDrawScene();
	Refresh();
	return 0;
}


LRESULT RRangingFrame::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(this->m_hHandle,&ps);
	// код отрисовки

	BitBlt(hdc,0,0,m_cScene.GetWidth(),m_cScene.GetHeight(),m_cScene.GetDC(),0,0,SRCCOPY);

	EndPaint(this->m_hHandle,&ps);
	return 0;
}

void RRangingFrame::CreateImage(RangingPoints *field, int BeginSel, int EndSel)
{
	CreateDrawGraph(field, BeginSel, EndSel);
	CreateDrawScene();
}

void RRangingFrame::CreateDrawGraph(RangingPoints *Points, int SelectorBegin, int SelectorEnd)
{
	RECT Rect;

	// создаём графический буффер
	{
		HDC hdc = GetDC(m_hHandle);
		if (!hdc)
			return;

		GetClientRect(Rect);

		if (!m_cGraph.CreateCompatible(hdc,Points->Length(),Rect.bottom))
			return;

		DeleteDC(hdc);
	}
	// закрашиваем чёрным
	m_cGraph.Blacken();
	// создаём перо для рисования графика
	IB_Pen GraphPen(PS_SOLID,1,RGB(0xff,0x00,0x00));
	// выбераем перо в буфер
	m_cGraph.SetPen(&GraphPen);
	// отриcовываем график
	for (int i = 0; i < SelectorBegin; i++)
		m_cGraph.DrawLine(i,Rect.bottom,i, Rect.bottom - (int)(Points->operator [] (i) * Rect.bottom));
	for (int i = SelectorEnd; i < Points->Length(); i++)
		m_cGraph.DrawLine(i,Rect.bottom,i, Rect.bottom - (int)(Points->operator [] (i) * Rect.bottom));

	// рисование  селектора
	IB_Pen SelectorPen(PS_SOLID,1,RGB(0xff,0xff,0x00));
	m_cGraph.SetPen(&SelectorPen);
	for (int i = SelectorBegin; i < SelectorEnd; i++)
		m_cGraph.DrawLine(i,Rect.bottom,i, Rect.bottom - (int)(Points->operator [] (i) * Rect.bottom));
}

void RRangingFrame::CreateDrawScene()
{
	RECT Rect;

	// создаём графический буффер
	{
		HDC hdc = GetDC(m_hHandle);
		if (!hdc)
			return;

		GetClientRect(Rect);

		if (!m_cScene.CreateCompatible(hdc,Rect.right,Rect.bottom))
			return;

		DeleteDC(hdc);
	}

	// инициализируем отступы от краёв
	int Indent = 15; // отступ для низа и боков
	int TopIndent = 30; // отступ для верха

	// получаем размер генерируемой картинки с учётом отступов
	int ImageWidth = Rect.right - 2 * Indent;
	int ImageHeight = Rect.bottom - (Indent + TopIndent);

	m_cScene.Blacken();

	if (ImageWidth <= 0 || ImageHeight <= 0)
		return;

	if (m_cGraph.GetDC())
		if (!StretchBlt(m_cScene.GetDC(), Indent, TopIndent, ImageWidth, ImageHeight, m_cGraph.GetDC(),0,0,m_cGraph.GetWidth(),m_cGraph.GetHeight(),SRCCOPY))
			return;


	ARGBColor AxesColor = RGB(0xff,0xff,0xff);
	IB_Pen AxesPen(PS_SOLID,1,AxesColor);
	m_cScene.SetPen(&AxesPen);

	// рисуем вертикальную ось
	int PointX = Indent;
	int PointY = TopIndent - 20;
	m_cScene.DrawLine(Indent,Rect.bottom - Indent,
					  PointX,PointY);
	
	m_cScene.SetPixel(PointX - 1, PointY + 2, AxesColor);
	m_cScene.SetPixel(PointX + 1, PointY + 2, AxesColor);
	m_cScene.SetPixel(PointX - 1, PointY + 3, AxesColor);
	m_cScene.SetPixel(PointX + 1, PointY + 3, AxesColor);
	m_cScene.SetPixel(PointX - 1, PointY + 4, AxesColor);
	m_cScene.SetPixel(PointX + 1, PointY + 4, AxesColor);
	m_cScene.SetPixel(PointX - 1, PointY + 5, AxesColor);
	m_cScene.SetPixel(PointX + 1, PointY + 5, AxesColor);
	m_cScene.SetPixel(PointX - 2, PointY + 5, AxesColor);
	m_cScene.SetPixel(PointX + 2, PointY + 5, AxesColor);

	// рисуем горизонтальную ось
	PointX = Rect.right - Indent + 5;
	PointY = Rect.bottom - Indent;
	m_cScene.DrawLine(Indent,Rect.bottom - Indent,
					  PointX, PointY);

	m_cScene.SetPixel(PointX - 2, PointY - 1, AxesColor);
	m_cScene.SetPixel(PointX - 2, PointY + 1, AxesColor);
	m_cScene.SetPixel(PointX - 3, PointY - 1, AxesColor);
	m_cScene.SetPixel(PointX - 3, PointY + 1, AxesColor);
	m_cScene.SetPixel(PointX - 4, PointY - 1, AxesColor);
	m_cScene.SetPixel(PointX - 4, PointY + 1, AxesColor);
	m_cScene.SetPixel(PointX - 5, PointY - 1, AxesColor);
	m_cScene.SetPixel(PointX - 5, PointY + 1, AxesColor);
	m_cScene.SetPixel(PointX - 5, PointY - 2, AxesColor);
	m_cScene.SetPixel(PointX - 5, PointY + 2, AxesColor);


	// нанесение штриховки
	int LineCount = 10;
	double LineHeigth = (double)ImageHeight / LineCount;
	while (LineHeigth < 20 && LineCount > 0)
	{
		LineCount--;
		LineHeigth = (double)ImageHeight / LineCount;
	}

	IB_Pen GridPen(PS_DOT,1,RGB(0x7f,0x7f,0x7f));
	m_cScene.SetPen(&GridPen);
	m_cScene.SetBkMode(TRANSPARENT);
	for (int i = 1; i <= LineCount; i++)
	{
		int LineY = Rect.bottom - Indent - int(LineHeigth*i);
		m_cScene.DrawLine(Indent + 1,LineY, Rect.right - Indent,LineY);
	}

	IB_Font GridFont(TEXT("Calibri"),16);
	m_cScene.SetFont(&GridFont);
	m_cScene.SetTextColor(AxesColor);

	m_cScene.DrawText(0,0,TEXT("%"));
	m_cScene.DrawText(Indent + ImageWidth+5,Indent + ImageHeight,m_strHorText.c_str());

	SIZE GridTextSize;
	LPCTSTR tmpText = TEXT("SomeText");
	if (!GetTextExtentPoint32(m_cScene.GetDC(), tmpText,8,&GridTextSize))
	{
		return;
	}

	for (int i = 0; i <= LineCount; i++)
	{
		int LineY = Rect.bottom - Indent - int(LineHeigth*i);
		m_cScene.DrawText(Indent + 2,LineY - GridTextSize.cy,(RConverter::ToString((int)((LineHeigth*i)/ImageHeight * 100.0 + 0.5)) + TEXT("")).c_str());
	}

	IB_Font LegendFont(TEXT("Calibri"),18);
	m_cScene.SetFont(&LegendFont);
	SIZE LegendSize;
	if (!GetTextExtentPoint32(m_cScene.GetDC(), m_strLegend.c_str(),m_strLegend.size(),&LegendSize))
	{
		// не рисуем в таком случае
		return;
	}

	int LegendPosX = (Rect.right - LegendSize.cx) / 2;
	int LegendPosY = 5;
	m_cScene.DrawText(LegendPosX,LegendPosY,m_strLegend.c_str());
}