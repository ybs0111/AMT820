// Run_Handler_Check.cpp: implementation of the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Handler_Check.h"
#include "Variable.h"				// ���� ���� ���� Class
#include "Public_Function.h"		// Motor �� Cylinder ���� Class
#include "FastechPublic_IO.h"			// Cylinder ���� Class
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
	n_emo_chk = FALSE;
	n_air_chk = FALSE;
	mn_autostep = 0;
	mn_manualstep = 0;
}

CRun_Handler_Check::~CRun_Handler_Check()
{

}

void CRun_Handler_Check::ButtonCheck_Start()
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return; // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// ��� â�� �� ������ �������� �ʴ´�.

	switch (StartStep)
	{
	case 0:
		// **********************************************************************
		// RUN Ű I/O �Է��� On �����̰� ���°� STOP ���¿����� �˻�             
		// -> �ٽ� ����, STOP ���¿����� RUN Ű �Է��� �����Ѵ�                  
		// **********************************************************************
		if (g_ioMgr.get_in_bit(st_io.i_Start_SwitchCheck, IO_ON) == IO_ON && st_work.mn_run_status == dSTOP)
		{
			// ���� Stop Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
			if (g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_ON) == IO_ON)
			{
				break;
			}

			StartStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START Ű�� ���������� On/Off �Ǵ� ���� �����ϱ� ���Ͽ� ��� ��        
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		StartStep = 200;
		break;

	case 200: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

		if(SwitchWaitTime[2] > 50 && g_ioMgr.get_in_bit(st_io.i_Start_SwitchCheck, IO_OFF) == IO_OFF )
		{
			SwitchWaitTime[0] = GetCurrentTime();
			StartStep = 300;
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		else if (g_ioMgr.get_in_bit(st_io.i_Start_SwitchCheck, IO_OFF) == IO_OFF)
		{
			StartStep = 0;
		}
		break;

	case 300:
		// **********************************************************************
		// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
		// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
		// **********************************************************************
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.get_in_bit(st_io.i_Start_SwitchCheck, IO_OFF) == IO_OFF)
			{
				StartStep = 600;
			}
			else
			{
				StartStep = 0;
			}
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		// **********************************************************************
		break;

	case 600:
		// **********************************************************************
		//  RUN�Ҽ� �ִ� �������� �˻��Ѵ�. �̻��� ����� RUN�� �ȴ�.            
		// **********************************************************************
		if (st_handler.mn_motor_init_check != CTL_YES)	// ���� �ʱ�ȭ�� ���� �ʾҴٸ� �������� �ʴ´�. 2K4/11/23/ViboX
		{
			Func.OnSet_IO_Port_Sound(IO_ON);
			sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial..., become Run.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			//900001 1 A "INITIALIZE_ERROR."
// 			alarm.mstr_code = _T("900001");		// Alarm ȭ���� ����ش�. 2K4/11/21/ViboX
// 			sprintf(alarm.mc_code, "900001");
// 			alarm.mn_count_mode = 0;				// �˶� ī��Ʈ ���� �÷��� ���� (�˶� ī��Ʈ �۾� ������)
// 			alarm.mn_type_mode = eWARNING;		// ���� �߻��� �˶� ���� �÷��� ����
// 			COMI.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm ȭ���� ����ش�. 2K4/11/21/ViboX
			StartStep = 0;
			CTL_Lib.Alarm_Error_Occurrence( 10, dWARNING, "900001");

			return;
		}

// 		// JAM�� ���� �Ǿ����� Ȯ���ϰ� RUN�� ��Ų��.
		if (alarm.mn_reset_status != CTL_NO)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first Jam Reset.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			StartStep = 0;
			break;
		}

		// Door�� ���������� RUN�� ���� �ʴ´�.
		if (Func.DoorOpenCheckSpot() != CTLBD_RET_GOOD)	
		{
			Func.OnSet_IO_Port_Sound(IO_ON);

			sprintf(st_msg.c_abnormal_msg, "[DOOR CHECK] Door Open being done Run become.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			
			st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm ȭ���� ����ش�. 2K4/11/21/ViboX

			StartStep = 0;
			break;
		}
	
		// Air ���� ������ Ȯ���Ѵ�.
		if (g_ioMgr.get_in_bit( st_io.i_Main_Air1_Check, IO_OFF) == IO_ON || g_ioMgr.get_in_bit( st_io.i_Main_Air2_Check, IO_OFF) == IO_ON)
		{
			Func.OnSet_IO_Port_Sound(IO_ON);
//			st_msg.mstr_abnormal_msg = _T("[AIR CHECK] Though supply Air, become Run.");
			sprintf(st_msg.c_abnormal_msg, "[AIR CHECK] Though supply Air, become Run.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			// 			810514 0 A "MAIN_AIR_1_OFF_CHECK_ERROR."
			// 			810515 0 A "MAIN_AIR_1_OFF_CHECK_ERROR."
		
			if (g_ioMgr.get_in_bit( st_io.i_Main_Air1_Check, IO_OFF) == IO_OFF ) CTL_Lib.Alarm_Error_Occurrence(700, CTL_dWARNING, "810514");
			else																 CTL_Lib.Alarm_Error_Occurrence(700, CTL_dWARNING, "810515");	
			StartStep = 0;
			return;
		}		

		//ybs
		if (g_ioMgr.get_in_bit(st_io.i_MC2_Check, IO_ON ) == IO_OFF || g_ioMgr.get_in_bit(st_io.i_MC3_Check, IO_ON) == IO_OFF)
		{
			st_handler.mstrSelectMessage = "Check MC2 or MC3. Motor,IO Power... Reset Button Check";
			st_handler.mnSelectMessage = 0;
			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
			
			StartStep = 610;
			break;
		}
		

		StartStep = 700;
		break;

	case 610:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);
			StartStep = 700;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);
			StartStep = 0;
		}
		break;

	case 620:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);
			StartStep = 0;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);
			StartStep = 0;
		}
		break;

	case 700:
		if(COMI.mn_run_status != dSTOP) break;

