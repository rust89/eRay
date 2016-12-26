#ifndef _GRAPHICALTYPES_H_
#define _GRAPHICALTYPES_H_

#include <windows.h>
#include <tchar.h>
#include <Strsafe.h>

#include <vector>
#include <string>

#include "Figure.h"

#ifdef _UNICODE
	typedef std::wstring gstring;
#else
	typedef std::string gstring;
#endif


typedef figure_point <double> fpoint;
typedef fpoint* lpfpoint;

typedef figure_point <int> ipoint;
typedef ipoint* lpipoint;

typedef Figure <double> ffigure;
typedef ffigure* lpffigure;

typedef Figure <int> ifigure;
typedef ifigure* lpifigure;

// определяет прямоуголную область
typedef figure_rectangle <double> frectangle;
typedef frectangle* lpfrectangle;

typedef figure_rectangle <int> irectangle;
typedef irectangle* lpirectangle;
// определяет параметры построения графика
typedef struct __GraphInfo
{
	RECT DrawRegion; // область отрисовки графика на устройстве hdc
	frectangle GraphRegion; // область построения графика
	long Xpos, Ypos; // кооррдинаты осей
	long GraphicIndent; // отступ графика от границы рисования
	ffigure PointMas; // массив исходных точек
	ffigure RegionPoints; // массив точек для регионного построения
	ifigure OutPoints; // массив перерасчитанных точек
	double RayRadius; // радиус фокального пятна
	double RayVoltage; // радиус фокального пятна
	double AmpTraffic; // ток луча
	double XPixelWeight; // вес пикселя вдоль оси X
	double YPixelWeight; // вес пикселя вдоль оси Y
	double X0; // координаты начала координат
	double Y0;
	bool Proportions; // соблюдать пропорции
	LONG Notch; // расстояние между засечками в пикселях
	LONG DivisionCount; // количество промежуточных делений на оси
} GraphInfo;

typedef GraphInfo* lpGraphInfo;

#endif