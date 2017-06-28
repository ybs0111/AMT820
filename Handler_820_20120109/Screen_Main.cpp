// Screen_Main.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Screen_Main.h"
//#include "Dialog_Teching_Data.h"

#include "Variable.h"			// 전역 변수 정의 클래스 추가
/* ****************************************************************************** */
/* 정보 출력 대화 상자 및 암호 검사 대화 상자 클래그 추가                         */
/* ****************************************************************************** */
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "DataAPI.h"			// 2004.7.24 ljs dll_ 자료 이종성'
#include "MyJamData.h"  // 알람 출력 정보 로딩 및 알람 메시지 저장 클래스 
#include "Screen_IO_Map.h"

#include "FastechPublic_IO.h"
#include "FAS_HSSI.h"
#include "Dialog_Select_Retest.h"
#include "Dialog_SelectDevice.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "BitmapSlider.h"
#include "KeyboardDll.h"

#include "SrcPart/PartFunction.h"
#include "Dialog_LotStart.h"
#include "AMTLotManager.h"
#include "Dialog_Data_Lot.h"
#include "IO_Manager.h"
#include "Dialog_Pass_Check.h"
#include "IO_Manager.h"
#include "Run_Device_Carrier_Robot.h"
#include "SrcPart/APartHandler.h"
#include "DeviceEditdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main
#define		CUT_Z_READY			0
#define		CUT_Z_START			1
#define		CUT_Z_END			2

#define		TM_TIME				100
IMPLEMENT_DYNCREATE(CScreen_Main, CFormView)

CScreen_Main::CScreen_Main()
	: CFormView(CScreen_Main::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Main)
	//}}AFX_DATA_INIT
	mp_main_font		= NULL;
	mp_main_big_font	= NULL;
	mp_main_font1		= NULL;
	mn_alarm_cnt = 0; //kwlee 2017.0425 test
	m_ncnt = 0;
	m_nDvcCnt =0;
}

CScreen_Main::~CScreen_Main()
{
	if( mp_main_font != NULL )
	{
		delete mp_main_font;
		mp_main_big_font	= NULL;
	}
	if( mp_main_big_font != NULL )
	{
		delete mp_main_big_font;
		mp_main_big_font = NULL;
	}
	if( mp_main_font1 != NULL )
	{
		delete mp_main_font1;
		mp_main_font1 = NULL;
	}

}

void CScreen_Main::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Main)
	DDX_Control(pDX, IDC_GROUP_TOP_INFO, m_group_Top_Info);
	DDX_Control(pDX, IDC_GROUP_HEATSINK, m_group_HeatSink);
	DDX_Control(pDX, IDC_GROUP_DEVICE, m_group_Device);
	DDX_Control(pDX, IDC_GROUP_BTM_INFO, m_group_Btm_Info);
	DDX_Control(pDX, IDC_HARDNESS_OUT, m_btn_hardness_Out);
	DDX_Control(pDX, IDC_BUTTON_TRAY_EDITOR, m_btn_Tray_Editor);
	DDX_Control(pDX, IDC_EPOXY_SAFETY, m_btn_Epoxy_Safety);
	DDX_Control(pDX, IDC_EPOXY_OUT, m_btn_Epoxy_Out);
	DDX_Control(pDX, IDC_EPOXY_OUT2, m_btn_Epoxy_Out2);
	DDX_Control(pDX, IDC_TRAY2_UNLOCK, m_tray2_unlock);
	DDX_Control(pDX, IDC_TRAY1_UNLOCK, m_tray1_unlock);
	DDX_Control(pDX, IDC_TRAY1_LOCK, m_tray1_lock);
	DDX_Control(pDX, IDC_TRAY2_LOCK, m_tray2_lock);
	DDX_Control(pDX, IDC_MSG_ALARM, m_msg_alarm);
	DDX_Control(pDX, IDC_MSG_MTBI, m_msg_mtbi);
	DDX_Control(pDX, IDC_MSG_STOP, m_msg_stop);
	DDX_Control(pDX, IDC_MSG_RUN, m_msg_run);
	DDX_Control(pDX, IDC_DGT_ALARM, m_dgt_alarm);
	DDX_Control(pDX, IDC_DGT_STOP, m_dgt_stop);
	DDX_Control(pDX, IDC_DGT_RUN, m_dgt_run);
	DDX_Control(pDX, IDC_DGT_MTBI, m_dgt_mtbi);
	DDX_Control(pDX, IDC_CHECK_ONLYCARRIER, m_chkOnlyCarrier);
	DDX_Control(pDX, IDC_LABEL_LOAD, m_ld_buffer);
	DDX_Control(pDX, IDC_LABEL_DISP_PLACE, m_disp_place);
	DDX_Control(pDX, IDC_LABEL_DISP_PICK, m_disp_pick);
	DDX_Control(pDX, IDC_LABEL_UNLOAD, m_uld_buffer);
	DDX_Control(pDX, IDC_CHECK1, m_changeTop_Btm);
	DDX_Control(pDX,IDC_CUSTOM_TOP_INFO,m_grid_Info_Top); //kwlee 2017.0428
	DDX_Control(pDX,IDC_CUSTOM_BTM_INFO,m_grid_Info_Btm); //kwlee 2017.0428
	DDX_Control(pDX,IDC_CUSTOM_HEATSINK,m_grid_Info_HeatSink); //kwlee 2017.0428
	DDX_Control(pDX,IDC_CUSTOM_DEVICE,m_grid_Info_Device); //kwlee 2017.0428
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Main, CFormView)
	//{{AFX_MSG_MAP(CScreen_Main)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_LOT_OPEN, OnLotOpen)
	ON_BN_CLICKED(IDC_BTN_DOOR_OPEN, OnBtnDoorOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TRAY2_LOCK, OnTray2Lock)
	ON_BN_CLICKED(IDC_TRAY2_UNLOCK, OnTray2Unlock)
	ON_BN_CLICKED(IDC_TRAY1_LOCK, OnTray1Lock) 
	ON_BN_CLICKED(IDC_TRAY1_UNLOCK, OnTray1Unlock)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_LABEL_UNLOAD, OnLabelUnload)
	ON_BN_CLICKED(IDC_CHECK_ONLYCARRIER, OnCheckOnlycarrier)
	ON_BN_CLICKED(IDC_EPOXY_OUT, OnEpoxyCleanOutPosMove)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_EDITOR, OnButtonTrayEditor)
	ON_BN_CLICKED(IDC_EPOXY_SAFETY, OnEpoxySafety)
	ON_BN_CLICKED(IDC_HARDNESS_OUT, OnHardnessOut)
	ON_BN_CLICKED(IDC_EPOXY_OUT2, OnEpoxyOut2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_RESET, OnButtonCleanReset)
	ON_BN_CLICKED(IDC_BUTTON_HARDNESS_RESET, OnButtonHardnessReset)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WORK_END, OnMain_Work_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
	ON_MESSAGE(SSM_CLICK  , OnCellClick) //kwlee 2017.0425
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main diagnostics

#ifdef _DEBUG
void CScreen_Main::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Main::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main message handlers
void CScreen_Main::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_main_font = NULL;
	mp_main_font = new CFont;
	mp_main_font->CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_main_big_font = NULL;
	mp_main_big_font = new CFont;
	mp_main_big_font->CreateFont(21,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"2002L");

	mp_main_font1 = NULL;
	mp_main_font1 = new CFont;
	mp_main_font1->CreateFont(24,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	mn_time_select = 1;				// 1 : Lot Time, 0 : Total Time...
	mn_discharge_step = 0;
	//kwlee 2017.0508
	m_grid_Device = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	m_grid_HeatSink = (TSpread*)GetDlgItem(IDC_CUSTOM_HEATSINK);
	OnMain_GroupBox_Set();
	OnMain_Button_Set();
	OnMain_Digital_Count_Set();
	OnMain_Label_Set();
	OnMain_Picture_Set();

	OnMain_Init_Lot_Yield_info();
	OnMain_Init_Daily_Yield_info();
	OnMain_Init_Lot_Info();
	OnMain_Init_B_Lot_Info();
	OnMain_Init_Door();


	OnMain_Time_Change(mn_time_select);	// 선택 라디오 버튼 상태 변경 함수

	OnMain_Timeinfo_Display();
	OnMain_Workinfo_Display();			// Work 정보도 Lot별, Day별로 보여준다.
	OnMain_Status_Display();
	
	//kwlee 2017.0412
	OnInitTopGrid();
	OnInitBTMGrid();
	OnInitGridRef();

	//kwlee 2017.0425
	OnInitHeatSinkGrid();
	OnInitDeviceGrid();
	//////////////////////////////////////////////////////////////////////////
	OnMain_Display_Lot_Info();
	OnMain_Display_Lot_Yield_info();
	OnMain_Display_Daily_Yield_info();
	OnMainTop_Loader_Recive_Info();
	OnMainTop_Input_Loader_Info();	
	OnMainTop_Epoxy_Info();	
	OnMainTop_Wait_Index_Info();	
	OnMainTop_Hs_Vision_Info();	
	OnMainTop_OutSend_Info();	
	OnMainTop_Unloader_Info();	
	OnMainBtm_Down_Info();	
	OnMainBtm_DownForward_Info();	
	OnMainBtm_Hs_Down_Info();	
	OnMainBtm_Index_Down_Info();	
	OnMainBtm_Epoxy_Down_Info();	
	OnMainBtm_Input_Down_Info();	
	OnMainBtm_Loader_Down_Info();

	
	//////////////////////////////////////////////////////////////////////////
// 	OnCheckOnlycarrier();


	//////////////////////////////////////////////////////////////////////////
	SetTimer( 3333, 500, NULL);
	SetTimer( TM_TIME, 1000, NULL);
	SetTimer( WM_CDIMM_UI, 1000, NULL);
	SetTimer( WM_MAIN_UI, 3600, NULL);

	st_handler.cwnd_main = this;		// MAIN 화면 핸들 설정
}


/* ****************************************************************************** */
/* 테스트 결과 정보 화면에 출력하기 위한 함수                                     */
/* ****************************************************************************** */

