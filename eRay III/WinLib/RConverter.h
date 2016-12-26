#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include "RException.h"

namespace RClasses {

class InvalidConvertException : public RException
{
	public:
		InvalidConvertException();
		~InvalidConvertException();
};

class OverflowConvertException : public RException
{
	public:
		OverflowConvertException();
		~OverflowConvertException();
};

class RConverter
{
	public:
		static long ToLong(LPCTSTR Source);
		static int ToInt(LPCTSTR Source);
		static double ToDouble(LPCTSTR Source);
		static RString ToString(long Source);
		static RString ToString(int Source);
		static RString ToString(double Source);
};

}

#endif