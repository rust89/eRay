#ifndef _CFRAME_H_
#define _CFRAME_H_

#include "RWindow.h"
#include "RArray.h"

namespace RClasses {

class RFrame : public RWindow
{
	public:
		RFrame();
		virtual ~RFrame();
		bool Create(DWORD ExStyle, LPCTSTR ClassName, RString Caption, 
	        DWORD Style, 
			RCustomWindow *pOwner, int x, int y,
			int width,int height, HMENU hMenu);
		static bool RegisterWndClass(LPCTSTR ClassName, UINT Style, HBRUSH hBrush, HCURSOR hCursor);
		static bool UnRegisterWndClass(LPCTSTR ClassName);
		
		virtual void TabManager(RCustomWindow *Sender);
	protected:
		RArray <RCustomWindow *> TabList;
		static LRESULT CALLBACK _WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

} // namespace RClasses

#endif