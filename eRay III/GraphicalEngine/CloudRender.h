#ifndef _BASECLOUDRENDER_H
#define _BASECLOUDRENDER_H

#include "BaseRender.h"
#include "CloudField.h"

typedef CloudField <double> PointsFieldFor3D;

typedef CloudField <double> tBaseCloudDump;

/**
* \brief Encapsulates node params for cloud grid.
*
* \author Rustam Z
*/
struct GridNodeInfo
{
	/**
	* \brief The overall share of power at the node.
	*/
	double Percent;

	/**
	* \brief The Power at the node.
	*/
	double Power;
};

typedef std::vector <std::vector <GridNodeInfo>> tThreadAccum;

/**
* \brief Render to drawing the cloud chart.
*
* \details The brightness of the color depends on the Number of power getting to the point.
* The accuracy depends on the number of input points.
* To increase speed you can use parallelization.
*
* \warning It consumes a lot of memory. Make sure that the parameters appropriate for the target machine.
*
* \author Rustam Z
*/
class CloudRender : public BaseRender
{
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		CloudRender();

		/**
		* \brief Drawing scene in ImageBuffer.
		*
		* \param ib1 The ImageBuffer for the gradient rendering.
		* \param ib2 The ImageBuffer for the rainbow-style rendering.
		* \param lpgi Contains the common settings for drawing.
		*
		* \return True if successful, false otherwise.
		*/
		bool DrawScene(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi);

		/**
		* \brief Drawing scene in ImageBuffer.
		*
		* \param ib1 The ImageBuffer for the gradient rendering.
		* \param ib2 The ImageBuffer for the rainbow-style rendering.
		* \param lpgi Contains the common settings for drawing.
		* \param CoreCount The number of processor cores to parallelize.
		*
		* \return True if successful, false otherwise.
		*/
		bool DrawScene(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi, UINT CoreCount);

		/**
		* \brief Drawing scene in ImageBuffer.
		*
		* \param lpgi Contains the common settings for drawing.
		* \param pf3d A pointer to the cloud field.
		*
		* \return True if successful, false otherwise.
		*/
		bool FillOutRanging(lpGraphInfo lpgi, PointsFieldFor3D *pf3d);

		void SetTotalEfficiency(double Value);
	private:
		// входная структура для фонового обработчика
		double Voltage;
		int PointCount;
		struct WorkerParam
		{
			double** field;
			long   xSize;
			long   ySize;
			long   xRad;
			long   yRad;
			ifigure *Points;
			size_t PointsBegin;
			size_t PointsEnd;
			HANDLE hEvent;
		};
	protected:
		bool GetMinMaxField(double *field[], long xSize, long ySize, double &Min, double &Max, long double &total_sum);
		COLORREF ColorByValue(double value); // получает цвета радуги в зависимости от интервала (0;1)
		bool DrawScaleNoGrad(ImageBuffer *ib, lpGraphInfo lpgi);
		bool DrawStepGrid(ImageBuffer *ib, lpGraphInfo lpgi);
		static void AddFieldRad(double **field, int xSize, int ySize, int X, int Y, int RadX, int RadY);
		double MomentRadMas(int RadX, int RadY);
		static void ThreadWork(void *param);
		bool DrawJewelry(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi);

		/**
		* \brief Rendering the scene in the buffer in the one streams.
		*
		* \param ib1 The ImageBuffer for the gradient rendering.
		* \param ib2 The ImageBuffer for the rainbow-style rendering.
		* \param lpgi Contains the common settings for drawing.
		*
		* \return True if successful, false otherwise.
		*/
		bool DrawSceneThread1(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi);

		/**
		* \brief Rendering the scene in the buffer in the two streams.
		*
		* \param ib1 The ImageBuffer for the gradient rendering.
		* \param ib2 The ImageBuffer for the rainbow-style rendering.
		* \param lpgi Contains the common settings for drawing.
		*
		* \return True if successful, false otherwise.
		*/
		bool DrawSceneThread2(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi);

		/**
		* \brief Rendering the scene in the buffer by the four streams.
		*
		* \param ib1 The ImageBuffer for the gradient rendering.
		* \param ib2 The ImageBuffer for the rainbow-style rendering.
		* \param lpgi Contains the common settings for drawing.
		*
		* \return True if successful, false otherwise.
		*/
		bool DrawSceneThread4(ImageBuffer *ib1, ImageBuffer *ib2, lpGraphInfo lpgi);
		void FillAccum(lpGraphInfo lpgi);
	public:
		// переменная хранит сырой дамп облака
		tBaseCloudDump BaseCloudDump;
		long double TotalPowerSum;

		// процентное соотношение
		std::vector <std::vector <double>> OutRanging;
		
		int PixelInOneStep; // кол-во пикселей в одном шаге
		tThreadAccum ThreadAccum; // 
		double TotalEfficiency; // общий кпд для модели

		BOUNDARYINFO BoundaryInfoGrad;
		BOUNDARYINFO BoundaryInfoNoGrad;

		AXESINFO AxesInfoGrad;
		AXESINFO AxesInfoNoGrad;

		// отрисовка границы
		bool vDrawBoundaryGrad;
		bool vDrawBoundaryNoGrad;

		// отриосвка рамки
		bool vDrawBorderGrad;
		bool vDrawBorderNoGrad;

		// отрисовка осей
		bool vDrawAxesGrad;
		bool vDrawAxesNoGrad;

		// отрисовка ленды
		bool vDrawScaleNoGrad;

		// отрисовка шаговой сетки
		bool vDrawStepGrid;
};


#endif