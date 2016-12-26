#ifndef _RABOUTBOX_H_
#define _RABOUTBOX_H_

#include <Windows.h>
#include "resource.h"

extern BOOL DrawPony;

INT_PTR WINAPI AboutBoxProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI AboutBoxInit(HWND hwnd, WPARAM wParam, LPARAM lParam);
INT_PTR WINAPI AboutBoxClose(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI PonyProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif