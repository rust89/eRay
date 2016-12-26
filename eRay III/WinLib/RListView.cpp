
#include "RListView.h"
#include <Uxtheme.h>

using namespace RClasses;

RListView::RListView()
	: RControl()
{
	ClassName = TEXT("RClasses::RListView");
	AddMessage(WM_SETFOCUS,MSG_EVENT_CAST(RListView::OnSetFocus),this);
}

RListView::~RListView()
{
	//
}

// возвращает индекс вставленного столбца
int RListView::CreateColumn(LPCTSTR Text, int Width)
{
	LVCOLUMN NewColumn;
	ZeroMemory(&NewColumn,sizeof(LVCOLUMN));
	NewColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	NewColumn.fmt  = LVCFMT_LEFT;
	NewColumn.cx = Width;
	NewColumn.pszText = (LPTSTR)Text;

	// опредеялем максимальный индекс колонки
	int MaxIndex = 0;
	for (int i = 0; i < Columns.Length(); i++)
		if (MaxIndex < Columns[i]) Columns = Columns[i];

	int ColumnIndex = ListView_InsertColumn(m_hHandle,MaxIndex+1,&NewColumn);
	Columns.Append(ColumnIndex);
	return ColumnIndex;
}

int RListView::AddItem(LPLVITEM lplvi)
{
	return ListView_InsertItem(m_hHandle,lplvi);
}

int RListView::AddItem(LPCTSTR Text)
{
	int Index = FindLastItemIndex();

	LVITEM lvitem;
	ZeroMemory(&lvitem,sizeof(LVITEM));
	lvitem.mask = LVIF_TEXT;
	
	lvitem.iItem = Index+1;
	lvitem.pszText = (LPTSTR)Text;
	lvitem.cchTextMax = _tcslen(Text);
	return ListView_InsertItem(m_hHandle,&lvitem);
}

int RListView::AddItem(LPCTSTR Text,int GroupID)
{
	LVITEM lvitem;
	ZeroMemory(&lvitem,sizeof(LVITEM));
	lvitem.mask = LVIF_GROUPID | LVIF_TEXT;
	
	lvitem.iItem = 0;
	lvitem.pszText = (LPTSTR)Text;
	lvitem.cchTextMax = _tcslen(Text);
	lvitem.iGroupId = GroupID;
	return ListView_InsertItem(m_hHandle,&lvitem);
}

void RListView::ClearList()
{
	ListView_DeleteAllItems(m_hHandle);
}

bool RListView::Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width, int height)
{
	return RControl::Create(Text, WC_LISTVIEW, ClassName.c_str(), WS_CHILD | WS_VISIBLE, 0, p_wndParent, uiID, x, y, width, height);
}

bool RListView::Create(RCustomWindow *p_wndParent, UINT uiID, int x, int y, int width,int height)
{
	return RControl::Create(NULL, WC_LISTVIEW, ClassName.c_str(), WS_CHILD | WS_VISIBLE, 0, p_wndParent, uiID, x, y, width, height);
}

void RListView::SetItemText(int ItemIndex, int SubItemIndex, LPCTSTR Text)
{
	ListView_SetItemText(m_hHandle,ItemIndex,SubItemIndex,(LPWSTR)Text);
}

void RListView::SetExListViewStyle(DWORD Style)
{
	ListView_SetExtendedListViewStyle(m_hHandle,Style);
}

void RListView::SetListViewState(ListViewStates State)
{	
	switch (State)
	{
		case lvsList:
			SetStyle((GetStyle() & ~(LVS_ICON | LVS_REPORT)) | LVS_LIST);
			break;
		case lvsIcon:
			SetStyle((GetStyle() & ~(LVS_LIST | LVS_REPORT)) | LVS_ICON);
			break;
		case lvsReport:
			SetStyle((GetStyle() & ~(LVS_LIST | LVS_ICON)) | LVS_REPORT);
			break;
	}
}

// добавление группы в конец списка
// возвращает идентификатор группы
int RListView::AddGroup(LPCTSTR Text)
{
	LVGROUP group;
	ZeroMemory(&group,sizeof(LVGROUP));

	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_HEADER | LVGF_GROUPID | LVGF_STATE;
	group.pszHeader = (LPTSTR)Text;
	group.state = LVGS_COLLAPSIBLE;

	int MaxID = 1;
	for (int i = 0; i < GroupItems.Length(); i++)
		if (MaxID <= GroupItems[i].Id) MaxID = GroupItems[i].Id + 1;

	group.iGroupId = MaxID;
	int GroupIndex = ListView_InsertGroup(m_hHandle, -1, &group);
	RGroupItem gitem;
	gitem.Index = GroupIndex;
	gitem.Id = MaxID;
	GroupItems.Append(gitem);
	return MaxID;
}

void RListView::SetGroupEnable(BOOL bEnable)
{
	ListView_EnableGroupView(m_hHandle,bEnable);
}

void RListView::SetFullRowSelect(BOOL bEnable)
{
	ListView_SetExtendedListViewStyle(m_hHandle, bEnable ? 
		ListView_GetExtendedListViewStyle(m_hHandle) | LVS_EX_FULLROWSELECT : 
		ListView_GetExtendedListViewStyle(m_hHandle) | ~LVS_EX_FULLROWSELECT);
}

void RListView::ScrollDown()
{
	RECT Rect = {LVIR_BOUNDS,0,0,0};
	if (SendMessage(m_hHandle,LVM_GETITEMRECT,0,(LPARAM)&Rect))
	{
		int VertScroll = (Rect.bottom - Rect.top) * GetItemCount();
		SendMessage(m_hHandle,LVM_SCROLL,0,VertScroll);	
	}
	
}

int RListView::GetItemCount()
{
	return SendMessage(m_hHandle,LVM_GETITEMCOUNT,0,0);
}

void RListView::CollapseAllGroup()
{
	for (int i = 0; i < GroupItems.Length(); i++)
	{
		LVGROUP group;
		ZeroMemory(&group,sizeof(LVGROUP));
		group.cbSize = sizeof(LVGROUP);

		if (ListView_GetGroupInfoByIndex(m_hHandle,GroupItems[i].Index,(LPARAM)&group))
		{
			group.state = LVGS_COLLAPSIBLE | LVGS_COLLAPSIBLE;
			SendMessage(m_hHandle,LVM_SETGROUPINFO,GroupItems[i].Id,(LPARAM)&group);
		}
	}
}

void RListView::SetExplorerStyle()
{
	SetWindowTheme(m_hHandle, L"Explorer", NULL);
	HWND hHeader;
	if (hHeader = (HWND)SendMessage(m_hHandle,LVM_GETHEADER,0,0))
	{
		SetWindowTheme(hHeader, L"Explorer", NULL);
	}
}

int RListView::FindLastItemIndex()
{
	int nLastIndex(0);
	int gni_result(-1);
	while ((gni_result = 
		ListView_GetNextItem(m_hHandle,gni_result,LVNI_ALL)) != -1)
	{
		nLastIndex = gni_result;
	}
	return nLastIndex;
}

LRESULT RListView::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = DefaultControlProc(WM_SETFOCUS,wParam,lParam);


	int gni_result(-1);
	if ((gni_result = 
		ListView_GetNextItem(m_hHandle,gni_result,LVNI_ALL)) != -1)
	{
		SndMsg(LVM_SETSELECTIONMARK,0,gni_result);
	}

	return Result;
}