// 		Ret = Func.Robot_Reinstatement_Position(1);		// ���� ���� Ȯ�� �ؾ� ��.
// 
// 		if (Ret == RET_GOOD)
// 		{	//����Ϸ�
// 
// 		}
// 		else
// 		{
// 			if(COMI.mn_run_status != dSTOP)
// 			{
// 				StartStep = 0;
// 			}
// 			break;
// 		}

		//2013,0124
// 		st_work.md_reinstatement_pos[0] = CTL_NO;
// 		st_work.md_reinstatement_pos[1] = CTL_NO;
// 		st_work.md_reinstatement_pos[2] = CTL_NO;

		if(st_handler.cwnd_title != NULL)
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);
		}


// 		if(st_handler.mn_virtual_supply)
// 		{
//  			st_work.mn_reinstate_stacker_robot[0] = CTL_YES;
//  			st_work.mn_reinstate_stacker_robot[1] = CTL_YES;
// 			st_work.mn_reinstate_stacker_robot[2] = CTL_YES;
// 		}
// 		if(st_basic.n_mode_recovery == CTL_NO)
// 		{
// 			st_work.mn_reinstate_xyz_robot = CTL_YES;
// 			st_work.mn_reinstate_reject_robot = CTL_YES;
// 			st_work.mn_reinstate_tester_robot[0] = CTL_YES;
// 			st_work.mn_reinstate_tester_robot[1] = CTL_YES;
// 			st_work.mn_reinstate_stacker_robot[0] = CTL_YES;
// 			st_work.mn_reinstate_stacker_robot[1] = CTL_YES;
// 			st_work.mn_reinstate_stacker_robot[2] = CTL_YES;
// 		}
// 		else
// 		{
// 			st_work.mn_run_status					= dREINSTATE;
// 		}
		StartStep = 800;
		break;

	case 800:
