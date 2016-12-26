#ifndef _GRAPHICMANAGER_H_
#define _GRAPHICMANAGER_H_

/*
	��������������: ��� ���� ��� ������ ����������� !!1,
	������� ������������ ���...
	����������: �� �� �������� ������
*/

#include "GraphicalTypes.h"
#include "ClassicRender.h"
#include "CloudRender.h"
#include "TemperatureRender.h"
#include "ImageBuffer.h"

class GraphicManager
{
	public:
		enum GraphTypes {Classic,HotNonGrad,HotGrad} GraphType;
	private:
		ImageBuffer ClassicImage; // ����� ��� ������������� �����������
		ImageBuffer CloudGrad; // ����������� ������
		ImageBuffer CloudGradDefault; // ����������� ������ �� ��������� - ������ ��� �������� �������� �����������
		ImageBuffer CloudNonGrad; // ������� ������
	protected:
		GraphInfo tmpGrInf;
		bool CalcParam(void);
		bool CalcGraphRegion(void);
		bool CalcNormalParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints);
		bool CalcZeroDeltaYParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints);
		bool CalcZeroDeltaXParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints);
		bool CalcZeroDeltaXYParam(lpfrectangle drawrect, ffigure &InputPoints, ifigure &OutputPoints);
		bool BuildClassicImage(HDC hdc);
		bool BuildCloud(HDC hdc);
		bool IsCrossing(fpoint &p11, fpoint &p12, fpoint &p21, fpoint &p22, fpoint &CrossPoint);
		HDC GetDC(GraphTypes Type);
	public:
		GraphInfo GrInf;
		bool DrawInRegion;
		bool UsingMultiCore;
		bool AccountRayRadByAutoScale;
		// ������ ������
		RenderClassic ClassGr;
		CloudRender CloudGr;
		TemperatureRender TemperatureGr;

		GraphicManager(void);
		~GraphicManager(void);
		void StoreGrInf(void);
		void ReStoreGrInf(void);
		bool SetDrawRegion(LPRECT dr);
		bool SetGraphRegion(lpfrectangle dr);
		bool DrawScene(HDC hdc);
		bool CanDraw(void);
		bool DrawScene(HDC hdc, int xScroll, int yScroll);
		bool ReBuildAll(HDC hdc);
		bool BuildCloudOutRanging(PointsFieldFor3D *pff3d);
		bool CalculateParams(void);
		void AddXY(double x, double y);
		void AddHole(void);
		
		void ClearXY(void);
		bool EmptyXY(void);
		void ClearAll(void);

		void SetRayRadius(double NewRayRadius);
		void SetRayVoltage(double NewRayVoltage);
		
		void SetRayAmpTraffic(double NewRayAmpTraffic);
		double GetRayAmp();
		
		int GetGraphicIndent();
		void GetDrawRegion(RECT *Rect);
		int GetImageWidth(void);
		int GetImageHeight(void);
		bool CaptureImage(LPCTSTR FileName);
		void AddContrast(int Contrast);
};
#endif