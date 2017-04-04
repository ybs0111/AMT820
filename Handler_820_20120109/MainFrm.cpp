// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "HANDLER.h"

#include "MainFrm.h"

/* ****************************************************************************** */
/* ��ȭ ���� �� �ܺ� Ŭ���� �߰�                                                  */
/* ****************************************************************************** */
#include "MenuHead.h"		// ȭ�鿡 ���� ��� ���ǵ� Ŭ���� �߰� 
#include "Dialog_Menu.h"	// �޴� ���� ��ȭ ���� Ŭ���� �߰� 

#include "Dialog_About.h"	// ���α׷� About ��ȭ ���� Ŭ���� �߰� 
#include "Dialog_Exit.h"	// ���� ���α׷� ���� ȭ�� Ŭ���� �߰� 
#include "Dialog_Select.h"
#include "Dialog_Pass_Check.h"
#include "Dialog_Message.h" // ���� ��� ��ȭ ���� Ŭ���� �߰� 
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
/* ���� ����ü ���� �κ�                                                           */
/* ******************************************************************************* */
st_handler_param			st_handler;			// ȭ�� ���� ���� �⺻ ������ ���� ����ü ���� ����
st_time_param				st_time;
st_message_param			st_msg;
st_initial_param			st_init;			// ��� �ʱ�ȭ�� ����ü ����
st_other_param				st_other;			// ��Ÿ ���� ���� ����ü ����
st_alarm_param				alarm;				// �˶� ������ ���� ����ü ����
st_dsp_alarm_param			st_alarm;			// �˶� ������ ���� ����ü ����
st_work_param				st_work;			// ��� ���� ���� �÷��� ���� ���� ����ü ���� 
st_basic_param				st_basic;			// ������ ȭ�� ȯ�� ���� ���� ���� ����ü ����
st_lamp_param				st_lamp;			// Tower Lamp ȭ�鿡 ���� ���� ���� ����ü ���� ����
st_home_param				st_home[MAX_MOTOR];	// Home Check �� ���� ����ü ����
//st_io_param					st_io;				// I/O ���� ������ ���� ����ü ���� ����
st_path_param				st_path;			// ���� ��θ� ���� ����ü ���� ����
st_serial_param				st_serial;			// Serial ��� ����ü ���� ����
st_io_wait_param			st_wait;			// I/O ���� ��� �ð� �� ����Ʈ �ð� ���� ����ü ����
st_sync_param				st_sync;			// �� Thread�� ����ȭ ����ü ����
st_traybuffer_param			st_traybuffer;		// TRAY & BUFFER ���� ���� ���� ����ü ����
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
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)		// �ؽ�Ʈ ���� ��� �ٿ� ��� ���� ����� ���� �޽��� ���� 
	ON_MESSAGE(WM_MAINFRAME_WORK, OnMainframe_Work)
	ON_MESSAGE(WM_FORM_CHANGE, OnViewChangeMode)					// Post Message�� ���� ȭ�� ��ȯ ����� ����� ���� �޽��� ���� 
//	ON_MESSAGE(WM_CLIENT_MSG_1, OnCommand_Client_1)							// Network���õ� �۾��� ����Ѵ�.
//	ON_MESSAGE(WM_SERVER_MSG_1, OnCommand_Server_1)							// Network���õ� �۾��� ����Ѵ�.
	ON_MESSAGE(WM_COMM_DATA, OnCommunication)						// RS-232C �ø��� ��Ʈ ���� �޽���
	ON_MESSAGE(WM_COMM_EVENT, OnCommunicationEvent)					// RS-232C �ø��� ��Ʈ �̺�Ʈ ���� �޽���
	ON_MESSAGE(WM_DATA_SEND, OnDataSend)							// RS-232C �ø��� ��Ʈ ���� ������ �۽� �޽���
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

	OnMain_Var_Default_Set();				// ���� ������ Ŭ���� ���� �ʱ�ȭ �Լ�

	Func.PublicVariableDataInitialize();	// ��� ���� ���� �÷��� �ʱ�ȭ �Լ�
	Func.On_IOFlagReset();					// ���۽� ����ϴ� IO ���� Flag �ʱ�ȭ �Լ� 2K4/12/10/ViboX
	mcls_frm_alarm.On_Alarm_Info_Load();	// ���Ͽ� ����� ��� �˶� ���� ���� ������ �����ϴ� �Լ�
	
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
  	
	st_handler.hWnd = GetSafeHwnd() ;  // ���� �����ӿ� ���� �ڵ� ���� ����

	// ���� �ػ� ����.
