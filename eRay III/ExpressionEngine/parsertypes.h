#ifndef _PARSERTYPES_H_
#define _PARSERTYPES_H_

#include "parser_string.h"

#include <vector>
  
using std::size_t;
using std::vector;
using std::pair;

// ����������� ����
enum eLexTypes
{
	lUnknown = 0, // ��� �� ��������
	lDigit, // �����
	lOperator, // ��������
	lFunction, // ���������� �������
	lVariable, // ����������
	lConst // ���������� ���������
};

// ���������� �������
enum eFunctionType
{
	fUnknown = 0, // ��� �� ��������
	fSin, // �����
	fCos, // �������
	fTan, // �������
	fCtn,  // ���������
	fSqrt, // ���������� ������
	fPow, // ���������� � �������
	
	fAbs, // ������
	fExp, // ����������
	fLog, // �������� �� ���������
	fLn, // ����������� ��������

	// ��������������� �������
	fsinh, // �������������� �����
	fcosh, // �������������� �������
	ftanh, // �������������� �������
	fctnh // �������������� ���������
};

// ����������� ��������
enum eOperators
{
	oUnknown = 0, // ��� �� ��������
	// ������
	oBracketOpen,
	oBracketClose,
	// ������� 
	oComma,
	// �������� ��������
	oComposing,	
	// �������� ���������
	oSubtraction,
	// ������� ��������
	oUnComposing,
	// ������� ���������
	oUnSubtraction,
	// �������� ���������
	oMultiplication,
	// �������� �������
	oDivision,
	// ���������� � �������
	oPow
};

// ���� ���������� ��������
enum eConstants
{
	cUnknown = 0, // ������������� ���������
	c_pi, // ����� ��
	c_e // ����� e
};

// ���������� ��������� �� � ����, ������������ � ������������ eOperators
int GetOperationPriority(int);

// ����� ��� ������������ ������ - ���������
typedef pair <int,parser_string> instruct;
typedef vector <instruct> instructvector;

//-----------------------------------------------------


// ���������� ���� ��������� �������������� � ����������� ������
enum SynError
{
	seNoError = 0,
	seEmptyExpr, // ������ ���������
	seLexError, // ����������� ������
	seInvalidExpr, // ������������ ���������
	seOpenBracketMissing, // ��������� ������������� ������
	seCloseBracketMissing, // ��������� ������������� ������
	seInvalidUnaryOpArg, // ������������ ���� ������������� �������� ��� ������� ��������
	seInvalidBinOpArg, // ������������ ���� ������������� �������� ��� ������� ��������
	seCantCalc, // ���������� ���������� ����������
	seInvalidCallArgCount, // ������������ ���������� ���������� � ������ �������
	seInvalidOperatorArgCount, // ������������ ���������� ���������� ��� ���������
	seUndefinedVar, // �������������������� ����������
	seUnknowFunction, // ����������� �������
	seUnknowOperator, // ����������� ��������
	seUnknowConstant, // ����������� ���������
	
	// ������ ����������
	seNegativeArgumentSqrt, // ���������� ������� ������ �� �������������� �����
	seInvalidPowArg, // ����������� ��������� ��� ���������� � �������
	seNegativeLogarithmArg, // ������������� �������� � ��������
	seDividedByZero // ������� �� ���� !!!11
};

// ��������: ��� ������ ����������?
bool IsCalcError(int errorcode);

// ����� �������� ������
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

// ����� ��� �������� ����������
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