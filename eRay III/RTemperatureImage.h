#ifndef _RTEMPERATUREIMAGE_H_
#define _RTEMPERATUREIMAGE_H_

#include "WinLib\WinLibCore.h"
using namespace RClasses;

#include "GraphicalEngine\TemperatureRender.h"

#include <map>

struct TemperatureInfo
{
	int Y;
	double Temperature;
	double Time;
};

typedef std::map <int,TemperatureInfo> TemperatureMap; 

enum eView
{
	ViewNOPE,
	ViewXY,
	ViewXYPhase,
	ViewXZ,
	ViewXZPhase,
	ViewYZ,
	ViewYZPhase,
	ViewTempLine
};

class RTemperatureImage : public RFrame
{
	private:
		eView m_eCurrentView;
		int m_nCurrentLayer;


		ImageBuffer m_cBeginView;
		ImageBuffer m_cProcView;

		int m_nSightX;
		int m_nSightY;
		bool m_bDrawSight;
		HPEN hSigthAxesPen;
		HPEN hBalloonPen;
		HBRUSH hBalloonBrush;
		HFONT hBalloonFont;

		RString TemperatureValue;
		RString TimeValue;


	// создание и уничтожение
	public:
		RTemperatureImage();
		~RTemperatureImage();

		TemperatureRender MainRender;
		ImageBuffer Image;
		TemperatureMap m_cTemp_map;

		bool SetSightDrawing(bool State);

		void SetCurrentView(eView View);
		void SetCurrentLayer(int Layer);
	protected:
		void DrawSight(HDC hdc, LPRECT DrawRect);
		void SaveCurrentImage();
		void SaveImageSeriesZ();
		void CopyDataToClipboard();
	protected:
		
		// события окна
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
		LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
		LRESULT OnMouseRDown(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
};

#endif