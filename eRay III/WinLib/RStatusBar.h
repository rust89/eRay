#ifndef _CSTATUS_BAR_H_
#define _CSTATUS_BAR_H_

#include "RControl.h"

namespace RClasses {

/*
 неполный контрол
*/

class RStatusBar : public RControl
{
	public:
		RStatusBar();
		~RStatusBar();

		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, bool SizeGrip, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);

		void SetSimple(bool Simple);
		bool IsSimple();
		void SetParts(const LPINT Parts, BYTE Size);
		void SetPanelText(BYTE Index, LPCTSTR Text);
		void AutoSize();
};

} // namespace RClasses

#endif