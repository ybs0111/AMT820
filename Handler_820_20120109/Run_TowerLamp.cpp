// Run_TowerLamp.cpp: implementation of the CRun_TowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Run_TowerLamp.h"

#include "Variable.h"				// 전역 변수 정의 Class
#include "Public_Function.h"		// Motor 및 Cylinder 동작 Class
#include "FastechPublic_IO.h"

#include "SrcPart/APartDatabase.h"
#include "IO_Manager.h"
#include "ComizoaPublic.h"
#include "SrcPart/APartTray.h"
//#include "SrcPart/APartTestSite.h"
#include "AMTLotManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_TowerLamp Run_TowerLamp;
CRun_TowerLamp Run_TowerLamp_Manual;

CRun_TowerLamp::CRun_TowerLamp()
{
	MoveStep = 0;
	mn_lamp_step = 0;
	mn_pcb_exist_check = CTL_NO;
}

CRun_TowerLamp::~CRun_TowerLamp()
{

}

void CRun_TowerLamp::Thread_Run()
{
	Run_Move();
}

void CRun_TowerLamp::Run_Move()
{
	int Ret = 0;
	int i = 0, j = 0, k = 0;
	int x = 0, y = 0;

	// **************************************************************************
    // 현재 출력된 화면이 I/O 모니터링 화면인지 검사한다                         
	// -> I/O 디버깅 시 타워 램프는 계속 출력되어 I/O 상태 확인이 어려워 수정    
	// -> dSTOP(0)    dRUN(1)    dJAM(2)    dLOTEND(3)   dINIT(4)    dWARNING(5) 
	// -> dLOCK(6)    dSELFCHECK(7)												 
    // **************************************************************************
	if (st_handler.n_menu_num == 701)  return;
	// **************************************************************************

	if (COMI.mn_run_status == dJAM || COMI.mn_run_status == dWARNING)
	{
	   g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_ON);
	   g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
	}

	if (COMI.mn_run_status == dINIT)
	{
		switch(mn_lamp_step)
		{
		case 0 :
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 1 :
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			else if ((ml_lamp_time[1] - ml_lamp_time[0]) < 0)
			{
				ml_lamp_time[0] = GetCurrentTime();
			}
			// ************************************************************************
			break;
		case 2 :
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 3 :
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			else if ((ml_lamp_time[1] - ml_lamp_time[0]) < 0)
			{
				ml_lamp_time[0] = GetCurrentTime();
			}
			// ************************************************************************
			break;
		case 4:
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 5:
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			else if ((ml_lamp_time[1] - ml_lamp_time[0]) < 0)
			{
				ml_lamp_time[0] = GetCurrentTime();
			}
			// ************************************************************************
			break;
		case 6 :
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 7 :
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step = 0;
			else if ((ml_lamp_time[1] - ml_lamp_time[0]) < 0)
			{
				ml_lamp_time[0] = GetCurrentTime();
			}
			// ************************************************************************
			break;
		}
	}
	else
	{
		if (st_handler.mn_reinstatement == CTL_YES)	// 복구 동작이다.
		{
			switch(mn_lamp_step)
			{
			case 0 :
				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다													
				// ************************************************************************
				g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);	// GREEN 타워 램프 ON
				// ************************************************************************

				g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_ON);		// Start Button 램프 ON

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;

			case 1 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step++;
				else if ((ml_lamp_time[1] - ml_lamp_time[0]) < 0)
				{
					ml_lamp_time[0] = GetCurrentTime();
				}
				// ************************************************************************
				break;
			
			case 2 :
				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다											
				// ************************************************************************
				g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);	// GREEN 타워 램프 OFF
				// ************************************************************************

				g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);			// Start Button 램프 OFF

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;

			case 3 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step = 0;
				else if ((ml_lamp_time[1] - ml_lamp_time[0]) < 0)
				{
					ml_lamp_time[0] = GetCurrentTime();
				}
				// ************************************************************************
				break;
			default:
				mn_lamp_step = 0;
				break;
			}
		}
		else
		{
			if (COMI.mn_run_status != dRUN)
			{
				st_work.mn_tl_status = COMI.mn_run_status;
			}
			else
			{
				k = 0;

// 				for (i = BT_REAR_BUFFER; i < BT_CUSTOMER_BUFFER; i++)
// 				{
// 					for (x = 0; x < 3; x++)
// 					{
// 						for (y = 0; y < 100; y++)
// 						{
// 							if (st_buffer_tray[i].nDeviceExistence[0][x][y] == CTL_YES)
// 							{
// 								k++;
// 								break;
// 							}
// 						}
// 					}
// 				}

// 				for( i=0; i<MAX_TRAY; i++ )
// 				{
// 					ATray tray = g_Tray.GetTray( i );
// 					if(	tray.GetPos() == TPOS_REJECT_TO_BIN ||
// 						tray.GetPos() == TPOS_BIN_TO_CONV3 ||
// 						tray.GetTargetPos() == TPOS_NEXT )
// 					{
// 						k++;
// 					}
// 					if( tray.GetPos() == TPOS_REJECT ||
// 						tray.GetPos() == TPOS_BIN1 ||
// 						tray.GetPos() == TPOS_BIN2 ||
// 						tray.GetPos() == TPOS_BIN3)
// 					{
// 						if( g_Tray.GetLoadModuleCnt() > 0 )
// 						{
// 							k++;
// 						}
// 					}
// 					if(k > 0)	break;
// 				}
// 				for( i=0; i<TSITE_SOCKET_CNT; i++ )
// 				{
// 					if(g_site.GetModule(i).IsExist())
// 					{
// 						k++;
// 					}
// 					if(k > 0)	break;
// 				}
// 				
// 				if (k > 0)			// PCB Existence ml_lamp_change_time
// 				{
// 					mn_pcb_exist_check = CTL_NO;
// 					st_work.mn_tl_status = COMI.mn_run_status;
// 				}
// 				else
// 				{
// 					if (mn_pcb_exist_check == CTL_NO)
// 					{
// 						mn_pcb_exist_check = CTL_READY;
// 						ml_lamp_change_time[0] = GetCurrentTime();	
// 					}
// 					else if (mn_pcb_exist_check == CTL_READY)
// 					{
// 						ml_lamp_change_time[1] = GetCurrentTime();
// 						ml_lamp_change_time[2] = ml_lamp_change_time[1] - ml_lamp_change_time[0];
// 						
// 						if (ml_lamp_change_time[2] > st_lamp.mn_lamp_change_time * 1000 * 60)
// 						{
// 							mn_pcb_exist_check = CTL_YES;
// 						}//2012,1220
// 						else if(ml_lamp_change_time[2] < 0)
// 						{
// 							ml_lamp_change_time[0] = GetCurrentTime();
// 						}
// 					}
// 					
// 					if (mn_pcb_exist_check == CTL_YES)
// 					{
// 						st_work.mn_tl_status = dSELFCHECK;
// 					}
// 					else
// 					{
// 						st_work.mn_tl_status = COMI.mn_run_status;
// 					}					
// 					
// 					if( st_timedb.mnTime_Section != st_timedb.mnSectionBuffer )
// 					{
// 						st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
// 						st_timedb.mnSectionBuffer = st_work.mn_tl_status;
// 						st_timedb.mole_date		= COleDateTime::GetCurrentTime();
// 						
// 						g_db.DB_Write_Time(st_timedb);
// 					}
// 				}
			}

			if(g_lotMgr.GetLotIDAt(0).GetLength() < 5 && COMI.mn_run_status == dRUN)
			{
				st_work.mn_tl_status = dSELFCHECK;
			}
			else
			{
				st_work.mn_tl_status = COMI.mn_run_status;
			}
			
			
			switch(mn_lamp_step)
			{
			case 0 :
				// ************************************************************************
				//  RED 타워 램프 출력 제어한다												
				// ************************************************************************
				if (st_lamp.mn_lamp_r[st_work.mn_tl_status] == 1)       // RED 타워 램프 ON
					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
				else if (st_lamp.mn_lamp_r[st_work.mn_tl_status] == 0)  // RED 타워 램프 OFF
					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
				else if (st_lamp.mn_lamp_r[st_work.mn_tl_status] == 2)  // RED 타워 램프 FLICK
					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
				// ************************************************************************

				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다													
				// ************************************************************************
				if (st_lamp.mn_lamp_g[st_work.mn_tl_status] == 1)       // GREEN 타워 램프 ON
					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
				else if (st_lamp.mn_lamp_g[st_work.mn_tl_status] == 0)  // GREEN 타워 램프 OFF
					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
				else if (st_lamp.mn_lamp_g[st_work.mn_tl_status] == 2)  // GREEN 타워 램프 FLICK
					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
				// ************************************************************************

				// ************************************************************************
				//  YELLOW 타워 램프 출력 제어한다											
				// ************************************************************************
				if (st_lamp.mn_lamp_y[st_work.mn_tl_status] == 1)       // YELLOW 타워 램프 ON
					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
				else if (st_lamp.mn_lamp_y[st_work.mn_tl_status] == 0)  // YELLOW 타워 램프 OFF
					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
				else if (st_lamp.mn_lamp_y[st_work.mn_tl_status] == 2)  // YELLOW 타워 램프 FLICK
					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
				// ************************************************************************

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;
			case 1 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step++;
				else if(ml_lamp_time[2] < 0)
				{
					ml_lamp_time[0] = GetCurrentTime();
				}
				// ************************************************************************
				break;
			case 2 :
				// ************************************************************************
				//  RED 타워 램프 출력 제어한다												
				// ************************************************************************
				if (st_lamp.mn_lamp_r[st_work.mn_tl_status] == 1)       // RED 타워 램프 ON
					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
				else if (st_lamp.mn_lamp_r[st_work.mn_tl_status] == 0)  // RED 타워 램프 OFF
					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
				else if (st_lamp.mn_lamp_r[st_work.mn_tl_status] == 2)  // RED 타워 램프 FLICK
					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다											
				// ************************************************************************
				if (st_lamp.mn_lamp_g[st_work.mn_tl_status] == 1)       // GREEN 타워 램프 ON
					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
				else if (st_lamp.mn_lamp_g[st_work.mn_tl_status] == 0)  // GREEN 타워 램프 OFF
					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
				else if (st_lamp.mn_lamp_g[st_work.mn_tl_status] == 2)  // GREEN 타워 램프 FLICK
					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				//  YELLOW 타워 램프 출력 제어한다											
				// ************************************************************************
				if (st_lamp.mn_lamp_y[st_work.mn_tl_status] == 1)       // YELLOW 타워 램프 ON
					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
				else if (st_lamp.mn_lamp_y[st_work.mn_tl_status] == 0)  // YELLOW 타워 램프 OFF
					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
				else if (st_lamp.mn_lamp_y[st_work.mn_tl_status] == 2)  // YELLOW 타워 램프 FLICK
					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;
			case 3 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step = 0;
				else if(ml_lamp_time[2] < 0)
				{
					ml_lamp_time[0] = GetCurrentTime();
				}
				// ************************************************************************
				break;
			default:
				mn_lamp_step = 0;
				break;
			}

			// Original
// 			switch(mn_lamp_step)
// 			{
// 			case 0 :
// 				// ************************************************************************
// 				//  RED 타워 램프 출력 제어한다												
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED 타워 램프 ON
// 					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED 타워 램프 OFF
// 					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED 타워 램프 FLICK
// 					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  GREEN 타워 램프 출력 제어한다													
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN 타워 램프 ON
// 					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN 타워 램프 OFF
// 					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN 타워 램프 FLICK
// 					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  YELLOW 타워 램프 출력 제어한다											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW 타워 램프 ON
// 					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW 타워 램프 OFF
// 					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW 타워 램프 FLICK
// 					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
// 				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
// 				// ************************************************************************
// 				ml_lamp_time[0] = GetCurrentTime();
// 				mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 1 :
// 				// ************************************************************************
// 				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
// 				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
// 				// ************************************************************************
// 				ml_lamp_time[1] = GetCurrentTime();
// 				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
// 					mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 2 :
// 				// ************************************************************************
// 				//  RED 타워 램프 출력 제어한다												
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED 타워 램프 ON
// 					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_ON);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED 타워 램프 OFF
// 					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED 타워 램프 FLICK
// 					g_ioMgr.set_out_bit(st_io.o_Tower_RedLamp, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  GREEN 타워 램프 출력 제어한다											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN 타워 램프 ON
// 					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_ON);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN 타워 램프 OFF
// 					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN 타워 램프 FLICK
// 					g_ioMgr.set_out_bit(st_io.o_Tower_GreenLamp, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  YELLOW 타워 램프 출력 제어한다											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW 타워 램프 ON
// 					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_ON);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW 타워 램프 OFF
// 					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW 타워 램프 FLICK
// 					g_ioMgr.set_out_bit(st_io.o_Tower_YellowLamp, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
// 				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
// 				// ************************************************************************
// 				ml_lamp_time[0] = GetCurrentTime();
// 				mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 3 :
// 				// ************************************************************************
// 				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
// 				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
// 				// ************************************************************************
// 				ml_lamp_time[1] = GetCurrentTime();
// 				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
// 					mn_lamp_step = 0;
// 				// ************************************************************************
// 				break;
// 			default:
// 				mn_lamp_step = 0;
// 				break;
// 			}
		}
	}
}

void CRun_TowerLamp::Run_FormChange()
{
	switch (mn_form_step)
	{
	case 100:
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 9, 1);  // IO
		ml_form_time[0] = GetCurrentTime();
		mn_form_step = 200;
		break;

	case 200:
		ml_form_time[1] = GetCurrentTime();
		ml_form_time[2] = ml_form_time[1] - ml_form_time[0];

		if (ml_form_time[2] > 100)
		{
			mn_form_step = 300;
		}
		else if(ml_form_time[2] < 0)
		{
			ml_form_time[0] = GetCurrentTime();
		}
		break;

	case 300:
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 1);  // XYZ
		ml_form_time[0] = GetCurrentTime();
		mn_form_step = 400;
		break;

	case 400:
		ml_form_time[1] = GetCurrentTime();
		ml_form_time[2] = ml_form_time[1] - ml_form_time[0];
		
		if (ml_form_time[2] > 100)
		{
			mn_form_step = 100;
		}
		else if(ml_form_time[2] < 0)
		{
			ml_form_time[0] = GetCurrentTime();
		}
		break;
	}
}
