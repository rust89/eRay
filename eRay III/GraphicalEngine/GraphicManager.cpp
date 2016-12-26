
#include "GraphicManager.h"


GraphicManager::GraphicManager(void)
{
	DrawInRegion = false;
	GrInf.Proportions = true;

	GrInf.DrawRegion.left = 0;
	GrInf.DrawRegion.top = 0;
	GrInf.DrawRegion.right = 0;
	GrInf.DrawRegion.bottom = 0;
	GrInf.Notch = 5;
	GrInf.DivisionCount = 10;
	GrInf.DrawRegion.left = 0;
	GrInf.DrawRegion.right = 0;
	GrInf.DrawRegion.top = 0;
	GrInf.DrawRegion.bottom = 0;
	GrInf.GraphicIndent = 20;
	GraphType = HotNonGrad;

	GrInf.AmpTraffic = 0;

	UsingMultiCore = true;
	AccountRayRadByAutoScale = true;
	// �������� ��� �������������
}

GraphicManager::~GraphicManager(void)
{
	ClearAll();
}

void GraphicManager::StoreGrInf(void)
{
	tmpGrInf = GrInf;
}
void GraphicManager::ReStoreGrInf(void)
{
	GrInf = tmpGrInf;
}

HDC GraphicManager::GetDC(GraphTypes Type)
{
	// ���� ��� �����
	if (GrInf.OutPoints.Empty())
		return NULL;
	// ���� ���� �����
	switch(Type)
	{
		case Classic:
		{
			return ClassicImage.GetDC();
		}
		case HotNonGrad:
		{
			return CloudNonGrad.GetDC();
		}
		case HotGrad:
		{
			return CloudGrad.GetDC();
		}
	}
	return 0;
}
bool GraphicManager::SetDrawRegion(LPRECT dr)
{
	if (!dr) return false;
	GrInf.DrawRegion = *dr;
	return true;
}
bool GraphicManager::SetGraphRegion(lpfrectangle dr)
{
	if (!dr) return false;
	GrInf.GraphRegion = *dr;
	return true;	
}
bool GraphicManager::CalcGraphRegion(void)
{
	GrInf.RegionPoints.CutRegion(GrInf.GraphRegion);
	return true;	
}
bool GraphicManager::CalcNormalParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints) // ��������� ��������� ����������� �������, ����� ��� �� ����� ������� ������� ��� �����
{
	// ��������� ������ �������
	double RegionWidth = drawrect->getWidth();
	// ��������� ������ �������
	double RegionHeight = drawrect->getHeight();

	// ��������� ������������ ����������� �������
	// ����������� ���������� ��� ������������ ������� �� ���� ������� ��c������
	double ZoomX = (GrInf.DrawRegion.right - GrInf.DrawRegion.left - GrInf.GraphicIndent*2) / RegionWidth;
	double ZoomY = (GrInf.DrawRegion.bottom - GrInf.DrawRegion.top - GrInf.GraphicIndent*2) / RegionHeight;

	if (GrInf.Proportions)
	{
		if (ZoomX < ZoomY)
		{
			ZoomY = ZoomX;
		}
		else
		{
			ZoomX = ZoomY;
		}
	}
	// ������������ �������� �������
	double dx = GrInf.DrawRegion.left + GrInf.GraphicIndent + // ����� 
		        (GrInf.DrawRegion.right - GrInf.DrawRegion.left - 2*GrInf.GraphicIndent - RegionWidth * ZoomX)/2;
	double dy = GrInf.DrawRegion.bottom - GrInf.GraphicIndent - // ����� 
		        (GrInf.DrawRegion.bottom - GrInf.DrawRegion.top - 2*GrInf.GraphicIndent - RegionHeight * ZoomY)/2;
	// �������� ������ ���, ��� �� �� ��� ���������� �� ���� ������� ���������
	OutputPoints.Resize(InputPoints.Count());
	for (int i = 0; i < GrInf.OutPoints.Count(); i++)
	{
		OutputPoints[i].setX(static_cast<int>(ZoomX*(InputPoints[i].getX() - drawrect->getMinX()) + dx));
		OutputPoints[i].setY(static_cast<int>(-ZoomY*(InputPoints[i].getY() - drawrect->getMinY()) + dy)); // ����� ����� ����������� ������
		OutputPoints[i].setHole(InputPoints[i].IsHole());
	}
	// ������ ������� ����
	double NewMinX = ZoomX*(0) + dx, NewMaxX = ZoomX*(drawrect->getWidth()) + dx;
	double NewMinY = -ZoomY*(drawrect->getHeight()) + dy, NewMaxY = -ZoomY*(0) + dy;
	// ������ � ������ ������� � ��������
	double WidthByPixel = NewMaxX - NewMinX;
	double HeightByPixel = NewMaxY - NewMinY;
	GrInf.Ypos = (long)(NewMinX - WidthByPixel/RegionWidth * drawrect->getMinX()); // ������� ��� Y �� x
	GrInf.Xpos = (long)(NewMaxY + HeightByPixel/RegionHeight * drawrect->getMinY()); // ������� ��� X �� y
	GrInf.X0 = 0;
	GrInf.Y0 = 0;
	if (GrInf.Ypos < NewMinX) 
	{
		GrInf.Ypos = (long)NewMinX;
		GrInf.X0 = drawrect->getMinX();
	}
	if (GrInf.Ypos > NewMaxX)
	{
		GrInf.Ypos = (long)NewMaxX;
		GrInf.X0 = drawrect->getMaxX();
	}
	if (GrInf.Xpos < NewMinY) 
	{
		GrInf.Xpos = (long)NewMinY;
		GrInf.Y0 = drawrect->getMaxY();
	}
	
	if (GrInf.Xpos > NewMaxY) 
	{
		GrInf.Xpos = (long)NewMaxY;
		GrInf.Y0 = drawrect->getMinY();
	}
	
	GrInf.XPixelWeight = RegionWidth / WidthByPixel; // (����� / �������)
	GrInf.YPixelWeight = RegionHeight / HeightByPixel;
	return true;
}
bool GraphicManager::CalcZeroDeltaYParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints) // ��������� ���������� ��� ����������� y
{
	// ��������� ������ �������
	double RegionWidth = drawrect->getWidth();
	// ��������� ����������� ����������� ������� �� X
	double ZoomX = (GrInf.DrawRegion.right - GrInf.DrawRegion.left - GrInf.GraphicIndent*2) / RegionWidth;
	// ��������� ������� Y
	double PosY = drawrect->getMinY();
	// ������������ �������� �������
	double dx = GrInf.DrawRegion.left + GrInf.GraphicIndent + // ����� 
		        (GrInf.DrawRegion.right - GrInf.DrawRegion.left - 2*GrInf.GraphicIndent - RegionWidth * ZoomX)/2;
	
	double dy = GrInf.DrawRegion.bottom - GrInf.GraphicIndent - // ����� 
		        (GrInf.DrawRegion.bottom - GrInf.DrawRegion.top - 2*GrInf.GraphicIndent)/2;
	
	// �������� ������ ���, ��� �� �� ��� ���������� �� ���� ������� ���������
	OutputPoints.Resize(InputPoints.Count());
	for (int i = 0; i < OutputPoints.Count(); i++)
	{
		OutputPoints[i].setX(static_cast<int>(ZoomX*(InputPoints[i].getX() - drawrect->getMinX()) + dx));
		OutputPoints[i].setY(static_cast<int>(-(InputPoints[i].getY() - PosY) + dy));
		OutputPoints[i].setHole(InputPoints[i].IsHole());
	}
	// ������ ������� ����
	double NewMinX = ZoomX*(0) + dx, NewMaxX = ZoomX*(drawrect->getWidth()) + dx;
	double NewPosY = dy; // � ����� � �������?
	// ������ � ������ ������� � ��������
	double WidthByPixel = NewMaxX - NewMinX; 
	// ������� ������ � ����� �������
	GrInf.XPixelWeight = RegionWidth / WidthByPixel;
	GrInf.YPixelWeight = RegionWidth / WidthByPixel;
	// ������� ���� � ������� ���������
	GrInf.Ypos = static_cast<long>(NewMinX - WidthByPixel/RegionWidth * drawrect->getMinX()); // ������� ��� Y �� x
	GrInf.Xpos = static_cast<long>(NewPosY + WidthByPixel/RegionWidth * PosY); // ������� ��� X �� y
	// ������ ���������� ����� � ������ ���������
	GrInf.X0 = 0;
	GrInf.Y0 = 0;
	if (GrInf.Ypos < NewMinX) 
	{
		GrInf.Ypos = static_cast<long>(NewMinX);
		GrInf.X0 = drawrect->getMinX();
	}
	if (GrInf.Ypos > NewMaxX) 
	{
		GrInf.Ypos = static_cast<long>(NewMaxX);
		GrInf.X0 = drawrect->getMaxX();
	}
	
	if (GrInf.Xpos < GrInf.DrawRegion.top + GrInf.GraphicIndent) 
	{
		GrInf.Y0 = -((GrInf.DrawRegion.top + GrInf.GraphicIndent) - GrInf.Xpos)*GrInf.XPixelWeight;
		GrInf.Xpos = static_cast<long>(GrInf.DrawRegion.top + GrInf.GraphicIndent);
	}
	if (GrInf.Xpos > GrInf.DrawRegion.bottom - GrInf.GraphicIndent) 
	{
		GrInf.Y0 = (GrInf.Xpos - (GrInf.DrawRegion.bottom - GrInf.GraphicIndent))*GrInf.XPixelWeight;
		GrInf.Xpos = static_cast<long>(GrInf.DrawRegion.bottom - GrInf.GraphicIndent);
	}
	return true;
}

