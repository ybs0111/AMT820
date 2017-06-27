// Screen_List_Step.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
//#include "Screen_List_Step.h"
//kwlee 20170420
#include "Screen_List_Boatteaching.h"
#include "Dialog_Message.h" 
#include "Dialog_KeyPad.h"
#include "Dialog_Select.h"

//kwlee 2017.0421
#include "CtlBd_Function.h"
#include "CtlBd_Library.h"
#include "Cmmsdk.h"
#include "SrcPart/PartFunction.h"
#include "SrcBase/ALocalization.h"
#include "Run_Motors.h"
#include "COMI_Manager.h"
/* ****************************************************************************** */
/* 쓰레드 관련 클래스                                                             */
/* ****************************************************************************** */


#include "io.h"				// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 추가

#include "Dialog_Infor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step
#define TM_POSITION_CHECK	200//kwlee 2017.0422

IMPLEMENT_DYNCREATE(CScreen_List_BoatTeacing, CFormView)

CScreen_List_BoatTeacing::CScreen_List_BoatTeacing()
	: CFormView(CScreen_List_BoatTeacing::IDD)
{

	for (int i =0; i<MAX_BCR_CNT; i++)
	{
		for (int j = 0; j<MAX_BOAT_CNT; j++)
		{
			m_dBoatPos_X[1][i][j] = 0;
			m_dBoatPos_Y[1][i][j] = 0;
			m_dBoatPos_Z[1][i][j] = 0;
		}
	}
	 m_d_data_x = 0;
	 m_d_data_y = 0;
	 m_d_data_z = 0;
	 
	 //kwlee 2017.0424
	m_n_move_mode_X = 0;
	m_n_move_mode_Y = 0;
	m_n_move_mode_Z = 0;

	//{{AFX_DATA_INIT(CScreen_List_Step)
	//}}AFX_DATA_INIT
}

CScreen_List_BoatTeacing::~CScreen_List_BoatTeacing()
{
}

void CScreen_List_BoatTeacing::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_List_Step)
	DDX_Control(pDX, IDC_MSG_THREAD_11, m_msg_thread_11);
	DDX_Control(pDX, IDC_DATA_THREAD_11, m_data_thread_11);
	DDX_Control(pDX, IDC_MSG_THREAD_9, m_msg_thread_9);
	DDX_Control(pDX, IDC_MSG_THREAD_8, m_msg_thread_8);
	DDX_Control(pDX, IDC_MSG_THREAD_7, m_msg_thread_7);
	DDX_Control(pDX, IDC_MSG_THREAD_10, m_msg_thread_10);
	DDX_Control(pDX, IDC_DATA_THREAD_9, m_data_thread_9);
	DDX_Control(pDX, IDC_DATA_THREAD_8, m_data_thread_8);
	DDX_Control(pDX, IDC_DATA_THREAD_7, m_data_thread_7);
	DDX_Control(pDX, IDC_DATA_THREAD_10, m_data_thread_10);
	DDX_Control(pDX, IDC_MSG_THREAD_6, m_msg_thread_6);
	DDX_Control(pDX, IDC_MSG_THREAD_5, m_msg_thread_5);
	DDX_Control(pDX, IDC_MSG_THREAD_4, m_msg_thread_4);
	DDX_Control(pDX, IDC_MSG_THREAD_3, m_msg_thread_3);
	DDX_Control(pDX, IDC_MSG_THREAD_2, m_msg_thread_2);
	DDX_Control(pDX, IDC_MSG_THREAD_1, m_msg_thread_1);
	DDX_Control(pDX, IDC_GROUP_STEP_LIST, m_group_step_list);
	DDX_Control(pDX, IDC_DATA_THREAD_6, m_data_thread_6);
	DDX_Control(pDX, IDC_DATA_THREAD_5, m_data_thread_5);
	DDX_Control(pDX, IDC_DATA_THREAD_4, m_data_thread_4);
	DDX_Control(pDX, IDC_DATA_THREAD_3, m_data_thread_3);
	DDX_Control(pDX, IDC_DATA_THREAD_2, m_data_thread_2);
	DDX_Control(pDX, IDC_DATA_THREAD_1, m_data_thread_1);
	DDX_Control(pDX, IDC_BTN_STEP_SAVE, m_btn_step_save);
	
	DDX_Control(pDX, IDC_BTN_RIGHT_X, m_btn_right_X);
	DDX_Control(pDX, IDC_BTN_LEFT_X, m_btn_left_X);
	DDX_Control(pDX, IDC_BTN_HOME_X, m_btn_home_X);
	DDX_Control(pDX, IDC_BTN_EMG_X, m_btn_emg_X);

	DDX_Control(pDX, IDC_BTN_RIGHT_Y, m_btn_right_Y);
	DDX_Control(pDX, IDC_BTN_LEFT_Y, m_btn_left_Y);
	DDX_Control(pDX, IDC_BTN_HOME_Y, m_btn_home_Y);
	DDX_Control(pDX, IDC_BTN_EMG_Y, m_btn_emg_Y);

	DDX_Control(pDX, IDC_BTN_RIGHT_Z, m_btn_right_Z);
	DDX_Control(pDX, IDC_BTN_LEFT_Z, m_btn_left_Z);
	DDX_Control(pDX, IDC_BTN_HOME_Z, m_btn_home_Z);
	DDX_Control(pDX, IDC_BTN_EMG_Z, m_btn_emg_Z);

	DDX_Control(pDX, IDC_GROUP_MOVE_X, m_group_X_Pos);
	DDX_Control(pDX, IDC_GROUP_MOVE_Y, m_group_Y_Pos);
	DDX_Control(pDX, IDC_GROUP_MOVE_Z, m_group_Z_Pos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_List_BoatTeacing, CFormView)
	//{{AFX_MSG_MAP(CScreen_List_Step)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STEP_SAVE, OnBtnStepSave)


	ON_BN_CLICKED(IDC_BTN_EMG_X, OnBtnEmg_X)
	ON_BN_CLICKED(IDC_BTN_HOME_X, OnBtnHome_X)

	ON_BN_CLICKED(IDC_BTN_EMG_Y, OnBtnEmg_Y)
	ON_BN_CLICKED(IDC_BTN_HOME_Y, OnBtnHome_Y)
	
	ON_BN_CLICKED(IDC_BTN_EMG_Z, OnBtnEmg_Z)
	ON_BN_CLICKED(IDC_BTN_HOME_Z, OnBtnHome_Z)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	//kwlee 2017.0422
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step diagnostics

#ifdef _DEBUG
void CScreen_List_BoatTeacing::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_List_BoatTeacing::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step message handlers

void CScreen_List_BoatTeacing::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_step_list_font = NULL;
	mp_step_list_font = new CFont;
	mp_step_list_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	OnList_Step_GroupBox_Set();
	OnList_Step_Lable_Set();

	//kwlee 2017.0420
	OnDataInit();
	OnInit_Button();
	OnInitGridPos_X();
	OnInitGridPos_Y();
	OninitGridPos_Z();

	
	OnInitGrid_X_info();
	OnInitGrid_Y_info();
	OnInitGrid_Z_info();
	
	if (COMI.mn_motorbd_init_end)
	{	
		SetTimer(TM_POSITION_CHECK, 100, NULL);
	}
}

//kwlee 2017.0422
void CScreen_List_BoatTeacing::Motor_X_PositionCheck()
{
	//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;
	
	m_grid_Boat_info_X = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_X_INFO);
	
	str_tmp.Format("%0.2f", g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X));
	m_p_grid.GridCellText(m_grid_Boat_info_X, 2, 3, str_tmp);
}
void CScreen_List_BoatTeacing::Motor_Y_PositionCheck()
{
	//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;
	
	m_grid_Boat_info_Y = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_Y_INFO);
	
	str_tmp.Format("%0.2f", g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y));
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 2, 3, str_tmp);
}
void CScreen_List_BoatTeacing::Motor_Z_PositionCheck()
{
	//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;
	
	m_grid_Boat_info_Z = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_Z_INFO);
	
	str_tmp.Format("%0.2f", g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z));
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 2, 3, str_tmp);
}


void CScreen_List_BoatTeacing::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TMR_STEP) 
	{
		if (mn_timer_kill != TRUE)
		{
			OnList_Step_Data_Set();			// 스텝 정보 클래스 변수에 설정하는 함수
			OnList_Step_Data_Display();		// 스텝 정보 화면 출력 함수
		}
		else
		{
			KillTimer(TMR_STEP);			// 타이머 재 삭제

			On_Step_List_File_Name_Init();  // 쓰레드 스텝 저장 파일 정보 설정 함수
		}
	}
	//kwlee 2017.0422
	else if (nIDEvent == TM_POSITION_CHECK)
	{
		Motor_X_PositionCheck();
		Motor_Y_PositionCheck();
		Motor_Z_PositionCheck();
	}

	CFormView::OnTimer(nIDEvent);
}

void CScreen_List_BoatTeacing::OnList_Step_GroupBox_Set()
{
	CSxLogFont step_list_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

//	m_group_step_list.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_step_list.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_step_list.SetFont(step_list_font);
	m_group_step_list.SetCatptionTextColor(BLUE_C);
	m_group_step_list.SetBorderColor(YELLOW_L);
	m_group_step_list.SetFontBold(TRUE);


	m_group_X_Pos.SetFont(step_list_font);
	m_group_X_Pos.SetCatptionTextColor(BLUE_C);
	//m_group_X_Pos.SetBorderColor(YELLOW_L);
	m_group_X_Pos.SetFontBold(TRUE);

	
	m_group_Y_Pos.SetFont(step_list_font);
	m_group_Y_Pos.SetCatptionTextColor(BLUE_C);
	//m_group_Y_Pos.SetBorderColor(YELLOW_L);
	m_group_Y_Pos.SetFontBold(TRUE);

	m_group_Z_Pos.SetFont(step_list_font);
	m_group_Z_Pos.SetCatptionTextColor(BLUE_C);
	//m_group_Z_Pos.SetBorderColor(YELLOW_L);
	m_group_Z_Pos.SetFontBold(TRUE);

}

