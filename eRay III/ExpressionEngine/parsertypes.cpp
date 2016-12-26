#include "parsertypes.h"

int GetOperationPriority(int code)
{
	switch(code)
	{
		case oComposing:
		case oSubtraction:
		{
			return 5;
		}		
		case oMultiplication:
		case oDivision:
		{
			return 4;
		}
		case oUnComposing:
		case oUnSubtraction:
		{
			return 3;
		}
		case oPow:
		{
			return 2;
		}
		case oBracketOpen:
		{
			return 1;
		}	
	}
	return 0;
}

bool IsCalcError(int errorcode)
{
	switch(errorcode)
	{
		case seNegativeArgumentSqrt:
		case seInvalidPowArg:
		case seNegativeLogarithmArg:
		case seDividedByZero:
			return true;
	}
	return false;
}

ExpressionError::ExpressionError(void)
{
	ErrorCode = seNoError;
}

void ExpressionError::SetError(int _ErrorCode, int _Param)
{
	ErrorCode = _ErrorCode;
	Param = _Param;
}

parser_string ExprErrorToStr(ExpressionError &ee)
{
	switch(ee.ErrorCode)
	{
		case seNoError:
			return PARSER_TEXT("Операция выполнена без ошибок");
		case seEmptyExpr:
			return PARSER_TEXT("Пустое выражение");
		case seLexError:
			return PARSER_TEXT("Лексическая ошибка");
		case seInvalidExpr:
			return PARSER_TEXT("Ошибка в записи выражения");
		case seOpenBracketMissing:
			return PARSER_TEXT("Пропущена открывающаяся скобка");
		case seCloseBracketMissing:
			return PARSER_TEXT("Пропущена закрывающаяся скобка");
		case seInvalidUnaryOpArg:
			return PARSER_TEXT("Недопстимое количество аргументов для унарной операции \"") + ee.ParamString + PARSER_TEXT("\". Допустимое количество: 1");
		case seInvalidBinOpArg:
			return PARSER_TEXT("Недопстимое количество аргументов для бинарной операции \"") + ee.ParamString + PARSER_TEXT("\". Допустимое количество: 2");
		case seCantCalc:
			return PARSER_TEXT("Невозможно получить значение выражения");
		case seInvalidCallArgCount:
			return PARSER_TEXT("Недопстимое количество аргументов для функции \"") + ee.ParamString + PARSER_TEXT("\" Допустимое количество: ") + parser_converter::to_string(ee.Param);
		case seInvalidOperatorArgCount:
			return PARSER_TEXT("Неправильное количество аргументов для данной оператора");
		case seUndefinedVar:
			return parser_string(PARSER_TEXT("Значение переменной ")) + ee.ParamString + parser_string(PARSER_TEXT(" не определено"));
		case seUnknowFunction:
			return PARSER_TEXT("Вызов неопределённой функции");
		case seUnknowOperator:
			return PARSER_TEXT("Вызов неизвестного оператора");
		case seUnknowConstant:
			return PARSER_TEXT("Вызов неизвестной константы");
		case seNegativeArgumentSqrt:
			return PARSER_TEXT("Невозможно получить корень отрицательно числа");
		case seInvalidPowArg:
			return PARSER_TEXT("Невозможно возвести в степень");
		case seNegativeLogarithmArg:
			return PARSER_TEXT("Невозможно вычислить аргумент отрицательно числа");
		case seDividedByZero:
			return PARSER_TEXT("Деление на ноль");
	}
	return PARSER_TEXT("undefined error code");
}


VarItem::VarItem(void)
{
	m_IsInit = false;
}

VarItem::VarItem(parser_string _VarName)
{
	m_VarName = _VarName;
	m_IsInit = false;
}

VarItem::VarItem(parser_string _VarName, double _Value)
{
	m_VarName = _VarName;
	m_Value = _Value;
	m_IsInit = true;
}

const parser_string VarItem::GetVarName(void) const
{
	return m_VarName;
}

const bool VarItem::IsInitialized(void) const
{
	return m_IsInit;
}

const double VarItem::GetValue(void) const
{
	return m_Value;
}

void VarItem::SetValue(double _Value)
{
	m_Value = _Value;
	m_IsInit = true;
}

void VarItem::SetName(parser_string _Name)
{
	m_VarName = _Name;
}

bool GetVarValue(Variables Vars, parser_string VarName, double &Value)
{
	for (size_t i = 0; i < Vars.size(); i++)
	{
		if (Vars[i].GetVarName() == VarName)
		{
			Value = Vars[i].GetValue();
			return true;
		}
	}
	return false;
}