
#include "CloudRender.h"

CloudRender::CloudRender()
{
	// инициализация параметров осей
	AxesInfoGrad.Color = RGB(0x00,0xff,0xff);
	AxesInfoGrad.Pen.Reset(PS_SOLID,1,AxesInfoGrad.Color);
	AxesInfoGrad.Font.Reset(0,12);
	
	AxesInfoNoGrad.Color = RGB(0x00,0x00,0x00);
	AxesInfoNoGrad.Pen.Reset(PS_SOLID,1,AxesInfoNoGrad.Color);
	AxesInfoNoGrad.Font.Reset(0,12);

	BoundaryInfoGrad.Pen.Reset(PS_DASH,1,RGB(0x00,0xff,0x00));
	BoundaryInfoNoGrad.Pen.Reset(PS_DASH,1,RGB(0x00,0x00,0x00));

	vDrawBoundaryGrad = true;
	vDrawBoundaryNoGrad = true;

	vDrawAxesGrad = true;
	vDrawAxesNoGrad = true;

	vDrawScaleNoGrad = true;

	// рисовать ли узловую сетку
	vDrawStepGrid = true;

	// кол-во пикселей на один узел сетки
	PixelInOneStep = 30;
}

bool CloudRender::DrawScene(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi)
{
	Voltage = lpgi->RayVoltage;
	PointCount = lpgi->OutPoints.Count();
	if (!DrawSceneThread1(ib1,ib2,lpgi)) return false;

	if (vDrawBoundaryGrad || vDrawBoundaryNoGrad)
	{
		RECT RegionRect = {lpgi->DrawRegion.left + lpgi->GraphicIndent,
				lpgi->DrawRegion.top + lpgi->GraphicIndent,
				lpgi->DrawRegion.right - lpgi->GraphicIndent,
				lpgi->DrawRegion.bottom - lpgi->GraphicIndent};
		if (vDrawBoundaryGrad) if (!DrawBoundary(ib1,&RegionRect,&BoundaryInfoGrad)) return false;
		if (vDrawBoundaryNoGrad) if (!DrawBoundary(ib2,&RegionRect,&BoundaryInfoNoGrad)) return false;
	}

	return DrawJewelry(ib1,ib2,lpgi);
}

bool CloudRender::DrawScene(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi, UINT CoreCount)
{
	Voltage = lpgi->RayVoltage;
	PointCount = lpgi->OutPoints.Count();
	if (CoreCount > 1 && 
		lpgi->OutPoints.Count() > 40 &&
		((long)(lpgi->RayRadius/lpgi->XPixelWeight) > (lpgi->DrawRegion.right - lpgi->DrawRegion.left) / 10))
	{
		if (!DrawSceneThread4(ib1,ib2,lpgi)) return false;
	}
	else
	{
		if (!DrawSceneThread2(ib1, ib2,lpgi)) return false;
	}

	if (vDrawBoundaryGrad || vDrawBoundaryNoGrad)
	{
		RECT RegionRect = {lpgi->DrawRegion.left + lpgi->GraphicIndent,
				lpgi->DrawRegion.top + lpgi->GraphicIndent,
				lpgi->DrawRegion.right - lpgi->GraphicIndent,
				lpgi->DrawRegion.bottom - lpgi->GraphicIndent};
		if (vDrawBoundaryGrad) if (!DrawBoundary(ib1,&RegionRect,&BoundaryInfoGrad)) return false;
		if (vDrawBoundaryNoGrad) if (!DrawBoundary(ib2,&RegionRect,&BoundaryInfoNoGrad)) return false;
	}

	return DrawJewelry(ib1,ib2,lpgi);
}

bool CloudRender::GetMinMaxField(double *field[], long xSize, long ySize, double &Min, double &Max, long double &total_sum)
{
	if (field == 0 || xSize == 0 || ySize == 0) return false;
	Min = field[0][0];
	Max = field[0][0];
	total_sum = 0;
	for (long i = 0; i < xSize; i++)
	{
		for (long j = 0; j < ySize; j++)
		{
			if (field[i][j] < Min) Min = field[i][j];
			if (field[i][j] > Max) Max = field[i][j];
			total_sum+= field[i][j];
		}
	}
	return true;
}

