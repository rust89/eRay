#ifndef _RENDER_CLASSIC_H_
#define _RENDER_CLASSIC_H_

#include "BaseRender.h"

/**
* \brief Graphics class to draw in the classical form
* 
* Drawing as a series of points connected by lines
*/
class RenderClassic : public BaseRender
{
	protected:
		/**
		* \brief Contains the width of the node
		*/
		long NodeWidth;

		/**
		* \brief Contains a brush to draw the node
		*/
		IB_Brush NodeBrush;

		/**
		* \brief Contains a brush to draw the background
		*/
		IB_Brush BackgroudBrush;

		/**
		* \brief Holds the line with graph
		*/
		int GraphWidth;

		/**
		* \brief Contains a pen to draw the graph
		*/
		IB_Pen GraphPen;

		/**
		* \brief Contains a color to draw the graph
		*/
		ARGBColor GraphColor;
		
		/**
		* \brief Draw the node
		*
		* \param ib Pointer to image buffer for drawing
		* \param xi Contains the x coordinate
		* \param yi Contains the y coordinate
		* \param width Contains the width of the node
		*
		* \return True if successful
		*/
		bool DrawGraphNode(ImageBuffer *ib, long xi, long yi, long width);

		/**
		* \brief Draw the graph background
		*
		* \param ib Pointer to image buffer for drawing
		* \param lpgi Contains the common settings for drawing
		*
		* \return True if successful
		*/
		bool DrawBackground(ImageBuffer *ib, lpGraphInfo lpgi);

		/**
		* \brief Draw the graph
		*
		* \param ib Pointer to image buffer for drawing
		* \param lpgi Contains the common settings for drawing
		*
		* \return True if successful
		*/
		bool DrawGraph(ImageBuffer *ib, lpGraphInfo lpgi);
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		RenderClassic(void);

		/**
		* \brief Rendering the scene graph in the image buffer
		*
		* \param ib Pointer to image buffer for drawing
		* \param lpgi Contains the common settings for drawing
		*
		* \return True if successful
		*/
		bool DrawScene(ImageBuffer *ib, lpGraphInfo lpgi);
		
		/**
		* \brief Contains the parameters for drawing axes
		*/	
		AXESINFO AxesInfo;

		/**
		* \brief Contains the parameters for drawing boundary
		*/	
		BOUNDARYINFO BoundaryInfo;
		
		/**
		* \brief Flag: draw axis?
		*/
		bool vDrawAxes;

		/**
		* \brief Flag: draw boundary?
		*/
		bool vDrawBoundary;

		/**
		* \brief Flag: draw node?
		*/
		bool vDrawNode;
};

#endif