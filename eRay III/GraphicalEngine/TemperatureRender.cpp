#include "TemperatureRender.h"

#include "..\WinLib\RConverter.h"

#ifdef _DEBUG
	#include <fstream>
#endif

TemperatureRender::TemperatureRender()
{
	// инициализируем радужный массив
	double r = 255;
	double g = 0;
	double b = 0;
	for(int i = 0, j = RainbowMasSize - 1; i < RainbowMasSize; i++, j--)
	{
		if(( i >= 0) && (i <= 13107))
			g+=0.0194552529182;
		if((i > 13107) && (i <= 26214))
			r-=0.0194552529182;
		if((i > 26214) && (i <= 39321))
			b+=0.0194552529182;
		if((i > 39321) && (i <= 52428))
			g-=0.0194552529182;
		if((i > 52428) && (i < RainbowMasSize))
			r+=0.0194552529182;
		RainbowMas[j]=RGB(r,g,b);
	}

	TemperatureFont.Reset(TEXT(""),14);
}


TemperatureRender::~TemperatureRender()
{

}

void TemperatureRender::SetQStreams(Expression &Qxl, Expression &Qxr, Expression &Qyl, Expression &Qyr, Expression &Qzb)
{
	m_fQxl = Qxl;
	m_fQxr = Qxr;
	m_fQyl = Qyl;
	m_fQyr = Qyr;
	m_fQzb = Qzb;
}

void TemperatureRender::SetBaseQStream(CloudField <double> &QztField, double Qzt)
{
	m_fQzt = Qzt;
	m_cQztField = QztField;
}

void TemperatureRender::SetDimensions(double L, double W, double H)
{
	m_fLength = L;
	m_fWidth = W;
	m_fHeight = H;
}

void TemperatureRender::ManualSteps(double DeltaTaw, int N, int M, int L)
{
	m_nN = N;
	m_nM = M;
	m_nL = L;
	m_fDeltaTaw = DeltaTaw;

	m_fDeltaX = m_fLength / m_nN;
	m_fDeltaY = m_fWidth / m_nM;
	m_fDeltaZ = m_fHeight / m_nL;

	double TotalQSum = 0;
	for (int i = 0; i < m_cQztField.GetWidth(); i++)
	{
		for (int j = 0; j < m_cQztField.GetHeight(); j++)
		{
			TotalQSum+= m_cQztField.Index(i,j);
		}
	}

	m_cQztGridField.ReSize(m_nN, m_nM);
	// формируем сетку теплового потока 
	int PixelInOneStepX = (int)(m_fDeltaX / m_cQztField.GetPixelWeight());
	int PixelInOneStepY = (int)(m_fDeltaY / m_cQztField.GetPixelWeight());
	double SquereWidth = PixelInOneStepX * m_cQztField.GetPixelWeight();
	double SquereHeight = PixelInOneStepY * m_cQztField.GetPixelWeight();
	double SquereS = SquereWidth * SquereHeight;
	double CommonS = m_cQztField.GetWidth() * m_cQztField.GetHeight() * m_cQztField.GetPixelWeight() * m_cQztField.GetPixelWeight();
	double Sdiv = CommonS/SquereS;
	for (int i = 0; i < m_cQztGridField.GetWidth(); i++)
	{
		int square_i_begin = PixelInOneStepX*i; // начальный индекс в квадрате по i
		int square_i_end = PixelInOneStepX*(i+1) < m_cQztField.GetWidth() ? 
			PixelInOneStepX*(i+1) : m_cQztField.GetWidth(); // конечный индекс в квадрате по i

		for (int j = 0; j < m_cQztGridField.GetHeight(); j++)
		{
			int square_j_begin = PixelInOneStepY*j; // начальный индекс в квадрате по j
			// вычисление суммы для квадрата (i,j)
			int square_j_end = PixelInOneStepY*(j+1) < m_cQztField.GetHeight() ? 
				PixelInOneStepY*(j+1) : m_cQztField.GetHeight(); // конечный индекс в квадрате по j

			double tmp_acc = 0;
			for (int ii = square_i_begin; ii < square_i_end; ii++)
			{
				for (int jj = square_j_begin; jj < square_j_end; jj++)
				{
					tmp_acc += m_cQztField.Index(ii, jj);
				}
			}

			m_cQztGridField.Index(i, j) = (tmp_acc / TotalQSum) * Sdiv;
		}
	}
}

void TemperatureRender::Initialization(double T0Liq, double T0Sol, double SolProp, double DropTemperature)
{
	m_fDropTemperature = DropTemperature;
	// устанавливаем температуру на нулевом временном слое
	m_cMidtemperature_cache.clear();
	m_ctemperature_cache.ReSize(m_nN,m_nM,m_nL);

	if (SolProp < 0)
		SolProp = 0;
	if (SolProp > 1)
		SolProp = 1;

	int SolHeight = (int)(SolProp * m_nL);

	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)
			for (int k = 0; k < SolHeight; k++)
			{
				m_ctemperature_cache.Index(i,j,k) = T0Sol;
			}

	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)
			for (int k = SolHeight; k < m_nL; k++)
			{
				m_ctemperature_cache.Index(i,j,k) = T0Liq;
			}
	// учитываем столбец перегретого металла
	Droplets(&m_ctemperature_cache);
	// вычисляем среднюю температуру по слою
	double TemperatureSum = 0;
	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)
			for (int k = 0; k < m_nL; k++)
			{
				TemperatureSum += m_ctemperature_cache.Index(i,j,k);
			}
	m_cMidtemperature_cache.push_back(TemperatureSum / (m_nN * m_nM * m_nL));
}

void TemperatureRender::InitMetalParam(MetalParamStruct *Param)
{
	if (Param) 
		MetalInfo = *Param;
}

