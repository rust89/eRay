#include "Semantics.h"


// преобразует строку в число
double GetDigitValue(parser_string str)
{
#ifdef UNICODE
	return _wtof(str.c_str());
#else
	return atof(str.c_str());
#endif
}

// проверяет на точность для заданной погрешности
bool IsZero(double &param)
{
	if (abs(param) < 0.00000000001000000000)
		return true;
	return false;
}

// проверка параметров на возможность применения функции pow
bool CheckPowParam(double par1, double par2)
{
	if (par1 < 0 && (par2 - (long)par2) != 0)
		return false;
	return true;
}

// возвращает параметры функции в виде вектора значений
bool GetFunctionParam(SynNode *froot, const Variables &Vars, std::vector <double> &params, ExpressionError &ExprErr)
{
	params.clear();
	if (!froot) return false;
	SynNode *ParamArg = const_cast<SynNode*>(froot->SubItem());
	while (ParamArg)
	{
		if (CalcTree(ParamArg, Vars, ExprErr))
		{
			params.push_back(ParamArg->getValue());
		}
		else
		{
			return false;
		}
		ParamArg = const_cast<SynNode*>(ParamArg->Next());
	}
	return true;
}

// расчёт дерева
bool CalcTree(SynNode *root, const Variables &Vars,  ExpressionError &ExprErr)
{
	if (!root) 
	{
		ExprErr.SetError(seEmptyExpr);
		return false;
	}
	vector <double> params;
	// разбор лексического типа
	switch(root->getType())
	{
		case lDigit:
		{
			//root->setValue(GetDigitValue(root->getText()));
			return true;
		}
		case lFunction: // вызов функции
		{
			// получаем параметры для функции
			if (!GetFunctionParam(root, Vars, params, ExprErr))
			{
				return false;
			}
			switch(root->getSpec())
			{
				// вызов синуса
				case fSin:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					#ifdef AUTO_RAD_CAST
						root->Value = sin(3.14159265358979323846*params[0]/180);
					#else
						root->setValue(sin(params[0]));
					#endif
					return true;
				}
				// вызов косинуса
				case fCos:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					#ifdef AUTO_RAD_CAST
						root->Value = cos(3.14159265358979323846*params[0]/180);
					#else
						root->setValue(cos(params[0]));
					#endif
					return true;
				}
				// вызов тангенса
				case fTan:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					
					if ((int)(abs(params[0]) + 90.0) % 180 == 0)
					{
						ExprErr.SetError(seDividedByZero);
						return false;
					}
					#ifdef AUTO_RAD_CAST
						root->Value = tan(3.14159265358979323846*params[0]/180);
					#else
						root->setValue(tan(params[0]));
					#endif
					return true;
				}
				// вызов катангенса
				case fCtn:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					if ((int)(abs(params[0])) % 180 == 0)
					{
						ExprErr.SetError(seDividedByZero);
						return false;
					}
					#ifdef AUTO_RAD_CAST
						root->Value = 1/tan(3.14159265358979323846*params[0]/180);
					#else
						root->setValue(1/tan(params[0]));
					#endif
					return true;
				}
				case fSqrt:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					if (params[0] < 0) 
					{
						ExprErr.SetError(seNegativeArgumentSqrt);
						return false;
					}
					root->setValue(sqrt(params[0]));
					return true;
				}
				case fPow:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,2);
						return false;
					}
					
					if (!CheckPowParam(params[0],params[1]))
					{
						ExprErr.SetError(seInvalidPowArg);
						return false;
					}
					root->setValue(pow(params[0],params[1]));
					return true;
				}
				case fAbs:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					root->setValue(abs(params[0]));
					return true;
				}
				case fExp:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					root->setValue(exp(params[0]));
					return true;
				}
				case fLog:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,2);
						return false;
					}
					
					if (!(params[0] > 0 && params[0] != 1)) 
					{
						ExprErr.SetError(seNegativeLogarithmArg);
						return false;
					}

					if (params[1] <= 0) 
					{
						ExprErr.SetError(seNegativeLogarithmArg);
						return false;
					}
					double znam = log10(params[0]);
					if (znam == 0)
					{
						ExprErr.SetError(seDividedByZero);
						return false;
					}
					root->setValue(log10(params[1])/znam);
					return true;
				}
				case fLn:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}
					
					if (params[0] <= 0) 
					{
						ExprErr.SetError(seNegativeLogarithmArg);
						return false;
					}

					root->setValue(log(params[0]));
					return true;
				}
				case fsinh:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}

					root->setValue((exp(params[0]) - exp(-params[0]))/2);
					return true;
				}
				case fcosh:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}

					root->setValue((exp(params[0]) + exp(-params[0]))/2);
					return true;
				}
				case ftanh:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}

					root->setValue((exp(params[0]) - exp(-params[0]))/(exp(params[0]) + exp(-params[0])));
					return true;
				}
				case fctnh:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidCallArgCount,1);
						return false;
					}

					root->setValue((exp(params[0]) + exp(-params[0]))/(exp(params[0]) - exp(-params[0])));
					return true;
				}
				default:
				{
					ExprErr.ParamString = root->getText();
					ExprErr.SetError(seUnknowFunction);
					return false;
				}
			}
		}
		case lOperator:
		{
			// получаем параметры для оператора
			if (!GetFunctionParam(root, Vars, params, ExprErr))
			{
				return false;
			}
			// в зависимости от оператора
			switch(root->getSpec())
			{
				// сложение
				case oComposing:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,2);
						return false;
					}
					root->setValue(params[0] + params[1]);
					return true;
				}
				// вычитание
				case oSubtraction:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,2);
						return false;
					}
					root->setValue(params[0] - params[1]);
					return true;
				}
				// умножение
				case oMultiplication:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,2);
						return false;
					}
					root->setValue(params[0] * params[1]);
					return true;
				}
				// деление
				case oDivision:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,2);
						return false;
					}

					if (IsZero(params[1]))
					{
						ExprErr.SetError(seDividedByZero);
						return false;
					}
					root->setValue(params[0] / params[1]);
					return true;
				}
				// унарный плюс
				case oUnComposing:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,1);
						return false;
					}
					root->setValue(params[0]);
					return true;
				}
				// унарный минус
				case oUnSubtraction:
				{
					if (params.size() != 1)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,1);
						return false;
					}
					root->setValue(-params[0]);
					return true;
				}
				// операция возведения в степень
				case oPow:
				{
					if (params.size() != 2)
					{
						ExprErr.ParamString = root->getText();
						ExprErr.SetError(seInvalidOperatorArgCount,2);
						return false;
					}

					if (!CheckPowParam(params[0],params[1]))
					{
						ExprErr.SetError(seInvalidPowArg);
						return false;
					}

					root->setValue(pow(params[0],params[1]));

					return true;
				}
				// спецификатор операции не найден
				default:
				{
					ExprErr.ParamString = root->getText();
					ExprErr.SetError(seUnknowOperator);
					return false;
				}
			}
		}
		case lVariable:
		{
			double tmpValue;
			if (!GetVarValue(Vars, root->getText(), tmpValue))
			{
				ExprErr.ParamString = root->getText(); 
				ExprErr.SetError(seUndefinedVar);
				return false;
			}
			root->setValue(tmpValue);
			return true;
		}
		case lConst:
		{
			switch(root->getSpec())
			{
				case c_pi:
				{
					root->setValue(3.14159265358979323846);
					return true;
				}
				case c_e:
				{
					root->setValue(2.71828182845904523536);
					return true;
				}
				default:
				{
					ExprErr.SetError(seUnknowConstant);
					return false;
				}
			}
		}
		default:
		{
			ExprErr.SetError(seCantCalc);
			return false;
		}
	}
	return true;
}