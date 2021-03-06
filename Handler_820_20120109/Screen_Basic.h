#if !defined(AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_)
#define AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"

/* ****************************************************************************** */
/* 설정 정보 로딩 및 저장 클래스                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */
#include "GridControlAlg.h"
#include "Digit.h"			// 디지털 카운터 클래스
/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Basic : public CFormView
{
public:
	CScreen_Basic();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Basic)

// Form Data
public:
	//{{AFX_DATA(CScreen_Basic)
	enum { IDD = IDD_SCREEN_BASIC };
	CEXDigitST	m_dgt_hardness_time;
	CEXDigitST	m_dgt_epoxy_clean_time;
	CXPGroupBox	m_group_Tray_Y;
	CXPGroupBox	m_group_Tray_X;
	CXPGroupBox	m_group_HsTray_Y;
	CXPGroupBox	m_group_HsTrayX;
	CXPGroupBox	m_group_DispenserVppmA;
	CEXDigitST	m_dgt_Tray_Y;
	CEXDigitST	m_dgt_Tray_X;
	CEXDigitST	m_dgt_HsTray_Y;
	CEXDigitST	m_dgt_HsTray_X;
	CEXDigitST	m_dgt_DispenserVppmA;
	CEXDigitST	m_dgt_EpoxyScrewCnt;
	CXPGroupBox	m_group_EpoxyScrewCnt;
	CXPGroupBox	m_group_Retry_Cnt;
	CEXDigitST	m_dgt_Retry_Cnt;
	CEXDigitST	m_dgt_Rubb;
	CEXDigitST	m_dgt_Pick_Retry_Cnt;
	CEXDigitST	m_dgt_Partial_Cnt;
	CEXDigitST	m_dgt_Epoxy_Y;
	CEXDigitST	m_dgt_Epoxy_X;
	CEXDigitST	m_dgt_Elv_Partial_Cnt;
	CEXDigitST	m_dgt_7387_Alarm_Cnt;
	CGradientStatic	m_msg_Epoxy_Y;
	CGradientStatic	m_msg_Epoxy_X;
	CXPGroupBox	m_group_Rubb_Cnt;
	CXPGroupBox	m_group_Pick_Retry_Cnt;
	CXPGroupBox	m_group_Patial_Cnt;
	CXPGroupBox	m_group_Epoxy_Offset;
	CXPGroupBox	m_group_Elv_Partial_Cnt;
	CXPGroupBox	m_group_7387_Alarm_Cnt;
	CButtonST	m_btn_delete;
	CButtonST	m_btn_create;
	CXPGroupBox	m_group_operate_method;
	CXPGroupBox	m_group_model;
	CMacButton	m_btn_cancel;
	CMacButton	m_btn_apply;
	//}}AFX_DATA

// Attributes
public:
	int				m_n_filename_pos;
	int				m_n_operate_cnt[MAXOPERATORCNT];
	int				m_n_operate_used[MAXOPERATORCNT];
	int				m_n_mode_interface[2]; //V
	int				m_n_mode_device[2]; ///V
	int				m_n_mode_retest[2];

	int				mn_device_name;

	//kwlee 2017.0404
	
	int             m_nModeTester[2];
	int             m_nModeBcr[2];
	int             m_nModeModule[2];
	int             m_nModeOffline[2];

	int				m_nmode_7387[2];
	int             m_nmode_tray_type[2]; //Type1, Type2
	int				m_n7387InspectAlarmSkipMode[2];
	int				m_n3874InspectAlarmSkipMode[2]; //kwlee 2017.0411
	int				m_n3874InspectMode[2];
	int				n_Light_Curtain_Mode[2];
	int				n_mode_case_assembly[2];

	//2017.0518
	int				m_nOnlyCarrierMove[2];
	
	int				m_n_count_retry[2];
	int             m_nCountPickRetry[2];
	int             m_nCountPartial[2];
	int             m_nCount_elevator_Partial[2];
	int				m_n7387AlarmCount[2];
	int				n_rubb_count[2];
	double			dEpoxyXLineOffSet[2];
	double			dEpoxyYLineOffSet[2];
	int				m_nEPoxyCleanTime[2];
	int				m_nHardnessUseTime[2];
	
	
	//kwlee 2017.0406
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
	

	CFont*			mp_basic_font;			// 폰트 정보 설정 변수
	GridControlAlg	m_p_grid;
	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	// **************************************************************************
	// 리스트 박스 컨트롤 정보 저장 클래스 변수 선언                             
	// **************************************************************************
	CString mstr_device_name[2];		// 선택된 디바이스 종류 저장 변수
	CString mstr_temp_device;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_load_file;				// 로딩할 파일에 대한 [폴더]+[파일명] 저장 변수

//	int mn_device_name;					// 선택된 디바이스의 리스트 위치 저장 변수


	CString mstr_new_device;			// 새로 생성할 디바이스의 이름.

	CMyBasicData     mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	/* ************************************************************************** */
	/* 칼라 에디트 박스 클래스 변수 선언                                          */
	/* ************************************************************************** */

	CEditEx	 m_edit_device_type;

// Operations
public:
	void Init_Grid_Operate();
	void Init_Grid_Operate_1();
	void Display_File(int n_mode, int n_count, CString str_filename);
	void Init_Grid_File();
	void OnBasic_Data_Display();
	void Init_Reject();
	void Init_Pass();
	void OnBasic_Data_HisoryLog();
	int OnBasic_Input_Device_Check(CString str_device);
	int OnBasic_New_Device_Check(CString str_device);
	BOOL AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	int OnBasic_Init_List(LPCTSTR pszPath);
	void OnBasic_Device_Display(CString str_dvc);

	
	void OnBasic_Data_Backup();
	void OnBasic_Data_Label();
    void OnBasic_Device_Focus_Set(); 
	//kwlee 2017.0403
	void Data_Recovery();
	void OnBasic_Data_Apply();
	int Data_Comparison();
	void OnBasic_Data_Set();
	void OnBasic_Digital();
	void OnBasic_Data_Group();
	void OnBasic_Data_Button();
	void OnBasic_Data_EditBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Basic)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Basic();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Basic)
	afx_msg void OnDestroy();
	afx_msg void OnBtnBasicCancel();
	afx_msg void OnBtnBasicApply();
	afx_msg void OnBtnBasicCreate();
	afx_msg void OnBtnDelete();
	afx_msg void OnDgtEpoxyX();
	afx_msg void OnDgtEpoxyY();
	afx_msg void OnDgtRubb();
	afx_msg void OnDgt7387AlarmCnt();
	afx_msg void OnDgtElvPartialCnt();
	afx_msg void OnDgtPickRetryCnt();
	afx_msg void OnDgtPartialCnt();
	afx_msg void OnDgtRetryCnt();
	afx_msg void OnDgtEpoxyScrewCnt();
	afx_msg void OnDgtHstrayY();
	afx_msg void OnDgtHstrayX();
	afx_msg void OnDgtTrayY();
	afx_msg void OnDgtTrayX();
	afx_msg void OnDgtDispenservppmA();
	afx_msg void OnDgtEpoxyCleanTime();
	afx_msg void OnButtonCleanReset();
	afx_msg void OnButtonHardnessReset();
	afx_msg void OnDgtHardnessTime();
	//}}AFX_MSG
	afx_msg void OnCell_L_Click(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCell_R_Click(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_)
