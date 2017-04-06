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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Main, CFormView)
	//{{AFX_MSG_MAP(CScreen_Main)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_LOT_OPEN, OnLotOpen)
	ON_BN_CLICKED(IDC_BTN_DOOR_OPEN, OnBtnDoorOpen)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WORK_END, OnMain_Work_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
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

	OnMain_GroupBox_Set();
	OnMain_Button_Set();
	OnMain_Label_Set();
	OnMain_Picture_Set();
	OnMain_Time_Change(mn_time_select);	// 선택 라디오 버튼 상태 변경 함수

	OnMain_Timeinfo_Display();
	OnMain_Workinfo_Display();			// Work 정보도 Lot별, Day별로 보여준다.
	OnMain_Status_Display();

	SetTimer(100, 50, NULL);
	SetTimer(200, 1, NULL);

	st_handler.cwnd_main = this;		// MAIN 화면 핸들 설정
}


/* ****************************************************************************** */
/* 테스트 결과 정보 화면에 출력하기 위한 함수                                     */
/* ****************************************************************************** */

void CScreen_Main::OnDestroy() 
{
	st_handler.cwnd_main = NULL;		// MAIN 화면 핸들 초기화
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
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


void CScreen_Main::OnMain_GroupBox_Set()
{	
	CSxLogFont main_font(16,FW_SEMIBOLD,false,"MS Sans Serif");

}

void CScreen_Main::OnMain_Label_Set()
{
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
					sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
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
					sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
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
				sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
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
				g_ioMgr.Set_Out_Bit(st_io.o_unload_safety, IO_OFF);
			}
			
			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_ready) == IO_ON)
			{
				g_ioMgr.Set_Out_Bit(st_io.o_unload_ready, IO_OFF);
			}
			
			if(st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				st_other.str_normal_msg = _T("Lot Start.");
				sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
		}
		else
		{
			st_handler.n_load_state = FALSE;

			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_safety) == IO_ON)
			{
				g_ioMgr.Set_Out_Bit(st_io.o_unload_safety, IO_OFF);
			}
			
			if(g_ioMgr.Get_Out_Bit(st_io.o_unload_ready) == IO_ON)
			{
				g_ioMgr.Set_Out_Bit(st_io.o_unload_ready, IO_OFF);
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
	}
}



void CScreen_Main::OnMain_Time_Display(int iFlag)
{
/*	int i_state;

	m_dgt_run_time.ActivateTimer(FALSE);
	m_dgt_stop_time.ActivateTimer(FALSE);
	m_dgt_alarm_time.ActivateTimer(FALSE);
//	m_dgt_maint_time.ActivateTimer(FALSE);

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
			m_dgt_stop_time.ActivateTimer();
			m_dgt_stop_time.SetTime(st_handler.m_tS.GetHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
			break;
		
		case dRUN:
			m_dgt_run_time.ActivateTimer();
			m_dgt_run_time.SetTime(st_handler.m_tR.GetHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
			break;

		case dWARNING:
		case dJAM:
			m_dgt_alarm_time.ActivateTimer();
			m_dgt_alarm_time.SetTime(st_handler.m_tJ.GetHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
			break;	
	}

	
	if(iFlag == TRUE) //현재까지의 시간을 모두 출력 해주자..
	{
		m_dgt_stop_time.SetTime(st_handler.m_tS.GetHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		m_dgt_run_time.SetTime(st_handler.m_tR.GetHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_alarm_time.SetTime(st_handler.m_tJ.GetHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
	}

	m_dgt_run_time.UpdateTimer();
	m_dgt_stop_time.UpdateTimer();
	m_dgt_alarm_time.UpdateTimer();*/
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
/*	switch(mn_time_select)
	{
	case 0:		// Total
//		m_dgt_maint_time.SetTime(st_handler.m_tDM.GetTotalHours(), st_handler.m_tDM.GetMinutes(), st_handler.m_tDM.GetSeconds());
		m_dgt_run_time.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
		m_dgt_alarm_time.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
		m_dgt_stop_time.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		break;

	case 1:		// Lot
//		m_dgt_maint_time.SetTime(st_handler.m_tM.GetTotalHours(), st_handler.m_tM.GetMinutes(), st_handler.m_tM.GetSeconds());
		m_dgt_run_time.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_alarm_time.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
		m_dgt_stop_time.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		break;
	}*/
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

	if(nIDEvent == TM_TIME)
	{
		OnMain_Time_Display();
	}
	CFormView::OnTimer(nIDEvent);
}