// возвращает false в случае прерывания
bool TemperatureRender::Next(int StepCount, TemperatureEvent pCalcEvent)
{
	// вызываем события говорящее о начале расчёта
	TemperatureWorkParam twp = {0,StepCount - 1,0,TemperatureWorkParam::WorkStart};
	if (pCalcEvent) pCalcEvent(&twp);

	twp.CurrentWorkEvent = TemperatureWorkParam::WorkStop;
	for (int i = 0; i < StepCount; i++)
	{
		// проверка флага об отмене расчёта
		if(StopProccess)
		{
			twp.Current = 0;
			twp.CurrentWorkEvent = TemperatureWorkParam::WorkStop;
			if (pCalcEvent) pCalcEvent(&twp);			
			return false;
		}

		// совешаем шаг на следующий временной слой
		MakeStep();
		
		// вызываем событие связанное с изменением состояния
		twp.Current = i;
		twp.CurrentWorkEvent = TemperatureWorkParam::WorkProcess;
		if (pCalcEvent) pCalcEvent(&twp);
	}

	m_fMinTemperature = m_ctemperature_cache.Index(0, 0, 0);
	m_fMaxTemperature = m_fMinTemperature;
	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)		
			for (int k = 0; k < m_nL; k++)
			{
				if (m_ctemperature_cache.Index(i, j, k) < m_fMinTemperature)
					m_fMinTemperature = m_ctemperature_cache.Index(i, j, k);
				if (m_ctemperature_cache.Index(i, j, k) > m_fMaxTemperature)
					m_fMaxTemperature = m_ctemperature_cache.Index(i, j, k);
			}

	// вызываем события знаменующее конец расчёт
	twp.Current = 0;
	twp.CurrentWorkEvent = TemperatureWorkParam::WorkStop;
	if (pCalcEvent) pCalcEvent(&twp);
	return true;
}

void TemperatureRender::MakeStep()
{
	double *T;
	double *Out;
	
	TemperatureField NewTimeLayer(m_nN,m_nM,m_nL);

	// определяем наибольшую сторону
	int MaxBuffersSize = m_nN > m_nM ? (m_nN > m_nL ? m_nN : m_nL) : (m_nM > m_nL ? m_nM : m_nL); 
	// выделяем память на временные буферы
	T = new double[MaxBuffersSize];
	Out = new double[MaxBuffersSize];
	double *Fo = new double[MaxBuffersSize];
	double *A = new double[MaxBuffersSize];
	double *C = new double[MaxBuffersSize];
	double *B = new double[MaxBuffersSize];
	double *D = new double[MaxBuffersSize];
	double *WSpeed = new double[MaxBuffersSize];
	// определяем среднюю скорость движения потока

	// проход вдоль x
	for (int j = 0; j < m_nM; j++)
		for (int k = 0; k < m_nL; k++)
		{		
			// определяем температуру плавления
			double MidLiqTemperature = (MetalInfo.TSol + MetalInfo.TLiq) / 2;

			double CurrentZ = m_fDeltaZ*k;

			// вычисляем число пекле
			// double Pe =  W0 * m_fLength / (GetLambda(MidTemperature)/(GetThermCap(MidTemperature)*GetDensity(MidTemperature)));

			for (int i = 0; i < m_nN; i++)
			{
				T[i] = m_ctemperature_cache.Index(i,j,k);
				Fo[i] = GetLambda(T[i])/(GetThermCap(T[i]) * GetDensity(T[i])) * m_fDeltaTaw / (m_fDeltaX*m_fDeltaX);

				if (T[i] > MidLiqTemperature)
				{
					double Density_for_w0 = GetDensity(T[i]);
					double W0 = 3 * MetalInfo.G * (1000.0f / 3600.0f) / (2 * Density_for_w0 * m_fWidth * m_fHeight);
					double CurrentX = m_fDeltaX*i;
					double Sigma = 4.64f * sqrt(CurrentX) / sqrt(W0 / (MetalInfo.Viscosity * 1e-6));
					double CurrentZDivSigma = (Sigma != 0 ? CurrentZ/Sigma : 0);
					double Wx = W0*(1.5*CurrentZDivSigma-0.5*CurrentZDivSigma*CurrentZDivSigma*CurrentZDivSigma);
					WSpeed[i] = m_fDeltaTaw * Wx / m_fDeltaX;
				}
				else
				{
					WSpeed[i] = 0;
				}
			}
			
			SweepMove(m_nN, WSpeed, Fo, A, B, C, D, T, Out);
			double Qxl = GetQStreamByTemperature(&m_fQxl,m_ctemperature_cache.Index(0,j,k));
			double Qxr = GetQStreamByTemperature(&m_fQxr,m_ctemperature_cache.Index(m_nN - 1,j,k));

			Out[0] = Out[1] - Qxl * m_fDeltaX / GetLambda(T[0]);
			Out[m_nN - 1] = Out[m_nN - 2] - Qxr * m_fDeltaX / GetLambda(T[m_nN - 1]);

			for (int i = 0; i < m_nN; i++)
				NewTimeLayer.Index(i,j,k) = Out[i];
		}
	// проход вдоль y
	for (int i = 0; i < m_nN; i++)
		for (int k = 0; k < m_nL; k++)
		{
			for (int j = 0; j < m_nM; j++)
			{
				T[j] = NewTimeLayer.Index(i,j,k);
				Fo[j] = GetLambda(T[j])/(GetThermCap(T[j]) * GetDensity(T[j])) * m_fDeltaTaw / (m_fDeltaY*m_fDeltaY);
			}
			
			Sweep(m_nM, Fo, A, B, C, D, T, Out);

			double Qyl = GetQStreamByTemperature(&m_fQyl,m_ctemperature_cache.Index(i,0,k));
			double Qyr = GetQStreamByTemperature(&m_fQyr,m_ctemperature_cache.Index(i,m_nM - 1,k));
			
			Out[0] = Out[1] - Qyl * m_fDeltaY / GetLambda(T[0]);
			Out[m_nM - 1] = Out[m_nM - 2] - Qyr * m_fDeltaY / GetLambda(T[m_nM - 1]);
			
			for (int j = 0; j < m_nM; j++)
				NewTimeLayer.Index(i,j,k) = Out[j];
		}
	// проход вдоль z
	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)
		{		
			for (int k = 0; k < m_nL; k++)
			{
				T[k] = NewTimeLayer.Index(i,j,k);
				Fo[k] = GetLambda(T[k])/(GetThermCap(T[k]) * GetDensity(T[k])) * m_fDeltaTaw / (m_fDeltaZ*m_fDeltaZ);
			}
			
			Sweep(m_nL, Fo, A, B, C, D, T, Out);

			double Qzb = GetQStreamByTemperature(&m_fQzb,m_ctemperature_cache.Index(i,j,0));
			// 10^7
			Out[0] = Out[1] - Qzb * m_fDeltaZ / GetLambda(T[0]);
			
			// температура поверхности
			double TopTemperature = m_ctemperature_cache.Index(i,j,m_nL - 1) / 100.0;
			double EnviromentTemperature = MetalInfo.EnvironmentTemperature / 100.0;
			// потери тепла на излучение
			double QRay = 5.67 * // постоянная Стефана-Больцмана 
				          MetalInfo.Blackness * (TopTemperature*TopTemperature*TopTemperature*TopTemperature
						                       - EnviromentTemperature*EnviromentTemperature*EnviromentTemperature*EnviromentTemperature);
#ifdef _DEBUG
			double QFromEGun = m_fQzt * m_cQztGridField.Index(i, j);
			double CurrentLambda = GetLambda(T[m_nL - 1]);
			double OutLm2 = Out[m_nL - 2];
			double OutLm1 = OutLm2 + (QFromEGun - QRay) * m_fDeltaZ / CurrentLambda;

#endif

			Out[m_nL - 1] = Out[m_nL - 2] + (
											m_fQzt * m_cQztGridField.Index(i, j) - 
											QRay // учёт потерь на излучение 
											) 
											* m_fDeltaZ / GetLambda(T[m_nL - 1]);

			for (int k = 0; k < m_nL; k++)
				NewTimeLayer.Index(i,j,k) = Out[k];
			
		}
	delete [] T;
	delete [] Out;
	delete [] A;
	delete [] C;
	delete [] B;
	delete [] D;
	delete [] WSpeed;

	Droplets(&NewTimeLayer);

	double TemperatureSum = 0;
	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)		
			for (int k = 0; k < m_nL; k++)
			{
				TemperatureSum += NewTimeLayer.Index(i,j,k);
			}

	m_cMidtemperature_cache.push_back(TemperatureSum / (m_nN * m_nM * m_nL));

	// добавляем временной слой в кеш
	m_ctemperature_cache = NewTimeLayer;
}

