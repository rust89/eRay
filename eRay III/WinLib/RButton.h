#ifndef _CBUTTON_H_
#define _CBUTTON_H_


#include "RControl.h"

namespace RClasses {

class RButton : public RControl
{
	public:
		RButton();
		~RButton();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
	protected:
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
		LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);

};

} // namespace RClasses

#endif