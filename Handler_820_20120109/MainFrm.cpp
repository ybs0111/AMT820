// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "HANDLER.h"

#include "MainFrm.h"

/* ****************************************************************************** */
/* 대화 상자 및 외부 클래스 추가                                                  */
/* ****************************************************************************** */
#include "MenuHead.h"		// 화면에 대한 헤더 정의된 클래스 추가 
#include "Dialog_Menu.h"	// 메뉴 선택 대화 상자 클래스 추가 

#include "Dialog_About.h"	// 프로그램 About 대화 상자 클래스 추가 
#include "Dialog_Exit.h"	// 응용 프로그램 종료 화면 클래스 추가 
#include "Dialog_Select.h"
#include "Dialog_Pass_Check.h"
#include "Dialog_Message.h" // 에러 출력 대화 상자 클래스 추가 
#include "DataAPI.h"
#include "Public_Function.h"
#include "Screen_List_Error.h"
#include "Screen_IO_Map.h"
#include "Screen_Basic.h"
#include "Screen_Motor.h"
#include "Screen_Motor_Speed.h"
//#include "Screen_Work_Info.h"
#include "Screen_Pgm_Info.h"
#include "ComizoaPublic.h"
#include "Cmmsdk.h"
#include "Comidas.h"
#include "ComizoaPublic.h"
#include "KeyboardDll.h"
#include "FastechPublic_IO.h"
#include "PPTooltip.h"
//#include "ServerSocket.h"
#include "IO_Manager.h"
#include "COMI_Manager.h"
#include "CtlBd_Library.h"
#include "SrcPart/APartShortkey.h"
#include "SrcPart/APartDatabase.h"
#include "SrcPart//PartFunction.h"
#include "Screen_Set_Maintenance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************************* */
/* 전역 구조체 선언 부분                                                           */
/* ******************************************************************************* */
st_handler_param			st_handler;			// 화면 동작 관련 기본 데이터 저장 구조체 변수 선언
st_time_param				st_time;
st_message_param			st_msg;
st_initial_param			st_init;			// 장비 초기화용 구조체 선언
st_other_param				st_other;			// 기타 정보 저장 구조체 선언
st_alarm_param				alarm;				// 알람 데이터 저장 구조체 선언
st_dsp_alarm_param			st_alarm;			// 알람 데이터 저장 구조체 선언
st_work_param				st_work;			// 장비 동작 제어 플래그 정보 저장 구조체 선언 
st_basic_param				st_basic;			// 베이직 화면 환경 설정 정보 저장 구조체 선언
st_lamp_param				st_lamp;			// Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언
st_home_param				st_home[MAX_MOTOR];	// Home Check 시 사용될 구조체 선언
//st_io_param					st_io;				// I/O 상태 데이터 저장 구조체 변수 선언
st_path_param				st_path;			// 파일 경로명 저장 구조체 변수 선언
st_serial_param				st_serial;			// Serial 사용 구조체 변수 선언
st_io_wait_param			st_wait;			// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언
st_sync_param				st_sync;			// 각 Thread간 동기화 구조체 선언
st_traybuffer_param			st_traybuffer;		// TRAY & BUFFER 설정 정보 저장 구조체 선언
st_modulemap_size			st_modulemap;
st_admin_param				st_admin;
st_ctlbd_param				st_ctlbd;
//st_motor_param				st_motor[MAX_MOTOR];
st_linearmotion_param		st_linearmot[8];
st_coordmotion_param		st_coordmot[16];  
st_client_param				st_client[10];
st_server_param				st_server[10];

st_time_database			st_timedb;
st_alarm_database			st_alarmdb;
st_ground_database			st_grounddb;

st_buffer_database			st_bufferdb;

st_part_motor_info			st_motor_info;
st_part_info				st_part;
st_buffer_tray_param		st_buffer_tray[MAX_SITE_INFO];

st_boat_database			st_boatdb;
st_scrap_database			st_scrapdb;
//st_set_data					st_setdata;
//st_scrap_data				st_scrapdata;
st_motor_param				st_motor[M_MOTOR_COUNT];
tagTEST_SITE_INFO			st_test_site_info[THREAD_MAX_SITE];
tagRECIPE_INFO				st_recipe;
tagALL_TRAY_INFO			st_tray_info[THREAD_MAX_SITE];
tag_BUFFER_INFO				st_buffer_info[THREAD_MAX_SITE];
tag_PICKER_INFO				st_picker[THREAD_MAX_SITE];
st_carrier_buffer_info_param	st_carrier_buff_info[MAX_SHIFT_DATA_NUM];
st_variable_param			st_var;
struct st_vision_camera		st_vision;

CPublic_Function			Func;
struct st_serial_info		rs_232;
extern CHANDLERApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MAIN, OnMain)
	ON_COMMAND(ID_BASIC, OnBasic)
	ON_COMMAND(ID_SETTING, OnSetting)
	ON_COMMAND(ID_MOTOR, OnMotor)
	ON_COMMAND(ID_LIST, OnList)
	ON_COMMAND(ID_IO, OnIo)
	ON_COMMAND(ID_LEVEL, OnLevel)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_ALARM, OnAlarm)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_ADMIN, OnAdmin)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_LOCK, OnLock)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_LIST_OPERATION, OnListOperation)
	ON_COMMAND(ID_LIST_WORK, OnListWork)
	ON_COMMAND(ID_LIST_ALARM, OnListAlarm)
	ON_COMMAND(ID_LIST_STEP, OnListStep)
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_PGM_INFO, OnPgmInfo)
	ON_COMMAND(ID_MOTOR_SPEED, OnMotorSpeed)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)		// 텍스트 툴바 드롭 다운 제어를 위한 사용자 정의 메시지 선언 
	ON_MESSAGE(WM_MAINFRAME_WORK, OnMainframe_Work)
	ON_MESSAGE(WM_FORM_CHANGE, OnViewChangeMode)					// Post Message에 대한 화면 전환 사용자 사용자 정의 메시지 선언 
//	ON_MESSAGE(WM_CLIENT_MSG_1, OnCommand_Client_1)							// Network관련된 작업을 담당한다.
//	ON_MESSAGE(WM_SERVER_MSG_1, OnCommand_Server_1)							// Network관련된 작업을 담당한다.
	ON_MESSAGE(WM_COMM_DATA, OnCommunication)						// RS-232C 시리얼 포트 제어 메시지
	ON_MESSAGE(WM_COMM_EVENT, OnCommunicationEvent)					// RS-232C 시리얼 포트 이벤트 설정 메시지
	ON_MESSAGE(WM_DATA_SEND, OnDataSend)							// RS-232C 시리얼 포트 통한 데이터 송신 메시지
	ON_MESSAGE(WM_SERIAL_PORT, OnSerialPort)
	ON_MESSAGE(WM_DATA_CHANGE, OnDataChange)
	ON_MESSAGE(WM_MAIN_EVENT, OnMainMessageEvent)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	int i;

	for(i=0; i<10; i++)
	{
		m_p_client[i] = NULL;
		m_p_server[i] = NULL;

		st_client[i].n_connect	= NO;
		st_server[i].n_connect	= NO;
	}

	st_handler.n_mot_board_initial	= FALSE;
	st_handler.n_load_state			= FALSE;
	st_handler.n_lot_flag			= CTL_LOTEND_FINISH;
	st_handler.n_ad_board_create	= NO;
	st_handler.n_initial_flag		= NO;

	st_handler.mn_virtual_mode = 1;
	COMI.mn_simulation_mode = 1;

	OnMain_Var_Default_Set();				// 메인 프레임 클래스 변수 초기화 함수

	Func.PublicVariableDataInitialize();	// 장비 동작 제어 플래그 초기화 함수
	Func.On_IOFlagReset();					// 동작시 사용하는 IO 관련 Flag 초기화 함수 2K4/12/10/ViboX
	mcls_frm_alarm.On_Alarm_Info_Load();	// 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수
	
	for(i=0; i<MAX_PORT; i++)
	{
		OnMain_Port_Create(i);
	}
}

