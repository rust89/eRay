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

// ���������� ������������ �������
typedef figure_rectangle <double> frectangle;
typedef frectangle* lpfrectangle;

typedef figure_rectangle <int> irectangle;
typedef irectangle* lpirectangle;
// ���������� ��������� ���������� �������
typedef struct __GraphInfo
{
	RECT DrawRegion; // ������� ��������� ������� �� ���������� hdc
	frectangle GraphRegion; // ������� ���������� �������
	long Xpos, Ypos; // ����������� ����
	long GraphicIndent; // ������ ������� �� ������� ���������
	ffigure PointMas; // ������ �������� �����
	ffigure RegionPoints; // ������ ����� ��� ���������� ����������
	ifigure OutPoints; // ������ ��������������� �����
	double RayRadius; // ������ ���������� �����
	double RayVoltage; // ������ ���������� �����
	double AmpTraffic; // ��� ����
	double XPixelWeight; // ��� ������� ����� ��� X
	double YPixelWeight; // ��� ������� ����� ��� Y
	double X0; // ���������� ������ ���������
	double Y0;
	bool Proportions; // ��������� ���������
	LONG Notch; // ���������� ����� ��������� � ��������
	LONG DivisionCount; // ���������� ������������� ������� �� ���
} GraphInfo;

typedef GraphInfo* lpGraphInfo;

#endif