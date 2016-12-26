
#include "RConverter.h"

using namespace RClasses;

InvalidConvertException::InvalidConvertException() 
{
	Message = TEXT("Невозможно преобразовать значение"); 
}

InvalidConvertException::~InvalidConvertException() 
{

}

OverflowConvertException::OverflowConvertException()
{
	Message = TEXT("Значение переполнено");
}

OverflowConvertException::~OverflowConvertException()
{

}

long RConverter::ToLong(LPCTSTR Source)
{
	LPTSTR End_Ptr;
	long val = _tcstol(Source,&End_Ptr,10);
	if (*End_Ptr) throw InvalidConvertException();
	if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE) throw OverflowConvertException();
	return val;
}

int RConverter::ToInt(LPCTSTR Source)
{
	LPTSTR End_Ptr;
	long val = _tcstol(Source,&End_Ptr,10);
	if (*End_Ptr) throw InvalidConvertException();
	if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE) throw OverflowConvertException();
	if (val > INT_MAX || val < INT_MIN) throw OverflowConvertException();
	return (int)val;
}

double RConverter::ToDouble(LPCTSTR Source)
{
	LPTSTR End_Ptr;
	double val = _tcstod(Source,&End_Ptr);
	if (*End_Ptr) throw InvalidConvertException();
	if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE) throw OverflowConvertException();
	return val;
}

RString RConverter::ToString(long Source)
{
	const int BUFFER_SIZE = 33;
	TCHAR BUFFER[BUFFER_SIZE];
	if (_ltot_s(Source,BUFFER,BUFFER_SIZE,10))
	{
		throw InvalidConvertException();
	}
	return RString(BUFFER);
}

RString RConverter::ToString(int Source)
{
	const int BUFFER_SIZE = 33;
	TCHAR BUFFER[BUFFER_SIZE];
	if (_ltot_s(Source,BUFFER,BUFFER_SIZE,10))
	{
		throw InvalidConvertException();
	}
	return RString(BUFFER);
}

RString RConverter::ToString(double Source)
{
	const int BUFFER_SIZE = 100;
	TCHAR BUFFER[BUFFER_SIZE];
			
	if (_stprintf_s(BUFFER,BUFFER_SIZE,TEXT("%.3f"),Source) == -1)
	{
		throw InvalidConvertException();
	}
	return RString(BUFFER);
}
