#ifndef _CLOUDFIELD_H_
#define _CLOUDFIELD_H_

/**
* \brief Class for simple cloud manage. It encapsulates the two-dimensional array.
*
* This class is similar to std::vector, but in contrast, is optimized for fast operation.
*/
template <class Type> class CloudField
{
	protected:
		/**
		* \brief A pointer to an array of data.
		*/
		Type **m_Data;

		/**
		* \brief The size of the array width.
		*/
		int m_Width;

		/**
		* \brief The size of the array height.
		*/
		int m_Height;

		/**
		* \brief The size of the array height.
		*/
		double m_PixelWeight;
	public:
		/**
		* \brief The default constructor
		*
		* Use it to initialize class fields with default values
		*/
		CloudField()
		{
			m_Data = 0;
			m_Width = 0;
			m_Height = 0;
		}

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param Width The size of the array width.
		* \param Height The size of the array height.
		*/
		CloudField(int Width, int Height)
		{
			m_Width = Width;
			m_Height = Height;
			m_Data = new Type*[m_Width];
			for (int i = 0; i < m_Width; i++)
			{
				m_Data[i] = new Type[m_Height];
			}
		}

		/**
		* \brief The constructor
		*
		* Use it to initialize class fields with custom params
		* \param Copy Cloned object
		*/
		CloudField(const CloudField &Copy)
		{
			m_Width = Copy.m_Width;
			m_Height = Copy.m_Height;
			m_PixelWeight = Copy.m_PixelWeight;
			m_Data = new Type*[m_Width];
			for (int i = 0; i < m_Width; i++)
			{
				m_Data[i] = new Type[m_Height];
				for (int j = 0; j < m_Height; j++)
				{
					m_Data[i][j] = Copy.m_Data[i][j];
				}
			}
		}

		/**
		* \brief The class destructor
		*
		* Use it to destroy object
		*/
		~CloudField()
		{
			Clear();
		}

		/**
		* \brief The GetWidth method return width of the array
		*
		* \return Width of the array
		*/
		const int GetWidth() const
		{
			return m_Width;
		}

		/**
		* \brief The GetHeight method return height of the array
		*
		* \return Height of the array
		*/
		const int GetHeight() const
		{
			return m_Height;
		}

		/**
		* \brief The GetPixelWeight method return Weight of the pixel for this array.
		*
		* \return Weight of the pixel
		*/
		const double GetPixelWeight() const
		{
			return m_PixelWeight;
		}

		/**
		* \brief The GetPixelWeight set the new Weight of the pixel for the array.
		*/
		void SetPixelWeight(double PixelWeight) 
		{
			m_PixelWeight = PixelWeight;
		}

		/**
		* \brief Cleaning of the memory.
		*/
		void Clear()
		{
			for (int i = 0; i < m_Width; i++)
			{
				delete [] m_Data[i];
			}
			delete [] m_Data;

			m_Data = 0;
			m_Width = 0;
			m_Height = 0;
		}

		/**
		* \brief Realloc array memory. Old data not save.
		*
		* \param Width The size of the array width.
		* \param Height The size of the array height.
		*/
		void ReSize(int Width, int Height)
		{
			Clear();
			m_Width = Width;
			m_Height = Height;
			m_Data = new Type*[m_Width];
			for (int i = 0; i < m_Width; i++)
			{
				m_Data[i] = new Type[m_Height];
			}			
		}

		/**
		* \brief Access to the element at index.
		*
		* \param i The index of the array by width.
		* \param j The index of the array by height.
		*
		* \return The reference to the element.
		*/
		Type& Index(int i, int j)
		{
			return m_Data[i][j];
		}


		CloudField& operator = (const CloudField &Right)
		{
			if (this == &Right)
				return *this;
			Clear();
			m_Width = Right.m_Width;
			m_Height = Right.m_Height;
			m_PixelWeight = Right.m_PixelWeight;
			m_Data = new Type*[m_Width];
			for (int i = 0; i < m_Width; i++)
			{
				m_Data[i] = new Type[m_Height];
				for (int j = 0; j < m_Height; j++)
				{
					m_Data[i][j] = Right.m_Data[i][j];
				}
			}
			return *this;
		}
};


#endif