COLORREF CloudRender::ColorByValue(double value)
{
	if (value <= 0.1428571428571429)
		return RGB(0xff,0x00,0xff); // фиолетовый
	if (value <= 0.2857142857142857)
		return RGB(0x00,0x00,0xff); // синий
	if (value <= 0.4285714285714286)
		return RGB(0x00,0xff,0xff); // голубой
	if (value <= 0.5714285714285714)
		return RGB(0x00,0xff,0x00); // зелёный
	if (value <= 0.7142857142857143)
		return RGB(0xff,0xff,0x00); // жёлтый
	if (value <= 0.8571428571428571)
		return RGB(0xff,0x7f,0x00); // оранжевый
	
	return RGB(0xff,0x00,0x00); // красный
}

double CloudRender::MomentRadMas(int RadX, int RadY)
{
	double Result = 0;

	int End = RadX;
	for (int i = - RadX; i < End; i++)
	{
		// получаем координату точки по y
		double yyp = sqrt((double)(RadX*RadX - i*i));

		// расстояние точки (i,0) от центра
		double RadLen = RadX - sqrt((double)i*i);

		// мощность потока в точке (i,0) в зависимости от удаления от центра пятна
		double powerstream = RadX - RadX*exp(-(RadLen*RadLen)/(RadX*RadX));

		// учитываем параметр в точке
		Result+=(int)powerstream;

		for (int j = 1; j < yyp; j++)
		{
			// расстояние точки (i,j) от центра
			RadLen = RadX - sqrt((double)i*i + (double)j*j);
			// мощность потока в точке (i,0) в зависимости от удаления от центра пятна
			powerstream = RadX - RadX*exp(-(RadLen*RadLen)/(RadX*RadX));
			// учитываем параметра в точке врехнего полукруга
			powerstream+=(long)powerstream;
			// учитываем параметра в точке нижнего полукруга
			powerstream+=(long)powerstream;
		}
	}

	return Result;
}

void CloudRender::AddFieldRad(double **field, int xSize, int ySize, int X, int Y, int RadX, int RadY)
{
	if (X < xSize) // если не выходит за правую границу
	{
		if (Y < ySize) // если не выходит за нижнюю границу
		{
			int End = RadX;
			int RealX = X;
			int RealY = Y;
			for (int i = - RadX; i < End; i++)
			{
				// получаем координату точки по y
				double yyp = sqrt((double)(RadX*RadX - i*i));

				// расстояние точки (i,0) от центра
				double RadLen = RadX - sqrt((double)i*i);

				// мощность потока в точке (i,0) в зависимости от удаления от центра пятна
				double powerstream = RadX - RadX*exp(-(RadLen*RadLen)/(RadX*RadX));

				if (RealX + i < xSize && RealX + i >= 0)
				{
					// учитываем параметр в точке
					field[RealX + i][RealY]+=powerstream;

					for (int j = 1; j < yyp; j++)
					{
						// расстояние точки (i,j) от центра
						RadLen = RadX - sqrt((double)i*i + (double)j*j);
						// мощность потока в точке (i,0) в зависимости от удаления от центра пятна
						powerstream = RadX - RadX*exp(-(RadLen*RadLen)/(RadX*RadX));

						// учитываем параметра в точке врехнего полукруга
						if (RealY + j < ySize)
							field[RealX + i][RealY + j]+=powerstream;

						// учитываем параметра в точке нижнего полукруга
						if (RealY - j >= 0)
							field[RealX + i][RealY - j]+=powerstream;
					}
				}
			}
		}
	}
}

