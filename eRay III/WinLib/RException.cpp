#include "RException.h"

using namespace RClasses;

RException::RException()
{
	Message = TEXT("Исключительная ситуация");
}

RException::~RException()
{
	//
}

RString RException::GetMessage()
{
	return Message;
}
