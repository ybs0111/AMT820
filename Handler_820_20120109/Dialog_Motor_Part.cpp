// Dialog_Motor_Part.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Motor_Part.h"
#include "Public_Function.h"
#include "Dialog_Part_IO.h"
#include "Dialog_Manual_Move.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Part dialog
#define WM_DIALOG_EXIT WM_USER+100

CDialog_Motor_Part::CDialog_Motor_Part(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Motor_Part::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Motor_Part)
	//}}AFX_DATA_INIT
	Func.m_p_io	= NULL;
	m_n_axis_sel			= 0;
}


void CDialog_Motor_Part::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Motor_Part)
	DDX_Control(pDX, IDC_BTN_MANUAL_MOVE, m_btn_manual_move);
	DDX_Control(pDX, IDC_BTN_IO, m_btn_io);
	DDX_Control(pDX, IDC_TAB_MOTOR_PART, m_tab_motor_part);
	DDX_Control(pDX, IDC_CHK_AXIS_9, m_chk_axis_9);
	DDX_Control(pDX, IDC_CHK_AXIS_8, m_chk_axis_8);
	DDX_Control(pDX, IDC_CHK_AXIS_7, m_chk_axis_7);
	DDX_Control(pDX, IDC_CHK_AXIS_6, m_chk_axis_6);
	DDX_Control(pDX, IDC_CHK_AXIS_5, m_chk_axis_5);
	DDX_Control(pDX, IDC_CHK_AXIS_4, m_chk_axis_4);
	DDX_Control(pDX, IDC_CHK_AXIS_3, m_chk_axis_3);
	DDX_Control(pDX, IDC_CHK_AXIS_2, m_chk_axis_2);
	DDX_Control(pDX, IDC_CHK_AXIS_10, m_chk_axis_10);
	DDX_Control(pDX, IDC_CHK_AXIS_1, m_chk_axis_1);
	DDX_Control(pDX, IDC_GROUP_PART, m_group_part);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btn_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Motor_Part, CDialog)
	//{{AFX_MSG_MAP(CDialog_Motor_Part)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_CHK_AXIS_1, OnChkAxis1)
	ON_BN_CLICKED(IDC_CHK_AXIS_10, OnChkAxis10)
	ON_BN_CLICKED(IDC_CHK_AXIS_2, OnChkAxis2)
	ON_BN_CLICKED(IDC_CHK_AXIS_3, OnChkAxis3)
	ON_BN_CLICKED(IDC_CHK_AXIS_4, OnChkAxis4)
	ON_BN_CLICKED(IDC_CHK_AXIS_5, OnChkAxis5)
	ON_BN_CLICKED(IDC_CHK_AXIS_6, OnChkAxis6)
	ON_BN_CLICKED(IDC_CHK_AXIS_7, OnChkAxis7)
	ON_BN_CLICKED(IDC_CHK_AXIS_8, OnChkAxis8)
	ON_BN_CLICKED(IDC_CHK_AXIS_9, OnChkAxis9)
	ON_BN_CLICKED(IDC_BTN_IO, OnBtnIo)
	ON_BN_CLICKED(IDC_BTN_MANUAL_MOVE, OnBtnManualMove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Part message handlers

BOOL CDialog_Motor_Part::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect_area;

	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	GetWindowRect(&rect_area);
	MoveWindow(m_cp_coord.x, m_cp_coord.y, rect_area.Width(), rect_area.Height());

	Init_Group();
	Init_Button();

	m_tab_motor_part.Init_Tab(this, m_n_part);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Motor_Part::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock) return;

	DestroyWindow();
}

BOOL CDialog_Motor_Part::Create()
{
	return CDialog::Create( CDialog_Motor_Part::IDD );
}


void CDialog_Motor_Part::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	if(Func.m_p_io != NULL)
	{
		Func.m_p_io->DestroyWindow();
	}

	if(Func.m_p_move != NULL)
	{
		Func.m_p_move->DestroyWindow();
	}

	delete this;
	Func.m_p_motor = NULL;
	
	CDialog::PostNcDestroy();
}

