// Screen_Motor_Speed.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Motor_Speed.h"

#include "Variable.h"				// 전역 변수 정의 클래스 추가
// ******************************************************************************
// 대화 상자 클래스 추가                                                         
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Infor.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************

#include "ComizoaPublic.h"
//#include "CXGemManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor_Speed

IMPLEMENT_DYNCREATE(CScreen_Motor_Speed, CFormView)

CScreen_Motor_Speed::CScreen_Motor_Speed()
	: CFormView(CScreen_Motor_Speed::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Motor_Speed)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	st_handler.cwnd_motorspeed = this;  // Motor Speed 화면에 대한 핸들 정보 설정
}

CScreen_Motor_Speed::~CScreen_Motor_Speed()
{
}

void CScreen_Motor_Speed::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Motor_Speed)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_MSG_SAVING, m_msg_saving);
	DDX_Control(pDX, IDC_DGT_RUN_RATE, m_dgt_run_rate);
	DDX_Control(pDX, IDC_DGT_MANUAL_RATE, m_dgt_manual_rate);
	DDX_Control(pDX, IDC_MSG_RUN_RATE, m_msg_run_rate);
	DDX_Control(pDX, IDC_MSG_MANUAL_RATE, m_msg_manual_rate);
	DDX_Control(pDX, IDC_GROUP_MOTOR_SPEED_SET, m_group_motor_speed_set);
	DDX_Control(pDX, IDC_GROUP_MOTOR_SPEED_RATE, m_group_motor_speed_rate);
	DDX_Control(pDX, IDC_BTN_MS_APPLY, m_btn_ms_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Motor_Speed, CFormView)
	//{{AFX_MSG_MAP(CScreen_Motor_Speed)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_MS_APPLY, OnBtnMsApply)
	ON_BN_CLICKED(IDC_DGT_RUN_RATE, OnDgtRunRate)
	ON_BN_CLICKED(IDC_DGT_MANUAL_RATE, OnDgtManualRate)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE(WM_MOTORSPEED_APPLY, OnMotorSpeed_Info_Apply)  // Motor Speed DATA를 APPLY 시키기 위한 사용자 정의 메시지 선언
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor_Speed diagnostics

#ifdef _DEBUG
void CScreen_Motor_Speed::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Motor_Speed::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor_Speed message handlers

void CScreen_Motor_Speed::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

// 	mp_msg_dlg = new CDialog_Event_Msg;
// 	mp_msg_dlg->Create();
// 	mp_msg_dlg->SetParent(this);
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
	mp_motorspeed_font = NULL;
	mp_motorspeed_font = new CFont;
	mp_motorspeed_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	mn_enable = TRUE;

	OnMotorSpeed_GroupBox_Set();
	OnMotorSpeed_Label_Set();
	OnMotroSpeed_LCD_Digital_Set();
		
	OnMotorSpeed_RATE_Digital_Counter_Set();

// 	OnMotorSpeed_Data_Set();							//전역 변수의 Data를 받아온다.
// 	OnMotorSpeed_Data_BackUp();	

	OnMotorSpeed_Init_Layout();
	OnMotorSpeed_Digital_Counter_Set();

	if (st_work.m_iRunStatus != CTL_dSTOP)			// Stop 상태가 아니면 Apply Button이 비활성화가 된다.
	{
		OnMotorSpeed_Controls_Enable(false);		// 모든 컨트롤 화면 출력 제어 함수
	}

	OnMotorSpeed_Controls_Show(0);
}

void CScreen_Motor_Speed::OnDestroy() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret;

	//프로그램을 빠져나가면서 Save 하지 않고, Form이 Destroy되는것이라면 Setting을 포기한다는 말이다.
	//비교도 하지 말자.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnMotorSpeed_Data_Comparison();

		if (Ret == CTLBD_RET_ERROR)
		{
			//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.
			CDialog_Infor info_dlg;
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			mn_response = info_dlg.DoModal();
			//Stop_www_tipssoft_com();			// 2014.12.24 - Bredmin.
			
			if (mn_response == IDOK)
			{
				OnMotorSpeed_Data_Apply();
			}
			else if (mn_response == IDCANCEL)
			{
				OnMotorSpeed_Data_Recovery();

				OnMotorSpeed_Data_Apply();
			}	
		}
	}

	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_motorspeed_font;
	mp_motorspeed_font = NULL;
	// **************************************************************************

	// ************************************************************************** //
	// LCD 디지털 문자 스크롤 동작 정지한다                                       //
	// ************************************************************************** //
	m_msg_saving.StopScroll();
	// ************************************************************************** //

	st_handler.cwnd_motorspeed = NULL;		// Motor Speed 화면에 대한 핸들 정보 저장 변수 초기화 
	
	CFormView::OnDestroy();	
}

