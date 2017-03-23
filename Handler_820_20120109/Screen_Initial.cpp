// Screen_Initial.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Screen_Initial.h"

/* ****************************************************************************** */
/* 대화 상자 클래스 추가                                                          */
/* ****************************************************************************** */
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
/* ****************************************************************************** */
#include "FastechPublic_IO.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial

IMPLEMENT_DYNCREATE(CScreen_Initial, CFormView)

extern CHANDLERApp theApp;
CScreen_Initial::CScreen_Initial()
	: CFormView(CScreen_Initial::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Initial)
	//}}AFX_DATA_INIT
//	int nleft, nright;

	m_nDataSendCnt = 0;
/*	st_work.n_left_init = st_work.n_right_init = NO;

	nleft = nright = 0;
	for(int i=0; i<48; i++){
		if(st_modulemap.DeviceLeftRack[D_EXIST][i] != DEV_NONE){
			nleft++;
		}

		if(st_modulemap.DeviceRightRack[D_EXIST][i] != DEV_NONE){
			nright++;
		}
	} 

	if(nleft > 0){
		st_work.n_left_init = YES;
	}

	if(nright > 0){
		st_work.n_right_init = YES;
	}*/
}

CScreen_Initial::~CScreen_Initial()
{
}

void CScreen_Initial::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Initial)
	DDX_Control(pDX, IDC_RADIO_TRANSFER_RIGHT, m_radio_transfer_right);
	DDX_Control(pDX, IDC_RADIO_TRANSFER_LEFT, m_radio_transfer_left);
	DDX_Control(pDX, IDC_RADIO_TEST_SITE, m_radio_test_site);
	DDX_Control(pDX, IDC_RADIO_TEST_DUMP, m_radio_test_dump);
	DDX_Control(pDX, IDC_RADIO_ROTATOR_TABLE, m_radio_rotator_table);
	DDX_Control(pDX, IDC_RADIO_ROTATOR_DUMP, m_radio_rotator_dump);
	DDX_Control(pDX, IDC_RADIO_MAIN_XYZ, m_radio_main_xyz);
	DDX_Control(pDX, IDC_RADIO_ELEVATOR_RIGHT, m_radio_elevator_right);
	DDX_Control(pDX, IDC_RADIO_ELEVATOR_LEFT, m_radio_elevator_left);
	DDX_Control(pDX, IDC_RADIO_CONVEYOR, m_radio_conveyor);
	DDX_Control(pDX, IDC_RADIO_MOT_BD, m_radio_mot_bd);
	DDX_Control(pDX, IDC_RADIO_IO_BD, m_radio_io_bd);
	DDX_Control(pDX, IDC_MSG_INITIAL, m_msg_initial);
	DDX_Control(pDX, IDC_GIF_INITIALIZING, m_gif_initializing);
	DDX_Control(pDX, IDC_BTN_INIT_SKIP, m_btn_init_skip);
	DDX_Control(pDX, IDC_BTN_INIT_RETRY, m_btn_init_retry);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Initial, CFormView)
	//{{AFX_MSG_MAP(CScreen_Initial)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INIT_SKIP, OnBtnInitSkip)
	ON_BN_CLICKED(IDC_BTN_INIT_RETRY, OnBtnInitRetry)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WORK_END, OnWorkMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial diagnostics

#ifdef _DEBUG
void CScreen_Initial::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Initial::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial message handlers

void CScreen_Initial::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
	mp_init_font = NULL;
	mp_init_font = new CFont;
	mp_init_font->CreateFont(35,20,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial Black Italic");
	// **************************************************************************

	st_handler.n_motor_init_check = READY;

	OnInitial_Progress_Set();
	OnInitial_Picture_Set();
	OnInitial_Lable_Set();
	OnInitial_RadioButton_Set();
	OnInitial_Controls_Enable(FALSE);

	OnInitial_Change_Status(0);

	OnInitial_Step_Clear();

	OnInitial_Initial_Ready();

	st_handler.cwnd_initial = this;
}



