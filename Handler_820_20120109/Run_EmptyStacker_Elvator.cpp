// Run_EmptyStacker_Elvator.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_EmptyStacker_Elvator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_EmptyStacker_Elvator
CRun_EmptyStacker_Elvator Run_EmptyStacker_Elvator;
IMPLEMENT_SERIAL(CRun_EmptyStacker_Elvator, CObject, 1);

CRun_EmptyStacker_Elvator::CRun_EmptyStacker_Elvator()
{
	mn_RunStep = -1;
	mn_InitStep = -1;
	m_nRetry = 0;
	m_nAxisNum = M_TRAY2_Z;
}

CRun_EmptyStacker_Elvator::~CRun_EmptyStacker_Elvator()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_EmptyStacker_Elvator message handlers


void CRun_EmptyStacker_Elvator::ThreadRun()
{

	switch( st_work.n_run_status)
	{
	case dINIT:
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


void CRun_EmptyStacker_Elvator::RunInit()
{
	if (st_sync.n_init_flag[THD_EMPTY_STACKER] != INIT_CLEAR)		return;		//INIT_CLEAR 일때만 초기화 작업을 한다. 초기화가 끝나면 INIT_CLEAR -> INIT_READY가 되기 떄문에...

	switch( mn_InitStep )
	{
	case -1:
		if( st_sync.n_init_flag[THD_WORK_TRANSFER] < INIT_READY ) break;
		mn_InitStep = 0;

	case 0:
		break;
	}
}

void CRun_EmptyStacker_Elvator::RunMove()
{

	int nRet_1, nRet_2=0, nRet_3=0,nRet_4=0;
	int nCount = 0;

	sFunc.ThreadFunctionStepTrace(3, mn_RunStep);		

	switch (mn_RunStep)
	{ 
	case -1: 

		mn_RunStep = 0;
		break;

	case  0:
		m_nFindLotNo_Flag = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetLotCount )
			{
				//load plate에 자재 요청
				m_nFindLotNo_Flag = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetLotCount )
				{
					m_nFindLotNo_Flag = 1;
					m_strLotNo = g_lotMgr.GetLotAt(1).GetLotID();
					m_strPartNo = g_lotMgr.GetLotAt(1).GetPartID();
				}
				else
				{
					return;
				}
			}
			mn_RunStep = 100;
		}
		break;

	case 100:
		nRet_1 = Ready_Stacker_Move_Check(0);
		if( 	nRet_1 == RET_GOOD )
		{
			mn_RunStep = 200;
		}
		else if( nRet_1 == RET_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence(2100, dWARNING, m_strAlarmCode);
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
		{
			m_strAlarmCode.Format(_T("910102")); //910102 1 A "EMPTY_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
			CTL_Lib.Alarm_Error_Occurrence(6060, dWARNING, m_strAlarmCode);
		}
		else if(nRet_1 == BD_ERROR)//로더 플레이드에 트레이가 감지 되지않은 상태 
		{
			mn_RunStep = 300;
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].d_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //트레이를 받을 위치로 미리 이동한다 
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
			CTL_Lib.Alarm_Error_Occurrence(6050, dWARNING, st_alarm_info.strCode);
			mn_RunStep = 300;
		}
		break; 

	case 400:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD)
		{
			mn_RunStep = 500;
			break;
			//m_strAlarmCode.Format(_T("910103")); //910103 1 A "EMPTY_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
			//CTL_Lib.Alarm_Error_Occurrence(4100, dWARNING, m_strAlarmCode);
		}
		else if(nRet_1 == BD_ERROR)
		{
			mn_RunStep = 500;
		}
		break;

	case 500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_Unloading_Stacker_Tray_Exist_Check, IO_OFF); //stacker tary 는 처음에는 없어야 한다  
		nRet_2 = FAS_IO.get_in_bit(st_io.i_Unloading_Stacker_Tray_Ready_Check, IO_OFF); //stacker tary 는 처음에는 없어야 한다  
		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			mn_RunStep = 1000;
		}
		else
		{//트레이 존재 에러 
			mn_RunStep = 1000;//james 2016.0915 빈 트레이가 있어도 진행하자. 향후 고객요청 상황을 파악하여 에러 여/부 결정 필요 
			break;

			if( nRet_1 == IO_ON )
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Unloading_Stacker_Tray_Exist_Check);  
			else
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Unloading_Stacker_Tray_Ready_Check);  
			CTL_Lib.Alarm_Error_Occurrence(4110, dWARNING, m_strAlarmCode);
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////
	// empty stacker와 연관하여 작업하는 영역
	//////////////////////////////////////////////////////////////////////////////////
	case 1000:
			if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_REQ) //트랜스퍼에서 empty stacker에 빈 트레이를 놓게 요청
			{
				if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)
				{
					mn_RunStep = 2000;
				}						
			}	
			else
			{
				if(m_nFindLotNo_Flag == 0 )//LOT_CURR
				{
					if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" &&
						st_sync.nLotEndFlag[THD_WORK_TRANSFER] == dLOTEND &&
						st_sync.nLotEndFlag[THD_EMPTY_STACKER] != dLOTEND)
					{
						mn_RunStep = 9000;
					}
				}
				else if(m_nFindLotNo_Flag == 1 ) //LOT_NEXT
				{
					if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" &&
						st_sync.nLotEndFlag[THD_WORK_TRANSFER] == dLOTEND &&
						st_sync.nLotEndFlag[THD_EMPTY_STACKER] != dLOTEND)
					{
						mn_RunStep = 9000;
					}
				}
			}
			break;

	case 1100:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //트레이를 트랜스퍼에서 받는다 

		if(nRet_1 == RET_GOOD)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Unloading_Stacker_Tray_Exist_Check); 
			CTL_Lib.Alarm_Error_Occurrence(4130, dWARNING, m_strAlarmCode); 

			mn_RunStep = 1000;
		}
		break;

	case 2000:
		//if(st_sync_info.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)//받는 동작으로 비어 있어도 됨 
		mn_RunStep = 3000;		
		break;

	case 3000:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //트레이를 트랜스퍼에서 받는다 
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 3100;
		}
		else if(nRet_1 == RET_ABORT)// TRAY FULL Error
		{
			if (st_handler.cwnd_main != NULL)
			{
				st_other.nBuzYesNo = YES;
				st_other.strBoxMsg = _T("[TRAY FULL] Empty stacker is full. \r\n Please Remove it.");
				st_handler.cwnd_main->SendMessage(WM_WORK_COMMAND, MAIN_MESSAGE_BOX_CREATE_REQ, 0);
			}
		}
		break;

	case 3100:
		m_nRetry = 0;
		st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_READY;
		mn_RunStep = 4000;
		break;

	case 3200:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET); //트레이를 트랜스퍼에서 받는다 

		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 4000;
		}
		break;

	case 4000:
		if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_FREE) //트레스퍼 place 트레이 작업 완료 
		{
			if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)
			{
				nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
				if(nRet_1 == BD_GOOD)//플레이트에 트레이가 감지 된 상태 
				{
					st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_CLEAR;
					st_sync_info.nWorkTransfer_Req[THD_EMPTY_STACKER][1] = CTL_CLEAR;
					mn_RunStep = 5000;
				}
				if(nRet_1 == BD_ERROR) //로더 플레이트에 트레이가 감지 안된 상태 
				{
					m_nRetry++;
					if (m_nRetry > 2)
					{
						m_nRetry = 0;
						m_strAlarmCode.Format(_T("900004")); //900001 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR" //kwlee 2016.0902 "900000" ->"900004"
						CTL_Lib.Alarm_Error_Occurrence(4180, dWARNING, m_strAlarmCode); 
					}
					mn_RunStep = 3200;

					break;
				}
			}				
		}
		break;

	case 5000:
		mn_RunStep = 1000;
		break;
		
	//////////////////////////////////////////////////////////////////////////
	//LOT END
	//////////////////////////////////////////////////////////////////////////
	case 9000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate); 

			if (nRet_1 == BD_GOOD) //좌측으로 이동  
			{			
				st_sync.nLotEndFlag[THD_EMPTY_STACKER] = dLOTEND;
				mn_RunStep = 0;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 9100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
				CTL_Lib.Alarm_Error_Occurrence(4200, dWARNING, st_alarm_info.strCode);
				mn_RunStep = 9100;
			}
			break; 

	}
}