void CScreen_Main::OnDestroy() 
{
	st_handler.cwnd_main = NULL;
	if( mp_main_font != NULL )
	{
		delete mp_main_font;
		mp_main_font = NULL;
	}

	if( mp_main_big_font != NULL )
	{
		delete mp_main_big_font;
		mp_main_big_font = NULL;
	}

	if( mp_main_font1 != NULL )
	{
		delete mp_main_font1;
		mp_main_font1 = NULL;
	}

	KillTimer(WM_CDIMM_UI);
	KillTimer(TM_TIME);
	KillTimer(WM_MAIN_UI);
	KillTimer(3333);

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CScreen_Main::OnMain_GroupBox_Set()
{	
	CSxLogFont main_font(16,FW_SEMIBOLD,false,"MS Sans Serif");
	
	m_group_Btm_Info.SetFont(main_font);
	m_group_Btm_Info.SetCatptionTextColor(BLUE_C);
	m_group_Btm_Info.SetFontBold(TRUE);

	m_group_Top_Info.SetFont(main_font);
	m_group_Top_Info.SetCatptionTextColor(BLUE_C);
	m_group_Top_Info.SetFontBold(TRUE);

	m_group_HeatSink.SetFont(main_font);
	m_group_HeatSink.SetCatptionTextColor(BLUE_C);
	m_group_HeatSink.SetFontBold(TRUE);

	m_group_Device.SetFont(main_font);
	m_group_Device.SetCatptionTextColor(BLUE_C);
	m_group_Device.SetFontBold(TRUE);

}

void CScreen_Main::OnMain_Label_Set()
{
	m_msg_alarm.SetFont(mp_main_font);
	m_msg_alarm.SetWindowText(_T("Alarm"));
	m_msg_alarm.SetCenterText();
	m_msg_alarm.SetColor(RGB(0, 0, 0));
	m_msg_alarm.SetGradientColor(RGB(255, 255, 0));
	m_msg_alarm.SetTextColor(RGB(255, 255, 255));
	
	m_msg_run.SetFont(mp_main_font);
	m_msg_run.SetWindowText(_T("Run"));
	m_msg_run.SetCenterText();
	m_msg_run.SetColor(RGB(0, 0, 0));
	m_msg_run.SetGradientColor(RGB(0, 0, 255));
	m_msg_run.SetTextColor(RGB(255, 255, 255));
	
	m_msg_stop.SetFont(mp_main_font);
	m_msg_stop.SetWindowText(_T("Stop"));
	m_msg_stop.SetCenterText();
	m_msg_stop.SetColor(RGB(0, 0, 0));
	m_msg_stop.SetGradientColor(RGB(255, 0, 0));
	m_msg_stop.SetTextColor(RGB(255, 255, 255));
	
	m_msg_mtbi.SetFont(mp_main_font);
	m_msg_mtbi.SetWindowText(_T("MTBI"));
	m_msg_mtbi.SetCenterText();
	m_msg_mtbi.SetColor(RGB(0, 0, 0));
	m_msg_mtbi.SetGradientColor(ORANGE_C);
	m_msg_mtbi.SetTextColor(RGB(255, 255, 255));
}

void CScreen_Main::OnMain_Picture_Set()
{
	
}



BOOL CScreen_Main::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CScreen_Main::OnMain_Loader_Set()
{
	int iResponse;  // 대화 상자에 대한 리턴 값 저장 변수
	CString str_cur_hour, str_cur_min, str_cur_sec;
	CString str_year, str_month, str_day;	// 문자형 년, 월, 일 저장 변수
	CString str_lot_day, str_lot_time, str_machine_status;
//	char chr_buf[20];

	CDialog_Infor select_dlg;
	CDialog_Message msg_dlg;

	// ================================================================================
	// 로딩 상태에 따라 로딩 화면 출력한다
	// -> st_work.b_load_key_flag : 로딩 작업 ON/OFF 상태 플래그
	// ===============================================================================

	if(st_work.b_load_key_flag != false)
	{
		if(st_work.n_robot_device_loading == TRUE)  //최소 1회이상 device 피킹 동작이 끝났다..
		{
			st_other.str_confirm_msg = _T("Warning : Loading request is canceled");

			iResponse = select_dlg.DoModal();

			if(iResponse == IDOK)
			{
				Func.OnSet_IO_Port_Load_Off();		// 로딩 OFF 시 플래그 설정 및 I/O 출력 함수

				st_work.b_load_key_flag = false;
				st_work.b_load_off_flag = true;
				st_handler.n_load_state = FALSE;

				if(st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					st_other.str_normal_msg = _T("[LOADING] We canceled Loading task.");
					sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
				}
			}
			else if(iResponse == IDCANCEL)
			{
				Func.OnSet_IO_Port_Load_Off();		// 로딩 OFF 시 플래그 설정 및 I/O 출력 함수

				st_handler.n_load_state = FALSE;

				if(st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					st_other.str_normal_msg = _T("[LOADING] Loading off is canceled.");
					sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
				}
			}
		}
		else  //loader off
		{
			Func.OnSet_IO_Port_Load_Off();		// 로딩 OFF 시 플래그 설정 및 I/O 출력 함수

			st_handler.n_load_state = FALSE;


			if(st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				st_other.str_normal_msg = _T("[LOADING OFF] Retry Again.");
				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
		}
	}
	else  // 로딩 진행 중인 경우
	{
		// ================================================================================
		// 현재 LOT END 시점인지 검사한다
		// -> st_work.b_lot_start_flag : LOT 시작 플래그
		// ================================================================================
/*		iResponse = lot_info_dlg.DoModal();

		if(iResponse == IDOK)
		{
//			if(st_work.b_lot_start_flag != false)
			if(st_handler.n_lot_flag != CTL_LOTEND_FINISH)
			{
				st_other.str_fallacy_msg = _T("It is available after Lot End.");
				msg_dlg.DoModal();

				st_handler.n_load_state = FALSE;
				return;
			}
			// ================================================================================

			//////////////////////////////////////////////////////////////////////////
			Func.OnSet_IO_Port_Load_On();   // 로딩 ON 시 플래그 설정 및 I/O 출력 함수

			// ================================================================================
			// -> st_handler.n_load_state : LOT 로딩 대화 상자 출력 플래그
			// -> st_handler.n_lot_start : LOT 시작 플래그
			// ================================================================================
			if(st_work.b_load_off_flag != false)
			{
				st_work.b_load_off_flag = false;
			}
			st_handler.n_load_state = FALSE;

			OnMain_Lot_Info_Init();  // LOT별 테스트 디바이스 갯수 정보 저장 변수 초기화 함수
				
			if(st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNT_INFO, 0);	// Count 정보 갱신.
			}
			// ================================================================================

			// ================================================================================
			//  Lot Name으로 사용할 날짜 정보 설정한다
			// ================================================================================
			COleDateTime cur_time = COleDateTime::GetCurrentTime();
			iYear = cur_time.GetYear();  
			iMonth = cur_time.GetMonth();  
			iDay = cur_time.GetDay();  

			str_year = LPCTSTR(_itoa( iYear, chr_buf, 10 ));  
			str_month.Format("%02d", iMonth);
			str_day.Format("%02d", iDay);

			str_lot_day = str_year;
			str_lot_day += str_month;
			str_lot_day += str_day;
			// ================================================================================

			// ================================================================================
			// 현재 시간 정보 얻는다
			// ================================================================================
			str_cur_hour.Format("%02d",cur_time.GetHour());		// 현재 시간 정보를 설정한다.
			str_cur_min.Format("%02d",cur_time.GetMinute());	// 현재 분 정보를 설정한다. 
			str_cur_sec.Format("%02d",cur_time.GetSecond());	// 현재 초 정보를 설정한다. 

			str_lot_time = str_cur_hour;
			str_lot_time += str_cur_min;
			str_lot_time += str_cur_sec;
			// ================================================================================

			if(st_basic.n_mode_interface == TRUE)
			{
				str_machine_status = "";	//online은 찍지 말자
			}
			else
			{
				str_machine_status = "_OFF";
			}

			Func.OnLotDataTempLoggin(0);	//초기 lot정보를 기록한다.. 0:모두초기화(lot파일명,시작시간은 들어감), 1:product,lot number저장, 2:모두저장(실질적인 lotend시점), 3:모두저장(이전lotend가 안되서 임시종료시점)

			//만약 offline으로 작업자가 양산하는 경우를 대비하기 위함..

			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_safety) == IO_ON)
			{
				g_ioMgr.set_out_bit(st_io.o_unload_safety, IO_OFF);
			}
			
			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_ready) == IO_ON)
			{
				g_ioMgr.set_out_bit(st_io.o_unload_ready, IO_OFF);
			}
			
			if(st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				st_other.str_normal_msg = _T("Lot Start.");
				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
		}
		else
		{
			st_handler.n_load_state = FALSE;

			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_safety) == IO_ON)
			{
				g_ioMgr.set_out_bit(st_io.o_unload_safety, IO_OFF);
			}
			
			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_ready) == IO_ON)
			{
				g_ioMgr.set_out_bit(st_io.o_unload_ready, IO_OFF);
			}
		}
*/
	}
}

void CScreen_Main::OnMain_Status_Display()
{
	OnMain_Count();

	OnMain_Time_Display(TRUE);

	if (st_handler.cwnd_title != NULL)
	{
		//st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic.n_conveyor_move_mode);		// HeatSink Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, st_basic.n_mode_device);			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FILE_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODEL_MODE, 0);
		//kwlee 2017.0406
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODE_7387, st_basic.n_mode_7387);			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODE_3874, st_basic.n_3874InspectMode);			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODE_BCR, st_basic.n_mode_bcr);			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, NEXTMACHINE_MODE, g_ioMgr.get_in_bit(st_io.i_Interface_Input_1, IO_ON));
	}
}



void CScreen_Main::OnMain_Time_Display(int iFlag)
{
	int i_state;

	m_dgt_run.ActivateTimer(FALSE);
	m_dgt_stop.ActivateTimer(FALSE);
	m_dgt_alarm.ActivateTimer(FALSE);
//	m_dgt_maint.ActivateTimer(FALSE);

	if(st_handler.n_maint_mode == YES)
	{
		i_state = dMAINT;
	}
	else
	{
		i_state = st_work.mn_run_status;
	}

	switch(i_state)	{
		case dMAINT:
//			m_dgt_maint_time.ActivateTimer();
//			m_dgt_maint_time.SetTime(st_handler.m_tM.GetHours(), st_handler.m_tM.GetMinutes(), st_handler.m_tM.GetSeconds());
			break;

		case dLOCK:
		case dSTOP:
		case dREINSTATE:
			m_dgt_stop.ActivateTimer();
			m_dgt_stop.SetTime(st_handler.m_tS.GetHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
			break;
		
		case dRUN:
			m_dgt_run.ActivateTimer();
			m_dgt_run.SetTime(st_handler.m_tR.GetHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
			break;

		case dWARNING:
		case dJAM:
			m_dgt_alarm.ActivateTimer();
			m_dgt_alarm.SetTime(st_handler.m_tJ.GetHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
			break;	
	}

	
	if(iFlag == TRUE) //현재까지의 시간을 모두 출력 해주자..
	{
		m_dgt_stop.SetTime(st_handler.m_tS.GetHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		m_dgt_run.SetTime(st_handler.m_tR.GetHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tJ.GetHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
	}

	m_dgt_run.UpdateTimer();
	m_dgt_stop.UpdateTimer();
	m_dgt_alarm.UpdateTimer();
}

void CScreen_Main::OnMain_Time_Change(int n_state)
{
	switch(n_state)
	{
	case 0:		// Daily Total info
//		m_radio_time_total.Depress(true);
//		m_radio_time_lot.Depress(false);
		break;

	case 1:		// Lot info
//		m_radio_time_total.Depress(false);
//		m_radio_time_lot.Depress(true);
		break;

	default:
		break;
	}

	OnMain_Timeinfo_Display();
	OnMain_Workinfo_Display();
}

void CScreen_Main::OnMain_Timeinfo_Display()
{
	m_dgt_run.ActivateTimer(FALSE);
	m_dgt_stop.ActivateTimer(FALSE);
	m_dgt_alarm.ActivateTimer(FALSE);
	m_dgt_mtbi.ActivateTimer(FALSE);
	
	
	if (mn_time_select == 0)
	{
		m_dgt_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
		m_dgt_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
		
		int nSec = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();
		if( g_handler.GetAlarmCnt() == 0 )
			nSec = nSec;
		else
		{
			nSec /= g_handler.GetAlarmCnt();
		}
		
		m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
	}
	else
	{
		m_dgt_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
		
		int nSec = st_handler.m_tR.GetTotalHours() * 3600 + st_handler.m_tR.GetMinutes() * 60 + st_handler.m_tR.GetSeconds();
		if( g_handler.GetAlarmCnt() == 0 )
			nSec = nSec;
		else
		{
			nSec /= g_handler.GetAlarmCnt();
		}
		
		m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
	}
	
	m_dgt_run.UpdateTimer();
	m_dgt_stop.UpdateTimer();
	m_dgt_alarm.UpdateTimer();
	m_dgt_mtbi.UpdateTimer();
}


void CScreen_Main::OnMain_Workinfo_Display()
{
	switch (mn_time_select)
	{
	case 0:		// Total
//		m_dgt_total_load_cnt.SetValue(st_work.n_total_day_load);
//		m_dgt_total_unload_cnt.SetValue(st_work.n_total_day_unload);

//		m_dgt_alarm_cnt.SetValue(mn_alarm_day_cnt);
		break;

	case 1:		// Lot
//		m_dgt_total_load_cnt.SetValue(st_work.n_total_load_cnt);
//		m_dgt_total_unload_cnt.SetValue(st_work.n_total_unload_cnt);

//		m_dgt_alarm_cnt.SetValue(mn_alarm_cnt);
		break;
	}
}


void CScreen_Main::OnTimer(UINT nIDEvent) 
{
	int nRet_1 = 0;

	if( nIDEvent == 3333 )
	{
		OnMain_Display_Door();		
	}
	else if(nIDEvent == TM_TIME)
	{
		OnMain_Time_Display( TRUE );
	}
	else if( nIDEvent == WM_CDIMM_UI )
	{
		//kwlee 2017.0425
		//HeatSink
		if (st_picker[THD_HEATSINK_RBT].st_pcb_info[0].nYesNo == CTL_YES)
		{
			//GridColor(IDC_CUSTOM_HEATSINK, 2, 1, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 0, BLUE);
		}
		else
		{
			//GridColor( IDC_CUSTOM_HEATSINK, 2, 1, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 0, GRAY);
		}
		
		if( st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES )
		{
			//GridColor( IDC_CUSTOM_HEATSINK, 2, 2, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 1, BLUE);
		}
		else
		{
			//GridColor( IDC_CUSTOM_HEATSINK, 2, 2, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 1, GRAY);
		}

		if( st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES )
		{
			//GridColor( IDC_CUSTOM_HEATSINK, 2, 3, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 2, BLUE);
		}
		else
		{
			//GridColor( IDC_CUSTOM_HEATSINK, 2, 3, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 2, GRAY);
		}


		if (st_picker[THD_HEATSINK_PRBT].st_pcb_info[0].nYesNo == CTL_YES)
		{
			//GridColor( IDC_CUSTOM_HEATSINK, 2, 4, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 3, BLUE);
		}
		else
		{
		//	GridColor( IDC_CUSTOM_HEATSINK, 2, 4, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_HeatSink.SetItemBkColour(0, 3, GRAY);
		}
		////////////

		if( st_picker[THD_LOAD_WORK_RBT].st_pcb_info[0].nYesNo == CTL_YES )
		{
			//GridColor( IDC_CUSTOM_DEVICE, 2,1, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_Device.SetItemBkColour(0, 0, BLUE);
		}
		else
		{

			//GridColor( IDC_CUSTOM_DEVICE, 2,1, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_Device.SetItemBkColour(0, 0, GRAY);
		}

		if( st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].nYesNo == CTL_YES )
		{
			//GridColor( IDC_CUSTOM_DEVICE, 2,2, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_Device.SetItemBkColour(0, 1, BLUE);
		}
		else
		{
			//GridColor( IDC_CUSTOM_DEVICE, 2,2, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_Device.SetItemBkColour(0, 1, GRAY);
		}

		if( st_buffer_info[THD_ULD_ALIGN_BUFF].st_pcb_info[0].nYesNo == CTL_YES )
		{
			//GridColor( IDC_CUSTOM_DEVICE, 2,3, BLUE, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_Device.SetItemBkColour(0, 2, BLUE);
		}
		else
		{
			//GridColor( IDC_CUSTOM_DEVICE, 2,3, GRAY, WHITE_C );
			//kwlee 2017.0508
			m_grid_Info_Device.SetItemBkColour(0, 2, GRAY);
		}
	}
	else if( nIDEvent == WM_EPOXY_MOVE_CLEAN )
	{
		nRet_1 = g_handler.MoveCleanEpoxy();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
		}
	}
	else if( nIDEvent == WM_EPOXY_MOVE_SAFETY)
	{
		nRet_1 = g_handler.MoveSafetyEpoxy();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
		}
	}
	else if( nIDEvent == WM_HARDNEDD_OUT )
	{
		nRet_1 = g_handler.MoveHardnessOut();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
		}
	}
	else if( nIDEvent == WM_EPOXY_OUT )
	{
		nRet_1 = g_handler.MoveEpoxyOut();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
		}
	}
	else if ( nIDEvent == WM_MAIN_UI )
	{
		if( g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Lock_Check, IO_ON ) == IO_ON &&
			g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Unlock_Check, IO_OFF ) == IO_OFF )
		{
			
		}
	}
	Invalidate(FALSE);
	CFormView::OnTimer(nIDEvent);
}