void CScreen_Initial::OnTimer(UINT nIDEvent) 
{
	int mn_chk_run;			// 초기화 진행 함수 리턴 플래그 설정 변수
	int n_response;
	
	CDialog_Message  msg_dlg;
	
	if(nIDEvent == TMR_INIT)  
	{
		if (st_handler.n_menu_lock != TRUE)  st_handler.n_menu_lock = TRUE;			// 메뉴 사용 불가능 상태 플래그 설정 
		
		// **********************************************************************
		// 초기화 작업을 진행한다.                                               
		// **********************************************************************
		mn_chk_run = OnInitial_Init_Excution(); // 초기화 작업 진행 함수
		
		if (mn_chk_run == RET_GOOD)
		{
			KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 
			
			st_other.str_normal_msg = _T("[초 기 화] 장비 초기화 완료");
			sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
			if (st_handler.cwnd_list != NULL)
			{
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			st_handler.n_mode_manual = YES;
			
			// ******************************************************************
			// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
			// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
			// ******************************************************************
			if (st_work.n_run_status != dSTOP)
			{
				alarm.mn_reset_status = NO;
				
				FAS_IO.Set_Out_Bit(st_io.o_Start_SwitchLamp, IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_Stop_SwitchLamp, IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer1, IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer2, IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer3, IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer4, IO_OFF);

				FAS_IO.Set_Out_Bit(st_io.o_tower_lamp[0], IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_tower_lamp[1], IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_tower_lamp[2], IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_tower_lamp[3], IO_OFF);
//				Func.OnSet_IO_Port_Stop(); // 장비 상태 : 정지 상태인 경우 I/O 출력 내보내는 함수

				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, 2, dSTOP);
			}
			// ******************************************************************
			
			if (st_handler.n_initial != TRUE)  st_handler.n_initial = TRUE;			// 초기화 작업 완료 플래그 설정 
			if (st_handler.n_menu_lock != FALSE)  st_handler.n_menu_lock = FALSE;	// 메뉴 사용 가능 상태 플래그 설정
			if (st_handler.n_lot_flag != CTL_LOTEND_FINISH) st_handler.n_lot_flag = CTL_LOTEND_FINISH;
			
			// 초기화가 정상적으로 끝났슴을 알려준다.
			// 			FAS_IO.Set_Out_Bit(st_io_def.o_reset, IO_ON);
			// 			FAS_IO.Set_Out_Bit(st_io_def.o_buzzer, IO_ON);
			
//			st_other.str_fallacy_msg = _T("Initialization completion");
			Func.OnSet_IO_Port_Stop(); 
			st_other.str_fallacy_msg = _T("장비 초기화 완료");
			
			n_response = msg_dlg.DoModal();
			
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면 전환 
		}
		else if (mn_chk_run == RET_ERROR)
		{
			KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 
			
			st_other.str_abnormal_msg = _T("[초 기 화] 장비 초기화 실패");
			sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
			
			st_handler.n_mode_manual = YES;			// 시작은 메뉴얼 모드이다.

//			FAS_IO.Set_Out_Bit(st_io_def.o_manual_mode, IO_ON);
			// ******************************************************************
			// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
			// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
			// ******************************************************************
/* //jong 1215
			if (st_work.n_run_status != dSTOP)
			{
				// 				FAS_IO.Set_Out_Bit(st_io_def.o_reset, IO_OFF);
				// 				FAS_IO.Set_Out_Bit(st_io_def.o_clear, IO_OFF);
				
				Func.OnSet_IO_Port_Stop(); // 장비 상태 : 정지 상태인 경우 I/O 출력 내보내는 함수
				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, 2, dSTOP);
			}
*/
			// ******************************************************************
			
			OnInitial_Controls_Enable(TRUE); // 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수
			
			// 초기화가 정상적으로 끝났슴을 알려준다.
			// 			FAS_IO.Set_Out_Bit(st_io_def.o_reset, IO_ON);
			// 			FAS_IO.Set_Out_Bit(st_io_def.o_buzzer, IO_ON);
			
			if (st_handler.n_initial != FALSE)		st_handler.n_initial = FALSE;	// 초기화 작업 완료 여부 초기화 
			if (st_handler.n_menu_lock != FALSE)	st_handler.n_menu_lock = FALSE; // 메뉴 사용 가능 상태 플래그 설정 
		}
		// ******************************************************************************
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Initial::OnInitial_Lable_Set()
{
	m_msg_initial.SetFont(mp_init_font);
	m_msg_initial.SetWindowText(_T("initializing~!! Wait moment~"));
	m_msg_initial.SetCenterText();
	m_msg_initial.SetColor(RGB(0,0,0));
	m_msg_initial.SetGradientColor(RGB(0,0,255));
	m_msg_initial.SetTextColor(RGB(255,255,255));
}

void CScreen_Initial::OnInitial_Picture_Set()
{
	if (m_gif_initializing.Load(MAKEINTRESOURCE(IDR_GIF_INITIALIZING),_T("GIF")))
		m_gif_initializing.Draw();
}

void CScreen_Initial::OnInitial_Progress_Set()
{
	CSuperProgressCtrl::RegisterClass();
	
	m_ctrlProgress.Create(this, 4, 250, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_1)), 0);
	m_ctrlProgress.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress.SetColours(RGB(255,0,0), RGB(0,0,0));
	m_ctrlProgress.SetRange(0, 50);
	m_ctrlProgress.SetStep(1);

	m_ctrlProgress1.Create(this, 4, 400, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_2)), 0);
	m_ctrlProgress1.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress1.SetColours(RGB(0,0,255), RGB(0,0,0));
	m_ctrlProgress1.SetRange(50, 100);
	m_ctrlProgress1.SetStep(1);
}

