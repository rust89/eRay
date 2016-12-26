

#ifndef _RAPPLICATION_H_
#define _RAPPLICATION_H_ 

#include <Windows.h>

#include "RFrame.h"
#include "RArray.h"

namespace RClasses {

typedef RArray <RString> CmdParams;

/**
@class RApplication
Главный класс приложения
Отвечает за обработку коммандной строки
Так же содержит цикл обработки сообщений
*/

class RApplication
{
	public:
		RApplication(); 
		~RApplication();
		void GetCmdParams(CmdParams &Params);
		void RApplication::Run();
		static void MsgLoop();
		static int GetDesktopWidth();
		static int GetDesktopHight();
	// атрибуты
		RFrame *m_wndMainWindow;
		HACCEL m_hMainAccel;
};

} // namespace RClasses

#endif