/* ****************************************************************************** */
/* 테스트 결과 정보 화면에 출력하기 위한 함수                                     */
/* ****************************************************************************** */
LRESULT CScreen_Main::OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam)   
{
	switch(wParam)
	{
		case MAIN_LOTINFO:
			OnMain_Display_Lot_Info();
			break;
	}


/*	switch(wParam)
	{
		case  MAIN_DEVICE_PART:
			OnMain_Device_Display(lParam);
			break;

		case MAIN_BARCODE_PART:
//			OnMain_Barcode_Display(lParam);
			break;

		case MAIN_THICKNESS_PART:
			OnMain_Thickness_Display(lParam);
			break;

		case MAIN_LENGTH_PART_1:
			OnMain_Length_Display_1(lParam);
			break;

		case MAIN_LENGTH_PART_2:
			OnMain_Length_Display_2(lParam);
			break;

		case MAIN_LENGTH_PART_3:
			OnMain_Length_Display_3(lParam);
			break;

		case MAIN_TEST_SITE_OUT_PART:
			OnMain_Test_Site_Display(lParam);
			break;

		case MAIN_DEVICE_CHK:
			if(st_data.by_device == YES)
			{
				m_msg_device_chk.SetBkColor(BLUE_C);
			}
			else
			{
				m_msg_device_chk.SetBkColor(BLACK_L);
			}
			break;

		case MAIN_TURN_TABLE_CHK:
			OnMain_Buffer_Display();
			break;

		case MAIN_TEST_SITE_CHK:
			OnMain_Test_Site_Display();
			break;

		case MAIN_PASS_TRAY_L_DISPLAY:
			OnMain_Pass_Tray_Left_Display(lParam);
			break;

		case MAIN_ELEVATOR_LEFT:
			OnMain_Elevator_Left();
			break;

		case MAIN_PASS_TRAY_R_DISPLAY:
			OnMain_Pass_Tray_Right_Display(lParam);
			break;

		case MAIN_ELEVATOR_RIGHT:
			OnMain_Elevator_Right();
			break;

		case MAIN_REJECT_TRAY_DISPLAY:
			OnMain_Reject_Tray_Display();
			break;

		case MAIN_COUNT_INFO:
			OnMain_Count();
			break;

		case MAIN_CYCLE_TIME:
			m_dgt_cycle_time.SetFloatVal(((float)st_work.n_cycle_time / (float)2) / (float)1000);
			break;

		case MAIN_LOT_END_CHECK:
			if(lParam == 0)
			{
				Run_Conveyor.Manual_Step = 0;

				m_chk_conveyor_mode.SetCheck(TRUE);
				st_basic.n_conveyor_move_mode = 1;
				SetDlgItemText(IDC_CHK_CONVEYOR_MODE, "수동 콘베어 이동");
			}
			else
			{
				m_chk_conveyor_mode.SetCheck(FALSE);
				SetDlgItemText(IDC_CHK_CONVEYOR_MODE, "자동 콘베어 이동");
			}

			OnMain_Status_Display();
			break;

		case MAIN_INDEX_DUMP_CHK:
			if(lParam == 0)
			{
				m_msg_index_dump.SetBkColor(BLACK_L);
			}
			else
			{
				m_msg_index_dump.SetBkColor(BLUE_C);
			}
			break;

		case MAIN_TEST_DUMP_CHK:
			if(lParam == 0)
			{
				m_msg_test_dump.SetBkColor(BLACK_L);
			}
			else
			{
				m_msg_test_dump.SetBkColor(BLUE_C);
			}
			break;
	}*/
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

void CScreen_Main::OnLotOpen() 
{
	CDialog_Data_Lot dlg;
	dlg.DoModal();
}


void CScreen_Main::OnMain_Display_Lot_Info()
{

	if( g_lotMgr.GetLotCount() > 0 )
	{
		GridData(IDC_CUSTOM_LOT_INFO, 1, 2, g_lotMgr.GetLotIDAt( 0 ) );
		GridData(IDC_CUSTOM_LOT_INFO, 2, 2, g_lotMgr.GetLotAt(0).GetPartID() );
		GridData(IDC_CUSTOM_LOT_INFO, 3, 2, g_lotMgr.GetLotAt(0).GetStrLastModule());
		GridData(IDC_CUSTOM_LOT_INFO, 4, 2, g_lotMgr.GetLotAt(0).GetStrRProtyModule());
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