void CScreen_List_BoatTeacing::OnList_Step_Lable_Set()
{
	// 여기부터 수정..
	/* ************************************************************************** */
	/* 타이틀 출력 칼라 텍스트 박스 생성한다                                      */
	/* ************************************************************************** */
	m_msg_thread_1.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_1.SetText(" Main Robot Step");
	m_msg_thread_1.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_1.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_1.SetFontBold(TRUE);
	m_msg_thread_1.SetFontName("MS Sans Serif");
	m_msg_thread_1.SetFontSize(12);

	m_msg_thread_2.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_2.SetText(" Unloader Elevator Left Step");
	m_msg_thread_2.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_2.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_2.SetFontBold(TRUE);
	m_msg_thread_2.SetFontName("MS Sans Serif");
	m_msg_thread_2.SetFontSize(12);

	m_msg_thread_3.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_3.SetText(" Unloader Elevator Right Step");
	m_msg_thread_3.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_3.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_3.SetFontBold(TRUE);
	m_msg_thread_3.SetFontName("MS Sans Serif");
	m_msg_thread_3.SetFontSize(12);
/*
	m_msg_thread_4.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_4.SetText(" Loader Site Step");
	m_msg_thread_4.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_4.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_4.SetFontBold(TRUE);
	m_msg_thread_4.SetFontName("MS Sans Serif");
	m_msg_thread_4.SetFontSize(12);

	m_msg_thread_5.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_5.SetText(" Unloader Site Step");
	m_msg_thread_5.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_5.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_5.SetFontBold(TRUE);
	m_msg_thread_5.SetFontName("MS Sans Serif");
	m_msg_thread_5.SetFontSize(12);
*/
	m_msg_thread_4.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_4.SetText(" Unloader Transfer Left Step");
	m_msg_thread_4.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_4.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_4.SetFontBold(TRUE);
	m_msg_thread_4.SetFontName("MS Sans Serif");
	m_msg_thread_4.SetFontSize(12);

	m_msg_thread_5.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_5.SetText(" Unloader Transfer Right Step");
	m_msg_thread_5.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_5.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_5.SetFontBold(TRUE);
	m_msg_thread_5.SetFontName("MS Sans Serif");
	m_msg_thread_5.SetFontSize(12);

	m_msg_thread_6.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_6.SetText(" Test Site Step");
	m_msg_thread_6.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_6.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_6.SetFontBold(TRUE);
	m_msg_thread_6.SetFontName("MS Sans Serif");
	m_msg_thread_6.SetFontSize(12);

	m_msg_thread_7.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_7.SetText(" Test Dump Step");
	m_msg_thread_7.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_7.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_7.SetFontBold(TRUE);
	m_msg_thread_7.SetFontName("MS Sans Serif");
	m_msg_thread_7.SetFontSize(12);

	m_msg_thread_8.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_8.SetText(" Rotator Table Step");
	m_msg_thread_8.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_8.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_8.SetFontBold(TRUE);
	m_msg_thread_8.SetFontName("MS Sans Serif");
	m_msg_thread_8.SetFontSize(12);

	m_msg_thread_9.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_9.SetText(" Rotator Dump Step");
	m_msg_thread_9.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_9.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_9.SetFontBold(TRUE);
	m_msg_thread_9.SetFontName("MS Sans Serif");
	m_msg_thread_9.SetFontSize(12);

	m_msg_thread_10.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_10.SetText(" Conveyor Step");
	m_msg_thread_10.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_10.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_10.SetFontBold(TRUE);
	m_msg_thread_10.SetFontName("MS Sans Serif");
	m_msg_thread_10.SetFontSize(12);

	m_msg_thread_11.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_11.SetText(" Module Gripper Step");
	m_msg_thread_11.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_11.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_11.SetFontBold(TRUE);
	m_msg_thread_11.SetFontName("MS Sans Serif");
	m_msg_thread_11.SetFontSize(12);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 쓰레드 스텝 출력 칼라 텍스트 박스 생성한다                                 */
	/* ************************************************************************** */
	m_data_thread_1.SetFont(mp_step_list_font);
	m_data_thread_1.SetCenterText();
	m_data_thread_1.SetColor(RGB(0,0,255));
	m_data_thread_1.SetGradientColor(RGB(0,0,0));
	m_data_thread_1.SetTextColor(RGB(255,255,255));

	m_data_thread_2.SetFont(mp_step_list_font);
	m_data_thread_2.SetCenterText();
	m_data_thread_2.SetColor(RGB(0,0,255));
	m_data_thread_2.SetGradientColor(RGB(0,0,0));
	m_data_thread_2.SetTextColor(RGB(255,255,255));

	m_data_thread_3.SetFont(mp_step_list_font);
	m_data_thread_3.SetCenterText();
	m_data_thread_3.SetColor(RGB(0,0,255));
	m_data_thread_3.SetGradientColor(RGB(0,0,0));
	m_data_thread_3.SetTextColor(RGB(255,255,255));

	m_data_thread_4.SetFont(mp_step_list_font);
	m_data_thread_4.SetCenterText();
	m_data_thread_4.SetColor(RGB(0,0,255));
	m_data_thread_4.SetGradientColor(RGB(0,0,0));
	m_data_thread_4.SetTextColor(RGB(255,255,255));

	m_data_thread_5.SetFont(mp_step_list_font);
	m_data_thread_5.SetCenterText();
	m_data_thread_5.SetColor(RGB(0,0,255));
	m_data_thread_5.SetGradientColor(RGB(0,0,0));
	m_data_thread_5.SetTextColor(RGB(255,255,255));

	m_data_thread_6.SetFont(mp_step_list_font);
	m_data_thread_6.SetCenterText();
	m_data_thread_6.SetColor(RGB(0,0,255));
	m_data_thread_6.SetGradientColor(RGB(0,0,0));
	m_data_thread_6.SetTextColor(RGB(255,255,255));

	m_data_thread_7.SetFont(mp_step_list_font);
	m_data_thread_7.SetCenterText();
	m_data_thread_7.SetColor(RGB(0,0,255));
	m_data_thread_7.SetGradientColor(RGB(0,0,0));
	m_data_thread_7.SetTextColor(RGB(255,255,255));

	m_data_thread_8.SetFont(mp_step_list_font);
	m_data_thread_8.SetCenterText();
	m_data_thread_8.SetColor(RGB(0,0,255));
	m_data_thread_8.SetGradientColor(RGB(0,0,0));
	m_data_thread_8.SetTextColor(RGB(255,255,255));

	m_data_thread_9.SetFont(mp_step_list_font);
	m_data_thread_9.SetCenterText();
	m_data_thread_9.SetColor(RGB(0,0,255));
	m_data_thread_9.SetGradientColor(RGB(0,0,0));
	m_data_thread_9.SetTextColor(RGB(255,255,255));

	m_data_thread_10.SetFont(mp_step_list_font);
	m_data_thread_10.SetCenterText();
	m_data_thread_10.SetColor(RGB(0,0,255));
	m_data_thread_10.SetGradientColor(RGB(0,0,0));
	m_data_thread_10.SetTextColor(RGB(255,255,255));

	m_data_thread_11.SetFont(mp_step_list_font);
	m_data_thread_11.SetCenterText();
	m_data_thread_11.SetColor(RGB(0,0,255));
	m_data_thread_11.SetGradientColor(RGB(0,0,0));
	m_data_thread_11.SetTextColor(RGB(255,255,255));
}

void CScreen_List_BoatTeacing::OnList_Step_Data_Set()
{
	
	CString str_temp;		// 스텝 임시 저장 변수

//	char chr_buf[20] ;
/*
	str_temp = LPCTSTR(_itoa((int)(Run_Main_Robot.Run_Step), chr_buf, 10)) ;
	mstr_thread_1 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_EL_L.Run_Step), chr_buf, 10)) ;
	mstr_thread_2 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_EL_R.Run_Step), chr_buf, 10)) ;
	mstr_thread_3 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_TF_L.Run_Step), chr_buf, 10)) ;
	mstr_thread_4 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_TF_R.Run_Step), chr_buf, 10)) ;
	mstr_thread_5 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Test_Site.Run_Step), chr_buf, 10)) ;
	mstr_thread_6 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Test_Dump.Run_Step), chr_buf, 10)) ;
	mstr_thread_7 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Index_Table.Run_Step), chr_buf, 10)) ;
	mstr_thread_8 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Index_Dump.Run_Step), chr_buf, 10)) ;
	mstr_thread_9 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Conveyor.Run_Step), chr_buf, 10)) ;
	mstr_thread_10 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Module.Run_Step), chr_buf, 10)) ;
	mstr_thread_11 = str_temp;*/
}

void CScreen_List_BoatTeacing::OnList_Step_Data_Display()
{
	m_data_thread_1.SetWindowText(_T(mstr_thread_1));
	m_data_thread_2.SetWindowText(_T(mstr_thread_2));
	m_data_thread_3.SetWindowText(_T(mstr_thread_3));
	m_data_thread_4.SetWindowText(_T(mstr_thread_4));
	m_data_thread_5.SetWindowText(_T(mstr_thread_5));
	m_data_thread_6.SetWindowText(_T(mstr_thread_6));
	m_data_thread_7.SetWindowText(_T(mstr_thread_7));
	m_data_thread_8.SetWindowText(_T(mstr_thread_8));
	m_data_thread_9.SetWindowText(_T(mstr_thread_9));
	m_data_thread_10.SetWindowText(_T(mstr_thread_10));
	m_data_thread_11.SetWindowText(_T(mstr_thread_11));
}