// 	if (::GetSystemMetrics(SM_CXSCREEN) != 1280 && ::GetSystemMetrics(SM_CYSCREEN ) != 1024)
// 	{
// 		DEVMODE mode ;
// 		mode.dmBitsPerPel = 32;				// 32��Ʈ Į��� ����
// 		mode.dmPelsWidth  = 1280;			
// 		mode.dmPelsHeight = 1024;
// 		mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
// 		ChangeDisplaySettings(&mode, CDS_FULLSCREEN);
// 	}
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// =============================================================================
    // Text ���� Icon ������ ����                                                 //
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
    // �ؽ�Ʈ ���� ��Ÿ�� ���� (��� �ٿ� �޴��� ����)                            //
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
    // Ÿ��Ʋ ���� ��� Initial Dialog bar ����                                   //
    // =============================================================================
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my init bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_title = &m_wndTitleBar;
	// =============================================================================

	// =============================================================================
    // ����Ʈ ���� ��� Initial Dialog bar ����                                   //
	// -> ���� �� �ؿ� ��ġ�ϵ��� �Ϸ��� �� �κ��� ���� �� ���� �κ� ���� �ø���. //
    // =============================================================================
	if (!m_wndListBar.Create(this, IDD_DIALOG_LIST, CBRS_ALIGN_BOTTOM, IDD_DIALOG_LIST))
	{
		TRACE0("Failed to create my information bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_list = &m_wndListBar;
	/* ************************************************************************** */
	
	// **************************************************************************
    // ���� ���� �� I/O ���� �ʱ�ȭ �� ��� ��Ʈ Ŭ���� �۾� �����Ѵ�        
    // **************************************************************************
	g_ioMgr.IO_Board_Initialize();				// IO ���� ���̺귯�� �ʱ�ȭ �Լ�
	Func.OnSet_IO_Out_Port_Clear();			// I/O ������ ��� ��Ʈ �ʱ�ȭ �Լ�
	g_comiMgr.SetMotBoardInit( CTL_Lib.Initialize_motor_board(1, st_path.mstr_motor) );	// MOTOR ���� �ʱ�ȭ ���� �˻��Ѵ�
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
    /* ���� ���α׷��� ���� Ÿ��Ʋ ���� ���                                      */
    /* ************************************************************************** */
	Init_ToolTip();

	SetWindowText("AMT820 : Ver.1.0.0");  // Ÿ��Ʋ ���� ��� 

	CenterWindow();  // ���� ���� ���α׷��� �� �߾ӿ� ��ġ

	/* ************************************************************************** */
    /* �ʱ�ȭ ���� ȭ������ ��ȯ�Ͽ� ��� �ʱ�ȭ �۾� �����Ѵ�                    */
    /* ************************************************************************** */
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);  // �ʱ�ȭ ���� ȭ�� ��ȯ 
	/* ************************************************************************** */
	
	SetTimer(TMR_MAIN_REFRESH, 500, NULL);  // ����Ʈ ���� ���� Ÿ�̸�
	SetTimer(TMR_FILE_CREATE, 1000, NULL);  // ����Ʈ ���� ���� Ÿ�̸�

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU;
	cs.style &= ~FWS_ADDTOTITLE;								// ȭ�� Ÿ��Ʋ ���� ���� �����ϵ��� ��Ÿ�� ���� 
	cs.style &= ~WS_MAXIMIZEBOX;

	cs.lpszClass=((CHANDLERApp*)AfxGetApp())->GetClassName();	// ���� ���� ���α׷��� ���� Ŭ���� �̸��� ��´�(Need for Single Instance)
	
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
		if(m_wndToolBar.IsFloating())  // ���ٰ� �÷��� �� ��쿡 �ٽ� ȭ�� ��ܿ� ���� 
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
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Main)))   return;

	OnSwitchToForm(IDW_SCREEN_MAIN);
}

void CMainFrame::OnBasic() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Basic)))   return;
	OnSwitchToForm(IDW_SCREEN_BASIC);
}

void CMainFrame::OnSetting() 
{
	CDialog_Menu menu_dlg;

	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
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
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
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

	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	st_other.str_parent_menu = "List Screen";  // ���� ���õ� �� �޴� ���� ����
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
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Operation)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_OPERATION);
}

void CMainFrame::OnListWork() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Work)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_WORK);
}

void CMainFrame::OnListAlarm() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Alarm)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_ALARM);
}

void CMainFrame::OnListStep() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Step)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_STEP);
}

