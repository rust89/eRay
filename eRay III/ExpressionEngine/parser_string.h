#ifndef _PARSER_STRING_
#define _PARSER_STRING_

#include <string>
#include <vector>

#ifdef UNICODE
	typedef std::wstring parser_string;
	typedef wchar_t PARSER_CHAR;
	#define __PARSER_TEXT(quote) L##quote
#else
	typedef std::string parser_string;
	typedef char PARSER_CHAR;
	#define __PARSER_TEXT(quote) quote
#endif

#define PARSER_TEXT(quote) __PARSER_TEXT(quote)

typedef std::vector <parser_string> parser_strings;

class parser_converter
{
	public:
		static parser_string to_string(int value);
		static parser_string to_string(double value);
		static bool try_to_double(parser_string &str, double &val);
		static bool try_to_integer(parser_string &str, long &val);
};


#endif