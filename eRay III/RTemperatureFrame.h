#ifndef RTEMPERATURE_FRAME_H_
#define RTEMPERATURE_FRAME_H_

#include "WinLib\WinLibCore.h"
using namespace RClasses;


#include "RTemperatureImage.h"
#include "RModelParamFrame.h"

#define TEMPERATURE_RENDER_START 101
#define TEMPERATURE_SWITCH_VIEW 102
#define TEMPERATURE_VIEW_DIMLAYER 103

#define TEMPERATURE_N 104
#define TEMPERATURE_M 105
#define TEMPERATURE_L 106
#define TEMPERATURE_DTAU 107
#define TEMPERATURE_STEP 108


struct RTemperatureFrameParam
{
	Expression Qxl;
	Expression Qxr;
	Expression Qyl;
	Expression Qyr;
	Expression Qzb;
	double Dtau;
	int N;
	int M;
	int L;
	int TimeLayer;

	double DropLetTemp;
	double T0Liq;
	double T0Sol;
	double SolProp;
	double TEnv;

	CMetalInfo m_cMetalInfo;
};

class RTemperatureFrame : public RFrame
{
	// внутренние типы данных
	protected:

		enum eStatusState
		{
			InProgress,
			InComplete
		};


	// данные моделирования получаемые из вне
	private:
		CloudField <double> m_cQzt;
		double m_fQzt;
	// атрибуты
	private:
		//HWND hOwner; // окно родитель
		eStatusState CurrentModuleState; // текущее состояние модуля
		CMetalInfo m_cMetalInfo;
		HANDLE hWorkThread; // описатель рабочего потока
	// создание и уничтожение
	public:
		RTemperatureFrame();
		~RTemperatureFrame();
	public:
	// внешние рабочие методы
		void InitQzt(CloudField <double> &QztField, double Qzt);
		void InitMetalParam(CMetalInfo *pMetalInfo);
		int ShowModal(RCustomWindow *Owner);
	// внутренние рабочие методы
	protected:
		void UpdateStatusMsg();
		void VisualStyles();
		void StartRender(WPARAM wParam, LPARAM lParam);
		static DWORD WINAPI StartRenderThread(LPVOID lpParam);
		void ChangeView(WPARAM wParam, LPARAM lParam);
		void ChangeDimLayer();
		eView GetCurrentView();
		static void WorkStep(TemperatureWorkParam *twp);
		void LockInterface();
		void UnlockInterface();
		void InitRenderParams();
		void DrawScene(eView ViewMode, int TrackPos);
		void SetTrackBarState(eView ViewMode);
		bool GetModelingParams(RTemperatureFrameParam *Params);
		bool CheckExpressionForTOnly(Expression *expr, RString &msg);

		LRESULT N_Command(WPARAM wParam, LPARAM lParam);
		LRESULT M_Command(WPARAM wParam, LPARAM lParam);
		LRESULT L_Command(WPARAM wParam, LPARAM lParam);
		LRESULT TimeChange(WPARAM wParam, LPARAM lParam);
		void UpdateDimantionLabel(eView CurrentView);
	// контролы
	protected:
		RComboBox m_wndSwitchView;
		RTrackBar m_wndDimLayer;
		RStaticText m_wndCurrentDimLayer;

		RTemperatureImage m_wndImage;

		RStaticText m_wndLabel1;
		RTextBoxEx m_wndStepCount;
		RButton m_wndStartButton;
		RStaticText m_wndLabelTime;

		RGroupBox m_wndQStreamBox;
		RStaticText m_wndStaticQxl;
		RStaticText m_wndStaticQxr;
		RStaticText m_wndStaticQyl;
		RStaticText m_wndStaticQyr;
		RStaticText m_wndStaticQzb;
		RTextBox m_wndTextBoxQxl;
		RTextBox m_wndTextBoxQxr;
		RTextBox m_wndTextBoxQyl;
		RTextBox m_wndTextBoxQyr;
		RTextBox m_wndTextBoxQzb;
		RStaticText m_wndStaticQxlLab;
		RStaticText m_wndStaticQxrLab;
		RStaticText m_wndStaticQylLab;
		RStaticText m_wndStaticQyrLab;
		RStaticText m_wndStaticQzbLab;

		RGroupBox m_wndModelParamBox;

		RStaticText m_wndStaticDeltaTau;
		RTextBoxEx m_wndTextBoxDeltaTau;
		RStaticText m_wndStaticDeltaTauLab;

		RStaticText m_wndStaticN;
		RTextBoxEx m_wndTextBoxN;
		RStaticText m_wndStaticNLab;

		RStaticText m_wndStaticM;
		RTextBoxEx m_wndTextBoxM;
		RStaticText m_wndStaticMLab;

		RStaticText m_wndStaticL;
		RTextBoxEx m_wndTextBoxL;
		RStaticText m_wndStaticLLab;

		RGroupBox m_wndModelBeginParamBox;

		RStaticText m_wndStaticDropletTemp;
		RTextBoxEx m_wndTextBoxDropletTemp;
		RStaticText m_wndStaticDropletTempLab;

		RStaticText m_wndStaticT0Liq;
		RTextBoxEx m_wndTextBoxT0Liq;
		RStaticText m_wndStaticT0LiqLab;

		RStaticText m_wndStaticT0Sol;
		RTextBoxEx m_wndTextBoxT0Sol;
		RStaticText m_wndStaticT0SolLab;

		RStaticText m_wndStaticSolProp;
		RTextBoxEx m_wndTextBoxSolProp;
		RStaticText m_wndStaticSolPropLab;

		RStaticText m_wndStaticTEnv;
		RTextBoxEx m_wndTextBoxTEnv;
		RStaticText m_wndStaticTEnvLab;

		RStatusBar m_wndStatusBar;
		RProgressBar m_wndProgress;
	// события окна
	protected:
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnClose(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
};


#endif