void CScreen_Initial::OnInitial_RadioButton_Set()
{
	m_radio_io_bd.SetImage(IDB_REDBUTTON, 15);
	m_radio_mot_bd.SetImage(IDB_REDBUTTON, 15);
	m_radio_conveyor.SetImage(IDB_REDBUTTON, 15);
	m_radio_main_xyz.SetImage(IDB_REDBUTTON, 15);
	m_radio_elevator_left.SetImage(IDB_REDBUTTON, 15);
	m_radio_elevator_right.SetImage(IDB_REDBUTTON, 15);
	m_radio_transfer_left.SetImage(IDB_REDBUTTON, 15);
	m_radio_transfer_right.SetImage(IDB_REDBUTTON, 15);
	m_radio_test_site.SetImage(IDB_REDBUTTON, 15);
	m_radio_test_dump.SetImage(IDB_REDBUTTON, 15);
	m_radio_rotator_table.SetImage(IDB_REDBUTTON, 15);
	m_radio_rotator_dump.SetImage(IDB_REDBUTTON, 15);
}

void CScreen_Initial::OnInitial_Controls_Enable(int n_init)
{
	if (n_init == TRUE)  m_btn_init_retry.ShowWindow(SW_SHOW);  
	else                 m_btn_init_retry.ShowWindow(SW_HIDE);
}

void CScreen_Initial::OnInitial_Change_Status(int n_status)
{
	switch(n_status)
	{
		case 0:  // 이니셜 초기화인 경우 
			m_radio_io_bd.Depress(FALSE);
			m_radio_mot_bd.Depress(FALSE);
			m_radio_main_xyz.Depress(FALSE);
			m_radio_elevator_left.Depress(FALSE);
			m_radio_elevator_right.Depress(FALSE);
			m_radio_transfer_left.Depress(FALSE);
			m_radio_transfer_right.Depress(FALSE);
			m_radio_test_site.Depress(FALSE);
			m_radio_test_dump.Depress(FALSE);
			m_radio_rotator_dump.Depress(FALSE);
			m_radio_rotator_table.Depress(FALSE);
			m_radio_conveyor.Depress(FALSE);
			break;
		case 1:
			m_radio_mot_bd.Depress(TRUE);
			break;
		case 2:
			m_radio_io_bd.Depress(TRUE);
			break;
		case 3:
			m_radio_main_xyz.Depress(TRUE);
			break;
		case 4:
			m_radio_elevator_left.Depress(TRUE);
			break;
		case 5:
			m_radio_elevator_right.Depress(TRUE);
			break;
		case 6:
			m_radio_transfer_left.Depress(TRUE);
			break;
		case 7:
			m_radio_transfer_right.Depress(TRUE);
			break;
		case 8:
			m_radio_test_site.Depress(TRUE);
			break;
		case 9:
			m_radio_test_dump.Depress(TRUE);
			break;
		case 10:
			m_radio_rotator_dump.Depress(TRUE);
			break;
		case 11:
			m_radio_rotator_table.Depress(TRUE);
			break;
		case 12:
			m_radio_conveyor.Depress(TRUE);
			break;
	}
}

