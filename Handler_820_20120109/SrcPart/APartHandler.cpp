// APartHandler.cpp: implementation of the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartHandler.h"
#include "../IO_Manager.h"
#include "../ComizoaPublic.h"
#include "../AMTRegistry.h"
#include "../Run_Epoxy_Transfer_Robot.h"
#include "../Run_HeatSinkVision_Transfer_Robot.h"
#include "../Cmmsdk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartHandler	g_handler;

APartHandler::APartHandler()
{
	m_empos = EMPOS_FRONT;
	nLightCurtainMutingOnStep = 0;
	nLightCurtainMutingOffStep = 0;
	nSetInterfaceHeartBitStep = 0;
	nGetInterfaceHeartBitStep = 0;
	n_InterfaceHeartBitCount = 0;
	m_nHardnessEpoxyStep = 0;
	m_safetyStep = 0;

	m_nEpoxyCnt = 0;
	m_nAlarmCnt = 0;

	m_nHardnessTime = 0;

	m_nAlarmCnt = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM );
	m_nEpoxyCnt = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_EPOXY );
	m_nHardnessTime = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_TIME_HARDNESS );

	st_handler.n_HSAbleOn = IO_OFF;
	st_handler.n_InterfaceConnectOK = IO_OFF;
	st_handler.n_CaseAssemblyUnloadingAbleOn = IO_OFF;

	m_nEpoxyMoveStep = 0;
	m_nEpoxySafetyStep = 0;
	m_nEpoxyCleanTimeStep= 0;
	m_nHardnessStep = 0;
	m_nEpoxyOutStep = 0;
	m_nChkEpoxyYes = CTL_NO;
	st_handler.m_nAssemblyAble = CTL_YES;

}

APartHandler::~APartHandler()
{

}

void APartHandler::Thread_Run()
{
	AlarmEpoxyCleanTime();
	AlarmHardnessExcessTimeEpoxyCnt();
	if( st_work.mn_run_status != dRUN )	
		return;

	Run_Move_Main();
}

void APartHandler::Run_Move_Main()
{
	if ( g_ioMgr.GetIOBoardInit() == true /* &&*/
		 /*st_handler.mn_motor_init_check == TRUE */)
	{
		HsInterfaceChk();
		Set_Interface_HeartBit();
		OnLightCurtainCheck();
	}	
}

void APartHandler::AddAlarmCnt()
{
	m_nAlarmCnt++;
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM, m_nAlarmCnt );
}

void APartHandler::AddEpoxyCnt()
{
	m_nEpoxyCnt++;
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_EPOXY, m_nEpoxyCnt );
}

void APartHandler::AddHardnessTime()
{
	m_nHardnessTime++;
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_TIME_HARDNESS, m_nHardnessTime );
}

void APartHandler::ClearAlarmCnt()
{
	m_nAlarmCnt = 0;
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM, m_nAlarmCnt );
}

void APartHandler::ClearEpoxyCnt()
{
	m_nEpoxyCnt = 0;
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_EPOXY, m_nEpoxyCnt );
}

void APartHandler::ClearHardnessTime()
{
	m_nHardnessTime = 0;//2017.0610
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_TIME_HARDNESS, m_nHardnessTime );
}

void APartHandler::Case_Assembly_Able_Check()
{
	st_handler.n_InterfaceConnectOK = 1;
	
	if(g_ioMgr.get_in_bit(st_io.i_Interface_Input_1, IO_ON) == IO_ON && st_handler.n_InterfaceConnectOK == 1)
	{
		//		st_inspect.n_InterfaceConnectOK = 0;
		st_handler.n_CaseAssemblyUnloadingAbleOn = IO_ON;
		st_handler.nInterLockTimeCheck = GetCurrentTime();
	}
	else
	{
		st_handler.n_CaseAssemblyUnloadingAbleOn = IO_OFF;
	}
}

void APartHandler::Case_Assembly_Place_Check()
{
	if(g_ioMgr.get_in_bit(st_io.i_Interface_Input_3, IO_ON) == IO_ON)
	{
		st_handler.n_HSLowerPlaceAbleCheck = IO_ON;
	}
	else
	{
		st_handler.n_HSLowerPlaceAbleCheck = IO_OFF;
	}
}

void APartHandler::HS_Able_Signal(int OnOff)
{
	if( st_handler.m_nAssemblyAble == CTL_NO)
	{
		OnOff = OFF;
	}

	switch(OnOff)
	{
	case ON:
		g_ioMgr.set_out_bit(st_io.o_Interface_Output_1, IO_ON);
		st_handler.n_HSAbleOn = 1;
		st_handler.nInterLockTimeCheck = GetCurrentTime();
		break;
		
	case OFF:
		g_ioMgr.set_out_bit(st_io.o_Interface_Output_1, IO_OFF);
		st_handler.n_HSAbleOn = 0;
		break;
	}
}