bool GraphicManager::CalcZeroDeltaXParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints) // ��������� ���������� ��� ����������� y
{
	// ��������� ������ �������
	double RegionHeight = drawrect->getWidth();
	// ��������� ����������� ����������� ������� �� Y
	double ZoomY = (GrInf.DrawRegion.bottom - GrInf.DrawRegion.top - GrInf.GraphicIndent*2) / RegionHeight;
	// ��������� ������� X
	double PosX = drawrect->getMinX();
	// ������������ �������� �������
	double dx = GrInf.DrawRegion.left + GrInf.GraphicIndent + // ����� 
		        (GrInf.DrawRegion.right - GrInf.DrawRegion.left - 2*GrInf.GraphicIndent)/2;

	double dy = GrInf.DrawRegion.bottom - GrInf.GraphicIndent - // ����� 
		        (GrInf.DrawRegion.bottom - GrInf.DrawRegion.top - 2*GrInf.GraphicIndent - RegionHeight * ZoomY)/2;
	
	// �������� ������ ���, ��� �� �� ��� ���������� �� ���� ������� ���������
	OutputPoints.Resize(InputPoints.Count());
	for (int i = 0; i < OutputPoints.Count(); i++)
	{
		OutputPoints[i].setX(static_cast<int>((InputPoints[i].getX() - PosX) + dx));
		OutputPoints[i].setY(static_cast<int>(-ZoomY*(InputPoints[i].getY() - drawrect->getMinY()) + dy));
		OutputPoints[i].setHole(InputPoints[i].IsHole());
	}
	// ������ ������� ����
	double NewPosX = dx;
	double NewMinY = -ZoomY*(drawrect->getHeight()) + dy, NewMaxY = -ZoomY*(0) + dy;

	// ������ � ������ ������� � ��������
	double HeightByPixel = NewMaxY - NewMinY; 
	// ������� ������ � ����� �������
	GrInf.XPixelWeight = RegionHeight / HeightByPixel;
	GrInf.YPixelWeight = RegionHeight / HeightByPixel;
	// ������� ���� � ������� ���������
	GrInf.Ypos = static_cast<long>(NewPosX - HeightByPixel/RegionHeight * PosX); // ������� ��� Y �� x
	GrInf.Xpos = static_cast<long>(NewMaxY + HeightByPixel/RegionHeight * drawrect->getMinY()); // ������� ��� X �� y
	// ������ ���������� ����� � ������ ���������
	GrInf.X0 = 0;
	GrInf.Y0 = 0;
	if (GrInf.Ypos < GrInf.DrawRegion.left + GrInf.GraphicIndent) 
	{
		GrInf.X0 = ((GrInf.DrawRegion.left + GrInf.GraphicIndent) - GrInf.Ypos)*GrInf.YPixelWeight;
		GrInf.Ypos = static_cast<long>(GrInf.DrawRegion.left + GrInf.GraphicIndent);
	}
	if (GrInf.Ypos > GrInf.DrawRegion.right - GrInf.GraphicIndent) 
	{
		GrInf.X0 = -(GrInf.Ypos - (GrInf.DrawRegion.right - GrInf.GraphicIndent))*GrInf.YPixelWeight;
		GrInf.Ypos = static_cast<long>(GrInf.DrawRegion.right - GrInf.GraphicIndent);
	}
	if (GrInf.Xpos < GrInf.DrawRegion.top + GrInf.GraphicIndent) 
	{
		GrInf.Y0 = -((GrInf.DrawRegion.top + GrInf.GraphicIndent) - GrInf.Xpos)*GrInf.XPixelWeight;
		GrInf.Xpos = static_cast<long>(GrInf.DrawRegion.top + GrInf.GraphicIndent);
	}
	if (GrInf.Xpos > GrInf.DrawRegion.bottom - GrInf.GraphicIndent) 
	{
		GrInf.Y0 = (GrInf.Xpos - (GrInf.DrawRegion.bottom - GrInf.GraphicIndent))*GrInf.XPixelWeight;
		GrInf.Xpos = static_cast<long>(GrInf.DrawRegion.bottom - GrInf.GraphicIndent);
	}
	return true;
}

