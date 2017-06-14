// Run_LdStacker_Elvator.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_LdStacker_Elvator.h"
#include "AMTLotManager.h"
#include "FastechPublic_IO.h"
#include "LogFromat.h"
#include "IO_Manager.h"
#include "CmmsdkDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_LdStacker_Elvator
CRun_LdStacker_Elvator Run_LdStacker_Elvator;
IMPLEMENT_SERIAL(CRun_LdStacker_Elvator, CObject, 1);


CRun_LdStacker_Elvator::CRun_LdStacker_Elvator()
{
	m_nAxisNum = M_TRAY1_Z;
}

CRun_LdStacker_Elvator::~CRun_LdStacker_Elvator()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_LdStacker_Elvator message handlers


void CRun_LdStacker_Elvator::Thread_Run()
{
	switch( st_work.mn_run_status)
	{
	case dINIT:
		RunInit();
		break;

	case dRUN:
 		RunMove();
		break;

	case dSTOP:

		break;

	case dWARNING:
		break;

	default:
		break;
	}
}

void CRun_LdStacker_Elvator::RunInit()
{
	int i = 0,nRet_1=0;

	if( st_handler.mn_init_state[INIT_LD_PLATE] == CTL_NO ) return;	
	if( st_handler.mn_init_state[INIT_LDSTACKER_ELV] != CTL_NO ) return;
// 	st_handler.mn_init_state[INIT_LDSTACKER_ELV] = CTL_YES;
	switch(mn_InitStep)
	{
		case 0:		
			mn_InitStep = 100;
			break;

		case 100:
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_stacker_tray1_type_chk, IO_ON );
			if( nRet_1 == IO_OFF )
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_stacker_tray1_type_chk);
				CTL_Lib.Alarm_Error_Occurrence(3001, dWARNING, m_strAlarmCode);
			}
			else
			{
				mn_InitStep = 200;
			}
			break;

		case 200:
			Set_Tray_Fix_Clamp_ForBackward(IO_ON);
			mn_InitStep = 210;
			break;

		case 210:
			nRet_1 = Chk_Tray_Fix_Clamp_ForBackward(IO_ON);
			if(nRet_1 == RET_GOOD)
			{
				mn_InitStep = 300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(3002, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			break;

		case 300:
// 			COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
// 			COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_LATCH, cmTRUE);//16
			CTL_Lib.SD_Sensor_Enable(0, m_nAxisNum, CTL_NO);
			mn_InitStep = 310;
			break;

		case 310:
			nRet_1 = COMI.HomeCheck_Mot( m_nAxisNum, st_motor[m_nAxisNum].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 400;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 3003, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 400:
			nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(3004, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			else if(nRet_1 == BD_ERROR)//로더 플레이드에 트레이가 감지 되지않은 상태 
			{
				mn_InitStep = 500;
			}
			break;

		case 500:
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Limit_Check, IO_OFF);
			if( nRet_1 == IO_ON)
			{
				m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Loading_Stacker_Tray_Limit_Check);
				CTL_Lib.Alarm_Error_Occurrence(3005, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			else
			{
				mn_InitStep = 1000;
			}
			break;

		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;

		case 1000:
			st_handler.mn_init_state[INIT_LDSTACKER_ELV] = CTL_YES;
			mn_InitStep = 0;
			break;
	}
}

void CRun_LdStacker_Elvator::RunMove()
{
	int nRet_1,nRet_2,x=0,y=0;

	switch( mn_RunStep )
	{
	case -1:
		mn_RunStep = 0;
		break;

	case 0:
		m_nFindLotNo_Flag = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES" &&
				g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{
				//load plate에 자재 요청
				m_nFindLotNo_Flag = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
				mn_RunStep = 10;
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetStrLastModule() != "YES" &&
					g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
				{
					m_nFindLotNo_Flag = 1;
					m_strLotNo = g_lotMgr.GetLotAt(1).GetLotID();
					m_strPartNo = g_lotMgr.GetLotAt(1).GetPartID();
					mn_RunStep = 10;
				}
				else
				{
					return;
				}
			}
		}
		if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" &&
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_WORK_TRANSFER] == LOTEND &&
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_STACKER] != LOTEND)
		{
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_STACKER] = LOTEND;
			mn_RunStep = 0;
		}

		break;

	case 10:
		CTL_Lib.SD_Sensor_Enable(0, m_nAxisNum, CTL_NO); //sd sensor clear			
		COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_MODE,cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //트레이를 받을 위치로 미리 이동한다 
		if (nRet_1 == BD_GOOD) //좌측으로 이동  
		{
			mn_retry_cnt = 0;
			mn_RunStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 10;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			CTL_Lib.Alarm_Error_Occurrence(3100, dWARNING, alarm.mstr_code);
			mn_RunStep = 10;
		}
		break; 		

	case 100:
		nRet_1 = Ready_Stacker_Move_Check(0);
		if(	nRet_1 == RET_GOOD )
		{
			mn_RunStep = 200;
		}
		else if( nRet_1 == RET_ERROR )
		{
// 			CTL_Lib.Alarm_Error_Occurrence(3101, dWARNING, m_strAlarmCode);
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
		{
			m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
			CTL_Lib.Alarm_Error_Occurrence(3102, dWARNING, m_strAlarmCode);
		}
		else if(nRet_1 == BD_ERROR)//로더 플레이드에 트레이가 감지 되지않은 상태 
		{
			mn_RunStep = 300;
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //트레이를 받을 위치로 미리 이동한다 
		if (nRet_1 == BD_GOOD) //좌측으로 이동  
		{			
			mn_RunStep = 400;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
			CTL_Lib.Alarm_Error_Occurrence(3103, dWARNING, alarm.mstr_code);
			mn_RunStep = 300;
		}
		break; 

	case 400:
		st_tray_info[THD_LD_STACKER].strLotNo = g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetLotID();
		st_tray_info[THD_LD_STACKER].strPartNo = g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetPartID();
		for(y = 0; y < st_recipe.nTrayY; y++)
		{
			for(x=0; x < st_recipe.nTrayX; x++)
			{
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strLotNo = st_tray_info[THD_LD_STACKER].strLotNo;
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strPartNo = st_tray_info[THD_LD_STACKER].strPartNo;
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strSerialNo = "";
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strCSerialNo = "";
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strBarcode = "";
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strArrNo = "";
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strPPID = "";
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strPSID = "";
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].strWWN = "";
				
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].nYesNo = CTL_YES;
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].nBin = BIN_CDIMM;
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].nRetest = 0;
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].nBdNum = 0;
				st_tray_info[THD_LD_STACKER].st_pcb_info[y][x].nEnable = CTL_YES;
			}
		}		
		
		mn_RunStep = 1000;
		break;

	case 1000:
		if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo)
		{
			if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetStrLastModule() == "YES" )
			{
				mn_RunStep = 5000;
			}
			else
			{
				if(st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] == CTL_REQ )
				{
					m_dwWaitTime[0] = GetCurrentTime();
					mn_RunStep = 1100;
				}
			}
		}
		if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" &&
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_WORK_TRANSFER] == LOTEND &&
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_STACKER] != LOTEND)
		{
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_STACKER] = LOTEND;
			mn_RunStep = 0;
		}