void CScreen_List_BoatTeacing::On_Step_List_File_Name_Init()
{
	CString str_folder_data;					// 쓰레드 스텝 저장 파일 생성할 폴더 저장 변수
	CString str_new_file;						// 생성할 파일명 저장 변수
	CString str_title_time;						// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수

	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수

	str_folder_data = (st_path.mstr_path_back) + "Step\\";  // 쓰레드 스텝 저장 파일 생성 폴더 설정

	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	mp_time_cur = CTime::GetCurrentTime() ;

	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	str_new_file.Format("%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_title_time = mp_time_cur.Format("%Y/%m/%d %H:%M:%S"); 
	/* ************************************************************************** */

	On_Step_List_File_Compare(str_folder_data, str_new_file, str_title_time);  // 쓰레드 스텝 저장 파일 존재 유무 검사 및 파일 생성 함수
}

void CScreen_List_BoatTeacing::On_Step_List_File_Compare(CString str_folder, CString str_file_name, CString str_time_info)
{
	CString str_step_file;		// 쓰레드 스텝 저장 파일명 설정 변수 [날짜로 구성]
	CString str_index_data;		// 파일 인덱스 저장 변수
	CString str_chk_file;		// 존재 유무 검사할 파일에 대한 [폴더]+[파일명] 저장 변수
	CString str_create_file;	// 생성할 파일 이름 저장 변수 [파일명] +[인덱스]+[확장자]
	int n_index_data;			// 파일 인덱스 저장 변수
	int n_return;				// 함수 리턴 플래그

	/* ************************************************************************** */
	/* 쓰레드 스텝 저장 파일명 및 인덱스 로딩한다                                 */
	/* ************************************************************************** */
	str_step_file = mcls_file.OnStep_File_Name_Load();    // 쓰레드 스텝 저장 파일명 로딩 함수
	str_index_data = mcls_file.OnStep_File_Index_Load();  // 쓰레드 스텝 저장 파일 인덱스 로딩 함수
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 생성할 파일명과 마지막 생성 파일명 비교하여 인덱스 설정                    */
	/* -> str_step_file : 마지막 생성된 파일명                                    */
	/* -> str_file_name : 새로 생성할 파일명                                      */
	/* ************************************************************************** */
	if (str_step_file == str_file_name)  
	{
		n_index_data = atoi(str_index_data);
		n_index_data++;
		if (n_index_data > 99)  // 인덱스 최대 설정 범위 검사
			n_index_data = 1;
	}
	else  
		n_index_data = 1;		// 날짜 변경되었으므로 인덱스 초기화

	str_index_data.Format("%02d", n_index_data);  // 인덱스 2자리 문자로 변경
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 생성할 파일에 대한 폴더+파일명 설정한다                                    */
	/* ************************************************************************** */
	str_chk_file = str_folder; 

	str_create_file = _T("AMT820_");
	str_create_file += str_file_name;
	str_create_file += str_index_data;
	str_create_file += ".TXT";

	str_chk_file += str_create_file;
	/* ************************************************************************** */

	n_return = On_Step_List_File_Search(str_chk_file);								// 파일 존재 유무 검사 함수
	if (n_return == TRUE)  
		On_Step_List_File_Create(str_chk_file, str_create_file, str_time_info);		// 쓰레드 스텝 저장 파일 생성 함수
	else if (n_return == FALSE)
		On_Step_List_File_Create(str_chk_file, str_create_file, str_time_info);		// 쓰레드 스텝 저장 파일 생성 함수

	On_Step_List_File_Info_Save(str_file_name, str_index_data);						// 쓰레드 스텝 저장 파일 이름 저장 함수
}

void CScreen_List_BoatTeacing::On_Step_List_File_Create(CString str_step_file, CString str_create_file, CString str_time)
{
	CString str_content;    // 한 라인에 출력할 문자열 저장 변수
	CString str_temp_data;  // 저장할 정보 임시 저장 변
//	char chr_buf[20];
//	int n_length;
	FILE  *fp ;				// 파일 포인터 변수

	if ((fp=fopen(str_step_file,"w+"))==NULL)  // 파일 연다
	{
		AfxMessageBox("파일 열기 실패!..");
		return ;
	}

	/* ************************************************************************** */
	/* 파일에 타이틀 정보 출력한다                                                */
	/* ************************************************************************** */
	str_content = "**************************************************";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "| Thread Step Info File                          |";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "| DIVISION  : AMT Coporation                     |";
	fprintf(fp,"%s\n", str_content) ;
	
	str_content = "| File Name : " + str_create_file;
    str_content += "              |\r\n";
	fprintf(fp,"%s", str_content) ;

	str_content = "| Save Time : " + str_time;
    str_content += "                |\r\n";
	fprintf(fp,"%s", str_content) ;

	str_content = "**************************************************";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "|  Thread Part                           | Step  |";
	fprintf(fp,"%s\n", str_content );
	
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content) ;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 쓰레드 스텝 파일에 출력한다                                                */
	/* ************************************************************************** */
/*
	str_temp_data = LPCTSTR(_itoa((int)(Run_Main_Robot.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Main Robot Run_Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_EL_L.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Elevator Left Run_Step               | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_EL_R.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Elevator Right Run_Step               | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_TF_L.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Transfer Left Run_Step                | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_TF_R.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Transfer Right Run_Step               | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Test_Site.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Test Site Run Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Test_Dump.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Test Dump Run Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Index_Table.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Rotator Table Run Step						| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Index_Dump.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Rotator Dump Run_Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Conveyor.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Conveyor Run Step                             | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Module.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  RightStep										| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);*/
/*
	str_temp_data = LPCTSTR(_itoa((int)(Run_Upper_Base.LeftElevatorStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  LeftElevatorStep                      | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	
	str_temp_data = LPCTSTR(_itoa((int)(Run_Upper_Base.RightElevatorStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  RightElevatorStep                     | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.Run_Step           | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mTrayPickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mTrayPickStep      | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mWhirlerPlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mWhirlerPlaceStep  | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mWhirlerPickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mWhirlerPickStep   | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mPlatePlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mPlatePlaceStep    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.Run_Step             | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mTrayPickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mTrayPickStep        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mLdPlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mLdPlaceStep         | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mPlatePickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mPlatePickStep       | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mTrayPlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mTrayPlaceStep       | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

//	str_temp_data = LPCTSTR(_itoa((int)(Run_Transfer_HeatSink.Run_Step), chr_buf, 10));
//	n_length = str_temp_data.GetLength();
	str_content = "|  XYZ Robot mLdPlaceStep                | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Transfer_Module.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  XYZ Robot mPickStep                   | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Whirler.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Whirler.Run_Step                  | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Whirler.WhirlerReadyStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Whirler.WhirlerReadyStep          | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Whirler.WhirlerWorkStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Whirler.WhirlerWorkStep           | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_TowerLamp.mn_lamp_step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Tower Lamp Step                       | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = st_handler.str_last_alarm;
	str_content = "|  Last Alarm Code                       |";
	TextAndLine(6, str_content, str_temp_data, fp);

	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);*/
	/* ************************************************************************** */

	if (ferror(fp)) 
	{
		AfxMessageBox("파일 쓰기 실패!..");
		return ;
	}

	fclose(fp);  // 파일 종료
}

int CScreen_List_BoatTeacing::On_Step_List_File_Search(CString str_chk_file)
{
	int n_existence;		// 파일 존재 유무 플래그

	char fileName[256];

	sprintf(fileName, "%s", str_chk_file);
	n_existence = access(fileName,0) ;
	if (n_existence == -1)  // 해당 파일이 미존재
		return FALSE;
	
	DeleteFile(fileName);	// 존재하는 쓰레드 스텝 저장 파일 삭제

	return TRUE;
}

void CScreen_List_BoatTeacing::On_Step_List_File_Info_Save(CString str_step_file, CString str_file_index)
{
	mcls_file.OnStep_File_Name_Save(str_step_file);		// 쓰레드 스텝 저장 파일명 파일에 저장하는 함수
	mcls_file.OnStep_File_Index_Save(str_file_index);	// 쓰레드 스텝 저장 파일 인덱스 파일에 저장하는 함수
}

CString CScreen_List_BoatTeacing::FileTextLine(int dir, int length)
{
	CString line;

	switch(dir)
	{
	case 0 :
		if (length == 1 || length == 0 )
		{
			line.Format("%14c", '|');
		}
		else if (length == 2)
		{
			line.Format("%13c", '|');
		}
		else if (length == 3)
		{
			line.Format("%12c", '|');
		}
		else if (length == 4)
		{
			line.Format("%11c", '|');
		}
		else if (length == 5)
		{
			line.Format("%10c", '|');
		}
		else if (length == 6)
		{
			line.Format("%9c", '|');
		}
		else if (length == 7)
		{
			line.Format("%8c", '|');
		}
		else if (length == 8)
		{
			line.Format("%7c", '|');
		}
		else
		{
			line.Format("%8c", '|');
		}
		break;
	case 1 :  
		if (length == 1 || length == 0 )
		{
			line.Format("%6c", '|');
		}
		else if (length == 2)
		{
			line.Format("%5c", '|');
		}
		else if (length == 3)
		{
			line.Format("%4c", '|');
		}
		else if (length == 4)
		{
			line.Format("%3c", '|');
		}
		else if (length == 5)
		{
			line.Format("%2c", '|');
		}
		else
		{
			line.Format("%1c", '|');
		}
		break;
	case 2 :
		if (length == 1 || length == 0 )
		{
			line.Format("%5c", '|');
		}
		else if (length == 2)
		{
			line.Format("%4c", '|');
		}
		else if (length == 3)
		{
			line.Format("%3c", '|');
		}
		else if (length == 4)
		{
			line.Format("%2c", '|');
		}
		else
		{
			line.Format("%1c", '|');
		}
		break;
	case 3 : 
		if (length == 1 || length == 0 )
		{
			//line.Format("%16c", '|');
			line.Format("%17c", '|');
		}
		else if (length == 2)
		{
			line.Format("%15c", '|');
		}
		else if (length == 3)
		{
			line.Format("%14c", '|');
		}
		else if (length == 4)
		{
			line.Format("%13c", '|');
		}
		else if (length == 5)
		{
			line.Format("%12c", '|');
		}
		else if (length == 6)
		{
			line.Format("%11c", '|');
		}
		else if (length == 7)
		{
			line.Format("%10c", '|');
		}
		else if (length == 8)
		{
			line.Format("%9c", '|');
		}
		else if (length == 9)
		{
			line.Format("%8c", '|');
		}
		else if (length == 10)
		{
			line.Format("%7c", '|');
		}
		else if (length == 11)
		{
			line.Format("%6c", '|');
		}
		else
		{
			line.Format("%5c", '|');
		}
		break;
	default :
		break;
	}

	return line;
}


void CScreen_List_BoatTeacing::OnBtnEmg_X()
{
	int i = 0;
	
	//m_b_stop_req = TRUE;	// ESTOP 요청 플래그
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).SetStopReq( true );
	COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_X );
	
	// 	for ( i = 0; i < M_MOTOR_COUNT; i++ )
	// 	{
	// 		COMI.Set_MotStop( 0, i );
	// 	}
	
	st_handler.mn_menu_lock = false;
} 
void CScreen_List_BoatTeacing::OnBtnHome_X()
{
	CString strTmp;
	
//	OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	// ==============================================================================
	// 메뉴 사용 불가능하도록 플래그 설정한다                                    
	// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
	// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
	//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
	// ==============================================================================
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// ==============================================================================
	
	strTmp.Format("[Motor] (HeatSink_X)_Axis Home Move");
	Func.On_LogFile_Add(99, strTmp);
	Func.On_LogFile_Add(0, strTmp);
	
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).MotorStepClear();
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).MotorHomeStart();
	
} 

void CScreen_List_BoatTeacing::OnBtnEmg_Y()
{
	int i = 0;
	
	//m_b_stop_req = TRUE;	// ESTOP 요청 플래그
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).SetStopReq( true );
	COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_Y );
	
	// 	for ( i = 0; i < M_MOTOR_COUNT; i++ )
	// 	{
	// 		COMI.Set_MotStop( 0, i );
	// 	}
	
	st_handler.mn_menu_lock = false;
} 
void CScreen_List_BoatTeacing::OnBtnHome_Y()
{
	CString strTmp;
	
	//	OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	// ==============================================================================
	// 메뉴 사용 불가능하도록 플래그 설정한다                                    
	// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
	// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
	//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
	// ==============================================================================
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// ==============================================================================
	
	strTmp.Format("[Motor] (HeatSink_Y)_Axis Home Move");
	Func.On_LogFile_Add(99, strTmp);
	Func.On_LogFile_Add(0, strTmp);
	
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).MotorStepClear();
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).MotorHomeStart();
}

void CScreen_List_BoatTeacing::OnBtnEmg_Z()
{
	int i = 0;
	
	//m_b_stop_req = TRUE;	// ESTOP 요청 플래그
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).SetStopReq( true );
	COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_Z );