bool GraphicManager::CalcZeroDeltaXYParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints) // ��������� ���������� ��� ����������� y
{
	double PosX = drawrect->getMinX();
	double PosY = drawrect->getMinY();
	// ������������ �������� �������
	double dx = GrInf.DrawRegion.left + GrInf.GraphicIndent + // ����� 
		        (GrInf.DrawRegion.right - GrInf.DrawRegion.left - 2*GrInf.GraphicIndent)/2;

	double dy = GrInf.DrawRegion.bottom - GrInf.GraphicIndent - // ����� 
		        (GrInf.DrawRegion.bottom - GrInf.DrawRegion.top - 2*GrInf.GraphicIndent)/2;
	
	// �������� ������ ���, ��� �� �� ��� ���������� �� ���� ������� ���������
	GrInf.OutPoints.Resize(InputPoints.Count());
	for (int i = 0; i < OutputPoints.Count(); i++)
	{
		OutputPoints[i].setX(static_cast<int>((InputPoints[i].getX() - PosX) + dx));
		OutputPoints[i].setY(static_cast<int>(-(InputPoints[i].getY() - PosY) + dy));
		OutputPoints[i].setHole(InputPoints[i].IsHole());
	}
	// ������ ������� ����
	double NewPosX = dx;
	double NewPosY = dy;
	 
	// ������� ������ � ����� �������
	if (PosX == 0.00 && PosY == 0.00)
	{
		GrInf.XPixelWeight = 1.00/(GrInf.DivisionCount * GrInf.Notch);
		GrInf.YPixelWeight = 1.00/(GrInf.DivisionCount * GrInf.Notch);
	}
	else if (PosX == 0.00 && PosY != 0.00)
	{
		GrInf.XPixelWeight = (double)abs(PosY/dx);
		GrInf.YPixelWeight = (double)abs(PosY/dy);
	}
	else if (PosX != 0.00 && PosY == 0.00)
	{
		GrInf.XPixelWeight = (double)abs(PosX/dx);
		GrInf.YPixelWeight = (double)abs(PosX/dy);
	}
	else
	{
		GrInf.XPixelWeight = (double)abs(PosX/dx);
		GrInf.YPixelWeight = (double)abs(PosY/dy);
	}
	// ������� ���� � ������� ���������
	GrInf.Ypos = (long)(NewPosX - 1.00/GrInf.XPixelWeight * PosX); // ������� ��� Y �� x
	GrInf.Xpos = (long)(NewPosY + 1.00/GrInf.YPixelWeight * PosY); // ������� ��� X �� y
	// ������ ���������� ����� � ������ ���������
	GrInf.X0 = 0;
	GrInf.Y0 = 0;
	if (GrInf.Ypos < GrInf.DrawRegion.left + GrInf.GraphicIndent) 
	{
		GrInf.X0 = ((GrInf.DrawRegion.left + GrInf.GraphicIndent) - GrInf.Ypos)*GrInf.YPixelWeight;
		GrInf.Ypos = (long)(GrInf.DrawRegion.left + GrInf.GraphicIndent);
	}
	if (GrInf.Ypos > GrInf.DrawRegion.right - GrInf.GraphicIndent) 
	{
		GrInf.X0 = -(GrInf.Ypos - (GrInf.DrawRegion.right - GrInf.GraphicIndent))*GrInf.YPixelWeight;
		GrInf.Ypos = (long)(GrInf.DrawRegion.right - GrInf.GraphicIndent);
	}
	if (GrInf.Xpos < GrInf.DrawRegion.top + GrInf.GraphicIndent) 
	{
		GrInf.Y0 = -((GrInf.DrawRegion.top + GrInf.GraphicIndent) - GrInf.Xpos)*GrInf.XPixelWeight;
		GrInf.Xpos = (long)(GrInf.DrawRegion.top + GrInf.GraphicIndent);
	}
	if (GrInf.Xpos > GrInf.DrawRegion.bottom - GrInf.GraphicIndent) 
	{
		GrInf.Y0 = (GrInf.Xpos - (GrInf.DrawRegion.bottom - GrInf.GraphicIndent))*GrInf.XPixelWeight;
		GrInf.Xpos = (long)(GrInf.DrawRegion.bottom - GrInf.GraphicIndent);
	}
	return true;
}