CMainFrame::~CMainFrame()
{
	if(mp_alarm_dlg != NULL) delete mp_alarm_dlg;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//  	int nRet;
  	
	st_handler.hWnd = GetSafeHwnd() ;  // 메인 프레임에 대한 핸들 정보 설정

	// 강제 해상도 변경.
// 	if (::GetSystemMetrics(SM_CXSCREEN) != 1280 && ::GetSystemMetrics(SM_CYSCREEN ) != 1024)
// 	{
// 		DEVMODE mode ;
// 		mode.dmBitsPerPel = 32;				// 32비트 칼라로 변경
// 		mode.dmPelsWidth  = 1280;			
// 		mode.dmPelsHeight = 1024;
// 		mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
// 		ChangeDisplaySettings(&mode, CDS_FULLSCREEN);
// 	}
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// =============================================================================
    // Text 툴바 Icon 사이즈 설정                                                 //
    // =============================================================================
//	SIZE size1={32, 32}, size2={62, 49};
//	m_wndToolBar.SetSizes(size2, size1);
	// =============================================================================
	m_wndToolBar.SetBorders(1, 1, 1, 1);

	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_wndToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
								   IDB_TOOLBAR_DRAW,
		                           IDB_TOOLBAR_DRAW_HOT,
								   IDB_TOOLBAR_DRAW_DISABLED);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);

	// =============================================================================
    // 텍스트 툴바 스타일 설정 (드롭 다운 메뉴로 설정)                            //
    // =============================================================================
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
///*
//	DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_MOTOR));
//    dwStyle |= TBSTYLE_DROPDOWN;
//    m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_MOTOR), dwStyle);

 //   dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_LIST));
//    dwStyle |= TBSTYLE_DROPDOWN;
//    m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_LIST), dwStyle);
	// =============================================================================

	// =============================================================================
    // 타이틀 정보 출력 Initial Dialog bar 생성                                   //
    // =============================================================================
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my init bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_title = &m_wndTitleBar;
	// =============================================================================

	// =============================================================================
    // 리스트 정보 출력 Initial Dialog bar 생성                                   //
	// -> 상태 바 밑에 위치하도록 하려면 이 부분을 상태 바 생성 부분 위로 올린다. //
    // =============================================================================
	if (!m_wndListBar.Create(this, IDD_DIALOG_LIST, CBRS_ALIGN_BOTTOM, IDD_DIALOG_LIST))
	{
		TRACE0("Failed to create my information bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_list = &m_wndListBar;
	/* ************************************************************************** */
	
	// **************************************************************************
    // 모터 보드 및 I/O 보드 초기화 후 출력 포트 클리어 작업 진행한다        
    // **************************************************************************
	g_ioMgr.IO_Board_Initialize();				// IO 보드 라이브러리 초기화 함수
	Func.OnSet_IO_Out_Port_Clear();			// I/O 보드의 출력 포트 초기화 함수
	g_comiMgr.SetMotBoardInit( CTL_Lib.Initialize_motor_board(1, st_path.mstr_motor) );	// MOTOR 보드 초기화 여부 검사한다
	if( st_handler.mn_virtual_mode )
		g_comiMgr.SetMotBoardInit( BD_GOOD );
	
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}
	
	if (st_handler.cwnd_title != NULL)	
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, dSTOP); 
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, 0);
	}
	




	OnMain_Thread_Creating();
	// **************************************************************************

	/* ************************************************************************** */
    /* 응용 프로그램에 대한 타이틀 정보 출력                                      */
    /* ************************************************************************** */
	Init_ToolTip();

	SetWindowText("AMT820 : Ver.1.0.0");  // 타이틀 정보 출력 

	CenterWindow();  // 현재 응용 프로그램을 정 중앙에 배치

	/* ************************************************************************** */
    /* 초기화 진행 화면으로 전환하여 장비 초기화 작업 진행한다                    */
    /* ************************************************************************** */
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);  // 초기화 진행 화면 전환 
	/* ************************************************************************** */
	
	SetTimer(TMR_MAIN_REFRESH, 500, NULL);  // 리스트 파일 생성 타이머
	SetTimer(TMR_FILE_CREATE, 1000, NULL);  // 리스트 파일 생성 타이머

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU;
	cs.style &= ~FWS_ADDTOTITLE;								// 화면 타이틀 정보 제어 가능하도록 스타일 설정 
	cs.style &= ~WS_MAXIMIZEBOX;

	cs.lpszClass=((CHANDLERApp*)AfxGetApp())->GetClassName();	// 현재 응용 프로그램에 대한 클래스 이름을 얻는다(Need for Single Instance)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_wndToolBar.m_hWnd)
	{
		if(m_wndToolBar.IsFloating())  // 툴바가 플로팅 된 경우에 다시 화면 상단에 정렬 
		{
			m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
			EnableDocking(CBRS_ALIGN_TOP);
			DockControlBar(&m_wndToolBar);
		}
	}

	m_p_tooltip.RelayEvent(pMsg);
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnMain() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Main)))   return;

	OnSwitchToForm(IDW_SCREEN_MAIN);
}

void CMainFrame::OnBasic() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Basic)))   return;
	OnSwitchToForm(IDW_SCREEN_BASIC);
}

void CMainFrame::OnSetting() 
{
	CDialog_Menu menu_dlg;

	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.n_level_teach !=  TRUE)
	{
		if (st_handler.n_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.n_level_teach !=  TRUE)
			{
				if (st_handler.n_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))   return;
	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);	

}




void CMainFrame::OnMotor() 
{
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.n_level_teach !=  TRUE)
	{
		if (st_handler.n_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.n_level_teach !=  TRUE)
			{
				if (st_handler.n_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor)))   return;
	OnSwitchToForm(IDW_SCREEN_MOTOR);
}


void CMainFrame::OnList() 
{
	CDialog_Menu menu_dlg;

	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	st_other.str_parent_menu = "List Screen";  // 현재 선택된 주 메뉴 정보 저장
	int nResponse = menu_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnListOperation() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Operation)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_OPERATION);
}

void CMainFrame::OnListWork() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Work)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_WORK);
}

void CMainFrame::OnListAlarm() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Alarm)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_ALARM);
}

void CMainFrame::OnListStep() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Step)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_STEP);
}

void CMainFrame::OnIo() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.n_level_teach !=  TRUE)
	{
		if (st_handler.n_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.n_level_teach !=  TRUE)
			{
				if (st_handler.n_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_IO_Map)))   return;
	OnSwitchToForm(IDW_SCREEN_IO_MAP);
}

void CMainFrame::OnLevel() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수 선언 
	
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 

	st_other.str_pass_level = "LevelChange";
	
	CDialog_Pass_Check pass_dlg;

	mn_response = pass_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

	}
	else if (mn_response == IDCANCEL)
	{

	}
}

void CMainFrame::OnSave() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	
	CDialog_Select  select_dlg;
	
	st_other.str_confirm_msg = _T("Main : All Setting Data Save?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_handler.n_menu_lock = TRUE;
		
		OnExitData_Saving();					// 화면에 설정된 모든 데이터 저장 함수
		
		st_handler.n_menu_lock = FALSE;
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CMainFrame::OnAlarm() 
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 

	/* ************************************************************************** */
    /* 모달리스 알람 화면 출력한다                                                */
	/* -> 현재 알람 화면 출력된 경우에는 해당 화면에 포커스만 전달                */
    /* ************************************************************************** */
	if (mp_alarm_dlg != NULL)  mp_alarm_dlg->SetFocus();
	else
	{
         mp_alarm_dlg = new CDialog_Alarm;
		 mp_alarm_dlg->Create(this);
		 mp_alarm_dlg->ShowWindow(SW_SHOW);
	}
	/* ************************************************************************** */
}

void CMainFrame::OnAbout() 
{
	CDialog_About about_dlg;

	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	/* ************************************************************************** */

	int nResponse = about_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnAdmin() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnReset() 
{
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수 선언 
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	if(st_handler.n_lot_flag == CTL_LOTSTART) return;
	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
	if (st_handler.n_level_teach !=  TRUE)
	{
		OnLevel();

		if (st_handler.n_level_teach !=  TRUE) return;
	}
	/* ************************************************************************** */

	//	CDialog_Infor info_dlg;
	CDialog_Select selct_dlg;
	CDialog_Message msg_dlg;

	/* ************************************************************************** */
	/* 모터 보드 초기화 여부 먼저 검사한다.                                       */
	/* ************************************************************************** */
	if (st_handler.n_mot_board_initial != TRUE)  
	{
		st_other.str_fallacy_msg = _T("Do to initialize first motion board.");
		
		n_response = msg_dlg.DoModal();
		if (n_response == IDOK) 
			return;
	}
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* 현재 장비 상태가 STOP 상태인지 검사                                        */
	/* ************************************************************************** */
	if (st_work.mn_run_status != dSTOP)
	{
		st_other.str_fallacy_msg = _T("Handler is active Stop first.!");
		n_response = msg_dlg.DoModal();
		if (n_response == IDOK)  return;
	}
	/* ************************************************************************** */

	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);  // 초기화 진행 화면 전환 
}