// 		if(st_work.mn_reinstate_xyz_robot == CTL_YES && st_work.mn_reinstate_reject_robot == CTL_YES &&
// 			st_work.mn_reinstate_tester_robot[0] == CTL_YES && st_work.mn_reinstate_tester_robot[1] == CTL_YES && 
// 			st_work.mn_reinstate_stacker_robot[0] == CTL_YES &&	st_work.mn_reinstate_stacker_robot[1] == CTL_YES && 
// 			st_work.mn_reinstate_stacker_robot[2] == CTL_YES)
// 		{
			StartStep = 900;
// 		}
// 		else
// 		{
// 			if(COMI.mn_run_status != dREINSTATE)	//���� ������ ������, ���е� �ƴѵ� ���͵��� ���°� �ƴ϶��..
// 			{
// 				COMI.mn_run_status = dSTOP;
// 				if(st_handler.cwnd_title != NULL)
// 				{
// 					st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, COMI.mn_run_status);
// 				}
// 				StartStep = 0;
// 			}
// 		}
		break;

	case 900:
		// **********************************************************************
		// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
		// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
		// **********************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// ���� ���������� �޽��� �����Ͽ� �˶� ȭ�� ���� ��û 
	
		if (st_handler.mn_menu_num != 101)
		{
			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// ���� ���������� �޽��� �����Ͽ� ���� ȭ������ ��ȯ 
		}

		StartStep = 1000;
		break;

	case 1000:
		Func.OnSet_IO_Port_Run();									// ��� ���� : ���� ������ ��� I/O ��� �������� �Լ�
		alarm.mn_emo_alarm = FALSE;
		n_emo_chk = FALSE;
		n_air_chk = FALSE;
		StartStep = 0;
		break;
	
	default :
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_Stop()
{
	CString str_tmp;

	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	switch (StopStep)
	{
	case 0:
		// **********************************************************************
		// STOP Ű I/O �Է��� On �����̰� ���� STOP ���°� �ƴ��� �˻�           
		// **********************************************************************
		if (g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_ON) == IO_ON && st_work.mn_run_status != dSTOP)
		{
			// ���� Start Button�� ���� ���ȴٸ� ���۵��� �ʴ´�.
			if (g_ioMgr.Get_In_Bit(st_io.i_Start_SwitchCheck) == TRUE)
			{
				break;
			}

			StartStep = 0;
			StopStep = 100;
//			StopStep = 600;
		}
		else if (g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_ON) == IO_ON)
		{
			StopStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START Ű�� ���������� On/Off �Ǵ� ���� �����ϱ� ���Ͽ� ��� ��        
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		StopStep = 200;
		break;
		
	case 200: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		if(SwitchWaitTime[2] <= 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			break;
		}
		
		if(SwitchWaitTime[2] > 50 && g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_OFF) == IO_OFF)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			StopStep = 300;
		}//2012,1220
		else if (g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_OFF) == IO_OFF)
		{
			StopStep = 0;
		}
		break;
		
	case 300:
		// **********************************************************************
		// RUN Ű I/O �Է��� Off �Ǿ����� �˻�                                   
		// -> �˶� ȭ�� ��� �����̸� �˶� ȭ�� ���� -> ���� ȭ������ ��ȯ       
		// **********************************************************************
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_OFF) == IO_OFF)
			{
				StopStep = 600;
			}
			else
			{
				StopStep = 0;
			}
		}//2012,1220
		else if(SwitchWaitTime[2] < 0)
		{
			SwitchWaitTime[0] = GetCurrentTime();
		}
		// **********************************************************************
		break;

	case 600:
		// **********************************************************************
		// STOP Ű I/O �Է��� Off �Ǿ����� �˻�                                  
		// **********************************************************************
		if (g_ioMgr.get_in_bit(st_io.i_Stop_SwitchCheck, IO_OFF) == IO_OFF)
		{
			StopStep = 0; 

			if (st_work.mn_run_status == dINIT)				// �ʱ�ȭ �߿� Stop ��ư�� ������... 2K5/02/23/ViboX
			{//900001 1 A "INITIALIZE_ERROR."
				st_handler.mn_initial_error = TRUE;				
// 				alarm.mstr_code = _T("900001");		// Alarm ȭ���� ����ش�. 2K5/03/10/ViboX
// 				sprintf(alarm.mc_code, "900001");
// 				alarm.mn_count_mode = 0;				// �˶� ī��Ʈ ���� �÷��� ���� (�˶� ī��Ʈ �۾� ������)
// 				alarm.mn_type_mode = eWARNING;		// ���� �߻��� �˶� ���� �÷��� ����
// 				COMI.mn_run_status = dWARNING;	// dJAM,dWARNING 
// 				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 0, dWARNING, "900001");
				break;
			}

			if (st_handler.mn_menu_num != 101)
			{
				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // ���� ���������� �޽��� �����Ͽ� ���� ȭ������ ��ȯ 
			}			
			Func.OnSet_IO_Port_Stop();
		}
		break;

	default : 
		break;
	}
}	