void APartHandler::Set_Light_Curtain_Muting_On()	
{
	switch(nLightCurtainMutingOnStep)
	{
	case 0:
		if(st_handler.n_MutingOn == 1)
		{
			g_ioMgr.set_out_bit(st_io.o_Loading_On_Release, IO_ON);
			lLightCurtainMutingOnTime[0] = GetCurrentTime();
			nLightCurtainMutingOnStep = 10;
		}
		break;
		
	case 10:
		lLightCurtainMutingOnTime[1] = GetCurrentTime();
		lLightCurtainMutingOnTime[2] = lLightCurtainMutingOnTime[1] - lLightCurtainMutingOnTime[0];
		
		if(lLightCurtainMutingOnTime[2] > 1000) //1초 
		{
			g_ioMgr.set_out_bit(st_io.o_Loading_On_Release, IO_OFF);
			lLightCurtainMutingOnTime[0] = GetCurrentTime();
			nLightCurtainMutingOnStep = 0;
			st_handler.n_MutingOn = 0;
		}
		break;
	}
}


void APartHandler::Set_Light_Curtain_Muting_Off()	
{
	switch(nLightCurtainMutingOffStep)
	{
	case 0:
		if(st_handler.n_MutingOff == 1)
		{
			g_ioMgr.set_out_bit(st_io.o_Loading_Off_Release, IO_ON);
			lLightCurtainMutingOffTime[0] = GetCurrentTime();
			nLightCurtainMutingOffStep = 10;
		}
		break;
		
	case 10:
		lLightCurtainMutingOffTime[1] = GetCurrentTime();
		lLightCurtainMutingOffTime[2] = lLightCurtainMutingOffTime[1] - lLightCurtainMutingOffTime[0];
		
		if(lLightCurtainMutingOffTime[2] > 1000) //1초 
		{
			g_ioMgr.set_out_bit(st_io.o_Loading_Off_Release, IO_OFF);
			lLightCurtainMutingOffTime[0] = GetCurrentTime();
			nLightCurtainMutingOffStep = 0;
			st_handler.n_MutingOff = 0;
		}
		break;
	}
}

void APartHandler::Set_Interface_HeartBit()
{

	switch(nSetInterfaceHeartBitStep)
	{
	case 0:
		g_ioMgr.set_out_bit(st_io.o_Interface_Output_2, IO_ON);
		lSetInterfaceHeartBitTime[0] = GetCurrentTime();
		nSetInterfaceHeartBitStep = 10;
		
		break;
		
	case 10:
		lSetInterfaceHeartBitTime[1] = GetCurrentTime();
		lSetInterfaceHeartBitTime[2] = lSetInterfaceHeartBitTime[1] - lSetInterfaceHeartBitTime[0];
		if(lSetInterfaceHeartBitTime[2] <= 0 ) lSetInterfaceHeartBitTime[0] = GetCurrentTime();
		if(lSetInterfaceHeartBitTime[2] > 500) //2초 
		{
			g_ioMgr.set_out_bit(st_io.o_Interface_Output_2, IO_OFF);
			lSetInterfaceHeartBitTime[0] = GetCurrentTime();
			nSetInterfaceHeartBitStep = 20;
		}
		break;
		
	case 20:
		lSetInterfaceHeartBitTime[1] = GetCurrentTime();
		lSetInterfaceHeartBitTime[2] = lSetInterfaceHeartBitTime[1] - lSetInterfaceHeartBitTime[0];
		if(lSetInterfaceHeartBitTime[2] <= 0 ) lSetInterfaceHeartBitTime[0] = GetCurrentTime();		
		if(lSetInterfaceHeartBitTime[2] > 500) //2초 
		{
			g_ioMgr.set_out_bit(st_io.o_Interface_Output_2, IO_ON);
			lSetInterfaceHeartBitTime[0] = GetCurrentTime();
			nSetInterfaceHeartBitStep = 10;
		}
		break;
	}
}

void APartHandler::Get_Interface_HeartBit()
{
	switch(nGetInterfaceHeartBitStep)
	{
	case 0:
		lGetInterfaceHeartBitTime[0] = GetCurrentTime();
		nGetInterfaceHeartBitStep = 10;
		break;
		
	case 10:
		lGetInterfaceHeartBitTime[1] = GetCurrentTime();
		lGetInterfaceHeartBitTime[2] = lGetInterfaceHeartBitTime[1] - lGetInterfaceHeartBitTime[0];
		
		if(g_ioMgr.get_in_bit(st_io.i_Interface_Input_2, IO_ON) == IO_ON/* && st_handler.n_InterfaceConnectOK != 1*/)
		{
			n_InterfaceHeartBitCount++;
			
			if(n_InterfaceHeartBitCount > 3)
			{
				n_InterfaceHeartBitCount = 0;
				st_handler.n_InterfaceConnectOK = 1;
			}
			lGetInterfaceHeartBitTime[0] = GetCurrentTime();
			nGetInterfaceHeartBitStep = 20;
		}
		else if((lGetInterfaceHeartBitTime[2] > 1000 * 10))
		{
			n_InterfaceHeartBitCount = 0;
			st_handler.n_InterfaceConnectOK = 0;
			nGetInterfaceHeartBitStep = 0;
		}
		break;
		
	case 20:
		lGetInterfaceHeartBitTime[1] = GetCurrentTime();
		lGetInterfaceHeartBitTime[2] = lGetInterfaceHeartBitTime[1] - lGetInterfaceHeartBitTime[0];
		
		if(g_ioMgr.get_in_bit(st_io.i_Interface_Input_2, IO_OFF) == IO_OFF)
		{
			lGetInterfaceHeartBitTime[0] = GetCurrentTime();
			nGetInterfaceHeartBitStep = 10;
		}
		else if((lGetInterfaceHeartBitTime[2] > 1000 * 10))
		{
			n_InterfaceHeartBitCount = 0;
			st_handler.n_InterfaceConnectOK = 0;
			nGetInterfaceHeartBitStep = 0;
		}
		
		break;
	}
}