int CRun_EmptyStacker_Elvator::Ready_Stacker_Move_Check( int mode )
{
	//mode : 0 : 처음 아래에서 동작할 경우 1: 위에서 SD 맞추면서 동작할 경우
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2,nRet_3,nRet_4,nRet_5,OnOff;

	if( st_basic.n_mode_tray_type == 0 )//TYPE1
	{
		OnOff = IO_ON;
	}
	else//TYPE2
	{
		OnOff = IO_OFF;
	}

	nRet_1 = FAS_IO.Chk_IO_OnOff( st_io.i_unloading_stacker_tray1_type_chk , OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	nRet_2 = FAS_IO.Chk_IO_OnOff( st_io.i_unloading_stacker_tray2_type_chk , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	if( mode == 0 )//아래에 있을 경우 
	{
		nRet_3 = FAS_IO.Chk_IO_OnOff( st_io.i_Unloading_Stacker_Tray_On_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
		nRet_4 = FAS_IO.Chk_IO_OnOff( st_io.i_Unloading_Stacker_Tray_Ready_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	}
	else
	{
		nRet_3 = IO_ON;
		nRet_4 = IO_ON;
	}
	nRet_5 = FAS_IO.Chk_IO_OnOff( st_io.i_Unloading_Stacker_Tray_Exist_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );

	if( (nRet_1 == RET_PROCEED || nRet_2 == RET_PROCEED ) && st_basic.n_mode_device != WITHOUT_DVC ) 
	{
		break;
	}
	else if( ( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR) && st_basic.n_mode_device != WITHOUT_DVC)
	{
		if( nRet_1 == RET_ERROR  )
		{
			m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_unloading_stacker_tray1_type_chk);
		}
		else
		{
			m_strAlarmCode.Format(_T("8%d%04d"), !OnOff, st_io.i_unloading_stacker_tray2_type_chk);
		}
		nFuncRet = RET_ERROR;		
	}
	else
	{
		nFuncRet = RET_GOOD;
	}
	return nFuncRet;
}
