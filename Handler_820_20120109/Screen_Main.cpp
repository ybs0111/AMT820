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
	DDX_Control(pDX, IDC_CHECK1, m_changeTop_Btm);
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
	
	//kwlee 2017.0412
	OnInitTopGrid();
	OnInitBTMGrid();
	OnInitGridRef();

	SetTimer(100, 50, NULL);
	SetTimer(200, 1, NULL);

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

void CScreen_Main::OnLotOpen() 
{
	CDialog_Data_Lot dlg;
	dlg.DoModal();
}
//kwlee 2017.0414
//kwlee 2017.0414
void CScreen_Main::OnMainTop_Loader_Recive_Info()
{

	CString strTemp[4];
	int nTmp[4] = {0,};
	
	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);

	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[i]);

		if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == CTL_YES)
		{

			if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 7, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 7, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 7, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Top, i+2, 7,strTemp[i]);

	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainTop_Input_Loader_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
	
	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].c_chBarcode[i]);

		if( st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER ].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2,6 ,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 6, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 6, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 6, GRAY, WHITE_C);	
		m_pGrid.GridCellText(m_grid_Top, i+2, 6,strTemp[i]);

	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainTop_Epoxy_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
	
	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].c_chBarcode[i]);

		if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[i] == CTL_YES )
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5 ,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 5, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 5, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 5, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Top, i+2, 5,strTemp[i]);
	
	}
	Invalidate(FALSE);
}
void CScreen_Main::OnMainTop_Wait_Index_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
	
	for (int i = 0; i<3; i++)
	{
		
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].c_chBarcode[i]);

		if( st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX ].n_exist[i] == CTL_YES)
		{

			if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4 ,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 4, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 4, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 4, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Top, i+2, 4,strTemp[i]);
			
		
	}
	Invalidate(FALSE);
}
void CScreen_Main::OnMainTop_Hs_Vision_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
	
	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i]);

		if( st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION ].n_exist[i] == CTL_YES )
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3 ,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 3, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 3, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 3, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Top, i+2, 3,strTemp[i]);
			
		
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainTop_OutSend_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);

	for (int i = 0; i<3; i++)
	{
		
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].c_chBarcode[i]);
		
		if( st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND ].n_exist[i] == CTL_YES )
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2 ,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 2, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 2, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 2, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Top, i+2, 2,strTemp[i]);	
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainTop_Unloader_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
	for (int i = 0; i<3; i++)
	{	
		
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].c_chBarcode[i]);

		if( st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER ].n_exist[i] == CTL_YES)
		{
			if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_CDIMM)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, BLUE, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1 ,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_EPOXY)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, NOR_L, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_HEATSINK)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, BLUE_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_VISION)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, GREEN_D, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);	
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_GOOD)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, OK_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_FAIL)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, NG_C, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
			}
			else if (st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].nBin[i] == BIN_NONE)
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, GRAY, YELLOW_C);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
			}
			else
			{
				m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
				m_pGrid.GridCellColor(m_grid_Top, i+2, 1, DVC_UNLOAD_C, BLACK);
				m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);
			}
		}
		m_pGrid.GridCellFont(m_grid_Top, i+2, 1, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Top, i+2, 1, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Top, i+2, 1,strTemp[i]);

	}
	Invalidate(FALSE);
}
//////

//kwlee 2017.0416
void CScreen_Main::OnMainBtm_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};


	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);

	for (int i = 0; i<3; i++)
	{
		
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[i] == CTL_YES) 
			
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 1, "MS Sans Serif", 10);
 			m_pGrid.GridCellColor(m_grid_Btm, i+2, 1, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 1,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 1, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 1, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Btm, i+2, 1,strTemp[i]);
			
		
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainBtm_DownForward_Info()
{
	
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	for (int i = 0; i<3; i++)
	{
		
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_exist[i] == CTL_YES)
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 2, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i+2, 2, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 2,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 2, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 2, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Btm, i+2, 2,strTemp[i]);
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainBtm_Hs_Down_Info()
{	
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_exist[i] == CTL_YES)
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 3, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i+2, 3, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 3,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 3, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 3, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Btm, i+2, 3,strTemp[i]);
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainBtm_Index_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};
	
	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	for (int i = 0; i<3; i++)
	{

		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_exist[i] == CTL_YES)
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 4, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i+2, 4, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 4,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 4, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 4, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Btm, i+2, 4,strTemp[i]);
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainBtm_Epoxy_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};


	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_exist[i] == CTL_YES)
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 5, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i+2, 5, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 5,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 5, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 5, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Btm, i+2, 5,strTemp[i]);
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainBtm_Input_Down_Info()
{
	
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	for (int i = 0; i<3; i++)
	{
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_exist[i] == CTL_YES)
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 6, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i+2, 6, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 6,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 6, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 6, GRAY, WHITE_C);	
		m_pGrid.GridCellText(m_grid_Btm, i+2, 6,strTemp[i]);
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMainBtm_Loader_Down_Info()
{
	CString strTemp[4];
	int nTmp[4] = {0,};

	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	for (int i = 0; i<3; i++)
	{
		
		strTemp[i].Format("Bcr : %s",st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].c_chBarcode[i]);

		if(st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_exist[i] == CTL_YES)
		{
			m_pGrid.GridCellFont(m_grid_Btm, i+2, 7, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i+2, 7, DVC_UNLOAD_C, BLACK);
			m_pGrid.GridCellText(m_grid_Btm, i+2, 7,strTemp[i]);
		}
		m_pGrid.GridCellFont(m_grid_Btm, i+2, 7, "MS Sans Serif", 10);
		m_pGrid.GridCellColor(m_grid_Btm, i+2, 7, GRAY, WHITE_C);
		m_pGrid.GridCellText(m_grid_Btm, i+2, 7,strTemp[i]);
	}
	Invalidate(FALSE);
}
////////

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