void APartHandler::HsInterfaceChk()
{
	//================================
	// 자신의 상태를 Upper에 알려준다.
	//================================
	// 	char Jamcode[10] ;
	
	CTime t_CurTime	=	CTime::GetCurrentTime();
	
	
	//INTERLOCK
	st_basic.n_mode_case_assembly = 0;
	if(st_basic.n_mode_case_assembly == 0)
	{
		Case_Assembly_Able_Check();
		Case_Assembly_Place_Check();
	}
	else
	{
		st_handler.n_CaseAssemblyUnloadingAbleOn = IO_ON;
		st_handler.n_HSLowerPlaceAbleCheck = IO_ON;
	}
	
	if( ( COMI.Get_MotCurrentPos(M_CARRIER_X) < ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] + COMI.md_allow_value[M_CARRIER_X] ) ) &&
		( COMI.Get_MotCurrentPos(M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + 2 )) &&
		st_handler.n_CaseAssemblyUnloadingAbleOn == IO_ON &&
		g_ioMgr.get_in_bit(st_io.i_Interface_Input_1, IO_ON) == IO_ON )
		{
			/////조립기 이동 불가능 ....
			HS_Able_Signal(IO_OFF);
		}
	else if( ( COMI.Get_MotCurrentPos(M_CARRIER_X) > (st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] - 5)) &&
			 ( COMI.Get_MotCurrentPos(M_LOADER_TRANSFER_Y) < ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + 2) ) )
	{
		HS_Able_Signal(IO_ON);
	}

	if( st_handler.n_CaseAssemblyUnloadingAbleOn == IO_OFF &&
		st_handler.n_HSAbleOn == 1 &&
		st_work.mn_run_status == dRUN &&
		( GetTickCount() > ( ( st_handler.nInterLockTimeCheck + 1000) * 60 * 5 ) ) )
	{
		st_handler.nInterLockTimeCheck = GetCurrentTime();
		CTL_Lib.Alarm_Error_Occurrence( 7899, dWARNING, "900013");		
	}	
}


int APartHandler::OnEmoCheck()
{
	if (g_ioMgr.get_in_bit(st_io.i_EMO_Switch1_Check, IO_ON) == IO_OFF)
	{
		CTL_Lib.Alarm_Error_Occurrence(7900, dWARNING, _T("811604"));
		
		return RET_ERROR;
	}
	
	if (g_ioMgr.get_in_bit(st_io.i_EMO_Switch2_Check, IO_ON) == IO_OFF)
	{
		CTL_Lib.Alarm_Error_Occurrence(7901, dWARNING, _T("811605"));
		
		return RET_ERROR;
	}
	
	if (g_ioMgr.get_in_bit(st_io.i_EMO_Switch3_Check, IO_ON) == IO_OFF)
	{
		CTL_Lib.Alarm_Error_Occurrence(7902, dWARNING, _T("811606"));
		
		return RET_ERROR;
	}

	
	return RET_GOOD;
}

