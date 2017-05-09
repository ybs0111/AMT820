// Run_EmptyStacker_Elvator.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_EmptyStacker_Elvator.h"
#include "IO_Manager.h"
#include "CmmsdkDef.h"
#include "AMTLotManager.h"
#include "FastechPublic_IO.h"
#include "LogFromat.h"

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
	mn_RunStep = 0;
	mn_InitStep = 0;
	m_nRetry = 0;
	m_nAxisNum = M_TRAY2_Z;
}

CRun_EmptyStacker_Elvator::~CRun_EmptyStacker_Elvator()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_EmptyStacker_Elvator message handlers


void CRun_EmptyStacker_Elvator::Thread_Run()
{


	switch( st_work.mn_run_status)
	{
	case dINIT:
		RunInit();
		break;

	case dRUN:
// 		RunMove();
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
	int nRet_1 = 0;

	if( st_handler.mn_init_state[INIT_EMPTYSTACKER_ELV] != CTL_NO ) return;

// 	st_handler.mn_init_state[INIT_EMPTYSTACKER_ELV] = CTL_YES;

	switch(mn_InitStep)
	{
	case 0:		
		mn_InitStep = 100;
		break;
		
	case 100:
		nRet_1 = g_ioMgr.get_in_bit( st_io.i_unloading_stacker_tray1_type_chk, IO_ON );
		if( nRet_1 == IO_OFF )
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_unloading_stacker_tray1_type_chk);
			CTL_Lib.Alarm_Error_Occurrence(3501, dWARNING, m_strAlarmCode);
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
			CTL_Lib.Alarm_Error_Occurrence(3502, dWARNING, m_strAlarmCode);
			mn_InitStep = 900;
		}
		break;
		
	case 300:
// 		COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD ��Ȱ��, cmTRUE = 1 SD Ȱ�� 	
// 		COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_LATCH, cmTRUE);//16
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
			CTL_Lib.Alarm_Error_Occurrence( 3503, dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 400:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
		{//910012 1 A "EMPTY_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
			m_strAlarmCode.Format(_T("910012"));
			CTL_Lib.Alarm_Error_Occurrence(3504, dWARNING, m_strAlarmCode);
			mn_InitStep = 900;
		}
		else if(nRet_1 == BD_ERROR)//�δ� �÷��̵忡 Ʈ���̰� ���� �������� ���� 
		{
			mn_InitStep = 500;
		}
		break;
		
	case 500:
		nRet_1 = g_ioMgr.get_in_bit( st_io.i_Unloading_Stacker_Tray_Limit_Check, IO_OFF);
		if( nRet_1 == IO_ON)
		{
			m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Unloading_Stacker_Tray_Limit_Check);
			CTL_Lib.Alarm_Error_Occurrence(3505, dWARNING, m_strAlarmCode);
			mn_RunStep = 900;
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
		st_handler.mn_init_state[INIT_EMPTYSTACKER_ELV] = CTL_YES;
		mn_InitStep = 0;
		mn_RunStep = 0;
		break;
	}
}