void CMainFrame::OnIo() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
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
	int mn_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ���� ���� 
	
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 

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
	int n_response;  // ��ȭ ���� ���� �÷���
	
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	
	CDialog_Select  select_dlg;
	
	st_other.str_confirm_msg = _T("Main : All Setting Data Save?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_handler.n_menu_lock = TRUE;
		
		OnExitData_Saving();					// ȭ�鿡 ������ ��� ������ ���� �Լ�
		
		st_handler.n_menu_lock = FALSE;
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CMainFrame::OnAlarm() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)  return;
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 

	/* ************************************************************************** */
    /* ��޸��� �˶� ȭ�� ����Ѵ�                                                */
	/* -> ���� �˶� ȭ�� ��µ� ��쿡�� �ش� ȭ�鿡 ��Ŀ���� ����                */
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
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
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
	int n_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ���� ���� 
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	if(st_handler.n_lot_flag == CTL_LOTSTART) return;
	/* ************************************************************************** */
	/* �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                               */
	/* -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                       */
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
	/* ���� ���� �ʱ�ȭ ���� ���� �˻��Ѵ�.                                       */
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
	/* ���� ��� ���°� STOP �������� �˻�                                        */
	/* ************************************************************************** */
	if (st_work.mn_run_status != dSTOP)
	{
		st_other.str_fallacy_msg = _T("Handler is active Stop first.!");
		n_response = msg_dlg.DoModal();
		if (n_response == IDOK)  return;
	}
	/* ************************************************************************** */

	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);  // �ʱ�ȭ ���� ȭ�� ��ȯ 
}

void CMainFrame::OnLock() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Pass_Check  pass_chk_dlg;

	/* ************************************************************************** */
	/* �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                               */
	/* -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                       */
	/* ************************************************************************** */
//	if (st_handler.n_level_teach !=  TRUE)
//	{
//		if (st_handler.n_level_maint !=  TRUE)  return ;
//	}
	/* ************************************************************************** */

	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;

	st_other.str_pass_level=_T("SystemLock");  // ��ȣ ���� ����

	n_response = pass_chk_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // ���� �����ӿ� SYSTEM LOCK ȭ�� ��� ��û
	
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
	int mn_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ���� ���� 
	
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)	return;
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ����
	
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
		All_Stop = 1 ;				// ������ ���� ���� ���� ���� �ʱ�ȭ
		
		OnMain_Thread_Destroy();	// ������ ���� �Լ�
		OnExitData_Saving();// ȭ�鿡 ������ ��� ������ ���� �Լ�
		
		st_handler.b_program_exit = true;

		if(st_handler.n_alarm_screen != FALSE)
		{
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// ���� ���������� �޽��� �����Ͽ� �˶� ȭ�� ���� ��û 
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
/* �޴����� ���õ� ȭ������ ��ȯ��Ű�� �Լ�                                       */
/* ****************************************************************************** */
void CMainFrame::OnSwitchToForm(int nForm)
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */

	if (st_handler.n_menu_lock != FALSE)	return;
	if (st_handler.n_system_lock != FALSE)  return;		// ���� �ý��� Lock ������ ��� ���� 

	OnDialog_Delete();

	m_pOldActiveView = GetActiveView();			// ���� ȭ�鿡 ��µ� �� ���� ���� (save old view)

	m_pNewActiveView = (CView*)GetDlgItem(nForm);  // ���� ����� �� ���� ���� (get new view)
	
	if(m_pNewActiveView == NULL)							// ���� ����� �䰡 ���� ��µ��� �ʾҴ��� �˻� (if it hasn't been created yet)
	{
		switch(nForm)			// �信 ������ ID ���� �� (these IDs are the dialog IDs of the view but can use anything)
		{
			case IDW_SCREEN_MAIN:				// ���� ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_Main;
				break;
			case IDW_SCREEN_LOCK:				// System Lock ȭ��
				m_pNewActiveView = (CView*)new CScreen_Lock;
				break;
			case IDW_SCREEN_INIT:				// �ʱ�ȭ ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_Initial;
				break;
			case IDW_SCREEN_BASIC:				// Basic ȭ��
				m_pNewActiveView = (CView*)new CScreen_Basic;
				break;
			case IDW_SCREEN_SET_MAINTENANCE:	// ����Ʈ ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_Set_Maintenance;
				break;
			case IDW_SCREEN_PGM_INFO:
				m_pNewActiveView = (CView*)new CScreen_Pgm_Info;
				break;
			case IDW_SCREEN_LIST_OPERATION:		// ���۷��̼� ���� ��� ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_List_Operation;
				break;
			case IDW_SCREEN_LIST_WORK:			// �׽�Ʈ ��� ����Ʈ ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_List_Work;
				break;
			case IDW_SCREEN_LIST_ALARM:			// �˶� ����Ʈ ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_List_Alarm;
				break;
			case IDW_SCREEN_LIST_STEP:			// ������ ���� ���� ��� ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_List_Step;
				break;
			case IDW_SCREEN_LIST_ERROR:
				m_pNewActiveView = (CView*)new CScreen_List_Error;
				break;
			case IDW_SCREEN_ADMINISTRATOR:		// ADMINISTRATOR ��� ȭ�� 
				m_pNewActiveView = (CView*)new CScreen_Administrator;
				break;
			case IDW_SCREEN_MOTOR:
				m_pNewActiveView = (CView*)new CScreen_Motor;
				break;
			case IDW_SCREEN_MOTOR_SPEED:
				m_pNewActiveView = (CView*)new CScreen_Motor_Speed;
				break;
			case IDW_SCREEN_SET_INTERFACE:		// Interface ���� ���� ȭ�� 
				break;
			case IDW_SCREEN_IO_MAP:
				m_pNewActiveView = (CView*)new CScreen_IO_Map;
				break;
		}

		CCreateContext context;  // ���ο� �信 ��ť��Ʈ Attach ( attach the document to the new view)
		context.m_pCurrentDoc = m_pOldActiveView->GetDocument();

		m_pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nForm, &context);
		m_pNewActiveView->OnInitialUpdate();  		
	}

	SetActiveView(m_pNewActiveView);			// ���� ��µ� �並 ��Ƽ�� ��� ���� (change the active view)
	m_pNewActiveView->ShowWindow(SW_SHOW);	// ���ο� �並 ��� (show the new window)

	m_pOldActiveView->ShowWindow(SW_HIDE);	// ������ ��µ� �並 ����� (hide the old)
	m_pOldActiveView->DestroyWindow();		// ������ ��µ� �並 �ı���Ų�� (Old View Destroy ��Ű�� ���Ͽ� �߰�)
	m_pOldActiveView = NULL;					// ������ ��µ� �信 ���� ���� ���� ���� �ʱ�ȭ 

	::SetWindowLong(m_pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
	RecalcLayout();							// �������� �����Ѵ� (adjust frame)
	
	delete	m_pOldActiveView;					// ������ ��µ� �� ������ �����Ѵ� (kill old view)
	
	st_handler.n_menu_num = nForm;

	/* ************************************************************************** */
	/* ���� ȭ�� ���� Ÿ��Ʋ�ٿ� ����Ѵ�.                                        */
	/* ************************************************************************** */
	if (st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nForm);
	/* ************************************************************************** */
}

