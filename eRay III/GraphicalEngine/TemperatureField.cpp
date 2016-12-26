#include "TemperatureField.h"

TemperatureField::TemperatureField()
{
	m_Data = 0;
	m_Length = 0;
	m_Width = 0;
	m_Height = 0;
}

TemperatureField::TemperatureField(int Length, int Width, int Height)
{
	m_Length = Length;
	m_Width = Width;
	m_Height = Height;
	m_Data = new double**[m_Length];
	for (int i = 0; i < m_Length; i++)
	{
		m_Data[i] = new double*[m_Width];
		for (int j = 0; j < m_Width; j++)
		{
			m_Data[i][j] = new double[m_Height];
		}
	}
}

TemperatureField::TemperatureField(const TemperatureField &Copy)
{
	m_Length = Copy.m_Length;
	m_Width = Copy.m_Width;
	m_Height = Copy.m_Height;
	m_Data = new double**[m_Length];
	for (int i = 0; i < m_Length; i++)
	{
		m_Data[i] = new double*[m_Width];
		for (int j = 0; j < m_Width; j++)
		{
			m_Data[i][j] = new double[m_Height];
			for (int k = 0; k < m_Height; k++)
			{
				m_Data[i][j][k] = Copy.m_Data[i][j][k];
			}
		}
	}
}

TemperatureField::~TemperatureField()
{
	Clear();
}

const int TemperatureField::GetLength()
{
	return m_Length;
}

const int TemperatureField::GetWidth()
{
	return m_Width;
}

const int TemperatureField::GetHeight()
{
	return m_Height;
}

void TemperatureField::Clear()
{
	for (int i = 0; i < m_Length; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			delete [] m_Data[i][j];
		}

		delete [] m_Data[i];
	}

	delete [] m_Data;

	m_Data = 0;
	m_Length = 0;
	m_Width = 0;
	m_Height = 0;
}

void TemperatureField::ReSize(int Length, int Width, int Height)
{
	Clear();
	m_Length = Length;
	m_Width = Width;
	m_Height = Height;
	m_Data = new double**[m_Length];
	for (int i = 0; i < m_Length; i++)
	{
		m_Data[i] = new double*[m_Width];
		for (int j = 0; j < m_Width; j++)
		{
			m_Data[i][j] = new double[m_Height];
		}
	}		
}

double& TemperatureField::Index(int i, int j, int k)
{
	return m_Data[i][j][k];
}

TemperatureField& TemperatureField::operator = (const TemperatureField &Right)
{
	if (this == &Right)
		return *this;

	Clear();
	m_Length = Right.m_Length;
	m_Width = Right.m_Width;
	m_Height = Right.m_Height;
	m_Data = new double**[m_Length];
	for (int i = 0; i < m_Length; i++)
	{
		m_Data[i] = new double*[m_Width];
		for (int j = 0; j < m_Width; j++)
		{
			m_Data[i][j] = new double[m_Height];
			for (int k = 0; k < m_Height; k++)
			{
				m_Data[i][j][k] = Right.m_Data[i][j][k];
			}
		}
	}
	return *this;
}