void CRun_EmptyStacker_Elvator::RunMove()
{

	int nRet_1, nRet_2=0, nRet_3=0,nRet_4=0;
	int nCount = 0;

	Func.ThreadFunctionStepTrace(3, mn_RunStep);		

	switch (mn_RunStep)
	{ 
	case -1: 

		mn_RunStep = 0;
		break;

	case  0:
		m_nFindLotNo_Flag = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( ( g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES") && g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{
				//load plate�� ���� ��û
				m_nFindLotNo_Flag = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
				mn_RunStep = 10;
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( ( g_lotMgr.GetLotAt(1).GetStrLastModule() != "YES") && g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
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
<<<<<<< HEAD
=======
			mn_RunStep = 10;
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
		}
		break;

	case 10:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //Ʈ���̸� ���� ��ġ�� �̸� �̵��Ѵ� 
		if (nRet_1 == BD_GOOD) //�������� �̵�  
		{
			mn_retry_cnt = 0;
			mn_RunStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 10;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�  
<<<<<<< HEAD
			CTL_Lib.Alarm_Error_Occurrence(3506, dWARNING, alarm.mstr_code);
=======
			CTL_Lib.Alarm_Error_Occurrence(3100, dWARNING, alarm.mstr_code);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
			mn_RunStep = 10;
		}
		break; 

	case 100:
		nRet_1 = Ready_Stacker_Move_Check(0);
		if( nRet_1 == RET_GOOD )
		{
			mn_RunStep = 200;
		}
		else if( nRet_1 == RET_ERROR )
		{
<<<<<<< HEAD
			CTL_Lib.Alarm_Error_Occurrence(3507, dWARNING, m_strAlarmCode);
=======
			CTL_Lib.Alarm_Error_Occurrence(3101, dWARNING, m_strAlarmCode);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
		{
			m_strAlarmCode.Format(_T("910102")); //910102 1 A "EMPTY_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
<<<<<<< HEAD
			CTL_Lib.Alarm_Error_Occurrence(3601, dWARNING, m_strAlarmCode);
=======
			CTL_Lib.Alarm_Error_Occurrence(3102, dWARNING, m_strAlarmCode);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
		}
		else if(nRet_1 == BD_ERROR)//�δ� �÷��̵忡 Ʈ���̰� ���� �������� ���� 
		{
			mn_RunStep = 300;
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //Ʈ���̸� ���� ��ġ�� �̸� �̵��Ѵ� 
		if (nRet_1 == BD_GOOD) //�������� �̵�  
		{			
			mn_RunStep = 400;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�  
<<<<<<< HEAD
			CTL_Lib.Alarm_Error_Occurrence(3602, dWARNING, alarm.mstr_code);
=======
			CTL_Lib.Alarm_Error_Occurrence(3103, dWARNING, alarm.mstr_code);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
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
<<<<<<< HEAD
			//CTL_Lib.Alarm_Error_Occurrence(3603, dWARNING, m_strAlarmCode);
=======
			//CTL_Lib.Alarm_Error_Occurrence(3104, dWARNING, m_strAlarmCode);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
		}
		else if(nRet_1 == BD_ERROR)
		{
			mn_RunStep = 500;
		}
		break;

	case 500:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Exist_Check, IO_OFF); //stacker tary �� ó������ ����� �Ѵ�  
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Ready_Check, IO_OFF); //stacker tary �� ó������ ����� �Ѵ�  
		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			mn_RunStep = 1000;
		}
		else
		{//Ʈ���� ���� ���� 
			mn_RunStep = 1000;//james 2016.0915 �� Ʈ���̰� �־ ��������. ���� ����û ��Ȳ�� �ľ��Ͽ� ���� ��/�� ���� �ʿ� 
			break;

			if( nRet_1 == IO_ON )
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Unloading_Stacker_Tray_Exist_Check);  
			else
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Unloading_Stacker_Tray_Ready_Check);  
<<<<<<< HEAD
			CTL_Lib.Alarm_Error_Occurrence(3604, dWARNING, m_strAlarmCode);
=======
			CTL_Lib.Alarm_Error_Occurrence(3105, dWARNING, m_strAlarmCode);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////
	// empty stacker�� �����Ͽ� �۾��ϴ� ����
	//////////////////////////////////////////////////////////////////////////////////
	case 1000:
		if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_REQ) //Ʈ�����ۿ��� empty stacker�� �� Ʈ���̸� ���� ��û
		{
			if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)
			{
				mn_RunStep = 2000;
			}						
		}	
		else
		{
// 			if(m_nFindLotNo_Flag == 0 )//LOT_CURR
// 			{
				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" &&
					st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_WORK_TRANSFER] == LOTEND &&
					st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_EMPTY_STACKER] != LOTEND)
				{
					mn_RunStep = 9000;
				}
// 			}
// 			else if(m_nFindLotNo_Flag == 1 ) //LOT_NEXT
// 			{
// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" &&
// 					st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_WORK_TRANSFER] == dLOTEND &&
// 					st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_EMPTY_STACKER] != dLOTEND)
// 				{
// 					mn_RunStep = 9000;
// 				}
// 			}

				if( g_lotMgr.GetLotCount() <= 0)
				{
					mn_RunStep = 9000;
				}
		}
		break;

	case 1100:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //Ʈ���̸� Ʈ�����ۿ��� �޴´� 

		if(nRet_1 == RET_GOOD)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Unloading_Stacker_Tray_Exist_Check); 