void CMainFrame::OnLock() 
{
	int n_response;  // 대화 상자 리턴 플래그

	CDialog_Pass_Check  pass_chk_dlg;

	/* ************************************************************************** */
	/* 메뉴 사용 불가능한 경우에 대한 조건 검사한다                               */
	/* -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                       */
	/* ************************************************************************** */
//	if (st_handler.n_level_teach !=  TRUE)
//	{
//		if (st_handler.n_level_maint !=  TRUE)  return ;
//	}
	/* ************************************************************************** */

	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;

	st_other.str_pass_level=_T("SystemLock");  // 암호 레벨 설정

	n_response = pass_chk_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // 메인 프레임에 SYSTEM LOCK 화면 출력 요청
	
		if (st_handler.cwnd_list != NULL)
		{
			st_other.str_normal_msg = "System Locking.";
			printf(st_other.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CMainFrame::OnClose() 
{
	OnMain_Port_Close();
	OnExit();
}

void CMainFrame::OnExit() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수 선언 
	
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)	return;
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴
	
	CDialog_Message msg_dlg;

	if (st_work.mn_run_status != dSTOP)
	{
		st_other.str_fallacy_msg = _T("Now Machine is Running...");
		mn_response = msg_dlg.DoModal();
		if (mn_response == IDOK)  return ;
	} 

	CDialog_Exit  dlg;
	
	mn_response = dlg.DoModal();
	
	if (mn_response == IDOK)     
	{
		All_Stop = 1 ;				// 쓰레드 상태 정보 저장 변수 초기화
		
		OnMain_Thread_Destroy();	// 쓰레드 삭제 함수
		OnExitData_Saving();// 화면에 설정된 모든 데이터 저장 함수
		
		st_handler.b_program_exit = true;

		if(st_handler.n_alarm_screen != FALSE)
		{
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
		}
		m_pNewActiveView->DestroyWindow();
		m_pNewActiveView = NULL;

		if(st_handler.n_mot_board_initial)
		{
//			COMI.Close_MotBoard();
		}

		CFrameWnd::OnClose();
	}
}

void CMainFrame::OnToolbarDropDown(NMTOOLBAR *pnmtb, LRESULT *plr)
{
	CWnd *pWnd = &m_wndToolBar;
    UINT nID = IDR_MAINFRAME;

    // load and display popup menu
    CMenu menu;
    menu.LoadMenu(nID);
	CMenu* pPopup;

    // Switch on button command id's.
    switch (pnmtb->iItem)
    {
    case ID_SETTING:
		pPopup = menu.GetSubMenu(1);
        break;
	case ID_MOTOR:
		pPopup = menu.GetSubMenu(2);
		break;
	case ID_LIST:
		pPopup = menu.GetSubMenu(3);
		break;
    default:
        return;
    }

    ASSERT(pPopup);
    
    CRect rc;
    pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
    pWnd->ClientToScreen(&rc);
    
    pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
    rc.left, rc.bottom, this, &rc);
}

/* ****************************************************************************** */
/* 메뉴에서 선택된 화면으로 전환시키는 함수                                       */
/* ****************************************************************************** */
void CMainFrame::OnSwitchToForm(int nForm)
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */

	if (st_handler.n_menu_lock != FALSE)	return;
	if (st_handler.n_system_lock != FALSE)  return;		// 현재 시스템 Lock 상태인 경우 리턴 

	OnDialog_Delete();

	m_pOldActiveView = GetActiveView();			// 현재 화면에 출력된 뷰 정보 설정 (save old view)

	m_pNewActiveView = (CView*)GetDlgItem(nForm);  // 새로 출력할 뷰 정보 설정 (get new view)
	
	if(m_pNewActiveView == NULL)							// 새로 출력할 뷰가 아직 출력되지 않았는지 검사 (if it hasn't been created yet)
	{
		switch(nForm)			// 뷰에 설정된 ID 정보 비교 (these IDs are the dialog IDs of the view but can use anything)
		{
			case IDW_SCREEN_MAIN:				// 메인 화면 
				m_pNewActiveView = (CView*)new CScreen_Main;
				break;
			case IDW_SCREEN_LOCK:				// System Lock 화면
				m_pNewActiveView = (CView*)new CScreen_Lock;
				break;
			case IDW_SCREEN_INIT:				// 초기화 화면 
				m_pNewActiveView = (CView*)new CScreen_Initial;
				break;
			case IDW_SCREEN_BASIC:				// Basic 화면
				m_pNewActiveView = (CView*)new CScreen_Basic;
				break;
			case IDW_SCREEN_SET_MAINTENANCE:	// 메인트 화면 
				m_pNewActiveView = (CView*)new CScreen_Set_Maintenance;
				break;
			case IDW_SCREEN_PGM_INFO:
				m_pNewActiveView = (CView*)new CScreen_Pgm_Info;
				break;
			case IDW_SCREEN_LIST_OPERATION:		// 오퍼레이션 정보 출력 화면 
				m_pNewActiveView = (CView*)new CScreen_List_Operation;
				break;
			case IDW_SCREEN_LIST_WORK:			// 테스트 결과 리스트 화면 
				m_pNewActiveView = (CView*)new CScreen_List_Work;
				break;
			case IDW_SCREEN_LIST_ALARM:			// 알람 리스트 화면 
				m_pNewActiveView = (CView*)new CScreen_List_Alarm;
				break;
			case IDW_SCREEN_LIST_STEP:			// 쓰레드 스텝 정보 출력 화면 
				m_pNewActiveView = (CView*)new CScreen_List_Step;
				break;
			case IDW_SCREEN_LIST_ERROR:
				m_pNewActiveView = (CView*)new CScreen_List_Error;
				break;
			case IDW_SCREEN_ADMINISTRATOR:		// ADMINISTRATOR 출력 화면 
				m_pNewActiveView = (CView*)new CScreen_Administrator;
				break;
			case IDW_SCREEN_MOTOR:
				m_pNewActiveView = (CView*)new CScreen_Motor;
				break;
			case IDW_SCREEN_MOTOR_SPEED:
				m_pNewActiveView = (CView*)new CScreen_Motor_Speed;
				break;
			case IDW_SCREEN_SET_INTERFACE:		// Interface 관련 설정 화면 
				break;
			case IDW_SCREEN_IO_MAP:
				m_pNewActiveView = (CView*)new CScreen_IO_Map;
				break;
		}

		CCreateContext context;  // 새로운 뷰에 도큐먼트 Attach ( attach the document to the new view)
		context.m_pCurrentDoc = m_pOldActiveView->GetDocument();

		m_pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nForm, &context);
		m_pNewActiveView->OnInitialUpdate();  		
	}

	SetActiveView(m_pNewActiveView);			// 새로 출력된 뷰를 액티브 뷰로 설정 (change the active view)
	m_pNewActiveView->ShowWindow(SW_SHOW);	// 새로운 뷰를 출력 (show the new window)

	m_pOldActiveView->ShowWindow(SW_HIDE);	// 기존에 출력된 뷰를 감춘다 (hide the old)
	m_pOldActiveView->DestroyWindow();		// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
	m_pOldActiveView = NULL;					// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화 

	::SetWindowLong(m_pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
	RecalcLayout();							// 프레임을 조율한다 (adjust frame)
	
	delete	m_pOldActiveView;					// 기존에 출력된 뷰 정보를 제거한다 (kill old view)
	
	st_handler.n_menu_num = nForm;

	/* ************************************************************************** */
	/* 현재 화면 정보 타이틀바에 출력한다.                                        */
	/* ************************************************************************** */
	if (st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nForm);
	/* ************************************************************************** */
}

/* ****************************************************************************** */
/* 사용자 정의 메시지에 따른 뷰 화면 전환 함수                                    */
/* ****************************************************************************** */
LRESULT CMainFrame::OnViewChangeMode(WPARAM wParam,LPARAM lParam)
{
	int mn_chg_num = wParam;  // 뷰 화면에 대한 ID 값 설정 

	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
	/* -> 알람 화면을 제외한 화면에 대해서는 메뉴 사용 가능 여부 검사             */
    /* ************************************************************************** */
	if (mn_chg_num != 9)  
	{
		if (st_handler.n_menu_lock != FALSE)  return 0;
		if (st_handler.n_system_lock != FALSE)  return 0;  // 현재 시스템 Lock 상태인 경우 리턴 
	}
	/* ************************************************************************** */

	switch(mn_chg_num)
	{
		case 1: 
			if (lParam==1)  OnMain();					// 메인 화면 전환 
			else if (lParam==2)							// System Locking 화면 전환 
			{
				if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Lock)))   return 0;
				OnSwitchToForm(IDW_SCREEN_LOCK) ;
			}
			else if (lParam==3)							// 초기화 화면 전환 
			{
				if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Initial)))   return 0;
				OnSwitchToForm(IDW_SCREEN_INIT) ;
			}
			break;
		case 2: 
			OnBasic();									// 베이직 화면 전환 
			break;
		case 3:
			break;
		case 4 :
