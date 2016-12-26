#ifndef _RTRACKBAR_H_
#define _RTRACKBAR_H_

#include "RControl.h"

namespace RClasses {

class RTrackBar : public RControl
{
	public:
		RTrackBar();
		~RTrackBar();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		void SetRange(WORD Min, WORD Max);
		void SetTrackPos(INT Pos);
		int GetTrackPos();
	protected:
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
};

}

#endif