#include "RGroupBox.h"

using namespace RClasses;

RGroupBox::RGroupBox()
{
	ClassName = TEXT("RClasses::RGroupBox");
}
RGroupBox::~RGroupBox()
{
	//
}

bool RGroupBox::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, WC_BUTTON, ClassName.c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, p_wndParent, uiID,x,y,width,height);
}