//			OnWait();
			break;
		case 5 : 
			if(lParam == 1) OnMotor();
			else if (lParam==2)	 OnMotorSpeed();			// 모터 속도 화면 전환 
			break;
		case 6 :
			if (lParam==1)       OnListOperation();		// Operation 결과 리스트 화면 전환 
			else if (lParam==2)  OnListWork();			// 작업 결과 리스트 화면 전환 
			else if (lParam==3)  OnListAlarm();			// 알람 리스트 화면 전환 
			else if (lParam==4)  OnListStep();			// 쓰레드 정보 출력 화면 전환 
			else if (lParam ==5) OnListError();
			
			break;
		case 7 : 
			if (lParam==1)       OnAlarm();				// 알람 화면 전환 
			else if (lParam==2)  OnAlarm_Destroy();		// 알람 화면 종료 함수
			break;
		case 8 : 
			if (lParam==1)       OnAdmin();				// Administrator 화면 전환 
			break;
		default:
			break;
	}

	return 0 ;
}
/* ****************************************************************************** */

int CMainFrame::OnMenu_Change_Checking()
{
	int Ret_Val;

	Ret_Val = TRUE;  // 초기 리턴 값 설정 

	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)	Ret_Val = FALSE;
	if (st_handler.n_system_lock != FALSE)  Ret_Val = FALSE;  // 현재 시스템 Lock 상태인 경우 리턴 

	/* ************************************************************************** */
	/* 현재 장비 상태가 STOP 상태인지 검사                                        */
	/* ************************************************************************** */
	if (st_work.mn_run_status != dSTOP && st_work.mn_run_status != dLOCK)
	{
		st_other.str_op_msg = _T("[장비 동작 중] 먼저 STOP 시킨 후에 사용하세요");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, 7); 

		Ret_Val = FALSE;
	}
	/* ************************************************************************** */
	
	return Ret_Val;
}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	//타이틀바를 더블클릭 했을때 작아지지 말라고 리턴시킨다.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnMain_Var_Default_Set()
{
	int i;

	for (i = 0; i < 8; i++)
	{
		st_handler.n_safety[i] = YES;
	}

	mp_alarm_dlg = NULL;
	st_handler.str_last_alarm = "";
	alarm.mn_emo_set = NO;

	st_work.b_load_key_flag = false;
//	st_handler.n_lot_flag	= FALSE;
	st_handler.n_load_state = false;

	OnMain_Path_Set();

	mcls_m_basic.OnBasic_Data_Load();
	mcls_m_basic.On_Teach_Data_Load();
	mcls_m_basic.OnMaintenance_Data_Load();
	mcls_m_basic.OnWaitTime_Data_Load();
	mcls_m_basic.OnMotorSpeed_Set_Data_Load();
	mcls_m_basic.OnInterface_Data_Load();

	st_handler.n_allhomecheck = CTL_NO;		// 전체 홈체크 모드가 아니다.

	st_handler.f_ad_resol	= (float)10 / (float)2048;
	st_handler.f_ad_device	= (float)10 / (float) 4096;

	st_handler.m_bAlarmMcPowerOff = false;


/*
	COleDateTime	oledate[2];
	CTime			time;

	time	= CTime::GetCurrentTime();				
	
	oledate[0].SetDateTime(time.GetYear()-1, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	oledate[1].SetDateTime(0, 0, 0, 0, 0, 0);
	
	// mysql db open 
	if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		// amt db 선택 
		if(Mysql.Database_Select("AMT"))
		{	
			// amt db에서 lot_data table 중에서 (테이블, 찾을 항목, 범위, 찾을 날짜)
			 Mysql.Record_Date_Delete("DEVICE_DATA", "DATE", "<", oledate);
		}
	}
	Mysql.MySql_Close();*/
}

void CMainFrame::OnMain_Path_Set()
{
	/*CString strTmp;

	st_path.str_path_file =		_T("c:\\AMT820\\");							// 파일 생성 폴더 설정
	Func.CreateFolder(st_path.str_path_file);

//	st_path.str_basic =			_T("c:\\AMT820\\Setting\\BasicTerm.TXT");		// 환경 설정 정보 저장 파일에 대한 폴더 설정 
	st_path.str_basic =			_T("c:\\AMT820\\Setting\\");		// 환경 설정 정보 저장 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.str_basic);
	st_path.str_path_dvc =		_T("c:\\AMT820\\File\\");					// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정
	Func.CreateFolder(st_path.str_path_dvc);
	
	st_path.str_path_alarm =	_T("c:\\AMT820\\Bmp\\");						// 출력 알람 정보 존재 폴더 설정
	Func.CreateFolder(st_path.str_path_alarm);
	st_path.str_path_op =		_T("c:\\AMT820\\Operator\\");					// 오퍼레이터 정보 저장 폴더 설정
	Func.CreateFolder(st_path.str_path_op);

	st_path.str_path_back =		_T("c:\\BackUp\\");									// BACKUP 파일 생성 폴더 설정
	Func.CreateFolder(st_path.str_path_back);

	st_path.str_file_basic =	_T("c:\\AMT820\\File\\File.TXT");									// 기본 셋팅 정보 저장 파일명 설정
	st_path.str_file_wait =		_T("WaitTime");										// 대기 시간 정보 저장 파일명 설정

	st_path.str_operation =		_T("c:\\AMT820\\Log\\Operation\\");			// Operation Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.str_operation);
	st_path.str_machine   =		_T("c:\\AMT820\\Log\\Machine\\");				// Machine Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.str_machine);
	st_path.str_total     =		_T("c:\\AMT820\\Log\\Total\\");				// Total Log 파일에 대한 폴더 설정 

	Func.CreateFolder(st_path.str_total);

	st_path.str_test_result =	_T("C:\\AMT820\\Log\\TestResult\\");			// Test 결과 저장에 대한 폴더 설정
	Func.CreateFolder(st_path.str_test_result);

	st_path.str_path_result = _T("C:\\AMT820\\Log\\Data\\");
	Func.CreateFolder(st_path.str_path_result);

	st_path.str_motor_pos = _T("C:\\AMT820\\Motor\\");
	Func.CreateFolder(st_path.str_motor_pos);

	Func.CreateFolder("C:\\AMT820\\Alarm\\Monthly\\");
	Func.CreateFolder("C:\\AMT820\\Alarm\\Daily\\");

	st_path.str_io_map			= "C:\\AMT820\\Setting\\AMT_IO_MAP.xls";
	st_path.str_io_part_map		= "C:\\AMT820\\Setting\\AMT_IO_PART_MAP.xls";
	st_path.str_motor_axis_map	= "C:\\AMT820\\Setting\\AMT_MOTOR_AXIS_MAP.xls";
	st_path.str_motor_part_map	= "C:\\AMT820\\Setting\\AMT_MOTOR_PART_MAP.xls";
	st_path.str_wait_time_map	= "C:\\AMT820\\Setting\\AMT_WAITTIME_MAP.xls";*/

	CString strMainPath;
	char chr_data[100];

	:: GetPrivateProfileString("Folder_Data", "Main",		"C:\\AMT820\\", chr_data, 100, ".\\PathInfo.TXT");
	strMainPath = chr_data;
	strMainPath.TrimLeft(' ');               
	strMainPath.TrimRight(' ');

	st_path.mstr_basic =			_T(strMainPath + "Setting\\BasicTerm.TXT");			// 환경 설정 정보 저장 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_basic);

	st_path.mstr_basic_folder =		_T(strMainPath + "Setting\\");						// 환경 설정 정보 저장 파일에 대한 폴더 설정 

	st_path.mstr_path_dvc =		_T(strMainPath + "Device\\");							// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정
	Func.CreateFolder(st_path.mstr_path_dvc);

	st_path.mstr_motor =			_T(strMainPath + "Setting\\AMT820.cme2");

	st_path.mstr_file_basic =		_T("BasicTerm");									// 기본 셋팅 정보 저장 파일명 설정
	st_path.mstr_file_wait =		_T("WaitTime");										// 대기 시간 정보 저장 파일명 설정

	CString FolderPath;
	
	st_path.mstr_path_alarm =		_T(strMainPath + "bmp\\");							// 출력 알람 정보 존재 폴더 설정
	Func.CreateFolder(st_path.mstr_path_alarm);
		
	st_path.mstr_path_file =		_T(strMainPath);									// 파일 생성 폴더 설정
	st_path.mstr_path_back =		_T("D:\\BackUp\\");									// BACKUP 파일 생성 폴더 설정
	
	st_path.mstr_operation =		_T(strMainPath + "Log\\Operation\\");				// Operation Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_operation);
	
	st_path.mstr_interface =		_T(strMainPath + "Log\\Interface\\");				// Interface Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_interface);
	
	st_path.mstr_machine   =		_T(strMainPath + "Log\\Machine\\");					// Machine Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_machine);
	
	st_path.mstr_total     =		_T(strMainPath + "Log\\Total\\");					// Total Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.mstr_total);

	st_path.m_strBoardLog = 		_T(strMainPath + "Log\\Board\\");					// Total Log 파일에 대한 폴더 설정 
	Func.CreateFolder(st_path.m_strBoardLog);

	st_path.mstr_ngbuffer  =		_T(strMainPath + "Log\\NGBuffer\\");				// NGBuffer Log 파일에 대한 폴더 설정 2014.12.19 - Bredmin.
	Func.CreateFolder(st_path.mstr_ngbuffer);

	st_path.mstr_tcpip =				_T(strMainPath + "Log\\TCPIP\\");
	Func.CreateFolder(st_path.mstr_tcpip);
	
	st_path.mstr_tcpip_tc_server =		_T(strMainPath + "Log\\TCPIP_TC_SERVER\\");
	Func.CreateFolder(st_path.mstr_tcpip_tc_server);

	st_path.mstr_tcpip_his =		_T(strMainPath + "Log\\TCPIP_HIS\\");
	Func.CreateFolder(st_path.mstr_tcpip_his);

	st_path.mstr_tcpip_vision =		_T(strMainPath + "Log\\TCPIP_VISION\\");
	Func.CreateFolder(st_path.mstr_tcpip_vision);

	st_path.mstr_tcpip_bpc =		_T(strMainPath + "Log\\TCPIP_BPC\\");
	Func.CreateFolder(st_path.mstr_tcpip_bpc);

	st_path.mstr_tcpip_cim =		_T(strMainPath + "Log\\TCPIP_CIM\\");
	Func.CreateFolder(st_path.mstr_tcpip_cim);

	st_path.mstr_tcpip_rfid =		_T(strMainPath + "Log\\TCPIP_RFID\\");
	Func.CreateFolder(st_path.mstr_tcpip_rfid);

	st_path.mstr_tcpip_bp =			_T(strMainPath + "Log\\TCPIP_BP\\");
	Func.CreateFolder(st_path.mstr_tcpip_bp);

	st_path.str_lot_data_path	=	_T(strMainPath + "Lot\\");	
	Func.CreateFolder(st_path.str_lot_data_path);

	st_path.str_daily_data_path =	_T(strMainPath + "Data\\");	
	Func.CreateFolder(st_path.str_daily_data_path);

	st_path.str_daily_pass_count =	_T(strMainPath + "Log\\Hour_Pass_Count\\");
	Func.CreateFolder(st_path.str_daily_pass_count);

	Func.CreateFolder(strMainPath + "Log\\AlarmCodeDebug\\");
	Func.CreateFolder(strMainPath + "Alarm\\Daily\\");
	Func.CreateFolder("D:\\BackUp\\");

	st_path.mstr_ppid = _T("C:\\AMT820\\Setting\\AMT820\\AMT820\\XWork\\Recipe\\");
	st_path.mstr_ppid_del = _T("C:\\AMT820\\Setting\\AMT820\\AMT820\\XWork\\Recipe");

	st_path.mstr_io_map				= _T(strMainPath + "Setting\\AMT820_IO_MAP.xls");
	st_path.mstr_motor_axis_map		= _T(strMainPath + "Setting\\AMT820_MOTOR_AXIS_MAP.xls");
	st_path.mstr_motor_part_map		= _T(strMainPath + "Setting\\AMT820_MOTOR_PART_MAP.xls");
	st_path.mstr_io_part_map		= _T(strMainPath + "Setting\\AMT820_IO_PART_MAP.xls");
	st_path.mstr_wait_time_map		= _T(strMainPath + "Setting\\AMT820_WAITTIME_MAP.xls");

	st_path.mstr_partno_match		= _T("C:\\AMT820\\Setting\\PartNo.txt");
	st_path.mstr_user_control_map	= _T(st_path.mstr_basic_folder + "AMT820_USERCONTROL_MAP.xls");

	st_path.m_strPcBoxIDSave		= _T("C:\\AMT820\\Setting\\PcBoXID.dat");

	st_path.m_strMars				= _T("C:\\AMT820\\Log\\Mars\\");
	Func.CreateFolder(st_path.m_strMars);

	st_path.mstr_xgem_cfg_path		= _T("C:\\AMT820\\Setting\\AMT820_Eq.cfg");

	st_path.mstr_heater =			_T(strMainPath + "Log\\HEATER\\");
	Func.CreateFolder(st_path.mstr_heater);

	st_path.m_strIndexLog =			_T(strMainPath + "Log\\Index\\");
	Func.CreateFolder(st_path.m_strIndexLog);

	st_path.m_strViboXLog =			_T(strMainPath + "Log\\ViboX\\");
	Func.CreateFolder(st_path.m_strViboXLog);

	st_path.m_strPickFailLog =		_T(strMainPath + "Log\\Machine\\");
	Func.CreateFolder(st_path.m_strPickFailLog);

	st_path.m_strLastModuleLog =	_T(strMainPath + "Log\\Machine\\");
	Func.CreateFolder(st_path.m_strLastModuleLog);

	st_path.m_strFtpPath = _T("C:\\AMT820\\Setting\\FTP.TXT");

}

