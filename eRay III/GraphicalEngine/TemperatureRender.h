#ifndef _TEMPERATURERENDER_H_
#define _TEMPERATURERENDER_H_

#include "CloudField.h"
#include "TemperatureField.h"
#include "ImageBuffer.h"
#include "GraphicalTypes.h"

#include "..\ExpressionEngine\Expression.h"

#include <vector>

struct TemperaturePoint
{
	int X;
	int Y;
	double Temperature;
	double Time;
};

struct TemperatureAxesInfo 
{
	int xPos; 
	int yPos;
	int ZoomedWidth;
	int ZoomedHeight; 
	int Indent;
	int BottomIndent;
};

struct MetalParamStruct
{
	Expression LambdaL;
	Expression LambdaH;

	Expression DensityL;
	Expression DensityH;

	Expression ThermalCapL;
	Expression ThermalCapH;

	double TLiq;
	double TSol;

	double Blackness; // степень черноты 
	double Fusion; // теплота плавлени€
	double Viscosity; // кинематическа€ в€зкость среды

	double G; // скорость поступлени€ расплава
	double EnvironmentTemperature; // “емпература окружающей среды
};

/**
* \brief The structure of the argument for the method of the events occurring in the calculation of the temperature field.
*/
struct TemperatureWorkParam
{
	/**
	* \brief The minimum value of the step.
	*/
	int Min;

	/**
	* \brief The maximum value of the step.
	*/
	int Max;

	/**
	* \brief The current value of the step.
	*/
	int Current;

	/**
	* \breif This enumeration is used to describe what is happening at the time of the event.
	*/
	enum eWorkEvent
	{
		
		/**
		* \breif An event of this type occurs before the beginning of the direct calculation.
		*/
		WorkStart,

		/**
		* \breif This event occurs during the calculation, and is called after each step.
		*/
		WorkProcess,

		/**
		* \breif This event occurs after the end of the calculation.
		*/
		WorkStop
	};

	/**
	* \brief The current event.
	*/
	eWorkEvent CurrentWorkEvent;
};

typedef void (*TemperatureEvent)(TemperatureWorkParam*);

#define RainbowMasSize 65536

/**
* \brief This class performs a calculation and drawing of the temperature field of the object in the form of a parallelepiped.
*/
class TemperatureRender
{
	private:
		IB_Font TemperatureFont;
		/**
		* \brief The array stores the colors of the rainbow, from red to violet. Initialized in the constructor.
		*/
		COLORREF RainbowMas[RainbowMasSize];

		// тепловой поток со всех сторон
		Expression m_fQxl;
		Expression m_fQxr;
		Expression m_fQyl;
		Expression m_fQyr;
		Expression m_fQzb;
		CloudField <double> m_cQztField;
		CloudField <double> m_cQztGridField;
		double m_fQzt;
		
		/**
		* \brief Plate length in meters.
		*/
		double m_fLength;
		
		/**
		* \brief Plate width in meters.
		*/
		double m_fWidth;
		
		/**
		* \brief Plate height in meters.
		*/
		double m_fHeight;

		// ќписание сетки
		int m_nN;
		int m_nM;
		int m_nL;

		/**
		* \brief Time step.
		*/
		double m_fDeltaTaw;
		
		/**
		* \brief Step on the coordinate x.
		*/
		double m_fDeltaX;
		
		/**
		* \brief Step on the coordinate x.
		*/
		double m_fDeltaY;
		
		/**
		* \brief Step on the coordinate x.
		*/
		double m_fDeltaZ;

		/**
		* \brief Step on the coordinate x.
		*/
		MetalParamStruct MetalInfo;

		/**
		* \brief The temperature water-cooled walls.
		*/
		double m_fTMelting;

		// последний временной слой
		TemperatureField m_ctemperature_cache;
		// средн€€ температура на всех пройденных сло€х
		std::vector <double> m_cMidtemperature_cache;
		// ћинимальна€ температура на слое
		double m_fMinTemperature;
		// ћаксимальна€ температура на слое
		double m_fMaxTemperature;
		// “емпература капающего металла
		double m_fDropTemperature;

		// флаг говор€щий об отмене расчЄта
		bool StopProccess;

	// создание
	public:
		TemperatureRender();
		~TemperatureRender();

	// св€зб с внешним миром
	public:
		// инициализаци€ потоков
		void SetQStreams(Expression &Qxl, Expression &Qxr, Expression &Qyl, Expression &Qyr, Expression &Qzb);

		void SetBaseQStream(CloudField <double> &QztField, double Qzt);
		
