#ifndef _GRAPHICMANAGER_H_
#define _GRAPHICMANAGER_H_

/*
	Предупреждение: эта дура жрёт память МЕГАБАЙТАМИ !!1,
	поэтому поосторожнее тут...
	Оправдание: за то работает быстро
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
		ImageBuffer ClassicImage; // буфер для классического отображения
		ImageBuffer CloudGrad; // градиентное облако
		ImageBuffer CloudGradDefault; // градиентное облако по умолчанию - служит для хранения иходного изображения
		ImageBuffer CloudNonGrad; // цветное облако
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
		// классы редеры
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