
#include "RAboutBox.h"
#include "build.h"
#include "WinLib\RString.h"

#include <GdiPlus.h>

using namespace Gdiplus;
using namespace RClasses;

/*
BOOL DrawPony = FALSE;
HWND hPonyWindow = 0;
const TCHAR *PonyWindowName = TEXT("PonyWindow");
BOOL bPonyWindowReg = FALSE;
Image *g_AboutBoxImage = 0;
UINT CurrFrame = 0;
RECT DrawRect;
const UINT rePaintTimer = 1;
*/

Image* LoadPicture(UINT nResID);

INT_PTR WINAPI AboutBoxProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_INITDIALOG:
		{
			return AboutBoxInit(hwnd,wParam,lParam);
		}
		case WM_CLOSE:
		{
			return AboutBoxClose(hwnd,wParam,lParam);
		}
	}
	return FALSE;
}

INT_PTR WINAPI AboutBoxInit(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	// установка номера сборки
	SetWindowText(GetDlgItem(hwnd,IDC_ABOUT_BUILD),TEXT(__BUILD_NUMBER));
	// загрузка лога
	DWORD BuffSize = MAX_PATH;
	TCHAR* lpBuffer = new TCHAR[BuffSize];
	DWORD GCDResult = GetCurrentDirectory(BuffSize, lpBuffer);
	if (GCDResult != 0 && GCDResult > BuffSize)
	{
		delete [] lpBuffer;
		BuffSize = GCDResult;
		lpBuffer = new TCHAR[GCDResult];
		GCDResult = GetCurrentDirectory(BuffSize, lpBuffer);
		if (!GCDResult) return FALSE;
	}
	RString FileName(lpBuffer);
	delete [] lpBuffer;
	FileName.append(TEXT("\\changelog.txt"));
	HANDLE hFile = CreateFile(FileName.c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		SetWindowText(GetDlgItem(hwnd,IDC_ABOUT_INFO),TEXT("Не удаётся загрузить лог"));
		return TRUE;
	}
	DWORD dwFileSize = SetFilePointer(hFile,0,0,FILE_END) + 1;
	SetFilePointer(hFile,0,0,FILE_BEGIN);
	CHAR *lpFileBuffer = new CHAR[dwFileSize];
	DWORD dwBytes;
	if (ReadFile(hFile,(LPVOID)lpFileBuffer,dwFileSize,&dwBytes,NULL) == TRUE)
	{
		lpFileBuffer[dwFileSize - 1] = '\0';
		SetWindowTextA(GetDlgItem(hwnd,IDC_ABOUT_INFO),(LPCSTR)lpFileBuffer);
	}
	else
	{
		SetWindowText(GetDlgItem(hwnd,IDC_ABOUT_INFO),TEXT("Не удаётся загрузить лог"));
	}
	delete [] lpFileBuffer;
	CloseHandle(hFile);
	return TRUE;
}

INT_PTR WINAPI AboutBoxClose(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hwnd, 0);
	return TRUE;
}

Image* LoadPicture(UINT nResID)
{
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(nResID), TEXT("Animation"));
    if (hResource == NULL)
        return NULL;

    HGLOBAL hImage = LoadResource(hModule, hResource);
    if (hImage == NULL)
        return NULL;
    LPVOID pImage = LockResource(hImage);
    if (pImage == NULL)
        return NULL;

    Image* pPicture = NULL;
    int size = SizeofResource(hModule, hResource);
    HGLOBAL hBlock = GlobalAlloc(GHND, size);
    if (hBlock != NULL) 
    {
        LPVOID pBlock = GlobalLock(hBlock);
        if (pBlock != NULL) 
        {
            CopyMemory(pBlock, pImage, size);
            IStream* pStream;
            if (CreateStreamOnHGlobal(hBlock, FALSE, &pStream) == S_OK) 
                pPicture = Image::FromStream(pStream);
            GlobalUnlock(pBlock);
            pStream->Release();
        }
        GlobalFree(hBlock);
    }
    return pPicture;
}