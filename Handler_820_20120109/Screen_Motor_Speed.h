#if !defined(AFX_SCREEN_MOTOR_SPEED_H__C78D7C1E_FF94_47D8_967A_3C2C4A07C145__INCLUDED_)
#define AFX_SCREEN_MOTOR_SPEED_H__C78D7C1E_FF94_47D8_967A_3C2C4A07C145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Motor_Speed.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"
#include "EXDigitST.h"			// 디지털 카운터 클래스
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"			// XP 칼라 버튼 클래스
#include "MacButtons.h"
#include "FloatST.h"
#include "Digit.h"
#include "MatrixStatic.h"	// LCD 문자 생성 클래스 추가 

/* ****************************************************************************** */
/*  정보 저장 및 로딩 클래스 추가                                                 */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */
/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor_Speed form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Motor_Speed : public CFormView
{
public:
	CScreen_Motor_Speed();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Motor_Speed)

// Form Data
public:
	//{{AFX_DATA(CScreen_Motor_Speed)
	enum { IDD = IDD_SCREEN_MOTOR_SPEED };
	CMatrixStatic	m_msg_saving;
	CGradientStatic	m_label_robot_name;
	CXPGroupBox	m_group_safety_interrupt;
	CMacCheckBox	m_chk_remove_robot;
	CMacCheckBox	m_chk_insert_robot;
	CMacCheckBox	m_chk_dculd_robot;
	CMacCheckBox	m_chk_dcld_robot;
	CDigit	m_dgt_run_rate;
	CDigit	m_dgt_manual_rate;
	CFloatST	m_msg_run_rate;
	CFloatST	m_msg_manual_rate;
	CXPGroupBox	m_group_motor_speed_set;
	CXPGroupBox	m_group_motor_speed_rate;
	CXPButton	m_btn_ms_apply;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_motorspeed_font;				// 폰트 정보 설정 변수

	CMyBasicData	mcls_motorspeed;		// 기본 셋팅 정보 로딩 및 저장 클래스
	TSpread			*m_grid_speed;
	GridControlAlg	mp_grid;

	// **************************************************************************
	// 모터 속도 데이터 저장 클래스 변수 선언
	// **************************************************************************
	int mn_manual_speed[2];
	int mn_run_speed[2];
	
	int mn_enable;
	
	double				md_acc[M_MOTOR_COUNT][2];
	double				md_dec[M_MOTOR_COUNT][2];
	double				md_vel[M_MOTOR_COUNT][2];
	double				md_home[M_MOTOR_COUNT][2];
	double				md_jog[M_MOTOR_COUNT][2];
	int					mn_allow[M_MOTOR_COUNT][2];
	double				md_allow[M_MOTOR_COUNT][2];			// 2K14/11/04/ViboX
	double				md_limit_p[M_MOTOR_COUNT][2];
	double				md_limit_m[M_MOTOR_COUNT][2];
	
	int					mn_motor_axis_cnt;
		
	CString				mstr_motor_axis_name[M_MOTOR_COUNT];
	
// Operations
public:
	void OnMotorSpeed_LogFile_Create();
	void OnMotorSpeed_Controls_Show(int n_state);
	void OnMotroSpeed_LCD_Digital_Set();
	void OnChangeValue(int nid, int nrow, int ncol);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	void OnMotorSpeed_Init_Layout();
	void OnMotorSpeed_Controls_Enable(bool b_state);
	void OnMotorSpeed_Digital_Counter_Set();
	void OnMotorSpeed_RATE_Digital_Counter_Set();
	void OnMotorSpeed_Data_BackUp();
	void OnMotorSpeed_Data_Apply();
	int OnMotorSpeed_Data_Comparison();
	void OnMotorSpeed_Data_Recovery();
	void OnMotorSpeed_Data_Set();
	void OnMotorSpeed_Label_Set();
	void OnMotorSpeed_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Motor_Speed)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Motor_Speed();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Motor_Speed)
	afx_msg void OnDestroy();
	afx_msg void OnBtnMsApply();
	afx_msg void OnDgtRunRate();
	afx_msg void OnDgtManualRate();
	afx_msg void OnChkDcldRobot();
	afx_msg void OnChkDculdRobot();
	afx_msg void OnChkInsertRobot();
	afx_msg void OnChkRemoveRobot();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotorSpeed_Info_Apply(WPARAM wParam, LPARAM lParam);  // Motor Speed 화면 정보 갱신 요청 메시지
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MOTOR_SPEED_H__C78D7C1E_FF94_47D8_967A_3C2C4A07C145__INCLUDED_)
