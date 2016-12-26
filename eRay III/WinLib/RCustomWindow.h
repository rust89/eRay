
#ifndef _CCUSTOMWINDOW_H_
#define _CCUSTOMWINDOW_H_

#include <Windows.h>
#include <CommCtrl.h>

#include "RMessageMap.h"
#include "RObject.h"

namespace RClasses {

#define MSG_EVENT_CAST(X) (CustomEvent)(&X)

class RCustomWindow : public RObject
{	
	public:
		typedef LRESULT (RCustomWindow::*CustomEvent) (WPARAM, LPARAM);

		RMessageMap <CustomEvent,RCustomWindow*> m_mMsgMap;

		RCustomWindow();
		virtual ~RCustomWindow();

		const HWND Handle() const;
		bool AddMessage(UINT Message, CustomEvent pEvent, RCustomWindow* pEventOwner);

		RString GetText();
		LONG GetStyle();
		LONG GetExStyle();
		LONG GetWidth();
		LONG GetHeight();
		LONG GetClientWidth();
		LONG GetClientHeight();
		BOOL GetClientRect(RECT& Rect);
		BOOL GetWindowRect(RECT& Rect);
		RCustomWindow* GetParent();
		LONG_PTR GetClassLong(int Index);

		bool IsEnabled();

		void SetText(LPCTSTR Text);
		void SetStyle(LONG Style);
		void SetExStyle(LONG ExStyle);
		void SetClassLong(int Index, LONG_PTR dwNewLong);
		void SetWindowPosition(HWND hWndInsertAfter, int x, int y, int width, int height, UINT uFlags);
		void BringWindowToTop();
		void SetParent(RCustomWindow *p_wndParent);
		void SetFocus();
		void Move(int x, int y, int width, int height, BOOL bRepaint);
		
		LRESULT SndMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
		
		/** 
		* �������������� ����������� ���� ��� ������������
		*/
		void SetEnable(BOOL State);

		/** 
		* ��������� ����������� ���������
		*/
		void AddToolTip(LPTSTR pText);
		
		/** 
		* �������� �������������� ����������� ����
		*/
		void Refresh();
		/**
		* ���������� ����
		*/
		void DestroyWindow();

		/**
		* ���������� ��� �������� ������
		*/
		const RString GetClassName() const;

		/** 
		* ������� ���������� ��������� ���������� tab
		* ��� ����� ��������. 
		* ���������� ������������ �� ����� ������� ������� ��������.
		*/
		virtual void TabManager(RCustomWindow *Sender);
	protected:
		HWND m_hHandle;
		RString m_WindowClassName;
};


} // namespace RClasses


#endif