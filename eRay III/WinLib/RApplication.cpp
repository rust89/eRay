#include "RApplication.h"

using namespace RClasses;

RApplication::RApplication()
{
	m_wndMainWindow = 0;
	m_hMainAccel = 0;
}

RApplication::~RApplication()
{

}

void RApplication::GetCmdParams(CmdParams &Params)
{
	Params.Clear();
	
	RString CmdLine(GetCommandLine());

	bool in_QM = false;
    bool in_TEXT = false;
	bool in_SPACE = true;
	int IndexBegin = -1;
	for (len_t i = 0; i < CmdLine.length(); i++)
	{
        if(in_QM) // если мы внутри ковычек 
		{
            if(CmdLine[i] == TEXT('\"')) 
			{
                in_QM = false;
				Params.Append(CmdLine.substr(IndexBegin,i - IndexBegin));
				IndexBegin = -1;
            } 
		}
		else  
		{
			switch(CmdLine[i]) 
			{
               case TEXT('\"'):
                    in_QM = true;
					IndexBegin = i + 1;
                    break;
                case TEXT(' '):
                case TEXT('\t'):
                case TEXT('\n'):
                case TEXT('\r'):
                    if(in_TEXT) 
					{
						
						Params.Append(CmdLine.substr(IndexBegin,i - IndexBegin));
						IndexBegin = -1;
                    }
                    in_TEXT = false;
					in_SPACE = true;
                    break;
                default:
					if (in_SPACE)
					{
						in_SPACE = false;
						IndexBegin = i;
					}
                    in_TEXT = true;
                    break;
            }
        }		
	}
	if (IndexBegin != -1 && in_TEXT)
		Params.Append(CmdLine.substr(IndexBegin,CmdLine.length() - IndexBegin));
}

void RApplication::Run()
{
	if (m_wndMainWindow)
	{
		ShowWindow(m_wndMainWindow->Handle(),SW_SHOW);
		UpdateWindow(m_wndMainWindow->Handle());

		MSG msg;

		while(GetMessage(&msg,0,0,0) != WM_NULL) // отработка сообщений
		{
			if (m_hMainAccel)
			{
				if (TranslateAccelerator(m_wndMainWindow->Handle(),m_hMainAccel,&msg))
					continue;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void RApplication::MsgLoop()
{
	MSG msg;

	while(GetMessage(&msg,0,0,0) != WM_NULL) // отработка сообщений
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int RApplication::GetDesktopHight()
{
	HDC hDCScreen = GetDC(NULL);
	int Vertres = GetDeviceCaps(hDCScreen, VERTRES);
	ReleaseDC(NULL, hDCScreen);
	return Vertres;
}

int RApplication::GetDesktopWidth()
{
	HDC hDCScreen = GetDC(NULL);
	int Horres = GetDeviceCaps(hDCScreen, HORZRES);
	ReleaseDC(NULL, hDCScreen);
	return Horres;
}