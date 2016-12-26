#ifndef _RPROGRESSBAR_H_
#define _RPROGRESSBAR_H_

#include "RControl.h"

namespace RClasses {

class RProgressBar : public RControl
{
	public:
		RProgressBar();
		~RProgressBar();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool Create(DWORD Style, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0, int height = 0);

		void SetMinMax(WORD Min, WORD Max);
		void SetPosition(DWORD Position);

	protected:
};

} // namespace RClasses

#endif