#include "RTextBoxEx.h"
#include "RConverter.h"

using namespace RClasses;

RTextBoxEx::RTextBoxEx()
	: RTextBox()
{
	m_FilterType = NonFilter;
	Signed = true;

	AddMessage(WM_CHAR,MSG_EVENT_CAST(RTextBoxEx::OnChar),this);
	AddMessage(WM_PASTE,MSG_EVENT_CAST(RTextBoxEx::OnPaste),this);
}

RTextBoxEx::~RTextBoxEx()
{

}

LRESULT RTextBoxEx::FloatFilter(WPARAM wParam, LPARAM lParam)
{
	// ���� ������ ctrl ���������� ����������
	if ((GetKeyState(VK_CONTROL) & 128) == 128) 
		return DefaultControlProc(WM_CHAR,wParam,lParam);
	
	// ��������: ���������� �� ����������� ������ ������
	if (IsNonFilteringKey(wParam))
		return DefaultControlProc(WM_CHAR,wParam,lParam);

	// �������� ������
	TCHAR Char = (TCHAR)wParam;
	// ��������� ������
	if (Char == TEXT(' '))
	{
		return 0;
	}

	// ����������� ����� �����
	if (Char == TEXT('-'))
	{
		if (Signed)
		{
			if (!GetWindowTextLength(m_hHandle))
				return DefaultControlProc(WM_CHAR,wParam,lParam);
		}
		else
		{
			return 0;
		}
	}

	// �������� �����
	RString DefaultText = GetText();
	// ������, ���� ����� ����������� �����
	int Index = GetCaretIndex();
	if (Index != -1)
	{
		DefaultText.insert(DefaultText.begin() + Index,Char);
		double Result;
		try
		{
			Result = RConverter::ToDouble(DefaultText.c_str());
		}
		catch(RException)
		{
			return 0;
		}
	}
	return DefaultControlProc(WM_CHAR,wParam,lParam);
}

LRESULT RTextBoxEx::IntFilter(WPARAM wParam, LPARAM lParam)
{
	// ���� ������ ctrl ���������� ����������
	if ((GetKeyState(VK_CONTROL) & 128) == 128) 
		return DefaultControlProc(WM_CHAR,wParam,lParam);
	
	// ��������: ���������� �� ����������� ������ ������
	if (IsNonFilteringKey(wParam))
		return DefaultControlProc(WM_CHAR,wParam,lParam);

	// �������� ������
	TCHAR Char = (TCHAR)wParam;
	// ��������� ������
	if (Char == TEXT(' '))
	{
		return 0;
	}

	// ����������� ����� �����
	if (Signed && Char == TEXT('-'))
	{
		if (!GetWindowTextLength(m_hHandle))
			return DefaultControlProc(WM_CHAR,wParam,lParam);
	}
	else if (Char == TEXT('-'))
	{
		return 0;
	}


	// �������� �����
	RString DefaultText = GetText();
	// ������, ���� ����� ����������� �����
	int Index = GetCaretIndex();
	if (Index != -1)
	{
		DefaultText.insert(DefaultText.begin() + Index,Char);
		int Result;
		try
		{
			Result = RConverter::ToInt(DefaultText.c_str());
		}
		catch(RException)
		{
			return 0;
		}
	}
	return DefaultControlProc(WM_CHAR,wParam,lParam);
}

LRESULT RTextBoxEx::OnChar(WPARAM wParam, LPARAM lParam)
{
	switch(m_FilterType)
	{
		case RealFilter:
			return FloatFilter(wParam, lParam);
		case IntegerFilter:
			return IntFilter(wParam, lParam);
		default:
			break;
	}

	return DefaultControlProc(WM_CHAR,wParam,lParam);
}

LRESULT RTextBoxEx::OnPaste(WPARAM wParam, LPARAM lParam)
{
	if (OpenClipboard(m_hHandle))
	{
		HANDLE hClipBoard = 
#ifndef _UNICODE
			GetClipboardData(CF_TEXT);
#else
			GetClipboardData(CF_UNICODETEXT);
#endif
		if (!hClipBoard)
			goto insert_ok;

		TCHAR* chBuffer= (TCHAR*)GlobalLock(hClipBoard); //��������� ������
		RString fromClipboard = chBuffer;
		GlobalUnlock(hClipBoard); //������������ ������
		CloseClipboard(); // ��������� ����� ������

		// �������� ������� �������
		int CaretIndex = GetCaretIndex();

		// �������� ������� �����
		RString WindowText = GetText();

		// ��������� � ������� ����� ����� �� ������ ������
		WindowText.insert(CaretIndex,fromClipboard);

		switch(m_FilterType)
		{
			case RealFilter:
			{
				double Result;
				try
				{
					Result = RConverter::ToDouble(WindowText.c_str());
				}
				catch(RException ex)
				{
					return 0;
				}

				if (Result < 0)
					if (!Signed) return 0;
				break;
			}
			case IntegerFilter:
			{
				int Result;
				try 
				{
					Result = RConverter::ToInt(WindowText.c_str());
				}
				catch(RException ex)
				{
					return 0;
				}

				if (Result < 0)
					if (!Signed) return 0;

				break;
			}
			default:
				break;
		}
	}

	insert_ok:
	return DefaultControlProc(WM_PASTE,wParam,lParam);
}

void RTextBoxEx::SetFilter(RTextBoxEx::eFilterType FilterType)
{
	m_FilterType = FilterType;
}

RTextBoxEx::eFilterType RTextBoxEx::GetFilterType()
{
	return m_FilterType;
}

bool RTextBoxEx::IsNonFilteringKey(WPARAM KeyCode)
{
	switch(KeyCode)
	{
		case VK_BACK:
			return true;
		default:
			break;
	}
	return false;
}

void RTextBoxEx::SetSignInput(bool Mode)
{
	Signed = Mode;
}

bool RTextBoxEx::GetSignInput()
{
	return Signed;
}