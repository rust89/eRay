
#ifndef _RMODELPARAMFRAME_H_
#define _RMODELPARAMFRAME_H_

#include "WinLib\WinLibCore.h"
#include "ExpressionEngine\Expression.h"

using namespace RClasses;


struct CMetalInfo
{
	Expression m_cLambdaLiq;
	Expression m_cLambdaSol;

	Expression m_cDensityLiq;
	Expression m_cDensitySol;

	Expression m_cTempCapLiq;
	Expression m_cTempCapSol;

	double m_fTemperatureLiq;
	double m_fTemperatureSol;

	// степень черноты
	double m_fBlackness;
	// удельна€ теплота плавлени€
	double m_fFusion;
	// кинематическа€ в€зкость среды
	double m_fViscosity;

	// габариты печи
	double m_fX; 
	double m_fY;
	double m_fZ;

	// ток электронного луча
	double m_fJ;

	// скорость поступлени€ шихты
	double m_fG;

	// кпд
	double m_fEfficiency;
};

class RModelParamFrame : public RFrame
{
	// создание и уничтожение
	public:
		RModelParamFrame();
		~RModelParamFrame();
		int ShowModal(RFrame *Owner);
		void setMetalInfo(CMetalInfo *MetalInfo);
		void getMetalInfo(CMetalInfo *MetalInfo);
	// внутренние методы
	protected:
		CMetalInfo m_cMetalInfo;

		bool ParseParams();
		void VisualStyles();
		bool CheckExpressionForTOnly(Expression &expr, RString &msg);
	// контролы
	protected:

		RButton m_wndButtonOk;
		RButton m_wndButtonCancel;


		RGroupBox m_wndMetalBox;
		RStaticText m_wndStaticMetalLambdaL;
		RTextBoxEx m_wndTextBoxMetalLambdaL;
		RStaticText m_wndStaticMetalLambdaLabL;
		RStaticText m_wndStaticMetalThermalCapL;
		RTextBoxEx m_wndTextBoxMetalThermalCapL;
		RStaticText m_wndStaticMetalThermalCapLabL;
		RStaticText m_wndStaticMetalDensityL;
		RTextBoxEx m_wndTextBoxMetalDensityL;
		RStaticText m_wndStaticMetalDensityLabL;
		RStaticText m_wndStaticMetalTempLiq;
		RTextBoxEx m_wndTextBoxMetalTempLiq;
		RStaticText m_wndStaticMetalTempLiqLab;
		RStaticText m_wndStaticMetalBlackness;
		RTextBoxEx m_wndTextBoxMetalBlackness;
		RStaticText m_wndStaticMetalBlacknessLab;


		RStaticText m_wndStaticMetalLambdaH;
		RTextBoxEx m_wndTextBoxMetalLambdaH;
		RStaticText m_wndStaticMetalLambdaLabH;
		RStaticText m_wndStaticMetalThermalCapH;
		RTextBoxEx m_wndTextBoxMetalThermalCapH;
		RStaticText m_wndStaticMetalThermalCapLabH;
		RStaticText m_wndStaticMetalDensityH;
		RTextBoxEx m_wndTextBoxMetalDensityH;
		RStaticText m_wndStaticMetalDensityLabH;
		RStaticText m_wndStaticMetalTempSol;
		RTextBoxEx m_wndTextBoxMetalTempSol;
		RStaticText m_wndStaticMetalTempSolLab;		
		RStaticText m_wndStaticMetalFusion;
		RTextBoxEx m_wndTextBoxMetalFusion;
		RStaticText m_wndStaticMetalFusionLab;

		RStaticText m_wndStaticMetalViscosity;
		RTextBoxEx m_wndTextBoxMetalViscosity;
		RStaticText m_wndStaticMetalViscosityLab;

		RGroupBox m_wndOvenBox;
		RStaticText m_wndStaticOvenX;
		RTextBoxEx m_wndTextBoxOvenX;
		RStaticText m_wndStaticOvenXLab;
		RStaticText m_wndStaticOvenY;
		RTextBoxEx m_wndTextBoxOvenY;
		RStaticText m_wndStaticOvenYLab;
		RStaticText m_wndStaticOvenZ;
		RTextBoxEx m_wndTextBoxOvenZ;
		RStaticText m_wndStaticOvenZLab;
		
		RStaticText m_wndStaticOvenAmpTraffic;
		RTextBoxEx m_wndTextBoxOvenAmpTraffic;
		RStaticText m_wndStaticOvenAmpTrafficLab;
		RStaticText m_wndStaticOvenG;
		RTextBoxEx m_wndTextBoxOvenG;
		RStaticText m_wndStaticOvenGLab;
		RStaticText m_wndStaticOvenEfficiency;
		RTextBoxEx m_wndTextBoxOvenEfficiency;
		RStaticText m_wndStaticOvenEfficiencyLab;

	// событи€
	protected:
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
		LRESULT OnClose(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		LRESULT OnFocus(WPARAM wParam, LPARAM lParam);
};

#endif