bool TemperatureRender::DrawSceneByXY(ImageBuffer *ib, int ZLayer)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;
	int BottomIndent = 50;

	double ZoomX = (double)(Width - Indent*2) / m_fLength; // ед/пикс
	double ZoomY = (double)(Height - Indent - BottomIndent) / m_fWidth; // ед/пикс

	// выбираем меньший зум
	if (ZoomX > ZoomY)
		ZoomX = ZoomY;
	else
		ZoomY = ZoomX;

	int ZoomedWidth = (int)(ZoomX * m_fLength);
	int ZoomedHeight = (int)(ZoomY * m_fWidth);

	ImageBuffer RenderBuffer;
	if (!RenderBuffer.CreateCompatible(*ib,m_nN,m_nM))
		return false;

	double TemperatureDelta = m_fMaxTemperature - m_fMinTemperature;
	for (int i = 0; i < m_nN; i++)
		for (int j = 0; j < m_nM; j++)
		{
			RenderBuffer.SetPixel(i,j,ColorByValue((m_ctemperature_cache.Index(i, j, ZLayer) - m_fMinTemperature)/TemperatureDelta));
		}

	int XCopyPos = ib->GetWidth() / 2 - ZoomedWidth / 2;
	int YCopyPos = (Indent + ib->GetHeight() - BottomIndent) / 2 - ZoomedHeight / 2;
	ib->Blacken();
	if (!StretchBlt(ib->GetDC(),XCopyPos,YCopyPos,ZoomedWidth,ZoomedHeight,RenderBuffer.GetDC(),0,0,m_nN,m_nM,SRCCOPY))
		return false;
	
	TemperatureAxesInfo tai = 
	{
		XCopyPos,
		YCopyPos,
		ZoomedWidth,
		ZoomedHeight,
		Indent,
		BottomIndent
	};

	DrawAxes(ib,&tai,TEXT("y"),TEXT("x"));

	return true;
}

bool TemperatureRender::DrawScenePhaseByXY(ImageBuffer *ib, int ZLayer)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;
	int BottomIndent = 50;

	double ZoomX = (double)(Width - Indent*2) / m_fLength; // ед/пикс
	double ZoomY = (double)(Height - Indent - BottomIndent) / m_fWidth; // ед/пикс

	// выбираем меньший зум
	if (ZoomX > ZoomY)
		ZoomX = ZoomY;
	else
		ZoomY = ZoomX;

	int ZoomedWidth = (int)(ZoomX * m_fLength);
	int ZoomedHeight = (int)(ZoomY * m_fWidth);

	ImageBuffer RenderBuffer;
	if (!RenderBuffer.CreateCompatible(*ib,m_nN,m_nM))
		return false;

	double TemperatureDelta = m_fMaxTemperature - m_fMinTemperature;
	double TemperatureMelta = (MetalInfo.TLiq + MetalInfo.TSol) / 2;
	for (int i = 0; i < m_nN; i++)
		for (int j = m_nM - 1, cache_j = 0; j >= 0; j--, cache_j++)
		{
			COLORREF Color;
			if (m_ctemperature_cache.Index(i, cache_j, ZLayer) <= TemperatureMelta)
				Color = RGB(0x00,0xff,0x00);
			else Color = RGB(0xff,0x00,0x00);
			RenderBuffer.SetPixel(i,j,Color);
		}

	int XCopyPos = ib->GetWidth() / 2 - ZoomedWidth / 2;
	int YCopyPos = (Indent + ib->GetHeight() - BottomIndent) / 2 - ZoomedHeight / 2;
	ib->Blacken();
	if (!StretchBlt(ib->GetDC(),XCopyPos,YCopyPos,ZoomedWidth,ZoomedHeight,RenderBuffer.GetDC(),0,0,m_nN,m_nM,SRCCOPY))
		return false;
	
	TemperatureAxesInfo tai = 
	{
		XCopyPos,
		YCopyPos,
		ZoomedWidth,
		ZoomedHeight,
		Indent,
		BottomIndent
	};
	DrawAxes(ib,&tai,TEXT("y"),TEXT("x"));

	DrawLegendForPhase(ib);

	return true;
}

