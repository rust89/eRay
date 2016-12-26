#ifndef _RANGING_FRAME_H
#define _RANGING_FRAME_H

#include "WinLib\WinLibCore.h"

using namespace RClasses;

// временнон решение 
#include <vector>
using namespace std;

#include "GraphicalEngine\ImageBuffer.h"

typedef RArray <double> RangingPoints;

class RRangingFrame : public RFrame
{
	public:
		RRangingFrame(void);
		~RRangingFrame(void);
		void CreateImage(RangingPoints *field, int BeginSel, int EndSel);
		void SetLegendText(LPCTSTR Text);
		void SetHorText(LPCTSTR Text);
		void ClearParams(void);
	protected:
		HDC m_hDc;
		int memDcWidth;
		int memDcHeight;

		ImageBuffer m_cScene;
		ImageBuffer m_cGraph;

		RString m_strLegend;
		RString m_strHorText;

		void CreateDrawGraph(RangingPoints *Points, int SelectorBegin, int SelectorEnd);
		void CreateDrawScene();

		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
};

#endif