// 	for ( i = 0; i < M_MOTOR_COUNT; i++ )
// 	{
// 		COMI.Set_MotStop( 0, i );
// 	}
	
	st_handler.mn_menu_lock = false;
} 
void CScreen_List_BoatTeacing::OnBtnHome_Z()
{
	CString strTmp;
	
	//	OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	// ==============================================================================
	// 메뉴 사용 불가능하도록 플래그 설정한다                                    
	// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
	// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
	//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
	// ==============================================================================
	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// ==============================================================================
	
	strTmp.Format("[Motor] (HeatSink_Z)_Axis Home Move");
	Func.On_LogFile_Add(99, strTmp);
	Func.On_LogFile_Add(0, strTmp);
	
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).MotorStepClear();
	GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).MotorHomeStart();
}

void CScreen_List_BoatTeacing::OnBtnStepSave() 
{
	int n_response;				// 대화 상자 리턴 플래그

	CDialog_Infor   info_dlg;

	st_other.str_confirm_msg = _T("Do you want Save data?");

	n_response = info_dlg.DoModal();
	if (n_response == IDOK)
	{
		for (int i = 0; i<12;i++)
		{
			for(int j =0; j<3; j++)
			{
				st_BoatTeaching[i].m_dPos_x[j] = m_dBoatPos_X[1][i][j];
				st_BoatTeaching[i].m_dPos_y[j] = m_dBoatPos_Y[1][i][j];
				st_BoatTeaching[i].m_dPos_z[j] = m_dBoatPos_Z[1][i][j];
			}
		}
	}	//mn_timer_kill = TRUE;	// 타이머 삭제 요청 플래그 설정 
}

void CScreen_List_BoatTeacing::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line, str_temp_data_temp, str_content;
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch(dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}

BOOL CScreen_List_BoatTeacing::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_step_list_font;
	mp_step_list_font = NULL;
	/* ************************************************************************** */

	KillTimer(TMR_STEP);			// 타이머 재 삭제
	return CFormView::DestroyWindow();
}

//kwlee 2017.0420
void CScreen_List_BoatTeacing::OnDataInit()
{
	for (int i =0; i<12; i++)
	{
		for (int j =0; j<3; j++)
		{
			m_dBoatPos_X[1][i][j] = st_BoatTeaching[i].m_dPos_x[j];
			m_dBoatPos_Y[1][i][j] = st_BoatTeaching[i].m_dPos_y[j];
			m_dBoatPos_Z[1][i][j] = st_BoatTeaching[i].m_dPos_z[j];
		}
	}
}


void CScreen_List_BoatTeacing::OnInitGridPos_X()
{
	int   i, j,nTmp = 0;
	CString str_tmp,strData;
	m_grid_Boat_pos_X = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_X);
	int nBcrTemp =0;
	int nPosTmp =0;
	//m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
//	m_p_grid.GridCellSelectDisplay(m_grid_Boat_pos_X, FALSE);
	
	m_p_grid.GridRowHeader(m_grid_Boat_pos_X, FALSE);
	m_p_grid.GridColHeader(m_grid_Boat_pos_X, FALSE);
	m_p_grid.GridHorizontal(m_grid_Boat_pos_X, FALSE);
	m_p_grid.GridVertical(m_grid_Boat_pos_X, TRUE);
	m_p_grid.GridAutoSize(m_grid_Boat_pos_X, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_pos_X, FALSE);
	m_p_grid.GridCellRows(m_grid_Boat_pos_X, 38);
	m_p_grid.GridCellCols(m_grid_Boat_pos_X, 5);
	
	for(i=0; i<MAX_BOAT_GRID_CNT+ 1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_Boat_pos_X, i+2,20);
		for(j=0; j<5; j++)
		{
			
			switch(j)
			{
			case 0:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_X, j+1, 7); //Num
				break;
			case 1:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_X, j+1, 7);
				break;
			case 2:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_X, j+1, 15);
				break;
			case 3:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_X, j+1, 7);
				break;
			case 4:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_X, j+1, 7);
				break;
			
			}
			m_p_grid.GridCellControlStatic(m_grid_Boat_pos_X, i, j+1);
		}
	}
	m_p_grid.GridCellMerge(m_grid_Boat_pos_X,1,1,1,5);
	m_p_grid.GridCellFont(m_grid_Boat_pos_X, 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_X, 1, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_X, 1, 1, "Posion_X");

	m_p_grid.GridCellFont(m_grid_Boat_pos_X, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_X, 2, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_X, 2, 1,"Num");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_X, 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_X, 2, 2, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_X, 2, 2,"Pos");

	m_p_grid.GridCellFont(m_grid_Boat_pos_X, 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_X, 2, 3, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_X,2, 3, "Positon Val");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_X, 2, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_X, 2, 4, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_X, 2, 4, "Move");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_X, 2, 5, "MS Sans Serif", 15);
	m_p_grid.GridCellColor(m_grid_Boat_pos_X, 2,5, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_X, 2, 5, "Read");
	
	for(i=0; i<MAX_BCR_CNT * MAX_BOAT_CNT; i++)
	{
		
		str_tmp.Format("%d",i+1);
		m_p_grid.GridCellMerge(m_grid_Boat_pos_X,i+3,1,3,1);
		m_p_grid.GridCellFont(m_grid_Boat_pos_X, i+3, 1, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_X, i+3, 1, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_X, (i+1)*3, 1, str_tmp);
		
		nBcrTemp = i/3;
		if (i % 3 == 0)
		{
			nPosTmp = 0;
			strData.Format("Top");
		}
		else if (i % 3 == 1)
		{
			strData.Format("Mid");
			nPosTmp = 1;
		}
		else if (i % 3 == 2)
		{
			strData.Format("Btm");
			nPosTmp = 2;
		}
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_X, i+3, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_X, i+3, 2, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_X, i+3, 2, strData);
		
		str_tmp.Format("%0.2f",m_dBoatPos_X[1][nBcrTemp][nPosTmp]);
		m_p_grid.GridCellFont(m_grid_Boat_pos_X, i+3, 3, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_X, i+3, 3, GREEN_L, BLACK_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_X, i+3, 3, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_X, i+3, 4, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_Boat_pos_X, i+3, 4, TEXT_BC, BLUE_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_X, i+3, 4, "Go.");
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_X, i+3, 5, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_Boat_pos_X, i+3, 5, TEXT_BC, RED_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_X, i+3, 5, "Read.");
	}
}

void CScreen_List_BoatTeacing::OnInitGridPos_Y()
{
	int   i, j,nTmp = 0;
	CString str_tmp,strData;
	m_grid_Boat_pos_Y = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_Y);
	
	//kwlee 2017.0421
	int nBcrTemp =0;
	int nPosTmp =0;	
	//m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	//m_p_grid.GridCellSelectDisplay(m_grid_Boat_pos_Y, FALSE);
	
	m_p_grid.GridRowHeader(m_grid_Boat_pos_Y, FALSE);
	m_p_grid.GridColHeader(m_grid_Boat_pos_Y, FALSE);
	m_p_grid.GridHorizontal(m_grid_Boat_pos_Y, FALSE);
	m_p_grid.GridVertical(m_grid_Boat_pos_Y, TRUE);
	m_p_grid.GridAutoSize(m_grid_Boat_pos_Y, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_pos_Y, FALSE);
	m_p_grid.GridCellRows(m_grid_Boat_pos_Y, 38);
	m_p_grid.GridCellCols(m_grid_Boat_pos_Y, 5);
	
	for(i=0; i<MAX_BOAT_GRID_CNT+ 1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_Boat_pos_Y, i+2,20);
		for(j=0; j<5; j++)
		{
			
			switch(j)
			{
			case 0:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Y, j+1, 7); //Num
				break;
			case 1:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Y, j+1, 7);
				break;
			case 2:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Y, j+1, 15);
				break;
			case 3:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Y, j+1, 7);
				break;
			case 4:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Y, j+1, 7);
				break;
			
			}
			m_p_grid.GridCellControlStatic(m_grid_Boat_pos_Y, i, j+1);
		}
	}
	m_p_grid.GridCellMerge(m_grid_Boat_pos_Y,1,1,1,5);
	m_p_grid.GridCellFont(m_grid_Boat_pos_Y, 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Y, 1, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Y, 1, 1, "Posion_Y");

	m_p_grid.GridCellFont(m_grid_Boat_pos_Y, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Y, 2, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Y, 2, 1,"Num");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Y, 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Y, 2, 2, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Y, 2, 2,"Pos");

	m_p_grid.GridCellFont(m_grid_Boat_pos_Y, 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Y, 2, 3, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Y,2, 3, "Positon Val");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Y, 2, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Y, 2, 4, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Y, 2, 4, "Move");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Y, 2, 5, "MS Sans Serif", 15);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Y, 2, 5, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Y, 2, 5, "Read");
	
	for(i=0; i<MAX_BCR_CNT * MAX_BOAT_CNT; i++)
	{
		nBcrTemp = i/3;
		if (i % 3 == 0)
		{
			strData.Format("Top");
			nPosTmp = 0;
		}
		else if (i % 3 == 1)
		{
			strData.Format("Mid");
			nPosTmp = 1;
		}
		else if (i % 3 == 2)
		{
			strData.Format("Btm");
			nPosTmp = 2;
		}

		str_tmp.Format("%d",i+1);
		m_p_grid.GridCellMerge(m_grid_Boat_pos_Y,i+3,1,3,1);
		m_p_grid.GridCellFont(m_grid_Boat_pos_Y, i+3, 1, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Y, i+3, 1, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Y, (i+1)*3, 1, str_tmp);
		
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_Y, i+3, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Y, i+3, 2, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Y, i+3, 2, strData);
		
		str_tmp.Format("%0.2f", m_dBoatPos_Y[1][nBcrTemp][nPosTmp]);
		m_p_grid.GridCellFont(m_grid_Boat_pos_Y, i+3, 3, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Y, i+3, 3, GREEN_L, BLACK_C);
 		m_p_grid.GridCellText(m_grid_Boat_pos_Y, i+3, 3, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_Y, i+3, 4, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Y, i+3, 4, TEXT_BC, BLUE_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Y, i+3, 4, "Go.");
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_Y, i+3, 5, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Y, i+3, 5, TEXT_BC, RED_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Y, i+3, 5, "Read.");
	}	
}