bool TemperatureRender::DrawScenePhaseByXZ(ImageBuffer *ib, int YLayer)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;
	int BottomIndent = 50;

	double ZoomX = (double)(Width - Indent*2) / m_fLength; // ед/пикс
	double ZoomZ = (double)(Height - Indent - BottomIndent) / m_fHeight; // ед/пикс

	// выбираем меньший зум
	if (ZoomX > ZoomZ)
		ZoomX = ZoomZ;
	else
		ZoomZ = ZoomX;

	int ZoomedWidth = (int)(ZoomX * m_fLength);
	int ZoomedHeight = (int)(ZoomZ * m_fHeight);


	ImageBuffer RenderBuffer;
	if (!RenderBuffer.CreateCompatible(*ib,m_nN,m_nL))
		return false;

	double TemperatureDelta = m_fMaxTemperature - m_fMinTemperature;
	double TemperatureMelta = (MetalInfo.TLiq + MetalInfo.TSol) / 2;
	for (int i = 0; i < m_nN; i++)
		for (int k = m_nL - 1, cache_k = 0; k >= 0; k--, cache_k++)
		{
			COLORREF Color;
			if (m_ctemperature_cache.Index(i, YLayer, cache_k) <= TemperatureMelta)
				Color = RGB(0x00,0xff,0x00);
			else Color = RGB(0xff,0x00,0x00);
			RenderBuffer.SetPixel(i,k,Color);
		}

	int XCopyPos = ib->GetWidth() / 2 - ZoomedWidth / 2;
	int YCopyPos = (Indent + ib->GetHeight() - BottomIndent) / 2 - ZoomedHeight / 2;
	ib->Blacken();
	if (!StretchBlt(ib->GetDC(),XCopyPos,YCopyPos,ZoomedWidth,ZoomedHeight,RenderBuffer.GetDC(),0,0,m_nN,m_nL,SRCCOPY))
		return false;

	TemperatureAxesInfo tai = 
	{
		XCopyPos,
		YCopyPos,
		ZoomedWidth,
		ZoomedHeight,
		Indent,
		BottomIndent
	};
	DrawAxes(ib,&tai,TEXT("z"),TEXT("x"));

	DrawLegendForPhase(ib);

	return true;
}

bool TemperatureRender::DrawScenePhaseByYZ(ImageBuffer *ib, int XLayer)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;
	int BottomIndent = 50;

	double ZoomY = (double)(Width - Indent*2) / m_fWidth; // ед/пикс
	double ZoomZ = (double)(Height - Indent*2) / m_fHeight; // ед/пикс

	// выбираем меньший зум
	if (ZoomY > ZoomZ)
		ZoomY = ZoomZ;
	else
		ZoomZ = ZoomY;

	int ZoomedWidth = (int)(ZoomY * m_fWidth);
	int ZoomedHeight = (int)(ZoomZ * m_fHeight);

	ImageBuffer RenderBuffer;
	if (!RenderBuffer.CreateCompatible(*ib,m_nM,m_nL))
		return false;

	double TemperatureDelta = m_fMaxTemperature - m_fMinTemperature;
	double TemperatureMelta = (MetalInfo.TLiq + MetalInfo.TSol) / 2;
	for (int j = 0; j < m_nM; j++)
		for (int k = m_nL - 1, cache_k = 0; k >= 0; k--, cache_k++)
		{
			COLORREF Color;
			if (m_ctemperature_cache.Index(XLayer, j, cache_k) <= TemperatureMelta)
				Color = RGB(0x00,0xff,0x00);
			else Color = RGB(0xff,0x00,0x00);
			RenderBuffer.SetPixel(j,k,Color);
		}

	int XCopyPos = ib->GetWidth() / 2 - ZoomedWidth / 2;
	int YCopyPos = (Indent + ib->GetHeight() - BottomIndent) / 2 - ZoomedHeight / 2;
	ib->Blacken();
	if (!StretchBlt(ib->GetDC(),XCopyPos,YCopyPos,ZoomedWidth,ZoomedHeight,RenderBuffer.GetDC(),0,0,m_nM,m_nL,SRCCOPY))
		return false;

	TemperatureAxesInfo tai = 
	{
		XCopyPos,
		YCopyPos,
		ZoomedWidth,
		ZoomedHeight,
		Indent,
		BottomIndent
	};
	DrawAxes(ib,&tai,TEXT("z"),TEXT("y"));

	DrawLegendForPhase(ib);

	return true;
}

void TemperatureRender::DrawLegendForPhase(ImageBuffer *ib)
{
	gstring Line1(TEXT("жидкая фаза"));
	gstring Line2(TEXT("твердая фаза"));

	IB_Font LegendFont(NULL,14);

	ib->SetFont(&LegendFont);
	
	SIZE Line1TextSize;
	if (!GetTextExtentPoint32(ib->GetDC(),Line1.c_str(),Line1.size(),&Line1TextSize))
	{
		// не рисуем в таком случае
		return;
	}

	SIZE Line2TextSize;
	if (!GetTextExtentPoint32(ib->GetDC(),Line2.c_str(),Line2.size(),&Line2TextSize))
	{
		// не рисуем в таком случае
		return;
	}
	
	int LagendIndent = 5;
	int LegendTextIndent = 5;
	int SqureSize = 10; // размер цветного кубика
	int LegendWidth = (Line1TextSize.cx > Line2TextSize.cx ? Line1TextSize.cx : Line2TextSize.cx) + 3 * LegendTextIndent + SqureSize;
	int LegendHeight = Line1TextSize.cy + Line2TextSize.cy + 3 * LegendTextIndent;
	RECT LegendRect = {
		ib->GetWidth() - LegendWidth - LagendIndent,
		LagendIndent,
		ib->GetWidth() - LagendIndent,
		LagendIndent + LegendHeight
	};

	// наносим облако легенды
	IB_Brush Brush(RGB(0xff,0xff,0xff));
	ib->FillRect(&LegendRect, &Brush);

	// наносим первый цвет
	{
		IB_Brush Line1Brush(RGB(0xff,0x00,0x00));
		int CenterColorSqureByY = LegendHeight / 3;
		RECT Line1ColorRect = {
			LegendRect.left + LegendTextIndent,
			LegendRect.top + CenterColorSqureByY - (SqureSize >> 1),
			LegendRect.left + LegendTextIndent + SqureSize,
			LegendRect.top + CenterColorSqureByY + (SqureSize >> 1)
		};
		ib->FillRect(&Line1ColorRect, &Line1Brush);

		ib->SetTextColor(RGB(0x00,0x00,0x00));
		ib->DrawText(LegendRect.left + 2*LegendTextIndent + SqureSize, LegendRect.top + LegendTextIndent,Line1.c_str());
	}

	// наносим второй цвет
	{
		IB_Brush Line2Brush(RGB(0x00,0xff,0x00));
		int CenterColorSqureByY = 2 * LegendHeight / 3;
		RECT Line2ColorRect = {
			LegendRect.left + LegendTextIndent,
			LegendRect.top + CenterColorSqureByY - (SqureSize >> 1), 
			LegendRect.left + LegendTextIndent + SqureSize,
			LegendRect.top + CenterColorSqureByY + (SqureSize >> 1)
		};
		ib->FillRect(&Line2ColorRect, &Line2Brush);

		ib->SetTextColor(RGB(0x00,0x00,0x00));
		ib->DrawText(LegendRect.left + 2*LegendTextIndent + SqureSize, LegendRect.top + 2*LegendTextIndent + SqureSize,Line2.c_str());
	}
}