int APartHandler::OnLightCurtainCheck()
{
	int nFuncRet = RET_GOOD;
	return nFuncRet;
	CString strErr;

// 	st_io.i_Light2_Curtain_Check					=	1600;						///PS1500
// 	st_io.i_LoadingOn_SW_Check						=	1602;						///PS1602
// 	st_io.i_LoadingOff_SW_Check						=	1603;						///PS1602
// 	st_io.o_Loading_Off_Release						=	1613;						///S1602
// 	st_io.o_Loading_On_Lamp							=	1614;						///S1603
// 	st_io.o_Loading_Off_Lamp						=	1615;						///S1604


	m_nSafetyChk[0] = g_ioMgr.get_in_bit( st_io.i_LoadingOn_SW_Check, IO_ON);
	m_nSafetyChk[1] = g_ioMgr.get_in_bit( st_io.i_LoadingOff_SW_Check, IO_OFF);
	m_nSafetyChk[2] = g_ioMgr.get_in_bit( st_io.i_Light1_Curtain_Check, IO_OFF);//sense

	switch(m_safetyStep)
	{
		case 0:
			if(/* m_nSafetyChk[1] == IO_ON ||*/ m_nSafetyChk[2] == IO_ON )
			{
				st_sync.nLdUld_LightChk = NO;
				m_safetyStep = 100;
				m_dwWaitSafetyTime[0] = GetCurrentTime();
			}
			else
			{
				if( st_sync.nLdUld_LightChk == YES)
				{
					st_sync.nLdUld_LightChk = NO;
				}
			}
			break;

		case 100:
			if(/* m_nSafetyChk[1] == IO_ON ||*/ m_nSafetyChk[2] == IO_ON )
			{
				m_dwWaitSafetyTime[1] = GetCurrentTime();
				m_dwWaitSafetyTime[2] = m_dwWaitSafetyTime[1] - m_dwWaitSafetyTime[0];
				if( m_dwWaitSafetyTime[2] <= 0) 
				{
					m_dwWaitSafetyTime[0] = GetCurrentTime();
				}
				else if( m_dwWaitSafetyTime[2] > 100 )
				{
					m_safetyStep = 200;
				}
			}
			else
			{
				m_safetyStep = 0;
			}
			break;

		case 200:
			if( /*m_nSafetyChk[1] == IO_ON ||*/ m_nSafetyChk[2] == IO_ON )
			{				
				if( m_nSafetyChk[0] == CTL_YES)
				{
					m_dwWaitSafetyTime[0] = GetCurrentTime();
					strErr.Format( _T("Safety : Load unload light curtain check error" ) );					
					st_msg.mstr_event_msg[0] = strErr;
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);	
					st_sync.nLdUld_LightChk = YES;
					m_safetyStep = 300;
				}
				else
				{
					//801214 1 0 ""LOAD/UNLOAD_ZONE_LIGHT_CURTAIN_OFF_CHECK_ERROR"
					CTL_Lib.Alarm_Error_Occurrence(9007, dWARNING, _T("801214"));
					nFuncRet =RET_ERROR;
					m_safetyStep = 0;
				}
			}
			else
			{
				m_safetyStep = 0;
			}
			break;

		case 300:
			if(/* m_nSafetyChk[1] == IO_ON ||*/ m_nSafetyChk[2] == IO_ON )
			{
				if( m_nSafetyChk[0] == CTL_YES && st_sync.nLdUld_LightChk == YES)
				{
					m_dwWaitSafetyTime[1] = GetCurrentTime();
					m_dwWaitSafetyTime[2] = m_dwWaitSafetyTime[1] - m_dwWaitSafetyTime[0];
					if( m_dwWaitSafetyTime[2] <= 0) 
					{
						m_dwWaitSafetyTime[0] = GetCurrentTime();
					}
					else if( m_dwWaitSafetyTime[2] > 20000 )
					{
						m_safetyStep = 0;
						::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
						//801214 1 0 ""LOAD/UNLOAD_ZONE_LIGHT_CURTAIN_OFF_CHECK_ERROR"
						CTL_Lib.Alarm_Error_Occurrence(9008, dWARNING, _T("801214"));
						nFuncRet =RET_ERROR;
					}
				}
				else
				{
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
					//801214 1 0 ""LOAD/UNLOAD_ZONE_LIGHT_CURTAIN_OFF_CHECK_ERROR"
					CTL_Lib.Alarm_Error_Occurrence(9007, dWARNING, _T("801214"));
					nFuncRet =RET_ERROR;
					m_safetyStep = 0;
				}
			}
			else
			{
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
				m_safetyStep = 0;
			}
			break;
	}
	return nFuncRet;

}

void APartHandler::ClearStep()
{
	m_nEpoxyMoveStep = 0;
	m_nEpoxySafetyStep = 0;
	m_nHardnessStep = 0;
	m_nHardnessOut = 0;
	m_nEpoxyCleanTimeStep = 0;
}