void CScreen_List_BoatTeacing::OninitGridPos_Z()
{
	int   i, j,nTmp = 0;
	CString str_tmp,strData;
	m_grid_Boat_pos_Z = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_Z);
	
	//kwlee 2017.0421
	int nBcrTemp =0;
	int nPosTmp =0;	

	//m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_Boat_pos_Z, FALSE);
	
	m_p_grid.GridRowHeader(m_grid_Boat_pos_Z, FALSE);
	m_p_grid.GridColHeader(m_grid_Boat_pos_Z, FALSE);
	m_p_grid.GridHorizontal(m_grid_Boat_pos_Z, FALSE);
	m_p_grid.GridVertical(m_grid_Boat_pos_Z, TRUE);
	m_p_grid.GridAutoSize(m_grid_Boat_pos_Z, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_pos_Z, FALSE);
	m_p_grid.GridCellRows(m_grid_Boat_pos_Z, 38);
	m_p_grid.GridCellCols(m_grid_Boat_pos_Z, 5);
	
	for(i=0; i<MAX_BOAT_GRID_CNT+ 1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_Boat_pos_Z, i+2,20);
		for(j=0; j<5; j++)
		{
			
			switch(j)
			{
			case 0:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Z, j+1, 7); //Num
				break;
			case 1:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Z, j+1, 7);
				break;
			case 2:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Z, j+1, 15);
				break;
			case 3:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Z, j+1,7);
				break;
			case 4:
				m_p_grid.GridCellWidth_L(m_grid_Boat_pos_Z, j+1, 7);
				break;
			
			}
			m_p_grid.GridCellControlStatic(m_grid_Boat_pos_Z, i, j+1);
		}
	}
	m_p_grid.GridCellMerge(m_grid_Boat_pos_Z,1,1,1,5);
	m_p_grid.GridCellFont(m_grid_Boat_pos_Z, 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Z, 1, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Z, 1, 1, "Posion_Z");

	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Z, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Z, 2, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Z, 2, 1,"Num");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Z, 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Z, 2, 2, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Z, 2, 2,"Pos");

	m_p_grid.GridCellFont(m_grid_Boat_pos_Z, 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Z, 2, 3, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Z,2, 3, "Positon Val");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Z, 2, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Z, 2, 4, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Z, 2, 4, "Move");
	
	m_p_grid.GridCellFont(m_grid_Boat_pos_Z, 2, 5, "MS Sans Serif", 15);
	m_p_grid.GridCellColor(m_grid_Boat_pos_Z, 2, 5, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_pos_Z, 2, 5, "Read");
	
	for(i=0; i<MAX_BCR_CNT * MAX_BOAT_CNT; i++)
	{
		
		str_tmp.Format("%d",i+1);
		m_p_grid.GridCellMerge(m_grid_Boat_pos_Z,i+3,1,3,1);
		m_p_grid.GridCellFont(m_grid_Boat_pos_Z, i+3, 1, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Z, i+3, 1, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Z, (i+1)*3, 1, str_tmp);
		
		nBcrTemp = i/3;
		if (i % 3 == 0)
		{
			strData.Format("Top");
			nPosTmp = 0;
		}
		else if (i % 3 == 1)
		{
			strData.Format("Mid");
			nPosTmp = 1;
		}
		else if (i % 3 == 2)
		{
			strData.Format("Btm");
			nPosTmp = 2;
		}
		m_p_grid.GridCellFont(m_grid_Boat_pos_Z, i+3, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Z, i+3, 2, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Z, i+3, 2, strData);
		
		str_tmp.Format("%0.2f", m_dBoatPos_Z[1][nBcrTemp][nPosTmp]);
		m_p_grid.GridCellFont(m_grid_Boat_pos_Z, i+3, 3, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Z, i+3, 3, GREEN_L, BLACK_C);
 		m_p_grid.GridCellText(m_grid_Boat_pos_Z, i+3, 3, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_Z, i+3, 4, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Z, i+3, 4, TEXT_BC, BLUE_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Z, i+3, 4, "Go.");
		
		m_p_grid.GridCellFont(m_grid_Boat_pos_Z, i+3, 5, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_Boat_pos_Z, i+3, 5, TEXT_BC, RED_C);
		m_p_grid.GridCellText(m_grid_Boat_pos_Z, i+3, 5, "Read.");
	}
}
//kwlee 2017.0422
void CScreen_List_BoatTeacing::OnInitGrid_X_info()
{
	int   i, j,nTmp = 0;
	CString str_tmp,strData;
	m_grid_Boat_info_X = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_X_INFO);
	int nBcrTemp =0;
	int nPosTmp =0;
	//m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	//	m_p_grid.GridCellSelectDisplay(m_grid_Boat_pos_X, FALSE);
	
	m_p_grid.GridRowHeader(m_grid_Boat_info_X, FALSE);
	m_p_grid.GridColHeader(m_grid_Boat_info_X, FALSE);
	m_p_grid.GridHorizontal(m_grid_Boat_info_X, FALSE);
	m_p_grid.GridVertical(m_grid_Boat_info_X, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_info_X, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_info_X, FALSE);
	m_p_grid.GridCellRows(m_grid_Boat_info_X, 2);
	m_p_grid.GridCellCols(m_grid_Boat_info_X, 6);
	
	for(i=0; i<2; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_Boat_info_X, i+1, 20);
		for(j=0; j<6; j++)
		{
			m_p_grid.GridCellWidth_L(m_grid_Boat_info_X, j+1, 7.5);
			
			m_p_grid.GridCellControlStatic(m_grid_Boat_info_X, i+1, j+1);
		}
	}
	m_p_grid.GridCellMerge(m_grid_Boat_info_X, 1, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 1, 1, "Motor Move");
	
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 2, 1, RED_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 2, 1, "Jog");
	
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 2, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 2, 2, "Distance");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_X, 1, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 1, 3, "Current Position");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_X, 2, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 2, 3, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 2, 3, "0");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_X, 1, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 1, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 1, 5, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 1, 5, "Move Distance (mm)");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_X, 2, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_X, 2, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_X, 2, 5, WHITE_C, BLACK_C);
	str_tmp.Format("%.2f", m_d_data_x);
	m_p_grid.GridCellText(m_grid_Boat_info_X, 2, 5, str_tmp);
}

//kwlee 2017.0422
void CScreen_List_BoatTeacing::OnInitGrid_Y_info()
{
	int   i, j,nTmp = 0;
	CString str_tmp,strData;
	m_grid_Boat_info_Y = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_Y_INFO);
	int nBcrTemp =0;
	int nPosTmp =0;
	//m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	//	m_p_grid.GridCellSelectDisplay(m_grid_Boat_pos_X, FALSE);
	
	m_p_grid.GridRowHeader(m_grid_Boat_info_Y, FALSE);
	m_p_grid.GridColHeader(m_grid_Boat_info_Y, FALSE);
	m_p_grid.GridHorizontal(m_grid_Boat_info_Y, FALSE);
	m_p_grid.GridVertical(m_grid_Boat_info_Y, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_info_Y, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_info_Y, FALSE);
	m_p_grid.GridCellRows(m_grid_Boat_info_Y, 2);
	m_p_grid.GridCellCols(m_grid_Boat_info_Y, 6);
	
	for(i=0; i<2; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_Boat_info_Y, i+1, 20);
		for(j=0; j<6; j++)
		{
			m_p_grid.GridCellWidth_L(m_grid_Boat_info_Y, j+1, 7.5);
			
			m_p_grid.GridCellControlStatic(m_grid_Boat_info_Y, i+1, j+1);
		}
	}
	m_p_grid.GridCellMerge(m_grid_Boat_info_Y, 1, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 1, 1, "Motor Move");
	
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 1, RED_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 2, 1, "Jog");
	
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 2, 2, "Distance");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Y, 1, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 1, 3, "Current Position");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Y, 2, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 3, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 2, 3, "0");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Y, 1, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 1, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 1, 5, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 1, 5, "Move Distance (mm)");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Y, 2, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Y, 2, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 5, WHITE_C, BLACK_C);
	str_tmp.Format("%.2f", m_d_data_y);
	m_p_grid.GridCellText(m_grid_Boat_info_Y, 2, 5, str_tmp);
}


void CScreen_List_BoatTeacing::OnInitGrid_Z_info()
{
	int   i, j,nTmp = 0;
	CString str_tmp,strData;
	m_grid_Boat_info_Z = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_Z_INFO);
	int nBcrTemp =0;
	int nPosTmp =0;
	//m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	//	m_p_grid.GridCellSelectDisplay(m_grid_Boat_pos_X, FALSE);
	
	m_p_grid.GridRowHeader(m_grid_Boat_info_Z, FALSE);
	m_p_grid.GridColHeader(m_grid_Boat_info_Z, FALSE);
	m_p_grid.GridHorizontal(m_grid_Boat_info_Z, FALSE);
	m_p_grid.GridVertical(m_grid_Boat_info_Z, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_info_Z, FALSE);
	m_p_grid.GridAutoSize(m_grid_Boat_info_Z, FALSE);
	m_p_grid.GridCellRows(m_grid_Boat_info_Z, 2);
	m_p_grid.GridCellCols(m_grid_Boat_info_Z, 6);
	
	for(i=0; i<2; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_Boat_info_Z, i+1, 20);
		for(j=0; j<6; j++)
		{
			m_p_grid.GridCellWidth_L(m_grid_Boat_info_Z, j+1, 7.5);
			
			m_p_grid.GridCellControlStatic(m_grid_Boat_info_Z, i+1, j+1);
		}
	}
	m_p_grid.GridCellMerge(m_grid_Boat_info_Z, 1, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 1, 1, "Motor Move");
	
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 1, RED_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 2, 1, "Jog");
	
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 2, 2, "Distance");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Z, 1, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 1, 3, "Current Position");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Z, 2, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 3, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 2, 3, "0");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Z, 1, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 1, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 1, 5, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 1, 5, "Move Distance (mm)");
	
	m_p_grid.GridCellMerge(m_grid_Boat_info_Z, 2, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_Boat_info_Z, 2, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 5, WHITE_C, BLACK_C);
	str_tmp.Format("%.2f", m_d_data_z);
	m_p_grid.GridCellText(m_grid_Boat_info_Z, 2, 5, str_tmp);
}

