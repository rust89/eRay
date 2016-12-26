
#include "RObject.h"

using namespace RClasses;

RObject::RObject()
{

}

HINSTANCE RObject::ModuleHandle()
{
	return GetModuleHandle(0);
}