void CScreen_Main::OnInitTopGrid()
{
	int   i, j;
	int	  max_row, max_col;
	CString str_tmp;
	
	max_row = 4;
	max_col = 7;
	
	m_grid_Top = (TSpread*)GetDlgItem(IDC_CUSTOM_TOP_INFO);
	
	m_pGrid.GridReset(m_grid_Top);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(m_grid_Top, TRUE);
	m_pGrid.GridRowHeader(m_grid_Top, FALSE);
	m_pGrid.GridColHeader(m_grid_Top, FALSE);
	m_pGrid.GridHorizontal(m_grid_Top, FALSE);
	m_pGrid.GridVertical(m_grid_Top, FALSE);
	m_pGrid.GridAutoSize(m_grid_Top, FALSE);
	//m_pGrid.GridAutoSize(m_grid_Top, FALSE);
	m_pGrid.GridCellRows(m_grid_Top, max_row);
	m_pGrid.GridCellCols(m_grid_Top, max_col);
	
	for(i=0; i<max_row+1; i++)
	{
		
		if (i == 0)
		{
			m_pGrid.GridCellHeight(m_grid_Top, i, 30);	 
		}
		else
		{
			m_pGrid.GridCellHeight(m_grid_Top, i+ 1, 50);	 
		}
		for(j=0; j<max_col+1; j++)
		{
			
			m_pGrid.GridCellWidth_L(m_grid_Top, j, 18);
			m_pGrid.GridCellFont(m_grid_Top, i, j, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Top, i, j, GRAY, YELLOW_C);
		}
		
	}
	
// 	m_pGrid.GridCellMerge(m_grid_Top, 1, 1, 1, 2);
 	m_pGrid.GridCellFont(m_grid_Top, 1, 1, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 1, BLUE, YELLOW_C);
 	m_pGrid.GridCellText(m_grid_Top, 1, 1, "Unload");

// 	m_pGrid.GridCellMerge(m_grid_Top, 2, 1, 1, 2);
 	m_pGrid.GridCellFont(m_grid_Top, 1, 2, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 2, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Top, 1, 2, "OutSend");
// 	
// 	m_pGrid.GridCellMerge(m_grid_Top, 3, 1, 1, 2);
 	m_pGrid.GridCellFont(m_grid_Top, 1, 3, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 3, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Top, 1, 3, "Hs Vsion");
// 	
// 	m_pGrid.GridCellMerge(m_grid_Top, 4, 1, 1, 2);
 	m_pGrid.GridCellFont(m_grid_Top, 1, 4, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 4, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Top, 1, 4, "Wait index");

	m_pGrid.GridCellFont(m_grid_Top, 1, 5, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 5, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Top, 1, 5, "Epoxy");

	m_pGrid.GridCellFont(m_grid_Top, 1, 6, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 6, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Top, 1, 6, "Input Load");

	m_pGrid.GridCellFont(m_grid_Top, 1, 7, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Top, 1, 7, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Top, 1, 7, "Load");

	
	Invalidate(FALSE);
}