void CMainFrame::OnExitData_Saving()
{
	
	st_handler.n_menu_lock = TRUE;

	if (st_handler.cwnd_basic != NULL)				// BASIC 화면 존재
	{
		st_handler.cwnd_basic->PostMessage(WM_BASIC_APPLY, 0, 0);
	}
	else if (st_handler.cwnd_maintenance != NULL)	// Tower Lamp 화면 존재
	{
		st_handler.cwnd_maintenance->PostMessage(WM_MAINTENANCE_APPLY, 0, 0);
	}
	else if (st_handler.cwnd_waittime != NULL)		// Wait Time 화면 존재
	{
		st_handler.cwnd_waittime->PostMessage(WM_WAITTIME_APPLY, 0, 0);
	}
	else if (st_handler.cwnd_motorspeed != NULL)	// Motor Speed 화면 존재
	{
		st_handler.cwnd_motorspeed->PostMessage(WM_MOTORSPEED_APPLY, 0, 0);
	}

	/* ************************************************************************** */
    /* 화면에 설정된 모든 데이터 파일에 저장한다                                  */
	/* -> 프로그램 종료 시점에서 오퍼레이터가 데이터 저장 파일 삭제하는 경우 대비 */
	/* -> 장비 동작 중에 파일 삭제되더라도 현재 설정 정보 유지된다                */
    /* ************************************************************************** */
	mcls_m_basic.OnBasic_Data_Save();				// 전역 변수에 저장된 Basic 셋팅 정보를 파일에 저장하는 함수

	mcls_m_basic.OnWaitTime_Data_Save();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 저장하는 함수
	mcls_m_basic.OnMotorSpeed_Set_Data_Save();		// 전역 변수에 저장된 Motor Speed 셋팅 정보를 파일에 저장하는 함수.

	mcls_m_basic.On_Teach_Data_Save();
	mcls_m_basic.OnMaintenance_Data_Save();
	mcls_m_basic.OnInterface_Data_Save();
	
	st_handler.n_menu_lock = FALSE;
}

void CMainFrame::OnMain_Thread_Creating()
{
	int i;
	// **************************************************************************
	// 쓰레드 생성 플래그 초기화                                                 
	// **************************************************************************
	All_Stop = 0 ;  // 쓰레드 상태 플래그 설정 : 쓰레드 동작 시작
	
	for (i = 0; i < 25; i++)
	{
		m_thread[i] = NULL;
	}
	// **************************************************************************
	
	// **********************************************************************
	// 타워 램프 출력 동작 제어 쓰레드 생성한다                              
 	// **********************************************************************
	// 20100805 tae 수정 
	m_thread[THREAD_HANDLER_CHK]=AfxBeginThread(OnThread_Handler_Check, this);			// 
	if (m_thread[THREAD_HANDLER_CHK] != NULL) 	
		hThrHandle[THREAD_HANDLER_CHK] = m_thread[THREAD_HANDLER_CHK]->m_hThread;

	m_thread[THREAD_TOWER_LAMP_CHK]=AfxBeginThread(OnThread_Tower_Lamp_Check, this);	// THREAD_PRIORITY_ABOVE_NORMAL) ;
 	if (m_thread[THREAD_TOWER_LAMP_CHK] != NULL) 	
 		hThrHandle[THREAD_TOWER_LAMP_CHK] = m_thread[THREAD_TOWER_LAMP_CHK]->m_hThread;
 	// **********************************************************************
	
	// **********************************************************************
	// 장비 동작 중 발생한 알람 출력하기 위한 쓰레드 생성한다                
	// **********************************************************************
	m_thread[THREAD_ALARM_DISPLAY]=AfxBeginThread(OnThread_Alarm_Display, this);		// THREAD_PRIORITY_ABOVE_NORMAL) ;
	if (m_thread[THREAD_ALARM_DISPLAY] != NULL) 	
		hThrHandle[THREAD_ALARM_DISPLAY] = m_thread[THREAD_ALARM_DISPLAY]->m_hThread;
	// **********************************************************************

	m_thread[THREAD_IO]=AfxBeginThread(OnThread_IO_Check, this);
	if (m_thread[THREAD_IO] != NULL) 	
		hThrHandle[THREAD_IO] = m_thread[THREAD_ALARM_DISPLAY]->m_hThread;

	m_thread[THREAD_MOTORS] = AfxBeginThread( OnThread_Motors, this );
	if( m_thread[THREAD_MOTORS] != NULL )
		hThrHandle[THREAD_MOTORS] = m_thread[THREAD_MOTORS]->m_hThread;
}