void CScreen_Motor_Speed::OnMotorSpeed_GroupBox_Set()
{
	CSxLogFont speed_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

	m_group_motor_speed_rate.SetFont(speed_font);
	m_group_motor_speed_rate.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor_speed_rate.SetFontBold(TRUE);

	m_group_motor_speed_set.SetFont(speed_font);
	m_group_motor_speed_set.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor_speed_set.SetFontBold(TRUE);
}

void CScreen_Motor_Speed::OnMotorSpeed_Label_Set()
{
	m_msg_run_rate.SetTextFont(mp_motorspeed_font);
	m_msg_run_rate.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_run_rate.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_manual_rate.SetTextFont(mp_motorspeed_font);
	m_msg_manual_rate.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_manual_rate.SetTextAlign(CFloatST::STA_CENTER, 10);
}

void CScreen_Motor_Speed::OnMotorSpeed_Data_Set()
{
	for (int i = 0; i < M_MOTOR_COUNT; i++)
	{
		md_vel[i][1] =		COMI.md_spd_vel[i][0];
		md_acc[i][1] =		COMI.md_spd_vel[i][1];
		md_dec[i][1] =		COMI.md_spd_vel[i][2];
		
		md_home[i][1] =		COMI.md_spd_home[i];
		md_jog[i][1] =		COMI.md_spd_jog[i];
		mn_allow[i][1] =	COMI.mn_allow_value[i];
		md_allow[i][1] =	COMI.md_allow_value[i];					// 2K14/11/04/ViboX

		md_limit_m[i][1]	= COMI.md_limit_position[i][0];
		md_limit_p[i][1]	= COMI.md_limit_position[i][1];
	}

	mn_run_speed[1] =		COMI.mn_runspeed_rate;
	mn_manual_speed[1] =	COMI.mn_manualspeed_rate;

	OnMotorSpeed_Data_BackUp();
}

void CScreen_Motor_Speed::OnMotorSpeed_Data_Recovery()
{
	for (int i = 0; i < M_MOTOR_COUNT; i++)
	{
		md_acc[i][1] =			md_acc[i][0];
		md_dec[i][1] =			md_dec[i][0];
		md_vel[i][1] =			md_vel[i][0];

		md_home[i][1] =			md_home[i][0];
		md_jog[i][1] =			md_jog[i][0];

		mn_allow[i][1] =		mn_allow[i][0];
		md_allow[i][1] =		md_allow[i][0];						// 2K14/11/04/ViboX

		md_limit_m[i][1] =		md_limit_m[i][0];
		md_limit_p[i][1] =		md_limit_p[i][0];
	}

	mn_run_speed[1] =		mn_run_speed[0];
	mn_manual_speed[1] =	mn_manual_speed[0];
}

int CScreen_Motor_Speed::OnMotorSpeed_Data_Comparison()
{
	int Ret = CTLBD_RET_GOOD;

	for (int i = 0; i < M_MOTOR_COUNT; i++)
	{
		if (md_acc[i][0] !=	md_acc[i][1])				Ret = CTLBD_RET_ERROR;
		if (md_dec[i][0] !=	md_dec[i][1])				Ret = CTLBD_RET_ERROR;
		if (md_vel[i][0] !=	md_vel[i][1])				Ret = CTLBD_RET_ERROR;

		if (md_home[i][0] != md_home[i][1])				Ret = CTLBD_RET_ERROR;
		if (md_jog[i][0] !=	md_jog[i][1])				Ret = CTLBD_RET_ERROR;

		if (mn_allow[i][0] != mn_allow[i][1])			Ret = CTLBD_RET_ERROR;
		if (md_allow[i][0] != md_allow[i][1])			Ret = CTLBD_RET_ERROR;		// 2K14/11/04/ViboX

		if (md_limit_m[i][0] !=	md_limit_m[i][1])		Ret = CTLBD_RET_ERROR;
		if (md_limit_p[i][0] !=	md_limit_p[i][1])		Ret = CTLBD_RET_ERROR;
	}

	if (mn_run_speed[0] != mn_run_speed[1])				Ret = CTLBD_RET_ERROR;
	if (mn_manual_speed[0] != mn_manual_speed[1])		Ret = CTLBD_RET_ERROR;

	return Ret;
}

