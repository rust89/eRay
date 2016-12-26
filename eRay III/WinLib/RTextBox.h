#ifndef _CTEXTBOX_H_
#define _CTEXTBOX_H_

#include "RControl.h"

namespace RClasses {

class RTextBox : public RControl
{
	protected:
		RString m_szBackgroundText;
		HFONT m_hBackgoundFont;
	public:
		RTextBox();
		~RTextBox();
		
		bool Create(LPCTSTR Text, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool Create(UINT Style, RCustomWindow *p_wndParent, UINT uiID, int x = 0, int y = 0, int width = 0,int height = 0);
		bool IsMultiLine();
		LRESULT GetLineCount();
		LRESULT GetLineLength(LRESULT LineIndex);
		void AddLine(RString Line);
		void AddText(RString Text);
		void SetLimit(UINT Count);
		void ScrollTextEnd();
		void Clear();
		int GetCaretIndex();
		void SetCaretPos(int CaretIndex);
		void SetBackgroundText(LPCTSTR Str);
	protected:
		virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);	
		virtual LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
};

} // namespace RClasses

#endif