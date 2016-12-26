#ifndef _RCOMBOBOX_H_
#define _RCOMBOBOX_H_


#include "RControl.h"

namespace RClasses { 

class RComboBox : public RControl
{
	public:
		RComboBox();
		~RComboBox();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);

		int AddString(LPCTSTR Text);
		int AddString(LPCTSTR Text, LPVOID lpData);
		int GetSelectedIndex();
		void SetSelectedIndex(int Index);
		LPVOID GetItemData(int Index);
};

} // namespace RClasses

#endif