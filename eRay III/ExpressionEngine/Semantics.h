#ifndef _SEMANTICS_H_
#define _SEMANTICS_H_

#include <math.h>

#include "parsertypes.h"
#include "SynNode.h"

//#define AUTO_RAD_CAST

// ����������� ������ � �����
double GetDigitValue(parser_string str);

// ��������� �� �������� ��� �������� �����������
bool IsZero(double &param);

// �������� ���������� �� ����������� ���������� ������� pow
bool CheckPowParam(double par1, double par2);

// ���������� ��������� ������� � ���� ������� ��������
bool GetFunctionParam(SynNode *froot, const Variables &Vars, std::vector <double> &params, ExpressionError &ExprErr);

// ������ ������
bool CalcTree(SynNode *root, const Variables &Vars,  ExpressionError &ExprErr);

#endif