void CDialog_Motor_Part::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_part.SetFont(main_font);
	m_group_part.SetCatptionTextColor(BLUE_C);
	m_group_part.SetBorderColor(YELLOW_L);
	m_group_part.SetFontBold(TRUE);
	m_group_part.SetWindowText(m_str_part_name);
}

void CDialog_Motor_Part::Init_Button()
{
	int i;
	short	shBtnColor = 30;

	m_btn_exit.SetFont(m_p_font);
	m_btn_exit.SetIcon(IDI_EXIT);
	m_btn_exit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_exit.SetButtonColor(1);

	m_btn_io.SetFont(m_p_font);
	m_btn_io.SetIcon(IDI_IO);
	m_btn_io.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_io.SetButtonColor(1);

	m_btn_manual_move.SetFont(m_p_font);
	m_btn_manual_move.SetIcon(IDI_MOTOR);
	m_btn_manual_move.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	
	m_chk_axis_1.ShowWindow(SW_HIDE);
	m_chk_axis_2.ShowWindow(SW_HIDE);
	m_chk_axis_3.ShowWindow(SW_HIDE);
	m_chk_axis_4.ShowWindow(SW_HIDE);
	m_chk_axis_5.ShowWindow(SW_HIDE);
	m_chk_axis_6.ShowWindow(SW_HIDE);
	m_chk_axis_7.ShowWindow(SW_HIDE);
	m_chk_axis_8.ShowWindow(SW_HIDE);
	m_chk_axis_9.ShowWindow(SW_HIDE);
	m_chk_axis_10.ShowWindow(SW_HIDE);

	for(i=0; i<st_motor_info.n_part_axis_cnt[m_n_part]; i++)
	{
		switch(i)
		{
			case 0:
				m_chk_axis_1.SetFont(m_p_font);
				m_chk_axis_1.SetIcon(IDI_MOTOR);
				m_chk_axis_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_1.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_1.SetButtonColor(1);
				
				m_chk_axis_1.SetCheck(TRUE);
				m_chk_axis_1.ShowWindow(SW_SHOW);
				break;

			case 1:
				m_chk_axis_2.SetFont(m_p_font);
				m_chk_axis_2.SetIcon(IDI_MOTOR);
				m_chk_axis_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_2.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_2.SetButtonColor(1);

				m_chk_axis_2.ShowWindow(SW_SHOW);
				break;

			case 2:
				m_chk_axis_3.SetFont(m_p_font);
				m_chk_axis_3.SetIcon(IDI_MOTOR);
				m_chk_axis_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_3.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_3.SetButtonColor(1);

				m_chk_axis_3.ShowWindow(SW_SHOW);
				break;

			case 3:
				m_chk_axis_4.SetFont(m_p_font);
				m_chk_axis_4.SetIcon(IDI_MOTOR);
				m_chk_axis_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_4.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_4.SetButtonColor(1);

				m_chk_axis_4.ShowWindow(SW_SHOW);
				break;

			case 4:
				m_chk_axis_5.SetFont(m_p_font);
				m_chk_axis_5.SetIcon(IDI_MOTOR);
				m_chk_axis_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_5.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_5.SetButtonColor(1);

				m_chk_axis_5.ShowWindow(SW_SHOW);
				break;

			case 5:
				m_chk_axis_6.SetFont(m_p_font);
				m_chk_axis_6.SetIcon(IDI_MOTOR);
				m_chk_axis_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_6.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_6.SetButtonColor(1);

				m_chk_axis_6.ShowWindow(SW_SHOW);
				break;

			case 6:
				m_chk_axis_7.SetFont(m_p_font);
				m_chk_axis_7.SetIcon(IDI_MOTOR);
				m_chk_axis_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_7.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_7.SetButtonColor(1);

				m_chk_axis_7.ShowWindow(SW_SHOW);
				break;

			case 7:
				m_chk_axis_8.SetFont(m_p_font);
				m_chk_axis_8.SetIcon(IDI_MOTOR);
				m_chk_axis_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_8.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_8.SetButtonColor(1);

				m_chk_axis_8.ShowWindow(SW_SHOW);
				break;

			case 8:
				m_chk_axis_9.SetFont(m_p_font);
				m_chk_axis_9.SetIcon(IDI_MOTOR);
				m_chk_axis_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_9.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_9.SetButtonColor(1);

				m_chk_axis_9.ShowWindow(SW_SHOW);
				break;

			case 9:
				m_chk_axis_10.SetFont(m_p_font);
				m_chk_axis_10.SetIcon(IDI_MOTOR);
				m_chk_axis_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
				m_chk_axis_10.SetWindowText(st_motor_info.str_part_axis_name[m_n_part][i]);
				m_chk_axis_10.SetButtonColor(1);

				m_chk_axis_10.ShowWindow(SW_SHOW);
				break;
				
		}
	}
}


