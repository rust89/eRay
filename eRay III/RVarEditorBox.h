#ifndef _RVAREDITORFRAME_H_
#define _RVAREDITORFRAME_H_

#include <Windows.h>
#include <CommCtrl.h>
#include <vector>
#include <stack>
#include "resource.h"

#include "WinLib\RString.h"

using namespace RClasses;
using namespace std;

class VarEditorUnit
{
	public:
		RString Name;
		RString Value;
		VarEditorUnit();
		VarEditorUnit(RString _Name, RString _Value);
};

typedef std::vector<VarEditorUnit> VarEditorVariables;
typedef std::vector<std::pair<VarEditorUnit,int>> tVarEditor_VarsAndIndex;

extern VarEditorVariables VarEditor_Vars;
extern tVarEditor_VarsAndIndex VarEditor_VarsAndIndex;
extern VarEditorUnit CurrentInsertVarUnit;

INT_PTR WINAPI VarEditorProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcInit(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcClickOk(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcClickCancel(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcClickAdd(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcClickEdit(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcClickRem(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcNotify(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarEditorProcClose(HWND hwnd, WPARAM wParam, LPARAM lParam);

INT_PTR WINAPI VarInsertProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarInsertProcInit(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarInsertProcClickOk(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarInsertProcClickCancel(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI VarInsertProcClose(HWND hwnd, WPARAM wParam, LPARAM lParam);

#endif