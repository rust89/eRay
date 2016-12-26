#ifndef _RENDER_VIEW_H_
#define _RENDER_VIEW_H_

#include <Windows.h>
#include <tchar.h>

class RenderView
{
	private:
		HWND m_Handle;
		ATOM WindowClassAtom;

		static LRESULT CALLBACK _WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	protected:
		LRESULT wmCreate(LPCREATESTRUCT lpcs);
		LRESULT wmSize(WPARAM ResizingRequest, INT NewClientWidth, INT NewClientHeight);
	public:
		RenderView();
		virtual ~RenderView();

		int Create(HWND hOwner, LPCTSTR pWindowName);
		void Show();
		void Hide();
		void SetPosition(int x, int y, int width, int height);
};

#endif