void CScreen_Initial::OnInitial_Step_Clear()
{
	int i;

	ml_init_step = 0;			// 초기화 진행 스텝 정보 저장 변수 초기화 
	mn_motor_init_count = 0;
	mn_pos_step = 0;					// 프로그레서 위치 정보 초기화
	
	for (i = 0; i < 30; i++)
	{
		st_sync.nLotEndFlag[i] = NO; // lot end 여부를 판가름 한다 YES이면 해당 쓰레드 LOT END
	}

	for(i=0; i<MAX_MOTOR; i++)
	{
		st_sync.n_init_flag[i] = CTL_NO;
	}

	st_handler.n_initial_flag			= NO;

	/*
	st_work.n_init_sorter_robot			= -1;
    st_work.n_init_uld_el_left			= -1;	
	st_work.n_init_uld_el_right			= -1;
    st_work.n_init_uld_tf_left			= -1;
	st_work.n_init_uld_tf_right			= -1;
    st_work.n_init_test_site			= -1; 
	st_work.n_init_test_dump			= -1;
    st_work.n_init_index_dump			= -1;
	st_work.n_init_index_table			= -1;
	st_work.n_init_conveyor				= -1;
	*/

	st_handler.n_level_maint			= FALSE;					
	st_handler.n_level_teach			= FALSE;	

}

void CScreen_Initial::OnInitial_Initial_Ready()
{
	int n_response;					// 대화 상자에 대한 리턴 값 저장 변수
	CDialog_Select select_dlg;
	char chr_data[25];	
	CString str_lock_check;
	
	st_init.n_initial_error = READY;

	// Lock상태 였는지 확인을 한다.
	:: GetPrivateProfileString("Password", "SysLockStatus", "NO", chr_data, 20, st_path.mstr_basic);
	str_lock_check = chr_data;

	if (str_lock_check == "YES")			// Lock상태에서 강제 종료한것이라면...
	{
		// **************************************************************************
		// Lock 화면으로 전환하여 장비 사용을 막는다.
		// **************************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // 메인 프레임에 SYSTEM LOCK 화면 출력 요청
		// **************************************************************************
		if (st_handler.cwnd_list != NULL)
		{
			st_other.str_normal_msg = "System Locking.";
			sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		return;
	}
	
	if (st_handler.n_motor_init_check == READY)			// 모터 초기화를 할것인지 말것인지 결정한다. 2K4/11/16/ViboX
	{
//		st_other.str_confirm_msg = _T("Handler Motor Init?");
		st_other.str_confirm_msg = _T("장비 초기화를 진행하시겠습니까?");
		
		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			// **************************************************************************
			// 현재 장비 상태 (INIT) 타이틀 바에 출력한다.                               
			// **************************************************************************
			if (st_work.n_run_status != dINIT)
			{
				Func.OnSet_IO_Port_Init(); // 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.n_run_status);
			}
			// **************************************************************************
			st_handler.n_motor_init_check = YES;
		}
		else if (n_response == IDCANCEL)
		{
			// **************************************************************************
			// 현재 장비 상태 (INIT) 타이틀 바에 출력한다.                               
			// **************************************************************************
			if (st_work.n_run_status != dINIT)
			{
				Func.OnSet_IO_Port_Init(); // 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.n_run_status);
			}
			// **************************************************************************
			st_handler.n_motor_init_check = NO;
		}
	}
	
	SetTimer(TMR_INIT, 250, NULL);  // 초기화 작업 진행 타이머 생성
}