void APartHandler::AlarmEpoxyCleanTime()
{


	switch( m_nEpoxyCleanTimeStep )
	{
		case 0:
			m_nEpoxyCleanTimeStep = 100;
			break;

		case 100:
			if( g_ioMgr.get_in_bit( st_io.i_Epoxy_Level_Bottom, IO_ON ) == IO_ON )
			{
				m_dwEpoxyCheckTime[0] = GetCurrentTime();
				m_nEpoxyCleanTimeStep = 110;
			}
			else
			{
				if( st_handler.m_nEpoxyCleanAlarm == CTL_YES )
					st_handler.m_nEpoxyCleanAlarm = CTL_NO;
			}
			break;
			//densorcheck YES;

		case 110:
			if( g_ioMgr.get_in_bit( st_io.i_Epoxy_Level_Bottom, IO_ON ) == IO_ON )
			{
				m_dwEpoxyCheckTime[1] = GetCurrentTime();
				m_dwEpoxyCheckTime[2] = m_dwEpoxyCheckTime[1] - m_dwEpoxyCheckTime[0];
				if( m_dwEpoxyCheckTime[2] <= 0 ) m_dwEpoxyCheckTime[0] = GetCurrentTime();
				if( m_dwEpoxyCheckTime[2] > 1000)
				{
					m_nEpoxyCleanTimeStep = 200;
					m_nChkEpoxyYes = YES;
				}
				else
				{
					m_nEpoxyCleanTimeStep = 110;
				}
			}
			else
			{
				m_nEpoxyCleanTimeStep = 0;
			}
			break;

		case 200:
			if( m_nChkEpoxyYes == CTL_YES )
			{
				st_handler.m_dwEpoxyCleanWaitTime[0] = GetCurrentTime();
				m_nEpoxyCleanTimeStep = 300;
			}
			if( g_ioMgr.get_in_bit( st_io.i_Epoxy_Level_Bottom, IO_ON ) == IO_OFF )
			{
				m_dwEpoxyCheckTime[0] = GetCurrentTime();
				m_nEpoxyCleanTimeStep = 210;
			}
			break;

		case 210:
			m_dwEpoxyCheckTime[1] = GetCurrentTime();
			m_dwEpoxyCheckTime[2] = m_dwEpoxyCheckTime[1] - m_dwEpoxyCheckTime[0];
			if( m_dwEpoxyCheckTime[2] <= 0 ) m_dwEpoxyCheckTime[0] = GetCurrentTime();
			if( g_ioMgr.get_in_bit( st_io.i_Epoxy_Level_Bottom, IO_ON ) == IO_ON )
			{
				if( m_dwEpoxyCheckTime[2] > 10000)
				{
					m_nEpoxyCleanTimeStep = 200;
					m_nChkEpoxyYes = YES;
				}
				else
				{
					m_nEpoxyCleanTimeStep = 210;
				}
			}
			else
			{
				if( m_dwEpoxyCheckTime[2] > 60000)
				{
					m_nEpoxyCleanTimeStep = 0;
					m_nChkEpoxyYes = NO;
				}
			}
			break;		

		case 300:
			if( st_handler.m_nEpoxyCleanAlarm == CTL_NO )
			{
				m_nEpoxyCleanTimeStep = 0;
				break;
			}

			if( m_nChkEpoxyYes == CTL_YES )
			{
				st_handler.m_nEpoxyCleanAlarm = CTL_READY;
				st_handler.m_dwEpoxyCleanWaitTime[1] = GetCurrentTime();
				st_handler.m_dwEpoxyCleanWaitTime[2] = st_handler.m_dwEpoxyCleanWaitTime[1] - st_handler.m_dwEpoxyCleanWaitTime[0];

				if( st_handler.m_dwEpoxyCleanWaitTime[2] > (st_basic.n_EPoxyCleanTime * 60 * 1000 ))
				{
					st_handler.m_nEpoxyCleanAlarm = CTL_YES;

					st_msg.mstr_event_msg[0] = "[EPOXY] EPOXY CLEAN!!!";

					if( st_handler.cwnd_list != NULL)
					{
						sprintf( st_msg.c_abnormal_msg, "%s", st_msg.mstr_event_msg[0]);
						st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					m_dwEpoxyCheckTime[0] = GetCurrentTime();

					m_nEpoxyCleanTimeStep = 1000;
				}
				else if( st_handler.m_dwEpoxyCleanWaitTime[2] <= 0 )
				{
					st_handler.m_dwEpoxyCleanWaitTime[0] = GetCurrentTime();
				}
			}
			if( g_ioMgr.get_in_bit( st_io.i_Epoxy_Level_Bottom, IO_ON ) == IO_OFF )
			{
				m_dwEpoxyCheckTime[0] = GetCurrentTime();
				m_nEpoxyCleanTimeStep = 210;
			}
			break;

		case 1000:
			m_dwEpoxyCheckTime[1] = GetCurrentTime();
			m_dwEpoxyCheckTime[2] = m_dwEpoxyCheckTime[1] - m_dwEpoxyCheckTime[0];
			if( m_dwEpoxyCheckTime[2] <= 0 ) m_dwEpoxyCheckTime[0] = GetCurrentTime();

			if( st_handler.m_nEpoxyCleanAlarm == CTL_NO )
			{
				m_nEpoxyCleanTimeStep = 0;
			}
			else
			{
				if( m_dwEpoxyCheckTime[2] > (5*60000) )
				{
					st_msg.mstr_event_msg[0] = "[EPOXY] EPOXY CLEAN!!!";
					
					if( st_handler.cwnd_list != NULL)
					{
						sprintf( st_msg.c_abnormal_msg, "%s", st_msg.mstr_event_msg[0]);
						st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
					m_dwEpoxyCheckTime[0] = GetCurrentTime();
				}
			}
			break;

	}

}

int APartHandler::MoveCleanEpoxy()
{
	int nRet_1 = 0, nRet_2 = 0;
	int nFunRet = RET_PROCEED;
	int m_nRobot_X = M_EPOXY_TRANSFER_X;
	int m_nRobot_Y = M_EPOXY_TRANSFER_Y;
	int m_nRobot_Z = M_EPOXY_TRANSFER_Z;
	
	switch( m_nEpoxyMoveStep )
	{
	case 0:
		m_nEpoxyMoveStep = 10;
		break;
		
	case 10:		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			Func.VppmOff();
			m_nEpoxyMoveStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_nEpoxyMoveStep = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(9899, dWARNING, alarm.mstr_code);
			m_nEpoxyMoveStep = 0;
			nFunRet = RET_ERROR;
		}
		break;
		
	case 100:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS], COMI.md_allow_value[m_nRobot_X] );
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nRobot_Y] );
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			m_nEpoxyMoveStep = 200;
		}
		else
		{
			Func.VppmOff();
			m_nEpoxyMoveStep = 1000;
		}
		break;
		
	case 200:
		nRet_1 = Run_Epoxy_Transfer_Robot.Robot_Move_Safety_Zone( 0, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			Run_Epoxy_Transfer_Robot.mn_SafetyStep = 0;
			m_nEpoxyMoveStep = 100;
		}
		break;
		
	case 1000:
		m_nEpoxyMoveStep = 1100;
		break;
		
	case 1100:			
		nRet_1 = Run_Epoxy_Transfer_Robot.Robot_Move_Safety_Zone( 2, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			m_nEpoxyMoveStep = 2000;
		}
		break;
		
	case 2000:
		m_nEpoxyMoveStep = 0;
		nFunRet = RET_GOOD;
		break;		
	}

	if( st_work.mn_run_status == dWARNING )
		nFunRet = RET_ERROR;

	return nFunRet;
}

