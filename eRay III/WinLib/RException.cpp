#include "RException.h"

using namespace RClasses;

RException::RException()
{
	Message = TEXT("�������������� ��������");
}

RException::~RException()
{
	//
}

RString RException::GetMessage()
{
	return Message;
}
