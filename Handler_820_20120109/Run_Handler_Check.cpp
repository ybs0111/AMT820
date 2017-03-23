// Run_Handler_Check.cpp: implementation of the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Handler_Check.h"
#include "Variable.h"				// 전역 변수 정의 Class
#include "Public_Function.h"		// Motor 및 Cylinder 동작 Class
#include "FastechPublic_IO.h"			// Cylinder 동작 Class
#include "ComizoaPublic.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_Handler_Check Run_Handler_Check;

CRun_Handler_Check::CRun_Handler_Check()
{
	FrontDoorStep = 0;
	m_n_separator_motor = NO;
}

CRun_Handler_Check::~CRun_Handler_Check()
{

}

void CRun_Handler_Check::ButtonCheck_Start()
{
	CString str_tmp;
	// =============================================================================
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // =============================================================================
	if(st_handler.n_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// =============================================================================

	if(st_handler.n_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.
	if(st_handler.n_lot_flag	== CTL_LOTEND_FINISH)	return;
	if(st_work.n_run_status == dRUN)		return;


	switch(StartStep)
	{
		case 0:
			// =============================================================================
			// RUN 키 I/O 입력이 On 상태이고 상태가 STOP 상태였는지 검사             
			// -> 다시 말해, STOP 상태에서만 RUN 키 입력이 동작한다                  
			// =============================================================================
			if(st_work.n_run_status == dSTOP)
			{
//				if(FAS_IO.Get_In_Bit(st_io.i_start_chk) == IO_ON)
				if(FAS_IO.get_in_bit(st_io.i_Start_SwitchCheck, IO_ON) == IO_ON)
				{
					SwitchWaitTime[0] = GetCurrentTime();
					StartStep = 100;
				}
				else if(st_handler.i_main_screen_start_req == TRUE)
				{
					st_handler.i_main_screen_start_req = FALSE;
					StartStep = 600;
				}
			}
			break;

		case 100: 
//			if(FAS_IO.Get_In_Bit(st_io.i_start_chk) == IO_ON)
			if(FAS_IO.get_in_bit(st_io.i_Start_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[1] = GetCurrentTime();
				SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

				if(SwitchWaitTime[2] < 0)
				{
					SwitchWaitTime[0] = GetCurrentTime();
				}
				else if(SwitchWaitTime[2] > 100)
				{
					StartStep = 300;
				}
			}
			else
			{
				StartStep = 0;
			}
			break;

		case 300:
			// =============================================================================
			// RUN 키 I/O 입력이 Off 되었는지 검사                                   
			// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
			// =============================================================================
//			if(FAS_IO.Get_In_Bit(st_io.i_start_chk) == IO_OFF)
			if(FAS_IO.get_in_bit(st_io.i_Start_SwitchCheck, IO_OFF) == IO_OFF)
			{
				StartStep = 600;
			}
			// =============================================================================
			break;

		case 600:
			if(Run_Possible_Check())
			{
				if(st_handler.n_alarm_screen != FALSE)
				{						// 알람 화면이 떠 있다면..
					if(st_work.n_run_status == dWARNING)
					{					// 알람 발생으로 뜬 알람 화면인 경우(우선 stop상태로 만듬)
						st_work.n_run_status = dSTOP;
					}

					::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);	// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
				}

				if(st_handler.n_menu_num != 101)
				{
					::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
				}
				
				memset(Reinstatement, NO, sizeof(Reinstatement));
				StartStep = 700;
			}
			else{
				StartStep = 0;
			}
			break;

		case 700:
			if(st_work.n_run_status != dSTOP) break;	//stop상태에서 run되는 경우만 동작하기 위함

				
			i_stop_press_cnt = 3;
			StartStep = 0;
			break;
		
		default :
			StartStep = 0;
			break;
	}
}

void CRun_Handler_Check::ButtonCheck_Stop()
{
	CString str_tmp;

	switch(StopStep)
	{
		case 0:
			// =============================================================================
			// STOP 키 I/O 입력이 On 상태이고 현재 STOP 상태가 아닌지 검사           
			// =============================================================================
//			if(FAS_IO.Get_In_Bit(st_io.i_stop_chk) == IO_ON && st_work.n_run_status != dSTOP)
			if(FAS_IO.get_in_bit(st_io.i_Stop_SwitchCheck, IO_ON) == IO_ON && st_work.n_run_status != dSTOP)
			{
				if(st_work.n_run_status == dINIT)
				{				// 초기화 중에 Stop 버튼이 눌리면... 2K5/02/23/ViboX
				
					st_init.n_initial_error = TRUE;

					//900003 0 90 "Handler Init About Error."
					alarm.mstr_code = _T("900003");		// Alarm 화면을 띄워준다. 2K5/03/10/ViboX
					alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
					alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
					COMI.mn_run_status = dWARNING;	// dJAM,dWARNING 

					alarm.n_alarm_assign_section = 300;
					break;
				}
	
				StartStep = 0;
				StopStep = 100;
				i_stop_press_cnt = 0;
			}
//			else if(FAS_IO.Get_In_Bit(st_io.i_stop_chk) == IO_ON)
			else if(FAS_IO.get_in_bit(st_io.i_Stop_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[0] = GetCurrentTime();
				StopStep = 100;
			}
			break;

		case 100:
			SwitchWaitTime[1] = GetCurrentTime();
			SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

			if(SwitchWaitTime[2] < 0)
			{
				SwitchWaitTime[0] = GetCurrentTime();
			}
			else if(SwitchWaitTime[2] > 10)
			{
				StopStep = 200;
			}
			break;

		case 200:
			// =============================================================================
			// STOP 키 I/O 입력이 Off 되었는지 검사                                  
			// =============================================================================
//			if(FAS_IO.Get_In_Bit(st_io.i_stop_chk) == IO_OFF)
			if(FAS_IO.get_in_bit(st_io.i_Stop_SwitchCheck, IO_OFF) == IO_OFF)
			{
				//stop key2회 눌릴시에 picker up하기 위함 ============================================
//				if(st_work.b_lot_start_flag != false){  //lot진행중에만 동작하기 위함~
				if(st_handler.n_lot_flag != FALSE)
				{
				
					i_stop_press_cnt++;
					if(i_stop_press_cnt == 2)
					{  //실제로 picker up
					
						//i_stop_press_cnt = 0;

					}
				}
				//=================================================================================================================
				//st_inspect.n_tray_set_request = 0;

				Func.OnSet_IO_Port_Stop();
//				FAS_IO.Set_Out_Bit(st_io.o_buzzer, IO_OFF);
//				FAS_IO.Set_Out_Bit(st_io.o_main_area_door_lock_cylinder,	IO_OFF);
				if(st_handler.cwnd_title != NULL)
				{
					st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.n_run_status);
				}

				if (FAS_IO.Get_Out_Bit(st_io.o_Buzzer1) == IO_OFF && alarm.mn_reset_status != NO)
				{
					alarm.mn_reset_status = NO;
					
					if(st_handler.cwnd_list != NULL)
					{
						st_other.str_normal_msg = _T("[JAM RESET] JAM RESET 되었습니다.");
						sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}
				}

				Func.On_LogFile_Add(99, str_tmp);

				StopStep = 0; 
			}
			break;

		default : 
			break;
	}
}	

void CRun_Handler_Check::ButtonCheck_Reset()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	switch(ResetStep)
	{
		case 0:
			// **********************************************************************
			//  Reset 키 I/O 입력이 On 상태인지 확인                                 
			// **********************************************************************
//			if(FAS_IO.Get_In_Bit(st_io.i_reset_chk) == IO_ON)
			if(FAS_IO.get_in_bit(st_io.i_AlarmClear_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[0] = GetCurrentTime();
				ResetStep = 100;
			}
			break;

		case 100: 
//			if(FAS_IO.Get_In_Bit(st_io.i_reset_chk) == IO_ON)
			if(FAS_IO.get_in_bit(st_io.i_AlarmClear_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[1] = GetCurrentTime();
				SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

				if(SwitchWaitTime[2] < 0)
				{
					SwitchWaitTime[0] = GetCurrentTime();
				}
				else if(SwitchWaitTime[2] > 100)
				{
					ResetStep = 200;
				}
			}
			else
			{
				ResetStep = 0;
			}
			break;

		case 200:
			// **********************************************************************
			// Reset 키 I/O 입력이 Off 되었는지 검사                                 
			// **********************************************************************
//			if(FAS_IO.Get_In_Bit(st_io.i_reset_chk) == IO_OFF)
			if(FAS_IO.get_in_bit(st_io.i_AlarmClear_SwitchCheck, IO_OFF) == IO_OFF)
			{
				if (alarm.mn_reset_status == YES)
				{
					alarm.mn_reset_status = NO;
					st_other.str_normal_msg = _T("[JAM RESET] JAM RESET.");
					sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
					if (st_handler.cwnd_list != NULL)
					{
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}
//					FAS_IO.Set_Out_Bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
					FAS_IO.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
					if(st_work.n_run_status != dRUN)
					{
						Func.OnSet_IO_Port_Stop();
					}
					Func.OnSet_IO_Port_Sound(IO_OFF);
				}
				else
				{
					st_other.str_normal_msg = _T("[SOUND] OFF.");
					sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
					if (st_handler.cwnd_list != NULL)
					{
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}
					Func.OnSet_IO_Port_Sound(IO_OFF);
				}
				ResetStep = 0;
			}
			break;

		default : 
			break;
	}
}

void CRun_Handler_Check::ButtonCheck_BuzzerOff()
{
	// 20100805 tae 수정 
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	switch(SoundStep)
	{
		case 0:
			// **********************************************************************
			//  Reset 키 I/O 입력이 On 상태인지 확인                                 
			// **********************************************************************
//			if(FAS_IO.Get_In_Bit(st_io.i_buzzer_chk) == IO_ON)
			if(FAS_IO.get_in_bit(st_io.i_Buzzer_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[0] = GetCurrentTime();
				SoundStep = 100;
			}
			break;
			
		case 100: 
//			if(FAS_IO.Get_In_Bit(st_io.i_buzzer_chk) == IO_ON)
			if(FAS_IO.get_in_bit(st_io.i_Buzzer_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[1] = GetCurrentTime();
				SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

				if(SwitchWaitTime[2] < 0)
				{
					SwitchWaitTime[0] = GetCurrentTime();
				}
				else if(SwitchWaitTime[2] > 100)
				{
					SoundStep = 200;
				}
			}
			else
			{
				SoundStep = 0;
			}
			break;
			
		case 200:
			// **********************************************************************
			// Reset 키 I/O 입력이 Off 되었는지 검사                                 
			// **********************************************************************
//			if(FAS_IO.Get_In_Bit(st_io.i_buzzer_chk) == IO_OFF)
			if(FAS_IO.get_in_bit(st_io.i_Buzzer_SwitchCheck, IO_OFF) == IO_OFF)
			{
//				if(FAS_IO.Get_Out_Bit(st_io.o_buzzer_lamp) == IO_ON)
				if(FAS_IO.get_in_bit(st_io.o_Buzzer_SwitchLamp, IO_ON) == IO_ON)
				{
					Func.OnSet_IO_Port_Sound(IO_OFF);
				}
				
				SoundStep = 0;
			}
			break;
			
		default : 
			break;
	}
}

void CRun_Handler_Check::ButtonCheck_Loader()
{
	// =============================================================================
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // =============================================================================
	if(st_handler.n_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// =============================================================================

	if(st_handler.n_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.
	if(st_work.n_run_status != dSTOP) return;
/*
	switch(LoaderStep){
		case 0:
			// =============================================================================
			// LOAD 키 I/O 입력이 On 상태인지 검사                                   
			// =============================================================================
			if(FAS_IO.Get_In_Bit(st_io.i_loader_chk) == TRUE){
				LoaderStep = 100;
			}
			break;

		case 100:
			// =============================================================================
			// 로더 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함         
			// =============================================================================
			SwitchWaitTime[0] = GetCurrentTime();
			LoaderStep = 200;
			break;

		case 200: 
			SwitchWaitTime[1] = GetCurrentTime();
			SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

			if(SwitchWaitTime[2] > 50 && FAS_IO.Get_In_Bit(st_io.i_loader_chk) == FALSE){
				SwitchWaitTime[0] = GetCurrentTime();
				LoaderStep = 300;
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_loader_chk) == FALSE){
				LoaderStep = 0;
			}
			break;

		case 300: 
			SwitchWaitTime[1] = GetCurrentTime();
			SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

			if(SwitchWaitTime[2] > 10){
				if(FAS_IO.Get_In_Bit(st_io.i_loader_chk) == FALSE){
					LoaderStep = 350;
				}
				else{
					LoaderStep = 0;
				}
			}
			// =============================================================================
			break;

		case 350:
			if(st_handler.n_motor_init_check != YES){	// 모터 초기화를 하지 않은 상태라면..
			
				st_other.str_abnormal_msg = _T("[MOTOR INIT CHECK] Motor Initial..., become Run.");
				if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

				LoaderStep = 0;
			}
			else{
				LoaderStep = 400;
			}
			break;

		case 400 :
			// =============================================================================
			// 알람 화면 출력된 경우 알람 화면 종료 -> 메인 화면으로 전환            
			// =============================================================================
			if(st_handler.n_alarm_screen != FALSE){
				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
			}
			
			if(st_handler.n_menu_num != 101){  // 현재 출력된 화면이 메인 화면인지 검사
			
				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
			}
			LoaderStep = 500;
			break;

		case 500:
			if(st_handler.n_menu_num != 101){  // 현재 출력된 화면이 메인 화면인지 검사
			
				break;
			}

			if(st_handler.n_load_state != TRUE){
				if(st_handler.cwnd_main != NULL){
					st_handler.n_load_state = TRUE;  // 현재 로딩 버튼 관련 대화 상자 출력 요청 중 플래그 설정 
//					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOADERSET, 0);
				}
			}

			LoaderStep = 0;
			break;

		default :
			break;
	}
*/
}

int CRun_Handler_Check::Run_Possible_Check()
{
	// ========================================================================
	//  RUN할수 있는 조건인지 검사한다. 이상이 없어야 RUN이 된다.            
	// ========================================================================
	if(st_handler.n_motor_init_check != YES){		//모터 초기화가 안된 상태라면..
	
		st_other.str_abnormal_msg = _T("[MOTOR INIT CHECK] Motor Initial..., become Run.");
		sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
		if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

		return FALSE;
	}

	if(alarm.mn_reset_status != NO){					// JAM이 리셋이 안된 상태라면..
	
		st_other.str_abnormal_msg = _T("[JAM CONFIRM] If do to do Run, do first Jam Reset.");
		sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
		if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

		return FALSE;
	}

	if(Func.DoorOpenCheck() == RET_ERROR){
		st_other.str_abnormal_msg = _T("[DOOR CHECK] Door Open being done Run become.");
		sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
		if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		
		alarm.mn_count_mode = 0;	
		alarm.mn_type_mode = eWARNING;
		st_work.n_run_status = dWARNING;

		return FALSE;
	}

	return TRUE;
}

void CRun_Handler_Check::HandlerCheck_RejectTray()
{	
    
}

void CRun_Handler_Check::OnReject_Tray_Reset()
{

}


