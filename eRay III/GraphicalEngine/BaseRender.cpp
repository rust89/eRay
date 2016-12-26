
#include "BaseRender.h"


bool BaseRender::DrawBoundary(ImageBuffer *ib, LPRECT Region, LPBOUNDARYINFO lpBoundaryInfo)
{
	ib->SetBkMode(TRANSPARENT);
	ib->SetPen(&(lpBoundaryInfo->Pen));
	
	if (!ib->MoveTo(Region->left,Region->top))
		return false;
	// крышка
	if (!ib->LineTo(Region->right,Region->top))
		return false;
	// правая стенка
	if (!ib->LineTo(Region->right,Region->bottom))
		return false;
	// нижняя стенка
	if (!ib->LineTo(Region->left,Region->bottom))
		return false;
	// левая стенка
	if (!ib->LineTo(Region->left,Region->top))
		return false;
	
	return true;
}

bool BaseRender::DrawAxes(ImageBuffer *ib, LPAXESINFO lpAxesInfo,lpGraphInfo lpgi)
{
	if (!lpgi) // передана нулевая структура 
	{
		return false;
	}
	// устанавливаем параметры отображения для данного буфера
	ib->SetTextColor(lpAxesInfo->Color);
	ib->SetBkMode(TRANSPARENT);
	ib->SetFont(&(lpAxesInfo->Font));

	long XBegin = lpgi->DrawRegion.left + lpgi->GraphicIndent,  XEnd = lpgi->DrawRegion.right - lpgi->GraphicIndent;
	long YBegin = lpgi->DrawRegion.bottom - lpgi->GraphicIndent, YEnd = lpgi->DrawRegion.top + lpgi->GraphicIndent;
	ib->SetPen(&(lpAxesInfo->Pen)); //if (SelectObject(ib->GetDC(),lpAxesInfo->hPen) == NULL) return false;
	if (!ib->DrawLine(XBegin, lpgi->Xpos, XEnd, lpgi->Xpos)) // отрисовка Ox
		return false;
	if (!ib->DrawLine(lpgi->Ypos, YBegin, lpgi->Ypos, YEnd)) // отрисовка Oy
		return false;

	// наносим засечки на координатные оси
	long NotchBegin, NotchEnd;
	// наносим деления на Ox
	NotchBegin = lpgi->Xpos - 1;
	NotchEnd = lpgi->Xpos + 2;
	for(long step = lpgi->Notch, StepCount = lpgi->DivisionCount * lpgi->Notch;/* нет условия */;step += lpgi->Notch)
	{
		long BottomBorder = lpgi->Ypos - step;
		long TopBorder = lpgi->Ypos + step;
		if (TopBorder >= XEnd && BottomBorder <= XBegin) break;
		if (BottomBorder > XBegin) 
		{
			if (step % StepCount == 0)
			{
				ib->DrawLine(BottomBorder, NotchBegin - 2, BottomBorder, NotchEnd + 2);
				PrintNumber(ib, BottomBorder, NotchEnd + 5, - lpgi->XPixelWeight * step + lpgi->X0);
			} else ib->DrawLine(BottomBorder, NotchBegin, BottomBorder, NotchEnd);
		}
		if (TopBorder < XEnd) 
		{	
			if (step % StepCount == 0) 
			{
				ib->DrawLine(TopBorder, NotchBegin - 2, TopBorder, NotchEnd + 2);
				PrintNumber(ib, TopBorder, NotchEnd + 5, lpgi->XPixelWeight * step + lpgi->X0);
			}
			else ib->DrawLine(TopBorder, NotchBegin, TopBorder, NotchEnd);
		}
	}
	// наносим деления на Oy
	NotchBegin = lpgi->Ypos - 1;
	NotchEnd = lpgi->Ypos + 2;
	for(long step = lpgi->Notch, StepCount = lpgi->DivisionCount * lpgi->Notch;/* нет условия */;step += lpgi->Notch)
	{
		long BottomBorder = lpgi->Xpos - step;
		long TopBorder = lpgi->Xpos + step;
		if (BottomBorder <= YEnd && TopBorder >= YBegin) break;
		if (BottomBorder > YEnd) 
		{
			if (step % StepCount == 0)
			{
				ib->DrawLine(NotchBegin - 2, BottomBorder, NotchEnd + 2, BottomBorder);
				PrintNumber(ib, NotchEnd + 5, BottomBorder, lpgi->YPixelWeight * step + lpgi->Y0);
			}
			else ib->DrawLine(NotchBegin, BottomBorder, NotchEnd, BottomBorder);
		}
		if (TopBorder < YBegin) 
		{
			if (step % StepCount == 0)
			{
				ib->DrawLine(NotchBegin - 2, TopBorder, NotchEnd + 2, TopBorder);
				PrintNumber(ib,NotchEnd + 5, TopBorder, - lpgi->YPixelWeight * step + lpgi->Y0);
			}
			else ib->DrawLine(NotchBegin, TopBorder, NotchEnd, TopBorder);
		}
	}
	// отрисовка стрелочки

	ib->DrawLine(XEnd, lpgi->Xpos, XEnd + 10 , lpgi->Xpos);
	ib->DrawLine(XEnd + 10, lpgi->Xpos, XEnd + 5 ,lpgi->Xpos + 3); // на Ox
	ib->DrawLine(XEnd + 10, lpgi->Xpos, XEnd + 5 ,lpgi->Xpos - 3);
	// подпись осей
	ib->DrawText(XEnd + 10, lpgi->Xpos, _TEXT("x"));

	ib->DrawLine(lpgi->Ypos, YEnd, lpgi->Ypos ,YEnd - 10);
	ib->DrawLine(lpgi->Ypos + 3, YEnd - 5, lpgi->Ypos ,YEnd - 10); // на Oy
	ib->DrawLine(lpgi->Ypos - 3, YEnd - 5, lpgi->Ypos ,YEnd - 10);
	// подпись оси
	ib->DrawText(lpgi->Ypos + 5 ,YEnd - 10, _TEXT("y"));

	return true;
}

bool BaseRender::PrintNumber(ImageBuffer *ib, long X, long Y, double Value)
{
	gstring OutText = DoubleToGString(Value);
	if (!OutText.size()) return false;
	if (!ib->DrawText(X, Y, OutText.c_str())) return false;
	return true;
}

gstring BaseRender::DoubleToGString(double value)
{
	const int BUFFER_SIZE = 100;
	_TCHAR BUFFER[BUFFER_SIZE];
	HRESULT hRes = StringCbPrintf(BUFFER,BUFFER_SIZE*sizeof(_TCHAR),_TEXT("%.2f"),value);
	if (FAILED(hRes)) return gstring(_TEXT(""));
	return gstring(BUFFER);
}