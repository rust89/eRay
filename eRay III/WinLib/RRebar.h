#ifndef _RREBAR_H
#define _RREBAR_H

#include "RControl.h"

namespace RClasses { 

class RRebar : public RControl
{
	public:
		RRebar();
		~RRebar();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);

};

} // namespace RClasses


#endif