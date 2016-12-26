#ifndef _BASE_RENDER_H_
#define _BASE_RENDER_H_

#include "GraphicalTypes.h"
#include "ImageBuffer.h"

/**
* \brief Contains the parameters necessary for drawing the axes.
*
* \author Rustam Z
*/
typedef struct __AXESINFO
{
	/**
	* \brief Color of the axes.
	*/
	ARGBColor Color;

	/**
	* \brief Font for axis labels
	*/
	IB_Font Font;

	/**
	* \brief Pen for axis.
	*/
	IB_Pen Pen;
} AXESINFO, *LPAXESINFO;

/**
* \brief Contains the parameters necessary for drawing the boundary.
*
* \author Rustam Z
*/
typedef struct __BOUNDARYINFO
{
	/**
	* \brief Pen for boundary.
	*/
	IB_Pen Pen;
} BOUNDARYINFO, *LPBOUNDARYINFO;

/**
* \brief Base class for other renders
* 
* \details Consist common methods for drawing simple graphics
*
* \author Rustam Z
*/
class BaseRender
{
	public:
		/**
		* \brief Draw boundary around the graphics
		*
		* \param ib Pointer to image buffer for drawing
		* \param Region Contains a drawing area
		* \param lpBoundaryInfo Contains the settings for drawing the boundaries
		*
		* \return True if successful
		*/
		bool DrawBoundary(ImageBuffer *ib, LPRECT Region, LPBOUNDARYINFO lpBoundaryInfo);

		/**
		* \brief Draw axes on the graph
		*
		* \param ib Pointer to image buffer for drawing
		* \param lpAxesInfo Contains a axes params
		* \param lpgi Contains the common settings for drawing
		*
		* \return True if successful
		*/
		bool DrawAxes(ImageBuffer *ib, LPAXESINFO lpAxesInfo,lpGraphInfo lpgi);

		/**
		* \brief Drawing of a real number in the buffer
		*
		* \param ib Pointer to image buffer for drawing
		* \param X Contains the x coordinate
		* \param Y Contains the y coordinate
		* \param Value The value for the rendering
		*
		* \return True if successful
		*/
		bool PrintNumber(ImageBuffer *ib, long X, long Y, double Value);
		
		/**
		* \brief Converting real number into a string
		*
		* \param value Value for the convert
		*
		* \return String value of the param
		*/
		gstring DoubleToGString(double value);
};

#endif