void CScreen_Motor_Speed::OnMotorSpeed_Data_Apply()
{
	int i;

	for (i = 0; i < M_MOTOR_COUNT; i++)
	{
		COMI.md_spd_vel[i][0] =			md_vel[i][1];
		COMI.md_spd_vel[i][1] =			md_acc[i][1];
		COMI.md_spd_vel[i][2] =			md_dec[i][1];
		
		COMI.md_spd_home[i] =			md_home[i][1];

		COMI.md_spd_jog[i] =			md_jog[i][1];
		COMI.mn_allow_value[i] =		mn_allow[i][1];
		COMI.md_allow_value[i] =		md_allow[i][1];		// 2K14/11/04/ViboX

		COMI.md_limit_position[i][0] = md_limit_m[i][1];
		COMI.md_limit_position[i][1] = md_limit_p[i][1];
	}

	COMI.mn_runspeed_rate = mn_run_speed[1];
	COMI.mn_manualspeed_rate = mn_manual_speed[1];

	Func.OnMot_Speed_Setting();
}

void CScreen_Motor_Speed::OnMotorSpeed_Data_BackUp()
{
	for (int i = 0; i < M_MOTOR_COUNT; i++)
	{
		md_acc[i][0] =					md_acc[i][1];
		md_dec[i][0] =					md_dec[i][1];
		md_vel[i][0] =					md_vel[i][1];

		md_home[i][0] =					md_home[i][1];
		md_jog[i][0] =					md_jog[i][1];

		mn_allow[i][0] =				mn_allow[i][1];
		md_allow[i][0] =				md_allow[i][1];		// 2K14/11/04/ViboX

		md_limit_m[i][0] =				md_limit_m[i][1];
		md_limit_p[i][0] =				md_limit_p[i][1];
	}
	
	mn_run_speed[0] =					mn_run_speed[1];
	mn_manual_speed[0] =				mn_manual_speed[1];
}

// ******************************************************************************
//  Motor Speed 화면 정보 갱신 요청 메시지                                       
// ******************************************************************************
LRESULT CScreen_Motor_Speed::OnMotorSpeed_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnMotorSpeed_Data_Apply();			// 화면 셋팅 정보 전역 변수에 전송 함수

	OnMotorSpeed_Data_BackUp();

	return 0;
}

void CScreen_Motor_Speed::OnMotorSpeed_RATE_Digital_Counter_Set()
{	
	m_dgt_run_rate.SetStyle(CDigit::DS_INT, 3, CDigit::DC_BLACK);
	m_dgt_manual_rate.SetStyle(CDigit::DS_INT, 3, CDigit::DC_BLACK);
}

void CScreen_Motor_Speed::OnMotorSpeed_Digital_Counter_Set()
{
	m_dgt_run_rate.SetVal(mn_run_speed[1]);
	m_dgt_manual_rate.SetVal(mn_manual_speed[1]);
}