bool CloudRender::DrawSceneThread1(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi)
{
	if (!lpgi) return false;
	// ширина и высота облака
	int CloudWidth = lpgi->DrawRegion.right - lpgi->DrawRegion.left;
	int CloudHeight = lpgi->DrawRegion.bottom - lpgi->DrawRegion.top;
	
	// задаём размер облака
	double **cloud = new double*[CloudWidth];
	for (int i = 0; i < CloudWidth; i++)
	{
		cloud[i] = new double[CloudHeight];
		for(int j = 0; j < CloudHeight; j++)
		{
			cloud[i][j] = 0.00;
		}
	}
	// расчёт облака
	int ImagineRadius = (int)(lpgi->RayRadius/lpgi->XPixelWeight);

	for (int i = 0; i < lpgi->OutPoints.Count(); i++)
	{
		AddFieldRad(cloud,CloudWidth,CloudHeight,lpgi->OutPoints[i].getX(),lpgi->OutPoints[i].getY(),ImagineRadius,ImagineRadius);
	}
	
	double Min,Max;
	if (!GetMinMaxField(cloud,CloudWidth,CloudHeight,Min,Max,TotalPowerSum)) 
	{
		return false;
	}
	
	BaseCloudDump.ReSize(CloudWidth,CloudHeight);
	BaseCloudDump.SetPixelWeight(lpgi->XPixelWeight);
	// отрисовка облака в графический буфер
	BYTE cr = 0;
	BYTE cg = 0;
	BYTE cb = 0;
	double part;
	OutRanging.resize(CloudWidth);
	for (long i = 0; i < CloudWidth; i++)
	{
		OutRanging[i].resize(CloudHeight);
		for(long j = 0; j < CloudHeight; j++)
		{
			// сохраняем "сырой дамп" в буффер
			BaseCloudDump.Index(i,j) = cloud[i][j];
			part = cloud[i][j]/Max;
			OutRanging[i][j] = part;
			// отрисовка градиентного облака
			cr = (BYTE)(0xff*part);
			ib1->SetPixel(int(i), int(j),RGB(cr,cg,cb));
			// отрисовка радужного облака
			ib2->SetPixel(int(i), int(j),ColorByValue((double)cloud[i][j]/Max));
		}
	}

	for (int i = 0; i < CloudWidth; i++)
	{
		delete [] cloud[i];
	}
	delete [] cloud;

	FillAccum(lpgi);

	return true;
}

bool CloudRender::DrawSceneThread2(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi)
{
	if (!lpgi) return false;
	// ширина и высота облака
	int CloudWidth = lpgi->DrawRegion.right - lpgi->DrawRegion.left;
	int CloudHeight = lpgi->DrawRegion.bottom - lpgi->DrawRegion.top;

	WorkerParam param1;
	WorkerParam param2;

	param1.field = new double*[CloudWidth];
	param2.field = new double*[CloudWidth];
	for (long i = 0; i < CloudWidth; i++)
	{
		param1.field[i] = new double[CloudHeight];
		param2.field[i] = new double[CloudHeight];
		for(long j = 0; j < CloudHeight; j++)
		{
			param1.field[i][j] = 0.00;
			param2.field[i][j] = 0.00;
		}
	}

	
	int ImagineRadius = (int)(lpgi->RayRadius/lpgi->XPixelWeight);
	
	param1.Points = &lpgi->OutPoints;
	param1.PointsBegin = 0;
	param1.PointsEnd = lpgi->OutPoints.Count()/2;
	param1.xRad = ImagineRadius;
	param1.yRad = ImagineRadius;
	param1.xSize = CloudWidth;
	param1.ySize = CloudHeight;
	param1.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	
	param2.Points = &lpgi->OutPoints;
	param2.PointsBegin = lpgi->OutPoints.Count()/2;
	param2.PointsEnd = lpgi->OutPoints.Count();
	param2.xRad = ImagineRadius;
	param2.yRad = ImagineRadius;
	param2.xSize = CloudWidth;
	param2.ySize = CloudHeight;
	param2.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	const HANDLE hObjects[2] = {param1.hEvent, param2.hEvent};
	DWORD dwThreadID;
	HANDLE hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWork,&param1,CREATE_SUSPENDED,&dwThreadID);
	HANDLE hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWork,&param2,CREATE_SUSPENDED,&dwThreadID);
	if (!hThread1 || !hThread2)
	{
		return false;
	}

	ResumeThread(hThread1);
	ResumeThread(hThread2);

	CloseHandle(hThread1);
	CloseHandle(hThread2);

	WaitForMultipleObjects(2,hObjects,TRUE,INFINITE);
	
	for (long i = 0; i < CloudWidth; i++)
	{
		for(long j = 0; j < CloudHeight; j++)
		{
			param1.field[i][j] += param2.field[i][j];
		}
	}
	
	double Min,Max;
	if (!GetMinMaxField(param1.field,CloudWidth,CloudHeight,Min,Max,TotalPowerSum)) 
	{
		return false;
	}
	
	BaseCloudDump.ReSize(CloudWidth,CloudHeight);
	BaseCloudDump.SetPixelWeight(lpgi->XPixelWeight);
	// отрисовка облака в графический буфер
	BYTE cr = 0;
	BYTE cg = 0;
	BYTE cb = 0;
	double part;
	OutRanging.resize(CloudWidth);
	for (int i = 0; i < CloudWidth; i++)
	{
		OutRanging[i].resize(CloudHeight);
		for(int j = 0; j < CloudHeight; j++)
		{
			// сохраняем "сырой дамп" в буффер
			BaseCloudDump.Index(i,j) = param1.field[i][j];
			part = param1.field[i][j]/Max;
			OutRanging[i][j] = part;
			// отрисовка градиентного облака
			cr = (BYTE)(0xff*part);
			ib1->SetPixel(int(i), int(j),RGB(cr,cg,cb));
			// отрисовка радужного облака
			ib2->SetPixel(int(i), int(j),ColorByValue((double)param1.field[i][j]/Max));
		}
	}

	// очистка буферов
	for (int i = 0; i < CloudWidth; i++)
	{
		delete [] param1.field[i];
		delete [] param2.field[i];
	}
	delete [] param1.field;
	delete [] param2.field;
	
	FillAccum(lpgi);

	return true;
}

