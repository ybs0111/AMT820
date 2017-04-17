#if !defined(AFX_SCREEN_SET_RECIPE_H__5E50EACA_5E36_4DDC_9260_2D45F2927DA3__INCLUDED_)
#define AFX_SCREEN_SET_RECIPE_H__5E50EACA_5E36_4DDC_9260_2D45F2927DA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Set_Recipe.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가
#include "Public_Function.h"
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "FloatST.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "UniButtonEx.h"	// 유니 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 생성 클래스 추가 
#include "MyBasicData.h"
/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Recipe form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "Ctlbd_Variable.h"	// Added by ClassView

class CScreen_Set_Recipe : public CFormView
{
public:
	CScreen_Set_Recipe();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Set_Recipe)

// Form Data
public:
	//{{AFX_DATA(CScreen_Set_Recipe)
	enum { IDD = IDD_SCREEN_SET_RECIPE };
	CXPGroupBox	m_group_data_setting;
	CXPGroupBox	m_group_epoxt_limit_usecnt;
	CGradientStatic	m_msg_HsRub_Y_Offset;
	CGradientStatic	m_msg_HsRub_X_Offset;
	CEXDigitST	m_dgt_HsRub_Y_Offset;
	CEXDigitST	m_dgt_HsRub_X_Offset;
	CEXDigitST	m_dgt_Epoxy_UseCnt;
	CXPGroupBox	m_group_DispenserVppmA;
	CEXDigitST	m_dgt_DispenserVppmA;
	CMacButton	m_btn_recipe_cancel;
	CMacButton	m_btn_recipe_apply;
	CXPGroupBox	m_group_size_setting;
	CXPGroupBox	m_group_pitch_data;
	CXPGroupBox	m_group_Carrier_Move;
	CGradientStatic	m_msg_Distance;
	CGradientStatic	m_msg_Offset_30;
	CGradientStatic	m_msg_Offset_20;
	CGradientStatic	m_msg_Offset_10;
	CGradientStatic	m_msg_Offset;
	CEXDigitST	m_dgt_Offset_Dis;
	CEXDigitST	m_dgt_Offset_30;
	CEXDigitST	m_dgt_Offset_20;
	CEXDigitST	m_dgt_Offset_10;
	CEXDigitST	m_dgt_Offset;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_recipe_font;			// 폰트 정보 설정 변수
	CFont* mp_recipe_combo_font;
	
	CMyBasicData     mcls_recipe;	// 기본 셋팅 정보 로딩 및 저장 클래스
	
	GridControlAlg m_pGrid;

	CEditEx	 m_edit_tool_id;
	CEditEx	 m_edit_vision_light;

	double md_pitch_data[14][2];
	
	float m_fProbeRealData[8][2];
	float m_fProbeJigData[14][8][2];
	
	float m_fSSDThicknessData[2];
	float m_fThicknessData[2];
	float m_fThicknessGapData[2][2];

	int m_iJigNumber;
	int m_iOrderNumber[8][2];
	int m_iTighteningMode;
	
	int mn_size_x[5][2];				// 0: Tray, 1:Case Tray, 2:Jig, 3:Buffer, 4:Uld Tray
	int mn_size_y[5][2];				// 0: Tray, 1:Case Tray, 2:Jig, 3:Buffer

	int m_iPCBFMode[2];
	int m_iPCBFCnt[2];
	int m_iULDFMode[2];
	int m_iCaseAssembleMode[2];

	int m_iPCBLDPickRotate[2];
	int m_iPCBLDPlaceRotate[2];
	int m_iPCBULDRjtFlipping[2];

	int m_iStepBackup;

	int m_iPartNo;
	int mn_device_name;

	int m_iAddRecipe;

	CString m_strToolID[2];
	CString m_strVisionLightValue[2];
	int m_iSSDSize[2];

	//kwlee 2017.0410
	double	m_dLoaderTransferTrayDeviceGap[2];
	DWORD m_dSatbleTime[2];
	double m_nEpoxyRunSpeed[2];
	double m_nEpoxyXYRunSpeed[2][3];//[0]//vel [1]:acc [2] dec
	
	double m_dEpoxyXOffSet[2];
	double m_dEpoxyYOffSet[2];
	double m_nRubHSRunSpeed[2];
	
	double m_dTrayPitch_Y[2];			//트레이 피치간격 
	double m_dTrayPitch_X[2];			//트레이 피치간격 	
	
	int m_nfDispenserVppmA[2];
	int m_nEpoxyDotScrewCount[2];
	int m_nHsTrayY[2];						// HeatSink tray Y 정보.......
	int m_nHsTrayX[2];						// HeatSink tray X 정보.......
	
	int m_nTrayY[2];						// tray Y 정보.......
	int m_nTrayX[2];	
	//kwlee 2017.0416
	double	dHSCarrierSpreadMoveOffset[2];
	double	dHSCarrierSpreadMoveDistance[2];
	double	dHeatSinkCarrierSpreadMove1Offset[2];
	double	dHeatSinkCarrierSpreadMove2Offset[2];
	double	dHeatSinkCarrierSpreadMove3Offset[2];

	double	m_dHeatsinkRubXOffset[2];
	double	m_dHeatsinkRubYOffset[2];
	int		m_nEpoxyLimitUseCont[2];

	GridControlAlg	m_p_grid;
	TSpread			*m_grid_operate;
// Operations
public:
	void OnSetRecipe_Label();

	void OnSetRecipe_EditBox_Set();
	void OnSetRecipe_Button_Set();
	BOOL AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	int OnRecipe_Init_List(LPCTSTR pszPath);
	void OnDisplay_Recipe_List();
	void OnInit_Recipe_List();
	bool CheckSamePartNo(CString strPartNo);
	void OnDisplay_PartNo_List();
	void OnInit_PartNo_List();
	int ScrewOrderCheck();
	void OnSetRecipe_Init_Pitch_Data();
	void OnSetRecipe_Init_Size_Data();
	void OnSetRecipe_Data_Change();
	void OnSetRecipe_Digital_Count_Set();
	void OnSetRecipe_GroupBox_Set();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	
	//kwlee 2017.0410
	int  Data_Comparison();
	void OnSetRecipe_Data_Set();
	void OnInitGrid();
	void OnSetRecipe_LogFile_Create();
	void OnSetRecipe_Data_BackUp();
	void OnSetRecipe_Data_Recovery();
	void OnSetRecipe_Data_Apply();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Set_Recipe)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Set_Recipe();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Set_Recipe)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnRecipeApply();
	afx_msg void OnBtnRecipeCancle();
	afx_msg void OnUserControl();
	afx_msg void OnDgtDispenservppma();
	afx_msg void OnDgtOffset();
	afx_msg void OnDgtOffset10();
	afx_msg void OnDgtOffset20();
	afx_msg void OnDgtOffset30();
	afx_msg void OnDgtOffsetDis();
	afx_msg void OnDgtEpoxyUsecnt();
	afx_msg void OnDgtHsRubXOffset();
	afx_msg void OnDgtHsRubYOffset();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetRecipe_Work_Info_Display(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_SET_RECIPE_H__5E50EACA_5E36_4DDC_9260_2D45F2927DA3__INCLUDED_)