void CMainFrame::OnMain_Thread_Destroy()
{
	for( int i=0; i<25; i++ )
	{
		if (hThrHandle[i])  // 쓰레드 존재
			::WaitForSingleObject(hThrHandle[i], 1500);
	}

}

void CMainFrame::OnAlarm_Destroy()
{
	if(mp_alarm_dlg)
	{
		if (mp_alarm_dlg->IsWindowVisible())
		{
			mp_alarm_dlg->DestroyWindow();
		}

		if (st_handler.n_alarm_screen != FALSE)  // 알람 화면 출력
			st_handler.n_alarm_screen = FALSE;
	}
}

void CMainFrame::OnDestroy() 
{
	/* ************************************************************************** */
	/* 만약 모달리스 알람 화면 삭제되지 않았다면 재 삭제 작업                     */
	/* -> mp_modal_dlg : 알람 화면에 대한 포인터 저장 변수                        */
	/* ************************************************************************** */
	if (mp_alarm_dlg)
	{
		OnAlarm_Destroy();			// 알람 화면 종료 함수 */
		delete this;
	}
	/* ************************************************************************** */

	KillTimer(TMR_MAIN_REFRESH);	// Time 출력 타이머 삭제
	KillTimer(TMR_FILE_CREATE);

	CFrameWnd::OnDestroy();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CString str_display_time;
	int n_hour, n_minute, n_second;
	int n_temp;  // 총 시간 [초] 저장 변수
	int n_year, n_month, n_day;				// 년, 월, 일 저장 변수

	CString mStartTime, mEndTime; 
	char cStartTime[60], cEndTime[60];

	COleDateTime cur_time;
	CTime bin_time;

	if (nIDEvent == TMR_MAIN_REFRESH)
	{
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan diff = cur - st_handler.m_tRef;
		st_handler.m_tRef = cur;

		if (st_handler.n_maint_mode == YES)		// 메인트 모드이면 메인트 타임만 업데이트 된다.
		{
			if(st_handler.n_lot_flag == TRUE)
			{
				st_handler.m_tM = st_handler.m_tM + diff;
			}
			st_handler.m_tDM = st_handler.m_tDM + diff;
			
			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, dMAINT);
			}
		}
		else
		{
			switch(st_work.mn_run_status)
			{			
			case dRUN:
				Dll_TimeDataLog(2, 1);						// Run임을 DLL에 알려준다.
				if(st_handler.n_lot_flag == TRUE)
				{
					st_handler.m_tR = st_handler.m_tR + diff;
				}
				st_handler.m_tDR = st_handler.m_tDR + diff;
				break;

			case dWARNING:
			case dJAM:		
				if(st_handler.n_lot_flag == TRUE)
				{
					st_handler.m_tJ = st_handler.m_tJ + diff;
				}
				st_handler.m_tDJ = st_handler.m_tDJ + diff;
				break;

			case dLOCK:
				if(st_handler.n_lot_flag == TRUE)
				{
					st_handler.m_tM = st_handler.m_tM + diff;
				}
				st_handler.m_tDM = st_handler.m_tDM + diff;
				break;
			
			case dLOTEND:												// Lot End 시에도 Stop으로 시간을 올린다.
			case dSTOP:
				if(st_handler.n_lot_flag == TRUE)
				{
					st_handler.m_tS = st_handler.m_tS + diff;
				}
				st_handler.m_tDS = st_handler.m_tDS + diff;
				break;
			}

			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, st_work.mn_run_status);
			}
		}
	}
	else if (nIDEvent == TMR_FILE_CREATE)
	{
		/* ********************************************************************** */
		/* 파일 생성 시 필요한 시간 설정한다                                      */
		/* ********************************************************************** */
		cur_time = COleDateTime::GetCurrentTime();

		n_hour = cur_time.GetHour();
		n_minute = cur_time.GetMinute();
		n_second = cur_time.GetSecond();
		n_temp = n_hour * 3600 + n_minute * 60 + n_second;
		/* ********************************************************************** */

		n_year = cur_time.GetYear();  
		n_month = cur_time.GetMonth();  
		n_day = cur_time.GetDay();  
		
		//050420 데이터 서머리 관리 Auto로 추가 2K5/04/22/ViboX
		if(n_hour == 21 && n_minute >= 59 && n_second >= 58)
		{
			if (st_work.mn_run_status == dLOTEND)
			{
				Dll_TimeDataLog(2, dSTOP);	
			}
			else
			{
				Dll_TimeDataLog(2, st_work.mn_run_status);
			}
		}
	    else if(n_hour == 22 && i_summary_flag == 0)
		{
			i_summary_flag = 1;

			CTime tmTime(n_year,n_month,n_day,22,0,0);
			tmTime -= CTimeSpan(1,0,0,0); //하루전날것부터 찾아 처리 
			mStartTime = tmTime.Format("%Y%m%d%H%M%S");	//20050419220000

			CTime tsTime(n_year,n_month,n_day,21,59,59);			
			mEndTime = tsTime.Format("%Y%m%d%H%M%S");	//20050420215959

			sprintf(cStartTime, "%s", mStartTime);
			sprintf(cEndTime, "%s", mEndTime);

			Dll_SearchSummaryData(0, "ALLSUM", "0", cStartTime, cEndTime);	//050418 API_** 에서 Dll_** 로 변경 
		}
		else if(n_hour < 21 && i_summary_flag == 1)
		{
			i_summary_flag = 0;
		}

		// 현재는 3 Shift까지만 가능하게 해 놨다. 게다가 Fix다. -_-a
		if ((n_hour == 14 || n_hour == 22) && n_minute == 0 && n_second < 2)
		{
			// Socket Yield Reset
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnMainframe_Work(WPARAM wParam, LPARAM lParam)
{	
	int mn_command_num = wParam;  // 네트워크 작업을 할 구분 변수
	
	switch (mn_command_num)
	{	
	case 1001:
		OnMainFrame_SelectMessageDisplay();
		break;
	}
	
	return 0;
}

void CMainFrame::OnMainFrame_SelectMessageDisplay()
{
	int ReturnVal;
	
	CDialog_Select select_dlg;
	
	st_msg.mstr_confirm_msg = _T(st_handler.mstrSelectMessage);
	ReturnVal = select_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.mnSelectMessage = 1;
	}
	else
	{
		st_handler.mnSelectMessage = 2;
	}
}

void CMainFrame::Init_View()
{
/*	if(theApp.m_pMatroxAlg.m_Image[LOWER_PART]!=NULL){
		delete theApp.m_pMatroxAlg.m_Image[LOWER_PART];
		theApp.m_pMatroxAlg.m_Image[LOWER_PART] = NULL;
	}
	theApp.m_pMatroxAlg.m_Image[LOWER_PART] = new CCB_Lib;

	if(theApp.m_pMatroxAlg.m_Image[UPPER_PART]!=NULL){
		delete theApp.m_pMatroxAlg.m_Image[UPPER_PART];
		theApp.m_pMatroxAlg.m_Image[UPPER_PART] = NULL;
	}
	theApp.m_pMatroxAlg.m_Image[UPPER_PART] = new CCB_Lib;

	if(theApp.m_pMatroxAlg.m_InspectionAlg[LOWER_PART]!=NULL){
		delete theApp.m_pMatroxAlg.m_InspectionAlg[LOWER_PART];
		theApp.m_pMatroxAlg.m_InspectionAlg[LOWER_PART] = NULL;
	}
	theApp.m_pMatroxAlg.m_InspectionAlg[LOWER_PART] = new CInsAlg;

	if(theApp.m_pMatroxAlg.m_InspectionAlg[UPPER_PART]!=NULL){
		delete theApp.m_pMatroxAlg.m_InspectionAlg[UPPER_PART];
		theApp.m_pMatroxAlg.m_InspectionAlg[UPPER_PART] = NULL;
	}
	theApp.m_pMatroxAlg.m_InspectionAlg[UPPER_PART] = new CInsAlg;

	theApp.m_pMatroxAlg.m_Image[LOWER_PART]->InitializeCBLib(LOWER_PART , CPoint( IMG_WIDTH , IMG_HEIGHT ) ) ; 
	theApp.m_pMatroxAlg.m_Image[UPPER_PART]->InitializeCBLib(UPPER_PART , CPoint( IMG_WIDTH , IMG_HEIGHT ) ) ; 
*/
}


/*
LRESULT CMainFrame::OnCommand_Client_1(WPARAM wParam, LPARAM lParam)
{	
	CString str_send;
	CString str_rev;
	CString str_tmp;

	switch(wParam)
	{
		case CLIENT_CONNECT:
			if(st_client[lParam].n_connect == NO)
			{
				m_p_client[lParam] = new CClientSocket;
				if(m_p_client[lParam]->Create(lParam, st_client[lParam].str_ip, st_client[lParam].n_port, 1))
				{
					st_client[lParam].n_connect = YES;
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Client_[%02d] Connect Success..", lParam);
						sprintf(st_other.c_normal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
					}
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Client_[%02d] Connect Fail..", lParam);
						sprintf(st_other.c_abnormal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
					}
				}
			}
			break;

		case CLIENT_CLOSE:
			if(st_client[lParam].n_connect == YES)
			{
				delete m_p_client[lParam];
				m_p_client[lParam]			= NULL;
				st_client[lParam].n_connect = NO;

				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Client_[%02d] Close..", lParam);
					sprintf(st_other.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
				}
			}
			break;

		case CLIENT_SEND:
			if(st_client[lParam].n_connect == YES)
			{
				str_send.Format("%s", st_client[lParam].ch_send);
				m_p_client[lParam]->Send(str_send, str_send.GetLength());

				if(st_client[lParam].n_rev_info == NO)
				{
					delete m_p_client[lParam];
					m_p_client[lParam]			= NULL;
					st_client[lParam].n_connect = NO;
				}

				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Client_[%02d] - %s Data Send", lParam, str_send);
					sprintf(st_other.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
				}
			}
			break;

		case CLIENT_REV:
			str_rev.Format("%s", st_client[lParam].ch_rev);

			if(st_client[lParam].n_rev_info == YES)
			{
				delete m_p_client[lParam];
				m_p_client[lParam]			= NULL;
				st_client[lParam].n_connect = NO;
			}

			if(st_handler.cwnd_list != NULL)
			{
				str_tmp.Format("Client_[%02d] - %s Data Receive", lParam, str_rev);
				sprintf(st_other.c_normal_msg, str_tmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
			break;
	}
	
	return 0;
}

LRESULT CMainFrame::OnCommand_Server_1(WPARAM wParam, LPARAM lParam)
{	
	CString str_send;
	CString str_rev;
	CString str_tmp;
	int		count;

	switch(wParam)
	{
		case SERVER_CONNECT:
			if(st_server[lParam].n_connect == NO)
			{
				m_p_server[lParam] = new CServerSocket;
				if(m_p_server[lParam]->Create(lParam, st_server[lParam].n_port))
				{
					st_server[lParam].n_connect = YES;
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Server_[%02d] Start Success..", lParam);
						sprintf(st_other.c_normal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
					}
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Server_[%02d] Start Fail..", lParam);
						sprintf(st_other.c_abnormal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
					}
				}
			}
			break;
			
		case SERVER_CLOSE:
			if(st_server[lParam].n_connect == YES)
			{
				delete m_p_server[lParam];
				m_p_server[lParam]			= NULL;
				st_server[lParam].n_connect = NO;
				
				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Server_[%02d] Close..", lParam);
					sprintf(st_other.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
				}
			}
			break;
			
		case SERVER_SEND:
			if(st_server[lParam].n_connect == YES)
			{
				count = m_p_server[lParam]->GetClientCount();

				str_send.Format("%s", st_server[lParam].ch_send);
				m_p_server[lParam]->SendClientAt(m_p_server[lParam]->GetClient(count-1), str_send, str_send.GetLength());
				
				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Server_[%02d] - %s Data Send", lParam, str_send);
					sprintf(st_other.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
				}
			}
			break;
			
		case SERVER_REV:
			str_rev.Format("%s", st_server[lParam].ch_rev);
			
			if(st_handler.cwnd_list != NULL)
			{
				str_tmp.Format("Server_[%02d] - %s Data Receive", lParam, str_rev);
				sprintf(st_other.c_normal_msg, str_tmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
			break;
	}
	
	return 0;
}
*/
void CMainFrame::OnMain_Port_Create(int n_port)
{
	char parity;
	DWORD dwCommEvents;
	
	if(st_serial.n_connect[n_port] == YES) return;
	
	dwCommEvents = m_ports[n_port].MmdSerialGetCommEvents();  // 시리얼 이벤트 설정
	dwCommEvents = 0;
	dwCommEvents |= EV_CTS;
	dwCommEvents |= EV_RXCHAR;
	
	switch(rs_232.n_serial_parity[n_port])
	{
	case 0:
		parity = 'E';
		break;
		
	case 1:
		parity = 'O';
		break;
		
	case 2:
		parity = 'N';
		break;
		
	default:
		parity = 'N';
		break;
	}
	
	if(m_ports[n_port].MmdSerialInitializePort(this, 
											   rs_232.n_serial_port[n_port], 
											   rs_232.n_serial_baudrate[n_port], 
											   parity, 
											   rs_232.n_serial_data[n_port], 
											   rs_232.n_serial_stop[n_port], 
											   1024, 
											   dwCommEvents)) 
	{
		m_ports[n_port].MmdSerialStartMonitoring();
	}
	else 
	{
		st_serial.str_port_chk[n_port] = "NOT FOUND";
	}
	
	mstr_received[n_port].Empty();  // 수신 메시지 임시 저장 변수 초기화
	// **************************************************************************
	
	if (st_serial.str_port_chk[n_port] == "NOT FOUND")
	{
		st_serial.n_connect[n_port] = NO;
	}
	else
	{
		st_serial.n_connect[n_port] = YES;
		st_serial.n_rec_chk[n_port] = TRUE;
	}
}

void CMainFrame::OnMain_Port_Close()
{
	int i;
	// **************************************************************************
	// 생성한 RS-232C 시리얼 포트 해제한다                                       
	// -> 동일한 RS-232C 시리얼 포트를 다른 화면에서 사용하고자 할 경우에는      
	//    반드시 어느 한 부분의 포트는 일단 해제한 후 사용해야 한다              
	//  : 이미 해당 RS-232C 시리얼 포트 열린 상태에서 다시 열 수 없기 때문이다   
	// **************************************************************************
	for(i = 0; i < MAX_PORT; i++){
		if(st_handler.Serial_Port_Creating[i] == YES){
			m_ports[i].MmdSerialDestroyObject();
			st_handler.Serial_Port_Creating[i] = NO;
		}	
	}
	// **************************************************************************
}

LRESULT CMainFrame::OnMainMessageEvent(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	CString strTemp;
	
	if (wParam == CTL_YES)
	{
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->SetFocus();	// 대화상자를 활성화
			mp_msg_dlg->OnEventMsg_Text_Set();
		}
		else
		{
			mp_msg_dlg = new CDialog_Event_Msg( st_msg.mstr_event_msg[0], st_msg.mstr_event_msg[1], st_msg.mstr_event_msg[2]);
			//2017.0201
			//mp_msg_dlg = new CDialog_Event_Msg;
			mp_msg_dlg->Create();
			mp_msg_dlg->ShowWindow(SW_SHOW);
		}
		g_ioMgr.set_out_bit(st_io.o_Buzzer2, IO_ON);
		
	}
	else if (wParam == CTL_NO)
	{
		for (i = 0; i < 3; i++)
		{
			st_msg.mstr_event_msg[i] = "";
		}
		
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->ShowWindow(SW_HIDE);
			mp_msg_dlg->DestroyWindow();
			delete mp_msg_dlg;
			mp_msg_dlg = NULL;
		}
		
		g_ioMgr.set_out_bit(st_io.o_Buzzer2, CTL_OFF);
	}
	
	return 0;
}

