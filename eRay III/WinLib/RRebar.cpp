#include "RRebar.h"

using namespace RClasses;

RRebar::RRebar()
{
	ClassName = TEXT("RClasses::RRebar");
}

RRebar::~RRebar()
{
	//
}

bool RRebar::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, REBARCLASSNAME, ClassName.c_str(), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
							WS_CLIPCHILDREN | RBS_VARHEIGHT |
							CCS_NODIVIDER | RBS_BANDBORDERS, WS_EX_TOOLWINDOW, p_wndParent, uiID, x, y, width, height);
}
