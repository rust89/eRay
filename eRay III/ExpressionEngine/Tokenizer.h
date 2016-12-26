#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "parsertypes.h"
#include "Token.h"

class Parser
{
	protected:
		instructvector functions;
		instructvector operators;
		instructvector constants;
		parser_string alf; // �������� �������
		// �������������� ���������
		void InitOperators(void);
		// �������������� �������
		void InitFunctions(void);
		// �������������� ���������
		void InitConstants(void);
		// �������� �� �������������� ������� � ������
		bool IsDigit(PARSER_CHAR _char);
		// ������������� ��������
		void InitAlfabit(void);
		// �������� �� �������������� ������ � ������������� �����
		bool IsNumber(parser_string &Input);
		// �������� �� �������������� ������ � ���������� �������
		int IsFunction(parser_string &Input);
		// �������� �� �������������� � ���� �������
		int IsConstant(parser_string &Input);
		// ������������ ��������� ����� ���������� � ������ � �������� �� � ������ �������
		// �� ������������ ������ ����� � ��������� � ������
		void FindOperators(parser_string &Input, tokenvector &tv);
		// ������������� �������� � ����������
		bool Variabling(tokenvector &tv);
		// ����������� ������ � ������� ��������
		void ToLowerCase(parser_string &str);
	public:
		Parser(void);
		// ������� ����� �������
		void DeleteWhiteSpace(const parser_string &Input,parser_string &Output);
		// ��������� ������� �� �������
		bool Work(const parser_string &Input, tokenvector &tv);
};

#endif // _TOKENIZER_H