bool TemperatureRender::DrawSceneByXZ(ImageBuffer *ib, int YLayer)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;
	int BottomIndent = 50;

	double ZoomX = (double)(Width - Indent*2) / m_fLength; // ед/пикс
	double ZoomZ = (double)(Height - Indent - BottomIndent) / m_fHeight; // ед/пикс

	// выбираем меньший зум
	if (ZoomX > ZoomZ)
		ZoomX = ZoomZ;
	else
		ZoomZ = ZoomX;

	int ZoomedWidth = (int)(ZoomX * m_fLength);
	int ZoomedHeight = (int)(ZoomZ * m_fHeight);


	ImageBuffer RenderBuffer;
	if (!RenderBuffer.CreateCompatible(*ib,m_nN,m_nL))
		return false;

	double TemperatureDelta = m_fMaxTemperature - m_fMinTemperature;
	for (int i = 0; i < m_nN; i++)
		for (int k = m_nL - 1, cache_k = 0; k >= 0; k--, cache_k++)
		{
			RenderBuffer.SetPixel(i,k,ColorByValue((m_ctemperature_cache.Index(i, YLayer, cache_k) - m_fMinTemperature)/TemperatureDelta));
		}

	int XCopyPos = ib->GetWidth() / 2 - ZoomedWidth / 2;
	int YCopyPos = (Indent + ib->GetHeight() - BottomIndent) / 2 - ZoomedHeight / 2;
	ib->Blacken();
	if (!StretchBlt(ib->GetDC(),XCopyPos,YCopyPos,ZoomedWidth,ZoomedHeight,RenderBuffer.GetDC(),0,0,m_nN,m_nL,SRCCOPY))
		return false;

	TemperatureAxesInfo tai = 
	{
		XCopyPos,
		YCopyPos,
		ZoomedWidth,
		ZoomedHeight,
		Indent,
		BottomIndent
	};
	DrawAxes(ib,&tai,TEXT("z"),TEXT("x"));

	return true;
}

bool TemperatureRender::DrawSceneByYZ(ImageBuffer *ib, int XLayer)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;
	int BottomIndent = 50;

	double ZoomY = (double)(Width - Indent*2) / m_fWidth; // ед/пикс
	double ZoomZ = (double)(Height - Indent*2) / m_fHeight; // ед/пикс

	// выбираем меньший зум
	if (ZoomY > ZoomZ)
		ZoomY = ZoomZ;
	else
		ZoomZ = ZoomY;

	int ZoomedWidth = (int)(ZoomY * m_fWidth);
	int ZoomedHeight = (int)(ZoomZ * m_fHeight);

	ImageBuffer RenderBuffer;
	if (!RenderBuffer.CreateCompatible(*ib,m_nM,m_nL))
		return false;

	double TemperatureDelta = m_fMaxTemperature - m_fMinTemperature;
	for (int j = 0; j < m_nM; j++)
		for (int k = m_nL - 1, cache_k = 0; k >= 0; k--, cache_k++)
		{
			RenderBuffer.SetPixel(j,k,ColorByValue((m_ctemperature_cache.Index(XLayer, j, cache_k) - m_fMinTemperature)/TemperatureDelta));
		}

	int XCopyPos = ib->GetWidth() / 2 - ZoomedWidth / 2;
	int YCopyPos = (Indent + ib->GetHeight() - BottomIndent) / 2 - ZoomedHeight / 2;
	ib->Blacken();
	if (!StretchBlt(ib->GetDC(),XCopyPos,YCopyPos,ZoomedWidth,ZoomedHeight,RenderBuffer.GetDC(),0,0,m_nM,m_nL,SRCCOPY))
		return false;

	TemperatureAxesInfo tai = 
	{
		XCopyPos,
		YCopyPos,
		ZoomedWidth,
		ZoomedHeight,
		Indent,
		BottomIndent
	};
	DrawAxes(ib,&tai,TEXT("z"),TEXT("y"));

	return true;
}