/* ****************************************************************************** */
/* ����� ���� �޽����� ���� �� ȭ�� ��ȯ �Լ�                                    */
/* ****************************************************************************** */
LRESULT CMainFrame::OnViewChangeMode(WPARAM wParam,LPARAM lParam)
{
	int mn_chg_num = wParam;  // �� ȭ�鿡 ���� ID �� ���� 

	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
	/* -> �˶� ȭ���� ������ ȭ�鿡 ���ؼ��� �޴� ��� ���� ���� �˻�             */
    /* ************************************************************************** */
	if (mn_chg_num != 9)  
	{
		if (st_handler.n_menu_lock != FALSE)  return 0;
		if (st_handler.n_system_lock != FALSE)  return 0;  // ���� �ý��� Lock ������ ��� ���� 
	}
	/* ************************************************************************** */

	switch(mn_chg_num)
	{
		case 1: 
			if (lParam==1)  OnMain();					// ���� ȭ�� ��ȯ 
			else if (lParam==2)							// System Locking ȭ�� ��ȯ 
			{
				if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Lock)))   return 0;
				OnSwitchToForm(IDW_SCREEN_LOCK) ;
			}
			else if (lParam==3)							// �ʱ�ȭ ȭ�� ��ȯ 
			{
				if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Initial)))   return 0;
				OnSwitchToForm(IDW_SCREEN_INIT) ;
			}
			break;
		case 2: 
			OnBasic();									// ������ ȭ�� ��ȯ 
			break;
		case 3:
			break;
		case 4 :