// 		else if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() != "YES" )
// 		{
// 			st_msg.mstr_event_msg[0] = "[LDSTACKER_ELV] There is no tray";
// 			st_msg.mstr_event_msg[1] = "in load stacker elevator";
// 			if( st_handler.cwnd_list != NULL )
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "%s", st_msg.mstr_event_msg[0]);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
// 		}
		break;

	case 1100:
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		if( m_dwWaitTime[2] <= 0 ) m_dwWaitTime[0] = GetCurrentTime();
		nRet_1 = Ready_Stacker_Move_Check(1);
		if(	nRet_1 == RET_GOOD )
		{
			st_tray_info[THD_LD_STACKER].nTrayExist = CTL_YES; //트레이존재 정보 셋
			mn_RunStep = 1200;
		}
		else
		{
			if( m_dwWaitTime[2] > 15000 )
			{
				//자재없음
				sprintf(st_msg.c_abnormal_msg, "[LDSTACKER_ELV] There is no tray in load stacker elevator. Please check sensor.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() != "YES" )
				{
					st_msg.mstr_event_msg[0] = "[LDSTACKER_ELV] There is no tray";
					st_msg.mstr_event_msg[1] = "in load stacker elevator";
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				}
				COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmFALSE);
				mn_RunStep = 1110;
			}
		}
		break;
		
	case 1110:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)
		{
			COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmTRUE);			
			mn_RunStep = 0;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			mn_RunStep = 1110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(3104, dWARNING, alarm.mstr_code);
			mn_RunStep = 1110;
		}
		break;

	case 1200:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
		if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			mn_retry_cnt = 0;
			mn_RunStep = 1300;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			CTL_Lib.Alarm_Error_Occurrence(3105, dWARNING, m_strAlarmCode);
		}
		break;

	case 1300:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
		{
			mn_retry_cnt++;
			if (mn_retry_cnt > 2)
			{
				mn_retry_cnt = 0;
				m_strAlarmCode.Format(_T("9100%d2"), m_nAxisNum); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(3106, dWARNING, m_strAlarmCode);
			}
			mn_RunStep = 1310;
			break;
		}

		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_OFF); //plate off 체크  
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON); //plate off 체크			
		if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			mn_RunStep = 2000;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(3107, dWARNING, m_strAlarmCode);
		}
		break;

	case 1310:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET);
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 2000;
		}
		break;


	case 2000://6100
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 != BD_GOOD) //로더 플레이트에 트레이가 감지 안된 상태 
		{
			mn_retry_cnt++;
			if (mn_retry_cnt > 2)
			{
				mn_retry_cnt = 0;
				m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(3108, dWARNING, m_strAlarmCode);
			}
			mn_RunStep = 1310;
			break;
		}

		//Tray lock cyclinder 체크 필요
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
		if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			mn_RunStep = 2100;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(3109, dWARNING, m_strAlarmCode);
		}
		break;

	case 2100://6200~6300
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET);
		if(nRet_1 == RET_GOOD)
		{
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_ON); //load stacker tray check    
			nRet_2 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == IO_OFF && nRet_2 != BD_GOOD) //트레이가 스태커에 없는 상태, SD 센서도 감지되지 않은 상태, 트레이가 없는 상태  
			{
				mn_RunStep = 5000;
			}
			else
			{
				mn_RunStep = 2200;
			}
		}
		break;

	case 2200:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_ERROR) //로더 플레이트에 트레이가 감지 된 상태 
		{
			m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
			CTL_Lib.Alarm_Error_Occurrence(3110, dWARNING, m_strAlarmCode);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		//load stacker elevator의 트레이 정보를 load plate로 쉬프트 한다 
		Func.Handler_Tray_DataInfo_Shift(0, CTL_YES, THD_LD_STACKER, THD_LD_TRAY_PLATE);  
		//계속 트레이를 공급하도 반복되어야 하니, 트레이 정보ㅡㄹ 셋팅만 하고 틀리어하지않는다 			
		//////////////////////////////////////////////////////////////////////////

		st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_READY;
		mn_RunStep = 3000;
		break;

	case 3000:
		mn_RunStep = 1000;
		break; 

	case 5000://7000
		//last check end 
// 		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_OFF);
// 		if(nRet_1 == IO_ON)
// 		{	
// 			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Exist_Check); 			
// 			CTL_Lib.Alarm_Error_Occurrence(3111, dWARNING, m_strAlarmCode);
// 			break;
// 		}

// 		if(st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] == CTL_REQ )
// 		{
// 			m_dwWaitTime[0] = GetCurrentTime();
// 			mn_RunStep = 5100;
// 		}
// 일단 머리아프니 랏 픽업이 모두 끝났으니 내리자
// 다음에 자재 유무 픽업 처리하자
		mn_RunStep = 6000;
		break;

	case 5100:
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		if( m_dwWaitTime[2] <= 0 ) m_dwWaitTime[0] = GetCurrentTime();
		nRet_1 = Ready_Stacker_Move_Check(1);
		if(	nRet_1 == RET_GOOD )
		{
			st_tray_info[THD_LD_STACKER].nTrayExist = CTL_YES; //트레이존재 정보 셋
			mn_RunStep = 5200;
		}
		else
		{
			if( m_dwWaitTime[2] > 15000 )
			{
				COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmFALSE);
				mn_RunStep = 6000;
			}
		}
		break;

	case 5200:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
		if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			mn_retry_cnt = 0;
			mn_RunStep = 5300;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			CTL_Lib.Alarm_Error_Occurrence(3905, dWARNING, m_strAlarmCode);
		}
		break;

	case 5300:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
		{
			mn_retry_cnt++;
			if (mn_retry_cnt > 2)
			{
				mn_retry_cnt = 0;
				m_strAlarmCode.Format(_T("9100%d2"), m_nAxisNum); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(3906, dWARNING, m_strAlarmCode);
			}
			mn_RunStep = 5310;
			break;
		}
		
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_OFF); //plate off 체크  
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON); //plate off 체크			
		if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			mn_RunStep = 5400;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(3907, dWARNING, m_strAlarmCode);
		}
		break;

	case 5310:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET);
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 5400;
		}
		break;

	case 5400://6100
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 != BD_GOOD) //로더 플레이트에 트레이가 감지 안된 상태 
		{
			mn_retry_cnt++;
			if (mn_retry_cnt > 2)
			{
				mn_retry_cnt = 0;
				m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(3108, dWARNING, m_strAlarmCode);
			}
			mn_RunStep = 5310;
			break;
		}
		
		//Tray lock cyclinder 체크 필요
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
		if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			mn_RunStep = 5500;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(3109, dWARNING, m_strAlarmCode);
		}
		break;
		
	case 5500://6200~6300
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET);
		if(nRet_1 == RET_GOOD)
		{
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_ON); //load stacker tray check    
			nRet_2 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == IO_OFF && nRet_2 != BD_GOOD) //트레이가 스태커에 없는 상태, SD 센서도 감지되지 않은 상태, 트레이가 없는 상태  
			{
				mn_RunStep = 6000;
			}
			else
			{
				mn_RunStep = 5600;
			}
		}
		break;

	case 5600:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_ERROR) //로더 플레이트에 트레이가 감지 된 상태 
		{
			m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
			CTL_Lib.Alarm_Error_Occurrence(3110, dWARNING, m_strAlarmCode);
			break;
		}
		
		//////////////////////////////////////////////////////////////////////////
		//load stacker elevator의 트레이 정보를 load plate로 쉬프트 한다 
		Func.Handler_Tray_DataInfo_Shift(0, CTL_YES, THD_LD_STACKER, THD_LD_TRAY_PLATE);  
		//계속 트레이를 공급하도 반복되어야 하니, 트레이 정보ㅡㄹ 셋팅만 하고 틀리어하지않는다 			
		//////////////////////////////////////////////////////////////////////////
		
		st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_READY;
		mn_RunStep = 5700;
		break;

	case 5700:
		if( st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] == CTL_CLEAR)//clear하는 부분이 필요하다
		{
			mn_RunStep = 6000;
		}
		break;


	case 6000:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);   //밑으로 이동한다 
		if (nRet_1 == BD_GOOD) //위로 이동 하면서 SD를 찾는다  
		{		
			mn_RunStep = 6010;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			mn_RunStep = 6000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{ 
			CTL_Lib.Alarm_Error_Occurrence(3112, dWARNING, alarm.mstr_code);
			mn_RunStep = 6000;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //2015.0407 james  //SD 위치까지 이동했는데 SD가 감지되지 않으면 트레이가 없는것이다
		if (nRet_1 == BD_GOOD)   
		{	 
			mn_RunStep = 6100;  			 
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			mn_RunStep = 6010;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{ 
			CTL_Lib.Alarm_Error_Occurrence(3113, dWARNING, alarm.mstr_code);
			mn_RunStep = 6010;
		}
		break;

	case 6100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_OFF); //stacker tary 는 없어야 한다  
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Ready_Check, IO_OFF);
			if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
			{
				st_tray_info[THD_LD_STACKER].nTrayExist = CTL_NO; //트레이존재 정보 클리어
				mn_RunStep = 1000;

				if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo )
				{
					if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetStrLastModule() == "YES" )
					{
						mn_RunStep = 0;
					}
				}
			}
			else
			{//트레이 존재가 존재한다  		

				if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo )
				{
					if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetStrLastModule() == "YES" )
					{
						mn_RunStep = 0;
						break;
					}
				}

				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Exist_Check); 				 
				CTL_Lib.Alarm_Error_Occurrence(3114, dWARNING, m_strAlarmCode);
			}			 
			break;

	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_LdStacker_Elvator  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}
}


