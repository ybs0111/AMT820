// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "IO_Manager.h"
#include "SrcPart/APartHandler.h"

#include "Srcbase\ALocalization.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtlBd_Library::CCtlBd_Library()
{

}

CCtlBd_Library::~CCtlBd_Library()
{

}

CCtlBd_Library CTL_Lib;

//////////////////////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 
//알람발생 처리방법, 모터 및 I/O 관련 라이브러리를 초기화 및 응용하여 실제사용하는 함수정리 
//
//참고 기본함수 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{
	//그간은 알람이 발생하면 각 쓰레드에서 jamcode 및 run status 정보를 바뀌어 장비를 정지하고 
	//문제를 해결했으나 앞으로는 이 함수를 모든곳에서 사용하여 알람정보 및 가동 상태를 표준함수 하나를 
	//사용하여 장비 가동 및 알람 문제를 처리한다 
	//
	//각 쓰레드에 각각있던 알람 처리방법은 이 함수를 모두 사용한다 
   //
	int n_cur_num = -1;

	CTL_Lib.mn_jampos = n_jampos; //jam이 발생한 위치를 저장한다 
	CTL_Lib.mn_run_status = n_run_status; //장비의 가동상태를 변경한다 
	COMI.mn_run_status = n_run_status;
	st_work.m_iRunStatus = n_run_status;
	alarm.mstr_code = c_alarmcode;
	alarm.stl_cur_alarm_time = GetCurrentTime();

	strcpy(mc_alarmcode, c_alarmcode);

	for(int n_cnt=0; n_cnt<2000; n_cnt++)
	{  
		if ((st_alarm.mstr_e_code[n_cnt]).Compare(alarm.mstr_code) == 0)
		{
			n_cur_num = n_cnt; 
			break;
		}
	}

	if (n_cur_num >= 0)
	{
		sprintf(st_msg.c_abnormal_msg, st_alarm.mstr_e_msg[n_cur_num]);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
	}

// 	st_handler.mn_dumping_alarm_count++;
// 
// 	sprintf( st_msg.c_normal_msg, "Dumping Test Pick & Place SSD : %d / %d" ,  st_handler.mn_dumping_alarm_count, st_handler.mn_dumping_test_count);
// 	st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG );

	//st_handler.m_tDMTBI = 0;
	
	CString mstr_temp;

	return BD_GOOD;
}