/* ****************************************************************************** */
/* 테스트 결과 정보 화면에 출력하기 위한 함수                                     */
/* ****************************************************************************** */
LRESULT CScreen_Main::OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam)   
{
	

	switch(wParam)
	{
		case MAIN_TIMEINFO:
			OnMain_Time_Display(lParam);
			break;

		case MAIN_COUNTINFO:
			OnMain_Display_Lot_Yield_info();
			break;

		case MAIN_LOTINFO:
			OnMain_Display_Lot_Info();
			break;

		//kwlee 2017.0416
		case TOPSHIFT_BUFF_LOADER_RECEIVE:
			OnMainTop_Loader_Recive_Info();
			break;

		case TOPSHIFT_BUFF_INPUT_LOADER:
			OnMainTop_Input_Loader_Info();
			break;

		case TOPSHIFT_BUFF_EPOXY:
			OnMainTop_Epoxy_Info();
			break;

		case TOPSHIFT_BUFF_WAIT_INDEX:
			OnMainTop_Wait_Index_Info();
			break;

		case TOPSHIFT_BUFF_HEATSINK_VISION:
			OnMainTop_Hs_Vision_Info();
			break;

		case TOPSHIFT_BUFF_OUTSEND:
			OnMainTop_OutSend_Info();
			break;

		case TOPSHIFT_BUFF_UNLOADER:
			OnMainTop_Unloader_Info();
			break;
			
			//kwlee 2017.0416
		case BTMSHIFT_BUFF_DOWN:
			OnMainBtm_Down_Info();
			break;
			
		case BTMSHIFT_BUFF_DOWNFORWARD:
			OnMainBtm_DownForward_Info();
			break;
			
		case BTMSHIFT_BUFF_HEATSINK_DOWN:
			OnMainBtm_Hs_Down_Info();
			break;
			
		case BTMSHIFT_BUFF_INDEX_DOWN:
			OnMainBtm_Index_Down_Info();
			break;
			
		case BTMSHIFT_BUFF_EPOXY_DOWN:
			OnMainBtm_Epoxy_Down_Info();
			break;
			
		case BTMSHIFT_BUFF_INPUT_DOWN:
			OnMainBtm_Input_Down_Info();
			break;
			
		case BTMSHIFT_BUFF_LOADER_DOWN:
			OnMainBtm_Loader_Down_Info();
			break;
			////
	}
	return 0 ;
}

void CScreen_Main::OnMainWork_Loader_Set()
{
/*	int iResponse, i;  // 대화 상자에 대한 리턴 값 저장 변수
int iYear, iMonth, iDay;				// 년, 월, 일 저장 변수
CString str_cur_hour, str_cur_min, str_cur_sec;
CString str_year, str_month, str_day;	// 문자형 년, 월, 일 저장 변수
CString str_lot_day, str_lot_time, str_machine_status;
char chr_buf[20];

  CDialog_Infor select_dlg;
	CDialog_Message msg_dlg;*/
	//	CDialog_Retest_Set retest_dlg;
}

void CScreen_Main::OnMain_Count()
{

}

void CScreen_Main::OnMain_Loader_Display()
{

}

void CScreen_Main::OnMain_Button_Set()
{
	short shBtnColor = 30;
	
	//m_btn_run.SetFont(mp_main_font);
	//m_btn_run.SetIcon(IDI_LOT_START);
	//m_btn_run.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	//m_btn_run.SetButtonColor(1);
}

// #include "Dialog_Lot_End.h"
void CScreen_Main::OnLotOpen() 
{
	CDialog_Data_Lot dlg;
	dlg.DoModal();

// 	CDialog_Lot_End dlg;
// 	dlg.DoModal();
}
//kwlee 2017.0414
void CScreen_Main::OnMainTop_Loader_Recive_Info()
{
// 	SS_CELLTYPE CellType;
// 
 	CString strTemp[4];
 	int nTmp[4] = {0,};
 	CString sTmp="";
 
// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	
// // 	m_grid_Top->SetTypeStaticText(&CellType, SS_TEXT_LEFT | SS_TEXT_VCENTER | SS_TEXT_WORDWRAP);
// 
// 	for (int i = 0; i<3; i++)
// 	{
// // 		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr: %s Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[i]);
// 		strTemp[i] += "\n\n";
// 		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[i]); 
// 		strTemp[i] += sTmp;
// 
// 		
// 
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 7, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 
// 		if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == CTL_YES)
// 		{
// 
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
// 			}
// 		}
// 
// 	}
// 
// 	Invalidate(FALSE);

	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{
	 // 		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i],
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[i]);
// 	 	strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[i]); 
// 	 	sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2]); 
// 	 	strTemp[i] += sTmp;
	 	
		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;
		///////
		m_grid_Info_Top.SetItemBkColour(i + 1, 6,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i+1, 6,WHITE_C);

	 	if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == CTL_YES)
		{
	 		if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_CDIMM)
	 		{

				m_grid_Info_Top.SetItemBkColour(i + 1, 6,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
 			}
 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_EPOXY)
 			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 6,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1, 6,BLACK_L);
 			}
 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_HEATSINK)
 			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 6,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
 			}
 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_VISION)
 			{
				//2017.0617
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][0];
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] <=0 ) st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][0] = GetCurrentTime();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_grid_Info_Top.SetItemBkColour(i + 1, 6,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
 			}
 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_GOOD)
 			{
				//2017.0617
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][0];
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] <=0 ) st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][0] = GetCurrentTime();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_grid_Info_Top.SetItemBkColour(i + 1, 6,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1, 6,BLACK_L);
 			}
 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_FAIL)
 			{

				m_grid_Info_Top.SetItemBkColour(i + 1, 6,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
 			}
 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_NONE)
 			{

				m_grid_Info_Top.SetItemBkColour(i + 1, 6,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 6,strTemp[i]);
 			}
 		} 
 	}
}

void CScreen_Main::OnMainTop_Input_Loader_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
 	CString sTmp="";
// 
// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// 		// 		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 6, GRAY, WHITE_C);	
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
// 
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER ].n_exist[i] == CTL_YES)
// 		{
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2,6 ,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
// 			}
// 		}
// 
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{
		
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_chBarcode[i],
// 		st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;
	
		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Top.SetItemBkColour(i + 1, 5,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i+1,5,WHITE_C);

		if(st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_CDIMM)
			{
				
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_EPOXY)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i + 1, 5,BLACK_L);

			}

			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_HEATSINK)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_VISION)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_GOOD)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,5,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_FAIL)
			{
				
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_NONE)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 5,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 5,strTemp[i]);
			}
		} 
 	}
}

void CScreen_Main::OnMainTop_Epoxy_Info()
{

	CString strTemp[4];
	int nTmp[4] = {0,};
 	CString sTmp="";

// 	CString strTemp[4];
// 	int nTmp[4] = {0,};
// 
// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// // 		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 5, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
// 
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[i] == CTL_YES )
// 		{
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5 ,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
// 			}
// 		}
// 	
// 	}
// 	Invalidate(FALSE);

	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Top.SetItemBkColour(i + 1, 4,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i+1, 4,WHITE_C);
		
		if(st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_CDIMM)
			{
				
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_EPOXY)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1, 4,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_HEATSINK)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_VISION)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_GOOD)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,4,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_FAIL)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_NONE)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 4,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 4,strTemp[i]);
			}
		} 
 	}
}
void CScreen_Main::OnMainTop_Wait_Index_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
 	CString sTmp="";

// 	CString strTemp[4];
// 	int nTmp[4] = {0,};
// 
// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// 		
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 4, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
// 
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX ].n_exist[i] == CTL_YES)
// 		{
// 
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4 ,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
// 			}
// 		}
// 			
// 		
// 	}
// 
// 	Invalidate(FALSE);

	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_chBarcode[i],
// 		st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;
		
		m_grid_Info_Top.SetItemBkColour(i + 1, 3,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i+1,3,WHITE_C);
		
		if(st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_CDIMM)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_EPOXY)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,3,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_HEATSINK)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_VISION)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_GOOD)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,3,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_FAIL)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_NONE)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 3,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 3,strTemp[i]);
			}
		} 
 	}
}
void CScreen_Main::OnMainTop_Hs_Vision_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
 	CString sTmp="";

// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 3, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
// 
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION ].n_exist[i] == CTL_YES )
// 		{
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3 ,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
// 			}
// 		}
// 			
// 		
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;
		
		m_grid_Info_Top.SetItemBkColour(i + 1, 2,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i+1,2,WHITE_C);

		if(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_CDIMM)
			{
				
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_EPOXY)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,2,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_HEATSINK)
			{
				//2017.0617
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][0];
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2] <=0 ) st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][0] = GetCurrentTime();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_VISION)
			{
				//2017.0617
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][0];
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][2] <=0 ) st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].dwBdTime[i][0] = GetCurrentTime();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_GOOD)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,1,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_FAIL)
			{
				
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_NONE)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 2,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 2,strTemp[i]);
			}
		} 
 	}
}

void CScreen_Main::OnMainTop_OutSend_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp="";
// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 
// 	for (int i = 0; i<3; i++)
// 	{
// 		
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 2, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);	
// 		
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND ].n_exist[i] == CTL_YES )
// 		{
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2 ,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
// 			}
// 		}
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;
		
		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;
		/////

		m_grid_Info_Top.SetItemBkColour(i + 1, 1,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i+1,1,WHITE_C);

		if(st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_EPOXY)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,1,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_HEATSINK)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_VISION)
			{
				//2017.0617
				st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][0];
				st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][2] <=0 ) st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].dwBdTime[i][0] = GetCurrentTime();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_GOOD)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,1,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_FAIL)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_NONE)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 1,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 1,strTemp[i]);
			}
		} 
 	}
}

void CScreen_Main::OnMainTop_Unloader_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp="";
// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	for (int i = 0; i<3; i++)
// 	{	
// 		
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_chBarcode[i],
// 			st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Top, i+2, 1, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER ].n_exist[i] == CTL_YES)
// 		{
// 			if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_CDIMM)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, BLUE, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1 ,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_EPOXY)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, NOR_L, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_HEATSINK)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, BLUE_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_VISION)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, GREEN_D, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);	
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_GOOD)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, OK_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_FAIL)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, NG_C, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 			}
// 			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_NONE)
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, GRAY, YELLOW_C);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 			}
// 			else
// 			{
// 				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
// 				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, DVC_UNLOAD_C, BLACK);
// 				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
// 			}
// 		}
// 
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_chBarcode[i],
// 		st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		///kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_lot_id[i],(int)st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Top.SetItemBkColour(i + 1, 0,GRAY);
		m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
		m_grid_Info_Top.SetItemFgColour(i +1, 0,WHITE_C);

		if(st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,BLUE);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_EPOXY)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,NOR_L);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,0,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_HEATSINK)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,BLUE_D);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_VISION)
			{
				//2017.0617
				st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][0];
				st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][2] <=0 ) st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].dwBdTime[i][0] = GetCurrentTime();
				///////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,GREEN_D);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_GOOD)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,OK_C);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,0,BLACK_L);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_FAIL)
			{	
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,NG_C);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_NONE)
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,GRAY);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				
			}
			else
			{
				m_grid_Info_Top.SetItemBkColour(i + 1, 0,DVC_UNLOAD_C);
				m_grid_Info_Top.SetItemText(i + 1, 0,strTemp[i]);
				m_grid_Info_Top.SetItemFgColour(i+1,0,BLACK_L);
 			}
		} 
	}
}
//////


//kwlee 2017.0416
void CScreen_Main::OnMainBtm_Down_Info()
{
 	CString strTemp[4];
 	int nTmp[4] = {0,};
	CString sTmp="";
	
// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 
// 	for (int i = 0; i<3; i++)
// 	{
// 		
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 1, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 1, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 1,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[i] == CTL_YES) 		
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 1, "MS Sans Serif", 10);
//  			m_pGrid.GridCellColor(m_grid_Btm, i+2, 1, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 1,strTemp[i]);
// 		}	
// 	}
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		
		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Btm.SetItemBkColour(i + 1, 0,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 0,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,0,WHITE_C);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 0,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 0,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,0,BLACK_L);
			}//2017.0617
			else if (st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[i] == BIN_VISION)
			{
				
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][0];
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].dwBdTime[i][0] = GetCurrentTime();
				m_grid_Info_Btm.SetItemBkColour(i + 1, 0,GREEN_D);
				m_grid_Info_Btm.SetItemText(i + 1, 0,strTemp[i]);
			}		
		} 
	}
}