//			OnWait();
			break;
		case 5 : 
			if(lParam == 1) OnMotor();
			else if (lParam==2)	 OnMotorSpeed();			// ���� �ӵ� ȭ�� ��ȯ 
			break;
		case 6 :
			if (lParam==1)       OnListOperation();		// Operation ��� ����Ʈ ȭ�� ��ȯ 
			else if (lParam==2)  OnListWork();			// �۾� ��� ����Ʈ ȭ�� ��ȯ 
			else if (lParam==3)  OnListAlarm();			// �˶� ����Ʈ ȭ�� ��ȯ 
			else if (lParam==4)  OnListStep();			// ������ ���� ��� ȭ�� ��ȯ 
			else if (lParam ==5) OnListError();
			
			break;
		case 7 : 
			if (lParam==1)       OnAlarm();				// �˶� ȭ�� ��ȯ 
			else if (lParam==2)  OnAlarm_Destroy();		// �˶� ȭ�� ���� �Լ�
			break;
		case 8 : 
			if (lParam==1)       OnAdmin();				// Administrator ȭ�� ��ȯ 
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

	Ret_Val = TRUE;  // �ʱ� ���� �� ���� 

	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.n_menu_lock != FALSE)	Ret_Val = FALSE;
	if (st_handler.n_system_lock != FALSE)  Ret_Val = FALSE;  // ���� �ý��� Lock ������ ��� ���� 

	/* ************************************************************************** */
	/* ���� ��� ���°� STOP �������� �˻�                                        */
	/* ************************************************************************** */
	if (st_work.mn_run_status != dSTOP && st_work.mn_run_status != dLOCK)
	{
		st_other.str_op_msg = _T("[��� ���� ��] ���� STOP ��Ų �Ŀ� ����ϼ���");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, 7); 

		Ret_Val = FALSE;
	}
	/* ************************************************************************** */
	
	return Ret_Val;
}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	//Ÿ��Ʋ�ٸ� ����Ŭ�� ������ �۾����� ����� ���Ͻ�Ų��.
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

	st_handler.n_allhomecheck = CTL_NO;		// ��ü Ȩüũ ��尡 �ƴϴ�.

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
		// amt db ���� 
		if(Mysql.Database_Select("AMT"))
		{	
			// amt db���� lot_data table �߿��� (���̺�, ã�� �׸�, ����, ã�� ��¥)
			 Mysql.Record_Date_Delete("DEVICE_DATA", "DATE", "<", oledate);
		}
	}
	Mysql.MySql_Close();*/
}

