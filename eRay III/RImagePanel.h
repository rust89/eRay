
#ifndef _RIMAGEPANEL_H_
#define _RIMAGEPANEL_H_

#include "WinLib\WinLibCore.h"
using namespace RClasses;

#include "GraphicalEngine\GraphicManager.h"

#include "AdditionalWndMessages.h"
#include "RTemperatureFrame.h"

class RImagePanel : public RFrame
{
	public:
		RImagePanel();
		~RImagePanel();
		GraphicManager GraphView;
		void UpDateScroll(int NewClientWidth, int NewClientHeight);
		void ResetSelector() {PointX = -1; PointY = -1;}
		void ClearScroll();
		void LockInterface();
		void UnLockInterface();
		void ClearImage();
		HPEN hCloudPenSelector; // ���� ��� ��������� ���������
		HPEN hCloudPenBalloon; // ���� ��� ��������� �������
		HBRUSH hCloudBrushBalloon; // ���� ��� ��������� �������
		HFONT hCloudFontBalloon; // ����� ��� �������
		int PointX;
		int PointY;
	protected:
		RString QStream;
		RString QPercent;
	// ������������� ��������
		PAINTSTRUCT m_ps;
		SCROLLINFO si;
	// ����������� ��������
		int xMaxScroll;
		int xMinScroll;
		int xCurrentScroll;

		int yMaxScroll;
		int yMinScroll;
		int yCurrentScroll;
	// �����
		BOOL fBlt; 
        BOOL fScroll; 
        BOOL fSize;
	// �������������� ������:
		// ��������� �������
		void DrawSight(HDC hdc, int X, int Y);
		// �������� ���������, ������������ �� ����� �� ��������
		void CreateClickMsg(GridNodeInfo *gni);
		// ��������� ����� ����������
		void DrawCommonInfo(HDC hdc, int X, int Y);
	// �������
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		LRESULT OnErsBckg(WPARAM wParam, LPARAM lParam);
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
		LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
		LRESULT OnVScroll(WPARAM wParam, LPARAM lParam);
		LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
		LRESULT OnClick(WPARAM wParam, LPARAM lParam);
		LRESULT OnDblClick(WPARAM wParam, LPARAM lParam);
		LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
};


#endif