bool TemperatureRender::DrawSceneTemperatureLine(ImageBuffer *ib)
{
	int Width = ib->GetWidth();
	int Height = ib->GetHeight();
	int Indent = 28;

	if (m_cMidtemperature_cache.empty())
		return false;

	double MinTemperature = m_cMidtemperature_cache[0];
	double MaxTemperature = MinTemperature;
	for (int i = 1; i < (signed)m_cMidtemperature_cache.size(); i++)
	{
		if (m_cMidtemperature_cache[i] < MinTemperature)
			MinTemperature = m_cMidtemperature_cache[i];
		if (m_cMidtemperature_cache[i] > MaxTemperature)
			MaxTemperature = m_cMidtemperature_cache[i];
	}

	double ZoomX = (double)(Width - Indent*2) / m_cMidtemperature_cache.size(); // ед/пикс
	double ZoomY = (double)(Height - Indent*2) / (MaxTemperature - MinTemperature); // ед/пикс

	// окраска области в чёрный
	ib->Blacken();
	
	// нанесение сетки
	{
		int VerticalStep = 20;
		int HorizontalStep = 20;
		IB_Pen Pen(PS_DOT,1,RGB(195,195,195));
		ib->SetPen(&Pen);
		ib->SetBkMode(TRANSPARENT);
		// горизонтальные линии
		for (int i = Height - Indent, i_stop = Indent; i >= i_stop; i-= VerticalStep)
		{
			ib->DrawLine(Indent,i,Width - Indent,i);
		}
		// вертикальные линии
		for (int i = Indent, i_stop = Width - Indent; i < i_stop; i+= HorizontalStep)
		{
			ib->DrawLine(i, Indent, i, Height - Indent);
		}
	}

	{
		IB_Pen Pen(PS_SOLID,1,RGB(255,255,255));
		ib->SetPen(&Pen);
		int LastX = (int)Indent;
		int LastY = (int)(-(Indent + (m_cMidtemperature_cache[0] - MinTemperature)*ZoomY) + Height);
		m_cTemperature_points.clear();
		
		// добавялем самую первую точку
		{
			TemperaturePoint temp_point = {LastX, LastY, m_cMidtemperature_cache[0],0};
			m_cTemperature_points.push_back(temp_point);
		}

		for (int i = 1; i < (signed)m_cMidtemperature_cache.size(); i++)
		{
			int x = (int)(Indent + i*ZoomX);
			int y = (int)(-(Indent + (m_cMidtemperature_cache[i] - MinTemperature)*ZoomY) + Height);
			if (x != LastX || y != LastY)
			{
				ib->DrawLine(LastX,LastY,x,y);
				
				for (int j = LastX + 1; j <= x; j++ )
				{
					TemperaturePoint temp_point = {
						j,
						(j - LastX)*(y - LastY)/(x - LastX) + LastY,
						m_cMidtemperature_cache[i],i*m_fDeltaTaw};

					m_cTemperature_points.push_back(temp_point);
				}
				LastX = x;
				LastY = y;
			}
		}
	}

	ib->SetFont(&TemperatureFont);
	ARGBColor AxesColor = RGB(255,255,255);
	IB_Pen Pen(PS_SOLID,1,AxesColor);
	ib->SetPen(&Pen);
	int HalfIndent = Indent >> 1;
	int EndLinePointX = Indent;
	int EndLinePointY = HalfIndent;
	ib->DrawLine(EndLinePointX, Height - Indent,EndLinePointX,EndLinePointY);

	// отрисока стрелочки
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 2, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 2, AxesColor);
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 3, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 3, AxesColor);
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 4, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 4, AxesColor);
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 5, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 5, AxesColor);
	ib->SetPixel(EndLinePointX - 2, EndLinePointY + 5, AxesColor);
	ib->SetPixel(EndLinePointX + 2, EndLinePointY + 5, AxesColor);

	ib->SetTextColor(AxesColor);
	ib->SetBkMode(TRANSPARENT);
	ib->DrawText(EndLinePointX + HalfIndent/2, EndLinePointY - HalfIndent/2, TEXT("T, \x00B0K"));

	EndLinePointX = Width - HalfIndent;
	EndLinePointY = Height - Indent;
	ib->DrawLine(Indent,EndLinePointY,EndLinePointX,EndLinePointY);


	// отрисовка стрелочки
	ib->SetPixel(EndLinePointX - 2, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 2, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 3, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 3, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 4, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 4, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY - 2, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY + 2, AxesColor);

	ib->DrawText(EndLinePointX-10,EndLinePointY + 2, TEXT("\x03C4, c"));

	return true;
}

#ifdef _DEBUG

bool TemperatureRender::WriteTemperatureDamp(LPCTSTR FileName, int ZLayer)
{
	std::ofstream File;

	File.open(FileName,std::ios::out | std::ios::trunc);

	if (!File.is_open())
		return false;
	
	for (int i = 0; i < m_ctemperature_cache.GetLength(); i++)
	{
		for (int j = 0; j < m_ctemperature_cache.GetWidth(); j++)
		{
			File << m_ctemperature_cache.Index(i,j,0) << " ";
		}
		File << std::endl;
	}

	File.close();
	return true;
}

#endif

void TemperatureRender::Sweep(int N, double *Fo, double *A, double *B, double *C, double *D, double *T, double *Out)
{
	A[1] = 0;
	B[1] = 1 + 2 * Fo[1];
	C[1] = Fo[1];
	D[1] = -T[1] - Fo[1] * T[0];

	A[N-2] = Fo[N-2];
	B[N-2] = 1 + 2 * Fo[N-2];
	C[N-2] = 0;
	D[N-2] = -T[N-2] - Fo[N-2] * T[N-1];
	
	for (int i = 2; i <= N - 3; i++)
	{
		A[i] = Fo[i];
		B[i] = 1 + 2 * Fo[i];
		C[i] = Fo[i];
		D[i] = -T[i];
	}

	for (int i = 2; i <= N - 2; i++)
	{
		B[i] = - A[i] * C[i-1]/B[i-1] + B[i];
		D[i] = A[i] * D[i-1]/B[i-1] + D[i];
	}

	Out[N-2] = -D[N-2]/B[N-2];

	for (int i = N - 3; i >= 1; i--)
	{
		Out[i] = -(D[i] - Out[i+1] * C[i])/B[i];
	}
}

void TemperatureRender::SweepMove(int N, double *SpeedC, double *Fo, double *A, double *B, double *C, double *D, double *T, double *Out)
{
#define REVERS_MOVE 0

	A[1] = 0;
#if REVERS_MOVE != 1
	B[1] = 1 + 2 * Fo[1] - SpeedC[1];
#else
	B[1] = 1 + 2 * Fo[1] + SpeedC[1];
#endif

#if REVERS_MOVE != 1
	C[1] = Fo[1];
#else
	C[1] = Fo[1] + SpeedC[1];
#endif
	
	D[1] = -T[1] - Fo[1] * T[0];

#if REVERS_MOVE != 1
	A[N-2] = Fo[N-2] - SpeedC[N-2];
#else
	A[N-2] = Fo[N-2];
#endif

	
#if REVERS_MOVE != 1
	B[N-2] = 1 + 2 * Fo[N-2] - SpeedC[N-2];
#else
	B[N-2] = 1 + 2 * Fo[N-2] + SpeedC[N-2];
#endif

#if REVERS_MOVE != 1
	C[N-2] = Fo[N-2];
#else
	C[N-2] = Fo[N-2] + SpeedC[N-2];
#endif

	D[N-2] = -T[N-2] - Fo[N-2] * T[N-1];
	
	for (int i = 2; i <= N - 3; i++)
	{
		
#if REVERS_MOVE != 1
		A[i] = Fo[i] - SpeedC[i];
		B[i] = 1 + 2 * Fo[i] - SpeedC[i];
		C[i] = Fo[i];
#else
		A[i] = Fo[i];
		B[i] = 1 + 2 * Fo[i] + SpeedC[i];
		C[i] = Fo[i] + SpeedC[i];
#endif
		D[i] = -T[i];
	}

	for (int i = 2; i <= N - 2; i++)
	{
		B[i] = - A[i] * C[i-1]/B[i-1] + B[i];
		D[i] = A[i] * D[i-1]/B[i-1] + D[i];
	}

	Out[N-2] = -D[N-2]/B[N-2];

	for (int i = N - 3; i >= 1; i--)
	{
		Out[i] = -(D[i] - Out[i+1] * C[i])/B[i];
	}
}