void CScreen_Main::OnMainBtm_DownForward_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp = "";
// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// 		
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 2, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 2, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 2,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_exist[i] == CTL_YES)
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 2, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 2, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 2,strTemp[i]);
// 		}
// 	}
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;
		////

		m_grid_Info_Btm.SetItemBkColour(i + 1, 1,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 1,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,1,WHITE_C);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 1,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 1,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,1,BLACK_L);
			}
		}//2017.0617
		else if (st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBin[i] == BIN_VISION)
		{			
			st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][1] = GetCurrentTime();
			st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][0];
			st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][2] /= 1000;
			if( st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].dwBdTime[i][0] = GetCurrentTime();
			m_grid_Info_Btm.SetItemBkColour(i + 1, 1,GREEN_D);
			m_grid_Info_Btm.SetItemText(i + 1, 1,strTemp[i]);
		} 
	}
}

void CScreen_Main::OnMainBtm_Hs_Down_Info()
{	
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp = "";
// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 3, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 3, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 3,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_exist[i] == CTL_YES)
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 3, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 3, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 3,strTemp[i]);
// 		}
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Btm.SetItemBkColour(i + 1, 2,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 2,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,2,WHITE_C);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 2,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 2,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,2,BLACK_L);
			}//2017.0617
			else if (st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBin[i] == BIN_VISION)
			{
				
				st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][0];
				st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].dwBdTime[i][0] = GetCurrentTime();
				m_grid_Info_Btm.SetItemBkColour(i + 1, 2,GREEN_D);
				m_grid_Info_Btm.SetItemText(i + 1, 2,strTemp[i]);
			}
		} 
	}
}

void CScreen_Main::OnMainBtm_Index_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp = "";
	
	
// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// 
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 4, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 4, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 4,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_exist[i] == CTL_YES)
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 4, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 4, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 4,strTemp[i]);
// 		}
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Btm.SetItemBkColour(i + 1, 3,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 3,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,3,WHITE_C);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 3,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 3,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,3,BLACK_L);
			}//2017.0617
			else if (st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBin[i] == BIN_VISION)
			{
				
				st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][0];
				st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].dwBdTime[i][0] = GetCurrentTime();
				m_grid_Info_Btm.SetItemBkColour(i + 1, 3,GREEN_D);
				m_grid_Info_Btm.SetItemText(i + 1, 3,strTemp[i]);
			}
		} 
	}
}

void CScreen_Main::OnMainBtm_Epoxy_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp = "";

// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 5, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 5, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 5,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_exist[i] == CTL_YES)
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 5, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 5, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 5,strTemp[i]);
// 		}
// 	}
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		
		m_grid_Info_Btm.SetItemBkColour(i + 1, 4,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 4,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,4,WHITE_C);
		
		if(st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 4,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 4,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,4,BLACK_L);
			}//2017.0617
			else if (st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBin[i] == BIN_VISION)
			{
				
				st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][0];
				st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].dwBdTime[i][0] = GetCurrentTime();
				m_grid_Info_Btm.SetItemBkColour(i + 1, 4,GREEN_D);
				m_grid_Info_Btm.SetItemText(i + 1, 4,strTemp[i]);
			}
		} 
	}
}

void CScreen_Main::OnMainBtm_Input_Down_Info()
{
	
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp = "";

// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 6, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 6, GRAY, WHITE_C);	
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 6,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_exist[i] == CTL_YES)
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 6, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 6, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 6,strTemp[i]);
// 		}
// 	}
// 	Invalidate(FALSE);
	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;
		

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;
		
		m_grid_Info_Btm.SetItemBkColour(i + 1, 5,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 5,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,5,WHITE_C);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 5,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 5,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,5,BLACK_L);
			}//2017.0617
			else if (st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBin[i] == BIN_VISION)
			{
				
				st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][0];
				st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].dwBdTime[i][0] = GetCurrentTime();
				m_grid_Info_Btm.SetItemBkColour(i + 1, 5,GREEN_D);
				m_grid_Info_Btm.SetItemText(i + 1, 5,strTemp[i]);
			}
		} 
	}
}

void CScreen_Main::OnMainBtm_Loader_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	CString sTmp = "";
// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	for (int i = 0; i<3; i++)
// 	{
// 		
// //		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_chBarcode[i]);
// 		strTemp[i].Format("Bcr : %s Dvc : %d",st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBinNum[i]);
// 		m_pGrid.GridCellFont(m_grid_Btm, i+2, 7, "MS Sans Serif", 10);
// 		m_pGrid.GridCellColor(m_grid_Btm, i+2, 7, GRAY, WHITE_C);
// 		m_pGrid.GridCellText(m_grid_Btm, i+2, 7,strTemp[i]);
// 
// 		if(st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_exist[i] == CTL_YES)
// 		{
// 			m_pGrid.GridCellFont(m_grid_Btm, i+2, 7, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 7, DVC_UNLOAD_C, BLACK);
// 			m_pGrid.GridCellText(m_grid_Btm, i+2, 7,strTemp[i]);
// 		}
// 	}
// 
// 	Invalidate(FALSE);

	//kwlee 2017.0428
	for (int i = 0; i<3; i++)
	{	
// 		strTemp[i].Format("Bcr: %s \t Dvc: %d ",st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_chBarcode[i],
// 			st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBinNum[i]);
// 		strTemp[i] += "\n";
// 		//kwlee 2017.0428
// 		//sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_lot_id[i]); 
// 		sTmp.Format("Lot: %s \t Time: %d", st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_lot_id[i],(int)st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][2]); 
// 		strTemp[i] += sTmp;

		//kwlee 2017.0614
		sTmp.Format("Bcr: %s",st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_chBarcode[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		sTmp.Format("Dvc: %d",st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBinNum[i]);
		sTmp += "\n";
		strTemp[i] += sTmp;
		
		
		sTmp.Format("Lot: %s", st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_lot_id[i]); 
		sTmp += "\n";
		strTemp[i] +=sTmp;
		
		sTmp.Format("Time: %d", (int)st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][2]); 
		strTemp[i] +=sTmp;

		m_grid_Info_Btm.SetItemBkColour(i + 1, 6,GRAY);
		m_grid_Info_Btm.SetItemText(i + 1, 6,strTemp[i]);
		m_grid_Info_Btm.SetItemFgColour(i+1,6,WHITE_C);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBin[i] == BIN_CDIMM)
			{	
				m_grid_Info_Btm.SetItemBkColour(i + 1, 6,DVC_UNLOAD_C);
				m_grid_Info_Btm.SetItemText(i + 1, 6,strTemp[i]);
				m_grid_Info_Btm.SetItemFgColour(i+1,6,BLACK_L);
			}//2017.0617
			else if (st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBin[i] == BIN_VISION)
			{
				
				st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][1] = GetCurrentTime();
				st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][2] = st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][1] - st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][0];
				st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][2] /= 1000;
				if( st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][2] <=0 ) st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].dwBdTime[i][0] = GetCurrentTime();
				m_grid_Info_Btm.SetItemBkColour(i + 1, 6,GREEN_D);
				m_grid_Info_Btm.SetItemText(i + 1, 6,strTemp[i]);
			}
		} 
	}
}
////////
void CScreen_Main::OnMain_Digital_Count_Set()
{
	m_dgt_alarm.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_run.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_stop.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_mtbi.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
}

void CScreen_Main::OnMain_Init_Lot_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 2;
	int i, j;
	
	//	CString str_tmp[4] = {"Lot No", "Part No", "Last Mdl", "ByPass"};
	//2013,0910
	CString str_tmp[5] = {"Lot No", "Part No", "Last Mdl", "Lot Tot", "ByPass"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 50);
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_LOT_INFO, i, j, 14);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 105);
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 0);
				
				if (i == 2)
				{
					GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);//10);
				}
				else
				{
					GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);
				}
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_LOT_INFO, i, 1, str_tmp[i - 1]);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Init_Daily_Yield_info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 2;
	int i, j;
	
	CString str_tmp[4] = {"Input", "Pass", "Fail", "Yield"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DAILY_YIELD_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 60);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 94);
			}
			
			if (i == 1 && j == 1)
			{
				GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, BLACK_C, GREEN_B);
				}
			}
			GridControl(IDC_CUSTOM_DAILY_YIELD_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, 15);
		}
	}
	
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 1, " ");
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 2, "Prime");
	
	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_DAILY_YIELD_INFO, i, 1, str_tmp[i - 2]);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Init_Lot_Yield_info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 2;
	int i, j;
	
	CString str_tmp[4] = {"Input", "Pass", "Fail", "Yield"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_YIELD_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 60);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 94);
			}
			
			if (i == 1 && j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, BLACK_C, GREEN_B);
				}
			}
			
			GridControl(IDC_CUSTOM_LOT_YIELD_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_LOT_YIELD_INFO, i, j, 15);
		}
	}
	
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 1, " ");
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 2, "Prime");
// 	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 3, "Cum");
	
	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_LOT_YIELD_INFO, i, 1, str_tmp[i - 2]);
	}
	Grid = NULL;
	delete Grid;
}


void CScreen_Main::OnMain_Init_Door()
{
	TSpread* Grid;
	int row = 6;
	int col = 2;
	
	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_DOOR );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for( int ih=0; ih<row; ih++ )
	{
		Grid -> SetRowHeightInPixels(ih + 1, 22);
		
		for( int iw = 0; iw<col; iw++ )
		{
			Grid -> SetColWidthInPixels(iw + 1, 75 );
			
			GridFont( IDC_CUSTOM_DOOR, ih + 1, iw + 1, 15);
			GridControl(IDC_CUSTOM_DOOR, STATIC, ih + 1, iw + 1, 0);
		}
	}
	
	GridData( IDC_CUSTOM_DOOR, 1, 1, "DOOR 10" );
	GridData( IDC_CUSTOM_DOOR, 1, 2, "DOOR 9" );
	GridData( IDC_CUSTOM_DOOR, 2, 1, "DOOR 8" );
	GridData( IDC_CUSTOM_DOOR, 2, 2, "DOOR 7" );	
	GridData( IDC_CUSTOM_DOOR, 3, 1, "DOOR 6" );
	GridData( IDC_CUSTOM_DOOR, 3, 2, "DOOR 5" );
	GridData( IDC_CUSTOM_DOOR, 4, 1, "DOOR 4" );
	GridData( IDC_CUSTOM_DOOR, 4, 2, "DOOR 3" );
	GridData( IDC_CUSTOM_DOOR, 5, 1, "DOOR 2" );
	GridData( IDC_CUSTOM_DOOR, 5, 2, "DOOR 1" );
	GridData( IDC_CUSTOM_DOOR, 6, 1, "Tray1 Lock" );
	GridData( IDC_CUSTOM_DOOR, 6, 2, "Tray2 Lock" );
	
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Display_Door()
{
	COLORREF col = RED_C;
	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck10 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 1, 1, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck9 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 1, 2, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck8 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 2, 1, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck7 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 2, 2, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck6 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 3, 1, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck5 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 3, 2, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck4 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 4, 1, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck3 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 4, 2, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck2 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 5, 1, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_DoorCheck1 ) == IO_ON )	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 5, 2, col, BLACK_C );

	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Lock_Check ) == IO_ON &&
						g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Unlock_Check ) == IO_OFF)	   col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 6, 1, col, BLACK_C );
	
	col = RED_C;	if( g_ioMgr.get_in_bit( st_io.i_Unloading_Stacker_Tray_Lock_Check ) == IO_ON &&
						g_ioMgr.get_in_bit( st_io.i_Unloading_Stacker_Tray_Unlock_Check ) == IO_OFF)	col = BLUE_L;
	GridColor( IDC_CUSTOM_DOOR, 6, 2, col, BLACK_C );

}


void CScreen_Main::OnMain_Init_B_Lot_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 2;
	int col = 2;
	int i, j;
	
	CString str_tmp[2] = {"Lot No", "Part No"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_B_LOT_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 24);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 50);
				GridColor(IDC_CUSTOM_B_LOT_INFO, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_B_LOT_INFO, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_B_LOT_INFO, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 105);
				GridColor(IDC_CUSTOM_B_LOT_INFO, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_B_LOT_INFO, STATIC, i, j, 0);
				
				if (i == 2)
				{
					GridFont(IDC_CUSTOM_B_LOT_INFO, i, j, 13);
				}
				else
				{
					GridFont(IDC_CUSTOM_B_LOT_INFO, i, j, 15);
				}
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_B_LOT_INFO, i, 1, str_tmp[i - 1]);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Display_Lot_Yield_info()
{	//st_work.nMdlInputCount
	CString strTemp;
	int nLotNum = -1;
	
	// Prime
	strTemp.Format( "%06d", g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) );
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 2, 2, strTemp);		// Input
	strTemp.Format( "%06d", g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) );
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 3, 2, strTemp);		// Pass
	strTemp.Format( "%06d", g_lotMgr.GetLotAt(0).GetFailCnt(PRIME) );
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 4, 2, strTemp);		// Reject
	
	double dYield = 0;
	if (g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) > 0)
	{
		dYield = (double)g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) / (double)g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) * 100.0f;
	}
	
	strTemp.Format("%3.2f%%", dYield);
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 5, 2, strTemp);		// Yield
	
		
// 	// Cum
// 	strTemp.Format("%06d", g_lotMgr.GetLotAt(0).GetInputCnt(CUM));
// 	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 2, 3, strTemp);		// Input
// 	strTemp.Format("%06d", g_lotMgr.GetLotAt(0).GetPassCnt(CUM));
// 	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 3, 3, strTemp);		// Pass
// 	strTemp.Format("%06d", g_lotMgr.GetLotAt(0).GetFailCnt(CUM));
// 	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 4, 3, strTemp);		// Reject
// 	
// 	dYield = 0;
// 	if (g_lotMgr.GetLotAt(0).GetInputCnt(CUM) > 0)
// 	{
// 		dYield = (double)g_lotMgr.GetLotAt(0).GetPassCnt(CUM) / (double)g_lotMgr.GetLotAt(0).GetInputCnt(CUM) * 100.0f;
// 	}
// 	
// 	strTemp.Format("%3.2f%%", dYield);
// 	
// 	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 5, 3, strTemp);		// Yield
	
}	