void CMainFrame::OnMain_Path_Set()
{
	/*CString strTmp;

	st_path.str_path_file =		_T("c:\\AMT820\\");							// ���� ���� ���� ����
	Func.CreateFolder(st_path.str_path_file);

//	st_path.str_basic =			_T("c:\\AMT820\\Setting\\BasicTerm.TXT");		// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 
	st_path.str_basic =			_T("c:\\AMT820\\Setting\\");		// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.str_basic);
	st_path.str_path_dvc =		_T("c:\\AMT820\\File\\");					// ����̽��� ƼĪ ���� ���� ���� ���� ���� ����
	Func.CreateFolder(st_path.str_path_dvc);
	
	st_path.str_path_alarm =	_T("c:\\AMT820\\Bmp\\");						// ��� �˶� ���� ���� ���� ����
	Func.CreateFolder(st_path.str_path_alarm);
	st_path.str_path_op =		_T("c:\\AMT820\\Operator\\");					// ���۷����� ���� ���� ���� ����
	Func.CreateFolder(st_path.str_path_op);

	st_path.str_path_back =		_T("c:\\BackUp\\");									// BACKUP ���� ���� ���� ����
	Func.CreateFolder(st_path.str_path_back);

	st_path.str_file_basic =	_T("c:\\AMT820\\File\\File.TXT");									// �⺻ ���� ���� ���� ���ϸ� ����
	st_path.str_file_wait =		_T("WaitTime");										// ��� �ð� ���� ���� ���ϸ� ����

	st_path.str_operation =		_T("c:\\AMT820\\Log\\Operation\\");			// Operation Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.str_operation);
	st_path.str_machine   =		_T("c:\\AMT820\\Log\\Machine\\");				// Machine Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.str_machine);
	st_path.str_total     =		_T("c:\\AMT820\\Log\\Total\\");				// Total Log ���Ͽ� ���� ���� ���� 

	Func.CreateFolder(st_path.str_total);

	st_path.str_test_result =	_T("C:\\AMT820\\Log\\TestResult\\");			// Test ��� ���忡 ���� ���� ����
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

	st_path.mstr_basic =			_T(strMainPath + "Setting\\BasicTerm.TXT");			// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_basic);

	st_path.mstr_basic_folder =		_T(strMainPath + "Setting\\");						// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 

	st_path.mstr_path_dvc =		_T(strMainPath + "Device\\");							// ����̽��� ƼĪ ���� ���� ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_dvc);

	st_path.mstr_motor =			_T(strMainPath + "Setting\\AMT820.cme2");

	st_path.mstr_file_basic =		_T("BasicTerm");									// �⺻ ���� ���� ���� ���ϸ� ����
	st_path.mstr_file_wait =		_T("WaitTime");										// ��� �ð� ���� ���� ���ϸ� ����

	CString FolderPath;
	
	st_path.mstr_path_alarm =		_T(strMainPath + "bmp\\");							// ��� �˶� ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_alarm);
		
	st_path.mstr_path_file =		_T(strMainPath);									// ���� ���� ���� ����
	st_path.mstr_path_back =		_T("D:\\BackUp\\");									// BACKUP ���� ���� ���� ����
	
	st_path.mstr_operation =		_T(strMainPath + "Log\\Operation\\");				// Operation Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_operation);
	
	st_path.mstr_interface =		_T(strMainPath + "Log\\Interface\\");				// Interface Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_interface);
	
	st_path.mstr_machine   =		_T(strMainPath + "Log\\Machine\\");					// Machine Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_machine);
	
	st_path.mstr_total     =		_T(strMainPath + "Log\\Total\\");					// Total Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_total);

	st_path.m_strBoardLog = 		_T(strMainPath + "Log\\Board\\");					// Total Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.m_strBoardLog);

	st_path.mstr_ngbuffer  =		_T(strMainPath + "Log\\NGBuffer\\");				// NGBuffer Log ���Ͽ� ���� ���� ���� 2014.12.19 - Bredmin.
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

	if (st_handler.cwnd_basic != NULL)				// BASIC ȭ�� ����
	{
		st_handler.cwnd_basic->PostMessage(WM_BASIC_APPLY, 0, 0);
	}
	else if (st_handler.cwnd_maintenance != NULL)	// Tower Lamp ȭ�� ����
	{
		st_handler.cwnd_maintenance->PostMessage(WM_MAINTENANCE_APPLY, 0, 0);
	}
	else if (st_handler.cwnd_waittime != NULL)		// Wait Time ȭ�� ����
	{
		st_handler.cwnd_waittime->PostMessage(WM_WAITTIME_APPLY, 0, 0);
	}
	else if (st_handler.cwnd_motorspeed != NULL)	// Motor Speed ȭ�� ����
	{
		st_handler.cwnd_motorspeed->PostMessage(WM_MOTORSPEED_APPLY, 0, 0);
	}

	/* ************************************************************************** */
    /* ȭ�鿡 ������ ��� ������ ���Ͽ� �����Ѵ�                                  */
	/* -> ���α׷� ���� �������� ���۷����Ͱ� ������ ���� ���� �����ϴ� ��� ��� */
	/* -> ��� ���� �߿� ���� �����Ǵ��� ���� ���� ���� �����ȴ�                */
    /* ************************************************************************** */
	mcls_m_basic.OnBasic_Data_Save();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�

	mcls_m_basic.OnWaitTime_Data_Save();			// ���� ������ ����� Wait Time ���� ������ ���Ͽ� �����ϴ� �Լ�
	mcls_m_basic.OnMotorSpeed_Set_Data_Save();		// ���� ������ ����� Motor Speed ���� ������ ���Ͽ� �����ϴ� �Լ�.

	mcls_m_basic.On_Teach_Data_Save();
	mcls_m_basic.OnMaintenance_Data_Save();
	mcls_m_basic.OnInterface_Data_Save();
	
	st_handler.n_menu_lock = FALSE;
}

void CMainFrame::OnMain_Thread_Creating()
{
	int i;
	// **************************************************************************
	// ������ ���� �÷��� �ʱ�ȭ                                                 
	// **************************************************************************
	All_Stop = 0 ;  // ������ ���� �÷��� ���� : ������ ���� ����
	
	for (i = 0; i < 25; i++)
	{
		m_thread[i] = NULL;
	}
	// **************************************************************************
	
	// **********************************************************************
	// Ÿ�� ���� ��� ���� ���� ������ �����Ѵ�                              
 	// **********************************************************************
	// 20100805 tae ���� 
	m_thread[THREAD_HANDLER_CHK]=AfxBeginThread(OnThread_Handler_Check, this);			// 
	if (m_thread[THREAD_HANDLER_CHK] != NULL) 	
		hThrHandle[THREAD_HANDLER_CHK] = m_thread[THREAD_HANDLER_CHK]->m_hThread;

	m_thread[THREAD_TOWER_LAMP_CHK]=AfxBeginThread(OnThread_Tower_Lamp_Check, this);	// THREAD_PRIORITY_ABOVE_NORMAL) ;
 	if (m_thread[THREAD_TOWER_LAMP_CHK] != NULL) 	
 		hThrHandle[THREAD_TOWER_LAMP_CHK] = m_thread[THREAD_TOWER_LAMP_CHK]->m_hThread;
 	// **********************************************************************
	
	// **********************************************************************
	// ��� ���� �� �߻��� �˶� ����ϱ� ���� ������ �����Ѵ�                
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
		if (hThrHandle[i])  // ������ ����
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

		if (st_handler.n_alarm_screen != FALSE)  // �˶� ȭ�� ���
			st_handler.n_alarm_screen = FALSE;
	}
}

