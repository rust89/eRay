#ifndef _SEMANTICS_H_
#define _SEMANTICS_H_

#include <math.h>

#include "parsertypes.h"
#include "SynNode.h"

//#define AUTO_RAD_CAST

// преобразует строку в число
double GetDigitValue(parser_string str);

// проверяет на точность для заданной погрешности
bool IsZero(double &param);

// проверка параметров на возможность применения функции pow
bool CheckPowParam(double par1, double par2);

// возвращает параметры функции в виде вектора значений
bool GetFunctionParam(SynNode *froot, const Variables &Vars, std::vector <double> &params, ExpressionError &ExprErr);

// расчёт дерева
bool CalcTree(SynNode *root, const Variables &Vars,  ExpressionError &ExprErr);

#endif