void CDialog_Motor_Part::OnChkAxis1() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_1.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_1.GetCheck() == FALSE)
		{
			m_chk_axis_1.SetCheck(TRUE);
			return;
		}

		
	}

	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}

	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}

	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}

	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}

	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}

	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}

	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}

	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}

	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 0;
	m_tab_motor_part.OnMotor_Axis_Change(0);
}

void CDialog_Motor_Part::OnChkAxis10() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_10.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_10.GetCheck() == FALSE)
		{
			m_chk_axis_10.SetCheck(TRUE);
			return;
		}	
	}

	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}

	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	m_n_axis_sel = 9;
	m_tab_motor_part.OnMotor_Axis_Change(9);
}

void CDialog_Motor_Part::OnChkAxis2() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_2.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_2.GetCheck() == FALSE)
		{
			m_chk_axis_2.SetCheck(TRUE);
			return;
		}
	}

	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}
	
	m_n_axis_sel = 1;
	m_tab_motor_part.OnMotor_Axis_Change(1);
}

void CDialog_Motor_Part::OnChkAxis3() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_3.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_3.GetCheck() == FALSE)
		{
			m_chk_axis_3.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}
	
	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 2;
	m_tab_motor_part.OnMotor_Axis_Change(2);
}

void CDialog_Motor_Part::OnChkAxis4() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_4.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_4.GetCheck() == FALSE)
		{
			m_chk_axis_4.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}
	
	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 3;
	m_tab_motor_part.OnMotor_Axis_Change(3);
}

void CDialog_Motor_Part::OnChkAxis5() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_5.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_5.GetCheck() == FALSE)
		{
			m_chk_axis_5.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}
	
	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 4;
	m_tab_motor_part.OnMotor_Axis_Change(4);
}

void CDialog_Motor_Part::OnChkAxis6() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_6.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_6.GetCheck() == FALSE)
		{
			m_chk_axis_6.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}

	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 5;
	m_tab_motor_part.OnMotor_Axis_Change(5);
}

void CDialog_Motor_Part::OnChkAxis7() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_7.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_7.GetCheck() == FALSE)
		{
			m_chk_axis_7.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}

	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}
	
	m_n_axis_sel = 6;
	m_tab_motor_part.OnMotor_Axis_Change(6);
}

void CDialog_Motor_Part::OnChkAxis8() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_8.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_8.GetCheck() == FALSE)
		{
			m_chk_axis_8.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}

	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_9.GetCheck())
	{
		m_chk_axis_9.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 7;
	m_tab_motor_part.OnMotor_Axis_Change(7);
}