bool GraphicManager::CalcParam(void)
{	
	// ������������� ����������
	frectangle drawrect;
	ffigure *InputPoints;

	if (DrawInRegion) // ���� ����������, �� ��������� ������
	{
		GrInf.RegionPoints = GrInf.PointMas;
		if (!GrInf.RegionPoints.CutRegion(GrInf.GraphRegion)) return false;
		InputPoints = &GrInf.RegionPoints;
		drawrect = GrInf.GraphRegion;
	}
	else
	{
		// ����� - ��������� ����
		InputPoints = &GrInf.PointMas;
		if (!InputPoints->GetRegion(&drawrect))
			return false;

		if (AccountRayRadByAutoScale)
		{	
			drawrect.setMinX(drawrect.getMinX() - GrInf.RayRadius);
			drawrect.setMaxX(drawrect.getMaxX() + GrInf.RayRadius);
			drawrect.setMinY(drawrect.getMinY() - GrInf.RayRadius);
			drawrect.setMaxY(drawrect.getMaxY() + GrInf.RayRadius);	
		}
	}

	double DeltaX = drawrect.getWidth();
	double DeltaY = drawrect.getHeight();
	if (DeltaX != 0.00 && DeltaY != 0.00)
		return CalcNormalParam(&drawrect,*InputPoints,GrInf.OutPoints);
	if (DeltaX != 0.00 && DeltaY == 0.00)
		return CalcZeroDeltaYParam(&drawrect,*InputPoints,GrInf.OutPoints);
	if (DeltaX == 0.00 && DeltaY != 0.00)
		return CalcZeroDeltaXParam(&drawrect,*InputPoints,GrInf.OutPoints);
	if (DeltaX == 0.00 && DeltaY == 0.00)
		return CalcZeroDeltaXYParam(&drawrect,*InputPoints,GrInf.OutPoints);
	return false;
}