int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor 보드 초기화시 사용
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if( n_bd_type == 0 ) //0:파스텍 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{		//모터 보드별 축수를 정의한다 

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 
 	}
	else if( n_bd_type == 1 ) //1:커미조아 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{
		//1. 보드 정보 읽어들인다 
		//2. 각종 셋팅 정보를 적용한다 
		nRet = COMI.Initialize_MotBoard(s_filename);			// MOTOR 보드 초기화 여부 검사한다
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD] 초기화 에러.");
			LogFile_Write("D:\\AMT_LOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 

		if(COMI.mn_motorbd_init_end == BD_YES)
		{			
			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
			{//n_simul_mode => 0:시뮬레이션 모드 비활성, 1:시뮬레이션 모드 활성

				if(COMI.mn_simulation_mode == 1) //모터를 가상으로 제어하여 실제 모터 출력이 나가고 기구물이 이동한것처럼 동작한다 
				{//활성
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//비활성 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}
	}

	return  BD_GOOD;
}

//아래 내용은 I/O 보드 초기화시 필요한 내용을 예로 들은것이니 
//장비별 필요한 내용을 셋팅하여 사용하면 된다.
int CCtlBd_Library::Initialize_io_Board(int n_bd_type)
{
	//if(n_bd_type == 0) //파스텍 HSSI I/O board 
	//{
	//}
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	

	/////////////////초기에 초기화시 //////////////////////////////////////////////////////////////////////
	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	if (Ret == FALSE)
	{
		return BD_ERROR;
	}

	FAS_IO.mn_io_board_initial_flag = BD_YES;
	
	return BD_GOOD;
}


int CCtlBd_Library::Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos, double d_jogcurrentpos) 	//모터 안전 정보 셋팅시 사용 
{
	// 예) 
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog, 4:Length Change
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet1 = 0, nRet2 = 0, nRet3 = 0, nRet4 = 0, nRet5 = 0, nRet6 = 0;
	double d_CurPos[M_MOTOR_COUNT] = {0,};
	double d_Pos[4] = {0,};
	double d_GapCheck = 0;
	int i = 0;

	if (d_jogcurrentpos != -1000)
	{
		d_CurPos[n_axis] = d_jogcurrentpos;
	}
	// **************************************************************************
	// 모터 알람 상태 검사한다                                                   
	// -> 알람 발생한 경우 알람 해제한다                                         
	// **************************************************************************
	//software limit 값 셋팅 체크 에러 
	if(d_targetpos < COMI.md_limit_position[n_axis][0] && d_targetpos < d_jogcurrentpos ) //
	{
		//sprintf(cJamcode, "%02d0004", n_axis);
		sprintf(cJamcode, "0%02d010", n_axis);			// 2014.12.16 - Bredmin.
		Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
		
// 		if (st_handler.cwnd_list != NULL)
// 		{  // 리스트 바 화면 존재
// 			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
// 		}
		// 2014.11.29 - Bredmin.
		CString strTemp = "";
		strTemp.Format( "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos );
		Func.On_LogFile_Add( LOG_TOTAL, strTemp );

		return BD_ERROR;
	}	

	if(d_targetpos > COMI.md_limit_position[n_axis][1] && d_targetpos > d_jogcurrentpos ) //
	{
		//sprintf(cJamcode, "%02d0004", n_axis);
		sprintf(cJamcode, "0%02d011", n_axis);			// 2014.12.16 - Bredmin.
		Alarm_Error_Occurrence(1, CTL_dWARNING, cJamcode);
		
// 		if (st_handler.cwnd_list != NULL)
// 		{  // 리스트 바 화면 존재
// 			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
// 		}
		// 2014.11.29 - Bredmin.
		CString strTemp = "";
		strTemp.Format( "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos );
		Func.On_LogFile_Add( LOG_TOTAL, strTemp );
		return BD_ERROR;
	}

	// **************************************************************************
	// 모터 파워 상태 검사한다                                                   
	// -> 모터 POWER OFF 시 POWER ON 상태로 만든다                               
	// **************************************************************************
	if (g_comiMgr.Get_MotPower(n_axis) == CTL_ERROR )          // 모터 POWER 상태 검사 함수 
	{
		if (COMI.Set_MotPower(n_axis, CTL_ON) == CTLBD_RET_GOOD)       // 모터 POWER ON 설정 함수 
		{
// 			if (st_handler.cwnd_list != NULL)
// 			{  // 리스트 바 화면 존재
// 				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotPower(n_axis) == CTL_ERROR)  // 모터 POWER 상태 검사 함수 
			{
				if (st_motor[n_axis].n_retry_cnt > (MOT_RTY_CNT))
				{
					//sprintf(cJamcode, "%02d0005", n_axis);
					sprintf(cJamcode, "0%02d004", n_axis);				// 2014.12.16 - Bredmin.
					Alarm_Error_Occurrence(2, CTL_dWARNING, cJamcode);	
					return BD_ERROR;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if (st_motor[n_axis].n_retry_cnt > MOT_RTY_CNT)
			{
				//printf(cJamcode, "%02d0005", n_axis);
				sprintf(cJamcode, "0%02d004", n_axis);				// 2014.12.16 - Bredmin.
				Alarm_Error_Occurrence(3, CTL_dWARNING, cJamcode);		
				return BD_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return BD_RETRY;
			}
		}
	}

	if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR)      // 모터 ALARM 상태 검사 함수 
	{
		if (COMI.Set_MotAlarmClear(n_axis) == CTL_GOOD)       // 모터 ALARM CLEAR 함수 
		{
			//091119 james			Sleep(1000);  // 일정 시간 후에 상태 확인하기 위해 SLEEP 사용한다 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis] );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR )  // 모터 ALARM 상태 검사 함수 
			{
				if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
				{			
					//sprintf(cJamcode, "%02d0002",  n_axis); 
					sprintf(cJamcode, "0%02d005",  n_axis);		// 2014.12.16 - Bredmin.
					alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					//sprintf(alarm.mc_code, "%02d0002", n_axis);
					sprintf(alarm.mc_code, "0%02d005", n_axis);		// 2014.12.16 - Bredmin.
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, cJamcode, CtlBdFunc.ms_ErrMsg);
					//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // 알람 해제 시도 횟수 
					return BD_RETRY;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
			{
				//sprintf(cJamcode, "%02d0002",  n_axis);		// 2014.12.16 - Bredmin.
				sprintf(cJamcode, "0%02d005",  n_axis); 
				alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
				//sprintf(alarm.mc_code, "%02d0002", n_axis);
				sprintf(alarm.mc_code, "0%02d005", n_axis);		// 2014.12.16 - Bredmin.
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Alarm Return Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, cJamcode, CtlBdFunc.ms_ErrMsg);
				
				//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // 알람 해제 시도 횟수 
				return BD_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return BD_RETRY;
			}
		}
	}
	// **************************************************************************

	// **************************************************************************
	// 모터의 안전 위치를 체크한다. 장비에 맞게 넣는것 보다. 딴쪽에 지정해 놓고 불러다 써야 할듯.                                           
	//***************************************************************************
	switch( n_axis )
	{
		case M_CARRIER_X:

			if(m_dTargetPos == st_motor[n_axis].md_pos[P_CARRIER_X_PUSH_POS])
			{
				nRet_1 = FAS_IO.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);
				nRet_2 = FAS_IO.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_OFF);
				nRet_3 = FAS_IO.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_OFF);
				nRet_4 = FAS_IO.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);
// 				nRet_5 = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, nOnOff);
// 			    nRet_6 = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, !nOnOff);
				nRet_5 = FAS_IO.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_6 = FAS_IO.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_OFF || nRet_4 != IO_ON || nRet_5 != IO_ON || nRet_6 != IO_OFF )
				{
					if		( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Press_Carrier_Holder_Up_Check));
					else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check));
					else if( nRet_3 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d", IO_OFF, st_io.i_Camera_Y_Jig_Press_Forward_Check));
					else if( nRet_4 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Camera_Y_Jig_Press_Backward_Check));
					else if( nRet_5 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Press_Up_Check));
					else if( nRet_6 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d", IO_OFF, st_io.i_Press_Down_Check));
					else								alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Press_Carrier_Holder_Up_Check));//혹시나
					CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			if(m_dTargetPos == st_motor[n_axis].md_pos[P_CARRIER_X_PRESS_POS])
			{
				nRet_1 = FAS_IO.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_2 = FAS_IO.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF )
				{
					if( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Press_Up_Check));
					else/* if( nRet_2 != IO_OFF )*/ alarm.mstr_code.Format(_T("8%d%04d", IO_OFF, st_io.i_Press_Down_Check));
					CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			else
			{
				nRet_1 = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
				nRet_2 = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
				nRet_3 = FAS_IO.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_4 = FAS_IO.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if      ( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Slide_Guide_X2_Up_Check));
				else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Down_Check));
				else if( nRet_3 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Press_Up_Check));
				else if( nRet_4 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d", IO_OFF, st_io.i_Press_Down_Check));
				else								 alarm.mstr_code.Format(_T("8%d%04d", IO_ON, st_io.i_Slide_Guide_X2_Up_Check));//혹시나
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				return CTL_ERROR;
			}
			break;

		case M_PRESS_Y:
			if( Chk_PressClamp_Safety(3) == RET_ERROR )
			{
				COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nPressAxisY);
				COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nPressAxisX);
				break;
			}
			if( COMI.Get_MotCurrentPos( M_CARRIER_X ) < ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) &&
				COMI.Get_MotCurrentPos( M_CARRIER_X ) > ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] + st_motor[m_nPressAxisX].mn_allow ))
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				return CTL_ERROR;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y ) > (st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
			{//080008 1 0 "M_LOADER_TRANSFER_Y_MOTOR_IS_NON_SAFETY_POS."
				alarm.mstr_code = _T("080008");
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Loader_Y_Motor is not safety." );				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "080008", CtlBdFunc.ms_ErrMsg);
				return CTL_ERROR;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X ) > (st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
			{//130008 1 0 "M_UNLOADER_TRANSFER_X_MOTOR_IS_NON_SAFETY_POS."
				alarm.mstr_code = _T("130008");
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "130008", CtlBdFunc.ms_ErrMsg);
				return CTL_ERROR;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X ) > (st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
			{//130008 1 0 "M_UNLOADER_TRANSFER_X_MOTOR_IS_NON_SAFETY_POS."
				alarm.mstr_code = _T("130008");
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "130008", CtlBdFunc.ms_ErrMsg);
				return CTL_ERROR;
			}
// 			if( COMI.Get_MotCurrentPos( M_HEATSINK_INSPECT_Y ) > (st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
// 			{//130008 1 0 "M_UNLOADER_TRANSFER_X_MOTOR_IS_NON_SAFETY_POS."
// 				alarm.mstr_code = _T("130008");
// 				CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "130008", CtlBdFunc.ms_ErrMsg);
// 				return CTL_ERROR;
// 			}
			break;

	}

	st_motor[n_axis].mn_retry_time_flag = CTL_NO;//2011.0201 추가 

	return BD_GOOD;
}

