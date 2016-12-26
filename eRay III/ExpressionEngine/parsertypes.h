#ifndef _PARSERTYPES_H_
#define _PARSERTYPES_H_

#include "parser_string.h"

#include <vector>
  
using std::size_t;
using std::vector;
using std::pair;

// лексические типы
enum eLexTypes
{
	lUnknown = 0, // тип не определён
	lDigit, // цифра
	lOperator, // оператор
	lFunction, // встроенная функция
	lVariable, // переменная
	lConst // встроенная константа
};

// встроенные функции
enum eFunctionType
{
	fUnknown = 0, // тип не определён
	fSin, // синус
	fCos, // косинус
	fTan, // тангенс
	fCtn,  // котангенс
	fSqrt, // квадратный корень
	fPow, // возведение в степень
	
	fAbs, // модуль
	fExp, // экспонента
	fLog, // логарифм по основанию
	fLn, // натуральный логарифм

	// гиперболические функции
	fsinh, // гперболический синус
	fcosh, // гперболический косинус
	ftanh, // гперболический тангенс
	fctnh // гперболический котангенс
};

// стандартные операции
enum eOperators
{
	oUnknown = 0, // тип не определён
	// скобки
	oBracketOpen,
	oBracketClose,
	// запятая 
	oComma,
	// операция сложения
	oComposing,	
	// операция вычитания
	oSubtraction,
	// унарное сложение
	oUnComposing,
	// унарное вычитание
	oUnSubtraction,
	// операция умножения
	oMultiplication,
	// операция деления
	oDivision,
	// возведение в степень
	oPow
};

// коды встроенных констант
enum eConstants
{
	cUnknown = 0, // неопределённая константа
	c_pi, // число пи
	c_e // число e
};

// возвращает приоритет по её коду, определённому в перечислении eOperators
int GetOperationPriority(int);

// сужит для соответствия фукция - константа
typedef pair <int,parser_string> instruct;
typedef vector <instruct> instructvector;

//-----------------------------------------------------


// определяет коды возможных синтаксических и лексических ошибок
enum SynError
{
	seNoError = 0,
	seEmptyExpr, // пустое выражение
	seLexError, // лексическая ошибка
	seInvalidExpr, // неправильное выражение
	seOpenBracketMissing, // пропущена открывающаяся скобка
	seCloseBracketMissing, // пропущена закрывающаяся скобка
	seInvalidUnaryOpArg, // неправильный либо отсутствующий аргумент для унарной операции
	seInvalidBinOpArg, // неправильный либо отсутствующий аргумент для унарной операции
	seCantCalc, // невозможно произвести вычисление
	seInvalidCallArgCount, // неправильное количество аргументов в вызове функции
	seInvalidOperatorArgCount, // неправильное количество аргументов для оператора
	seUndefinedVar, // неинициализированная переменная
	seUnknowFunction, // неизвестная функция
	seUnknowOperator, // неизвестный оператор
	seUnknowConstant, // неизвестная константа
	
	// ошибки вычисления
	seNegativeArgumentSqrt, // невозможно извлечь корень из отрицательного числа
	seInvalidPowArg, // неправилные аргументы для возведения в степень
	seNegativeLogarithmArg, // отрицательный аргумент у логарфма
	seDividedByZero // деление на ноль !!!11
};

// проверка: это ошибка вычисления?
bool IsCalcError(int errorcode);

// класс хранения ошибок
class ExpressionError
{
	public:
		int ErrorCode;
		int Param;
		parser_string ParamString;
		ExpressionError(void);
		void SetError(int _ErrorCode, int _Param = 0);
};

parser_string ExprErrorToStr(ExpressionError &ee);

// класс для хранения переменной
class VarItem
{
	protected:
		/**
		* The field m_VarName is the name of the variable.
		*/
		parser_string m_VarName;

		/**
		* The field m_Value is the value of the variable.
		*/
		double m_Value;

		/**
		* The flag, which contains information about variable initialization.
		*/
		bool m_IsInit;
	public:
		VarItem(void);
		VarItem(parser_string _VarName);
		VarItem(parser_string _VarName, double _Value);
		const parser_string GetVarName(void) const;
		const bool IsInitialized(void) const;
		const double GetValue(void) const;
		void SetValue(double _Value);
		void SetName(parser_string _Name);
};

//-------------------------------------------
typedef vector <VarItem> Variables;
//-------------------------------------------

bool GetVarValue(Variables, parser_string, double&);

#endif