void CRun_Handler_Check::ButtonCheck_Reset()
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************

	switch(ResetStep)
	{
		case 0:
			// **********************************************************************
			//  Reset Ű I/O �Է��� On �������� Ȯ��                                 
			// **********************************************************************
//			if(g_ioMgr.Get_In_Bit(st_io.i_reset_chk) == IO_ON)
			if(g_ioMgr.get_in_bit(st_io.i_AlarmClear_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[0] = GetCurrentTime();
				ResetStep = 100;
			}
			break;

		case 100: 
//			if(g_ioMgr.Get_In_Bit(st_io.i_reset_chk) == IO_ON)
			if(g_ioMgr.get_in_bit(st_io.i_AlarmClear_SwitchCheck, IO_ON) == IO_ON)
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
			// Reset Ű I/O �Է��� Off �Ǿ����� �˻�                                 
			// **********************************************************************
//			if(g_ioMgr.Get_In_Bit(st_io.i_reset_chk) == IO_OFF)
			if(g_ioMgr.get_in_bit(st_io.i_AlarmClear_SwitchCheck, IO_OFF) == IO_OFF)
			{
				if (alarm.mn_reset_status == YES)
				{
					alarm.mn_reset_status = NO;
					st_other.str_normal_msg = _T("[JAM RESET] JAM RESET.");
					sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
					if (st_handler.cwnd_list != NULL)
					{
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}
//					g_ioMgr.Set_Out_Bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
					if(st_work.mn_run_status != dRUN)
					{
						Func.OnSet_IO_Port_Stop();
					}
					Func.OnSet_IO_Port_Sound(IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
				}
				else
				{
					st_other.str_normal_msg = _T("[SOUND] OFF.");
					sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
					if (st_handler.cwnd_list != NULL)
					{
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					}
					Func.OnSet_IO_Port_Sound(IO_OFF);
					g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
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
	// 20100805 tae ���� 
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.n_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************
	
	switch(SoundStep)
	{
		case 0:
			// **********************************************************************
			//  Reset Ű I/O �Է��� On �������� Ȯ��                                 
			// **********************************************************************
//			if(g_ioMgr.Get_In_Bit(st_io.i_buzzer_chk) == IO_ON)
			if(g_ioMgr.get_in_bit(st_io.i_Buzzer_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[0] = GetCurrentTime();
				SoundStep = 100;
			}
			break;
			
		case 100: 
//			if(g_ioMgr.Get_In_Bit(st_io.i_buzzer_chk) == IO_ON)
			if(g_ioMgr.get_in_bit(st_io.i_Buzzer_SwitchCheck, IO_ON) == IO_ON)
			{
				SwitchWaitTime[1] = GetCurrentTime();
				SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

				if(SwitchWaitTime[2] < 0)
				{
					SwitchWaitTime[0] = GetCurrentTime();
				}
				else if(SwitchWaitTime[2] > 100)
				{
					SwitchWaitTime[0] = GetCurrentTime();
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
			// Reset Ű I/O �Է��� Off �Ǿ����� �˻�                                 
			// **********************************************************************
//			if(g_ioMgr.Get_In_Bit(st_io.i_buzzer_chk) == IO_OFF)
			if(g_ioMgr.get_in_bit(st_io.i_Buzzer_SwitchCheck, IO_OFF) == IO_OFF)
			{
				SoundStep = 300;
			}
			else
			{
				SwitchWaitTime[1] = GetCurrentTime();
				SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
				
				if(SwitchWaitTime[2] > 10000)
				{
					SoundStep = 0;
				}
			}
			break;

		case 300:
			// **********************************************************************
			// Reset Ű I/O �Է��� Off �Ǿ����� �˻�                                 
			// **********************************************************************
			if (FAS_IO.Get_Out_Bit(st_io.o_Buzzer_SwitchLamp) == IO_ON)
			{
				Func.OnSet_IO_Port_Sound(IO_OFF);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer_SwitchLamp, IO_OFF);
			}
			
			SoundStep = 0;
			break;
			
		default : 
			break;
	}
}

int CRun_Handler_Check::Run_Possible_Check()
{
	// ========================================================================
	//  RUN�Ҽ� �ִ� �������� �˻��Ѵ�. �̻��� ����� RUN�� �ȴ�.            
	// ========================================================================
	if(st_handler.mn_motor_init_check != YES){		//���� �ʱ�ȭ�� �ȵ� ���¶��..
	
		st_other.str_abnormal_msg = _T("[MOTOR INIT CHECK] Motor Initial..., become Run.");
		sprintf(st_msg.c_abnormal_msg, st_other.str_abnormal_msg);
		if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

		return FALSE;
	}

	if(alarm.mn_reset_status != NO){					// JAM�� ������ �ȵ� ���¶��..
	
		st_other.str_abnormal_msg = _T("[JAM CONFIRM] If do to do Run, do first Jam Reset.");
		sprintf(st_msg.c_abnormal_msg, st_other.str_abnormal_msg);
		if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

		return FALSE;
	}

	if(Func.DoorOpenCheck() == RET_ERROR){
		st_other.str_abnormal_msg = _T("[DOOR CHECK] Door Open being done Run become.");
		sprintf(st_msg.c_abnormal_msg, st_other.str_abnormal_msg);
		if(st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		
		alarm.mn_count_mode = 0;
		alarm.n_alarm_assign_section = 100;
		alarm.mn_type_mode = eWARNING;
		st_work.mn_run_status = dWARNING;
		return FALSE;
	}
	//TRAY CHECK
		
	if(g_ioMgr.get_in_bit(st_io.i_HeatSink_Box1_OnOff_Check, IO_ON) == IO_ON)
	{
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box1_Guide_PIN_Forward_Sol, IO_ON);
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box1_Guide_PIN_Backward_Sol, IO_OFF);	
	}
	else
	{
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box1_Guide_PIN_Forward_Sol, IO_OFF);
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box1_Guide_PIN_Backward_Sol, IO_ON);
	
		alarm.mstr_code.Format("8%d%04d", IO_OFF, st_io.i_HeatSink_Box1_OnOff_Check);
		alarm.mn_count_mode = 0;
		alarm.n_alarm_assign_section = 101;	
		alarm.mn_type_mode = eWARNING;
		st_work.mn_run_status = dWARNING;
	}
	
	if(g_ioMgr.get_in_bit(st_io.i_HeatSink_Box2_OnOff_Check, IO_ON) == IO_ON )
	{
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box2_Guide_PIN_Forward_Sol, IO_ON);
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box2_Guide_PIN_Backward_Sol, IO_OFF);
	}
	else
	{
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box2_Guide_PIN_Forward_Sol, IO_OFF);
		FAS_IO.Set_Out_Bit(st_io.o_HeatSink_Box2_Guide_PIN_Backward_Sol, IO_ON);
		alarm.mstr_code.Format("8%d%04d", IO_OFF, st_io.i_HeatSink_Box2_OnOff_Check);
		alarm.mn_count_mode = 0;
		alarm.n_alarm_assign_section = 102;	
		alarm.mn_type_mode = eWARNING;
		st_work.mn_run_status = dWARNING;
	}
	return TRUE;
}