BOOL CCtlBd_Library::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	CString strTemp;
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

int CCtlBd_Library::FileSizeCheck(char * cp_filename, int n_size, int n_check)
{
	CFileFind finder;
	long nLen=0, nflag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(cp_filename))
	{	
		if (n_check == BD_YES)			// Size를 체크하는것이면...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			if(nLen > n_size)
			{
				nflag = 1;
			}
			finder.Close();	
		}
		else						// Size를 확인하는것이면...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			
			nflag = nLen;
			
			finder.Close();	
		}
	}
	
	return nflag;
}

int CCtlBd_Library::LogFile_Write(char * cp_dir_path, char * cp_msg)
{
//2011.0417	sing.Lock(); //2011.0417 

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name, mstr_dir_path, str_msg;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// 파일 존재 유무 설정 플래그 
	char cfileName[256]={0,};			// 검색할 파일 정보 설정 함수 
	FILE  *fp ;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //저장할 메세지가 없다 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime() ;  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// 현재 시간 정보를 설정한다. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");	// 리스트 파일에 출력할 타이틀 시간 정보 설정 
	// **************************************************************************


	mstr_dir_path = cp_dir_path;
	mstr_file_name = mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = mstr_dir_path + mstr_file_name;

	mstr_create_file += ".TXT";

	sprintf(cfileName, "%s", mstr_create_file);
	mn_existence = access(cfileName,0) ;	
	if (mn_existence == -1)
	{//해당 파일이 존재하지 않는 상태이다 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//해당 파일이 이미 존재한다.
		//정상 
	}
	
	Ret = FileSizeCheck(cfileName, 1048576, BD_YES); //size and rename	
	if(Ret == 1) //file size over
	{
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		//AfxMessageBox("File open failure!..");
		sprintf(mc_normal_msg,"LogFile_Write a+ Error"); 
		
 		return BD_ERROR;
	}
	// **************************************************************************
	
	mstr_content += str_display_time;
	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content) ;
	if (ferror(fp))  
	{
		sprintf(mc_normal_msg,"LogFile_Write ferror return error");

		clearerr(fp); //2011.0418 

		fclose(fp) ;  //2011.0417  파일 종료

//2011.0417		sing.Unlock(); //2011.0417 추가 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // 파일을 종료한다.

//2011.0417	sing.Unlock(); //2011.0417 추가 
	return BD_GOOD;
}

int CCtlBd_Library::Single_Move(int n_RunMethod, int n_MotNum, double d_MovePos, int n_SpeedRate) 
{
	int nFuncRet = BD_PROCEED;
	long dwMotionDone=0;
	int nRet_1;
	int nAxis[2];
	double dTarget[2];
	
	
	switch(mn_single_motmove_step[n_MotNum])
	{
	case 0:
		if(n_RunMethod == ONLY_MOVE_CHECK)
		{
			mn_single_motmove_step[n_MotNum] = 200;
			break;
		}

		nRet = COMI.Check_MotPosRange(n_MotNum, d_MotPos, COMI.md_allow_value[n_MotNum]);
		if (nRet == BD_GOOD) //이미 해당 위치에 와 있으면 동작하지 않고 모터 이동을 끝낸다 
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_mot_retry_cnt[n_MotNum] = 10;
		mn_mot_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
		mn_single_motmove_step[n_MotNum] = 100;
		m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		break;

	case 10:
		//20150428 james ���� state�� �о� ���ͻ��¸� üũ�� ó��
		if(m_dwUntil_Wait[n_MotNum][0]  < 0) m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][1] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][2] = m_dwUntil_Wait[n_MotNum][1] - m_dwUntil_Wait[n_MotNum][0];

		//20150428 james ���� state�� �о� ���ͻ��¸� üũ�� ó��
		nRet_1 = COMI.Check_Motion_State(n_MotNum, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotNum, &dwMotionDone);
		if (nRet_1 == BD_GOOD) 
		{//���� ���°� mMST_STOP �̴� 
			mn_single_motmove_step[n_MotNum] = 100;
		}
		else if(nRet_1 == BD_PROCEED) //���� ���°� �������̴� 
		{ 
			if(m_dwUntil_Wait[n_MotNum][2] > 15000) //max 10sec wait
			{
				if (st_handler_info.cWndList != NULL)  
				{
					clsMem.OnNormalMessageWrite(_T("CCtlBd_Library::Single_Move = 10 1"));
					//st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
				}
				cmmSxStopEmg(n_MotNum);	
			
				mn_single_motmove_step[n_MotNum] = 100;
			} 
		}
		else if(nRet_1 == BD_ERROR)//
		{  
			if(m_dwUntil_Wait[n_MotNum][2] > 200)
			{
				if (st_handler_info.cWndList != NULL)  
				{
					clsMem.OnNormalMessageWrite(_T("CCtlBd_Library::Single_Move = 10 2"));
					//st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
				}
	
				cmmSxStopEmg(n_MotNum);	

				mn_single_motmove_step[n_MotNum] = 0;
			}
		}
		break;
		
	case 100:
		nRet = COMI.Start_SingleMove(n_MotNum, d_MotPos, n_SpeedRate);  // 해당 위치로 이동
		
		if (nRet == BD_GOOD)
		{
			if(n_RunMethod == ONLY_MOVE_START)
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_GOOD;
				break;
			}
			else
			{
				mn_single_motmove_step[n_MotNum] = 200;
			}
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29898;
				nFuncRet = nRet;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet == BD_RETRY)  // 동작 재시도
		{
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29899;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
		
	case 200:
		if(n_RunMethod == ONLY_MOVE_START)
		{
			mn_single_motmove_step[n_MotNum] = 0;
			break;
		}

		nRet = COMI.Check_SingleMove(n_MotNum, d_MotPos);  // 이동 완료 확인
		if (nRet == BD_GOOD)  // 정상 완료
		{
			mn_mot_retry_cnt[n_MotNum] = 0;
			mn_single_motmove_step[n_MotNum] = 0;
			nFuncRet = nRet;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29901;
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = nRet;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet == BD_RETRY)  // 동작 재시도
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29902;
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
	}
	return nFuncRet;
}