int CRun_LdStacker_Elvator::Ready_Stacker_Move_Check( int mode )
{
	//mode : 0 : 처음 아래에서 동작할 경우 1: 위에서 SD 맞추면서 동작할 경우
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2,nRet_3,nRet_4,nRet_5,OnOff;

	st_basic.n_mode_tray_type = 0;
	if( st_basic.n_mode_tray_type == 0 )//TYPE1
	{
		OnOff = IO_ON;
	}
	else//TYPE2
	{
		OnOff = IO_OFF;
	}

//	nRet_1 = FAS_IO.Chk_IO_OnOff( st_io.i_loading_stacker_tray1_type_chk , OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
//	nRet_2 = FAS_IO.Chk_IO_OnOff( st_io.i_loading_stacker_tray2_type_chk , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_stacker_tray1_type_chk , OnOff);//, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_stacker_tray2_type_chk , !OnOff);//, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	if( mode == 0 )//아래에 있을 경우 
	{
// 		nRet_3 = FAS_IO.Chk_IO_OnOff( st_io.i_Loading_Stacker_Tray_On_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
// 		nRet_4 = FAS_IO.Chk_IO_OnOff( st_io.i_Loading_Stacker_Tray_Ready_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
		nRet_3 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_On_Check , !OnOff);//, IO_STABLE_WAIT, IO_STABLE_LIMIT );
		nRet_4 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Ready_Check , !OnOff);//, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	}
	else
	{
		nRet_3 = IO_ON;
		nRet_4 = IO_ON;
	}