int CScreen_Initial::OnInitial_Init_Excution()
{
	int Ret;
	int i, j = 0;
	CString str_macaddr;

	CDialog_Message msg_dlg;

	mn_init_flag = RET_PROCEED;

	if (st_work.n_run_status != dINIT)  return mn_init_flag;				// 현재 장비 상태가 RUN 아니면 리턴한다.

	switch (ml_init_step)
	{
		case 0:
/*			if(Func.DoorOpenCheck() == RET_GOOD)
			{
				ml_init_step = 1;
			}
			else
			{
				alarm.mstr_code = _T("900133"); 
				alarm.mn_count_mode = 0;
				alarm.mn_type_mode = eWARNING;
				st_work.n_run_status  = dWARNING;
				return RET_ERROR;
			}*/
			
			ml_init_step = 10;
			break;
		case 1 :
			if(st_init.n_manual == YES)
			{	
				ml_init_step = 5;
				mn_pos_step = 9;
				break;
			}

//			Func.OnSet_IO_Port_Load_Off();					// Loader key가 눌려있었을수가 있으니까 꺼준다.
			mn_pos_step = 9;

			ml_init_step = 5;
			break;
		case 5 :	
			if (FAS_IO.mn_io_board_initial_flag == BD_NO)  
			{
				alarm.mstr_code = _T("900001");			// 현재 발생한 알람 코드 정보 설정 
				alarm.mn_count_mode = 0;					// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;			// 현재 발생한 알람 상태 플래그 설정 
				st_work.n_run_status = dWARNING;		// dJAM,dWARNING 

				ml_init_step = 0;						// 초기화 진행 스텝 정보 초기화 
				mn_init_flag = RET_ERROR;				// 초기화 작업 에러 플래그 설정

				// **************************************************************************
				//  MAC ADDRESS를 이용하여 개발용인지 확인한다.                              
				// **************************************************************************
				str_macaddr = Func.GetLocalMacAddress();

				str_macaddr.MakeUpper();

				if(str_macaddr != "00-1A-92-04-B5-BA" || str_macaddr != "00-1D-7D-08-B8-B7")
				{
//j					::PostQuitMessage(0);
				}
			}
			else
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_other.str_normal_msg = _T("I/O Board Initialized...");
					sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				// **********************************************************************
				// I/O 보드 입/출력 상태를 확인하는 쓰레드 생성한다                
				// **********************************************************************
				m_thread[3]=AfxBeginThread(OnThread_IO_Check, this);
				if (m_thread[3] != NULL) 	
					hThrHandle[3] = m_thread[3]->m_hThread;
				// **********************************************************************

				Func.OnSet_IO_Out_Port_Clear();

				mn_pos_step += 8;
		
				OnInitial_Change_Status(0);				// I/O보드 초기화 완료.
				ml_init_step = 10;
			}
			break;

		case 10 :
			if (st_handler.n_mot_board_initial == TRUE)
			{
				mn_pos_step += 8;
				
				OnInitial_Change_Status(1);				// MOTOR 보드 초기화 완료.
				
				ml_init_step = 15;
				break;
			}

			Ret = COMI.Initialize_MotBoard("c:\\AMT820\\Setting\\Cutting.cme2");			// MOTOR 보드 초기화 여부 검사한다
			
			if (Ret == CTLBD_RET_ERROR)
			{
				alarm.mstr_code = _T("900000");			// 현재 발생한 알람 코드 정보 설정 
				alarm.mn_count_mode = 0;					// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;			// 현재 발생한 알람 상태 플래그 설정 
				st_work.n_run_status = dWARNING;		// dJAM,dWARNING 
				
				st_handler.n_mot_board_initial = FALSE;
				ml_init_step = 0;						// 초기화 진행 스텝 정보 초기화 
				mn_init_flag = RET_ERROR;				// 초기화 작업 에러 플래그 설정 
				// **************************************************************************
				//  MAC ADDRESS를 이용하여 개발용인지 확인한다.                              
				// **************************************************************************
				str_macaddr = Func.GetLocalMacAddress();
				
				str_macaddr.MakeUpper();
				
				if (str_macaddr != "00-1A-92-04-B5-BA" || str_macaddr != "00-1D-7D-08-B8-B7")
				{
					//j					::PostQuitMessage(0);
				}
				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_other.str_abnormal_msg = _T("[MOTOR BOARD] 초기화 에러.");
					sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

	 			return RET_ERROR;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_other.str_normal_msg = _T("Motor Board Open Success!");
					sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
				
				for (i = 0; i < MAXMOTOR; i++)
				{
					st_handler.n_home_state[i] = NO;
				}
				
				Func.OnMot_Speed_Setting();					// MOTOR 보드가 정상적으로 OPEN이 된 후에 속도를 세팅한다. 2K5/03/24/ViboX

				st_handler.n_mot_board_initial = TRUE;
				mn_pos_step += 8;
				
				OnInitial_Change_Status(2);					// MOTOR 보드 초기화 완료.
/*				
				motor[0] = M_SORTER_X;
				motor[1] = M_SORTER_Y;

				st_linearmot[SORTER_MOT_MAP].l_AxisCnt			= 2;
				st_linearmot[SORTER_MOT_MAP].lp_AxisNum[0]		= motor[0];
				st_linearmot[SORTER_MOT_MAP].lp_AxisNum[1]		= motor[1];

				COMI.Create_LmIndexMapAxis(SORTER_MOT_MAP, 2, motor);

				motor[0] = M_INDEX_DUMP_X;
				motor[1] = M_INDEX_DUMP_Z;

				st_linearmot[INDEX_DUMP_MOT_MAP].l_AxisCnt		= 2;2
				st_linearmot[INDEX_DUMP_MOT_MAP].lp_AxisNum[0]	= motor[0];
				st_linearmot[INDEX_DUMP_MOT_MAP].lp_AxisNum[1]	= motor[1];
				
				COMI.Create_LmIndexMapAxis(INDEX_DUMP_MOT_MAP, 2, motor);
				
				motor[0] = M_TEST_DUMP_Y;
				motor[1] = M_TEST_DUMP_Z;
				
				st_linearmot[TEST_DUMP_MOT_MAP].l_AxisCnt		= 2;
				st_linearmot[TEST_DUMP_MOT_MAP].lp_AxisNum[0]	= motor[0];
				st_linearmot[TEST_DUMP_MOT_MAP].lp_AxisNum[1]	= motor[1];
				
				COMI.Create_LmIndexMapAxis(TEST_DUMP_MOT_MAP, 2, motor);
*/
				st_handler.n_initial_flag = YES;
				ml_init_step = 15;
			}
			break;

		case 15:
			st_init.n_initial_error = FALSE;

			if(st_handler.n_motor_init_check == YES)
			{			
				// 모터 초기화를 할것인지 말것인지 결정한다. 2K4/11/16/ViboX
				if(st_init.n_manual == NO)
				{
					ml_init_step = 100;
				}
				else if(st_init.n_manual == YES)
				{
					ml_init_step = 200;
				}
			}
			else if (st_handler.n_motor_init_check == NO)
			{
				ml_init_step = 200;
			}
			break;

		case 20 :
			// **********************************************************************
			// 스위치 검사 동작 제어 쓰레드 생성한다                                 
			// **********************************************************************
			///////////////////////////////////////// Main Thread //////////////////////////////
/*			m_thread[1]=AfxBeginThread(OnThread_Tower_Lamp_Check, this);	// THREAD_PRIORITY_ABOVE_NORMAL) ;
			if (m_thread[1] != NULL) 	
 			hThrHandle[1] = m_thread[1]->m_hThread;

			m_thread[3]=AfxBeginThread(OnThread_Main_Robot, this);			// 
			if (m_thread[3] != NULL) 	
				hThrHandle[3] = m_thread[3]->m_hThread;

			m_thread[4]=AfxBeginThread(OnThread_Unloader_Elevator_Left, this);			// 
			if (m_thread[4] != NULL) 	
				hThrHandle[4] = m_thread[4]->m_hThread;

 			m_thread[5]=AfxBeginThread(OnThread_Unloader_Elevator_Right, this);			// 
			if (m_thread[5] != NULL) 	
				hThrHandle[5] = m_thread[5]->m_hThread;

			m_thread[6]=AfxBeginThread(OnThread_Unloader_Transfer_Left, this);			// 
			if (m_thread[6] != NULL) 	
				hThrHandle[6] = m_thread[6]->m_hThread;

			m_thread[7]=AfxBeginThread(OnThread_Unloader_Transfer_Right, this);			// 
			if (m_thread[7] != NULL) 	
				hThrHandle[7] = m_thread[7]->m_hThread;

			m_thread[8]=AfxBeginThread(OnThread_Test_Site, this);			// 
			if (m_thread[8] != NULL) 	
				hThrHandle[8] = m_thread[8]->m_hThread;

			m_thread[9]=AfxBeginThread(OnThread_Test_Dump, this);			// 
			if (m_thread[9] != NULL) 	
				hThrHandle[9] = m_thread[9]->m_hThread;

			m_thread[10]=AfxBeginThread(OnThread_Index_Dump, this);			// 
			if (m_thread[10] != NULL) 	
				hThrHandle[10] = m_thread[10]->m_hThread;

			m_thread[11]=AfxBeginThread(OnThread_Index_Table, this);			// 
			if (m_thread[11] != NULL) 	
				hThrHandle[11] = m_thread[11]->m_hThread;

//			m_thread[12]=AfxBeginThread(OnThread_Conveyor, this);			// 
//			if (m_thread[12] != NULL) 	
//				hThrHandle[12] = m_thread[12]->m_hThread;

			m_thread[13]=AfxBeginThread(OnThread_Module, this);			// 
			if (m_thread[13] != NULL) 	
				hThrHandle[13] = m_thread[13]->m_hThread;*/
			ml_init_step = 30;
			break;
			/*
		case 30 :// 초기화 완료 체크
			if (st_work.n_init_sorter_robot == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(3);
				
				st_work.n_init_sorter_robot = COMPLETE;
			}

			if (st_work.n_init_uld_el_left == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(4);
				
				st_work.n_init_uld_el_left = COMPLETE;
			}

			if (st_work.n_init_uld_el_right == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(5);
				
				st_work.n_init_uld_el_right = COMPLETE;
			}

			if (st_work.n_init_uld_tf_left == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(6);
				
				st_work.n_init_uld_tf_left = COMPLETE;
			}

			if (st_work.n_init_uld_tf_right == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(7);
				
				st_work.n_init_uld_tf_right = COMPLETE;
			}

			if (st_work.n_init_test_site == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(8);
				
				st_work.n_init_test_site = COMPLETE;
			}

			if (st_work.n_init_test_dump == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(9);
				
				st_work.n_init_test_dump = COMPLETE;
			}

			if (st_work.n_init_index_dump == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(10);
				
				st_work.n_init_index_dump = COMPLETE;
			}

			if (st_work.n_init_index_table == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(11);
				
				st_work.n_init_index_table = COMPLETE;
			}

			if (st_work.n_init_conveyor == READY)
			{
				mn_pos_step += 5;
				
				OnInitial_Change_Status(12);
				
				st_work.n_init_conveyor = COMPLETE;
			}

			if(st_work.n_init_sorter_robot	== COMPLETE	&&
			   st_work.n_init_uld_el_left	== COMPLETE	&& st_work.n_init_uld_el_right	== COMPLETE &&
			   st_work.n_init_uld_tf_left	== COMPLETE	&& st_work.n_init_uld_tf_right	== COMPLETE &&
			   st_work.n_init_test_site		== COMPLETE	&& st_work.n_init_test_dump		== COMPLETE &&
			   st_work.n_init_index_dump	== COMPLETE	&& st_work.n_init_index_table	== COMPLETE &&
			   st_work.n_init_conveyor		== COMPLETE)
			{
				st_handler.n_initial_flag = YES;
				ml_init_step = 100;
			}	
			break;
			*/

		case 100 :
			Ret = COMI.HomeCheck_Mot(0, 1, MOT_TIMEOUT);
			
			if(Ret == CTLBD_RET_GOOD)				// 정상적으로 Home Check가 끝났을 경우.
			{
				OnInitial_Change_Status(1);
				ml_init_step = 200;
			}
			else if(Ret == CTLBD_RET_ERROR)		// Home Check에 실패 했을 경우.
			{
				mn_init_flag = RET_ERROR;
			}
			else if(Ret == CTLBD_RET_SAFETY)
			{
				mn_init_flag = RET_ERROR;
			}
			break;

		case 200:
			mn_pos_step = 100;

			mn_motor_init_count = 0;
			ml_init_step = 0; 
			mn_init_flag = RET_GOOD;
			break;
	}

		if (mn_pos_step < 51)
		{
			m_ctrlProgress.SetPos(mn_pos_step);
		}
		else if (mn_pos_step > 49)
		{
			if (mn_pos_step > 100)	mn_pos_step = 100;
			m_ctrlProgress.SetPos(50);
			m_ctrlProgress1.SetPos(mn_pos_step);
		}
		
		//초기화중 문제가 생겼다~
		if (st_init.n_initial_error == TRUE)
		{
			mn_pos_step = 0;
			m_ctrlProgress.SetPos(0);
			m_ctrlProgress1.SetPos(50);
			
			ml_init_step = 0;			// 초기화 진행 스텝 정보 초기화 
			mn_init_flag = RET_ERROR;	// 초기화 작업 에러 플래그 설정 
		}

	return mn_init_flag;
}