double TemperatureRender::GetDoubleMod(double dividend, double divider)
{
	if (divider == 0.0f) return 0;

	bool IsNegative = (dividend / divider) < 0;

	if (dividend < 0) dividend*= -1.0f;
	if (divider < 0) divider*= -1.0f;

	while (dividend >= divider)
		dividend -= divider;

	return IsNegative ? - dividend : dividend;
}

int TemperatureRender::GetDoubleDiv(double dividend, double divider)
{
	if (divider == 0.0f) return 0;

	bool IsNegative = (dividend / divider) < 0;

	if (dividend < 0) dividend*= -1.0f;
	if (divider < 0) divider*= -1.0f;

	int Div = 0;
	while (dividend >= divider)
	{
		dividend -= divider;
		Div++;
	}

	return IsNegative ? - Div : Div;
}

COLORREF TemperatureRender::ColorByValue(double value)
{
	int Index = (int)(value*RainbowMasSize-1);
	
	if (Index < 0)
		Index = 0;

	if (Index > (RainbowMasSize - 1))
		Index = (RainbowMasSize - 1);

	return RainbowMas[Index];
}

int TemperatureRender::GetN() const
{
	return m_nN;
}

int TemperatureRender::GetM() const
{
	return m_nM;
}

int TemperatureRender::GetL() const
{
	return m_nL;
}

void TemperatureRender::SetStopEvent(bool State)
{
	StopProccess = State;
}

int TemperatureRender::GetActualN() const
{
	int TempN = m_cQztField.GetWidth() / 4;
	if (TempN < 3) TempN = 3;
	return TempN;
}

int TemperatureRender::GetActualM() const
{
	int TempM = m_cQztField.GetHeight() / 4;
	if (TempM < 3) TempM = 3;
	return TempM;
}

int TemperatureRender::GetActualL() const
{
	int TempN = GetActualN(),
		TempM = GetActualM();

	int TempL;
	if (TempN < TempM)
	{
		double deltaX = m_fLength/TempN;
		TempL = (int)(m_fHeight / deltaX);
	}
	else
	{
		double deltaY = m_fWidth/TempM;
		TempL = (int)(m_fHeight / deltaY);
	}

	if (TempL < 3) TempL = 3;
	return TempL;
}

void TemperatureRender::DrawAxes(ImageBuffer *ib, TemperatureAxesInfo *tai, ImageBuffer_cstring vertAx,ImageBuffer_cstring horzAx)
{
	ARGBColor AxesColor = RGB(255,255,255);
	IB_Pen Pen(PS_SOLID,1,AxesColor);
	ib->SetPen(&Pen);
	int HalfIndent = tai->Indent >> 1;
	int EndLinePointX = tai->xPos - HalfIndent;
	int EndLinePointY = tai->yPos - HalfIndent;
	ib->DrawLine(EndLinePointX,tai->yPos + tai->ZoomedHeight + HalfIndent,EndLinePointX,EndLinePointY);

	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 2, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 2, AxesColor);
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 3, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 3, AxesColor);
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 4, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 4, AxesColor);
	ib->SetPixel(EndLinePointX - 1, EndLinePointY + 5, AxesColor);
	ib->SetPixel(EndLinePointX + 1, EndLinePointY + 5, AxesColor);
	ib->SetPixel(EndLinePointX - 2, EndLinePointY + 5, AxesColor);
	ib->SetPixel(EndLinePointX + 2, EndLinePointY + 5, AxesColor);

	ib->SetTextColor(AxesColor);
	ib->SetBkMode(TRANSPARENT);
	ib->DrawText(EndLinePointX - HalfIndent, EndLinePointY, vertAx);

	EndLinePointX = tai->xPos + tai->ZoomedWidth + HalfIndent;
	EndLinePointY = tai->yPos + tai->ZoomedHeight + HalfIndent;
	ib->DrawLine(tai->xPos - HalfIndent,EndLinePointY,EndLinePointX,EndLinePointY);

	ib->SetPixel(EndLinePointX - 2, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 2, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 3, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 3, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 4, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 4, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY - 1, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY + 1, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY - 2, AxesColor);
	ib->SetPixel(EndLinePointX - 5, EndLinePointY + 2, AxesColor);
	ib->DrawText(EndLinePointX+2,EndLinePointY - (HalfIndent >> 1), horzAx);
	for (int i = 0, i_stop = ib->GetWidth(),
		j = ib->GetHeight() - 17,
		j_stop = ib->GetHeight() - 1; i < i_stop; i++)
	{
		IB_Pen Pen(PS_SOLID,1,ColorByValue((double)i/i_stop));
		ib->SetPen(&Pen);
		ib->DrawLine(i,j,i,j_stop);
	}

	ib->SetFont(&TemperatureFont);
	RClasses::RString str1 = RClasses::RConverter::ToString(m_fMinTemperature) + TEXT(" K");
	RClasses::RString str2 = RClasses::RConverter::ToString(m_fMaxTemperature) + TEXT(" K");
	RECT Rect1 = {5,ib->GetHeight() - 17,ib->GetHeight()/2,ib->GetHeight() - 1};
	DrawText(ib->GetDC(),str1.c_str(),-1,&Rect1,DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	RECT Rect2 = {ib->GetWidth() / 2,ib->GetHeight() - 17,ib->GetWidth() - 5,ib->GetHeight() - 1};
	DrawText(ib->GetDC(),str2.c_str(),-1,&Rect2,DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
}

double TemperatureRender::GetPsi(double T)
{
	return (MetalInfo.TLiq - T) / (MetalInfo.TLiq - MetalInfo.TSol);
}

double TemperatureRender::GetLambda(double T)
{
	static double ResultL;
	static double ResultH;
	static Variables Vars(1);
	static ExpressionError ExprErr;

	if (T > MetalInfo.TLiq)
	{
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.LambdaL.GetValue(ResultL,Vars,ExprErr))
			ResultL = 0;
		return ResultL;
	}
	else if (T > MetalInfo.TSol)
	{
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.LambdaL.GetValue(ResultL,Vars,ExprErr))
			ResultL = 0;
		
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.LambdaH.GetValue(ResultH,Vars,ExprErr))
			ResultH = 0;

		return ResultH*GetPsi(T) + ResultL*(1 - GetPsi(T));
	}

	Vars[0] = VarItem(PARSER_TEXT("t"),T);
	if (!MetalInfo.LambdaH.GetValue(ResultH,Vars,ExprErr))
		ResultH = 0;
	return ResultH;
}