//kwlee 2017.0422
LONG CScreen_List_BoatTeacing::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CDialog_Message msg_dlg;
	CString strTmp;
	double dPos;
	int		nRet;
	
	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
		return 0;

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT_X)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode_X == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, M_HEATSINK_TRANSFER_X, 0);
			
			if (nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
		//	OnButtonControl(FALSE);
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X);

			strTmp.Format("[Motor] HeatSink_X Axis (%.3f) + Jog Move", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).JogMoveStart( MINUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.mn_menu_lock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......
		//	OnButtonControl(FALSE);							// 버튼 비활성화
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) - m_d_data_x;
			double dMotGotoSpeed	= (COMI.md_spd_jog[M_HEATSINK_TRANSFER_X] / COMI.md_spd_vel[M_HEATSINK_TRANSFER_X][0]) * 100;

			strTmp.Format("[Motor] HeatSink_X_Axis (%.3f) - Data Move", m_d_data_x);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);

			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_DISTANCE );
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT_X)
	{ 
		if (m_n_move_mode_X == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, M_HEATSINK_TRANSFER_X, 0);
			
			if (nRet == CTLBD_RET_ERROR)
				return 0;
			
		//	OnButtonControl(FALSE);						// 버튼 비활성화
			
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X);

			strTmp.Format("[Motor] HeatSink_X_Axis (%.3f) + Jog Move", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).JogMoveStart( PLUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.mn_menu_lock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......				
			//OnButtonControl(FALSE);						// 버튼 비활성화
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) + m_d_data_x;
			double dMotGotoSpeed	= (COMI.md_spd_jog[M_HEATSINK_TRANSFER_X] / COMI.md_spd_vel[M_HEATSINK_TRANSFER_X][0]) * 100;

			strTmp.Format("[Motor] HeatSink_X_Axis (%.3f) + Data Move", m_d_data_x);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);

			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_DISTANCE );
		}
	}
	else if (pButton->GetTag() == IDC_BTN_LEFT_Y)
	{
		if (m_n_move_mode_Y == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, M_HEATSINK_TRANSFER_Y, 0);
			
			if (nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			//	OnButtonControl(FALSE);
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y);
			
			strTmp.Format("[Motor] HeatSink_Y Axis (%.3f) + Jog Move", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).JogMoveStart( MINUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.mn_menu_lock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......
			//	OnButtonControl(FALSE);							// 버튼 비활성화
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y) - m_d_data_y;
			double dMotGotoSpeed	= (COMI.md_spd_jog[M_HEATSINK_TRANSFER_Y] / COMI.md_spd_vel[M_HEATSINK_TRANSFER_Y][0]) * 100;
			
			strTmp.Format("[Motor] HeatSink_Y_Axis (%.3f) - Data Move", m_d_data_x);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_DISTANCE );
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT_Y)
	{
		if (m_n_move_mode_Y == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, M_HEATSINK_TRANSFER_Y, 0);
			
			if (nRet == CTLBD_RET_ERROR)
				return 0;
			
			//	OnButtonControl(FALSE);						// 버튼 비활성화
			
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y);
			
			strTmp.Format("[Motor] HeatSink_Y_Axis (%.3f) + Jog Move", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).JogMoveStart( PLUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.mn_menu_lock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......				
			//OnButtonControl(FALSE);						// 버튼 비활성화
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y) + m_d_data_y;
			double dMotGotoSpeed	= (COMI.md_spd_jog[M_HEATSINK_TRANSFER_Y] / COMI.md_spd_vel[M_HEATSINK_TRANSFER_Y][0]) * 100;
			
			strTmp.Format("[Motor] HeatSink_Y_Axis (%.3f) + Data Move", m_d_data_y);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_DISTANCE );
		}
	}
	else if (pButton->GetTag() == IDC_BTN_LEFT_Z)
	{
		//if (m_n_move_mode_Y == 0)
		//kwlee 2017.0627
		if (m_n_move_mode_Z == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, M_HEATSINK_TRANSFER_Z, 0);
			
			if (nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			//	OnButtonControl(FALSE);
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z);
			
			strTmp.Format("[Motor] HeatSink_Z Axis (%.3f) + Jog Move", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).JogMoveStart( MINUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.mn_menu_lock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......
			//	OnButtonControl(FALSE);							// 버튼 비활성화
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z) - m_d_data_z;
			double dMotGotoSpeed	= (COMI.md_spd_jog[M_HEATSINK_TRANSFER_Z] / COMI.md_spd_vel[M_HEATSINK_TRANSFER_Z][0]) * 100;
			
			strTmp.Format("[Motor] HeatSink_Z_Axis (%.3f) - Data Move", m_d_data_z);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_DISTANCE );
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT_Z)
	{
		//if (m_n_move_mode_Y == 0)
		//kwlee 2017.0627
		if (m_n_move_mode_Z == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, M_HEATSINK_TRANSFER_Z, 0);
			
			if (nRet == CTLBD_RET_ERROR)
				return 0;
			
			//	OnButtonControl(FALSE);						// 버튼 비활성화
			
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z);
			
			strTmp.Format("[Motor] HeatSink_Z_Axis (%.3f) + Jog Move", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).JogMoveStart( PLUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.mn_menu_lock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......				
			//OnButtonControl(FALSE);						// 버튼 비활성화
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z) + m_d_data_z;
			double dMotGotoSpeed	= (COMI.md_spd_jog[M_HEATSINK_TRANSFER_Z] / COMI.md_spd_vel[M_HEATSINK_TRANSFER_Z][0]) * 100;
			
			strTmp.Format("[Motor] HeatSink_Z_Axis (%.3f) + Data Move", m_d_data_z);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_DISTANCE );
		}
	}
	return 0;
}