int CCtlBd_Library::Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio)
{
	int nFuncRet = BD_PROCEED;
	int nRet[4] = {0,}, nCnt =0, i=0;
	
	switch(mn_linear_motmove_step[n_LinearMapIndex])
	{
	case 0:
		nCnt = 0;
		st_linearmot[n_LinearMapIndex].l_AxisCnt = l_AxisCnt;
		for(i=0; i<l_AxisCnt; i++)
		{		
			st_linearmot[n_LinearMapIndex].lp_AxisNum[i] = lp_AxisNum[i]; 
			st_linearmot[n_LinearMapIndex].dp_PosList[i] = dp_PosList[i];

			nRet[i] = COMI.Check_MotPosRange(lp_AxisNum[i], dp_PosList[i], COMI.md_allow_value[lp_AxisNum[i]]);
			if(nRet[i] == BD_GOOD) nCnt++;
		}

		if(nCnt == l_AxisCnt)
		{//이미 모든 모터의 현재 위치가 이동하려는 위치와 허용오차 안에 있으면 동작할 필요가 없으니 끝내자.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
		mn_linear_motmove_step[n_LinearMapIndex] = 100;
		break;
		
	case 100:
		nRet[0] = COMI.Start_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList, dp_SpdRatio);
		if (nRet[0] == BD_GOOD)
		{
/*			CString strData1="";
			CString strData2="";
			
			m_vecStLogData_Add1.clear();
			m_vecStLogData_Add2.clear();

			strData1 = "TYPE";
			strData2 = "MOTOR";
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "ACT_NAME";
			strData2 = Func.Get_MotorName( n_LinearMapIndex, 1 );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[0] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[0] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );

			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[1] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[1] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			st_standard_log.strDeviceID		= Func.Get_MotorName( n_LinearMapIndex );
			st_standard_log.iLogType		= LOG_FNC;
			st_standard_log.strEventID.Format( "MOVE_%s_%s", Func.GetMotorPos( lp_AxisNum[0] ), Func.GetMotorPos( lp_AxisNum[0], dp_PosList[0] ) );
			strStLogStartLinearData[n_LinearMapIndex] = st_standard_log.strEventID;
			st_standard_log.strStatus		= "START";
			st_standard_log.strMatrialID	= "MODULE";
			st_standard_log.strMatrialType	= "MODULE_TYPE";
			
			Func.OnStandardLogWrite( st_standard_log, m_vecStLogData_Add1, m_vecStLogData_Add2 );
*/
			mn_linear_motmove_step[n_LinearMapIndex] = 200;
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
		{
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		break;
		
	case 200:
		nRet[0] = COMI.Check_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList) ;
		if (nRet[0] == BD_GOOD)  // 정상 완료
		{
/*			CString strData1="";
			CString strData2="";
			
			m_vecStLogData_Add1.clear();
			m_vecStLogData_Add2.clear();

			strData1 = "TYPE";
			strData2 = "MOTOR";
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "ACT_NAME";
			strData2 = Func.Get_MotorName( n_LinearMapIndex, 1 );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[0] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[0] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[1] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[1] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			st_standard_log.strDeviceID		= Func.Get_MotorName( n_LinearMapIndex );
			st_standard_log.iLogType		= LOG_FNC;
			
			st_standard_log.strEventID = strStLogStartLinearData[n_LinearMapIndex];
			st_standard_log.strStatus		= "END";
			st_standard_log.strMatrialID	= "MODULE";
			st_standard_log.strMatrialType	= "MODULE_TYPE";
			
			Func.OnStandardLogWrite( st_standard_log, m_vecStLogData_Add1, m_vecStLogData_Add2 );
*/
			mn_linear_motmove_step[n_LinearMapIndex] = 0;
			nFuncRet = nRet[0];
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
		{
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없다
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}	
		break;
	}
	return nFuncRet;
}


int CCtlBd_Library::SD_Sensor_Enable(int n_Mode, int n_AxisNum, int n_Enable)
{//Constant speed mode 에서는 SD 신호 입력이 무시됩니다.
	int nFuncRet = BD_PROCEED;

	if(n_Enable == CTL_YES) 
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmTRUE);  
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmFALSE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmTRUE);

		st_motor[n_AxisNum].n_sd_mv_chk = 1; //sd를 조건을 확인하기 위해 이동한다 
	}
	else
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  
		//COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmTRUE);
		//COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmFALSE);
		st_motor[n_AxisNum].n_sd_mv_chk = 0;
	}

	return BD_GOOD;
}