bool GraphicManager::DrawScene(HDC hdc, int xScroll, int yScroll)
{
	HDC ActualDC = GetDC(GraphType);
	// ���� ��� �����
	if (!ActualDC) return true;
	if (!BitBlt(hdc,GrInf.DrawRegion.left + xScroll,GrInf.DrawRegion.top + yScroll,GrInf.DrawRegion.right - GrInf.DrawRegion.left,
			GrInf.DrawRegion.bottom - GrInf.DrawRegion.top,ActualDC,GrInf.DrawRegion.left,GrInf.DrawRegion.top,SRCCOPY))
	{
		return false;
	}
	return true;
}

bool GraphicManager::DrawScene(HDC hdc)
{
	return DrawScene(hdc,0,0);
}

bool GraphicManager::CanDraw(void)
{
	return GetDC(GraphType) != NULL ? true : false;
}

bool GraphicManager::BuildClassicImage(HDC hdc)
{
	if (!ClassicImage.CreateCompatible(hdc,GrInf.DrawRegion.right - GrInf.DrawRegion.left,GrInf.DrawRegion.bottom - GrInf.DrawRegion.top))
	{
		return false;
	}
	if(!ClassGr.DrawScene(&ClassicImage,&GrInf))
	{
		return false;
	}
	return true;
}
bool GraphicManager::BuildCloud(HDC hdc)
{
	// ������ ����������� �����
	if (!CloudGrad.CreateCompatible(hdc,GrInf.DrawRegion.right - GrInf.DrawRegion.left,GrInf.DrawRegion.bottom - GrInf.DrawRegion.top) ||
		!CloudNonGrad.CreateCompatible(hdc,GrInf.DrawRegion.right - GrInf.DrawRegion.left,GrInf.DrawRegion.bottom - GrInf.DrawRegion.top))
	{
		return false;
	}

	if (UsingMultiCore)
	{
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		if (!CloudGr.DrawScene(&CloudGrad,&CloudNonGrad,&GrInf,SysInfo.dwNumberOfProcessors)) return false;
	}
	else
	{
		if(!CloudGr.DrawScene(&CloudGrad,&CloudNonGrad,&GrInf)) return false;
	}
	CloudGradDefault.Copy(CloudGrad);
	return true;
}