LONG CScreen_List_BoatTeacing::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
{	
	CDialog_Message msg_dlg;
	CString strTmp;
	double dPos;
	int		nRet;


	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT_X)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode_X == 0)
		{
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X);
			strTmp.Format("[Motor] HeatSink_X_Axis (%.3f) - Jog Stop", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			cmmSxStop(M_HEATSINK_TRANSFER_X, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			//OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT_X)
	{ 
		if (m_n_move_mode_X == 0)
		{
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X);
			strTmp.Format("[Motor] HeatSink_X_Axis (%.3f) + Jog Stop", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			cmmSxStop(M_HEATSINK_TRANSFER_X, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			//OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_LEFT_Y)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode_Y == 0)
		{
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y);
			strTmp.Format("[Motor] HeatSink_Y_Axis (%.3f) - Jog Stop", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			cmmSxStop(M_HEATSINK_TRANSFER_Y, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			//OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT_Y)
	{ 
		if (m_n_move_mode_Y == 0)
		{
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y);
			strTmp.Format("[Motor] HeatSink_Y_Axis (%.3f) + Jog Stop", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			cmmSxStop(M_HEATSINK_TRANSFER_Y, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			//OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_LEFT_Z)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode_Z == 0)
		{
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z);
			strTmp.Format("[Motor] HeatSink_Z_Axis (%.3f) - Jog Stop", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			cmmSxStop(M_HEATSINK_TRANSFER_Z, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			//OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT_Z)
	{ 
		if (m_n_move_mode_Z == 0)
		{
			dPos =  g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z);
			strTmp.Format("[Motor] HeatSink_Z_Axis (%.3f) + Jog Stop", dPos);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);
			
			cmmSxStop(M_HEATSINK_TRANSFER_Z, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			//OnButtonControl(TRUE);	
		}
	}
	return 0;
}
///////////
//kwlee 2017.0420
void CScreen_List_BoatTeacing::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;
	
//	int	n_response;
	CDialog_Select select_dlg;
	CString strTmp, strChange, str_file;
	CString	sBeforePos;
	CString str_tmp, str_data;
	double value;
	int nResponse;

	CString strText;
	
	CDialog_KeyPad	pad_dlg;
	
	//kwlee 2017.0421
	m_nCmdRow	= lpcc->Row;
	m_nCmdCol	= lpcc->Col;
 
	m_nBcrNum = (m_nCmdRow/3) -1; //Carrier Buffer 1~12
	if (m_nCmdRow%3 ==0)
	{
		m_nPos = 0; //Top
	}
	else if (m_nCmdRow%3 ==1)
	{
		m_nPos = 1; //Middle
	}
	else if (m_nCmdRow%3 ==2)
	{
		m_nPos = 2; //Btm
	}

	if (lpcc->Row == 0 || lpcc->Row < 3 || lpcc->Col<3 ) return;
	
	if (st_handler.mn_menu_lock) 
	{
		st_handler.mn_menu_lock = false;
		return;
	}

	if (wParam == IDC_CUSTOM_BOAT_PART_X)
	{
 		//if (lpcc->Row == 0 ) return;
 
 		m_grid_Boat_pos_X = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_X);
 		
		m_nAxis = M_HEATSINK_TRANSFER_X;
		switch(lpcc->Col)
		{
		
			case 3:				
				strText = m_p_grid.GridCellText(m_grid_Boat_pos_X, lpcc->Row, lpcc->Col - 1);
				st_msg.mstr_keypad_msg.Format("[HeatSink_X Motor] %s set",  strText);

				strTmp = m_p_grid.GridCellText(m_grid_Boat_pos_X, lpcc->Row, lpcc->Col);
				st_msg.mstr_keypad_val = strTmp;
				
			//	strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[m_n_axis][0], COMI.md_limit_position[m_n_axis][1], double(atof(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				//kwlee 2017.0421
				strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[M_HEATSINK_TRANSFER_X][0], COMI.md_limit_position[M_HEATSINK_TRANSFER_X][1], double(atof(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				
	// 			if( AMotor::CheckTeachSafety( m_n_axis, lpcc->Row-1, atof(strTmp) ) == false )
	// 			{
	// 				m_strTempMSG = "이 티칭값은 위험할 수도 있습니다. 계속하시겠습니까?";
	// 				if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("It might dangerous this teaching value. Do you want to continue?");
	// 				if( DoModal_Select( m_strTempMSG ) != IDOK )
	// 				{
	// 					g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
	// 					break;
	// 				}
	// 			}
	// 			m_d_position[lpcc->Row-1][1] = atof(strTmp);
	// 			m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, strTmp);
				//kwlee 2017.0421
				
				
				//st_BoatTeaching[nBcrNum].m_dPos_x[nPos][1] = atof(strTmp);
				//kwlee 2017.0421
				m_dBoatPos_X[1][m_nBcrNum][m_nPos] = atof(strTmp);
				m_p_grid.GridCellText(m_grid_Boat_pos_X, lpcc->Row, lpcc->Col, strTmp);
				break;

			case 4:
				{
					if (COMI.mn_motorbd_init_end != TRUE)  
					{
						if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
							return ;
					}
					
					m_p_grid.GridCellText(m_grid_Boat_pos_X, lpcc->Row, lpcc->Col, "ing...");
					m_p_grid.GridCellColor(m_grid_Boat_pos_X, lpcc->Row, lpcc->Col, NOR_C, BLACK_C);
					
					//double dMotGotoPos = m_d_position[lpcc->Row-1][1];
					//kwlee 2017.0421
					double dMotGotoPos = m_dBoatPos_X[1][m_nBcrNum][m_nPos];
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X).SetMotGotoPos( dMotGotoPos );
					
					nResponse = Func.OnMotor_Go_Check(M_HEATSINK_TRANSFER_X, dMotGotoPos);
					
					if (nResponse != IDOK)
					{
						MotorCompletion(M_HEATSINK_TRANSFER_X,m_nCmdRow, m_nCmdCol);
						return;
					}
					
					if (dMotGotoPos < 0)
					{
						MotorCompletion(M_HEATSINK_TRANSFER_X,m_nCmdRow, m_nCmdCol);
						//return;  //ybs
					}
					
					if (st_handler.mn_menu_lock != TRUE)
					{
						st_handler.mn_menu_lock = TRUE;
					}
					// =============================================================================
					
	// 				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
	// 				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_MANUAL );
					//kwlee 2017.0421
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X).MotorStepClear();
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_X).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_MANUAL );
					str_tmp.Format( "[HeatSink_X Motor] Axis Move start -> %.3f", dMotGotoPos);
					Func.On_LogFile_Add(0, str_tmp);
				}
				break;

		case 5:
			if (COMI.mn_motorbd_init_end != TRUE)  
			{
				if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
					return ;
			}
		
			value = g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
			
			nResponse = Func.On_Encoder_Check(value);

			if (nResponse == CTL_YES)
			{
// 				if( AMotor::CheckTeachSafety( m_n_axis, lpcc->Row-1, value ) == false )
// 				{
// 					m_strTempMSG = "이 티칭값은 위험할 수도 있습니다. 계속하시겠습니까?";
// 					if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("It might dangerous this teaching value. Do you want to continue?");
// 					if( DoModal_Select( m_strTempMSG ) != IDOK )
// 					{
// 						g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
// 						break;
// 					}
// 				}
				//m_d_position[lpcc->Row-1][1] = value;
				//strTmp.Format("%.3f", m_d_position[lpcc->Row-1][1]);
				//kwlee 2017.0421
				m_dBoatPos_X[1][m_nBcrNum][m_nPos] = value;
				strTmp.Format("%.3f", m_dBoatPos_X[1][m_nBcrNum][m_nPos]);
				m_p_grid.GridCellText(m_grid_Boat_pos_X, lpcc->Row, 3, strTmp);	
			}
			//MotorCompletion(m_n_cmd_row, m_n_cmd_col);
			break;
		}
 	}
 	else if (wParam == IDC_CUSTOM_BOAT_PART_Y)
 	{
		m_nAxis = IDC_CUSTOM_BOAT_PART_Y;
 	
		m_grid_Boat_pos_Y = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_Y);
		
		switch(lpcc->Col)
		{
		
			case 3:				
				strText = m_p_grid.GridCellText(m_grid_Boat_pos_Y, lpcc->Row, lpcc->Col - 1);
				st_msg.mstr_keypad_msg.Format("[HeatSink_Y Motor] %s set",  strText);

				strTmp = m_p_grid.GridCellText(m_grid_Boat_pos_Y, lpcc->Row, lpcc->Col);
				st_msg.mstr_keypad_val = strTmp;
				
			//	strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[m_n_axis][0], COMI.md_limit_position[m_n_axis][1], double(atof(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				//kwlee 2017.0421
				strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[M_HEATSINK_TRANSFER_Y][0], COMI.md_limit_position[M_HEATSINK_TRANSFER_Y][1], double(atof(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				
	// 			if( AMotor::CheckTeachSafety( m_n_axis, lpcc->Row-1, atof(strTmp) ) == false )
	// 			{
	// 				m_strTempMSG = "이 티칭값은 위험할 수도 있습니다. 계속하시겠습니까?";
	// 				if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("It might dangerous this teaching value. Do you want to continue?");
	// 				if( DoModal_Select( m_strTempMSG ) != IDOK )
	// 				{
	// 					g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
	// 					break;
	// 				}
	// 			}
	// 			m_d_position[lpcc->Row-1][1] = atof(strTmp);
	// 			m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, strTmp);
				//kwlee 2017.0421
				
				
				//st_BoatTeaching[nBcrNum].m_dPos_x[nPos][1] = atof(strTmp);
				//kwlee 2017.0421
				m_dBoatPos_Y[1][m_nBcrNum][m_nPos] = atof(strTmp);
				m_p_grid.GridCellText(m_grid_Boat_pos_Y, lpcc->Row, lpcc->Col, strTmp);
				break;

			case 4:
				{
					if (COMI.mn_motorbd_init_end != TRUE)  
					{
						if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
							return ;
					}
					
					m_p_grid.GridCellText(m_grid_Boat_pos_Y, lpcc->Row, lpcc->Col, "ing...");
					m_p_grid.GridCellColor(m_grid_Boat_pos_Y, lpcc->Row, lpcc->Col, NOR_C, BLACK_C);
					
					//double dMotGotoPos = m_d_position[lpcc->Row-1][1];
					//kwlee 2017.0421
					double dMotGotoPos = m_dBoatPos_Y[1][m_nBcrNum][m_nPos];
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y).SetMotGotoPos( dMotGotoPos );
					
					nResponse = Func.OnMotor_Go_Check(M_HEATSINK_TRANSFER_Y, dMotGotoPos);
					
					if (nResponse != IDOK)
					{
						MotorCompletion(M_HEATSINK_TRANSFER_Y,m_nCmdRow, m_nCmdCol);
						return;
					}
					
					if (dMotGotoPos < 0)
					{
						MotorCompletion(M_HEATSINK_TRANSFER_Y,m_nCmdRow, m_nCmdCol);
						//return;  //ybs
					}
					
					if (st_handler.mn_menu_lock != TRUE)
					{
						st_handler.mn_menu_lock = TRUE;
					}
					// =============================================================================
					
	// 				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
	// 				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_MANUAL );
					//kwlee 2017.0421
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y).MotorStepClear();
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Y).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_MANUAL );
					str_tmp.Format( "[HeatSink_Y Motor] Axis Move start -> %.3f", dMotGotoPos);
					Func.On_LogFile_Add(0, str_tmp);
				}
				break;

		case 5:
			if (COMI.mn_motorbd_init_end != TRUE)  
			{
				if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
					return ;
			}
		
			value = g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
			
			nResponse = Func.On_Encoder_Check(value);

			if (nResponse == CTL_YES)
			{
// 				if( AMotor::CheckTeachSafety( m_n_axis, lpcc->Row-1, value ) == false )
// 				{
// 					m_strTempMSG = "이 티칭값은 위험할 수도 있습니다. 계속하시겠습니까?";
// 					if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("It might dangerous this teaching value. Do you want to continue?");
// 					if( DoModal_Select( m_strTempMSG ) != IDOK )
// 					{
// 						g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
// 						break;
// 					}
// 				}
				//m_d_position[lpcc->Row-1][1] = value;
				//strTmp.Format("%.3f", m_d_position[lpcc->Row-1][1]);
				//kwlee 2017.0421
				m_dBoatPos_Y[1][m_nBcrNum][m_nPos] = value;
				strTmp.Format("%.3f", m_dBoatPos_Y[1][m_nBcrNum][m_nPos]);
				m_p_grid.GridCellText(m_grid_Boat_pos_Y, lpcc->Row, 3, strTmp);	
			}
			//MotorCompletion(m_n_cmd_row, m_n_cmd_col);
			break;
		}
	}
	else if (wParam == IDC_CUSTOM_BOAT_PART_Z)
	{

		m_nAxis = IDC_CUSTOM_BOAT_PART_Z;

		m_grid_Boat_pos_Z = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_Z);
 			
		switch(lpcc->Col)
		{
		
			case 3:				
				strText = m_p_grid.GridCellText(m_grid_Boat_pos_Z, lpcc->Row, lpcc->Col - 1);
				st_msg.mstr_keypad_msg.Format("[HeatSink_Z Motor] %s set",  strText);

				strTmp = m_p_grid.GridCellText(m_grid_Boat_pos_Z, lpcc->Row, lpcc->Col);
				st_msg.mstr_keypad_val = strTmp;
				
			//	strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[m_n_axis][0], COMI.md_limit_position[m_n_axis][1], double(atof(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				//kwlee 2017.0421
				strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[M_HEATSINK_TRANSFER_Z][0], COMI.md_limit_position[M_HEATSINK_TRANSFER_Z][1], double(atof(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				
	// 			if( AMotor::CheckTeachSafety( m_n_axis, lpcc->Row-1, atof(strTmp) ) == false )
	// 			{
	// 				m_strTempMSG = "이 티칭값은 위험할 수도 있습니다. 계속하시겠습니까?";
	// 				if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("It might dangerous this teaching value. Do you want to continue?");
	// 				if( DoModal_Select( m_strTempMSG ) != IDOK )
	// 				{
	// 					g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
	// 					break;
	// 				}
	// 			}
	// 			m_d_position[lpcc->Row-1][1] = atof(strTmp);
	// 			m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, strTmp);
				//kwlee 2017.0421
				
				
				//st_BoatTeaching[nBcrNum].m_dPos_x[nPos][1] = atof(strTmp);
				//kwlee 2017.0421
				m_dBoatPos_Z[1][m_nBcrNum][m_nPos] = atof(strTmp);
				m_p_grid.GridCellText(m_grid_Boat_pos_Z, lpcc->Row, lpcc->Col, strTmp);
				break;

			case 4:
				{
					if (COMI.mn_motorbd_init_end != TRUE)  
					{
						if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
							return ;
					}
					
					m_p_grid.GridCellText(m_grid_Boat_pos_Z, lpcc->Row, lpcc->Col, "ing...");
					m_p_grid.GridCellColor(m_grid_Boat_pos_Z, lpcc->Row, lpcc->Col, NOR_C, BLACK_C);
					
					//double dMotGotoPos = m_d_position[lpcc->Row-1][1];
					//kwlee 2017.0421
					double dMotGotoPos = m_dBoatPos_Z[1][m_nBcrNum][m_nPos];
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z).SetMotGotoPos( dMotGotoPos );
					
					nResponse = Func.OnMotor_Go_Check(M_HEATSINK_TRANSFER_Z, dMotGotoPos);
					
					if (nResponse != IDOK)
					{
						MotorCompletion(M_HEATSINK_TRANSFER_Z,m_nCmdRow, m_nCmdCol);
						return;
					}
					
					if (dMotGotoPos < 0)
					{
						MotorCompletion(M_HEATSINK_TRANSFER_Z,m_nCmdRow, m_nCmdCol);
						//return;  //ybs
					}
					
					if (st_handler.mn_menu_lock != TRUE)
					{
						st_handler.mn_menu_lock = TRUE;
					}
					// =============================================================================
					
	// 				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
	// 				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_MANUAL );
					//kwlee 2017.0421
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z).MotorStepClear();
					GETMOTOR( (ENUM_MOTOR)M_HEATSINK_TRANSFER_Z).MotorMoveStart( dMotGotoPos, st_basic.nManualSpeed, MMMODE_MANUAL );
					str_tmp.Format( "[HeatSink_Z Motor] Axis Move start -> %.3f", dMotGotoPos);
					Func.On_LogFile_Add(0, str_tmp);
				}
				break;

		case 5:
			if (COMI.mn_motorbd_init_end != TRUE)  
			{
				if (DoModal_Msg( g_local.Get("L_M_0000") ) == IDOK)
					return ;
			}
		
			value = g_comiMgr.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
			
			nResponse = Func.On_Encoder_Check(value);

			if (nResponse == CTL_YES)
			{
// 				if( AMotor::CheckTeachSafety( m_n_axis, lpcc->Row-1, value ) == false )
// 				{
// 					m_strTempMSG = "이 티칭값은 위험할 수도 있습니다. 계속하시겠습니까?";
// 					if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTempMSG = _T("It might dangerous this teaching value. Do you want to continue?");
// 					if( DoModal_Select( m_strTempMSG ) != IDOK )
// 					{
// 						g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_ON);
// 						break;
// 					}
// 				}
				//m_d_position[lpcc->Row-1][1] = value;
				//strTmp.Format("%.3f", m_d_position[lpcc->Row-1][1]);
				//kwlee 2017.0421
				m_dBoatPos_Z[1][m_nBcrNum][m_nPos] = value;
				strTmp.Format("%.3f", m_dBoatPos_Z[1][m_nBcrNum][m_nPos]);
				m_p_grid.GridCellText(m_grid_Boat_pos_Z, lpcc->Row, 3, strTmp);	
			}
			//MotorCompletion(m_n_cmd_row, m_n_cmd_col);
			break;
		}
	}
	//kwlee 2017.0422
	else if (wParam == IDC_CUSTOM_POS_X_INFO)
	{
		m_grid_Boat_info_X = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_X_INFO);
		
		switch(lpcc->Row)
		{
				
		case 2:
			// Motor Move
			switch(lpcc->Col)
			{
			case 1:
				if (m_n_move_mode_X != 0)
				{
					m_n_move_mode_X = 0;
					
					m_p_grid.GridCellColor(m_grid_Boat_info_X, 8, 1, RED_C, BLACK_C);
					m_p_grid.GridCellColor(m_grid_Boat_info_X, 8, 2, BLACK_L, YELLOW_C);
				}
				break;
				
			case 2:
				if (m_n_move_mode_X != 1)
				{
					m_n_move_mode_X = 1;
					
					m_p_grid.GridCellColor(m_grid_Boat_info_X, 8, 1, BLACK_L, YELLOW_C);
					m_p_grid.GridCellColor(m_grid_Boat_info_X, 8, 2, RED_C, BLACK_C);
				}
				break;
				
			case 5:
			case 6:
				strTmp.Format("%.3f", m_d_data_x);
				st_msg.mstr_keypad_msg.Format("[Jog Data]  set");
				st_msg.mstr_keypad_val = strTmp;
				
				strTmp = KeyPad.GetNumEditString_D((double)0.001, (double)10000, double(atoi(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
				
				m_d_data_x = atof(strTmp);
				m_p_grid.GridCellText(m_grid_Boat_info_X, 8, 5, strTmp);
				break;
			}
			break;
		}
	}
	else if (wParam == IDC_CUSTOM_POS_Y_INFO)
	{
		m_grid_Boat_info_Y = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_Y_INFO);
		
		switch(lpcc->Row)
		{
			
			case 2:
				// Motor Move
				switch(lpcc->Col)
				{
					case 1:
						if (m_n_move_mode_Y != 0)
						{
							m_n_move_mode_Y = 0;
							
							m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 1, RED_C, BLACK_C);
							m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 2, BLACK_L, YELLOW_C);
						}
						break;
						
					case 2:
						if (m_n_move_mode_Y != 1)
						{
							m_n_move_mode_Y = 1;
							
							m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 1, BLACK_L, YELLOW_C);
							m_p_grid.GridCellColor(m_grid_Boat_info_Y, 2, 2, RED_C, BLACK_C);
						}
						break;
						
					case 5:
					case 6:
						strTmp.Format("%.3f", m_d_data_y);
						st_msg.mstr_keypad_msg.Format("[Jog Data]  set");
						st_msg.mstr_keypad_val = strTmp;
						
						strTmp = KeyPad.GetNumEditString_D((double)0.001, (double)10000, double(atoi(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
						
						m_d_data_y = atof(strTmp);
						m_p_grid.GridCellText(m_grid_Boat_info_Y, 2, 5, strTmp);
						break;
					
				}
				break;
		}
	}
	else if (wParam == IDC_CUSTOM_POS_Z_INFO)
	{
		m_grid_Boat_info_Z = (TSpread*)GetDlgItem(IDC_CUSTOM_POS_Z_INFO);
		
		switch(lpcc->Row)
		{			
		case 2:	// Motor Move
			switch(lpcc->Col)
			{
				case 1:
					if (m_n_move_mode_Z != 0)
					{
						m_n_move_mode_Z = 0;
						
						m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 1, RED_C, BLACK_C);
						m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 2, BLACK_L, YELLOW_C);
					}
					break;
					
				case 2:
					if (m_n_move_mode_Z != 1)
					{
						m_n_move_mode_Z = 1;
						
						m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 1, BLACK_L, YELLOW_C);
						m_p_grid.GridCellColor(m_grid_Boat_info_Z, 2, 2, RED_C, BLACK_C);
					}
					break;
					
				case 5:
				case 6:
					strTmp.Format("%.3f", m_d_data_z);
					st_msg.mstr_keypad_msg.Format("[Jog Data]  set");
					st_msg.mstr_keypad_val = strTmp;
					
					strTmp = KeyPad.GetNumEditString_D((double)0.001, (double)10000, double(atoi(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
					
					m_d_data_z = atof(strTmp);
					m_p_grid.GridCellText(m_grid_Boat_info_Z, 2, 5, strTmp);
					break;
				}
				break;
			}
		}
	}



void CScreen_List_BoatTeacing::MotorCompletion(int nPos,int nrow, int ncol)
{
	if( nrow < 0 || ncol < 0 )
		return;
	
	if (nPos == M_HEATSINK_TRANSFER_X)
	{
		m_grid_Boat_pos_X = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_X);
		
		if (ncol == 4)
		{
			m_p_grid.GridCellText(m_grid_Boat_pos_X, nrow, ncol, "Go.");
			m_p_grid.GridCellColor(m_grid_Boat_pos_X, nrow, ncol, TEXT_BC, BLUE_C);
		}
		else
		{
			m_p_grid.GridCellText(m_grid_Boat_pos_X, nrow, ncol, "Read.");
			m_p_grid.GridCellColor(m_grid_Boat_pos_X, nrow, ncol, TEXT_BC, RED_C);
		}
	}
	else if (nPos == M_HEATSINK_TRANSFER_Y)
	{
		m_grid_Boat_pos_Y = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_Y);
		
		if (ncol == 4)
		{
			m_p_grid.GridCellText(m_grid_Boat_pos_Y, nrow, ncol, "Go.");
			m_p_grid.GridCellColor(m_grid_Boat_pos_Y, nrow, ncol, TEXT_BC, BLUE_C);
		}
		else
		{
			m_p_grid.GridCellText(m_grid_Boat_pos_Y, nrow, ncol, "Read.");
			m_p_grid.GridCellColor(m_grid_Boat_pos_Y, nrow, ncol, TEXT_BC, RED_C);
		}
	}
	else if (nPos == M_HEATSINK_TRANSFER_Z)
	{
		m_grid_Boat_pos_Z = (TSpread*)GetDlgItem(IDC_CUSTOM_BOAT_PART_Z);
		
		if (ncol == 4)
		{
			m_p_grid.GridCellText(m_grid_Boat_pos_Z, nrow, ncol, "Go.");
			m_p_grid.GridCellColor(m_grid_Boat_pos_Z, nrow, ncol, TEXT_BC, BLUE_C);
		}
		else
		{
			m_p_grid.GridCellText(m_grid_Boat_pos_Z, nrow, ncol, "Read.");
			m_p_grid.GridCellColor(m_grid_Boat_pos_Z, nrow, ncol, TEXT_BC, RED_C);
		}
	}
	else
	{

	}
}
void CScreen_List_BoatTeacing::OnInit_Button()
{
	short	shBtnColor = 30;
	CString strTmp;

	m_btn_emg_X.SetFont(mp_step_list_font);
	m_btn_emg_X.SetIcon(IDI_EMG_STOP);
	m_btn_emg_X.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_emg_X.SetButtonColor(1);
	strTmp.Format("HeatSink_X Emergency Step");
	m_btn_emg_X.SetTooltipText(strTmp);

	m_btn_emg_Y.SetFont(mp_step_list_font);
	m_btn_emg_Y.SetIcon(IDI_EMG_STOP);
	m_btn_emg_Y.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_emg_Y.SetButtonColor(1);
	strTmp.Format("HeatSink_Y Emergency Step");
	m_btn_emg_Y.SetTooltipText(strTmp);

	m_btn_emg_Z.SetFont(mp_step_list_font);
	m_btn_emg_Z.SetIcon(IDI_EMG_STOP);
	m_btn_emg_Z.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_emg_Z.SetButtonColor(1);
	strTmp.Format("HeatSink_Z Emergency Step");
	m_btn_emg_X.SetTooltipText(strTmp);

	//left
	m_btn_left_X.SetIcon(IDI_LEFT_E);
	m_btn_left_X.SetTag(IDC_BTN_LEFT_X);
	m_btn_left_X.SetFont(mp_step_list_font);
	m_btn_left_X.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left_X.SetButtonColor(1);

	m_btn_left_Y.SetIcon(IDI_DN_E);
	m_btn_left_Y.SetTag(IDC_BTN_LEFT_Y);
	m_btn_left_Y.SetFont(mp_step_list_font);
	m_btn_left_Y.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left_Y.SetButtonColor(1);

	m_btn_left_Z.SetIcon(IDI_UP_E);
	m_btn_left_Z.SetTag(IDC_BTN_LEFT_Z);
	m_btn_left_Z.SetFont(mp_step_list_font);
	m_btn_left_Z.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left_Z.SetButtonColor(1);

	
	//Right
	m_btn_right_X.SetIcon( IDI_RIGHT_E );
	m_btn_right_X.SetTag(IDC_BTN_RIGHT_X);
	m_btn_right_X.SetFont(mp_step_list_font);
	m_btn_right_X.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right_X.SetButtonColor(1);


	m_btn_right_Y.SetIcon(IDI_UP_E);
	m_btn_right_Y.SetTag(IDC_BTN_RIGHT_Y);
	m_btn_right_Y.SetFont(mp_step_list_font);
	m_btn_right_Y.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right_Y.SetButtonColor(1);

	m_btn_right_Z.SetIcon(IDI_DN_E);
	m_btn_right_Z.SetTag(IDC_BTN_RIGHT_Z);
	m_btn_right_Z.SetFont(mp_step_list_font);
	m_btn_right_Z.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right_Z.SetButtonColor(1);
	
	m_btn_home_X.SetTag(IDC_BTN_HOME);
	m_btn_home_X.SetFont(mp_step_list_font);
	m_btn_home_X.SetIcon(IDI_HOME_E);
	m_btn_home_X.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home_X.SetButtonColor(1);

	m_btn_home_Y.SetTag(IDC_BTN_HOME);
	m_btn_home_Y.SetFont(mp_step_list_font);
	m_btn_home_Y.SetIcon(IDI_HOME_E);
	m_btn_home_Y.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home_Y.SetButtonColor(1);

	m_btn_home_Z.SetTag(IDC_BTN_HOME);
	m_btn_home_Z.SetFont(mp_step_list_font);
	m_btn_home_Z.SetIcon(IDI_HOME_E);
	m_btn_home_Z.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home_Z.SetButtonColor(1);
}
