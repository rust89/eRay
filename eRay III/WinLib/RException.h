#ifndef _REXCEPTION_H_
#define _REXCEPTION_H_

#include "RString.h"

namespace RClasses {

class RException
{
	protected:
		RString Message;
	public:
		RException();
		~RException();
		RString GetMessage();
};

}


#endif