void CScreen_Motor_Speed::OnBtnMsApply() 
{
	int n_response;  // 대화 상자 리턴 플래그

	//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	CDialog_Select  select_dlg;

	st_msg.mstr_confirm_msg = _T("Motor Speed : Setting Data Apply!");
	n_response = select_dlg.DoModal();
	//Stop_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	if (n_response == IDOK)
	{
		OnMotorSpeed_Controls_Show(1);
		OnMotorSpeed_Data_Apply();			// 화면 셋팅 정보 전역 변수에 전송 함수
		OnMotorSpeed_LogFile_Create();
		OnMotorSpeed_Data_BackUp();
		mcls_motorspeed.OnMotorSpeed_Set_Data_Save();
		OnMotorSpeed_Controls_Show(0);
		/*
		if( st_basic.m_iXGemMode == CTL_YES )
		{
			CMyBasicData    mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
			
			mcls_m_basic.OnPPID_Data_Save();
			
			if( g_XGemMgr.IsXGemRdy() )
			{
				g_XGemMgr.XGEMUploadRecipe(st_basic.mstr_device_name, st_path.mstr_ppid);
			}
		}
		*/
	}
	else if (n_response == IDCANCEL)
	{

	}
}

void CScreen_Motor_Speed::OnDgtRunRate() 
{
	CString str_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] = {NULL};
	int n_rate;

	if (mn_enable != TRUE)	return;
	
	n_rate = m_dgt_run_rate.GetVal();
	
	str_temp = _T("Running Speed Rate Set");
	
	CRect r;
	
	m_dgt_run_rate.GetWindowRect(&r);
	
	str_temp = KeyPad.GetNumEditString_I(1, 100, n_rate, str_temp, &r);
	
	mn_run_speed[1] = atoi(str_temp);
	m_dgt_run_rate.SetVal(mn_run_speed[1]);
}

void CScreen_Motor_Speed::OnDgtManualRate() 
{
	CString str_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;

	if (mn_enable != TRUE)	return;
	
	mn_manual_speed[1] = m_dgt_manual_rate.GetVal();
	str_temp = LPCTSTR(_itoa(mn_manual_speed[1], chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Manual Speed Rate Set");
    
	st_msg.mstr_keypad_val = str_temp;

	str_temp = KeyPad.GetNumEditString_I(10, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	mn_manual_speed[1] = atoi(str_temp);
	m_dgt_manual_rate.SetVal(mn_manual_speed[1]);
}

void CScreen_Motor_Speed::OnMotorSpeed_Controls_Enable(bool b_state)
{
	m_btn_ms_apply.EnableWindow(b_state);

	mn_enable = b_state;
}

void CScreen_Motor_Speed::OnMotorSpeed_Init_Layout()
{
	int i, j;
	CString strTmp;
	CString strHeader[9] = {"Axis", "Accel(ms)", "Decel(ms)", "Velocity(mm/s)", "Home(mm/s)", "Jog(mm/s)", "Allow(mm)", "- Limit", "+ Limit"};

	m_grid_speed = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_SPEED);
	
	mp_grid.GridFileOpen(m_grid_speed, st_path.mstr_motor_axis_map, "sheet1");
	                                   
	mn_motor_axis_cnt = 0;
	mn_motor_axis_cnt = atoi(mp_grid.GridCellText(m_grid_speed, 1, 2));
	
	if(mn_motor_axis_cnt == 0) return;
	
	for(i=0; i<mn_motor_axis_cnt; i++)
	{
		mstr_motor_axis_name[i]	= mp_grid.GridCellText(m_grid_speed, 4, (i+1)*2);
	}

	OnMotorSpeed_Data_Set();

	mp_grid.GridReset(m_grid_speed);
	// 대문자 
	mp_grid.GridCellSelectDisplay(m_grid_speed, TRUE);
	mp_grid.GridRowHeader(m_grid_speed, FALSE);
	mp_grid.GridColHeader(m_grid_speed, TRUE);
	mp_grid.GridHorizontal(m_grid_speed, FALSE);
	mp_grid.GridVertical(m_grid_speed, TRUE);
	mp_grid.GridAutoSize(m_grid_speed, FALSE);
	
	mp_grid.GridCellRows(m_grid_speed, mn_motor_axis_cnt);
	mp_grid.GridCellCols(m_grid_speed, 9);
	
	for(i=0; i<mn_motor_axis_cnt+1; i++)
	{
		mp_grid.GridCellHeight_L(m_grid_speed, i, (double)30);
		
		if(i == 0)
		{
			for(j=0; j<9; j++)
			{
				switch(j)
				{
					case 0:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)23);
						break;

					case 1:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 2:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 3:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 4:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 5:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 6:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 7:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;

					case 8:
						mp_grid.GridCellWidth_L(m_grid_speed, j+1, (double)12);
						break;
				}

				mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
				mp_grid.GridCellColor(m_grid_speed, i, j+1, BLUE_D, WHITE_C);
				mp_grid.GridCellText(m_grid_speed, i, j+1, strHeader[j]);

				mp_grid.GridCellControlStatic(m_grid_speed, i, j+1);
			}
		}
		else
		{
			for (j = 0; j < 9; j++)
			{
				switch(j)
				{
				case 0:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 23);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, BLUE_D, WHITE_C);
					mp_grid.GridCellText(m_grid_speed, i, j+1, mstr_motor_axis_name[i-1]);
					break;
					
				case 1:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, RED_L, BLACK_C);
					strTmp.Format("%d", (int)md_acc[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 2:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, GREEN_L, BLACK_C);
					strTmp.Format("%d", (int)md_dec[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 3:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, YELLOW_L, BLACK_C);
					strTmp.Format("%d", (int)md_vel[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 4:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, YELLOW_L, BLACK_C);
					strTmp.Format("%d", (int)md_home[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 5:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, YELLOW_L, BLACK_C);
					strTmp.Format("%d", (int)md_jog[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 6:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, SKY_C, BLACK_C);
//					strTmp.Format("%d", mn_allow[i-1][1]);
					strTmp.Format("%.3f", md_allow[i-1][1]);		// 2K14/11/04/ViboX
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 7:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, RED_L, BLACK_C);
					strTmp.Format("%.3f", md_limit_m[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
					
				case 8:
					mp_grid.GridCellWidth_L(m_grid_speed, j+1, 12);
					
					mp_grid.GridCellFont(m_grid_speed, i, j+1, "MS Sans Serif", 14);
					mp_grid.GridCellColor(m_grid_speed, i, j+1, GREEN_L, BLACK_C);
					strTmp.Format("%.3f", md_limit_p[i-1][1]);
					mp_grid.GridCellText(m_grid_speed, i, j+1, strTmp);
					break;
				}
				
				mp_grid.GridCellControlStatic(m_grid_speed, i, j+1);
			}
		}
	}
}

void CScreen_Motor_Speed::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

	Grid = NULL;
	delete Grid;
}

void CScreen_Motor_Speed::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
			Grid->SetTypeComboBox(&CellType, 0, strList);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}
	
	Grid->SetCellType(col,row,&CellType);

	Grid = NULL;
	delete Grid;
}

