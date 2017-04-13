#if !defined(AFX_SCREEN_WAIT_TIME_H__F41690DE_3119_41C4_AEBD_B5B088DAF340__INCLUDED_)
#define AFX_SCREEN_WAIT_TIME_H__F41690DE_3119_41C4_AEBD_B5B088DAF340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Wait_Time.h : header file
//

#include "Variable.h"			// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "MacButtons.h"
#include "EXDigitST.h"			// 디지털 카운터 생성 클래스 추가 
#include "FloatST.h"
#include "XpButton.h"			// XP 칼라 버튼 클래스

/* ****************************************************************************** */
/* 설정 정보 로딩 및 저장 클래스                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */

/////////////////////////////////////////////////////////////////////////////
// CScreen_Wait_Time form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Wait_Time : public CFormView
{
public:
	CScreen_Wait_Time();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Wait_Time)
		
// Form Data
public:
	//{{AFX_DATA(CScreen_Wait_Time)
	enum { IDD = IDD_SCREEN_WAIT_TIME };
	CXPGroupBox	m_group_temp_check;
	CXPGroupBox	m_group_ta_temp_data;
	CXPGroupBox	m_group_ta_etub;
	CXPGroupBox	m_group_ta_etlb;
	CXPGroupBox	m_group_ta_delta_data;
	CXPGroupBox	m_group_ta_avg_bd;
	CMacButton	m_btn_wait_apply;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_wait_time_font;		// 폰트 정보 설정 변수
	TSpread			*m_grid_time;
	
	CMyBasicData     mcls_waittime;	// 기본 셋팅 정보 로딩 및 저장 클래스

	GridControlAlg	mp_grid;
	
	int				mn_wait_time_cnt;
	int				m_iOnWait[MAX_WAIT_TIME_NUM][2];
	int				m_iOffWait[MAX_WAIT_TIME_NUM][2];
	int				m_iLimit[MAX_WAIT_TIME_NUM][2];
	int				mn_on_used[MAX_WAIT_TIME_NUM];
	int				mn_off_used[MAX_WAIT_TIME_NUM];
	int				mn_limit_used[MAX_WAIT_TIME_NUM];
	
	CString			mstr_wait_time_name[MAX_WAIT_TIME_NUM];
	
	int mn_time_count[100][2];
	
	int mn_enable;

	CEXDigitST  m_dgt_ta_avg_bd;	// Ta 평균산출 보드 최소수량. 2K14/12/17/ViboX
	
	CEXDigitST  m_dgt_ta_etlb;		// Ta 하한값. 2K15/03/03/ViboX
	CEXDigitST  m_dgt_ta_etub;		// Ta 상한값. 2K15/03/03/ViboX
	CEXDigitST  m_dgt_temp_check;		// Temp Check. 2K15/03/03/ViboX

	int mn_ta_avg_bd_min[2];
	
	int mn_ta_etlb[2];
	int mn_ta_etub[2];
	
	int mn_temp_check[2];

// Operations
public:
	void OnWaitTime_Display_Temp_Data();
	void OnWaitTime_Display_Delta_Data();
	void OnWaitTime_Init_Delta_Data();
	void OnWaitTime_Init_Temp_Data();
	void OnWaitTime_GroupBox_Set();
	void On_Controls_Enable(int nState);
	void OnWaitTime_Init_Time();
	void OnWaitTime_LogFile_Create();
	void OnWaitTime_Data_Apply();
	void OnWaitTime_Data_BackUp();
	int OnWaitTime_Data_Comparison();
	void OnWaitTime_Data_Display();
	void OnWaitTime_Data_Recovery();
	void OnWaitTime_Data_Set();
	void OnChangeValue(int nid, int nrow, int ncol);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Wait_Time)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Wait_Time();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Wait_Time)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnWaitApply();
	afx_msg void OnChkTaFuncUsed();
	afx_msg void OnDgtTaAvgBd();
	afx_msg void OnDgtTaEtlb();
	afx_msg void OnDgtTaEtub();
	afx_msg void OnDgtTempCheck();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWaitTime_Info_Apply(WPARAM wParam, LPARAM lParam);  // Wait Time 화면 정보 갱신 요청 메시지
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_WAIT_TIME_H__F41690DE_3119_41C4_AEBD_B5B088DAF340__INCLUDED_)
