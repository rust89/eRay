#ifndef _RARRAY_H_
#define _RARRAY_H_

namespace RClasses {

template <class Type> class RArray
{
	protected:
		Type *m_pData;
		int m_vSize;
		int m_vCapacity;

		int GetDelta(int NewSize);
		void GrowCapacity(int NewCapacity);

		void ShiftRight(int BeginIndex, int StepCount);
		void ShiftLeft(int BeginIndex, int StepCount);
	public:
		RArray();
		RArray(int Size);
		~RArray();

		void Clear(void);
		const int Length(void) const;

		RArray <Type> & Assign(const RArray& Source);
		RArray <Type> & Append(const RArray& Source);
		RArray <Type> & Append(const Type& Source);
		void Insert(int BeginIndex, const RArray& SubStr);
		void Erase(int BeginIndex, int Count);
		void Resize(int NewSize);

		RArray <Type> & operator = (const RArray& Source);
		RArray <Type> & operator += (const RArray& Source);
		RArray <Type> & operator += (const Type& Source);
		RArray <Type> operator + (const RArray& Other);
		Type& operator [] (int Index);
		bool operator == (const RArray& Other);
};

template <class Type> RArray <Type> ::RArray()
{
	m_vSize = 0;
	m_vCapacity = GetDelta(m_vSize);
	m_pData = new Type[m_vCapacity];
}

template <class Type> RArray <Type> ::RArray(int Size)
{
	m_vSize = Size;
	m_vCapacity = m_vSize + GetDelta(m_vSize);
	m_pData = new Type[m_vCapacity];
}

template <class Type> RArray <Type> ::~RArray()
{
	Clear();
}

template <class Type> RArray <Type> & RArray <Type> ::Assign(const RArray& Source)
{
	if (&Source == this) return *this;
	if (Source.m_vSize > m_vCapacity)
		GrowCapacity(m_vSize + Source.m_vSize);
	for (int i = 0; i < Source.m_vSize; i++)
		m_pData[i] = Source.m_pData[i];
	return *this;
}

template <class Type> RArray <Type> & RArray <Type> ::Append(const RArray& Source)
{
	if (Source.m_vSize)
	{
		int NewSize = m_vSize + Source.m_vSize;
		if (NewSize >= m_vCapacity)
			GrowCapacity(NewSize + GetDelta(NewSize));
		for (int i = m_vSize, j = 0; j < Source.m_vSize; j++, i++)
			m_pData[i] = Source.m_pData[j];
		m_vSize = NewSize;
	}
	return *this;
}

template <class Type> RArray <Type> & RArray <Type> ::Append(const Type& Source)
{
	int NewSize = m_vSize + 1;
	if (NewSize >= m_vCapacity)
		GrowCapacity(NewSize + GetDelta(NewSize));
	m_pData[m_vSize] = Source;
	m_vSize = NewSize;
	return *this;
}

template <class Type> const int RArray <Type> ::Length(void) const
{
	return m_vSize;
}

template <class Type> void RArray <Type> ::Clear(void)
{

	if (m_pData)
	{
		delete [] m_pData;
		m_pData = 0;
		m_vSize = 0;
		m_vCapacity = 0;
	}
}

template <class Type> void RArray <Type> ::GrowCapacity(int NewCapacity)
{
	if (NewCapacity >= m_vCapacity)
	{
		Type *NewData = new Type[NewCapacity];
		for (int i = 0; i < m_vSize; i++)
			NewData[i] = m_pData[i];
		delete [] m_pData;
		m_pData = NewData;
		m_vCapacity = NewCapacity;
	}
}

template <class Type> int RArray <Type> ::GetDelta(int NewSize)
{
	if (NewSize > 1000)
		return 200;
	else if (NewSize > 100)
		return 100;
	return 50;
}

template <class Type> void RArray <Type> ::ShiftLeft(int BeginIndex, int StepCount) // +
{
	if (BeginIndex < 0 || BeginIndex > m_vSize) return;
	int NewSize = m_vSize - StepCount;
	if (NewSize < 0) 
	{
		NewSize = 0;
		m_vSize = NewSize;
		return;
	}
	for (int i = BeginIndex; i < m_vSize; i++)
	{
		int Index = i - StepCount;
		if (Index >= 0)
			m_pData[Index] = m_pData[i];
	}
	m_vSize = NewSize;
}

template <class Type> void RArray <Type> ::ShiftRight(int BeginIndex, int StepCount) // +
{
	if (BeginIndex < 0 || BeginIndex > m_vSize) return;
	int NewSize = m_vSize + StepCount;
	if (NewSize >= m_vCapacity)
		GrowCapacity(NewSize + GetDelta(NewSize));
	for (int i = m_vSize - 1; i >= BeginIndex; i--)
	{
		m_pData[i+StepCount] = m_pData[i];
	}
	m_vSize = NewSize;
}

template <class Type> void RArray <Type> ::Insert(int BeginIndex, const RArray& SubStr) // +
{
	ShiftRight(BeginIndex,SubStr.m_vSize);
	for (int i = 0, j = BeginIndex; i < SubStr.m_vSize; i++,j++)
	{
		m_pData[j] = SubStr.m_pData[i];
	}
}

template <class Type> void RArray <Type> ::Erase(int BeginIndex, int Count) // +
{
	if (BeginIndex < 0 || BeginIndex >= m_vSize) return;
	int tmpBeginIndex = BeginIndex + Count;
	if (tmpBeginIndex >= m_vSize) 
	{	
		tmpBeginIndex = m_vSize;
		Count = m_vSize - BeginIndex;
	}
	ShiftLeft(tmpBeginIndex, Count);
}

template <class Type> RArray <Type> & RArray <Type> ::operator = (const RArray& Source)
{
	return Assign(Source);
}

template <class Type> RArray <Type> & RArray <Type> ::operator += (const RArray& Source)
{
	return Append(Source);
}

template <class Type> RArray <Type> & RArray <Type> ::operator += (const Type& Source)
{
	return Append(Source);
}

template <class Type> RArray <Type> RArray <Type> ::operator + (const RArray& Other)
{
	return RArray(*this).Append(Other);
}

template <class Type> Type& RArray <Type> ::operator [] (int Index)
{
	return m_pData[Index];
}

template <class Type> bool RArray <Type> ::operator == (const RArray& Other)
{
	if (this == &Other) return true;
	if (Other.m_vSize != m_vSize) return false;

	for (int i = 0; i < m_vSize; i++)
		if (Other.m_pData[i] != m_pData[i])
			return false;
	return true;
}

template <class Type> void RArray <Type> ::Resize(int NewSize)
{
	Clear();
	m_vSize = NewSize;
	m_vCapacity = m_vSize + GetDelta(m_vSize);
	m_pData = new Type[m_vCapacity];
}

}

#endif