bool GraphicManager::ReBuildAll(HDC hdc)
{
	return (!GrInf.OutPoints.Empty() ? 
		(BuildClassicImage(hdc) && BuildCloud(hdc)) : true);
}

bool GraphicManager::BuildCloudOutRanging(PointsFieldFor3D *pff3d)
{
	return (!GrInf.OutPoints.Empty() ? 
		(CloudGr.FillOutRanging(&GrInf,pff3d)) : true);
}

bool GraphicManager::CalculateParams(void)
{
	return CalcParam();
}

void GraphicManager::AddXY(double x, double y)
{
	GrInf.PointMas.AddPoint(fpoint(x, y, 0, false));
}

void GraphicManager::AddHole(void)
{
	GrInf.PointMas.AddHole();
}

void GraphicManager::ClearXY(void)
{
	GrInf.PointMas.Clear();
}

bool GraphicManager::EmptyXY(void)
{
	return GrInf.PointMas.Empty();
}

void GraphicManager::SetRayRadius(double NewRayRadius)
{
	this->GrInf.RayRadius = NewRayRadius;
}

void GraphicManager::SetRayVoltage(double NewRayVoltage)
{
	this->GrInf.RayVoltage = NewRayVoltage;
}

int GraphicManager::GetImageWidth(void)
{
	return GrInf.DrawRegion.right -  GrInf.DrawRegion.left;
}

int GraphicManager::GetImageHeight(void)
{
	return GrInf.DrawRegion.bottom -  GrInf.DrawRegion.top;
}

void GraphicManager::GetDrawRegion(RECT *Rect)
{
	*Rect = GrInf.DrawRegion;
}

bool GraphicManager::CaptureImage(LPCTSTR FileName)
{
	switch(GraphType)
	{
		// ������������ ������
		case Classic:
		{
			return ClassicImage.SaveToFile(FileName);
		}
		// �������� ������
		case HotNonGrad:
		{
			return CloudNonGrad.SaveToFile(FileName);
		}
		// ����������� ������
		case HotGrad:
		{
			return CloudGrad.SaveToFile(FileName);
		}
	}
	return false;
}

void GraphicManager::AddContrast(int Contrast) 
{
	CloudGrad.Copy(CloudGradDefault); 
	CloudGrad.Contrast(Contrast); 
}

void GraphicManager::SetRayAmpTraffic(double NewRayAmpTraffic)
{
	GrInf.AmpTraffic = NewRayAmpTraffic;
}

double GraphicManager::GetRayAmp()
{
	return GrInf.AmpTraffic;
}

int GraphicManager::GetGraphicIndent()
{
	return GrInf.GraphicIndent;
}

void GraphicManager::ClearAll(void)
{
	// ���������� ������
	ClassicImage.Clear();
	CloudGrad.Clear();
	CloudGradDefault.Clear();
	CloudNonGrad.Clear();

	GrInf.DrawRegion.left = 0;
	GrInf.DrawRegion.top = 0;
	GrInf.DrawRegion.right = 0;
	GrInf.DrawRegion.bottom = 0;
}