bool CloudRender::DrawSceneThread4(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi)
{
	if (!lpgi) return false;
	// ширина и высота облака
	int CloudWidth = lpgi->DrawRegion.right - lpgi->DrawRegion.left;
	int CloudHeight = lpgi->DrawRegion.bottom - lpgi->DrawRegion.top;

	WorkerParam param1;
	WorkerParam param2;
	WorkerParam param3;
	WorkerParam param4;

	param1.field = new double*[CloudWidth];
	param2.field = new double*[CloudWidth];
	param3.field = new double*[CloudWidth];
	param4.field = new double*[CloudWidth];
	for (long i = 0; i < CloudWidth; i++)
	{
		param1.field[i] = new double[CloudHeight];
		param2.field[i] = new double[CloudHeight];
		param3.field[i] = new double[CloudHeight];
		param4.field[i] = new double[CloudHeight];
		for(long j = 0; j < CloudHeight; j++)
		{
			param1.field[i][j] = 0x00;
			param2.field[i][j] = 0x00;
			param3.field[i][j] = 0x00;
			param4.field[i][j] = 0x00;
		}
	}

	
	int ImagineRadius = (int)(lpgi->RayRadius/lpgi->XPixelWeight);
	
	param1.Points = &lpgi->OutPoints;
	param1.PointsBegin = 0;
	param1.PointsEnd = lpgi->OutPoints.Count()/4;
	param1.xRad = ImagineRadius;
	param1.yRad = ImagineRadius;
	param1.xSize = CloudWidth;
	param1.ySize = CloudHeight;
	param1.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	
	param2.Points = &lpgi->OutPoints;
	param2.PointsBegin = lpgi->OutPoints.Count()/4;
	param2.PointsEnd = lpgi->OutPoints.Count()/2;
	param2.xRad = ImagineRadius;
	param2.yRad = ImagineRadius;
	param2.xSize = CloudWidth;
	param2.ySize = CloudHeight;
	param2.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	param3.Points = &lpgi->OutPoints;
	param3.PointsBegin = lpgi->OutPoints.Count()/2;
	param3.PointsEnd = static_cast<size_t>(lpgi->OutPoints.Count()*(3.0/4));
	param3.xRad = ImagineRadius;
	param3.yRad = ImagineRadius;
	param3.xSize = CloudWidth;
	param3.ySize = CloudHeight;
	param3.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	param4.Points = &lpgi->OutPoints;
	param4.PointsBegin = static_cast<size_t>(lpgi->OutPoints.Count()*(3.0/4));
	param4.PointsEnd = lpgi->OutPoints.Count();
	param4.xRad = ImagineRadius;
	param4.yRad = ImagineRadius;
	param4.xSize = CloudWidth;
	param4.ySize = CloudHeight;
	param4.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	const HANDLE hObjects[4] = {param1.hEvent, param2.hEvent, param3.hEvent, param4.hEvent};
	DWORD dwThreadID;
	HANDLE hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWork,&param1,CREATE_SUSPENDED,&dwThreadID);
	HANDLE hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWork,&param2,CREATE_SUSPENDED,&dwThreadID);
	HANDLE hThread3 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWork,&param3,CREATE_SUSPENDED,&dwThreadID);
	HANDLE hThread4 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWork,&param4,CREATE_SUSPENDED,&dwThreadID);

	if (!hThread1 || !hThread2 || !hThread3 || !hThread4)
	{
		return false;
	}

	ResumeThread(hThread1);
	ResumeThread(hThread2);
	ResumeThread(hThread3);
	ResumeThread(hThread4);

	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);

	WaitForMultipleObjects(4,hObjects,TRUE,INFINITE);
	
	for (int i = 0; i < CloudWidth; i++)
	{
		for(int j = 0; j < CloudHeight; j++)
		{
			param1.field[i][j] += param2.field[i][j] + param3.field[i][j] + param4.field[i][j];
		}
	}

		// очистка буферов
	for (int i = 0; i < CloudWidth; i++)
	{
		delete [] param2.field[i];
		delete [] param3.field[i];
		delete [] param4.field[i];
	}
	delete [] param2.field;
	delete [] param3.field;
	delete [] param4.field;
	
	double Min,Max;
	if (!GetMinMaxField(param1.field,CloudWidth,CloudHeight,Min,Max,TotalPowerSum)) 
	{
		return false;
	}
	
	BaseCloudDump.ReSize(CloudWidth,CloudHeight);
	BaseCloudDump.SetPixelWeight(lpgi->XPixelWeight);
	// отрисовка облака в графический буфер
	BYTE cr = 0;
	BYTE cg = 0;
	BYTE cb = 0;
	double part;
	OutRanging.resize(CloudWidth);
	for (int i = 0; i < CloudWidth; i++)
	{
		OutRanging[i].resize(CloudHeight);
		for(int j = 0; j < CloudHeight; j++)
		{
			// сохраняем "сырой дамп" в буффер
			BaseCloudDump.Index(i,j) = param1.field[i][j];	
			part = param1.field[i][j]/Max;
			OutRanging[i][j] = part;
			// отрисовка градиентного облака
			cr = (BYTE)(0xff*part);
			ib1->SetPixel(int(i), int(j),RGB(cr,cg,cb));
			// отрисовка радужного облака
			ib2->SetPixel(int(i), int(j),ColorByValue((double)param1.field[i][j]/Max));
		}
	}

	// очистка буферов
	for (int i = 0; i < CloudWidth; i++)
	{
		delete [] param1.field[i];
	}
	delete [] param1.field;

	FillAccum(lpgi);

	return true;
}

