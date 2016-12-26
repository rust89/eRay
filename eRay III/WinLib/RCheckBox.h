#ifndef _CCHECKBOX_H_
#define _CCHECKBOX_H_


#include "RControl.h"

namespace RClasses { 

class RCheckBox : public RControl
{
	public:
		RCheckBox();
		~RCheckBox();
		UINT GetState();
		bool GetCheck();
		void SetState(UINT CheckState);
		void SetCheck(bool Check);
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
	protected:
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
};

} // namespace RClasses

#endif