#include "RDialog.h"

using namespace RClasses;

RDialog::RDialog()
	: RWindow()
{

}
RDialog::~RDialog()
{

}

INT_PTR WINAPI RDialog::DialogProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