// ******************************************************************************
// RS-232C 시리얼 포트 제어 함수                                                 
// ******************************************************************************
LONG CMainFrame::OnCommunication(WPARAM port, LPARAM ch)
{
	// **************************************************************************
    // RS-232C 시리얼 포트 검사한다                                              
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM 포트 검사
		return -1;
	// **************************************************************************
    
	if (ch == -1)  // 읽기 에러
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_serial.comm_err[port-1] = _T("[수신 데이터] 시리얼 포트 읽기 에러 발생");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, RECEIVE_MSG);  // 에러 메시지 출력 요청
		}

		st_serial.n_rec_chk[port-1] = COM_ERROR;  // 수신 에러 플래그 설정
		return 0;
	}
	else if (ch == -2)  // 쓰기 에러
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_serial.comm_err[port-1] = _T("[수신 데이터] 시리얼 포트 쓰기 에러 발생");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, SEND_MSG);  // 에러 메시지 출력 요청
		}

		st_serial.n_rec_chk[port-1] = COM_ERROR;  // 수신 에러 플래그 설정
		return 0;
	}

	if (port == LOT_BARCODE_PORT)
	{
//		OnMain_Lot_Barcode(port, ch);     // 수신된 바코드 리더기 데이터 처리 함수
	}
	else
	{
//		OnMain_Device_Barcode(port, ch);     // 수신된 바코드 리더기 데이터 처리 함수
	}


	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C 시리얼 포트 이벤트 설정 함수                                          
