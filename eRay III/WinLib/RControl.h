#ifndef _CCONTROL_H
#define _CCONTROL_H

#include "RCustomWindow.h"

#include <CommCtrl.h>

namespace RClasses {

class RControl : public RCustomWindow
{
	protected:
		WNDPROC ControlBaseProc;
	protected:
		RString ClassName;
		static LRESULT WINAPI ControlProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam);

		bool Create(LPCTSTR Text, LPCTSTR BaseClassName, LPCTSTR ClassName, UINT Style, UINT ExStyle, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
	public:
		RControl();
		virtual ~RControl();
		virtual bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0) = 0;
		LRESULT DefaultControlProc(UINT Message, WPARAM wParam, LPARAM lParam);

		void SetFont(HFONT hFont);		
};

}

#endif