int APartHandler::MoveSafetyEpoxy()
{
	int nRet_1 = 0, nRet_2 = 0;
	int nFunRet = RET_PROCEED;
	int m_nRobot_X = M_EPOXY_TRANSFER_X;
	int m_nRobot_Y = M_EPOXY_TRANSFER_Y;
	int m_nRobot_Z = M_EPOXY_TRANSFER_Z;
	
	switch( m_nEpoxySafetyStep )
	{
	case 0:
		m_nEpoxySafetyStep = 10;
		break;
		
	case 10:		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			Func.VppmOff();
			m_nEpoxySafetyStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_nEpoxySafetyStep = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(9899, dWARNING, alarm.mstr_code);
			m_nEpoxySafetyStep = 0;
			nFunRet = RET_ERROR;
		}
		break;
		
	case 100:
		nRet_1 = Run_Epoxy_Transfer_Robot.Robot_Move_Safety_Zone( 0, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			Run_Epoxy_Transfer_Robot.mn_SafetyStep = 0;
			m_nEpoxySafetyStep = 200;
		}
		break;

	case 200:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS], COMI.md_allow_value[m_nRobot_X] );
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nRobot_Y] );
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			m_nEpoxySafetyStep = 100;
		}
		else
		{
			m_nEpoxySafetyStep = 1000;
		}
		break;
		
	case 1000:
		Func.VppmOff();
		m_nEpoxySafetyStep = 2000;
		break;		
		
	case 2000:
		m_nEpoxySafetyStep = 0;
		nFunRet = RET_GOOD;
		break;		
	}
	
	if( st_work.mn_run_status == dWARNING )
		nFunRet = RET_ERROR;
	
	return nFunRet;
}

int APartHandler::MoveHardnessOut()
{
	int nRet_1 = 0, nRet_2 = 0, nRet_3 = 0;
	int nFunRet = RET_PROCEED;
	int m_nAxisNum = M_DISPENSER_Y;
	int m_nRobot_DisY = M_DISPENSER_Y;
	CString strAlarm = _T("");
	
	double dCurrDisY, dCurrHeatSinkX, dCurrHeatSinkY;
	
	dCurrHeatSinkX = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_X );
	dCurrHeatSinkY = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y );
	
	switch( m_nHardnessStep )
	{
		case 0:
			m_nHardnessOut = 0;
			m_nHardnessStep = 5;
			break;

		case 5:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_nHardnessStep = 10;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_nHardnessStep = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9780, dWARNING, alarm.mstr_code);
				nFunRet = RET_ERROR;
			}
			break;

		case 10:
			dCurrDisY = COMI.Get_MotCurrentPos(m_nRobot_DisY);
			if( dCurrDisY > st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_DisY] )
			{
				m_nHardnessStep = 0;
			}
			else
			{
				m_nHardnessStep = 100;
			}
			break;			

		case 100:
			m_nHardnessStep = 1000;
			break;

		case 1000://Picker ask disponsor if picker could place device
			//dispensor가 요청하면 heatsink는 투입한다.
			if( COMI.Get_MotCurrentPos(M_DISPENSER_Y) > st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_DisY] )
			{
				m_nHardnessStep = 10;
			}
			else
			{

				nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF);
				if( nRet_2 != IO_ON || nRet_3 != IO_OFF)
				{
					if( nRet_2 != IO_ON ) strAlarm.Format("8%d%04d", IO_ON, st_io.i_HeatSink_Reverse_0_Check);
					else				  strAlarm.Format("8%d%04d", IO_OFF, st_io.i_HeatSink_Reverse_180_Check);						
					CTL_Lib.Alarm_Error_Occurrence(10720, dWARNING, strAlarm);
					nFunRet = RET_ERROR;
				}
				else
				{
					m_nHardnessStep = 1100;
				}
			}
			break;

		case 1100:
			m_nHardnessStep = 1110;
			break;

		case 1110:
			if( ( dCurrHeatSinkX > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + COMI.md_allow_value[M_HEATSINK_TRANSFER_X]) ) &&
				( dCurrHeatSinkY < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - COMI.md_allow_value[M_HEATSINK_TRANSFER_Y]) ) )
			{
				strAlarm.Format("%02d0008", m_nRobot_DisY );
				CTL_Lib.Alarm_Error_Occurrence(9701, dWARNING, strAlarm);
				m_nHardnessStep = 0;
				nFunRet = RET_ERROR;
				break;
			}


			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Color_Check, IO_ON);
			if( st_basic.n_mode_7387 == CTL_NO) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON)
			{
				m_nHardnessStep = 1200;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Color_Check);
				CTL_Lib.Alarm_Error_Occurrence(10722, dWARNING, strAlarm);
				m_nHardnessStep = 0;
				nFunRet = RET_ERROR;
			}
			break;

		case 1200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_GABAGE_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_nHardnessStep = 1300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_nHardnessStep = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(10713, dWARNING, alarm.mstr_code);
				m_nHardnessStep = 0;
				nFunRet = RET_ERROR;
			}
			break;

		case 1300:
			Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_ON);
			m_dwHardnessWaitTime[0] = GetCurrentTime();
			m_nHardnessStep = 1310;
			break;

		case 1310:
			m_dwHardnessWaitTime[1] = GetCurrentTime();
			m_dwHardnessWaitTime[2] = m_dwHardnessWaitTime[1] - m_dwHardnessWaitTime[0];
			if( m_dwHardnessWaitTime[2] <= 0 ) m_dwHardnessWaitTime[0] = GetCurrentTime();
			if( m_dwHardnessWaitTime[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_OFF);
				m_nHardnessStep = 2000;
			}
			break;

		case 2000:
			m_nHardnessOut++;
			if( m_nHardnessOut > 100 )
			{
				m_nHardnessStep = 3000;
			}
			else
			{
				m_nHardnessStep = 1300;
			}
			break;

		case 3000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_nHardnessStep = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_nHardnessStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(10718, dWARNING, alarm.mstr_code);
				m_nHardnessStep = 0;
				nFunRet = RET_ERROR;
			}
			break;

		case 3100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Color_Check, IO_ON);
			if( st_basic.n_mode_7387 == CTL_NO) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON)
			{
				m_nHardnessStep = 4000;
			}
			else
			{
// 				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Color_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(10719, dWARNING, strAlarm);
// 				m_nHardnessStep = 0;
// 				nFunRet = RET_ERROR;

			}
			m_nHardnessStep = 4000;
			break;


		case 4000:
			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_Off_Sol, IO_ON);
			m_nHardnessStep = 0;
			nFunRet = RET_GOOD;
			break;	
	}
	
	if( st_work.mn_run_status == dWARNING )
		nFunRet = RET_ERROR;
	
	return nFunRet;
}