void CDialog_Motor_Part::OnChkAxis9() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock == TRUE)
	{
		OnAxis_Position(m_n_axis_sel);

		m_chk_axis_9.SetCheck(FALSE);
		return;
	}
	else
	{
		if(m_chk_axis_9.GetCheck() == FALSE)
		{
			m_chk_axis_9.SetCheck(TRUE);
			return;
		}
	}
	
	if(m_chk_axis_1.GetCheck())
	{
		m_chk_axis_1.SetCheck(FALSE);
	}

	if(m_chk_axis_2.GetCheck())
	{
		m_chk_axis_2.SetCheck(FALSE);
	}
	
	if(m_chk_axis_3.GetCheck())
	{
		m_chk_axis_3.SetCheck(FALSE);
	}
	
	if(m_chk_axis_4.GetCheck())
	{
		m_chk_axis_4.SetCheck(FALSE);
	}
	
	if(m_chk_axis_5.GetCheck())
	{
		m_chk_axis_5.SetCheck(FALSE);
	}
	
	if(m_chk_axis_6.GetCheck())
	{
		m_chk_axis_6.SetCheck(FALSE);
	}
	
	if(m_chk_axis_7.GetCheck())
	{
		m_chk_axis_7.SetCheck(FALSE);
	}
	
	if(m_chk_axis_8.GetCheck())
	{
		m_chk_axis_8.SetCheck(FALSE);
	}
	
	if(m_chk_axis_10.GetCheck())
	{
		m_chk_axis_10.SetCheck(FALSE);
	}

	m_n_axis_sel = 8;
	m_tab_motor_part.OnMotor_Axis_Change(8);
}

void CDialog_Motor_Part::OnBtnIo() 
{
	// TODO: Add your control notification handler code here
	CRect rectArea;
	
	GetDlgItem(IDC_BTN_IO)->GetWindowRect(rectArea);

	if(Func.m_p_move != NULL)
	{
		Func.m_p_move->DestroyWindow();
	}

	if(Func.m_p_io == NULL)
	{
		Func.m_p_io = new CDialog_Part_IO;
		Func.m_p_io->m_n_part = m_n_part;
		Func.m_p_io->m_cp_coord.x	= rectArea.left;
		Func.m_p_io->m_cp_coord.y	= rectArea.top;

		Func.m_p_io->Create();
		Func.m_p_io->ShowWindow(SW_SHOW);
	}
	else
	{
		Func.m_p_io->SetFocus();
		Func.m_p_io->ShowWindow(SW_SHOW);
	}
}


void CDialog_Motor_Part::OnAxis_Position(int n_pos)
{
	switch(n_pos)
	{
		case 0:
			m_chk_axis_1.SetCheck(TRUE);
			break;

		case 1:
			m_chk_axis_2.SetCheck(TRUE);
			break;

		case 2:
			m_chk_axis_3.SetCheck(TRUE);
			break;

		case 3:
			m_chk_axis_4.SetCheck(TRUE);
			break;

		case 4:
			m_chk_axis_5.SetCheck(TRUE);
			break;

		case 5:
			m_chk_axis_6.SetCheck(TRUE);
			break;

		case 6:
			m_chk_axis_7.SetCheck(TRUE);
			break;

		case 7:
			m_chk_axis_8.SetCheck(TRUE);
			break;

		case 8:
			m_chk_axis_9.SetCheck(TRUE);
			break;

		case 9:
			m_chk_axis_10.SetCheck(TRUE);
			break;
	}
}

void CDialog_Motor_Part::OnBtnManualMove() 
{
	// TODO: Add your control notification handler code here
	CRect rectArea;
	
	GetDlgItem(IDC_BTN_MANUAL_MOVE)->GetWindowRect(rectArea);
	
	if(Func.m_p_io != NULL)
	{
		Func.m_p_io->DestroyWindow();
	}

	if(Func.m_p_move == NULL)
	{
		Func.m_p_move = new CDialog_Manual_Move;
		Func.m_p_move->m_n_part = m_n_part;
		Func.m_p_move->m_cp_coord.x	= rectArea.left;
		Func.m_p_move->m_cp_coord.y	= rectArea.top;
		
		Func.m_p_move->Create();
		Func.m_p_move->ShowWindow(SW_SHOW);
	}
	else
	{
		Func.m_p_move->SetFocus();
		Func.m_p_move->ShowWindow(SW_SHOW);
	}
}