void CMainFrame::OnDestroy() 
{
	/* ************************************************************************** */
	/* ���� ��޸��� �˶� ȭ�� �������� �ʾҴٸ� �� ���� �۾�                     */
	/* -> mp_modal_dlg : �˶� ȭ�鿡 ���� ������ ���� ����                        */
	/* ************************************************************************** */
	if (mp_alarm_dlg)
	{
		OnAlarm_Destroy();			// �˶� ȭ�� ���� �Լ� */
		delete this;
	}
	/* ************************************************************************** */

	KillTimer(TMR_MAIN_REFRESH);	// Time ��� Ÿ�̸� ����
	KillTimer(TMR_FILE_CREATE);

	CFrameWnd::OnDestroy();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CString str_display_time;
	int n_hour, n_minute, n_second;
	int n_temp;  // �� �ð� [��] ���� ����
	int n_year, n_month, n_day;				// ��, ��, �� ���� ����

	CString mStartTime, mEndTime; 
	char cStartTime[60], cEndTime[60];

	COleDateTime cur_time;
	CTime bin_time;

	if (nIDEvent == TMR_MAIN_REFRESH)
	{
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan diff = cur - st_handler.m_tRef;
		st_handler.m_tRef = cur;

		if (st_handler.n_maint_mode == YES)		// ����Ʈ ����̸� ����Ʈ Ÿ�Ӹ� ������Ʈ �ȴ�.
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
				Dll_TimeDataLog(2, 1);						// Run���� DLL�� �˷��ش�.
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
			
			case dLOTEND:												// Lot End �ÿ��� Stop���� �ð��� �ø���.
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
		/* ���� ���� �� �ʿ��� �ð� �����Ѵ�                                      */
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
		
		//050420 ������ ���Ӹ� ���� Auto�� �߰� 2K5/04/22/ViboX
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
			tmTime -= CTimeSpan(1,0,0,0); //�Ϸ������ͺ��� ã�� ó�� 
			mStartTime = tmTime.Format("%Y%m%d%H%M%S");	//20050419220000

			CTime tsTime(n_year,n_month,n_day,21,59,59);			
			mEndTime = tsTime.Format("%Y%m%d%H%M%S");	//20050420215959

			sprintf(cStartTime, "%s", mStartTime);
			sprintf(cEndTime, "%s", mEndTime);

			Dll_SearchSummaryData(0, "ALLSUM", "0", cStartTime, cEndTime);	//050418 API_** ���� Dll_** �� ���� 
		}
		else if(n_hour < 21 && i_summary_flag == 1)
		{
			i_summary_flag = 0;
		}

		// ����� 3 Shift������ �����ϰ� �� ����. �Դٰ� Fix��. -_-a
		if ((n_hour == 14 || n_hour == 22) && n_minute == 0 && n_second < 2)
		{
			// Socket Yield Reset
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnMainframe_Work(WPARAM wParam, LPARAM lParam)
{	
	int mn_command_num = wParam;  // ��Ʈ��ũ �۾��� �� ���� ����
	
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
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
					}
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Client_[%02d] Connect Fail..", lParam);
						sprintf(st_other.c_abnormal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
					}
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Server_[%02d] Start Fail..", lParam);
						sprintf(st_other.c_abnormal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
				}
			}
			break;
			
		case SERVER_REV:
			str_rev.Format("%s", st_server[lParam].ch_rev);
			
			if(st_handler.cwnd_list != NULL)
			{
				str_tmp.Format("Server_[%02d] - %s Data Receive", lParam, str_rev);
				sprintf(st_other.c_normal_msg, str_tmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
	
	dwCommEvents = m_ports[n_port].MmdSerialGetCommEvents();  // �ø��� �̺�Ʈ ����
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
	
	mstr_received[n_port].Empty();  // ���� �޽��� �ӽ� ���� ���� �ʱ�ȭ
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
	// ������ RS-232C �ø��� ��Ʈ �����Ѵ�                                       
	// -> ������ RS-232C �ø��� ��Ʈ�� �ٸ� ȭ�鿡�� ����ϰ��� �� ��쿡��      
	//    �ݵ�� ��� �� �κ��� ��Ʈ�� �ϴ� ������ �� ����ؾ� �Ѵ�              
	//  : �̹� �ش� RS-232C �ø��� ��Ʈ ���� ���¿��� �ٽ� �� �� ���� �����̴�   
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
			mp_msg_dlg->SetFocus();	// ��ȭ���ڸ� Ȱ��ȭ
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
// RS-232C �ø��� ��Ʈ ���� �Լ�                                                 
// ******************************************************************************
LONG CMainFrame::OnCommunication(WPARAM port, LPARAM ch)
{
	// **************************************************************************
    // RS-232C �ø��� ��Ʈ �˻��Ѵ�                                              
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM ��Ʈ �˻�
		return -1;
	// **************************************************************************
    
	if (ch == -1)  // �б� ����
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[port-1] = _T("[���� ������] �ø��� ��Ʈ �б� ���� �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, RECEIVE_MSG);  // ���� �޽��� ��� ��û
		}

		st_serial.n_rec_chk[port-1] = COM_ERROR;  // ���� ���� �÷��� ����
		return 0;
	}
	else if (ch == -2)  // ���� ����
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[port-1] = _T("[���� ������] �ø��� ��Ʈ ���� ���� �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, SEND_MSG);  // ���� �޽��� ��� ��û
		}

		st_serial.n_rec_chk[port-1] = COM_ERROR;  // ���� ���� �÷��� ����
		return 0;
	}

	if (port == LOT_BARCODE_PORT)
	{
//		OnMain_Lot_Barcode(port, ch);     // ���ŵ� ���ڵ� ������ ������ ó�� �Լ�
	}
	else
	{
//		OnMain_Device_Barcode(port, ch);     // ���ŵ� ���ڵ� ������ ������ ó�� �Լ�
	}


	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C �ø��� ��Ʈ �̺�Ʈ ���� �Լ�                                          
// ******************************************************************************
LONG CMainFrame::OnCommunicationEvent(WPARAM port, LPARAM event)
{
	// **************************************************************************
    // �̺�Ʈ ������ �ø��� ��Ʈ ��Ʈ �˻��Ѵ�                                   
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM ��Ʈ �˻�
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
// RS-232C �ø��� ��Ʈ�� ���� �޽��� ���� �Լ�                                   
// -> wParam : �޽��� ������ ��Ʈ �ѹ�                                           
// ******************************************************************************
LRESULT CMainFrame::OnDataSend(WPARAM wParam, LPARAM lParam)
{
	int nport = wParam;

	if (nport == DEVICE_BARCODE_PORT)
	{
//		OnMain_Send_Barcode(wParam, lParam);     // ���ŵ� ���ڵ� ������ ������ ó�� �Լ�
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
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
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
	// �ʱ� �ӵ��� 2000���� ��´�.
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

	CString str_chk_file;  // ������ [����]+[���ϸ�] ���� ����
	CString str_chk_ext;   // Ȯ���� ���� ����
	int n_pos;

	/* ************************************************************************** */
    /* �Էµ� ����̽��� �����Ѵ�                                                 */
    /* ************************************************************************** */
	m_str_new_device.MakeUpper();
	m_str_new_device.TrimLeft(' ');               
	m_str_new_device.TrimRight(' ');

	if (m_str_new_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
//			st_other.str_op_msg = _T("[DEVICE] A name input error occurrence.");
			st_other.str_op_msg = _T("[DEVICE] ���ϸ� �Է� �����Դϴ�.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return FALSE;
	}
	/* ************************************************************************** */

	str_chk_file = st_path.mstr_basic + m_str_new_device;  // ������ [����]+[���ϸ�] ����
	n_pos = str_chk_file.Find(".");  // Ȯ���� �˻�
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // Ȯ���� ���� ����
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				st_other.str_op_msg = _T("[DEVICE] ���ϸ� �Է� �����Դϴ�..");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			return FALSE;
		}
	}

	m_str_load_file = str_chk_file;  // ������ [����]+[���ϸ�]+[Ȯ����] ����

	return TRUE;
}

void CMainFrame::OnMain_Create_Device()
{
	CDialog_Message msg_dlg;
	CFileFind finder;
	
	if(finder.FindFile(m_str_load_file))
	{
		st_other.str_fallacy_msg = _T("[DEVICE] ���ϸ� �Է� �����Դϴ�. ���� ���ϸ��� �����մϴ�.");
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

	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
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
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
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