int APartHandler::MoveEpoxyOut()
{
	int nRet_1 = 0, nRet_2 = 0;
	int nFuncRet = RET_PROCEED;
	int m_nRobotZ = M_EPOXY_TRANSFER_Z;
	int m_nRobotS = M_EPOXY_SCREW;
	
	switch(m_nEpoxyOutStep)
	{
	case 0:
		nRet_1 = COMI.Check_MotPosRange( M_EPOXY_TRANSFER_X, st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], 5 );
		nRet_2 = COMI.Check_MotPosRange( M_EPOXY_TRANSFER_Y, st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], 5 );

		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			st_msg.mstr_event_msg[0] = "EXPOXY 로봇이 Clean 위치가 아닙니다.";
			if( st_handler.cwnd_list != NULL )
			{
				sprintf( st_msg.c_abnormal_msg, "%s", st_msg.mstr_event_msg[0]);
				st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			if (st_handler.cwnd_main != NULL)
			{
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			}
			m_nEpoxyOutStep = 0;
			nFuncRet = RET_ERROR;
		}
		else
		{
			m_nEpoxyOutStep = 10;
		}
		break;

	case 10:
 		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobotZ, st_motor[m_nRobotZ].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			Func.VppmSet();
			m_nEpoxyOutStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_nEpoxyOutStep = 10;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(10930, dWARNING, alarm.mstr_code);
			Func.VppmOff();
			m_nEpoxyOutStep = 0;
			nFuncRet = RET_ERROR;
		}
		break;

	case 100:
		nRet_1 = COMI.Start_JogMove( m_nRobotS, PLUS );

		if( nRet_1 == CTL_YES )
		{
// 			COMI.Set_MotStop( 0 , m_nRobotS);
			m_dwEpoxyOutWaitTime[0] = GetCurrentTime();
			m_nEpoxyOutStep = 200;
		}
		else if( nRet_1 == BD_RETRY )
		{
			m_nEpoxyOutStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(10931, dWARNING, alarm.mstr_code);
			Func.VppmOff();
			m_nEpoxyOutStep = 0;
			nFuncRet = RET_ERROR;
		}
		break;
		
	case 200:
		//nRet_1 = CTL_YES;
		m_dwEpoxyOutWaitTime[1] = GetCurrentTime();
		m_dwEpoxyOutWaitTime[2] = m_dwEpoxyOutWaitTime[1] - m_dwEpoxyOutWaitTime[0];
		if( m_dwEpoxyOutWaitTime[2]  <= 0 )  m_dwEpoxyOutWaitTime[0] = GetCurrentTime();
		if( m_dwEpoxyOutWaitTime[2] < 20000 ) break;

		COMI.Set_MotStop( 0 , m_nRobotS);
		m_dwEpoxyOutWaitTime[0] = GetCurrentTime();
		m_nEpoxyOutStep = 210;
		break;

	case 210:
		m_dwEpoxyOutWaitTime[1] = GetCurrentTime();
		m_dwEpoxyOutWaitTime[2] = m_dwEpoxyOutWaitTime[1] - m_dwEpoxyOutWaitTime[0];
		if( m_dwEpoxyOutWaitTime[2]  <= 0 )  m_dwEpoxyOutWaitTime[0] = GetCurrentTime();
		if( m_dwEpoxyOutWaitTime[2] < 2000 ) break;
		m_nEpoxyOutStep = 220;
		break;

	case 220:
