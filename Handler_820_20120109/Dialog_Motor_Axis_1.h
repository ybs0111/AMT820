#if !defined(AFX_DIALOG_MOTOR_AXIS_1_H__2C597DAB_C483_4F88_AE46_F5EBF7E4FAFA__INCLUDED_)
#define AFX_DIALOG_MOTOR_AXIS_1_H__2C597DAB_C483_4F88_AE46_F5EBF7E4FAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Motor_Axis_1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_1 dialog
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"
#include "Picture.h"

class CDialog_Motor_Axis_1 : public CDialog
{
public:
	CPicture		m_p_bmp_view;
	GridControlAlg	m_p_grid;
	
	BOOL			m_b_stop_req;	// ESTOP 요청 플래그 초기화
	
	TSpread			*m_grid_motor_pos;
	TSpread			*m_grid_motor_info;
	
	double			m_d_data;
	double			m_d_position[25][2];
	
	int				m_n_curr_pos;
	int				m_n_move_mode;
	int				m_n_axis;
	int				m_n_part;
	int				m_n_position_move;
	int				m_n_cmd_id;
	int				m_n_cmd_row;
	int				m_n_cmd_col;
	int				m_n_position_num;
	int				m_n_actioncnt;
	int				m_n_conti_wait;
	int				m_n_plus_el;
	int				m_n_minus_el;
	int				m_n_home;
	int				m_n_sd;
	int				m_n_direction;
	int				m_n_account;
	int				m_n_cmd_no;			// 실행 명령 번호 저장 변수 초기화
	int				m_n_cmd_motor_no;	// 모터 번호 저장 변수 초기화
	int				m_n_home_step;		// 모터 HOMING 처리 스텝 저장 변수 초기화
	int				m_n_move_step;
	int				m_n_retry_cnt;
	int				m_n_retry_wait;
	int				m_n_allow[2];
	int				m_n_per_jog[2];
	int				m_n_per_home[2];
	int				m_n_per_run[2];
	int				m_n_per_manual[2];
	int				m_n_acc[2];
	int				m_n_dec[2];
	int				m_n_max_speed[2];
	
	CString			m_str_cmd_motor;
	CString			m_str_position_name[25];
	CString			m_str_position_bmp[25];
	CString			m_str_axis_name;
	CFont			* m_p_font;

	TSpread			*m_grid_input;
	TSpread			*m_grid_output;

	int				m_n_in[32];
	int				m_n_in_port[32];
	int				m_n_in_num[32];
	int				m_n_out[32];
	int				m_n_out_port[32];
	int				m_n_out_num[32];
	int				m_n_in_cnt;
	int				m_n_out_cnt;

	CString			m_str_in[32];
	CString			m_str_out[32];

// Construction
public:
	void OnOutputCheck();
	void OnInputCheck();
	void Init_Output();
	void Init_Input();
	void Init_Grid_IO();
	void	SetDataForSelected(int nSelected);
	void	Init_Grid_Info();
	void	OnButtonControl(BOOL b_flag);
	int		MotorHomingExcution();
	void	MotorCompletion(int nrow, int ncol);
	void	MotorMoveStepClear();
	void	MotorPositionCheck();
	void	MotorStatusCheck();
	void	Data_HistoryLog();
	void	Data_Recovery();
	int		Data_Comparison();
	void	Data_Apply();
	void	Data_Backup();
	void	Data_Init(int nSelected);
	void	Init_Grid_Pos();
	void	Init_Radio();
	void	Init_Digit();
	void	Init_Group();
	void	Init_Label();
	void	Init_Button();
	CDialog_Motor_Axis_1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Motor_Axis_1)
	enum { IDD = IDD_MOTOR_AXIS_1 };
	CMacButton	m_btn_apply;
	CButtonST	m_btn_right;
	CButtonST	m_btn_left;
	CButtonST	m_btn_home;
	CXPGroupBox	m_group_motor;
	CXPGroupBox	m_group_move;
	CButtonST	m_btn_emg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Motor_Axis_1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Motor_Axis_1)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBtnEmg();
	afx_msg void OnBtnHome();
	afx_msg void OnBtnApply();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotorCompletion(WPARAM wParam,LPARAM lParam);

	LONG OnUserLButtonDown(WPARAM wParam, LPARAM lParam);
	LONG OnUserLButtonUp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MOTOR_AXIS_1_H__2C597DAB_C483_4F88_AE46_F5EBF7E4FAFA__INCLUDED_)
