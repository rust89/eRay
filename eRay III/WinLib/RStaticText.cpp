
#include "RStaticText.h"

using namespace RClasses;

RStaticText::RStaticText()
{
	ClassName = TEXT("RClasses::RStatic");
}

RStaticText::~RStaticText()
{

}

bool RStaticText::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, WC_STATIC, ClassName.c_str(), 
							WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_LEFT, 0, 
							p_wndParent, uiID, x, y, width, height);
}

bool RStaticText::Create(UINT Style, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width,int height)
{
	return RControl::Create(TEXT(""), WC_STATIC, ClassName.c_str(),
							Style, 0, 
							p_wndParent, uiID,x, y, width, height);
}

bool RStaticText::Create(LPCTSTR Text, UINT Style, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width,int height)
{
	return RControl::Create(Text, WC_STATIC, ClassName.c_str(),
							Style, 0, 
							p_wndParent, uiID,x, y, width, height);
}