		/*
		* \brief This method sets the dimensions of the modeled plate.
		*
		* \param L Plate length in meters.
		* \param W Plate width in meters.
		* \param H Plate height in meters.
		*/
		void SetDimensions(double L, double W, double H);


		/*
		* \brief This method sets the steps of the method of finite differences.
		*
		* \param DeltaTaw Step by time
		*/
		void ManualSteps(double DeltaTaw, int N, int M, int L);

		/**
		* \brief The method M performs the initialization parameters required for calculation.
		*
		* \param T0 The temperature at the first time layer in Celsiy.
		* \param a The thermal diffusivity of the object.
		* \param DeltaTaw Step by time in seconds.
		* \param DeltaX Step by X in meters.
		* \param DeltaY Step by Y in meters.
		* \param DeltaZ Step by Z in meters.
		*/
		void Initialization(double T0Liq,double T0Sol, double SolProp, double DropTemperature);


		void InitMetalParam(MetalParamStruct *Param);
		
		// расчЄт дл€ следующего временного сло€ и сохранение его в кеш
		bool Next(int StepCount, TemperatureEvent pCalcEvent);

		bool DrawSceneByXY(ImageBuffer *ib, int ZLayer);
		bool DrawSceneByXZ(ImageBuffer *ib, int YLayer);
		bool DrawSceneByYZ(ImageBuffer *ib, int XLayer);
		bool DrawSceneTemperatureLine(ImageBuffer *ib);

		bool DrawScenePhaseByXY(ImageBuffer *ib, int ZLayer);
		bool DrawScenePhaseByXZ(ImageBuffer *ib, int ZLayer);
		bool DrawScenePhaseByYZ(ImageBuffer *ib, int ZLayer);

		int GetN() const;
		int GetM() const;
		int GetL() const;

		double GetDeltaX() const;
		double GetDeltaY() const;
		double GetDeltaZ() const;

		int GetActualN() const;
		int GetActualM() const;
		int GetActualL() const;

		double GetLength() const;
		double GetWidth() const;
		double GetHeight() const;

		void SetStopEvent(bool State);

		// температурный график
		std::vector <TemperaturePoint> m_cTemperature_points;

		void CopyXYLayer(gstring &Buffer, int CurrentLayer);
		void CopyXZLayer(gstring &Buffer, int CurrentLayer);
		void CopyYZLayer(gstring &Buffer, int CurrentLayer);

#ifdef _DEBUG
		bool WriteTemperatureDamp(LPCTSTR FileName, int ZLayer);
#endif

	// внутренние методы
	protected:
		void MakeStep();
		
		double GetQStreamByTemperature(Expression *Expr, double Temperature);
		/**
		* \brief This method solves the linear algebraic equation in the form of a tridiagonal matrix using the sweep method.
		*
		* \param Fo The F number.
		* \param T A column of free terms.
		* \param Out The column which is the solution.
		*/
		void Sweep(int N, double *Fo, double *A, double *B, double *C, double *D, double *T, double *Out);

		/**
		* \brief ћодификаци€ метода прогонки в котором учитываетс€ движение среды вдоль X
		*/
		void SweepMove(int N, double *SpeedC, double *Fo, double *A, double *B, double *C, double *D, double *T, double *Out);

		/**
		* \brief Method GetDoubleMod returns the remainder after dividing one number by another.
		*
		* \param dividend Dividend number.
		* \param divider Divider number.
		* 
		* \return The remainder after dividing.
		*/
		double GetDoubleMod(double dividend, double divider);

		/**
		* \brief Method GetDoubleDiv performs integer division of one number by another.
		*
		* \param dividend Dividend number.
		* \param divider Divider number.
		* 
		* \return An integer particular.
		*/
		int GetDoubleDiv(double dividend, double divider);

		/**
		* \brief The method ColorByValue returns a rainbow of colors from red to purple depending on the value of the argument.
		*
		* \param value A value between 0 and 1 which depends on the return color.
		*
		* \return One of the colors of the rainbow.
		*/
		COLORREF ColorByValue(double value);

		/**
		* учитываем капающий перегретый металл
		*/
		void Droplets(TemperatureField *field);

		void DrawAxes(ImageBuffer *ib, TemperatureAxesInfo *tai, ImageBuffer_cstring vertAx,ImageBuffer_cstring horzAx);

		void DrawLegendForPhase(ImageBuffer *ib);

		double GetPsi(double T);
		double GetLambda(double T);
		double GetDensity(double T);
		double GetThermCap(double T);
};

#endif