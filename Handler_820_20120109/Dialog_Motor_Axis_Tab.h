#if !defined(AFX_DIALOG_MOTOR_AXIS_TAB_H__873E2E2D_FC0F_426C_A66C_CE0DAE9A1E58__INCLUDED_)
#define AFX_DIALOG_MOTOR_AXIS_TAB_H__873E2E2D_FC0F_426C_A66C_CE0DAE9A1E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Motor_Axis_Tab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_Tab window
#include "Variable.h"

class CDialog_Motor_Axis_1;
// class CDialog_Motor_Axis_2;
// class CDialog_Motor_Axis_3;
// class CDialog_Motor_Axis_4;
// class CDialog_Motor_Axis_5;
// class CDialog_Motor_Axis_6;
// class CDialog_Motor_Axis_7;
// class CDialog_Motor_Axis_8;
// class CDialog_Motor_Axis_9;
// class CDialog_Motor_Axis_10;

class CDialog_Motor_Axis_Tab : public CTabCtrl
{
public:
	int							m_n_part;
	int							m_n_motor_axis_cnt;
	int							m_n_motor_axis[10];

	CString						m_str_motor_axis[10];

	CDialog_Motor_Axis_1		*m_p_dialog_motor_axis_1;
// 	CDialog_Motor_Axis_2		*m_p_dialog_motor_axis_2;
// 	CDialog_Motor_Axis_3		*m_p_dialog_motor_axis_3;
// 	CDialog_Motor_Axis_4		*m_p_dialog_motor_axis_4;
// 	CDialog_Motor_Axis_5		*m_p_dialog_motor_axis_5;
// 	CDialog_Motor_Axis_6		*m_p_dialog_motor_axis_6;
// 	CDialog_Motor_Axis_7		*m_p_dialog_motor_axis_7;
// 	CDialog_Motor_Axis_8		*m_p_dialog_motor_axis_8;
// 	CDialog_Motor_Axis_9		*m_p_dialog_motor_axis_9;
// 	CDialog_Motor_Axis_10		*m_p_dialog_motor_axis_10;
// Construction
public:
	CDialog_Motor_Axis_Tab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Motor_Axis_Tab)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnMotor_Axis_Create();
	void OnMotor_Axis_Delete();
	void OnMotor_Axis_Change(int n_mode);
	void Init_Tab(CWnd *p_wnd, int n_part);
	virtual ~CDialog_Motor_Axis_Tab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialog_Motor_Axis_Tab)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MOTOR_AXIS_TAB_H__873E2E2D_FC0F_426C_A66C_CE0DAE9A1E58__INCLUDED_)
