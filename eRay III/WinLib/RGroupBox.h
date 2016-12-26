#ifndef _CGROUPBOX_H_
#define _CGROUPBOX_H_

#include "RControl.h"

namespace RClasses {

class RGroupBox : public RControl
{
	public:
		RGroupBox();
		~RGroupBox();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
};

} // namespace RClasses

#endif