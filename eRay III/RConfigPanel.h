#ifndef _CONFIGPANEL_H
#define _CONFIGPANEL_H

#include "WinLib\WinLibCore.h"

using namespace RClasses;

#include "RVarEditorBox.h"
#include "AdditionalWndMessages.h"

// временное решение
#include <vector>
using namespace std;

class RConfigPanel : public RFrame
{
	public:
		RConfigPanel();
		~RConfigPanel();
		void SetExamples(int Index);
		void UpdateVarList();
		void LockInterface();
		void UnLockInterface();
	protected:
		int TextBoxHeight;
		HFONT hEditFont;
	public:
		VarEditorVariables VarList;

		RTextBox ExprY;
		RTextBox ExprX;
		RStaticText LabelExprY;
		RStaticText LabelExprX;

		RListView MoreVariables;

		RGroupBox TParamsBox;
		RTextBoxEx TBegin;
		RTextBoxEx TStep;
		RTextBoxEx TEnd;

		RGroupBox ModelBox;
		RStaticText LabelRayRadius;
		RTextBoxEx RayRadius;
		RStaticText LabelRayVoltage;
		RTextBoxEx RayVoltage;
		RButton RayAddParams;

		RGroupBox ResolutionBox;
		RTextBoxEx ModelWidth;
		RTextBoxEx ModelHeight;
		RStaticText ResolutionComment;

		RGroupBox BoundBox;
		RCheckBox BoundCheck;
		const int id_BoundCheck;
		const int id_AddParams;
		RTextBoxEx MinX;
		RTextBoxEx MaxX;
		RTextBoxEx MinY;
		RTextBoxEx MaxY;
	protected:
		void AdjustWindowsLayout(int ClientWidth, int ClientHeight);
		void SetDefaultVisualStyle(void);
		void LoadDefaultParam();
		bool CreateEditFont();
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		LRESULT OnCtlClrBtn(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		LRESULT OnFocus(WPARAM wParam, LPARAM lParam);

		LRESULT OnRegionClick(HWND hControl, WORD cID, WORD Notify);
};

#endif