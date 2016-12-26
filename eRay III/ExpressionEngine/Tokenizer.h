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
		parser_string alf; // содержит алфавит
		// инициализирует операторы
		void InitOperators(void);
		// инициализирует функции
		void InitFunctions(void);
		// инициализирует константы
		void InitConstants(void);
		// проверка на принадлежность символа к цифрам
		bool IsDigit(PARSER_CHAR _char);
		// инициализация алфавита
		void InitAlfabit(void);
		// проверка на принадлежнсоть строки к вещественному числу
		bool IsNumber(parser_string &Input);
		// проверка на принадлежность строки к встроенной функции
		int IsFunction(parser_string &Input);
		// проверка на принадлежность к типу контант
		int IsConstant(parser_string &Input);
		// осуществляет первичный поиск операторов в строке и выделяет их в массив токенов
		// не распознанные токены также в ключаются в массив
		void FindOperators(parser_string &Input, tokenvector &tv);
		// распознавание констант и переменных
		bool Variabling(tokenvector &tv);
		// преобразует строку к нижнему регистру
		void ToLowerCase(parser_string &str);
	public:
		Parser(void);
		// удаляет знаки пробела
		void DeleteWhiteSpace(const parser_string &Input,parser_string &Output);
		// выполняет деление на лексемы
		bool Work(const parser_string &Input, tokenvector &tv);
};

#endif // _TOKENIZER_H