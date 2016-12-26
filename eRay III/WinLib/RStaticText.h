#ifndef _RSTATICTEXT_H_
#define _RSTATICTEXT_H_


#include "RControl.h"

namespace RClasses {

/*
 неполный контрол
*/

class RStaticText : public RControl
{
	public:
		RStaticText();
		~RStaticText();
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool Create(UINT Style, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool Create(LPCTSTR Text, UINT Style, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0); 
};

} // RClasses

#endif