void CScreen_Main::OnMain_Display_Lot_Info()
{
	CString strTmp="";

	if( g_lotMgr.GetLotCount() > 0 )
	{
		GridData(IDC_CUSTOM_LOT_INFO, 1, 2, g_lotMgr.GetLotIDAt( 0 ) );
		GridData(IDC_CUSTOM_LOT_INFO, 2, 2, g_lotMgr.GetLotAt(0).GetPartID() );
		GridData(IDC_CUSTOM_LOT_INFO, 3, 2, g_lotMgr.GetLotAt(0).GetStrLastModule());
		strTmp.Format("%d",g_lotMgr.GetLotAt(0).GetTotLotCount());
		GridData(IDC_CUSTOM_LOT_INFO, 4, 2, strTmp);
		GridData(IDC_CUSTOM_LOT_INFO, 5, 2, g_lotMgr.GetLotAt(0).GetStrBypass());
	}
	else
	{
		GridData(IDC_CUSTOM_LOT_INFO, 1, 2, "" );
		GridData(IDC_CUSTOM_LOT_INFO, 2, 2, "" );
		GridData(IDC_CUSTOM_LOT_INFO, 3, 2, "");
		GridData(IDC_CUSTOM_LOT_INFO, 4, 2, "");
		GridData(IDC_CUSTOM_LOT_INFO, 5, 2, "");
	}

	if( g_lotMgr.GetLotCount() > 1 )
	{
		GridData( IDC_CUSTOM_B_LOT_INFO, 1, 2, g_lotMgr.GetLotIDAt( 1 ) );
		GridData( IDC_CUSTOM_B_LOT_INFO, 2, 2, g_lotMgr.GetLotAt( 1 ).GetPartID() );
	}
	else
	{
		GridData( IDC_CUSTOM_B_LOT_INFO, 1, 2, "" );
		GridData( IDC_CUSTOM_B_LOT_INFO, 2, 2, "" );
	}
}

///kwlee 2017.0425
void CScreen_Main::OnInitHeatSinkGrid()
{
	int   i, j,nRow;
	int	  max_row, max_col;
	CString str_tmp;
// 	SS_CELLTYPE CellType;
	
	max_row =1;
	max_col = 4;
	
 	
// 	
// 	m_pGrid.GridReset(m_grid_HeatSink);
// 	// 대문자 
// 	m_pGrid.GridCellSelectDisplay(m_grid_HeatSink, FALSE);
// 	m_pGrid.GridRowHeader(m_grid_HeatSink, FALSE);
// 	m_pGrid.GridColHeader(m_grid_HeatSink, FALSE);
// 	m_pGrid.GridHorizontal(m_grid_HeatSink, FALSE);
// 	m_pGrid.GridVertical(m_grid_HeatSink, FALSE);
// 	m_pGrid.GridAutoSize(m_grid_HeatSink, FALSE);
// 	//m_pGrid.GridAutoSize(m_grid_Top, FALSE);
// 	m_pGrid.GridCellRows(m_grid_HeatSink, max_row);
// 	m_pGrid.GridCellCols(m_grid_HeatSink, max_col);
 	
// 	
// 	for(i=0; i<max_row+1; i++)
// 	{
// 		
// 		if (i == 0)
// 		{
// 			m_pGrid.GridCellHeight(m_grid_HeatSink, i, 30);	 
// 		}
// 		else
// 		{
// 			m_pGrid.GridCellHeight(m_grid_HeatSink, i+ 1, 35);	 
// 		}
// 		for(j=0; j<max_col+1; j++)
// 		{
// 			
// 			m_pGrid.GridCellWidth_L(m_grid_HeatSink, j, 13);
// 			m_pGrid.GridCellFont(m_grid_HeatSink, i, j, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_HeatSink, i, j, GRAY, WHITE_C);
// 			
// 		}
// 		
// 	}
// 	
 //	m_pGrid.GridCellMerge(m_grid_HeatSink, 1, 1, 1, 4);
// 	m_pGrid.GridCellFont(m_grid_HeatSink, 1, 1, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_HeatSink, 1, 1, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_HeatSink, 1, 1, "HEASINK");
// 	m_pGrid.GridCellControlStatic(m_grid_HeatSink, 1, 1);
// 
// 	// 	m_pGrid.GridCellMerge(m_grid_Top, 2, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_HeatSink, 2, 1, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_HeatSink, 2, 1, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_HeatSink, 2, 1, "HS BUFF PICK");
// 	// 	
// 	// 	m_pGrid.GridCellMerge(m_grid_Top, 3, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_HeatSink, 2, 2, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_HeatSink, 2, 2, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_HeatSink, 2, 2, "HS PLACE");
// 	// 	
// 	// 	m_pGrid.GridCellMerge(m_grid_Top, 4, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_HeatSink, 2, 3, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_HeatSink, 2, 3, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_HeatSink, 2, 3, "DISP PICK");
// 
// 	m_pGrid.GridCellFont(m_grid_HeatSink, 2, 4, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_HeatSink, 2, 4, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_HeatSink, 2, 4, "JIG PICK");
// 	
// 	Invalidate(FALSE);

	//kwlee 2017.0428
	//m_grid_HeatSink = (TSpread*)GetDlgItem(IDC_CUSTOM_HEATSINK);
		
	m_grid_HeatSink -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	m_grid_HeatSink -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	
	
	m_grid_Info_HeatSink.SetFrameFocusCell(FALSE);
	m_grid_Info_HeatSink.SetTrackFocusCell(FALSE);
	m_grid_Info_HeatSink.EnableSelection(FALSE);
	
	m_grid_Info_HeatSink.SetGridLineColor(BLACK_C);
	m_grid_Info_HeatSink.SetGridLines(GVS_BOTH);
	
	m_grid_Info_HeatSink.SetRowCount(max_row);
	m_grid_Info_HeatSink.SetColumnCount(max_col);
	m_grid_Info_HeatSink.SetFixedRowCount(0);
	m_grid_Info_HeatSink.SetFixedColumnCount(0);
	// 	m_grid_Top.SetFixedBkColor(RGB(0,0,200));
	// 	m_grid_Top.SetFixedBkColor(RGB(200,200,255));
	// 	m_grid_Top.SetTextBkColor(RGB(150,150,200));
	//kwlee 2017.0428
	m_grid_Info_HeatSink.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_HeatSink.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_HeatSink.SetTextBkColor(RGB(90,80,80));
	
	
	for (i=0; i<max_row; i++) 
	{
		if (i == 0)
		{
			m_grid_Info_HeatSink.SetRowHeight(i, 50);
		}
		else
		{
			m_grid_Info_HeatSink.SetRowHeight(i, 40);
		}
		
		
		for (j=0; j<max_col; j++) 
		{ 
			m_grid_Info_HeatSink.SetColumnWidth(j, 103);
			m_grid_Info_HeatSink.SetItemFormat(i, j, DT_LEFT|DT_LEFT|SS_TT_MULTILINE_MULTI);
			m_grid_Info_HeatSink.SetItemState(i, j, GVIS_READONLY);
			m_grid_Info_HeatSink.SetItemBkColour(i, j,  CLR_DEFAULT);
		}
	}
	
	DWORD dwTextStyle = DT_VCENTER|DT_CENTER|DT_SINGLELINE; 
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;	
	Item.nFormat = dwTextStyle;	
	
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_HeatSink.SetItemText(nRow, 0,"HS BUFF PICK");
	nRow = 0;
	Item.row = nRow;
	Item.col = 0;
	Item.strText = "HS BUFF PICK";
	m_grid_Info_HeatSink.SetItem(&Item);
	m_grid_Info_HeatSink.SetItemBkColour(nRow, 0, GRAY);
	m_grid_Info_HeatSink.SetItemFgColour(nRow, 0, WHITE_C);
	
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_HeatSink.SetItemText(nRow, 1,"HS PLACE");
	Item.row = nRow;
	Item.col = 1;
	Item.strText = "HS PLACE";
	m_grid_Info_HeatSink.SetItem(&Item);
	m_grid_Info_HeatSink.SetItemBkColour(nRow, 1, GRAY);
	m_grid_Info_HeatSink.SetItemFgColour(nRow, 1, WHITE_C);
	

	Item.row = nRow;
	Item.col = 2;
	Item.strText = "DISP PICK";
	m_grid_Info_HeatSink.SetItem(&Item);
	m_grid_Info_HeatSink.SetItemBkColour(nRow, 2, GRAY);
	m_grid_Info_HeatSink.SetItemFgColour(nRow, 2, WHITE_C);
	
	
	Item.row = nRow;
	Item.col = 3;
	Item.strText = "JIG PICK";
	m_grid_Info_HeatSink.SetItem(&Item);
	m_grid_Info_HeatSink.SetItemBkColour(nRow, 3, GRAY);
	m_grid_Info_HeatSink.SetItemFgColour(nRow, 3, WHITE_C);
	
}

