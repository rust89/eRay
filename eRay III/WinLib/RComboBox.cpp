
#include "RComboBox.h"

using namespace RClasses;

RComboBox::RComboBox()
{
	ClassName = TEXT("RClasses::RComboBox");
}

RComboBox::~RComboBox()
{
	//
}

bool RComboBox::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, WC_COMBOBOX, ClassName.c_str(), WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST , WS_EX_LEFT, p_wndParent, uiID,x,y,width,height);
}

int RComboBox::AddString(LPCTSTR Text)
{
	return SendMessage(m_hHandle,CB_ADDSTRING,0,(LPARAM)Text);
}

int RComboBox::AddString(LPCTSTR Text, LPVOID lpData)
{
	int Index = SendMessage(m_hHandle,CB_ADDSTRING,0,(LPARAM)Text);
	if (Index == CB_ERR || Index == CB_ERRSPACE)
		return Index;
	
	int Res = SendMessage(m_hHandle,CB_SETITEMDATA,Index,(LPARAM)lpData);
	return Index;
}

int RComboBox::GetSelectedIndex()
{
	return SendMessage(m_hHandle,CB_GETCURSEL,0,0);
}

LPVOID RComboBox::GetItemData(int Index)
{
	return (LPVOID)SendMessage(m_hHandle,CB_GETITEMDATA,Index,0);
}

void RComboBox::SetSelectedIndex(int Index)
{
	SendMessage(m_hHandle,CB_SETCURSEL,Index,0);
}