void CRun_Handler_Check::HandlerCheck_Safety()
{
	BOOL bPoWerOff = true;
	if(g_ioMgr.get_in_bit(st_io.i_MC2_Check, IO_ON ) == IO_OFF || g_ioMgr.get_in_bit(st_io.i_MC2_Check, IO_ON) == IO_OFF)
	{
		bPoWerOff = false;
	}
	
	if(bPoWerOff == false)
	{
		if(st_handler.m_bAlarmMcPowerOff == IO_OFF)
		{
			m_dwSafetyWaitTime[0] = GetCurrentTime();
			st_handler.m_bAlarmMcPowerOff = IO_ON;
		}
		else
		{
			m_dwSafetyWaitTime[1] = GetCurrentTime();
			m_dwSafetyWaitTime[2] = m_dwSafetyWaitTime[1] - m_dwSafetyWaitTime[0];
			if( m_dwSafetyWaitTime[2] <= 0) m_dwSafetyWaitTime[0] = GetCurrentTime();
			if( m_dwSafetyWaitTime[2] > 100)
			{
				st_handler.m_bAlarmMcPowerOff = IO_OFF;
				// 			810514 0 A "MAIN_AIR_1_OFF_CHECK_ERROR."
				// 			810515 0 A "MAIN_AIR_1_OFF_CHECK_ERROR."
				if (g_ioMgr.get_in_bit( st_io.i_Main_Air1_Check, IO_OFF) == IO_OFF ) 
					CTL_Lib.Alarm_Error_Occurrence(700, CTL_dWARNING, "810514");
				else																 
					CTL_Lib.Alarm_Error_Occurrence(700, CTL_dWARNING, "810515");	

			} 
		}
		
	}
	else
	{
		st_handler.m_bAlarmMcPowerOff = IO_OFF;
	}
	
	//////MutingCheck
	if(st_basic.n_Light_Curtain_Mode == 0)
	{
		Func.Set_Light_Curtain_Muting_On();	//Light Curtain Muting On
		Func.Set_Light_Curtain_Muting_Off();	//Light Curtain Muting On
// 		PublicFunction.Set_Light_Curtain_Muting_On_HeatSink();
// 		PublicFunction.Set_Light_Curtain_Muting_Off_HeatSink();
	}
	
	
	//////20130904������	
	// 		if(FAS_IO.Get_In_Bit(st_io.i_MC2_Check) == IO_OFF || FAS_IO.Get_In_Bit(st_io.i_MC2_Check) == IO_OFF)
	// 		{
	// 			PublicFunction.OnSet_IO_Port_Stop();
	// 		}
	///////
	
	if (st_handler.mn_motor_init_check == TRUE)
	{
		Run_Handler_Check.HsInterfaceChk();			//20130723
	}    
}



