#ifndef _IMAGE_BUFFER_OBJECTS_H_
#define _IMAGE_BUFFER_OBJECTS_H_

#include <Windows.h>
#include <tchar.h>


#ifdef UNICODE
	typedef wchar_t ImageBuffer_char;
#else
	typedef char ImageBuffer_char;
#endif

typedef ImageBuffer_char* ImageBuffer_string;
typedef const ImageBuffer_char* ImageBuffer_cstring;

typedef unsigned int ARGBColor; // цвет

/**
* \brief Base class for ImageBuffer objects
* 
* Class encapsulates the standard work gdi objects
*/
class Base_ImageBuffer_Object
{
	protected:
		/**
		* \brief Contains a handle to an abstract graphic object
		*/
		HGDIOBJ m_hHandle;
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		Base_ImageBuffer_Object();

		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~Base_ImageBuffer_Object();

		/**
		* \brief Return the native GDI object
		*
		* \return GDI object
		*/
		const HGDIOBJ GetNativeObject() const;
		
		/**
		* \brief Destroy native GDI object
		*/
		void Clear();
};

/**
* \brief Class encapsulates work with Font objects
*/
class IB_Font : public Base_ImageBuffer_Object
{
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		IB_Font();

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param FontName Font family
		* \param size The font size
		*/
		IB_Font(ImageBuffer_cstring FontName, int size);
		
		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~IB_Font();

		/**
		* \brief Reinitializing the object with new parameters
		*
		* \param FontName Font family
		* \param size The font size
		*/
		void Reset(ImageBuffer_cstring FontName, int size);
};

/**
* \brief Class encapsulates work with Pen objects
*/
class IB_Pen : public Base_ImageBuffer_Object
{
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		IB_Pen();

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param fnPenStyle The pen style. It can be any one of the following values.
		*  - PS_SOLID : The pen is solid
		*  - PS_DASH : The pen is dashed. This style is valid only when the pen width is one or less in device units.
		*  - PS_DOT : The pen is dotted. This style is valid only when the pen width is one or less in device units.
		*  - PS_DASHDOT : The pen has alternating dashes and dots. This style is valid only when the pen width is one or less in device units.
		*  - PS_DASHDOTDOT : The pen has alternating dashes and double dots. This style is valid only when the pen width is one or less in device units.
		*  - PS_NULL : The pen is invisible.
		*  - PS_INSIDEFRAME : The pen is solid. When this pen is used in any GDI drawing function that takes a bounding rectangle, the dimensions of the figure are shrunk so that it fits entirely in the bounding rectangle, taking into account the width of the pen. This applies only to geometric pens.
		* \param nWidth The width of the pen, in logical units. If nWidth is zero, the pen is a single pixel wide, regardless of the current transformation.
		* \param crColor A color reference for the pen color.
		*/
		IB_Pen(int fnPenStyle, int nWidth, ARGBColor crColor);

		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~IB_Pen();

		/**
		* \brief Reinitializing the object with new parameters
		*
		* \param fnPenStyle The pen style. It can be any one of the following values.
		*  - PS_SOLID : The pen is solid
		*  - PS_DASH : The pen is dashed. This style is valid only when the pen width is one or less in device units.
		*  - PS_DOT : The pen is dotted. This style is valid only when the pen width is one or less in device units.
		*  - PS_DASHDOT : The pen has alternating dashes and dots. This style is valid only when the pen width is one or less in device units.
		*  - PS_DASHDOTDOT : The pen has alternating dashes and double dots. This style is valid only when the pen width is one or less in device units.
		*  - PS_NULL : The pen is invisible.
		*  - PS_INSIDEFRAME : The pen is solid. When this pen is used in any GDI drawing function that takes a bounding rectangle, the dimensions of the figure are shrunk so that it fits entirely in the bounding rectangle, taking into account the width of the pen. This applies only to geometric pens.
		* \param nWidth The width of the pen, in logical units. If nWidth is zero, the pen is a single pixel wide, regardless of the current transformation.
		* \param crColor A color reference for the pen color.
		*/
		void Reset(int fnPenStyle, int nWidth, ARGBColor crColor);
};

/**
* \brief Class encapsulates work with Brush objects
*/
class IB_Brush : public Base_ImageBuffer_Object
{
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		IB_Brush();

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param crColor A color reference for the brush color.
		*/
		IB_Brush(ARGBColor crColor);

		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~IB_Brush();
		
		/**
		* \brief Reinitializing the object with new parameters
		*
		* \param crColor A color reference for the brush color.
		*/
		void Reset(ARGBColor crColor);
};

#endif