#if !defined(AFX_SCREEN_INITIAL_H__D700A6C5_7D59_4037_85CE_3C394D80183F__INCLUDED_)
#define AFX_SCREEN_INITIAL_H__D700A6C5_7D59_4037_85CE_3C394D80183F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Initial.h : header file
//

#include "Variable.h"				// ���� ���� ���� Ŭ���� �߰� 
#include "PictureEx.h"				// GIF ���� �ε� Ŭ���� �߰� 
#include "GradientStatic.h"			// �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XPButton.h"				// XP ���� Į�� ��ư ���� Ŭ���� �߰� 
#include "LedButton.h"				// LED ��ư ���� Ŭ���� �߰� 
#include "Public_Function.h"
#include "MacButtons.h"
#include "SuperProgressCtrl.h"		// ProgressCtrl ���� Ŭ���� �߰�

#include "Thread.h"					// ������ ���� Ŭ���� �߰� 
// ******************************************************************************
// ������ ���� ���� Ŭ���� �߰�                                                  
// ******************************************************************************
//#include "Run_Dispenser.h"
//#include "Run_Elevator_HeatSink.h"
//#include "Run_Elevator_Module.h"
//#include "Run_Lower_Base.h"
//#include "Run_Robot_HeatSink.h"
//#include "Run_Robot_Module.h"
//#include "Run_Transfer_HeatSink.h"
//#include "Run_Transfer_Module.h"
//#include "Run_Upper_Base.h"
//#include "Run_Whirler.h"

//#include "Run_Handler_Check.h"
//#include "Run_TowerLamp.h"
// ******************************************************************************

/* ****************************************************************************** */
/* ���� ���� �ε� �� ���� Ŭ����                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"

/* ****************************************************************************** */
/* �ʱ�ȭ ȭ�� ���� Ÿ�̸� ID ����                                                */
/* ****************************************************************************** */
#define TMR_INIT	710  // �ʱ�ȭ �۾� ���� Ÿ�̸� ID ���� 
/* ****************************************************************************** */
/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Run_Handler_Check.h"

class CScreen_Initial : public CFormView
{
public:
	CScreen_Initial();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Initial)

// Form Data
public:
	//{{AFX_DATA(CScreen_Initial)
	enum { IDD = IDD_SCREEN_INITIAL };
	CLedButton	m_radio_transfer_right;
	CLedButton	m_radio_transfer_left;
	CLedButton	m_radio_test_site;
	CLedButton	m_radio_test_dump;
	CLedButton	m_radio_rotator_table;
	CLedButton	m_radio_rotator_dump;
	CLedButton	m_radio_main_xyz;
	CLedButton	m_radio_elevator_right;
	CLedButton	m_radio_elevator_left;
	CLedButton	m_radio_conveyor;
	CLedButton	m_radio_mot_bd;
	CLedButton	m_radio_io_bd;
	CGradientStatic	m_msg_initial;
	CPictureEx	m_gif_initializing;
	CMacButton	m_btn_init_skip;
	CMacButton	m_btn_init_retry;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_init_font;  // ��Ʈ ���� ���� ���� 

	/* ************************************************************************** */
	/* ȯ�� ���� ������ �ε� �� ���� Ŭ���� ���� ����                             */
	/* ************************************************************************** */
	CMyBasicData     mcls_m_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	/* ************************************************************************** */

	UINT m_nDataSendCnt;

	int mn_motor_init_count;
	int mn_pos_step;				// ���α׷��� ��ġ ���� ���� ����

	/* ************************************************************************** */
	/* �ʱ�ȭ ���� : Step ������ ���� ���� �� ���� �÷��� ����                    */
	/* ************************************************************************** */
	long ml_init_step ;
	int mn_init_flag;
	int m_nleft_init;
	int m_nright_init;
	/* ************************************************************************** */

// Operations
public:
	void OnInitial_Index_Table_Check();
	void OnInitial_TestDump_Check();
	void OnInitial_IndexDump_Check();
	int OnInitial_Init_Excution();
	void OnInitial_Initial_Ready();
	void OnInitial_Step_Clear();
	void OnInitial_Change_Status(int n_status);
	void OnInitial_Controls_Enable(int n_init);
	void OnInitial_RadioButton_Set();
	void OnInitial_Progress_Set();
	void OnInitial_Picture_Set();
	void OnInitial_Lable_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Initial)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSuperProgressCtrl m_ctrlProgress;
	CSuperProgressCtrl m_ctrlProgress1;
	
	virtual ~CScreen_Initial();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Initial)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnInitSkip();
	afx_msg void OnBtnInitRetry();
	//}}AFX_MSG
	afx_msg LRESULT OnWorkMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_INITIAL_H__D700A6C5_7D59_4037_85CE_3C394D80183F__INCLUDED_)