void CScreen_Initial::OnBtnInitSkip() 
{
	KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 
	// **************************************************************************
	// 화면 전환 가능하도록 플래그 설정한다                                      
	// -> st_handler.n_menu_lock : 메뉴 사용 가능 상태 플래그                    
	// -> st_handler.n_initial   : 초기화 작업 완료 플래그                       
	// **************************************************************************
	if (st_handler.n_initial != TRUE)
	{
		st_handler.n_initial = TRUE;
	}
	
	if (st_handler.n_menu_lock != FALSE)
	{
		st_handler.n_menu_lock = FALSE;
	}
	// **************************************************************************
	
	if (st_work.n_run_status != dSTOP)  // STOP 상태 
	{
		Func.OnSet_IO_Port_Stop();		// 장비 STOP 시 플래그 설정 및 I/O 출력 함수
	}
}
/*
void CScreen_Initial::OnBtnInitRetry() 
{
	m_ctrlProgress.SetPos(0);
	m_ctrlProgress1.SetPos(50);
	
	return;
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수
	
	CDialog_Select select_dlg;
	
	st_other.str_confirm_msg = _T("Initialization work progress");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		if (st_handler.n_initial != FALSE)  st_handler.n_initial = FALSE;  // 초기화 작업 완료 여부 초기화 
		
		st_init.n_manual = YES;
		mn_pos_step = 0;					// 프로그레서 위치 정보 초기화
		
		OnInitial_Controls_Enable(FALSE);	// 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수
		
		OnInitial_Change_Status(0);			// 초기화 화면 이니셜 진행 버튼 상태 변경 함수
		
		OnInitial_Step_Clear();				// 초기화 작업 : Step 데이터 저장 변수 초기화 함수
		
		OnInitial_Initial_Ready();			// 초기화 준비 작업 설정 함수
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}
*/
BOOL CScreen_Initial::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	if(mp_init_font != NULL)
	{
		delete mp_init_font;
		mp_init_font = NULL;
	}

	st_handler.cwnd_initial = NULL;	
	KillTimer(TMR_INIT);
	/* ************************************************************************** */

	return CFormView::DestroyWindow();
}

