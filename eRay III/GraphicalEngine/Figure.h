#ifndef _FIGURE_H_
#define _FIGURE_H_

template <class Type> class figure_point
{
	private:

		Type m_tX;
		Type m_tY;
		Type m_tZ;
		bool m_bIsHole;
	public:
		
		figure_point() : m_tX(0) , m_tY(0), m_tZ(0), m_bIsHole(false)
		{
			//
		}

		figure_point(Type X, Type Y, Type Z, bool IsHole = false)
		{
			m_tX = X;
			m_tY = Y;
			m_tZ = Z;
			m_bIsHole = IsHole;
		}

		figure_point(const figure_point& Copy)
		{
			m_tX = Copy.m_tX;
			m_tY = Copy.m_tY;
			m_tZ = Copy.m_tZ;
			m_bIsHole = Copy.m_bIsHole;
		}

		figure_point& operator = (const figure_point& Right)
		{
			if (this == &Right)
				return *this;
			m_tX = Right.m_tX;
			m_tY = Right.m_tY;
			m_tZ = Right.m_tZ;
			m_bIsHole = Right.m_bIsHole;
			return *this;
		}

		inline Type getX() const
		{
			return m_tX;
		}

		inline Type getY() const
		{
			return m_tY;
		}

		inline Type getZ() const
		{
			return m_tZ;
		}

		inline bool IsHole() const
		{
			return m_bIsHole;
		}

		void setX(Type X)
		{
			m_tX = X;
		}

		void setY(Type Y)
		{
			m_tY = Y;
		}

		void setZ(Type Z)
		{
			m_tZ = Z;
		}

		void setHole(bool IsHole)
		{
			m_bIsHole = IsHole;
		}

};


template <class Type> class figure_rectangle
{
	private:
		Type m_vMinX;
		Type m_vMinY;
		Type m_vMaxX;
		Type m_vMaxY;
	public:
		figure_rectangle()
		{
			//
		}

		figure_rectangle(Type MinX, Type MinY, Type MaxX, Type MaxY)
		{
			m_vMinX = MinX;
			m_vMinY = MinY;
			m_vMaxX = MaxX;
			m_vMaxY = MaxY;			
		}

		figure_rectangle(const figure_rectangle& Object)
		{
			m_vMinX = Object.m_vMinX;
			m_vMinY = Object.m_vMinY;
			m_vMaxX = Object.m_vMaxX;
			m_vMaxY = Object.m_vMaxY;			
		}

		figure_rectangle& operator = (const figure_rectangle& Object)
		{
			if (&Object == this)
				return *this;
			m_vMinX = Object.m_vMinX;
			m_vMinY = Object.m_vMinY;
			m_vMaxX = Object.m_vMaxX;
			m_vMaxY = Object.m_vMaxY;
			return *this;
		}

		~figure_rectangle()
		{
			//
		}

		inline Type getWidth() const
		{
			return m_vMaxX - m_vMinX;
		}

		inline Type getHeight() const
		{
			return m_vMaxY - m_vMinY;
		}

		inline Type getMinX() const
		{
			return m_vMinX;
		}

		inline Type getMinY() const
		{
			return m_vMinY;
		}

		inline Type getMaxX() const
		{
			return m_vMaxX;
		}

		inline Type getMaxY() const
		{
			return m_vMaxY;
		}

		void setMinX(Type MinX)
		{
			m_vMinX = MinX;
		}

		void setMinY(Type MinY)
		{
			m_vMinY = MinY;
		}

		void setMaxX(Type MaxX)
		{
			m_vMaxX = MaxX;
		}

		void setMaxY(Type MaxY)
		{
			m_vMaxY = MaxY;
		}

		// проверка точки на принадлежность региону
		bool InRegion(const figure_point <Type> &Point) const
		{
			if (m_vMinX <= Point.getX() && Point.getX() <= m_vMaxX)
				if (m_vMinY <= Point.getY() && Point.getY() <= m_vMaxY) return true;
				else return false;
			return false;
		}
};

#include <vector>