<<<<<<< HEAD
			CTL_Lib.Alarm_Error_Occurrence(3605, dWARNING, m_strAlarmCode); 
=======
			CTL_Lib.Alarm_Error_Occurrence(3106, dWARNING, m_strAlarmCode); 
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4

			mn_RunStep = 1000;
		}
		break;

	case 2000:
		//if(st_sync_info.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)//�޴� �������� ��� �־ �� 
		mn_RunStep = 3000;		
		break;

	case 3000:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //Ʈ���̸� Ʈ�����ۿ��� �޴´� 
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 3100;
		}
		else if(nRet_1 == RET_ABORT)// TRAY FULL Error
		{
			if (st_handler.cwnd_main != NULL)
			{
				st_msg.mstr_event_msg[0] = _T("[TRAY FULL] Empty stacker is full. \r\n Please Remove it.");
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			}
		}
		break;

	case 3100:
		m_nRetry = 0;
		st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_READY;
		mn_RunStep = 4000;
		break;

	case 3200:
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //Ʈ���̸� Ʈ�����ۿ��� �޴´� 

		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 4000;
		}
		break;

	case 4000:
		if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_FREE) //Ʈ������ place Ʈ���� �۾� �Ϸ� 
		{
			if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)
			{
				nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
				if(nRet_1 == BD_GOOD)//�÷���Ʈ�� Ʈ���̰� ���� �� ���� 
				{
					st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_CLEAR;
					st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] = CTL_CLEAR;
					mn_RunStep = 5000;
				}
				if(nRet_1 == BD_ERROR) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �ȵ� ���� 
				{
					m_nRetry++;
					if (m_nRetry > 2)
					{
						m_nRetry = 0;
						m_strAlarmCode.Format(_T("900004")); //900001 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR" //kwlee 2016.0902 "900000" ->"900004"
<<<<<<< HEAD
						CTL_Lib.Alarm_Error_Occurrence(3607, dWARNING, m_strAlarmCode); 
=======
						CTL_Lib.Alarm_Error_Occurrence(3107, dWARNING, m_strAlarmCode); 
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
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

			if (nRet_1 == BD_GOOD) //�������� �̵�  
			{			
				st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_EMPTY_STACKER] = LOTEND;
				mn_RunStep = 0;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 9100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�  
<<<<<<< HEAD
				CTL_Lib.Alarm_Error_Occurrence(3608, dWARNING, alarm.mstr_code);
=======
				CTL_Lib.Alarm_Error_Occurrence(3108, dWARNING, alarm.mstr_code);
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
				mn_RunStep = 9100;
			}
			break; 

	}
}

int CRun_EmptyStacker_Elvator::Ready_Stacker_Move_Check( int mode )
{
	//mode : 0 : ó�� �Ʒ����� ������ ��� 1: ������ SD ���߸鼭 ������ ���
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
	if( mode == 0 )//�Ʒ��� ���� ��� 
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

void CRun_EmptyStacker_Elvator::Set_Tray_Fix_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Lock_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Unloading_Stacker_Tray_Unlock_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("ULD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("ULD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

//������ ���� sol ��½ð���ŭ ������
int CRun_EmptyStacker_Elvator::Chk_Tray_Fix_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Lock_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Unlock_Check, IO_ON) == IO_ON )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Lock_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Unlock_Check, IO_ON) == IO_ON )
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
				clsLog.LogFunction(_T("ULD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Unloading_Stacker_Tray_Lock_Check); 
				clsLog.LogFunction(_T("ULD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Lock_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Unlock_Check, IO_OFF) == IO_OFF )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Lock_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Unlock_Check, IO_OFF) == IO_OFF )
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
				clsLog.LogFunction(_T("ULD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Unloading_Stacker_Tray_Lock_Check); 
				clsLog.LogFunction(_T("ULD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}