void CScreen_Motor_Speed::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CScreen_Motor_Speed::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Motor_Speed::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CScreen_Motor_Speed::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	int nId = -1;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	if (mn_enable != TRUE)	return;

	switch (wParam)
	{
	case IDC_CUSTOM_MOTOR_SPEED:
		nId = 0;
		break;
	}
	
	if (nId > -1)
	{
		if (lpcc->Col != 1)
		{
			OnChangeValue(nId, lpcc->Row, lpcc->Col);
		}
	}
}

void CScreen_Motor_Speed::OnChangeValue(int nid, int nrow, int ncol)
{
	TSpread *Grid; 
	char chvalue[100];
	char chText[100];
	char chPos[100];
	CString strText;
	CString strPos;
	double dvalue;
	CString strTmp, strTmp2;
	int i;

	Grid = NULL;
	delete Grid;	
	CString str_temp;	// 저장할 정보 임시 저장 변수 

	if (ncol == 4 || ncol == 5 || ncol == 6 || ncol == 8 || ncol == 9)
	{
//		if (st_handler.mn_level_speed != TRUE)
//		{
//			return;
//		}
	}
	
	if (nid == 0)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_SPEED);
	
		Grid->GetValue(ncol, nrow, chvalue);
		dvalue = atof(chvalue);
		strTmp.Format("%.2f", dvalue);

		Grid->GetValue(1, nrow, chText);
		strText = chText;
		Grid->GetValue(ncol, 1, chPos);
		strPos = chPos;

		st_msg.mstr_keypad_msg.Format("[%s Motor] %s speed set", strText, strPos);
		
		st_msg.mstr_keypad_val = strTmp;
		
		if (ncol == 2 || ncol == 3)
		{
			st_msg.mstr_pad_high_limit = "5000";
			st_msg.mstr_pad_low_limit = "10";
		}
		else if (ncol == 7)
		{
			st_msg.mn_dot_use = CTL_YES;
			st_msg.mstr_pad_high_limit = "10";
			st_msg.mstr_pad_low_limit = "0";
		}
		else if (ncol == 8)
		{
			st_msg.mstr_pad_high_limit = "5000";
			st_msg.mstr_pad_low_limit = "-100";
		}
		else
		{
			st_msg.mstr_pad_high_limit = "5000";
			st_msg.mstr_pad_low_limit = "1";
		}
				
		strTmp = KeyPad.GetNumEditString_D(double(atoi(st_msg.mstr_pad_low_limit)), double(atoi(st_msg.mstr_pad_high_limit)), atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
		
		if (nrow == 0)			// 전체
		{
			for (i = 0; i < M_MOTOR_COUNT; i++)
			{
				if (ncol == 1)
				{
				}
				else if (ncol == 2)
				{
					md_acc[i][1] = atof(strTmp);
					strTmp.Format("%.0f", md_acc[i][1]);
				}
				else if (ncol == 3)
				{
					md_dec[i][1] = atof(strTmp);
					strTmp.Format("%.0f", md_dec[i][1]);
				}
				else if (ncol == 4)
				{
					md_vel[i][1] = atof(strTmp);
					strTmp.Format("%.0f", md_vel[i][1]);
				}
				else if (ncol == 5)
				{
					md_home[i][1] = atof(strTmp);
					strTmp.Format("%.0f", md_home[i][1]);
				}
				else if (ncol == 6)
				{
					md_jog[i][1] = atof(strTmp);
					strTmp.Format("%.0f", md_jog[i][1]);
				}
				else if (ncol == 7)
				{
// 					mn_allow[i][1] = atoi(strTmp);
// 					strTmp.Format("%d", mn_allow[i][1]);
					md_allow[i][1] = atof(strTmp);				// 2K14/11/04/ViboX
					strTmp.Format("%.0f", md_allow[i][1]);
				}
				else if (ncol == 8)
				{
					md_limit_m[i][1] = atoi(strTmp);
					strTmp.Format("%.0f", md_limit_m[i][1]);
				}
				else if (ncol == 9)
				{
					md_limit_p[i][1] = atoi(strTmp);
					strTmp.Format("%.0f", md_limit_p[i][1]);
				}
				
				Grid->SetValue(ncol, i + 1, strTmp);
			}
			return;
		}

		if (ncol == 1)
		{
		}
		else if (ncol == 2)
		{
			md_acc[nrow-1][1] = atof(strTmp);
			strTmp.Format("%.0f", md_acc[nrow-1][1]);
		}
		else if (ncol == 3)
		{
			md_dec[nrow-1][1] = atof(strTmp);
			strTmp.Format("%.0f", md_dec[nrow-1][1]);
		}
		else if (ncol == 4)
		{
			md_vel[nrow-1][1] = atof(strTmp);
			strTmp.Format("%.0f", md_vel[nrow-1][1]);
		}
		else if (ncol == 5)
		{
			md_home[nrow-1][1] = atof(strTmp);
			strTmp.Format("%.0f", md_home[nrow-1][1]);
		}
		else if (ncol == 6)
		{
			md_jog[nrow-1][1] = atof(strTmp);
			strTmp.Format("%.0f", md_jog[nrow-1][1]);
		}
		else if (ncol == 7)
		{
// 			mn_allow[nrow-1][1] = atoi(strTmp);
// 			strTmp.Format("%d", mn_allow[nrow-1][1]);		// 2K14/11/04/ViboX
			md_allow[nrow-1][1] = atof(strTmp);
			strTmp.Format("%.3f", md_allow[nrow-1][1]);
		}
		
		Grid->SetValue(ncol, nrow, strTmp);
	}
}