void CRun_Handler_Check::HsInterfaceChk()
{
	//================================
	// �ڽ��� ���¸� Upper�� �˷��ش�.
	//================================
// 	char Jamcode[10] ;
	
	CTime t_CurTime	=	CTime::GetCurrentTime();
	
	
	//INTERLOCK
	if(st_basic.n_mode_case_assembly == 0)
	{
		Func.Case_Assembly_Able_Check();
		Func.Case_Assembly_Place_Check();
	}
	else
	{
		st_handler.n_CaseAssemblyUnloadingAbleOn = 1;
		st_handler.n_HSLowerPlaceAbleCheck = 1;
	}
	
// 	if(st_work.nHSLowerAble == 1 ||	st_work.nDeviceUnPressMoveThreadRunMode == 1 ||
// 		(COMI.Get_MotCurrentPos(M_CARRIER_X) < st_motor[M_CARRIER_X].d_pos[P_CARRIER_X_PUSH_POS] - 5)
// 		|| ((COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X) > 800)
// 		&& (COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X) < 400))
// 		&& st_inspect.n_CaseAssemblyUnloadingAbleOn == ON
// 		&& FAS_IO.Get_In_Bit(st_io.i_Interface_Input_1) == IO_ON )
// 	{
// 		/////������ �̵� �Ұ��� ....
// 		PublicFunction.HS_Able_Signal(OFF);
// 	}
// 	else if(st_work.nDeviceUnPressMoveThreadRunMode == 0 || 
// 		(COMI.Get_MotCurrentPos(M_CARRIER_X) > st_motor[M_CARRIER_X].d_pos[P_CARRIER_X_PUSH_POS] - 5)
// 		&& ((COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X) < 800)
// 		&& (COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X) > 400)))
// 	{
// 		/////������ �̵� ���� ....
// 		if(((COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_Y) < st_motor[M_UNLOADER_TRANSFER_Y].d_pos[P_UNLOADER_TRANSFER_Y_PLACE_POS] + 5)
// 			&& (COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_Y) > st_motor[M_UNLOADER_TRANSFER_Y].d_pos[P_UNLOADER_TRANSFER_Y_PLACE_POS] - 5)
// 			&& (COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X) < st_motor[M_UNLOADER_TRANSFER_X].d_pos[P_UNLOADER_TRANSFER_X_PLACE_POS] + 5)))
// 		{
// 			PublicFunction.HS_Able_Signal(OFF);
// 		}
// 		else
// 			PublicFunction.HS_Able_Signal(ON);
// 	}
	
// 	if(st_inspect.n_CaseAssemblyUnloadingAbleOn == 0
// 		&& st_inspect.n_HSAbleOn == 0
// 		&& st_work.n_run_status == dRUN
// 		&& GetTickCount() > st_inspect.nInterLockTimeCheck + 1000 * 60 * 5)
// 	{
// 		st_inspect.nInterLockTimeCheck = GetTickCount();
// 		sprintf(Jamcode, "900013");
// 		
// 		alarm.str_code = _T(Jamcode);
// 		alarm.n_count_mode = 3002;
// 		alarm.n_type_mode = CTL_dWARNING;
// 		st_work.n_run_status = CTL_dWARNING;
// 		CtlBdFunc.Alarm_Error_Occurrence(alarm.n_count_mode, alarm.n_type_mode, st_work.n_run_status, alarm.str_code);
// 		
// 	}
	
}


