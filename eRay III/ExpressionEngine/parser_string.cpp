#include "parser_string.h"

parser_string parser_converter::to_string(int value)
{	
	parser_string Result;
	const int BUFFER_SIZE = 33;	
#ifdef UNICODE
	wchar_t BUFFER[BUFFER_SIZE];
	if (!_itow_s(value,BUFFER,BUFFER_SIZE,10))
	{
		return PARSER_TEXT("");
	}
#else
	char BUFFER[BUFFER_SIZE];
	if (!_itoa_s(value,BUFFER,BUFFER_SIZE,10))
	{
		return PARSER_TEXT("");
	}
#endif
	return Result.assign(BUFFER);
}

parser_string parser_converter::to_string(double value)
{	
	parser_string Result;
	const int BUFFER_SIZE = 33;	
#ifdef UNICODE
	wchar_t BUFFER[BUFFER_SIZE];
	
	if (swprintf_s(BUFFER,BUFFER_SIZE,L"%f",value) == -1)
	{
		return PARSER_TEXT("");
	}
#else
	char BUFFER[BUFFER_SIZE];
	if (sprintf_s(BUFFER,BUFFER_SIZE,"%f",value) == -1)
	{
		return PARSER_TEXT("");
	}
#endif
	return Result.assign(BUFFER);
}

// безопасное преобразование строки в число
bool parser_converter::try_to_double(parser_string &str, double &val)
{
#ifdef UNICODE	
	wchar_t *stopscan;
	val = wcstod(str.c_str(),&stopscan);
#else
	char *stopscan;
	val = strtod(str.c_str(),&stopscan);
#endif
	if (*stopscan) return false;
	return true;
}

// безопасное преобразование строки в число
bool parser_converter::try_to_integer(parser_string &str, long &val)
{
#ifdef UNICODE	
	wchar_t *stopscan;
	val = wcstol(str.c_str(), &stopscan, 10);
#else
	char *stopscan;
	val = strtol(str.c_str(), &stopscan, 10);
#endif
	if (*stopscan) return false;
	return true;
}