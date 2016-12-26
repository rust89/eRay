#ifndef _RLISTVIEW_H_
#define _RLISTVIEW_H_

#include "RControl.h"
#include "RArray.h"

namespace RClasses {

/*
 неполный контрол
*/

struct RGroupItem 
{
	int Index;
	int Id;
};

typedef RArray <RGroupItem> RGroupItems;

class RListView : public RControl
{
	protected:
		RArray <int> Columns;
		RGroupItems GroupItems;

		// возвращает индекс последнего элемента
		int FindLastItemIndex();
	public:
		RListView();
		~RListView();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool Create(RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);

		int AddItem(LPLVITEM lplvi);
		int AddItem(LPCTSTR Text);
		int AddItem(LPCTSTR Text, int GroupID);
		int CreateColumn(LPCTSTR Text, int Width);
		int AddGroup(LPCTSTR Text);
		int GetItemCount();
		void SetItemText(int ItemIndex, int SubItemIndex, LPCTSTR Text);
		void SetExListViewStyle(DWORD Style);
		void ClearList();

		enum ListViewStates {lvsList,lvsReport,lvsIcon};
		void SetListViewState(ListViewStates State);
		void SetGroupEnable(BOOL bEnable);
		void SetFullRowSelect(BOOL bEnable);
		void ScrollDown();
		void CollapseAllGroup();
		// установка стил€ проводника дл€ списка
		// дл€ работы данного метода неопходимо подключить UxTheme.lib
		void SetExplorerStyle();
	protected:
		LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
};

} // namespace RClasses

#endif