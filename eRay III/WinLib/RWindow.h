#ifndef _RWINDOW_H_
#define _RWINDOW_H_

#include "RCustomWindow.h"
#include "RArray.h"

namespace RClasses {

class RWindow : public RCustomWindow
{
	public:
		RWindow();
		virtual ~RWindow();
		void SetBigIcon(HICON hIcon);
		void SetSmallIcon(HICON hIcon);
		
		/**
		* Show window as modal dialog
		*/
		int ShowModal();
};

} // namespace RClasses

#endif