double TemperatureRender::GetDensity(double T)
{
	static double ResultL;
	static double ResultH;
	static Variables Vars(1);
	static ExpressionError ExprErr;
	
	if (T > MetalInfo.TLiq)
	{
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.DensityL.GetValue(ResultL,Vars,ExprErr))
			ResultL = 0;
		return ResultL;
	}
	else if (T > MetalInfo.TSol)
	{
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.DensityL.GetValue(ResultL,Vars,ExprErr))
			ResultL = 0;
		
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.DensityH.GetValue(ResultH,Vars,ExprErr))
			ResultH = 0;

		return ResultH*GetPsi(T) + ResultL*(1 - GetPsi(T));
	}

	Vars[0] = VarItem(PARSER_TEXT("t"),T);
	if (!MetalInfo.DensityH.GetValue(ResultH,Vars,ExprErr))
		ResultH = 0;
	return ResultH;
}

double TemperatureRender::GetThermCap(double T)
{
	static double ResultL;
	static double ResultH;
	static Variables Vars(1);
	static ExpressionError ExprErr;

	if (T > MetalInfo.TLiq)
	{
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.ThermalCapL.GetValue(ResultL,Vars,ExprErr))
			ResultL = 0;
		return ResultL;
	}
	else if (T > MetalInfo.TSol)
	{
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.ThermalCapL.GetValue(ResultL,Vars,ExprErr))
			ResultL = 0;
		
		Vars[0] = VarItem(PARSER_TEXT("t"),T);
		if (!MetalInfo.ThermalCapH.GetValue(ResultH,Vars,ExprErr))
			ResultH = 0;

		return ResultH*GetPsi(T) + ResultL*(1 - GetPsi(T)) - MetalInfo.Fusion*(1/(MetalInfo.TLiq - MetalInfo.TSol));
	}

	Vars[0] = VarItem(PARSER_TEXT("t"),T);
	if (!MetalInfo.ThermalCapH.GetValue(ResultH,Vars,ExprErr))
		ResultH = 0;

	return ResultH;
}

double TemperatureRender::GetQStreamByTemperature(Expression *Expr, double Temperature)
{
	static double Result;
	static Variables Vars(1);
	static ExpressionError ExprErr;

	Vars[0] = VarItem(PARSER_TEXT("t"),Temperature);
	if (!Expr->GetValue(Result,Vars,ExprErr))
			Result = 0;
	return Result;
}

void TemperatureRender::Droplets(TemperatureField *field)
{
	if (!field) return;
	// температура перегретого металла
	double Temperature = m_fDropTemperature;
	// масса расплава ежемоментно поступающая в ПЕ
	double mas = MetalInfo.G * (1000.0f/3600.0f) * m_fDeltaTaw;
	// плотность перегретого металла
	double den = GetDensity(Temperature);
	// радиус перегретого столба
	double Rad = sqrt(mas/(3.1415*m_fHeight*den));

	// определяем сколько это клеток вдоль x
	int rad_by_step_in_x = (int)(Rad / m_fDeltaX + 0.5);
	// определяем сколько это клеток вдоль y
	int rad_by_step_in_y = (int)(Rad / m_fDeltaY + 0.5);

	int y_base_index = m_nM / 2;
	for (int i = -rad_by_step_in_x; i <= rad_by_step_in_x; i++)
	{
		int y_height = (int)floor(sqrt(1.0f-((double)(i*i))/(rad_by_step_in_x*rad_by_step_in_x)));

		if (y_height == 0)
			continue;

		// для каждого слоя по высоте
		for (int k = 0; k < m_nL; k++)
		{
			for (int j = 0; j < y_height; j++)
			{
				int 
					i_index = i + rad_by_step_in_x,
					j_index1 = y_base_index + j,
					j_index2 = y_base_index - j;

				if (i_index > 0 && i_index < m_nN)
				{
					if (j_index1 > 0 && j_index1 < m_nM)
						field->Index(i_index,j_index1,k) = Temperature;
					if (j_index2 > 0 && j_index2 < m_nM)
						field->Index(i_index,j_index2,k) = Temperature;
				}
			}
		}
	}
}

void TemperatureRender::CopyXYLayer(gstring &Buffer, int CurrentLayer)
{
	Buffer.clear();
	for (int i = 0; i < m_nN; i++)
	{
		for (int j = 0; j < m_nM; j++)
		{
			Buffer += RClasses::RConverter::ToString(m_ctemperature_cache.Index(i,j,CurrentLayer)) + TEXT("\t");
		}
		Buffer += TEXT("\n");
	}
}

void TemperatureRender::CopyXZLayer(gstring &Buffer, int CurrentLayer)
{
	Buffer.clear();
	for (int i = 0; i < m_nN; i++)
	{
		for (int k = m_nL - 1, cache_k = 0; k >= 0; k--, cache_k++)
		{
			Buffer += RClasses::RConverter::ToString(m_ctemperature_cache.Index(i,CurrentLayer,k)) + TEXT("\t");
		}
		Buffer += TEXT("\n");
	}
}

void TemperatureRender::CopyYZLayer(gstring &Buffer, int CurrentLayer)
{
	Buffer.clear();
	for (int j = 0; j < m_nM; j++)
	{
		for (int k = m_nL - 1, cache_k = 0; k >= 0; k--, cache_k++)
		{
			Buffer += RClasses::RConverter::ToString(m_ctemperature_cache.Index(CurrentLayer,j,k)) + TEXT("\t");
		}
		Buffer += TEXT("\n");
	}
}

double TemperatureRender::GetLength() const
{
	return m_fLength;
}
double TemperatureRender::GetWidth() const
{
	return m_fWidth;
}

double TemperatureRender::GetHeight() const
{
	return m_fHeight;
}

double TemperatureRender::GetDeltaX() const
{
	return m_fDeltaX;
}

double TemperatureRender::GetDeltaY() const
{
	return m_fDeltaY;
}

double TemperatureRender::GetDeltaZ() const
{
	return m_fDeltaZ;
}