// 		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobotS, st_recipe.nEpoxyDotScrewCount -2 , st_recipe.nEpoxyRunSpeed);
		nRet_1 = COMI.Start_JogMove( m_nRobotS, MINUS );

		if( nRet_1 == CTL_YES )
		{
			COMI.Set_MotStop( 0 , m_nRobotS);
			Func.VppmOff();

			double dCurrPos = 0.0;// g_comiMgr.Get_MotCurrentPos(m_nRobotS);
		
			cmmStSetPosition(m_nRobotS, cmCNT_COMM, (double)0);
			cmmStSetPosition(m_nRobotS, cmCNT_FEED, (double)0);


			m_nEpoxyOutStep = 300;
			//2017.0616
			st_handler.m_nEpoxyCleanAlarm = CTL_NO;
		}
		else if( nRet_1 == BD_RETRY )
		{
			m_nEpoxyOutStep = 220;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(10932, dWARNING, alarm.mstr_code);
			Func.VppmOff();
			m_nEpoxyOutStep = 0;
			nFuncRet = RET_ERROR;
		}
		break;

	case 300:
		Func.VppmOff();
		m_nEpoxyOutStep = 0;
		nFuncRet = RET_GOOD;
	}

	if( st_work.mn_run_status == dWARNING )
		nFuncRet = RET_ERROR;
	
	return nFuncRet;
}


void APartHandler::AlarmHardnessExcessTimeEpoxyCnt()
{
	int nRet_1 = 0, nRet_2 = 0;
	int nFuncRet = RET_PROCEED;
	int m_nRobotZ = M_EPOXY_TRANSFER_Z;
	int m_nRobotS = M_EPOXY_SCREW;
	


	switch(m_nHardnessEpoxyStep)
	{

		case 0:
			nRet_1 = GetEpoxyCnt();
			if( nRet_1 > st_recipe.nEpoxyUseLimitCont )
			{
				m_nHardnessEpoxyStep = 100;
				break;
			}
			nRet_1 = GetHardnessTime();
			if( nRet_1 > st_basic.n_HardnessUseTime )
			{
				m_nHardnessEpoxyStep = 200;
			}
			break;

		case 100:
			st_msg.mstr_event_msg[0] = _T("Epoxy Cnt: Epoxy 사용회수 초과. 교체해 주세요." );					
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			if( st_handler.cwnd_list != NULL )
			{
				sprintf(st_msg.c_abnormal_msg,"%s",st_msg.mstr_event_msg[0]);
				st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			m_dwHardnessEpoxyWaitTime[0] = GetCurrentTime();
			m_nHardnessEpoxyStep = 110;
			break;

		case 110:
			m_dwHardnessEpoxyWaitTime[1] = GetCurrentTime();
			m_dwHardnessEpoxyWaitTime[2] = m_dwHardnessEpoxyWaitTime[1] - m_dwHardnessEpoxyWaitTime[0];
			if( m_dwHardnessEpoxyWaitTime[2] <= 0 ) m_dwHardnessEpoxyWaitTime[0] = GetCurrentTime();
			if( m_dwHardnessEpoxyWaitTime[2] > 60000 )
			{
				m_nHardnessEpoxyStep = 0;
			}
			else
			{
				nRet_1 = GetHardnessTime();
				if( nRet_1 > st_basic.n_HardnessUseTime )
				{
					m_nHardnessEpoxyStep = 200;
				}
			}
			break;

		case 200:
			st_msg.mstr_event_msg[0] = _T("경화제시간: 경화제사용시간 초과. 교체해 주세요." );					
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			if( st_handler.cwnd_list != NULL )
			{
				sprintf(st_msg.c_abnormal_msg,"%s",st_msg.mstr_event_msg[0]);
				st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			m_dwHardnessEpoxyWaitTime[0] = GetCurrentTime();
			m_nHardnessEpoxyStep = 210;
			break;

		case 210:
			m_dwHardnessEpoxyWaitTime[1] = GetCurrentTime();
			m_dwHardnessEpoxyWaitTime[2] = m_dwHardnessEpoxyWaitTime[1] - m_dwHardnessEpoxyWaitTime[0];
			if( m_dwHardnessEpoxyWaitTime[2] <= 0 ) m_dwHardnessEpoxyWaitTime[0] = GetCurrentTime();
			if( m_dwHardnessEpoxyWaitTime[2] > 60000 )
			{
				m_nHardnessEpoxyStep = 0;
			}
			else
			{
				nRet_1 = GetEpoxyCnt();
				if( nRet_1 > st_recipe.nEpoxyUseLimitCont )
				{
					m_nHardnessEpoxyStep = 100;
					break;
				}
			}
			break;
			
	}

}