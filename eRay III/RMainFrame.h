#ifndef _RMAINFRAME_H_
#define _RMAINFRAME_H_

#include "WinLib\WinLibCore.h"

// прикладные библиотеки
#include "ExpressionEngine\Expression.h"

#include "eRayDocument.h"
#include "RConfigPanel.h"
#include "RImagePanel.h"
#include "RRangingFrame.h"
#include "resource.h"

#include "RConfigBox.h"
#include "RVarEditorBox.h"
#include "RAboutBox.h"

// временное решение, нужно заменить на что-то своё
#include <vector>
using namespace std;

using namespace RClasses;

#define id_Combo 11
#define id_Track 12

struct ModelingParams
{
	Expression ExprY;
	Expression ExprX;
	Variables Vars;
	double *TParams;
	int TParamsCount;
	double RayRadius;
	double RayVoltage;
	long XRes;
	long YRes;
	bool UseRegion;
	double MinX;
	double MaxX;
	double MinY;
	double MaxY;
};


class RMainFrame : public RFrame
{
	public:
		RMainFrame(void);
		~RMainFrame(void);
		
		void SetText(LPCTSTR Text) {RFrame::SetText(RString(TEXT("eRay - ") + RString(Text)).c_str());}
		bool OpenAfterLoad;
		RString ProjectFileName; // содержит полное имя проекта
		bool SaveBeforExitPromt; // задавать ли вопрос по поводу уверенности пользователя - выйти
	// контролы
	protected:
		eRayDocument Document;
		typedef vector <RString> OutMsg;
		int ConfigPanelWidth;
		int InfoTextHeight;
		e3DOption Option3d;

		RRangingFrame RangingByX;
		RRangingFrame RangingByY;
	
		RStatusBar MainStatusBar;
		RConfigPanel ConfigPanel;
		RImagePanel ImagePanel;
		RListView InfoList;
		RToolBar ToolBar;
		RComboBox ComboSwitch;
		RTrackBar ContrastTrack;
	// дополнительные переменные
		POINT MinSize;
		RImageList ImageList;
	// дополнительные методы
		RString GetCurTime();
	// события и методы
		void AddMenuIcon(HMENU hMenu, UINT mID, UINT bID);
		void AdjustWindowsLayout(int ClientWidth, int ClientHeight);
		bool GetParamValue(double **Var, int *VarCount, OutMsg &MsgList);
		bool GetVariable(Variables &var, OutMsg &MsgList);
		bool InitControlImages();
		void InitConfigParam(ConfigParams *cp);
		void ApplyConfigParam(ConfigParams *cp);
		bool SaveProject(LPCTSTR FileName);
		bool LoadProject(LPCTSTR FileName);
		void Load3dView();
		bool ParseModelingParams(ModelingParams *mp, OutMsg &MsgList);
		void LockInterface();
		void UnLockInterface();

		// добавление группы для события. Возвращает id группы
		int AddEventGroup(LPCTSTR szText);
		// добавляет сообщение в группу, если возможно
		int AddEventMsg(bool ViewTime, LPCTSTR szText, int idGroup);

		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnClose(WPARAM wParam, LPARAM lParam);
		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		LRESULT OnMinMaxInfo(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		LRESULT OnErsBckg(WPARAM wParam, LPARAM lParam);
		LRESULT OnMenuSelect(WPARAM wParam, LPARAM lParam);
		LRESULT OnChangeContrast(WPARAM wParam, LPARAM lParam);
		LRESULT OnChangeRanging(WPARAM wParam, LPARAM lParam);
		LRESULT OnFocus(WPARAM wParam, LPARAM lParam);

		LRESULT OnGetTransParams(WPARAM wParam, LPARAM lParam);
		LRESULT OnSetTransParams(WPARAM wParam, LPARAM lParam);
		LRESULT OnViewAddParams(WPARAM wParam, LPARAM lParam);
		LRESULT OnViewTemperatureModel(WPARAM wParam, LPARAM lParam);
	// Разбор Событий WM_COMMAND
		LRESULT OnMenuSave(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnMenuSaveAs(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnMenuOpen(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnMenuClose(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnMenuStart(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnConfig(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnImageSave(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnMenuGetHelp(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnMenuAbout(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnViewSwitch(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnExampleRing(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnExampleSin(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnExampleSpir(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnExampleKren(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnExampleRose(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnVarEditor(HWND hControl, WORD cID, WORD Notify);
		LRESULT OnRun3d(HWND hControl, WORD cID, WORD Notify);
		static DWORD WINAPI BuildThread(LPVOID Param);
		static DWORD WINAPI OutRangingFor3D(LPVOID Param);
};

#endif