// ******************************************************************************
LONG CMainFrame::OnCommunicationEvent(WPARAM port, LPARAM event)
{
	// **************************************************************************
    // 이벤트 설정할 시리얼 포트 포트 검사한다                                   
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM 포트 검사
		return -1;
	// **************************************************************************

	if (event & EV_BREAK)
		AfxMessageBox ("EV_BREAK event");
	else if (event & EV_CTS)
		AfxMessageBox ("EV_CTS event");
	else if (event & EV_DSR)
		AfxMessageBox ("EV_DSR event");
	else if (event & EV_ERR)
		AfxMessageBox ("EV_ERR event");
	else if (event & EV_RING)
		AfxMessageBox ("EV_RING event");
	else if (event & EV_RLSD)
		AfxMessageBox ("EV_RLSD event");
	else if (event & EV_RXCHAR)
		AfxMessageBox ("EV_RXCHAR event");
	else if (event & EV_RXFLAG)
		AfxMessageBox ("EV_RXFLAG event");
	else if (event & EV_TXEMPTY)
		AfxMessageBox ("EV_TXEMPTY event");

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C 시리얼 포트를 통한 메시지 전송 함수                                   
// -> wParam : 메시지 전송할 포트 넘버                                           
// ******************************************************************************
LRESULT CMainFrame::OnDataSend(WPARAM wParam, LPARAM lParam)
{
	int nport = wParam;

	if (nport == DEVICE_BARCODE_PORT)
	{
//		OnMain_Send_Barcode(wParam, lParam);     // 수신된 바코드 리더기 데이터 처리 함수
	}
	return 0;
}

LRESULT CMainFrame::OnSerialPort(WPARAM wParam,LPARAM lParam)
{
	if (wParam == YES)
	{
		OnMain_Port_Create(lParam);
	}
	else if (wParam == NO)
	{
		OnMain_Port_Close();
	}
	return 0;
}

void CMainFrame::OnListError()
{
	/* ************************************************************************** */
    /* 화면 뷰 전환 불가능한 정보 검사한다.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Error)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_ERROR);
}

void CMainFrame::OnMain_Motor_Setting()
{
//	int i;
/*	
	COMI.Set_MotUnitDist(M_SORTER_X,	25.99981539);
	COMI.Set_MotUnitDist(M_SORTER_Y,	25.99981539);
	COMI.Set_MotUnitDist(M_SORTER_Z,	10);
	COMI.Set_MotUnitDist(M_LOWER_B_Y,	33.33229112);
	COMI.Set_MotUnitDist(M_UPPER_B_Y,	20);
	COMI.Set_MotUnitDist(M_UPPER_B_Z,	7.998493232);
	COMI.Set_MotUnitDist(M_LROBOT_X,	25.99981539);
	COMI.Set_MotUnitDist(M_LROBOT_Y,	25.99981539);
	COMI.Set_MotUnitDist(M_UROBOT_X,	25.99981539);
	COMI.Set_MotUnitDist(M_UROBOT_Y,	25.99981539);
	COMI.Set_MotUnitDist(M_LROBOT_Z,	10);
	COMI.Set_MotUnitDist(M_UROBOT_Z,	10);
	
	for (i = 0; i < 50; i++)
	{
		cmmCfgSetCtrlMode(i, 1);
		st_motor[i].mn_homecheck_method = 6;
	}
*/	
	// 초기 속도를 2000으로 잡는다.
//	COMI.Set_MotInitSpeed(M_SORTER_Z, 20000);
//	COMI.Set_MotInitSpeed(M_UPPER_B_Z, 20000);
//	COMI.Set_MotInitSpeed(M_LROBOT_Z, 20000);
//	COMI.Set_MotInitSpeed(M_UROBOT_Z, 20000);
}

void CMainFrame::OnDataChange(WPARAM port, LPARAM ch)
{
	if(port == 0)
	{
		mcls_m_basic.OnBasic_Data_Save();
		mcls_m_basic.On_Teach_Data_Save();
		mcls_m_basic.OnMaintenance_Data_Save();
		mcls_m_basic.OnWaitTime_Data_Save();
		mcls_m_basic.OnMotorSpeed_Set_Data_Save();
		mcls_m_basic.OnInterface_Data_Save();
	}
}


void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	return;
	
	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	return;

	CFrameWnd::OnNcRButtonDown(nHitTest, point);
}


void CMainFrame::OnDialog_Delete()
{
/*	int i;

	for(i=0; i<MAX_MOTOR; i++)
	{
		if(Func.m_pAxis[i] != NULL)
		{
			Func.Axis_Delete(i);
		}
	}

	if(Func.m_pIO != NULL)
	{
		Func.IO_Delete();
	}*/
}

int CMainFrame::OnMain_New_Device_Check()
{

	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;

	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	m_str_new_device.MakeUpper();
	m_str_new_device.TrimLeft(' ');               
	m_str_new_device.TrimRight(' ');

	if (m_str_new_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_other.str_op_msg = _T("[DEVICE] A name input error occurrence.");
			st_other.str_op_msg = _T("[DEVICE] 파일명 입력 에러입니다.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */

	str_chk_file = st_path.mstr_basic + m_str_new_device;  // 생성할 [폴더]+[파일명] 설정
	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_op_msg = _T("[DEVICE] 파일명 입력 에러입니다..");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}

	m_str_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정

	return TRUE;
}

void CMainFrame::OnMain_Create_Device()
{
	CDialog_Message msg_dlg;
	CFileFind finder;
	
	if(finder.FindFile(m_str_load_file))
	{
		st_other.str_fallacy_msg = _T("[DEVICE] 파일명 입력 에러입니다. 동일 파일명이 존재합니다.");
		msg_dlg.DoModal();
	}
	else
	{
		mcls_m_basic.OnBasic_Data_Save_As(m_str_new_device);
//		mcls_m_basic.On_Teach_Data_Save_As(m_str_new_device);
		mcls_m_basic.OnMaintenance_Data_Save_As(m_str_new_device);
		mcls_m_basic.OnInterface_Data_Save_As(m_str_new_device);
	}
}

void CMainFrame::OnPgmInfo() 
{
	// TODO: Add your command handler code here
	CDialog_Pass_Check pass_dlg;
	int n_response;

	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	st_other.str_pass_level = "Master";
	
	n_response = pass_dlg.DoModal();
	
	if (n_response != IDOK)
	{
		return;
	}
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Pgm_Info)))   return;
	
	OnSwitchToForm(IDW_SCREEN_PGM_INFO);
}

void CMainFrame::Init_ToolTip()
{
	CRect	rect_hot;
	int		n_index;

	m_p_tooltip.Create(this);
	
	n_index = m_wndToolBar.CommandToIndex(ID_MAIN);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Main Screen</b><ct=0x00C000><br><hr=100%><br></ct>Total Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_BASIC);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Basic Screen</b><ct=0x00C000><br><hr=100%><br></ct>Basic Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_SETTING);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Setting Screen</b><ct=0x00C000><br><hr=100%><br></ct>Setting Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_MOTOR);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Motor Screen</b><ct=0x00C000><br><hr=100%><br></ct>Motor Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_IO);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>IO Screen</b><ct=0x00C000><br><hr=100%><br></ct>IO Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_LIST);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>List Screen</b><ct=0x00C000><br><hr=100%><br></ct>List Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_SAVE);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Job File Save</b><ct=0x00C000><br><hr=100%><br></ct>Job File Save."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_ALARM);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Alarm Screen</b><ct=0x00C000><br><hr=100%><br></ct>Alarm Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_LOCK);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Program Lock/UnLock Screen</b><ct=0x00C000><br><hr=100%><br></ct>Program Lock/UnLock View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_PGM_INFO);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Program Information Screen</b><ct=0x00C000><br><hr=100%><br></ct>Program Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_EXIT);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Program Exit</b><ct=0x00C000><br><hr=100%><br></ct>Program Exit."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
}

void CMainFrame::OnMotorSpeed() 
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
	// **************************************************************************
	if (st_handler.n_level_teach !=  TRUE)
	{
		if (st_handler.n_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.n_level_teach !=  TRUE)
			{
				if (st_handler.n_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor_Speed)))   return;
	OnSwitchToForm(IDW_SCREEN_MOTOR_SPEED);		
}