int CCtlBd_Library::Elevator_Job_Move_Pos(int nMode, int n_AxisNum,  int n_TargetPos_Site)
{
	int nFuncRet = RET_PROCEED;
	int nRet_1, nRet_2;
	double dCurrentPos;
	CString strlog;
	//	double dPos_1, dPos_2, dPos_3;

	Func.ThreadFunctionStepTrace(30, m_nSD_Elv_MoveStep[n_AxisNum]);


	//LIGHT CURTAIN
	if( st_sync.nLdUld_LightChk == YES )
	{
		if( n_AxisNum == M_TRAY1_Z || n_AxisNum == M_TRAY2_Z )
		{
			cmmSxStop(n_AxisNum, FALSE, FALSE); 
			return BD_RETRY;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//SD 센서가 기준점을 잡는 센서 이다 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(st_work.n_run_status != dRUN)
	{   //time out 시간 체크 문제 보완을 위해  
		if(m_bSD_MoveFlag[n_AxisNum] == true) m_bSD_MoveFlag[n_AxisNum] = false; 
	}

	switch(m_nSD_Elv_MoveStep[n_AxisNum])
	{
	case 0:	
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//트레이를 작업 가능한 영역에 UP하여 공급하는 위치, SD pos 센서 On 감지 후 센서 기준 - 방항으로 벗어난 후 + 방향으로 P_ELV_SUPPLY_OFFSET 티칭만큼 up 한후 모터 동작은 완료하고, 트레이르, 받든다 
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}			
		else 
		{//910004 1 A "THERE_IS_NO_POSITION_THAT ELV MOVE_IN_LOAD_STACKER."
			CTL_Lib.Alarm_Error_Occurrence(272, dWARNING, "910004");
		}			
		break;
		///////////////////////////////////////////////////////////////////////////
		// SD Sensor 기준 위 또는 아래인지 확인하여, 위면 내려가고, 아래면 올라간다 
		///////////////////////////////////////////////////////////////////////////
	case 1000: //기준점 위치 확인 (SD Sensor 대비 high or 같은 위치인지인지, low인지 두가지 위치를 확인한다)			 
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 	
		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear			

		if(nRet_1 == BD_GOOD)
		{//SD가 감지되면 위에 있는 상태 
			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 6000; //기준점보다 위에 있는 상태로 아래로 내리면서 SD를 지나가기를 기다리자 
		}
		else //if(nRet_1 == BD_ERROR)
		{//SD도 감지되지 않았고, SD 위치보다 낮은 상태로, 위로 올라가면서 SD를 찾는다 
			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 2000; //traget pos를 이동하는 루틴 
		}	 
		break;

		////////////////////////////////////////////////////////////
		//SD 센서를 찾으면서 위로 이동하다 
		////////////////////////////////////////////////////////////	
	case 2000: 
		if(m_bSD_MoveFlag[n_AxisNum] == false)
		{
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			m_bSD_MoveFlag[n_AxisNum] = true;
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_YES); //sd sensor set
		}
		else if(m_bSD_MoveFlag[n_AxisNum] == true)
		{
			m_dwSdWaitTime[n_AxisNum][1] = GetTickCount();
			m_dwSdWaitTime[n_AxisNum][2] = m_dwSdWaitTime[n_AxisNum][1] - m_dwSdWaitTime[n_AxisNum][0];
			if(m_dwSdWaitTime[n_AxisNum][2] <= 0 )
			{
				m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
				break;
			}
			if(m_dwSdWaitTime[n_AxisNum][2] > MOT_ELE_MOVE_WAITLIMIT)
			{//limit 시간 지남, 에러 처리후 재 시도하자 
				COMI.Set_MotStop(1, n_AxisNum) ; //긴급정지 
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);   //제일 밑까지 이동한다 
		if (nRet_1 == BD_GOOD) //위로 이동 하면서 SD를 찾는다  
		{					 				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 2100;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			CTL_Lib.Alarm_Error_Occurrence(272, dWARNING, alarm.mstr_code);

			if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2000"));
				//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2100:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_GOOD)
		{//정상적으로 체크했다. 
			COMI.Set_MotStop(1, n_AxisNum) ; //긴급정지 

			st_motor_info[n_AxisNum].n_sd_mv_chk = 0; //clear
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear

			m_nSD_Elv_MoveStep[n_AxisNum] = 2200;
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);  //2015.0407 james  //SD 위치까지 이동했는데 SD가 감지되지 않으면 트레이가 없는것이다
		if (nRet_1 == BD_GOOD)   
		{	//트레이가 없는 상태일것이다, 미리 트레이를 체크가능하여 트레이가 없으면 올라 갈 필요가 없다 
			//이곳에 오면 문제가 있음 
			if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//트레이를 작업 가능한 영역에 UP하여 공급하는 위치, SD pos 센서 On 감지 후 센서 기준 - 방항으로 벗어난 후 + 방향으로 P_ELV_SUPPLY_OFFSET 티칭만큼 up 한후 모터 동작은 완료하고, 트레이르, 받든다 
			{
				if(n_AxisNum == M_TRAY1_Z)
				{
					nRet_1 = FAS_IO.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check,	IO_ON); //load stacker tray check    
					if(nRet_1 == IO_OFF)
					{
						m_nSD_Elv_MoveStep[n_AxisNum] = 0;
						nFuncRet = RET_GOOD;
					}
				}
				else
				{
					//james test m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //트레이가 없는 상태  
					m_nSD_Elv_MoveStep[n_AxisNum] = 0;
					nFuncRet = RET_GOOD;
				}
			}			
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			CTL_Lib.Alarm_Error_Occurrence(273, dWARNING, st_alarm_info.strCode);

			////////if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2010"));
				//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2200:
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//트레이를 작업 가능한 영역에 UP하여 공급하는 위치, SD pos 센서 On 감지 후 센서 기준 - 방항으로 벗어난 후 + 방향으로 P_ELV_SUPPLY_OFFSET 티칭만큼 up 한후 모터 동작은 완료하고, 트레이르, 받든다 
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 != BD_GOOD)
			{//SD 센서가 감지되지 않으면 에러 
				COMI.Set_MotStop(1, n_AxisNum) ; //긴급정지 

				m_bSD_MoveFlag[n_AxisNum] = false;
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //재시도  루틴  
				break;
			}
		}

		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//트레이를 작업 가능한 영역에 UP하여 공급하는 위치, SD pos 센서 On 감지 후 센서 기준 - 방항으로 벗어난 후 + 방향으로 P_ELV_SUPPLY_OFFSET 티칭만큼 up 한후 모터 동작은 완료하고, 트레이르, 받든다 
		{
			m_dTargetPos[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_SUPPLY_OFFSET]); //현재 위치에 + P_ELV_SUPPLY_OFFSET 
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);   
		if (nRet_1 == BD_GOOD) //조건에 따라 위로/아래로  이동  
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 2210;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			//st_sync_info.nSmema_Tray_Output_Req++;
			//if( st_sync_info.nSmema_Tray_Output_Req > 3)
			//{
				//st_sync_info.nSmema_Tray_Output_Req = 0;
				CTL_Lib.Alarm_Error_Occurrence(274, dWARNING, alarm.mstr_code);

				if (st_handler.cwnd_list != NULL)  
				{
					//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2100"));
					//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
				}
			//}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2210:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);  //2015.0407 james  //SD 위치까지 이동했는데 SD가 감지되지 않으면 트레이가 없는것이다
		if (nRet_1 == BD_GOOD)   
		{	//트레이가 없는 상태일것이다, 미리 트레이를 체크가능하여 트레이가 없으면 올라 갈 필요가 없다 
			//이곳에 오면 문제가 있음 
			m_nSD_Elv_MoveStep[n_AxisNum] = 2300; //트레이가 있는 상태 				 
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			//2016.1117
// 			st_sync_info.nSmema_Tray_Output_Req++;
// 			if( st_sync_info.nSmema_Tray_Output_Req > 3)
// 			{
				st_sync_info.nSmema_Tray_Output_Req = 0;
				CTL_Lib.Alarm_Error_Occurrence(275, dWARNING, st_alarm_info.strCode);

				if (st_handler.cwnd_list != NULL)  
				{
					sprintf( st_msg.c_abnormal_msg,"Motor:%d Target: %.3f Feedback: %.3f", n_AxisNum, m_dTargetPos[n_AxisNum], COMI.Get_MotCurrentPos(n_AxisNum));
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , ABNORMAL_MSG);
				}
//			}

			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2300:
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//트레이를 작업 가능한 영역에 UP하여 공급하는 위치, SD pos 센서 On 감지 후 센서 기준 - 방항으로 벗어난 후 + 방향으로 P_ELV_SUPPLY_OFFSET 티칭만큼 up 한후 모터 동작은 완료하고, 트레이르, 받든다 
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 == BD_GOOD)
			{//정상적으로 체크했다. 					
				m_nSD_Elv_MoveStep[n_AxisNum] = 2400; // 루틴   
			}
			else
			{//error
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //재시도  루틴   
			}
		}
		break;

	case 2400:
		m_nSD_Elv_MoveStep[n_AxisNum] = 2500;
		break;

	case 2500:
		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//트레이를 작업 가능한 영역에 UP하여 공급하는 위치, SD pos 센서 On 감지 후 센서 기준 - 방항으로 벗어난 후 + 방향으로 P_ELV_SUPPLY_OFFSET 티칭만큼 up 한후 모터 동작은 완료하고, 트레이르, 받든다 
		{
			m_dSD_Supply_Pos_Backup[n_AxisNum] = fabs(dCurrentPos + st_motor_info[n_AxisNum].d_pos[P_ELV_SUPPLY_OFFSET]); //현재 위치에 + P_ELV_SUPPLY_OFFSET 
		}

		//m_dReference_Pos_Backup

		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear
		m_nSD_Elv_MoveStep[n_AxisNum] = 0;
		nFuncRet = RET_GOOD;
		break; 

		//
		////////////////////////////////////////////////////////////
		//SD 센서를 감지하고 있으니 아래로 내려가자 
		////////////////////////////////////////////////////////////	
	case 6000: 
		if(m_bSD_MoveFlag[n_AxisNum] == false)
		{
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			m_bSD_MoveFlag[n_AxisNum] = true;
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor set
		}
		else if(m_bSD_MoveFlag[n_AxisNum] == true)
		{
			m_dwSdWaitTime[n_AxisNum][1] = GetTickCount();
			m_dwSdWaitTime[n_AxisNum][2] = m_dwSdWaitTime[n_AxisNum][1] - m_dwSdWaitTime[n_AxisNum][0];
			if(m_dwSdWaitTime[n_AxisNum][2] <= 0)
			{
				m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
				break;
			}
			if(m_dwSdWaitTime[n_AxisNum][2] > MOT_ELE_MOVE_WAITLIMIT)
			{//limit 시간 지남, 에러 처리후 재 시도하자 
				COMI.Set_MotStop(1, n_AxisNum) ; //긴급정지 
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate/3);   //밑으로 이동한다 
		if (nRet_1 == BD_GOOD) //위로 이동 하면서 SD를 찾는다  
		{					 				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 6100;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			CTL_Lib.Alarm_Error_Occurrence(375, dWARNING, alarm.mstr_code);

			if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2000"));
				//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 				sprintf( st_msg.c_abnormal_msg,"Motor:%d Target: %.3f Feedback: %.3f", n_AxisNum, m_dTargetPos[n_AxisNum], COMI.Get_MotCurrentPos(n_AxisNum));
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , ABNORMAL_MSG);

			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 6100:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_ERROR)
		{//정상적으로 체크했다. 
			COMI.Set_MotStop(1, n_AxisNum) ; //긴급정지 			
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear

			m_nSD_Elv_MoveStep[n_AxisNum] = 7000;
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)   
		{	 
			m_nSD_Elv_MoveStep[n_AxisNum] = 6200;  //완전히 내렸으나 아직도 SD룰 감지하고 있는 상태로 , TRAY FULL 상태이다 				 
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			CTL_Lib.Alarm_Error_Occurrence(276, dWARNING, alarm.mstr_code);

			////////if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2010"));
				//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 6200:
		/////////////////////////////////////////////////////////////////////
		// TRAY FULL Error
		/////////////////////////////////////////////////////////////////////
		nFuncRet = RET_ABORT;
		m_nSD_Elv_MoveStep[n_AxisNum] = 6100; 

		if (st_handler.cwnd_list != NULL)
		{
			st_other.nBuzYesNo = YES;
			if( n_AxisNum == M_TRAY1_Z )
			{
				st_other.strBoxMsg = _T("[ TRAY ELV#1 FULL] Tray Elv#1 is Full. \r\n Please Remove it.");
			}
			else if( n_AxisNum == M_TRAY2_Z )
			{
				st_other.strBoxMsg = _T("[ TRAY ELV#2 FULL] Tray Elv#2 is Full. \r\n Please Remove it.");
			}			
			else
			{
				st_other.strBoxMsg = _T("[TRAY FULL] Warnning : Tray is Full. \r\n Please Remove it.");
			}
			if( st_handler.cwnd_main != NULL )
			{
				st_handler.cwnd_main->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
			}
			if( st_handler.cwnd_list != NULL )
			{
				sprintf(st_msg.c_abnormal_msg, st_other.strBoxMsg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			Func.OnSet_IO_Port_Stop();
		}

		break;


	case 7000:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_ERROR)
		{//SD 센서가 감지
			COMI.Set_MotStop(1, n_AxisNum) ; //긴급정지 

			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //이제 다시 위로 올라가 정해진 루틴으로 SD 기준 위치에 맞춘다    
		}
		else
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //이제 다시 위로 올라가 정해진 루틴으로 SD 기준 위치에 맞춘다    
		}
		break;
	}	 

	if(m_nSD_Elv_MoveStep[n_AxisNum] == 0 || nFuncRet == RET_GOOD)
	{
		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear
		m_nSd_Motor_MoveStart_Flag[n_AxisNum] = 0; //모터 이동 클리어 
	}

	return nFuncRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//									MOTOR ERROR CODE DEFINITIONs								//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCtlBd_Library::Motor_Error_Occurrence(int n_Mode, long l_Axis, long l_ErrorIDNum, CString strErrorMsg)
{
	CString strMsg, strMsg_1, strMsg_2, strMsg_ErrIDNum;	 
	 

	strMsg = _T("");
	strMsg_1 = _T("");
	strMsg_2 = _T("");
	strMsg_ErrIDNum = _T("");

	
	////////////////////
	//모터 이름 정의 
	////////////////////
	switch(l_Axis)
	{//AMT8562는 29개의 모터를 사용한다 
	case -1: //사용하지 않는 루틴
		l_Axis = 0; //알람 코드 생성을 위해 초기화 추가 
		strMsg_1 = _T("");		
		break;
	case 0: //0번모터 
		strMsg_1.Format(_T("M_TRAY1_Z[%02ld]"), l_Axis);
		break;
	case 1:  //1번모터 
		strMsg_1.Format(_T("M_TRAY2_Z[%02ld]"), l_Axis);
		break;
	case 2:  //2번모터 
		strMsg_1.Format(_T("M_PRESS_Y[%02ld]"), l_Axis);
		break;
	case 3:   
		strMsg_1.Format(_T("M_EPOXY_TRANSFER_X[%02ld]"), l_Axis);
		break;
	case 4:   
		strMsg_1.Format(_T("M_EPOXY_TRANSFER_Y[%02ld]"), l_Axis);
		break;
	case 5:   
		strMsg_1.Format(_T("M_EPOXY_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 6:   
		strMsg_1.Format(_T("M_EPOXY_SCREW[%02ld]"), l_Axis);
		break;
	case 7:   
		strMsg_1.Format(_T("M_EPOXY_SCREW[%02ld]"), l_Axis);
		break;
	case 8:   
		strMsg_1.Format(_T("M_CARRIER_X[%02ld]"), l_Axis);
		break;
	case 9:   
		strMsg_1.Format(_T("M_LOADER_TRANSFER_Y[%02ld]"), l_Axis);
		break;
	case 10:   
		strMsg_1.Format(_T("M_LOADER_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 11:   
		strMsg_1.Format(_T("M_HEATSINK_TRANSFER_X[%02ld]"), l_Axis);
		break;
	case 12:   
		strMsg_1.Format(_T("M_HEATSINK_TRANSFER_Y[%02ld]"), l_Axis);
		break;
	case 13:   
		strMsg_1.Format(_T("M_HEATSINK_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 14:   
		strMsg_1.Format(_T("M_UNLOADER_TRANSFER_X[%02ld]"), l_Axis);
		break;
	case 15:   
		strMsg_1.Format(_T("M_UNLOADER_TRANSFER_Y[%02ld]"), l_Axis);
		break;
		
	case 16:   
		strMsg_1.Format(_T("M_UNLOADER_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 17:   
		strMsg_1.Format(_T("M_DISPENSER_Y[%02ld]"), l_Axis);
		break;
	case 18:   
		strMsg_1.Format(_T("M_TRAY_REMOVE_X[%02ld]"), l_Axis);
		break;
	case 19:   
		strMsg_1.Format(_T("M_HEATSINK_INSPECT_Y[%02ld]"), l_Axis);
		break;
	case 20:   
		strMsg_1.Format(_T("M_HEATSINK_DISPENSOR_Z[%02ld]"), l_Axis);
		break;
	case 21:   
		strMsg_1.Format(_T("M_HEATSINK_PICKER_PITCH[%02ld]"), l_Axis);
		break;

	default :
		strMsg_1.Format(_T("MOTOR_NUM_UNKNOWN_DEFAULT_ERROR[%02ld]"), l_Axis);
		l_Axis = 0;
		break;  

	}//모터 정보 이름 
		 

	//////////////////////////////////////////////////////////////////
	//커미조아 모터 함수 에러 정보 취합
	//////////////////////////////////////////////////////////////////
	switch(l_ErrorIDNum)
	{
	case -1:        //사용하지 않는 루틴 
		l_ErrorIDNum = 1; //알람 코드 생성을 위해 초기화 추가  
		strMsg_2.Format(_T(""));
		 break;	

	//모터 구동작업중 알람 정보
	case 1:
		 strMsg_2.Format(_T("MOTOR_POWER_OFF_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 2:
		 strMsg_2.Format(_T("MOTOR_ALARM_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 3:
		 strMsg_2.Format(_T("MOTOR_HOME_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 4:
		 strMsg_2.Format(_T("MOTOR_MOVE_START_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 5:
		 strMsg_2.Format(_T("MOTOR_MOVE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 6:
		 strMsg_2.Format(_T("MOTOR_MOVE_END_TIME_OUT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 7:
		 strMsg_2.Format(_T("MOTOR_SAFETY_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	//모터 구동 관련 상태 및 리턴 관려 커미조아 사용 알람 정보
	case 10: 
		strMsg_2.Format(_T("MOTOR_SET_IO_PROPERTY_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 11: 
		strMsg_2.Format(_T("MOTOR_PARAMETER_SETTING_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 12: 
		strMsg_2.Format(_T("MOTOR_MOVE_POS_MINUS_LIMIT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 13: 
		strMsg_2.Format(_T("MOTOR_MOVE_POS_PLUS_LIMIT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 14: 
		strMsg_2.Format(_T("MOTOR_CMDPOS_FEEDPOS_DEVITION_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 15: 
		strMsg_2.Format(_T("MOTOR_ALARM_CLEAR_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 16: 
		strMsg_2.Format(_T("MOTOR_ALARM_STATUS_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 17: 
		strMsg_2.Format(_T("MOTOR_ESTOP_RETURN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 18: 
		strMsg_2.Format(_T("MOTOR_GETCURRENTPOS_RETURN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 19: 
		strMsg_2.Format(_T("MOTOR_INPOSITION_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 20: 
		strMsg_2.Format(_T("MOTOR_MOTION_DONE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 21: 
		strMsg_2.Format(_T("MOTOR_MOTION_DONE_ZERO_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 22: 
		strMsg_2.Format(_T("MOTOR_HOME_CONFIG_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 23: 
		strMsg_2.Format(_T("MOTOR_HOME_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 24: 
		strMsg_2.Format(_T("MOTOR_OVERRIDE_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 25: 
		strMsg_2.Format(_T("MOTOR_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	case 26: 
		strMsg_2.Format(_T("MOTOR_UNIT_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 27: 
		strMsg_2.Format(_T("MOTOR_INTERRUPT_STATUS_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 28: 
		strMsg_2.Format(_T("MOTOR_TRIGGERSET_ONE_COMPARE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 29: 
		strMsg_2.Format(_T("MOTOR_UNIT_DISTANCE_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 30: 
		strMsg_2.Format(_T("MOTOR_SPEED_RATIO_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 31: 
		strMsg_2.Format(_T("MOTOR_MOTION_STATE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	//List Motion 구동 동작 관련, 모터 번호 또는 이름에 이 내용을 더해서 메세지를 출력한다  
	case 41: 
		strMsg_2.Format(_T("MOTOR_LIST_MOTION_LMMAPAXIS_CREATE_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 42: 
		strMsg_2.Format(_T("MOTOR_LINEAR_MOTION_IXMAPAXIS_CREATE_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	//Motor board 관련 에러 
	case 50: 
		strMsg_2.Format(_T("MOTOR_BOARD_LOAD_DLL_OPEN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 51: 
		strMsg_2.Format(_T("MOTOR_BOARD_LOAD_DEVICE_OPEN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 52: 
		strMsg_2.Format(_T("MOTOR_BOARD_CME_FILE_OPEN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
 

	////////////////////////////////////////////////////////////////////////////////////////////////
    //Motor dll return error id num
	////////////////////////////////////////////////////////////////////////////////////////////////
	case cmERR_NONE: //0: //정상인 상태 
		 strMsg_2 = _T("[%04ld]"), abs(l_ErrorIDNum);
		 break;
	case cmERR_UNKNOWN:
		strMsg_2.Format(_T("MOTOR_UNKNOWN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;	
	case cmERR_MEM_ALLOC_FAIL://-290
		 strMsg_2.Format(_T("Memory allocation fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_GLOBAL_MEM_FAIL://-292
		 strMsg_2.Format(_T("Global memory allocation fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_ISR_CONNEC_FAIL://-310
		 strMsg_2.Format(_T("ISR(Interrupt Service Routine) registration fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_DIVIDE_BY_ZERO://-400
		 strMsg_2.Format(_T("Cause divide by zero error[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_WORNG_NUM_DATA://-500
		 strMsg_2.Format(_T("Number of data is too small or too big[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_VER_MISMATCH://-600
		 strMsg_2.Format(_T("Version(of file or device) mismatch[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_DEVICE_ID://-1010
		 strMsg_2.Format(_T("User set invalid device id. Refer to “DeviceId” property.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_HANDLE://-1020
		 strMsg_2.Format(_T("Device handle is not valid. This means that loading a device has been failed or not performed. Refer to “GnLoadDevcie” function[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_UNSUPORTED_FUNC://-1030
		 strMsg_2.Format(_T("User called an unsupported function for the specified product.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_PARAMETER://-1101
		 strMsg_2.Format(_T("Some of the function parameters are invalid.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_CHANNEL://-1105
		 strMsg_2.Format(_T("UThe channel setting parameter(s) is(are) invalid.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_INPUT_RANGE://-1111
		 strMsg_2.Format(_T("Invalid range value (AI, AO).[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_FREQ_RANGE://-1121
		 strMsg_2.Format(_T("User selected invalid frequency range.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_FILE_CREATE_FAIL://-1501
		 strMsg_2.Format(_T("File creation has been failed.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_FILE_OPEN_FAIL://-1511
		 strMsg_2.Format(_T("File opening has been failed.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_FILE_READ_FAIL://-1522
		 strMsg_2.Format(_T("File reading fail.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_EVENT_CREATE_FAIL://-1550
		 strMsg_2.Format(_T("Event handle creation has been failed.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_DITHREAD_CRE://-1570
		 strMsg_2.Format(_T("D/I state change monitor thread creation fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_BUFFER_SMALL://-1580
		 strMsg_2.Format(_T("Buffer size is too small[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_HIGH_TIMER_UNSUPP://-1090
		 strMsg_2.Format(_T("The installed hardware doesn‟t support a high-resolution performance counter.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_OUT_OF_RANGE://-1600
		 strMsg_2.Format(_T("The range of some parameter is out of range that it is occured.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_ON_MOTION://-5001
		 strMsg_2.Format(_T("This code is just a symbolic code. This error will never occur.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SLP://-5002
		 strMsg_2.Format(_T("Abnormally stopped by positive soft limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SLN://-5003
		 strMsg_2.Format(_T("Abnormally stopped by negative soft limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CMP3://-5004
		 strMsg_2.Format(_T("Abnormally stopped by comparator3[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CMP4://-5005
		 strMsg_2.Format(_T("Abnormally stopped by comparator4[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CMP5://-5006
		 strMsg_2.Format(_T("Abnormally stopped by comparator5[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_ELP://-5007
		 strMsg_2.Format(_T("Abnormally stopped by (+) external limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_ELN://-5008
		 strMsg_2.Format(_T("Abnormally stopped by (-) external limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_ALM://-5009
		 strMsg_2.Format(_T("Abnormally stopped by alarm input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CSTP://-5010
		 strMsg_2.Format(_T("Abnormally stopped by CSTP input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CEMG://-5011
		 strMsg_2.Format(_T("Abnormally stopped by CEMG input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SD://-5012
		 strMsg_2.Format(_T("Abnormally stopped by SD input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_DERROR://-5013
		 strMsg_2.Format(_T("Abnormally stopped by operation data error[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_IP://-5014
		 strMsg_2.Format(_T("Abnormally stopped by other axis error during interpolation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_PO://-5015
		 strMsg_2.Format(_T("An overflow occurred in the PA/PB input buffer[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_AO://-5016
		 strMsg_2.Format(_T("Out of range position counter during interpolation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_EE://-5017
		 strMsg_2.Format(_T("An EA/EB input error occurred (does not stop)[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_PE://-5018
		 strMsg_2.Format(_T("An PA/PB input error occurred (does not stop)[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SLVERR://-5019
		 strMsg.Format(_T("Abnormally stopped because slave axis has been stopped[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SEMG://-5120
		 strMsg_2.Format(_T("Abnormally stopped by software emergency setting[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_MOT_MAOMODE://-5110
		 strMsg_2.Format(_T("Master output mode is not CW/CCW mode during Master/Slave operation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_MOT_SLAVE_SET://-5120
		 strMsg_2.Format(_T("Slave start has been failed during Master/Slave operation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_SPEED_RANGE_OVER://-5130
		 strMsg_2.Format(_T("Speed setting value exceeds setting range[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_SPEED_SET://-5140
		 strMsg_2.Format(_T("Speed setting value is not valid[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_IXMAP://-5150
		 strMsg_2.Format(_T("Invalid Interpolation Map[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_LMMAP://-5160
		 strMsg_2.Format(_T("Invalid List-Motion Map[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_MOT_SEQ_SKIPPED://-5170
		 strMsg_2.Format(_T("Invalid List-Motion Map[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_CMPIX_INVALID_MAP://-5180
		 strMsg_2.Format(_T("Interpolated position compare output map is not valid[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_ARC_POS://-5190
		 strMsg_2.Format(_T("Position data for circular interpolation is invalid[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_LMX_ADD_ITEM_FAIL://-5200
		 strMsg_2.Format(_T("failed to add an job item to extend list motion[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_LMX_IS_NOT_ACTIVE://-5300
		 strMsg_2.Format(_T("Extended ListMotion' is not active extend list motion[%04ld]"), abs(l_ErrorIDNum));
		 break;
	default:
		strMsg_2.Format(_T("MOTOR_UNKNOWN_DEFAULT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		l_ErrorIDNum = 1; //초기화 처리 
		break;
	 }

	 strMsg = strMsg_1 + _T("") + strMsg_2 + _T("") + strErrorMsg;	
	 strMsg_ErrIDNum.Format(_T("%02ld%04ld"), l_Axis, abs(l_ErrorIDNum)); //필요시 사용위해 6자리의 에러 코드 정의 
	
	 if(n_Mode > 0)
	 {//장비에 알람 정보 처리 

		 //쓰레드에서 알람 관련 장비 스톱은 처리하게 하자 st_handler_info.nRunStatus = dWARNING;
	 }
	 else if(n_Mode == 0)
	 {//모터 관련 메세지만 로깅하여 파일에 쓰고, 장비는 정상 가동시킨다 
		 //같은 메세지를 계속 쓰지 못하게 할것 
		 if(str_Motor_Msg == strMsg)
		 {
			 return;
		 }
	 }

	 sprintf(mc_alarmcode, strMsg_ErrIDNum);

	 if(COMI.mn_errormsg_debug_mode)
	 {
		//OnStringToChar(strMsg, mc_normal_msg);  //msg 화면 출력 
		sprintf( mc_normal_msg, strMsg);
		COMI.Debug_File_Create(0, mc_normal_msg);    //file로 log	  
	 }

	 if (st_handler.cwnd_list != NULL)
	 {
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	 }
}