void CloudRender::ThreadWork(void *param)
{
	WorkerParam *wp = static_cast<WorkerParam*>(param);

	for (std::size_t i = wp->PointsBegin; i < wp->PointsEnd; i++)
	{
		AddFieldRad(wp->field,wp->xSize,wp->ySize,wp->Points->operator[](i).getX(),wp->Points->operator[](i).getY(),wp->xRad,wp->xRad);
	}
	SetEvent(wp->hEvent);
}

bool CloudRender::DrawScaleNoGrad(ImageBuffer *ib, lpGraphInfo lpgi)
{
	long ScaleWidth = 25;
	long ScaleItemHeight = 35;
	
	long ImageHeight = lpgi->DrawRegion.bottom - lpgi->DrawRegion.top - (lpgi->GraphicIndent << 1);
	if ((ScaleItemHeight << 3) > ImageHeight)
	{
		ScaleItemHeight = ImageHeight >> 3;
	}
	long ScaleX = lpgi->DrawRegion.right - lpgi->GraphicIndent - ScaleWidth + 1;
	long ScaleY = lpgi->GraphicIndent;

	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfHeight = max(5,ScaleItemHeight / 4);
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = DEFAULT_QUALITY;
	HFONT hScaleFont = CreateFontIndirect(&lf);
	
	SelectObject(ib->GetDC(),hScaleFont);
	ib->SetBkMode(TRANSPARENT);
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(0x00,0x00,0x00));
	if (!hPen) return false;
	SelectObject(ib->GetDC(),hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(0xff,0x00,0x00)); // красный
	if (!hBrush) return false;
	SelectObject(ib->GetDC(),hBrush);
	RECT ItemRect = {ScaleX,ScaleY,ScaleX + ScaleWidth,ScaleY + ScaleItemHeight + 1};
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top,ItemRect.right,ItemRect.bottom);
	DrawText(ib->GetDC(),_TEXT("100"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0xff,0x7f,0x00)); // оранжевый
	if (!hBrush) return false;
	SelectObject(ib->GetDC(),hBrush);
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top+=ScaleItemHeight,ItemRect.right,ItemRect.bottom+=ScaleItemHeight + 1);
	DrawText(ib->GetDC(),_TEXT("85.7"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0xff,0xff,0x00)); // жёлтый
	if (!hBrush) return false;
	SelectObject(ib->GetDC(),hBrush);
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top+=ScaleItemHeight,ItemRect.right,ItemRect.bottom+=ScaleItemHeight + 1);
	DrawText(ib->GetDC(),_TEXT("71.4"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0x00,0xff,0x00)); // зелёный
	if (!hBrush) return false;	
	SelectObject(ib->GetDC(),hBrush);
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top+=ScaleItemHeight,ItemRect.right,ItemRect.bottom+=ScaleItemHeight + 1);
	DrawText(ib->GetDC(),_TEXT("57.0"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0x00,0xff,0xff)); // голубой
	if (!hBrush) return false;	
	SelectObject(ib->GetDC(),hBrush);
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top+=ScaleItemHeight,ItemRect.right,ItemRect.bottom+=ScaleItemHeight + 1);
	DrawText(ib->GetDC(),_TEXT("42.9"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0x00,0x00,0xff)); // синий
	if (!hBrush) return false;		
	SelectObject(ib->GetDC(),hBrush);
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top+=ScaleItemHeight,ItemRect.right,ItemRect.bottom+=ScaleItemHeight + 1);
	DrawText(ib->GetDC(),_TEXT("28.6"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	hBrush = CreateSolidBrush(RGB(0xff,0x00,0xff)); // фиолетовый
	if (!hBrush) return false;		
	SelectObject(ib->GetDC(),hBrush);
	Rectangle(ib->GetDC(),ItemRect.left,ItemRect.top+=ScaleItemHeight,ItemRect.right,ItemRect.bottom+=ScaleItemHeight + 1);
	DrawText(ib->GetDC(),_TEXT("14.3"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hBrush);
	ItemRect.top+=ScaleItemHeight; ItemRect.bottom+=ScaleItemHeight + 1;
	DrawText(ib->GetDC(),_TEXT("%"),-1,&ItemRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	return true;
}

bool CloudRender::FillOutRanging(lpGraphInfo lpgi, PointsFieldFor3D *pf3d)
{
	if (!lpgi) return false;
	// ширина и высота облака
	int CloudWidth = lpgi->DrawRegion.right - lpgi->DrawRegion.left;
	int CloudHeight = lpgi->DrawRegion.bottom - lpgi->DrawRegion.top;

	// задаём размер облака
	double **cloud = new double*[CloudWidth];
	for (int i = 0; i < CloudWidth; i++)
	{
		cloud[i] = new double[CloudHeight];
		for(int j = 0; j < CloudHeight; j++)
		{
			cloud[i][j] = 0.00;
		}
	}

	// расчёт облака
	int ImagineRadius = (int)(lpgi->RayRadius/lpgi->XPixelWeight);

	for (int i = 0; i < lpgi->OutPoints.Count(); i++)
	{
		AddFieldRad(cloud,CloudWidth,CloudHeight,lpgi->OutPoints[i].getX(),lpgi->OutPoints[i].getY(),ImagineRadius,ImagineRadius);
	}
	
	double Min,Max;
	long double OtherSum;
	if (!GetMinMaxField(cloud,CloudWidth,CloudHeight,Min,Max,OtherSum)) 
	{
		return false;
	}
	
	double part;
	pf3d->ReSize(CloudWidth, CloudHeight);
	for (int i = 0; i < CloudWidth; i++)
	{
		for(int j = 0; j < CloudHeight; j++)
		{
			part = cloud[i][j]/Max;
			pf3d->Index(i,j) = part;
		}
	}

	// очистка мусора
	for (int i = 0; i < CloudWidth; i++)
	{
		delete [] cloud[i];
	}
	delete [] cloud;

	return true;
}

void CloudRender::FillAccum(lpGraphInfo lpgi)
{
	// вычисляем кол-во квадратов по горизонтале
	int AccumWidth = BaseCloudDump.GetWidth() / PixelInOneStep + (BaseCloudDump.GetWidth() % PixelInOneStep ? 1 : 0);
	// вычисляем кол-во квадратов по вертикале
	int AccumHeight = BaseCloudDump.GetHeight() / PixelInOneStep + (BaseCloudDump.GetHeight() % PixelInOneStep ? 1 : 0);
	
	// выделяем необходимое количество памяти
	ThreadAccum.resize(AccumWidth);
	for (std::size_t i = 0; i < ThreadAccum.size(); i++)
	{
		ThreadAccum[i].resize(AccumHeight);
	}

	// общий кпд
	TotalEfficiency = 0.78; 
	/*
	double SquereWidth = PixelInOneStep * BaseCloudDump.GetPixelWeight();
	double SquereHeight = PixelInOneStep * BaseCloudDump.GetPixelWeight();
	double SquereS = SquereWidth * SquereHeight;
	double CommonS = BaseCloudDump.GetWidth() * BaseCloudDump.GetHeight() * BaseCloudDump.GetPixelWeight() * BaseCloudDump.GetPixelWeight();
	*/
	// Вычисляем средний поток по площади c учётом эффективности
	double MidlPower = pow(lpgi->RayVoltage,2.5) * lpgi->AmpTraffic * TotalEfficiency * 1000;

	for (std::size_t i = 0; i < ThreadAccum.size(); i++)
	{
		int square_i_begin = i*PixelInOneStep; // начальный индекс в квадрате по i
		int square_i_end = (signed)(i+1)*PixelInOneStep < BaseCloudDump.GetWidth() ? 
			(i+1)*PixelInOneStep : BaseCloudDump.GetWidth(); // конечный индекс в квадрате по i
		for (std::size_t j = 0; j < ThreadAccum[i].size(); j++)
		{
			int square_j_begin = j*PixelInOneStep; // начальный индекс в квадрате по j
			// вычисление суммы для квадрата (i,j)
			int square_j_end = (signed)(j+1)*PixelInOneStep < BaseCloudDump.GetHeight() ? 
				(j+1)*PixelInOneStep : BaseCloudDump.GetHeight(); // конечный индекс в квадрате по j

			double tmp_acc = 0;
			for (int ii = square_i_begin; ii < square_i_end; ii++)
			{
				for (int jj = square_j_begin; jj < square_j_end; jj++)
				{
					tmp_acc += BaseCloudDump.Index(ii, jj);
				}
			}
			ThreadAccum[i][j].Percent = tmp_acc / TotalPowerSum;
			ThreadAccum[i][j].Power = ( tmp_acc * MidlPower / TotalPowerSum );
		}
	}
}

bool CloudRender::DrawStepGrid(ImageBuffer *ib, lpGraphInfo lpgi)
{
	IB_Pen Pen(PS_DOT,1,RGB(0x7f,0x7f,0x7f));
	ib->SetPen(&Pen);

	ib->SetBkMode(TRANSPARENT);

	// отрисовка вертикальных линий
	for (long i = 0; i < ib->GetWidth(); i += this->PixelInOneStep)
	{
		ib->DrawLine(i,0,i,ib->GetHeight());
	}

	// отрисовка горизонтальных линий
	for (long i = 0; i < ib->GetHeight(); i += this->PixelInOneStep)
	{
		ib->DrawLine(0,i,ib->GetWidth(),i);
	}
	return true;
}

bool CloudRender::DrawJewelry(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi)
{
	if (vDrawStepGrid)
		if (!DrawStepGrid(ib1,lpgi)) return false;

	if (vDrawAxesGrad)
		if (!DrawAxes(ib1,&AxesInfoGrad,lpgi)) return false;

	if (vDrawAxesNoGrad)
		if (!DrawAxes(ib2,&AxesInfoNoGrad,lpgi)) return false;

	if (vDrawScaleNoGrad)
		if (!DrawScaleNoGrad(ib2,lpgi)) return false;

	return true;
}

void CloudRender::SetTotalEfficiency(double Value)
{
	TotalEfficiency = Value;
}