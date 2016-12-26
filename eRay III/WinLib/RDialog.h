#ifndef _RDIALOG_H_
#define _RDIALOG_H_

#include "RWindow.h"

namespace RClasses {

class RDialog : public RWindow
{
	public:
		RDialog();
		virtual ~RDialog();
	protected:
		static INT_PTR WINAPI DialogProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};


} // namespace RClasses

#endif