void CRun_Handler_Check::ButtonCheck_Auto()
{
	int nRet;
	if(st_work.mn_run_status == dRUN)			return;
	if(st_handler.n_system_lock != FALSE)			return; // ���� �ý��� Lock ������ ��� ���� 
	if(st_work.mn_machine_mode	== MACHINE_AUTO)	return;

	switch(mn_autostep)
	{
		case 0:
			if(g_ioMgr.get_in_bit(st_io.i_Auto_SwitchCheck, IO_ON) == IO_ON)
			{
				m_dwAutoWaitTime[0] = GetTickCount();
				mn_autostep = 100;
			}
			break;

		case 100:
			if(g_ioMgr.get_in_bit(st_io.i_Auto_SwitchCheck, IO_ON) == IO_ON)
			{
				m_dwAutoWaitTime[1] = GetTickCount();
				m_dwAutoWaitTime[2] = m_dwAutoWaitTime[1] - m_dwAutoWaitTime[0];

				if(m_dwAutoWaitTime[2] > 2000)
				{
					mn_autostep = 200;
				}
			}
			else
			{
				mn_autostep = 0;
			}
			break;

		case 200:
			nRet = Func.DoorOpenCheckSpot();

			if(nRet == CTLBD_RET_GOOD)
			{
				Func.OnSet_Door_Close();
				st_work.mn_machine_mode = MACHINE_AUTO;
				mn_autostep = 0;

			}
			else if(nRet != CTLBD_RET_PROCEED)
			{
				alarm.n_alarm_assign_section = 3003; 
				alarm.mn_type_mode = CTL_dWARNING; 
				st_work.mn_run_status = CTL_dWARNING;
				mn_autostep = 300;
			}
			break;

		case 300:
			nRet = Func.DoorOpenCheckSpot();

			if(nRet == CTLBD_RET_GOOD)
			{
				mn_autostep = 0;
			}
			break;

	}
}