void CScreen_Motor_Speed::OnMotroSpeed_LCD_Digital_Set()
{
	m_msg_saving.SetNumberOfLines(1);
	m_msg_saving.SetXCharsPerLine(28);
	m_msg_saving.SetSize(CMatrixStatic::LARGE);
	m_msg_saving.SetDisplayColors(RGB(0, 0, 0), RGB(255, 60, 0), RGB(103, 30, 0));
	m_msg_saving.AdjustClientXToSize(28);
	m_msg_saving.AdjustClientYToSize(1);
	m_msg_saving.SetAutoPadding(true, '!');
	m_msg_saving.DoScroll(300, CMatrixStatic::LEFT);
}

void CScreen_Motor_Speed::OnMotorSpeed_Controls_Show(int n_state)
{
	switch(n_state)
	{
	case 0 :
		m_msg_saving.ShowWindow(SW_HIDE);
		break;
		
	case 1: 
		m_msg_saving.ShowWindow(SW_SHOW);
		m_msg_saving.SetText(_T("Motor speed data saving~...."));
		break;
		
	default:
		break;
	}
}

void CScreen_Motor_Speed::OnMotorSpeed_LogFile_Create()
{
	CString str_msg;
	CString str_data;
	CString str_old_data;
	CString str_motor;

	int i;

	for (i = 0; i < M_MOTOR_COUNT; i++)
	{
		str_motor = Func.Get_MotorName(i);

		if (md_vel[i][1] != md_vel[i][0])
		{
			str_data.Format("%d", (int)md_vel[i][1]);
			str_old_data.Format("%d", (int)md_vel[i][0]);
			str_msg.Format("[Motor Speed] %s Robot Vel Speed was changed by %s To %s", str_motor, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (md_acc[i][1] != md_acc[i][0])
		{
			str_data.Format("%d", (int)md_acc[i][1]);
			str_old_data.Format("%d", (int)md_acc[i][0]);
			str_msg.Format("[Motor Speed] %s Robot Acc Speed was changed by %s To %s", str_motor, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (md_dec[i][1] != md_dec[i][0])
		{
			str_data.Format("%d", (int)md_dec[i][1]);
			str_old_data.Format("%d", (int)md_dec[i][0]);
			str_msg.Format("[Motor Speed] %s Robot Dec Speed was changed by %s To %s", str_motor, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		
		if (md_home[i][1] != md_home[i][0])
		{
			str_data.Format("%d", (int)md_home[i][1]);
			str_old_data.Format("%d", (int)md_home[i][0]);
			str_msg.Format("[Motor Speed] %s Robot Home Speed was changed by %s To %s", str_motor, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (md_jog[i][1] != md_jog[i][0])
		{
			str_data.Format("%d", (int)md_jog[i][1]);
			str_old_data.Format("%d", (int)md_jog[i][0]);
			str_msg.Format("[Motor Speed] %s Robot Jog Speed was changed by %s To %s", str_motor, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

// 		if (mn_allow[i][1] != mn_allow[i][0])
// 		{
// 			str_data.Format("%d", mn_allow[i][1]);
// 			str_old_data.Format("%d", mn_allow[i][0]);
// 			str_msg.Format("[Motor Speed] %s Robot Allow was changed by %s To %s", str_motor, str_old_data, str_data);
// 			Func.On_LogFile_Add(0, str_msg);
// 			Func.On_LogFile_Add(99, str_msg);
// 		}

		if (md_allow[i][1] != md_allow[i][0])
		{
			str_data.Format("%.3f", md_allow[i][1]);
			str_old_data.Format("%.3f", md_allow[i][0]);
			str_msg.Format("[Motor Speed] %s Robot Allow was changed by %s To %s", str_motor, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
	}
	
	if (mn_run_speed[1] != mn_run_speed[0])
	{
		str_data.Format("%d", mn_run_speed[1]);
		str_old_data.Format("%d", mn_run_speed[0]);
		str_msg.Format("[Motor Speed] Run Speed was changed by %s To %s", str_old_data, str_data);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_manual_speed[1] != mn_manual_speed[0])
	{
		str_data.Format("%d", mn_manual_speed[1]);
		str_old_data.Format("%d", mn_manual_speed[0]);
		str_msg.Format("[Motor Speed] Manual Speed was changed by %s To %s", str_old_data, str_data);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
}