void CScreen_Initial::OnBtnInitRetry() 
{
	// TODO: Add your control notification handler code here
	m_ctrlProgress.SetPos(0);
	m_ctrlProgress1.SetPos(50);
	
	return;
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수
	
	CDialog_Select select_dlg;
	
	st_other.str_confirm_msg = _T("Initialization work progress");
	
//	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		if (st_handler.n_initial != FALSE)  st_handler.n_initial = FALSE;  // 초기화 작업 완료 여부 초기화 
		
		st_init.n_manual = YES;
		mn_pos_step = 0;					// 프로그레서 위치 정보 초기화
		
		OnInitial_Controls_Enable(FALSE);	// 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수
		
		OnInitial_Change_Status(0);			// 초기화 화면 이니셜 진행 버튼 상태 변경 함수
		
		OnInitial_Step_Clear();				// 초기화 작업 : Step 데이터 저장 변수 초기화 함수
		
		OnInitial_Initial_Ready();			// 초기화 준비 작업 설정 함수
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

LRESULT CScreen_Initial::OnWorkMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case INITIAL_INDEX_DUMP:
			OnInitial_IndexDump_Check();
			break;

		case INITIAL_TEST_DUMP:
			OnInitial_TestDump_Check();
			break;

		case INITIAL_INDEX_TABLE:
			OnInitial_Index_Table_Check();
			break;
	}

	return 0;
}

