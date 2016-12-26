#ifndef _TEMPERATUREFIELD_H_
#define _TEMPERATUREFIELD_H_

/**
* \brief Class for simple cloud manage. It encapsulates the two-dimensional array.
*
* This class is similar to std::vector, but in contrast, is optimized for fast operation.
*/
class TemperatureField
{
	protected:
		/**
		* \brief A pointer to an array of data.
		*/
		double ***m_Data;

		/**
		* \brief The Length of the object.
		*/
		int m_Length;		

		
		/**
		* \brief The Width of the object.
		*/
		int m_Width;

		/**
		* \brief The size of the array height.
		*/
		int m_Height;
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		TemperatureField();

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param Length The size of the array length.
		* \param Width The size of the array width.
		* \param Height The size of the array height.
		*/
		TemperatureField(int Length, int Width, int Height);

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param Copy Cloned object
		*/
		TemperatureField(const TemperatureField &Copy);

		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~TemperatureField();

		/**
		* \brief The GetLength method return the length of the array.
		*
		* \return Length of the array.
		*/
		const int GetLength();

		/**
		* \brief The GetWidth method return the width of the array
		*
		* \return Width of the array
		*/
		const int GetWidth();

		/**
		* \brief The GetHeight method return height of the array
		*
		* \return Height of the array
		*/
		const int GetHeight();

		/**
		* \brief Cleaning of the memory.
		*/
		void Clear();

		/**
		* \brief Realloc array memory. Old data not save.
		*
		* \param Width The size of the array width.
		* \param Height The size of the array height.
		*/
		void ReSize(int Length, int Width, int Height);

		/**
		* \brief Access to the element at index.
		*
		* \param i The index of the array by length.
		* \param j The index of the array by width.
		* \param k The index of the array by height.
		*
		* \return The reference to the element.
		*/
		double& Index(int i, int j, int k);

		TemperatureField& operator = (const TemperatureField &Right);
};

#endif