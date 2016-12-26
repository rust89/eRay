
#include "Tokenizer.h"
//---------------------------------------------------
Parser::Parser(void)
{
	InitFunctions();
	InitOperators();
	InitConstants();
	InitAlfabit();
}
//---------------------------------------------------
void Parser::ToLowerCase(parser_string &str)
{
	for (size_t i(0), i_end(str.size()); i < i_end; i++)
	{
		#ifdef UNICODE
		str[i] = towlower(str[i]);
		#else
		str[i] = tolower(str[i]);
		#endif
	}
}
//---------------------------------------------------
void Parser::DeleteWhiteSpace(const parser_string &Input, parser_string &Output)
{
	Output.clear();
	for (size_t i(0), i_end(Input.size()); i < i_end; i++)
	{
		if (Input[i] != PARSER_TEXT(' '))
		{
			Output+= Input[i];
		}
	}
}
//---------------------------------------------------
// проверка на вещественное число
bool Parser::IsNumber(parser_string &Input)
{
	bool ExPoint = false; // встречалась ли точка
	for (size_t i(0), i_end(Input.size()); i < i_end; i++)
	{
		if(IsDigit(Input[i]))
		{
			continue;
		}
		else
		{
			if (Input[i] == PARSER_TEXT('.') && !ExPoint)
			{
				if (i == 0) return false; // точка не может быть в начале числа
				if (i == i_end - 1) return false; // точка не может быть в конце числа
				ExPoint = true;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}
//---------------------------------------------------
int Parser::IsFunction(parser_string &Input)
{
	for (size_t i(0), i_end(functions.size()); i < i_end; i++)
	{
		if (functions[i].second == Input)
		{
			return functions[i].first;
		}
	}
	return fUnknown;
}
//---------------------------------------------------
int Parser::IsConstant(parser_string &Input)
{
	for (size_t i = 0, i_end(constants.size()); i < i_end; i++)
	{
		if (constants[i].second == Input)
		{
			return constants[i].first;
		}
	}
	return cUnknown;
}
//---------------------------------------------------
bool Parser::IsDigit(PARSER_CHAR _char)
{
	switch(_char)
	{
		case PARSER_TEXT('0'):
		case PARSER_TEXT('1'):
		case PARSER_TEXT('2'):
		case PARSER_TEXT('3'):
		case PARSER_TEXT('4'):
		case PARSER_TEXT('5'):
		case PARSER_TEXT('6'):
		case PARSER_TEXT('7'):
		case PARSER_TEXT('8'):
		case PARSER_TEXT('9'):
			return true;
	}
	return false;
}
//---------------------------------------------------
void Parser::InitFunctions()
{
	functions.resize(14);
	functions[0].first = fSin; functions[0].second = PARSER_TEXT("sin");
	functions[1].first = fCos; functions[1].second = PARSER_TEXT("cos");
	functions[2].first = fTan; functions[2].second = PARSER_TEXT("tan");
	functions[3].first = fCtn; functions[3].second = PARSER_TEXT("ctn");
	functions[4].first = fSqrt; functions[4].second = PARSER_TEXT("sqrt");
	functions[5].first = fPow; functions[5].second = PARSER_TEXT("pow");
	functions[6].first = fAbs; functions[6].second = PARSER_TEXT("abs");
	functions[7].first = fExp; functions[7].second = PARSER_TEXT("exp");
	functions[8].first = fLog; functions[8].second = PARSER_TEXT("log");
	functions[9].first = fLn; functions[9].second = PARSER_TEXT("ln");

	functions[10].first = fsinh; functions[10].second = PARSER_TEXT("sinh");
	functions[11].first = fcosh; functions[11].second = PARSER_TEXT("cosh");
	functions[12].first = ftanh; functions[12].second = PARSER_TEXT("tanh");
	functions[13].first = fctnh; functions[13].second = PARSER_TEXT("ctnh");
}
//---------------------------------------------------
void Parser::InitOperators()
{
	operators.resize(8);
	operators[0].first = oBracketOpen; operators[0].second = PARSER_TEXT("(");
	operators[1].first = oBracketClose; operators[1].second = PARSER_TEXT(")");
	operators[2].first = oComma; operators[2].second = PARSER_TEXT(",");
	operators[3].first = oComposing; operators[3].second = PARSER_TEXT("+");
	operators[4].first = oSubtraction; operators[4].second = PARSER_TEXT("-");
	operators[5].first = oMultiplication; operators[5].second = PARSER_TEXT("*");
	operators[6].first = oDivision; operators[6].second = PARSER_TEXT("/");
	operators[7].first = oPow; operators[7].second = PARSER_TEXT("^");
}
//---------------------------------------------------
void Parser::InitConstants()
{
	constants.resize(2);
	constants[0].first = c_pi; constants[0].second = PARSER_TEXT("c_pi");
	constants[1].first = c_e; constants[1].second = PARSER_TEXT("c_e");
}
//---------------------------------------------------
void Parser::FindOperators(parser_string &Input, tokenvector &tv)
{
	vector <bool> BitMap(Input.size(), false); // карта включений
	tv.clear();

	for (size_t i(0), i_end(operators.size()), finded_pos, last_pos; i < i_end; i++)
	{
		finded_pos = parser_string::npos, last_pos = 0;
		while((finded_pos = Input.find(operators[i].second,last_pos)) != parser_string::npos)
		{
			size_t tp, token_end = finded_pos + operators[i].second.size();
			for (tp = finded_pos; tp < token_end; tp++)
			{	
				if (BitMap[tp]) break;
			}
			if (tp != token_end) 
			{
				// если позиция занята
				// то продолжаем перебор
				last_pos = finded_pos + operators[i].second.size();
				continue;
			}
			else
			{
				// если позиция не занята
				// то занимаем позицию
				for (tp = finded_pos; tp < token_end; tp++)
				{
					BitMap[tp] = true;
				}
				// сохраняем токен в буфер
				tv.push_back(Token(operators[i].second, lOperator,operators[i].first,finded_pos));
			}
			last_pos = token_end;
		}
	}

	// включение в буфер нераспознанных областей
	for (size_t i(0), i_end(Input.size()); i < i_end; i++)
	{
		if (!BitMap[i])
		{
			size_t j;
			for (j = i+1; j < Input.size(); j++)
			{
				if (!BitMap[j]) continue;
				else
				{
					tv.push_back(Token(Input.substr(i,j-i),0,0,i));
					i = j;
					break;
				}
			}
			if (j == Input.size())
			{
				tv.push_back(Token(Input.substr(i,j-i),0,0,i));
				i = j;
				break;
			}
		}
	}
}
//---------------------------------------------------
bool Parser::Work(const parser_string &Input, tokenvector &tv)
{
	if (!Input.size()) return true; 
	parser_string buffer;
	DeleteWhiteSpace(Input,buffer);
	
	ToLowerCase(buffer);
	FindOperators(buffer,tv);
	TokenVectorSortByPos(tv);
	for (size_t i(0), i_end(tv.size()); i < i_end; i++)
	{
		if (tv[i].getType() == lUnknown)
		{
			parser_string temp_str = tv[i].getText();
			if(IsNumber(temp_str))
			{
				tv[i].setType(lDigit);
			}
			else
			{
				int Lext = IsFunction(temp_str);
				if (Lext != fUnknown)
				{
					tv[i].setType(lFunction); tv[i].setSpec(Lext);
				}
			}
		}
	}
	return Variabling(tv);
}
//---------------------------------------------------------
bool Parser::Variabling(tokenvector &tv)
{
	for (size_t i(0), i_end(tv.size()); i < i_end; i++)
	{
		if (tv[i].getType() == lUnknown)
		{
			int constt;
			parser_string temp_str = tv[i].getText();
			
			// проверка на константу
			if ((constt = IsConstant(temp_str)) != cUnknown)
			{
				tv[i].setType(lConst); 
				tv[i].setSpec(constt); 
				continue; 
			}
			// проверка на удволетворение требования переменных (состоит из букв)
			for (size_t j(0), j_end(temp_str.size()); j < j_end; j++)
			{
				size_t fined_pos = alf.find(temp_str[j]);
				if (fined_pos == parser_string::npos)
				{
					return false;
				}
			}
			tv[i].setType(lVariable);
		}
	}
	return true;
}

//---------------------------------------------------------
void Parser::InitAlfabit(void)
{
	alf.assign(PARSER_TEXT("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"));
}
//---------------------------------------------------------