void CScreen_Main::OnInitDeviceGrid()
{
	int   i, j,nRow;
	int	  max_row, max_col;
	CString str_tmp;
// 	SS_CELLTYPE CellType;
	
	max_row = 1;
	max_col = 3;
	
 	
// 	
// 	m_pGrid.GridReset(m_grid_Device);
// 	// 대문자 
// 	m_pGrid.GridCellSelectDisplay(m_grid_Device, FALSE);
// 	m_pGrid.GridRowHeader(m_grid_Device, FALSE);
// 	m_pGrid.GridColHeader(m_grid_Device, FALSE);
// 	m_pGrid.GridHorizontal(m_grid_Device, FALSE);
// 	m_pGrid.GridVertical(m_grid_Device, FALSE);
// 	m_pGrid.GridAutoSize(m_grid_Device, FALSE);
// 	//m_pGrid.GridAutoSize(m_grid_Top, FALSE);
// 	m_pGrid.GridCellRows(m_grid_Device, max_row);
// 	m_pGrid.GridCellCols(m_grid_Device, max_col);
// 	m_grid_HeatSink->SetTypeStaticText(&CellType, SS_TEXT_LEFT | SS_TEXT_VCENTER | SS_TT_MULTILINE_MULTI);
// 	
// 	for(i=0; i<max_row+1; i++)
// 	{
// 		
// 		if (i == 0)
// 		{
// 			m_pGrid.GridCellHeight(m_grid_Device, i, 30);	 
// 		}
// 		else
// 		{
// 			m_pGrid.GridCellHeight(m_grid_Device, i+ 1, 35);	 
// 		}
// 		for(j=0; j<max_col+1; j++)
// 		{
// 			
// 			m_pGrid.GridCellWidth_L(m_grid_Device, j, 17);
// 			m_pGrid.GridCellFont(m_grid_Device, i, j, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Device, i, j, GRAY, WHITE_C);
// 		}
// 		
// 	}
// 	
// 	m_pGrid.GridCellMerge(m_grid_Device, 1, 1, 1, 4);
// 	m_pGrid.GridCellFont(m_grid_Device, 1, 1, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_Device, 1, 1, BLUE, YELLOW_C);
// 	m_pGrid.GridCellControlStatic(m_grid_Device, 1, 1);
// 	m_pGrid.GridCellText(m_grid_Device, 1, 1, "DEVICE");
// 	
// 	// 	m_pGrid.GridCellMerge(m_grid_Top, 2, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_Device, 2, 1, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_Device, 2, 1, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_Device, 2, 1, "DVC PICK" );
// 	// 	
// 	// 	m_pGrid.GridCellMerge(m_grid_Top, 3, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_Device, 2, 2, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_Device, 2, 2, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_Device, 2, 2, "LOAD BUFF");
// 	
// 	m_pGrid.GridCellFont(m_grid_Device, 2, 3, "MS Sans Serif", 10);
// 	m_pGrid.GridCellColor(m_grid_Device, 2, 3, GRAY, WHITE_C);
// 	m_pGrid.GridCellText(m_grid_Device, 2, 3, "UNLOAD BUFF");
// 	// 	
// 	Invalidate(FALSE);

	//kwlee 2017.0428
	//m_grid_Device = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);

	m_grid_Device -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	m_grid_Device -> SetBool(SSB_VERTSCROLLBAR, FALSE);

	m_grid_Info_Device.SetFrameFocusCell(FALSE);
	m_grid_Info_Device.SetTrackFocusCell(FALSE);
	m_grid_Info_Device.EnableSelection(FALSE);
	
	m_grid_Info_Device.SetGridLineColor(BLACK_C);
	m_grid_Info_Device.SetGridLines(GVS_BOTH);
	
	m_grid_Info_Device.SetRowCount(max_row);
	m_grid_Info_Device.SetColumnCount(max_col);
	m_grid_Info_Device.SetFixedRowCount(0);
	m_grid_Info_Device.SetFixedColumnCount(0);
	// 	m_grid_Top.SetFixedBkColor(RGB(0,0,200));
	// 	m_grid_Top.SetFixedBkColor(RGB(200,200,255));
	// 	m_grid_Top.SetTextBkColor(RGB(150,150,200));
	//kwlee 2017.0428
	m_grid_Info_Device.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_Device.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_Device.SetTextBkColor(RGB(90,80,80));
	
	

	for (i=0; i<max_row; i++) 
	{
		if (i == 0)
		{
			m_grid_Info_Device.SetRowHeight(i,50);
		}
		else
		{
			m_grid_Info_Device.SetRowHeight(i, 40);
		}
		
		for (j=0; j<max_col; j++) 
		{
			m_grid_Info_Device.SetColumnWidth(j, 133);
			m_grid_Info_Device.SetItemFormat(i, j, DT_LEFT|DT_LEFT|SS_TT_MULTILINE_MULTI);
			m_grid_Info_Device.SetItemState(i, j, GVIS_READONLY);
			m_grid_Info_Device.SetItemBkColour(i, j,  CLR_DEFAULT);	
		}
	}
	
// 	nRow = 0;
// 	m_grid_Info_Device.SetItemBkColour(nRow, 0, RGB(219, 229, 241));
// 	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_Device.SetItemText(nRow, 0,"DEVICE");
// 	m_grid_Info_Device.SetItemFgColour(nRow,0,BLACK_C);
	//	m_grid_Top.SetItemText(nRow, 0,strTmp);
	
	//nRow = 1;
	DWORD dwTextStyle = DT_VCENTER|DT_CENTER|DT_SINGLELINE; 
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;	
	Item.nFormat = dwTextStyle;	
	

	nRow = 0;
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_Device.SetItemText(nRow, 0,"DVC PICK");
	Item.row = nRow;
	Item.col = 0;
	Item.strText = "DVC PICK";
	m_grid_Info_Device.SetItem(&Item);
	m_grid_Info_Device.SetItemBkColour(nRow, 0, GRAY);
	m_grid_Info_Device.SetItemFgColour(nRow,0,WHITE_C);
	
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	// 	m_grid_Info_Device.SetItemText(nRow, 1,"LOAD BUFF");
	Item.row = nRow;
	Item.col = 1;
	Item.strText = "LOAD BUFF";
	m_grid_Info_Device.SetItem(&Item);
	m_grid_Info_Device.SetItemBkColour(nRow, 1, GRAY);
	m_grid_Info_Device.SetItemFgColour(nRow,1,WHITE_C);
	
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	// 	m_grid_Info_Device.SetItemText(nRow, 2,"UNLOAD BUFF");
	Item.row = nRow;
	Item.col = 2;
	Item.strText = "UNLOAD BUFF";
	m_grid_Info_Device.SetItem(&Item);
	m_grid_Info_Device.SetItemBkColour(nRow, 2, GRAY);
	m_grid_Info_Device.SetItemFgColour(nRow,2,WHITE_C);	
}
/////


void CScreen_Main::OnInitTopGrid()
{
	int   i, j;
	int	  max_row, max_col,nRow;//,nCol;
	
	CString strTemp[4];
	int nTmp[4] = {0,};
 	CString sTmp="";

	CString strTmp;
// 	SS_CELLTYPE CellType;

	max_row = 4;
	max_col = 7;

// 	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
// 	
// 	m_pGrid.GridReset(m_grid_Top);
// 	// 대문자 
// 	
// 	m_pGrid.GridCellSelectDisplay(m_grid_Top, TRUE);
// 	m_pGrid.GridRowHeader(m_grid_Top, FALSE);
// 	m_pGrid.GridColHeader(m_grid_Top, FALSE);
// 	m_pGrid.GridHorizontal(m_grid_Top, FALSE);
// 	m_pGrid.GridVertical(m_grid_Top, FALSE);
// 	m_pGrid.GridAutoSize(m_grid_Top, FALSE);
// 	//m_pGrid.GridAutoSize(m_grid_Top, FALSE);
// 	m_pGrid.GridCellRows(m_grid_Top, max_row);
// 	m_pGrid.GridCellCols(m_grid_Top, max_col);
// 	m_grid_Top->SetTypeStaticText(&CellType, SS_TEXT_LEFT | SS_TEXT_VCENTER | SS_TT_MULTILINE_MULTI);
// 	
// 	
// 
// 	for(i=0; i<max_row+1; i++)
// 	{
// 		
// 		if (i == 0)
// 		{
// 			m_pGrid.GridCellHeight(m_grid_Top, i, 30);	 
// 		}
// 		else
// 		{
// 			m_pGrid.GridCellHeight(m_grid_Top, i+ 1, 50);	 
// 			
// 		}
// 		for(j=0; j<max_col+1; j++)
// 		{
// 			
// 			m_pGrid.GridCellWidth_L(m_grid_Top, j, 18);
// 			m_pGrid.GridCellFont(m_grid_Top, i, j, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Top, i, j, GRAY, YELLOW_C);
// 		}
// 		
// 	}
// 	
// // 	m_pGrid.GridCellMerge(m_grid_Top, 1, 1, 1, 2);
//  	m_pGrid.GridCellFont(m_grid_Top, 1, 1, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 1, BLUE, YELLOW_C);
//  	m_pGrid.GridCellText(m_grid_Top, 1, 1, "Unload");
// 
// // 	m_pGrid.GridCellMerge(m_grid_Top, 2, 1, 1, 2);
//  	m_pGrid.GridCellFont(m_grid_Top, 1, 2, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 2, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Top, 1, 2, "OutSend");
// // 	
// // 	m_pGrid.GridCellMerge(m_grid_Top, 3, 1, 1, 2);
//  	m_pGrid.GridCellFont(m_grid_Top, 1, 3, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 3, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Top, 1, 3, "Hs Vsion");
// // 	
// // 	m_pGrid.GridCellMerge(m_grid_Top, 4, 1, 1, 2);
//  	m_pGrid.GridCellFont(m_grid_Top, 1, 4, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 4, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Top, 1, 4, "Wait index");
// 
// 	m_pGrid.GridCellFont(m_grid_Top, 1, 5, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 5, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Top, 1, 5, "Epoxy");
// 
// 	m_pGrid.GridCellFont(m_grid_Top, 1, 6, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 6, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Top, 1, 6, "Input Load");
// 
// 	m_pGrid.GridCellFont(m_grid_Top, 1, 7, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Top, 1, 7, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Top, 1, 7, "Load");
// 
// 	Invalidate(FALSE);
	
	//kwlee 2017.0428	
	m_grid_Top = (TSpread*)GetDlgItem( IDC_CUSTOM_TOP_INFO );
	m_grid_Top -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	m_grid_Top -> SetBool(SSB_VERTSCROLLBAR, FALSE);

	m_grid_Info_Top.SetFrameFocusCell(FALSE);
	m_grid_Info_Top.SetTrackFocusCell(FALSE);
	m_grid_Info_Top.EnableSelection(FALSE);

	m_grid_Info_Top.SetGridLineColor(BLACK_C);
	m_grid_Info_Top.SetGridLines(GVL_BOTH);
	
	m_grid_Info_Top.SetRowCount(max_row);
	m_grid_Info_Top.SetColumnCount(max_col);
	m_grid_Info_Top.SetFixedRowCount(0);
	m_grid_Info_Top.SetFixedColumnCount(0);
// 	m_grid_Top.SetFixedBkColor(RGB(0,0,200));
// 	m_grid_Top.SetFixedBkColor(RGB(200,200,255));
// 	m_grid_Top.SetTextBkColor(RGB(150,150,200));
	//kwlee 2017.0428
	m_grid_Info_Top.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_Top.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_Top.SetTextBkColor(RGB(90,80,80));



	for (i=0; i<max_row; i++) 
	{
		if (i == 0)
		{
			m_grid_Info_Top.SetRowHeight(i, 30);
		}
		else
		{
			m_grid_Info_Top.SetRowHeight(i, 65);
		}
		

		for (j=0; j<max_col; j++) 
		{
			m_grid_Info_Top.SetColumnWidth(j, 143);
			m_grid_Info_Top.SetItemFormat(i, j, DT_VCENTER|DT_LEFT|SS_TT_MULTILINE_SINGLE);
			m_grid_Info_Top.SetItemState(i, j, GVIS_READONLY);
			m_grid_Info_Top.SetItemBkColour(i, j,  CLR_DEFAULT);	
		}
	}



	DWORD dwTextStyle = DT_VCENTER|DT_LEFT|DT_SINGLELINE; 
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;	
	Item.nFormat = dwTextStyle;	
	
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	// 	m_grid_Info_HeatSink.SetItemText(nRow, 0,"HS BUFF PICK");
	nRow = 0;
	Item.row = nRow;
	Item.col = 0;
	Item.strText = "Unload";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 0, RGB(219, 229, 241));
	//kwlee 2017.0512
// 	m_grid_Info_Top.SetItemBkColour(nRow, 0, RGB(219, 229, 241));
// //	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_Top.SetItemText(nRow, 0,"Unload");
// //	m_grid_Top.SetItemText(nRow, 0,strTmp);
	


	nRow = 0;
	Item.row = nRow;
	Item.col = 1;
	Item.strText = "OutSend";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 1, RGB(219, 229, 241));
	//kwlee 2017.0512
//	m_grid_Info_Top.SetItemBkColour(nRow, 1, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
//	m_grid_Info_Top.SetItemText(nRow, 1,"OutSend");



	Item.row = nRow;
	Item.col = 2;
	Item.strText = "Hs Vsion";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 2, RGB(219, 229, 241));
	//kwlee 2017.0512
// 	m_grid_Info_Top.SetItemBkColour(nRow, 2, RGB(219, 229, 241));
// 	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_Top.SetItemText(nRow, 2,"Hs Vsion");


	Item.row = nRow;
	Item.col = 3;
	Item.strText = "Wait index";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 3, RGB(219, 229, 241));
	//kwlee 2017.0512
