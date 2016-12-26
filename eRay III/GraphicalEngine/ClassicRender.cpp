#include "ClassicRender.h"

RenderClassic::RenderClassic(void)
{
	// инициализация параметров графика по умолчанию
	
	// инициализация параметров осей
	AxesInfo.Color = RGB(0x00,0x00,0x00);
	AxesInfo.Pen.Reset(PS_SOLID,1,AxesInfo.Color);
	AxesInfo.Font.Reset(0,12);
	
	// инициализация параметров границы
	BoundaryInfo.Pen.Reset(PS_DASH,1,RGB(0x00,0xff,0x00));

	// инициализация параметров графика
	GraphColor = RGB(0xff,0x00,0x00);
	GraphWidth = 1;
	GraphPen.Reset(PS_SOLID,GraphWidth,GraphColor);

	NodeWidth = 3;
	NodeBrush.Reset(GraphColor);

	BackgroudBrush.Reset(RGB(0xff,0xff,0xff));

	vDrawNode = true;
	vDrawAxes = true;
	vDrawBoundary = true;
}

bool RenderClassic::DrawBackground(ImageBuffer *ib, lpGraphInfo lpgi)
{
	if (!ib->FillRect(&lpgi->DrawRegion,&BackgroudBrush))
	{
		return false;
	}
	return true;
}

bool RenderClassic::DrawScene(ImageBuffer *ib, lpGraphInfo lpgi)
{
	if (!DrawBackground(ib,lpgi)) return true;
	if (vDrawBoundary) 
	{
		RECT RegionRect = {lpgi->DrawRegion.left + lpgi->GraphicIndent,
						   lpgi->DrawRegion.top + lpgi->GraphicIndent,
						   lpgi->DrawRegion.right - lpgi->GraphicIndent,
						   lpgi->DrawRegion.bottom - lpgi->GraphicIndent};
		if (!DrawBoundary(ib,&RegionRect,&BoundaryInfo)) return false;
	}
	if (vDrawAxes) if (!DrawAxes(ib,&AxesInfo,lpgi)) return true;
	if (!DrawGraph(ib,lpgi)) return true;
	return true;
}

bool RenderClassic::DrawGraph(ImageBuffer *ib, lpGraphInfo lpgi)
{
	if (!ib->SetBkMode(TRANSPARENT)) return false;
	ib->SetPen(&GraphPen); //if (SelectObject(ib->GetDC(),GraphPen) == NULL) return false;
	if (vDrawNode) ib->SetBrush(&NodeBrush); //if (vDrawNode) if (SelectObject(ib->GetDC(),NodeBrush) == NULL) return false;
	// отрисовка графика
	for (int i = 1, progress = 0; i < lpgi->OutPoints.Count(); i++,progress++)
	{
		if (lpgi->OutPoints[i - 1].IsHole() || lpgi->OutPoints[i].IsHole()) // если предыдущий интервал не дырка
		{
			continue;
		}
		else
		{
			if (!ib->DrawLine((long)lpgi->OutPoints[i - 1].getX(),(long)lpgi->OutPoints[i - 1].getY(),(long)lpgi->OutPoints[i].getX(),(long)lpgi->OutPoints[i].getY()))
				return false;
			if (vDrawNode)
			{
				if (!DrawGraphNode(ib,(long)lpgi->OutPoints[i - 1].getX(),(long)lpgi->OutPoints[i - 1].getY(),3)) return false;
				if (!DrawGraphNode(ib,(long)lpgi->OutPoints[i].getX(),(long)lpgi->OutPoints[i].getY(),3)) return false;
			}
		}
	}
	return true;
}

bool RenderClassic::DrawGraphNode(ImageBuffer *ib, long xi, long yi, long width)
{
	return Ellipse(ib->GetDC(),xi-width,yi-width,xi+width,yi+width) == TRUE ? true : false;
}