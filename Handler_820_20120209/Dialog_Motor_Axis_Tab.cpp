// Dialog_Motor_Axis_Tab.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Motor_Axis_Tab.h"
#include "Dialog_Motor_Axis_1.h"
// #include "Dialog_Motor_Axis_2.h"
// #include "Dialog_Motor_Axis_3.h"
// #include "Dialog_Motor_Axis_4.h"
// #include "Dialog_Motor_Axis_5.h"
// #include "Dialog_Motor_Axis_6.h"
// #include "Dialog_Motor_Axis_7.h"
// #include "Dialog_Motor_Axis_8.h"
// #include "Dialog_Motor_Axis_9.h"
// #include "Dialog_Motor_Axis_10.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_Tab
CDialog_Motor_Axis_Tab::CDialog_Motor_Axis_Tab()
{
	m_p_dialog_motor_axis_1		= NULL;
// 	m_p_dialog_motor_axis_2		= NULL;
// 	m_p_dialog_motor_axis_3		= NULL;
// 	m_p_dialog_motor_axis_4		= NULL;
// 	m_p_dialog_motor_axis_5		= NULL;
// 	m_p_dialog_motor_axis_6		= NULL;
// 	m_p_dialog_motor_axis_7		= NULL;
// 	m_p_dialog_motor_axis_8		= NULL;
// 	m_p_dialog_motor_axis_9		= NULL;
// 	m_p_dialog_motor_axis_10	= NULL;
}

CDialog_Motor_Axis_Tab::~CDialog_Motor_Axis_Tab()
{
	OnMotor_Axis_Delete();
}


BEGIN_MESSAGE_MAP(CDialog_Motor_Axis_Tab, CTabCtrl)
	//{{AFX_MSG_MAP(CDialog_Motor_Axis_Tab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_Tab message handlers

void CDialog_Motor_Axis_Tab::Init_Tab(CWnd *p_wnd, int n_part)
{
	m_n_part	= n_part;

	OnMotor_Axis_Create();
	OnMotor_Axis_Change(0);
}

void CDialog_Motor_Axis_Tab::OnMotor_Axis_Change(int n_mode)
{
	if (st_handler.n_menu_lock) return;

	if (m_p_dialog_motor_axis_1 != NULL)
	{
		m_p_dialog_motor_axis_1->SetDataForSelected(n_mode);
	}
}

void CDialog_Motor_Axis_Tab::OnMotor_Axis_Delete()
{
	if(m_p_dialog_motor_axis_1 != NULL)
	{
		m_p_dialog_motor_axis_1->DestroyWindow();
		
		delete m_p_dialog_motor_axis_1;
		m_p_dialog_motor_axis_1 = NULL;
	}
}

void CDialog_Motor_Axis_Tab::OnMotor_Axis_Create()
{

	if (m_p_dialog_motor_axis_1 == NULL)
	{
		m_p_dialog_motor_axis_1				= new CDialog_Motor_Axis_1;
		m_p_dialog_motor_axis_1->m_n_part	= m_n_part;
		m_p_dialog_motor_axis_1->Create(IDD_MOTOR_AXIS_1, this);
		m_p_dialog_motor_axis_1->ShowWindow(SW_SHOW);
	}
}
