#ifndef _IMAGE_BUFFER_H_
#define _IMAGE_BUFFER_H_

#include "ImageBufferObjects.h"

/**
* \brief Encapsulates a GDI drawing surface.
* 
* The ImageBuffer class provides methods for drawing objects to the display device. A ImageBuffer object is associated with a specific device context.
*/
class ImageBuffer
{
	private:
		/**
		* \brief Contains the new compatible bitmap
		*/
		HBITMAP m_hNewBitmap;

		/**
		* \brief Contains the old compatible bitmap
		*/
		HBITMAP m_hOldBitmap;
	protected:
		/**
		* \brief Contains a handle of the device context
		*/
		HDC m_hDc;

		/**
		* \brief Contains drawing surface width
		*/
		long m_vWidth;

		/**
		* \brief Contains drawing surface height
		*/
		long m_vHeight;
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		ImageBuffer();

		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~ImageBuffer();

		/**
		* \brief Create ImageBuffer from Copy
		*
		* \param Copy Cloned object
		*/
		bool Copy(const ImageBuffer& Copy);

		/**
		* \brief Cleaning of graphics memory.
		*/
		void Clear();

		/**
		* \brief Return the native handle dc
		*
		* \return HDC object
		*/
		HDC GetDC() const;

		/**
		* \brief This method creates a memory device context (DC) compatible with the specified device.
		*
		* \param hdc Handle to an existing device context.
		* \param Width GDI drawing surface width
		* \param Height GDI drawing surface height
		*
		* \return True if successful
		*/
		bool CreateCompatible(HDC hdc, long Width, long Height);

		/**
		* \brief This method creates a memory device context (DC) compatible with the specified device.
		*
		* \param hdc Handle to an existing device context.
		* \param hBitMap Handle to the bitmap source
		*
		* \return True if successful
		*/
		bool CreateCompatible(HDC hdc, HBITMAP hBitMap);

		/**
		* \brief This method creates a memory device context (DC) compatible with the specified device.
		*
		* \param ib ImageBuffer for existing device context.
		* \param Width GDI drawing surface width
		* \param Height GDI drawing surface height
		*
		* \return True if successful
		*/
		bool CreateCompatible(ImageBuffer &ib, long Width, long Height);
		
		// The SetPixel function sets the pixel at the specified coordinates to the specified color.
		/**
		* \brief The SetPixel method sets the pixel at the specified coordinates to the specified color.
		*
		* \param x The x-coordinate, in logical units, of the point to be set.
		* \param y The y-coordinate, in logical units, of the point to be set.
		* \param Color The color to be used to paint the point.
		*/
		void SetPixel(int x, int y, ARGBColor Color);
		
		/**
		* \brief The method updates the current position to the specified point. 
		*
		* \param x Specifies the x-coordinate, in logical units, of the new position, in logical units.
		* \param y Specifies the y-coordinate, in logical units, of the new position, in logical units.
		*
		* \return True if successful
		*/
		bool MoveTo(int x, int y);

		/**
		* \brief The Method draws a line from the current position up to, but not including, the specified point.
		*
		* \param x Specifies the x-coordinate, in logical units, of the new position, in logical units.
		* \param y Specifies the y-coordinate, in logical units, of the new position, in logical units.
		*
		* \return True if successful
		*/
		bool LineTo(int x, int y);

		/**
		* \brief The Method draws a line from the one position to another.
		*
		* \param x1 Specifies the x-coordinate, in logical units, of the first position, in logical units.
		* \param y1 Specifies the y-coordinate, in logical units, of the first position, in logical units.
		* \param x2 Specifies the x-coordinate, in logical units, of the second position, in logical units.
		* \param y2 Specifies the y-coordinate, in logical units, of the second position, in logical units.
		*
		* \return True if successful
		*/
		bool DrawLine(int x1, int y1, int x2, int y2);
		
		/**
		* \brief The FillRect Method fills a rectangle by using the specified brush. 
		*
		* This method includes the left and top borders, but excludes the right and bottom borders of the rectangle.
		*
		* \param Rect A pointer to a RECT structure that contains the logical coordinates of the rectangle to be filled.
		* \param Brush A pointer to the brush object used to fill the rectangle.
		*
		* \return True if successful
		*/
		bool FillRect(LPRECT Rect, IB_Brush *Brush);

		/**
		* \brief The DrawText method writes a character string at the specified location, using the currently selected font, background color, and text color.
		*
		* \param x The x-coordinate, in logical coordinates, of the reference point that the system uses to align the string.
		* \param y The y-coordinate, in logical coordinates, of the reference point that the system uses to align the string.
		* \param Text A pointer to the string to be drawn.
		*
		* \return True if successful
		*/		
		bool DrawText(int x, int y, LPCTSTR Text);
		
		/**
		* \brief The SetBkMode method sets the background mix mode. The background mix mode is used with text, hatched brushes, and pen styles that are not solid lines. 
		*
		* \param Mode The background mode. This parameter can be one of the following values.
		*  - OPAQUE : Background is filled with the current background color before the text, hatched brush, or pen is drawn.
		*  - TRANSPARENT : Background remains untouched.
		*
		* \return If the function succeeds, the return value specifies the previous background mode. If the function fails, the return value is zero.
		*/	
		int SetBkMode(int Mode);
		// повышает контраст изображения на величину Value [-100; 100] в %

		/**
		* \brief The Contrast method sets the cotrast value between -100 and 100 percent.
		*
		* \param Value Percent value of the contrast that will be add.
		*/
		void Contrast(int Value);
		
		/**
		* \brief The SetFont method selects an Font object. 
		*
		* The new Font object replaces the previous Font object.
		*
		* \param Font A pointer to the Font object to be selected.
		*/		
		void SetFont(IB_Font *Font);
		
		/**
		* \brief The SetPen method selects an Pen object. 
		*
		* The new Pen object replaces the previous Pen object.
		*
		* \param Pen A pointer to the Pen object to be selected.
		*/	
		void SetPen(IB_Pen *Pen);

		/**
		* \brief The SetBrush method selects an Brush object. 
		*
		* The new Brush object replaces the previous Brush object.
		*
		* \param Brush A pointer to the Brush object to be selected.
		*/
		void SetBrush(IB_Brush *Brush);

		/**
		* \brief The SetTextColor method sets the text color to the specified color. 
		*
		* \param Color The color of the text.
		*/
		void SetTextColor(ARGBColor Color);
		
		/**
		* \brief The GetWidth method return width of the drawing surface
		*
		* \return Width of the draw surface
		*/
		long GetWidth(void);

		/**
		* \brief The GetHeight method return height of the drawing surface
		*
		* \return Height of the draw surface
		*/
		long GetHeight(void);

		/**
		* \brief The Blacken method paints the surface with black.
		*/
		void Blacken();

		/**
		* \brief The SaveToFile method saves image to the specified file.
		*
		* \param FileName The name of the file that will be made account.
		*
		* \return True if success, zero overwise.
		*/
		bool SaveToFile(ImageBuffer_cstring FileName);
};

#endif