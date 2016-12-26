#ifndef _RTEXTBOXFILTER_H_
#define _RTEXTBOXFILTER_H_

#include "RTextBox.h"

namespace RClasses {

class RTextBoxEx : public RTextBox
{
	public:
		RTextBoxEx();
		virtual ~RTextBoxEx();
	public:
		enum eFilterType
		{
			NonFilter,
			RealFilter,
			IntegerFilter
		};

		void SetFilter(eFilterType FilterType);
		eFilterType GetFilterType();

		void SetSignInput(bool Mode);
		bool GetSignInput();

	protected:

		/**
		* Определяет тип фильтра
		*/
		eFilterType m_FilterType;

		/** 
		* возможна ли установка знака
		*/
		bool Signed;

		bool IsNonFilteringKey(WPARAM KeyCode);
		LRESULT FloatFilter(WPARAM wParam, LPARAM lParam);
		LRESULT IntFilter(WPARAM wParam, LPARAM lParam);
	protected:
		virtual LRESULT OnChar(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnPaste(WPARAM wParam, LPARAM lParam);
};

}

#endif