void CRun_Handler_Check::ButtonCheck_Manual()
{
//	if(st_work.n_run_status		== dRUN)			return;
	if(st_handler.n_system_lock != FALSE)			return; // ���� �ý��� Lock ������ ��� ���� 
	if(st_work.mn_machine_mode	== MACHINE_MANUAL)	return;
	
	switch(mn_manualstep)
	{
		case 0:
			if(g_ioMgr.get_in_bit(st_io.i_Manual_SwitchCheck, IO_ON) == IO_ON)
			{
				m_dwManualWaitTime[0] = GetTickCount();
				mn_manualstep = 100;
			}
			break;
			
		case 100:
			if(g_ioMgr.get_in_bit(st_io.i_Manual_SwitchCheck, IO_ON) == IO_ON)
			{
				m_dwManualWaitTime[1] = GetTickCount();
				m_dwManualWaitTime[2] = m_dwManualWaitTime[1] - m_dwManualWaitTime[0];
				
				if(m_dwManualWaitTime[2] > 200)
				{
					mn_manualstep = 200;
				}
			}
			else
			{

				mn_manualstep = 0;
			}
			break;
			
		case 200:
			Func.OnSet_Door_Open();
			st_work.mn_machine_mode = MACHINE_MANUAL;
			Func.OnSet_IO_Port_Stop();
			mn_manualstep = 0;
			break;
	}
}

void CRun_Handler_Check::StackerMutingCheck()
{
	// ========================================================================
	//  RUN �������� �˻��Ѵ�.           
	// ========================================================================
	
	//	if(st_work.n_run_status != dRUN && st_work.n_run_status != dINIT) return;
	
	if(g_ioMgr.get_in_bit(st_io.i_LoadingOff_SW_Check, IO_ON) == IO_ON)
	{
		st_handler.nStackerLightCurtainFlag	= 1;
	}
	
	if(g_ioMgr.get_in_bit(st_io.i_LoadingOn_SW_Check, IO_ON) == IO_ON)
	{
		st_handler.nStackerLightCurtainFlag	= 0;
	}
	
	if(g_ioMgr.get_in_bit(st_io.i_Auto_SwitchCheck, IO_ON) == IO_OFF
		&& st_work.mn_run_status != dRUN)
	{
		/////////��ŸƮ ���۽� ���� ���� ..
		st_handler.n_MutingOn = 1;//�Ŀ����� O
		Func.nLightCurtainMutingOnStep = 0;
		st_handler.n_HeatSinkMutingOn = 1;
		//Func.nLightCurtainHeatSinkMutingOnStep = 0;
		
		st_handler.nStackerLightCurtainFlag = 1;
		st_handler.nHeatSinkLightCurtainFlag = 1;
	}
}