void CScreen_Main::OnInitBTMGrid()
{
	int   i, j;
	int	  max_row, max_col;
	CString str_tmp;
	
	max_row = 4;
	max_col = 7;
	
	m_grid_Btm = (TSpread*)GetDlgItem(IDC_CUSTOM_BTM_INFO);
	
	m_pGrid.GridReset(m_grid_Btm);
	// 대문자 
//	m_pGrid.GridCellSelectDisplay(m_grid_Btm, TRUE);
	m_pGrid.GridRowHeader(m_grid_Btm, FALSE);
	m_pGrid.GridColHeader(m_grid_Btm, FALSE);
	m_pGrid.GridHorizontal(m_grid_Btm, FALSE);
	m_pGrid.GridVertical(m_grid_Btm, FALSE);
	//m_pGrid.GridAutoSize(m_grid_Btm, FALSE);
	m_pGrid.GridAutoSize(m_grid_Btm, FALSE);
	m_pGrid.GridCellRows(m_grid_Btm, max_row);
	m_pGrid.GridCellCols(m_grid_Btm, max_col);
		
	for(i=0; i<max_row+1; i++)
	{
		
		if (i == 0)
		{
			m_pGrid.GridCellHeight_L(m_grid_Btm, i, 30);
		}
		else
		{
			m_pGrid.GridCellHeight_L(m_grid_Btm, i+ 1, 50);
		}
		
		 

		for(j=0; j<max_col+1; j++)
		{
			m_pGrid.GridCellWidth_L(m_grid_Btm, j, 18);
			/*m_pGrid.GridCellMerge(m_grid_Top, i, j, 1, 2);*/
			m_pGrid.GridCellFont(m_grid_Btm, i, j, "MS Sans Serif", 10);
			m_pGrid.GridCellColor(m_grid_Btm, i, j, GRAY, YELLOW_C);	
		}
	}
	
// 	m_pGrid.GridCellMerge(m_grid_Btm, 1, 1, 1, 2);
	m_pGrid.GridCellFont(m_grid_Btm, 1, 1, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 1, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Btm, 1, 1, "Loader Down");
// 	
// 	m_pGrid.GridCellMerge(m_grid_Btm, 2, 1, 1, 2);
 	m_pGrid.GridCellFont(m_grid_Btm, 1, 2, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 2, BLUE, YELLOW_C);
 	m_pGrid.GridCellText(m_grid_Btm, 1, 2, "Input Down");

// 	m_pGrid.GridCellMerge(m_grid_Btm, 3, 1, 1, 2);
	m_pGrid.GridCellFont(m_grid_Btm, 1, 3, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 3, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Btm, 1, 3, "Epoxy Down");
// 	
// 	m_pGrid.GridCellMerge(m_grid_Btm, 4, 1, 1, 2);
	m_pGrid.GridCellFont(m_grid_Btm, 1, 4, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 4, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Btm, 1, 4, "Index Down");

	m_pGrid.GridCellFont(m_grid_Btm, 1, 5, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 5, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Btm, 1, 5, "Hs Down");

	m_pGrid.GridCellFont(m_grid_Btm, 1, 6, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 6, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Btm, 1, 6, "Down Fwd");

	m_pGrid.GridCellFont(m_grid_Btm, 1, 7, "MS Sans Serif", 10);
 	m_pGrid.GridCellColor(m_grid_Btm, 1, 7, BLUE, YELLOW_C);
	m_pGrid.GridCellText(m_grid_Btm, 1, 7, "Buff Down");
	Invalidate(FALSE);
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
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 2, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 2, NG_C, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 2, "FAIL");
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 3, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 3, GRAY, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 3, "DVC_NO");
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 4, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,4, DVC_UNLOAD_C, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 4, "UNLOAD");


	m_pGrid.GridCellFont(m_grid_Ref, 1, 5, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,5, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 5, "HS INSP");
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 6, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,6, BLUE_D, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 6, "HEAT SINK");

	m_pGrid.GridCellFont(m_grid_Ref, 1, 7, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,7, ORANGE_C, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 7, "EPOXY INSP");
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 8, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1,8, NOR_L, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 8, "EPOXY");
	
	//m_pGrid.GridCellMerge(m_grid_Btm, 1, 1, 1, 2);
	m_pGrid.GridCellFont(m_grid_Ref, 1, 9, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 9, SKY_C, BLACK);
	m_pGrid.GridCellText(m_grid_Ref, 1, 9, "READY");//! khj[20101012]
	
	m_pGrid.GridCellFont(m_grid_Ref, 1, 10, "MS Sans Serif", 10);
	m_pGrid.GridCellColor(m_grid_Ref, 1, 10, BLUE, WHITE_C);
	m_pGrid.GridCellText(m_grid_Ref, 1, 10, "LOAD");
		
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
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Lock_Sol, IO_ON );
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Unlock_Sol, IO_OFF );
}

void CScreen_Main::OnTray2Unlock() 
{
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Lock_Sol, IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Unlock_Sol, IO_ON );	
}

void CScreen_Main::OnTray1Lock() 
{
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Lock_Sol, IO_ON );
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Unlock_Sol, IO_OFF );
}

void CScreen_Main::OnTray1Unlock() 
{
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Lock_Sol, IO_OFF );
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Unlock_Sol, IO_ON );
}

void CScreen_Main::OnCheck1() 
{
// 	if (m_changeTop_Btm.GetCheck() == TRUE)	mn_buzzer_mode[1] = TRUE;
// 	else
	
}