template <class Type> class Figure
{
	protected:
		typedef std::vector <figure_point <Type> > FigurePoints;
	private:
		FigurePoints m_cPoints;
	public:
		Figure()
		{
			//
		}

		Figure(const Figure &Copy)
		{
			m_cPoints = Copy.m_cPoints;
		}

		~Figure()
		{
			Clear();
		}

		void Clear()
		{
			m_cPoints.clear();
		}

		void AddPoint(const figure_point <Type> &Item)
		{
			m_cPoints.push_back(Item);
		}

		void AddHole()
		{
			if (!m_cPoints.empty()) // если фигура непуста€
			{
				if (!m_cPoints.back().IsHole()) // если последн€€ добавленна€ точка не дырка
				{
					figure_point <Type> Item(0,0,0,true);
					m_cPoints.push_back(Item);
				}
			}
		}

		bool Empty() const
		{
			return m_cPoints.empty();
		}

		void Resize(int NewSize)
		{
			m_cPoints.resize(NewSize);
		}

		Figure& operator = (const Figure &Right)
		{
			if (this == &Right)
				return *this;
			m_cPoints = Right.m_cPoints;
			return *this;
		}
		
		figure_point <Type>& operator[] (int index)
		{
			return m_cPoints[index];
		}

		int Count() const
		{
			return m_cPoints.size();
		}

		bool GetMinX(Type *MinX) const
		{
			if (MinX == 0 || m_cPoints.empty())
				return false;
			*MinX = m_cPoints[0].getX();
			for (std::size_t i = 1; i < m_cPoints.size(); i++)
				if (m_cPoints[i].getX() < *MinX) *MinX = m_cPoints[i].getX();
			return true;
		}

		bool GetMaxX(Type *MaxX) const
		{
			if (MaxX == 0 || m_cPoints.empty())
				return false;
			*MaxX = m_cPoints[0].getX();
			for (std::size_t i = 1; i < m_cPoints.size(); i++)
				if (m_cPoints[i].getX() > *MaxX) *MaxX = m_cPoints[i].getX();
			return true;
		}

		bool GetMinY(Type *MinY) const
		{
			if (MinY == 0 || m_cPoints.empty())
				return false;
			*MinY = m_cPoints[0].getY();
			for (std::size_t i = 1; i < m_cPoints.size(); i++)
				if (m_cPoints[i].getY() < *MinY) *MinY = m_cPoints[i].getY();
			return true;
		}

		bool GetMaxY(Type *MaxY) const
		{
			if (MaxY == 0 || m_cPoints.empty())
				return false;
			*MaxY = m_cPoints[0].getY();
			for (std::size_t i = 1; i < m_cPoints.size(); i++)
				if (m_cPoints[i].getY() > *MaxY) *MaxY = m_cPoints[i].getY();
			return true;
		}

		bool GetMinZ(Type *MinZ) const
		{
			if (MinZ == 0 || m_cPoints.empty())
				return false;
			*MinZ = m_cPoints[0].getZ();
			for (std::size_t i = 1; i < m_cPoints.size(); i++)
				if (m_cPoints[i].getZ() < *MinZ) *MinZ = m_cPoints[i].getZ();
			return true;
		}

		bool GetMaxZ(Type *MaxZ) const
		{
			if (MaxZ == 0 || m_cPoints.empty())
				return false;
			*MaxZ = m_cPoints[0].getZ();
			for (std::size_t i = 1; i < m_cPoints.size(); i++)
				if (m_cPoints[i].getZ() > *MaxZ) *MaxZ = m_cPoints[i].getZ();
			return true;
		}

		bool GetRegion(figure_rectangle <Type> *Rectangle) const
		{
			if (Rectangle == 0 || m_cPoints.empty())
				return false;

			Type MinX = m_cPoints[0].getX();
			Type MaxX = m_cPoints[0].getX();
			Type MinY = m_cPoints[0].getY();
			Type MaxY = m_cPoints[0].getY();

			for (std::size_t i = 1; i < m_cPoints.size(); i++)
			{
				if (m_cPoints[i].getX() < MinX) MinX = m_cPoints[i].getX();
				if (m_cPoints[i].getX() > MaxX) MaxX = m_cPoints[i].getX();

				if (m_cPoints[i].getY() < MinY) MinY = m_cPoints[i].getY();
				if (m_cPoints[i].getY() > MaxY) MaxY = m_cPoints[i].getY();
			}

			Rectangle->setMinX(MinX);
			Rectangle->setMaxX(MaxX);
			Rectangle->setMinY(MinY);
			Rectangle->setMaxY(MaxY);

			return true;
		}

		// вычисл€ет точку перечечени€ между двум€ пр€мыми образованными точками p11, p12 и p21, p22
		bool IsCrossing(const figure_point <Type> &p11, 
			            const figure_point <Type> &p12, 
						const figure_point <Type> &p21, 
						const figure_point <Type> &p22, 
						figure_point <Type> &CrossPoint)
		{	
			// вычисление коэффициентов дл€ первой пр€мой
			if ((p11.getX() - p12.getX()) == 0.00)
				return false;
			double A1 = (p11.getY() - p12.getY())/(p11.getX() - p12.getX());
			double C1 = (p11.getX()*p12.getY() - p12.getX()*p11.getY())/(p11.getX() - p12.getX());
			// вычисление коэффициентов дл€ второй пр€мой
			if ((p21.getX() - p22.getX()) == 0.00)
				return false;
			double A2 = (p21.getY() - p22.getY())/(p21.getX() - p22.getX());
			double C2 = (p21.getX()*p22.getY() - p22.getX()*p21.getY())/(p21.getX() - p22.getX());
			// вычисление точки пересечени€ этих двух пр€мых
			if ((A1 - A2) == 0.00)
				return false;
			CrossPoint.setX(-(C1 - C2)/(A1 - A2));
			CrossPoint.setY((A1*C2 - A2*C1)/(A1 - A2));
			return true;
		}

		bool IsCrossingRegion(const figure_rectangle <Type> &Region,
							  const figure_point <Type>  &p1, 
							  const figure_point <Type>  &p2,
							  figure_point <Type>  &CrossPoint)
		{
			// p1 - точка вне региона
			// p2 - точка внутри региона
			// –ассто€ние от "заграничной" точки до точки пересечени€
			double Length = -1;
			// точка дл€ хранени€ временного результата
			figure_point <Type> TempPoint;
			// точки образованные регионом
			figure_point <Type> 
				RegionTopLeft(Region.getMinX(),Region.getMaxY(),0), 
				RegionTopRight(Region.getMaxX(),Region.getMaxY(),0), 
				RegionBottomRight(Region.getMaxX(),Region.getMinY(),0), 
				RegionBottomLeft(Region.getMinX(),Region.getMinY(),0);
			// провер€ем левую стенку
			if (IsCrossing(p1,p2,RegionTopLeft,RegionBottomLeft,TempPoint))
			{
				double TempLenght = sqrt((p1.getX()-TempPoint.getX())*(p1.getX()-TempPoint.getX()) + (p1.getY()-TempPoint.getY())*(p1.getY()-TempPoint.getY()));
				Length = TempLenght;
				CrossPoint = TempPoint;
			}
			// провер€ем вехнюю стенку
			if (IsCrossing(p1,p2,RegionTopLeft,RegionTopRight,TempPoint))
			{
				double TempLenght = sqrt((p1.getX()-TempPoint.getX())*(p1.getX()-TempPoint.getX()) + (p1.getY()-TempPoint.getY())*(p1.getY()-TempPoint.getY()));
				if (Length != -1)
				{
					if (Length > TempLenght)
					{
						CrossPoint = TempPoint;
						Length = TempLenght;
					}
				}
				else
				{
					CrossPoint = TempPoint;
					Length = TempLenght;
				}
			}
			// провер€ем правую стенку
			if (IsCrossing(p1,p2,RegionTopRight,RegionBottomRight,TempPoint))
			{
				double TempLenght = sqrt((p1.getX()-TempPoint.getX())*(p1.getX()-TempPoint.getX()) + (p1.getY()-TempPoint.getY())*(p1.getY()-TempPoint.getY()));
				if (Length != -1)
				{
					if (Length > TempLenght)
					{
						CrossPoint = TempPoint;		
						Length = TempLenght;
					}
				}
				else
				{
					CrossPoint = TempPoint;	
					Length = TempLenght;
				}
			}
			// провер€ем нижнюю стенку
			if (IsCrossing(p1,p2,RegionBottomRight,RegionBottomLeft,TempPoint))
			{
				double TempLenght = sqrt((p1.getX()-TempPoint.getX())*(p1.getX()-TempPoint.getX()) + (p1.getY()-TempPoint.getY())*(p1.getY()-TempPoint.getY()));
				if (Length != -1)
				{
					if (Length > TempLenght)
					{
						CrossPoint = TempPoint;
						Length = TempLenght;
					}
				}
				else
				{
					CrossPoint = TempPoint;
					Length = TempLenght;
				}
			}
			if (Length != -1) return true;
			return false;
		}
		
		// производит отсечение точек не вход€щих в регион
		bool CutRegion(const figure_rectangle <Type> &Region) 
		{
			FigurePoints RegionPoints;	
			for (std::size_t i = 0; i < m_cPoints.size(); i++)
			{
				if (m_cPoints[i].IsHole()) 
				{
					// если точка €вл€етс€ дыркой
					continue;
				}
				if(Region.InRegion(m_cPoints[i])) // если точка входит в регион
				{
					if (i > 0) // возможно ли считать предыдущую точку
					{
						std::size_t LastI = i - 1;
						if (!m_cPoints[LastI].IsHole()) // если предыдуща€ точка не дырка
						{
							if(!Region.InRegion(m_cPoints[LastI])) // и не входит в регион
							{
								// определ€ем пересечение линии соедин€ющей с границей региона
								figure_point <Type> ResultPoint;
								if (IsCrossingRegion(Region, m_cPoints[i], m_cPoints[LastI], ResultPoint))
								{
									RegionPoints.push_back(ResultPoint);
								}
							} else RegionPoints.push_back(m_cPoints[i]);
						} else RegionPoints.push_back(m_cPoints[i]);
					} else RegionPoints.push_back(m_cPoints[i]);
				}
				else // если точка не входит в регион
				{
					if (i > 0) // возможно ли считать предыдущую точку
					{
						std::size_t LastI = i - 1;
						if (!m_cPoints[LastI].IsHole()) // елси предыдуща€ точка не дырка
						{
							if(Region.InRegion(m_cPoints[LastI])) // и входит в регион
							{
								// определ€ем пересечение линии соедин€ющей с границей региона
								figure_point <Type> ResultPoint;
								if (IsCrossingRegion(Region, m_cPoints[i], m_cPoints[LastI], ResultPoint))
								{
									RegionPoints.push_back(ResultPoint);
									continue;
								}
							}
						}	
					}
				}
				
			}
			
			m_cPoints = RegionPoints;
			return true;
		}
};

#endif