void CScreen_Initial::OnInitial_IndexDump_Check()
{
	CDialog_Select select_dlg;
	int n_response;

	st_handler.n_chk_dump_conti_dlg = YES;
	
	st_other.str_confirm_msg = _T("Index Dump Robot Safety Check");	// Lot End를 하시겠습니까?
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)					// 자재가 없다.
	{
		st_handler.n_chk_dump_conti = 2;
		st_handler.n_chk_dump_conti_dlg = NO;
	}
	else if (n_response == IDCANCEL)		// 자재가 더 있다.
	{
		st_handler.n_chk_dump_conti = 1;
	}
}

void CScreen_Initial::OnInitial_TestDump_Check()
{
	CDialog_Select select_dlg;
	int n_response;

	st_handler.n_chk_dump_conti_dlg = YES;
	
	st_other.str_confirm_msg = _T("Test Dump Robot Safety Check");	// Lot End를 하시겠습니까?
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)					// 자재가 없다.
	{
		st_handler.n_chk_dump_conti = 2;
		st_handler.n_chk_dump_conti_dlg = NO;
	}
	else if (n_response == IDCANCEL)		// 자재가 더 있다.
	{
		st_handler.n_chk_dump_conti = 1;
	}
}

void CScreen_Initial::OnInitial_Index_Table_Check()
{
	CDialog_Select select_dlg;
	int n_response;

	st_handler.n_chk_index_table_conti_dlg = YES;
	
	st_other.str_confirm_msg = _T("Index Table에 배출부에 모듈이 감지되었습니다. 제거해 주세요.");	// Lot End를 하시겠습니까?
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)					// 자재가 없다.
	{
		st_handler.n_chk_index_table_conti = 2;
		st_handler.n_chk_index_table_conti_dlg = NO;
	}
}