// 	m_grid_Info_Top.SetItemBkColour(nRow, 3, RGB(219, 229, 241));
// 	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
// 	m_grid_Info_Top.SetItemText(nRow, 3,"Wait index");


	Item.row = nRow;
	Item.col = 4;
	Item.strText = "Wait Epoxy";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 4, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Top.SetItemBkColour(nRow, 4, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Top.SetItemText(nRow, 4,"Epoxy");

	Item.row = nRow;
	Item.col = 5;
	Item.strText = "Input Load";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 5, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Top.SetItemBkColour(nRow, 5, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Top.SetItemText(nRow, 5,"Input Load");

	Item.row = nRow;
	Item.col = 6;
	Item.strText = "Load";
	m_grid_Info_Top.SetItem(&Item);
	m_grid_Info_Top.SetItemBkColour(nRow, 6, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Top.SetItemBkColour(nRow, 6, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Top.SetItemText(nRow, 6,"Load");
}


void CScreen_Main::OnInitBTMGrid()
{
	int   i, j,nRow;
	int	  max_row, max_col;
	CString str_tmp;
	
	max_row = 4;
	max_col = 7;

	
// 	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
// 	
// 	m_pGrid.GridReset(m_grid_Btm);
// 	// 대문자 
// //	m_pGrid.GridCellSelectDisplay(m_grid_Btm, TRUE);
// 	m_pGrid.GridRowHeader(m_grid_Btm, FALSE);
// 	m_pGrid.GridColHeader(m_grid_Btm, FALSE);
// 	m_pGrid.GridHorizontal(m_grid_Btm, FALSE);
// 	m_pGrid.GridVertical(m_grid_Btm, FALSE);
// 	//m_pGrid.GridAutoSize(m_grid_Btm, FALSE);
// 	m_pGrid.GridAutoSize(m_grid_Btm, FALSE);
// 	m_pGrid.GridCellRows(m_grid_Btm, max_row);
// 	m_pGrid.GridCellCols(m_grid_Btm, max_col);
// 		
// 	for(i=0; i<max_row+1; i++)
// 	{
// 		
// 		if (i == 0)
// 		{
// 			m_pGrid.GridCellHeight_L(m_grid_Btm, i, 30);
// 		}
// 		else
// 		{
// 			m_pGrid.GridCellHeight_L(m_grid_Btm, i+ 1, 50);
// 		}
// 		
// 		 
// 
// 		for(j=0; j<max_col+1; j++)
// 		{
// 			m_pGrid.GridCellWidth_L(m_grid_Btm, j, 18);
// 			/*m_pGrid.GridCellMerge(m_grid_Top, i, j, 1, 2);*/
// 			m_pGrid.GridCellFont(m_grid_Btm, i, j, "MS Sans Serif", 10);
// 			m_pGrid.GridCellColor(m_grid_Btm, i, j, GRAY, YELLOW_C);	
// 		}
// 	}
// 	
// // 	m_pGrid.GridCellMerge(m_grid_Btm, 1, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_Btm, 1, 7, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 7, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Btm, 1, 7, "Loader Down");
// // 	
// // 	m_pGrid.GridCellMerge(m_grid_Btm, 2, 1, 1, 2);
//  	m_pGrid.GridCellFont(m_grid_Btm, 1, 6, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 6, BLUE, YELLOW_C);
//  	m_pGrid.GridCellText(m_grid_Btm, 1, 6, "Input Down");
// 
// // 	m_pGrid.GridCellMerge(m_grid_Btm, 3, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_Btm, 1, 5, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 5, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Btm, 1, 5, "Epoxy Down");
// // 	
// // 	m_pGrid.GridCellMerge(m_grid_Btm, 4, 1, 1, 2);
// 	m_pGrid.GridCellFont(m_grid_Btm, 1, 4, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 4, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Btm, 1, 4, "Index Down");
// 
// 	m_pGrid.GridCellFont(m_grid_Btm, 1, 3, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 3, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Btm, 1, 3, "Hs Down");
// 
// 	m_pGrid.GridCellFont(m_grid_Btm, 1, 2, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 2, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Btm, 1, 2, "Down Fwd");
// 
// 	m_pGrid.GridCellFont(m_grid_Btm, 1, 1, "MS Sans Serif", 10);
//  	m_pGrid.GridCellColor(m_grid_Btm, 1, 1, BLUE, YELLOW_C);
// 	m_pGrid.GridCellText(m_grid_Btm, 1, 1, "Buff Down");
// 
// 	Invalidate(FALSE);
	//kwlee 2017.0428	
	m_grid_Btm = (TSpread*)GetDlgItem( IDC_CUSTOM_BTM_INFO );
	m_grid_Btm -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	m_grid_Btm -> SetBool(SSB_VERTSCROLLBAR, FALSE);

	m_grid_Info_Btm.SetFrameFocusCell(FALSE);
	m_grid_Info_Btm.SetTrackFocusCell(FALSE);
	m_grid_Info_Btm.EnableSelection(FALSE);
	
	m_grid_Info_Btm.SetGridLineColor(BLACK_C);
	m_grid_Info_Btm.SetGridLines(GVL_BOTH);
	
	m_grid_Info_Btm.SetRowCount(max_row);
	m_grid_Info_Btm.SetColumnCount(max_col);
	m_grid_Info_Btm.SetFixedRowCount(0);
	m_grid_Info_Btm.SetFixedColumnCount(0);
	// 	m_grid_Top.SetFixedBkColor(RGB(0,0,200));
	// 	m_grid_Top.SetFixedBkColor(RGB(200,200,255));
	// 	m_grid_Top.SetTextBkColor(RGB(150,150,200));
	//kwlee 2017.0428
	m_grid_Info_Btm.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_Btm.SetFixedBkColor(RGB(0,0,255));
	m_grid_Info_Btm.SetTextBkColor(RGB(90,80,80));
	
	
	for (i=0; i<max_row; i++) 
	{
		if (i == 0)
		{
			m_grid_Info_Btm.SetRowHeight(i, 30);
		}
		else
		{
			m_grid_Info_Btm.SetRowHeight(i, 65);
		}
		
		
		for (j=0; j<max_col; j++) 
		{
			m_grid_Info_Btm.SetColumnWidth(j, 143);
			m_grid_Info_Btm.SetItemFormat(i, j, DT_LEFT|DT_VCENTER|SS_TT_MULTILINE_SINGLE);
			m_grid_Info_Btm.SetItemState(i, j,GVIS_READONLY);
			m_grid_Info_Btm.SetItemBkColour(i, j,  CLR_DEFAULT);	
		}
	}

	DWORD dwTextStyle = DT_VCENTER|DT_LEFT|DT_SINGLELINE; 
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;	
	Item.nFormat = dwTextStyle;	

	nRow = 0;
	Item.row = nRow;
	Item.col = 0;
	Item.strText = "Buff Down";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 0, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 0, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
//	m_grid_Info_Btm.SetItemText(nRow, 0,"Buff Down");
	

	Item.row = nRow;
	Item.col = 1;
	Item.strText = "Down Fwd";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 1, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 1, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Btm.SetItemText(nRow, 1,"Down Fwd");
	

	Item.row = nRow;
	Item.col = 2;
	Item.strText = "Hs Down";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 2, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 2, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Btm.SetItemText(nRow, 2,"Hs Down");


	Item.row = nRow;
	Item.col = 3;
	Item.strText = "Index Down";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 3, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 3, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Btm.SetItemText(nRow, 3,"Index Down");


	Item.row = nRow;
	Item.col = 4;
	Item.strText = "Epoxy Down";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 4, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 4, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Btm.SetItemText(nRow, 4,"Epoxy Down");
	
	Item.row = nRow;
	Item.col = 5;
	Item.strText = "Input Down";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 5, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 5, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Btm.SetItemText(nRow, 5,"Input Down");	
	
	Item.row = nRow;
	Item.col = 6;
	Item.strText = "Loader Down";
	m_grid_Info_Btm.SetItem(&Item);
	m_grid_Info_Btm.SetItemBkColour(nRow, 6, RGB(219, 229, 241));
	//kwlee 2017.0512
	//m_grid_Info_Btm.SetItemBkColour(nRow, 6, RGB(219, 229, 241));
	//	m_grid_Top.SetItemFont(nRow, 0, &clsFunc.OnLogFont(16));
	//m_grid_Info_Btm.SetItemText(nRow, 6,"Loader Down");
	//	m_grid_Top.SetItemText(nRow, 0,strTmp);
}
void CScreen_Main::OnInitGridRef()
{
	int   i, j;
	int	  max_row, max_col;
	CString str_tmp;
	
	max_row = 1;
	max_col = 10;
	
	
	m_grid_Ref = (TSpread*)GetDlgItem(IDC_CUSTOM_BIN_REFERNCE);
	
	m_pGrid.GridReset(m_grid_Ref);
	// 대문자 
	//m_pGrid.GridCellSelectDisplay(m_grid_Ref, TRUE);
	m_pGrid.GridRowHeader(m_grid_Ref, FALSE);
	m_pGrid.GridColHeader(m_grid_Ref, FALSE);
	m_pGrid.GridHorizontal(m_grid_Ref, FALSE);
	m_pGrid.GridVertical(m_grid_Ref, FALSE);
	m_pGrid.GridAutoSize(m_grid_Ref, FALSE);
	m_pGrid.GridAutoSize(m_grid_Ref, FALSE);
	m_pGrid.GridCellRows(m_grid_Ref, max_row);
	m_pGrid.GridCellCols(m_grid_Ref, max_col);
	
	for(i=0; i<max_row+1; i++)
	{
		
		m_pGrid.GridCellHeight_L(m_grid_Ref, i, 25);
		
		
		for(j=0; j<max_col+1; j++)
		{
			m_pGrid.GridCellWidth_L(m_grid_Ref, j, 12);
			/*m_pGrid.GridCellMerge(m_grid_Top, i, j, 1, 2);*/
			m_pGrid.GridCellFont(m_grid_Ref, i, j, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Ref, i, j, YELLOW_L, BLACK_L);	
		}
	}

	m_pGrid.GridCellFont(m_grid_Ref, 1, 1, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 1, OK_C, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 1, "PASS");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 1);
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 2, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 2, NG_C, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 2, "FAIL");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 2);
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 3, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 3, GRAY, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 3, "DVC_NO");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 3);
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 4, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,4, DVC_UNLOAD_C, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 4, "UNLOAD");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 4);

	m_pGrid.GridCellFont(m_grid_Ref, 1, 5, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,5, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 5, "HS INSP");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 5);
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 6, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,6, BLUE_D, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 6, "HEAT SINK");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 6);

	m_pGrid.GridCellFont(m_grid_Ref, 1, 7, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,7, ORANGE_C, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 7, "EPOXY INSP");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 7);
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 8, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,8, NOR_L, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 8, "EPOXY");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 8);
	
	//m_pGrid.GridCellMerge(m_grid_Btm, 1, 1, 1, 2);
	m_pGrid.GridCellFont(m_grid_Ref, 1, 9, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 9, SKY_C, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 9, "READY");//! khj[20101012]
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 9);
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 10, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 10, BLUE, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 10, "LOAD");
	m_pGrid.GridCellControlStatic( m_grid_Ref, 1, 10);

	Invalidate(FALSE);
}


void CScreen_Main::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CScreen_Main::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnBtnDoorOpen() 
{
	CDialog_Select select_dlg;
	CDialog_Pass_Check	pass_dlg;
	int					n_response;
	CTL_Lib.Alarm_Error_Occurrence(201, dWARNING, "910004");
	return;

	if(st_work.mn_run_status == dRUN || st_work.mn_run_status == dINIT || st_work.mn_machine_mode == MACHINE_AUTO)
	{
		st_msg.mstr_event_msg[0] = _T("Door Open은 장비가 정지 상태 이고, Key 스위치가 Manual 상태에서만 가능합니다.");
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		return;
	}
	
	st_other.str_confirm_msg = "Door를 Open하시겠습니까?...";
	
	if(select_dlg.DoModal() == IDOK) 
	{
		if (st_handler.n_menu_lock != FALSE)  return;
		if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴
		
		st_other.str_pass_level = "LevelChange";
		
		n_response = pass_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			Func.OnSet_Door_Open();
		}	
	}
}


void CScreen_Main::OnTray2Lock() 
{
	if(st_work.mn_run_status == dRUN) return;
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Lock_Sol, IO_ON );
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Unlock_Sol, IO_OFF );
}

void CScreen_Main::OnTray2Unlock() 
{
	if(st_work.mn_run_status == dRUN) return;
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Lock_Sol, IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Unlock_Sol, IO_ON );	
}

void CScreen_Main::OnTray1Lock() 
{
	
	CString strTemp;
	if(st_work.mn_run_status == dRUN) return;
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Lock_Sol, IO_ON );
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Unlock_Sol, IO_OFF );
	return;

	//kwlee 2017.0621 carrier Buff Test
	// 	strTemp.Format("%d",m_ncnt);
// 	AfxMessageBox(strTemp);
//  	if (m_ncnt == 0)
// 	{
// 		Run_Device_Carrier_Robot.AllBufferClear();
// 
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE );	
// 		st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE, 0);
// 		
// 	}
// 	else if (m_ncnt == 1)
// 	{
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[MIDDLE]= ++m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[BTM] = ++m_nDvcCnt;
// 
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[TOP]	= BIN_CDIMM;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[MIDDLE] = BIN_CDIMM;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[BTM]	= BIN_CDIMM;
// 
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_LOADER_RECEIVE );	
// 		//Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 1, TOPSHIFT_BUFF_UNLOADER );
// 		//st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_RECEIVE, 0);
// 
// 	}
// 	else if (m_ncnt == 2)
// 	{
// 		st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBinNum[BTM] = m_nDvcCnt;
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_INPUT_LOADER );	
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_INPUT_LOADER, 0);
// 		
// 	}
// 	else if (m_ncnt == 3)
// 	{
// 		st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBinNum[BTM] = m_nDvcCnt;
// 		
// 		st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[TOP]	= BIN_EPOXY;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[MIDDLE] = BIN_EPOXY;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[BTM]	= BIN_EPOXY;
// 	
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_EPOXY );	
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_EPOXY, 0);	
// 	}
// 	else if (m_ncnt == 4)
// 	{
// 		st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBinNum[BTM] = m_nDvcCnt;
// 
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_WAIT_INDEX );	
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_WAIT_INDEX, 0);
// 		
// 	}
// 	else if (m_ncnt == 5)
// 	{	
// 		st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[BTM] = m_nDvcCnt;
// 
// 		st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[TOP]	= BIN_HEATSINK;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[MIDDLE] = BIN_HEATSINK;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[BTM]	= BIN_HEATSINK;
// 
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_HEATSINK_VISION );
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_HEATSINK_VISION, 0);
// 		
// 	}
// 	else if (m_ncnt == 6)
// 	{
// 		st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[TOP]	= BIN_VISION;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[MIDDLE] = BIN_VISION;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[BTM]	= BIN_VISION;
// 
// 		st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBinNum[BTM]   = m_nDvcCnt;
// 
// 	
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_OUTSEND );
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_OUTSEND, 0);
// 		
// 	}
// 	else if (m_ncnt == 7)
// 	{
// 		st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBinNum[BTM] = m_nDvcCnt;
// 
// 		Run_Device_Carrier_Robot.Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_UNLOADER );
// 		
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_UNLOADER, 0);
// 	}
// 	else if (m_ncnt == 8)
// 	{
// 		
// 		//Run_Device_Carrier_Robot.AllBufferClear();
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBinNum[BTM] = m_nDvcCnt;
// 		
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[TOP]	= BIN_VISION;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[MIDDLE] = BIN_VISION;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[BTM]	= BIN_VISION;
// 
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_DOWN );
// // 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_INDEX_DOWN );
// // 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_EPOXY_DOWN );
// // 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_INPUT_DOWN );
// // 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_LOADER_DOWN );
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_DOWN, 0);	
// 	}
// 	else if (m_ncnt == 9)
// 	{
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].nBinNum[BTM] = m_nDvcCnt;
// 		
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_DOWNFORWARD );
// 		
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_DOWNFORWARD, 0);
// 	}
// 	else if (m_ncnt == 10)
// 	{
// 		st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].nBinNum[BTM] = m_nDvcCnt;
// 
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_HEATSINK_DOWN );
// 		
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_HEATSINK_DOWN, 0);
// 		
// 	}
// 	else if (m_ncnt == 11)
// 	{
// 		st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].nBinNum[BTM] = m_nDvcCnt;
// 
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_INDEX_DOWN );
// 		
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INDEX_DOWN, 0);
// 		
// 	}
// 	else if (m_ncnt == 12)
// 	{
// 		st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].nBinNum[BTM] = m_nDvcCnt;
// 
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_EPOXY_DOWN );
// 		
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_EPOXY_DOWN, 0);
// 		
// 	}
// 	else if (m_ncnt == 13)
// 	{
// 		st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].nBinNum[BTM] = m_nDvcCnt;
// 		
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_INPUT_DOWN );
// 		
// 		//st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INPUT_DOWN, 0);
// 		
// 	}
// 	else if (m_ncnt == 14)
// 	{
// 		st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBinNum[TOP]	= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBinNum[MIDDLE]= m_nDvcCnt;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBinNum[BTM] = m_nDvcCnt;
// 
// 		st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBin[TOP]	= BIN_GOOD;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBin[MIDDLE] = BIN_GOOD;
// 		st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].nBin[BTM]	= BIN_GOOD;
// 		
// 		Run_Device_Carrier_Robot.Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_LOADER_DOWN );
// 		//Run_Device_Carrier_Robot.AllBufferClear();
// 	//	st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_LOADER_DOWN, 0);
// 	}
// 	m_nDvcCnt++;
// 	if (m_ncnt > 14)
// 	{
// 		m_ncnt = 1;
// 	}
// 	else
// 	{
// 		m_ncnt++;
// 	}
// 	return;
	
