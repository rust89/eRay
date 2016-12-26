#ifndef _ROBJECT_H_
#define _ROBJECT_H_


#include <windows.h>

#include "RString.h"

namespace RClasses {

class RObject
{
	public:
		RObject();
		HINSTANCE ModuleHandle();
};

} // RClasses
#endif