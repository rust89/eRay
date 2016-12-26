#ifndef _RTOOLBAR_H_
#define _RTOOLBAR_H_

#include "RControl.h"
#include "RImageList.h"

namespace RClasses { 

class RToolBar : public RControl
{
	public:
		RToolBar();
		~RToolBar();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		void AutoSize();
		void BindingImageList(RImageList &ImageList);
		void SetButtonEnable(UINT btnID, bool bEnable);
		int BtnCount();
		bool GetItemRect(int index, LPRECT lpRect);
		int GetItemWidth();
		int GetBtnWidth();
		int GetBtnHeight();
};

} // namespace RClasses

#endif