// 	if(nUnloadingButtonFlag == 0)
// 	{
// 		((CButton*)GetDlgItem(IDC_BUTTON_UNLOADING_STACKER_LOADING))->SetBitmap(m_BitmapUnLoadingBW);		
// 	}
// 	else
// 	{
// 		((CButton*)GetDlgItem(IDC_BUTTON_UNLOADING_STACKER_LOADING))->SetBitmap(m_BitmapUnLoadingBY);		
// 	}
// 	
// 	if(nLoadingButttonFlag1 == 0)
// 	{
// 		((CButton*)GetDlgItem(IDC_BUTTON_LOADING_STACKER_UNLOADING))->SetBitmap(m_BitmapLoadingSW);		
// 	}
// 	else
// 	{
// 		((CButton*)GetDlgItem(IDC_BUTTON_LOADING_STACKER_UNLOADING))->SetBitmap(m_BitmapLoadingSY);			
// 	}
// 

}

void CScreen_Main::OnTray1Unlock() 
{
	if(st_work.mn_run_status == dRUN) return;
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Lock_Sol, IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Unlock_Sol, IO_ON );
}

void CScreen_Main::OnCheck1() 
{
// 	if (m_changeTop_Btm.GetCheck() == TRUE)	mn_buzzer_mode[1] = TRUE;
// 	else
	
}

void CScreen_Main::OnLabelUnload() 
{
	CDialog_Infor select_dlg;
	st_other.str_confirm_msg = _T("Warning : Wanna remove on device in unload Buffer?");
	
	int iResponse = select_dlg.DoModal();
	
	if(iResponse == IDOK)
	{
		st_buffer_info[THD_ULD_ALIGN_BUFF].st_pcb_info[0].nYesNo = CTL_NO;
	}
	
}

void CScreen_Main::OnCheckOnlycarrier() 
{
	if( m_chkOnlyCarrier.GetCheck() == IO_OFF )
	{
		m_chkOnlyCarrier.SetCheck(IO_ON);
		st_work.n_OnlyCarrierMove = CTL_YES;
	}
	else if( m_chkOnlyCarrier.GetCheck() == IO_ON )
	{
		m_chkOnlyCarrier.SetCheck(IO_OFF);
		st_work.n_OnlyCarrierMove = CTL_NO;
	}
	st_work.n_OnlyCarrierMove = CTL_NO;
	
}

void CScreen_Main::OnMain_Display_Daily_Yield_info()
{
	CString strTemp;
	// Prime
	strTemp.Format( "%06d", g_lotMgr.GetInputCnt(PRIME) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 2, 2, strTemp);		// Input
	strTemp.Format( "%06d", g_lotMgr.GetPassCnt(PRIME) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 3, 2, strTemp);		// Pass
	strTemp.Format( "%06d", g_lotMgr.GetFailCnt(PRIME) );
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 4, 2, strTemp);		// Reject
	
	double dYield = 0;
	if (g_lotMgr.GetInputCnt(PRIME) > 0)
	{
		dYield = (double)g_lotMgr.GetPassCnt(PRIME) / (double)g_lotMgr.GetInputCnt(PRIME) * 100.0f;
	}
	strTemp.Format("%3.2f%%", dYield);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 5, 2, strTemp);		// Yield
}

void CScreen_Main::OnEpoxyCleanOutPosMove() 
{
	
	if(st_work.mn_run_status == dRUN) return;
	g_handler.ClearStep();
	SetTimer( WM_EPOXY_MOVE_CLEAN, 100, NULL );	
	return;
	
	//kwlee 2017.0425
	if (mn_alarm_cnt > 6)
	{
		mn_alarm_cnt = 0;
	}

	CDialog_Select select_dlg;
	st_other.str_confirm_msg = _T("Warning : Would you move to Epoxy Clean pos");
	
	int iResponse = select_dlg.DoModal();
	
	mn_alarm_cnt++;
	return;
	
}

//kwlee 2017.0425
void CScreen_Main::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;
	
	//	int	n_response;
	CDialog_Select select_dlg;
	CString strTmp, strChange, str_file;
	CString	sBeforePos;
	CString str_tmp, str_data;
	int nResponse;

	//kwlee 2017.0425
	CDialog_Infor  info_dlg;
	CString m_strTemp;
	CString strText;

	m_nRow	= lpcc->Row;
	m_nCol	= lpcc->Col;

	if (m_nRow <= 1)
	{
		//g_handler.ClearStep();
		//SetTimer( WM_EPOXY_MOVE_CLEAN, 100, NULL );	
		return;
	}

	if (wParam == IDC_CUSTOM_TOP_INFO)
	{
		switch(m_nCol)
		{
		case 1:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff Uld : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_UNLOADER %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 2:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff OutSnd : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_OUTSEND %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 3:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff HS Vision : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_HEATSINK_VISION %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 4:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff Wait : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_WAIT_INDEX %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 5:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff Epoxy : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_EPOXY %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 6:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff Input : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_INPUT_LOADER %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 7:
			if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("TopShift Buff Rcv : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("TOPSHIFT_BUFF_LOADER_RECEIVE %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
		}
	}
	else if (wParam == IDC_CUSTOM_BTM_INFO)
	{
		switch(m_nCol)
		{
		case 1:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff Down : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_DOWN %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 2:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff Down Fwd: Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_DOWNFORWARD %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 3:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff HS Down: Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_HEATSINK_DOWN %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 4:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff Index Down: Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_INDEX_DOWN %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 5:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff Epoxy Down: Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_EPOXY_DOWN %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 6:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff Input Down: Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_INPUT_DOWN %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
			
		case 7:
			if (st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_exist[m_nRow - 2] == CTL_YES)
			{
				st_other.str_confirm_msg = _T("BtmShift Buff Loader Down: Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_exist[m_nRow - 2] = CTL_NO;
					m_strSkipData.Format("BTMSHIFT_BUFF_LOADER_DOWN %d DVC SKIP",m_nRow - 1);
				}
			}
			break;
		}
	}
	else if (wParam == IDC_CUSTOM_HEATSINK)
	{
		switch(m_nCol)
		{
		case 1:
			if (st_picker[THD_HEATSINK_RBT].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Heat Sink Rbt : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{				
					st_picker[THD_HEATSINK_RBT].st_pcb_info[0].nYesNo = CTL_NO;
					m_strSkipData.Format("THD_HEATSINK_RBT HeatSink SKIP");
				}
			}				
			break;

		case 2:
			if (st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Heat Sink Buff Place : Do You Want SKIP?");	
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo = CTL_NO;
					m_strSkipData.Format("THD_PLACE_HEATSINK_DVC HeatSink SKIP");
				}
			}
			break;

		case 3:	
			if (st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Heat Sink Buff Pick : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{				
					st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo = CTL_NO;
					m_strSkipData.Format("THD_PICK_HEATSINK_DVC HeatSink SKIP");
				}
			}
			break;
			
		

		case 4:
			if(st_picker[THD_HEATSINK_PRBT].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Heat Sink Jig : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{				
					st_picker[THD_HEATSINK_PRBT].st_pcb_info[0].nYesNo = NO;
					m_strSkipData.Format("THD_HEATSINK_PRBT HeatSink SKIP");
				}
			}
			break;
		}
	}
	else if (wParam == IDC_CUSTOM_DEVICE)
	{	
		switch(m_nCol)
		{
		case 1:
			if (st_picker[THD_LOAD_WORK_RBT].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Dvc Work Rbt : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{
					st_picker[THD_LOAD_WORK_RBT].st_pcb_info[0].nYesNo = CTL_NO;
					m_strSkipData.Format("THD_LOAD_WORK_RBT DVC SKIP");
				}
			}				
			break;
			
		case 2:	
			if (st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Align Buff : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{				
					st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].nYesNo = CTL_NO;
					m_strSkipData.Format("THD_LD_ALIGN_BUFF DVC SKIP");
				}
			}
			break;
			
		case 3:
			if (st_buffer_info[THD_ULD_ALIGN_BUFF].st_pcb_info[0].nYesNo == CTL_YES)
			{
				st_other.str_confirm_msg = _T("Align Unload Buff : Do You Want SKIP?");
				nResponse = info_dlg.DoModal();
				if (nResponse == IDOK)
				{				
					st_buffer_info[THD_ULD_ALIGN_BUFF].st_pcb_info[0].nYesNo = CTL_NO;
					m_strSkipData.Format("THD_ULD_ALIGN_BUFF DVC SKIP");
				}
			}
			break;		
		}
	}
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg,m_strSkipData);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	}
}
//
void CScreen_Main::OnButtonTrayEditor() 
{
	if(st_work.mn_run_status == dRUN) return;
	if( st_recipe.nHsTrayX <= 0 || st_recipe.nHsTrayY <= 0)
	{
		if(st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			st_other.str_abnormal_msg = _T("Heat sink trayX, Tray Y is ZERO");
			sprintf(st_msg.c_abnormal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;
	}


	CDeviceEditdlg dlg;
	dlg.DoModal();	
}

void CScreen_Main::OnEpoxySafety() 
{
	if(st_work.mn_run_status == dRUN) return;
	CDialog_Select select_dlg;
	st_other.str_confirm_msg = _T("Warning : Would you move to Epoxy Safety pos");
	
	int iResponse = select_dlg.DoModal();
	if(iResponse == IDOK)
	{
		g_handler.ClearStep();
		SetTimer( WM_EPOXY_MOVE_SAFETY, 100, NULL );	
	}
	
}

void CScreen_Main::OnHardnessOut() 
{
	if(st_work.mn_run_status == dRUN) return;
	CDialog_Select select_dlg;
	st_other.str_confirm_msg = _T("Warning : 경화제를 강제 토출하시겠습니까?");
	
	int iResponse = select_dlg.DoModal();
	if(iResponse == IDOK)
	{
		g_handler.ClearStep();
		SetTimer( WM_HARDNEDD_OUT, 100, NULL );	
	}	
}

void CScreen_Main::OnEpoxyOut2() 
{

	if(st_work.mn_run_status == dRUN) return;
	CDialog_Select select_dlg;
	st_other.str_confirm_msg = _T("Warning : EPOXY를 강제 토출하시겠습니까?");
	
	int iResponse = select_dlg.DoModal();
	if(iResponse == IDOK)
	{
		g_handler.ClearStep();
		SetTimer( WM_EPOXY_OUT, 100, NULL );	
	}	
}

void CScreen_Main::OnButtonCleanReset() 
{
	if(st_work.mn_run_status == dRUN) return;
	CDialog_Select	select_dlg;
	st_other.str_confirm_msg = _T("Epoxy Clean시간을 리셋하시겠습니까?");
	
	int n_response = select_dlg.DoModal();
	
	if( n_response == IDOK)
	{
		st_handler.m_nEpoxyCleanAlarm = CTL_NO;
		st_handler.m_dwEpoxyCleanWaitTime[0] = GetCurrentTime();
		if( st_handler.cwnd_list != NULL )
		{
			sprintf( st_msg.c_normal_msg,"Epoxy Clean시간을 리셋했습니다.");
			st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG);
		}	
	}	
}

void CScreen_Main::OnButtonHardnessReset() 
{
	if(st_work.mn_run_status == dRUN) return;
	CDialog_Select	select_dlg;
	st_other.str_confirm_msg = _T("경화제 사용시간(480분)을 리셋하시겠습니까?");
	
	int n_response = select_dlg.DoModal();
	
	if( n_response == IDOK)
	{
		g_handler.ClearHardnessTime();
		if( st_handler.cwnd_list != NULL )
		{
			sprintf( st_msg.c_normal_msg,"경화제 사용시간을 리셋했습니다.");
			st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}		
}
