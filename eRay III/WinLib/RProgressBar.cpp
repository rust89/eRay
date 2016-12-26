#include "RProgressBar.h"

using namespace RClasses;

RProgressBar::RProgressBar()
	: RControl()
{
	ClassName = TEXT("RClasses::RProgressBar");
}

RProgressBar::~RProgressBar()
{

}

bool RProgressBar::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, PROGRESS_CLASS, ClassName.c_str(), WS_CHILD | WS_VISIBLE, 0, p_wndParent, uiID,x,y,width,height);
}

bool RProgressBar::Create(DWORD Style, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(TEXT(""), PROGRESS_CLASS, ClassName.c_str(), WS_CHILD | WS_VISIBLE | Style, 0, p_wndParent, uiID,x,y,width,height);
}

void RProgressBar::SetMinMax(WORD Min, WORD Max)
{
	SendMessage(m_hHandle,PBM_SETRANGE,0,MAKELPARAM(Min,Max));
}

void RProgressBar::SetPosition(DWORD Position)
{
	SendMessage(m_hHandle,PBM_SETPOS,Position,0);
}