//	nRet_5 = FAS_IO.Chk_IO_OnOff( st_io.i_Loading_Stacker_Tray_Exist_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	nRet_5 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Exist_Check , IO_ON);//, IO_STABLE_WAIT, IO_STABLE_LIMIT );

	if( (nRet_1 == IO_ON && nRet_2 == IO_OFF ))
	{
		if( mode == 0 )
		{
			if( nRet_3 == IO_ON && nRet_5 == IO_ON)
			{
				nFuncRet = RET_GOOD;
			}
			else
			{
				nFuncRet = RET_ERROR;
				if( nRet_3 != IO_ON)
					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_On_Check);
// 				else if( nRet_4 != IO_ON)
// 					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Ready_Check);
				else
					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Exist_Check);
			}
			
		}
		else
		{
			if( nRet_5 != IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Exist_Check);
				nFuncRet = RET_ERROR;
			}
			else
			{
				nFuncRet = RET_GOOD;
			}
			
		}		
	}
	else
	{
		m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_loading_stacker_tray1_type_chk);
	}

// 	if( (nRet_1 == IO_ON || nRet_2 == RET_PROCEED ))// && st_basic.n_mode_device != WITHOUT_DVC ) 
// 	{
// 		nFuncRet = RET_GOOD;
// 	}
// 	else if( ( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR))// && st_basic.n_mode_device != WITHOUT_DVC)
// 	{
// 		if( nRet_1 == RET_ERROR  )
// 		{
// 			m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_loading_stacker_tray1_type_chk);
// 		}
// 		else
// 		{
// 			m_strAlarmCode.Format(_T("8%d%04d"), !OnOff, st_io.i_loading_stacker_tray2_type_chk);
// 		}
// 		nFuncRet = RET_ERROR;		
// 	}
// 	else
// 	{
// 		//nFuncRet = RET_GOOD;
// 	}
	return nFuncRet;
}


void CRun_LdStacker_Elvator::Set_Tray_Fix_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Lock_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Loading_Stacker_Tray_Unlock_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

//센서가 없다 sol 출력시간만큼 딜레이
int CRun_LdStacker_Elvator::Chk_Tray_Fix_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Lock_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Unlock_Check, IO_ON) == IO_ON )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Lock_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Unlock_Check, IO_ON) == IO_ON )
		{
			m_dwClampOnOff[1] = GetCurrentTime();
			m_dwClampOnOff[2] = m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[1] = GetCurrentTime();
			m_dwClampOnOff[2] = m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Lock_Check); 
				clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Lock_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Unlock_Check, IO_OFF) == IO_OFF )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Lock_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Unlock_Check, IO_OFF) == IO_OFF )
		{
			m_dwClampOnOff[1]	= GetCurrentTime();
			m_dwClampOnOff[2]	= m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwClampOnOff[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[1]	= GetCurrentTime();
			m_dwClampOnOff[2]	= m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Lock_Check); 
				clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}
