// Run_EmptyTrayTransfer.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_EmptyTrayTransfer.h"
#include "Public_Function.h"
#include "AMTLotManager.h"
#include "IO_Manager.h"
#include "math.h"
#include "LogFromat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_EmptyTrayTransfer
CRun_EmptyTrayTransfer Run_EmptyTrayTransfer;
IMPLEMENT_SERIAL(CRun_EmptyTrayTransfer, CObject, 1);

CRun_EmptyTrayTransfer::CRun_EmptyTrayTransfer()
{
	m_nAxisNum = M_TRAY_REMOVE_X;
}

CRun_EmptyTrayTransfer::~CRun_EmptyTrayTransfer()
{
}

////////////////////////////////////////////////////////////////////////////
// CRun_EmptyTrayTransfer message handlers

void CRun_EmptyTrayTransfer::Thread_Run()
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


void CRun_EmptyTrayTransfer::RunInit()
{
	int nRet_1 = 0;
	
	if( st_handler.mn_init_state[INIT_LDSTACKER_ELV] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_EMPTYSTACKER_ELV] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_LD_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_EPOXY_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_EMPTYTRAY_TRANSFER] != CTL_NO ) return;
// 	st_handler.mn_init_state[INIT_EMPTYTRAY_TRANSFER] = CTL_YES;
	switch(mn_InitStep)
	{
		case 0:		
			mn_InitStep = 100;
			break;

		case 100://�Ǹ��� ��
			Set_Tray_Remover_Z_UpDown( IO_OFF );
			mn_InitStep = 110;
			break;

		case 110:
			nRet_1 = Chk_Tray_Remover_Z_UpDown( IO_OFF );
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 200;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 2001, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			break;

		case 200:
			Set_Tray_Grip_Vacuum_OnOff(IO_ON);
			mn_InitStep = 210;
			break;

		case 210:
			nRet_1 = Chk_Tray_Grip_Vacuum_OnOff(IO_ON);
			if( nRet_1 == RET_GOOD )
			{
				CTL_Lib.Alarm_Error_Occurrence( 2002, dWARNING, m_strAlarmCode);
			}
			else if( nRet_1 == RET_ERROR)
			{
				Set_Tray_Grip_Vacuum_OnOff( IO_OFF);
				mn_InitStep = 300;
			}
			break;

		case 300:
// 			COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD ��Ȱ��, cmTRUE = 1 SD Ȱ�� 	
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
				CTL_Lib.Alarm_Error_Occurrence( 2003, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_INIT_POS], COMI.mn_runspeed_rate);  //left�� �̸� �̵��� �ִ´� 
			if (nRet_1 == BD_GOOD) //�������� �̵�  
			{			
				mn_InitStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�  
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence(2004, dWARNING, alarm.mstr_code);
			}
			break;

		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;

		case 1000:
			st_handler.mn_init_state[INIT_EMPTYTRAY_TRANSFER] = CTL_YES;
			mn_InitStep = 0;
			break;

		}

}

void CRun_EmptyTrayTransfer::RunMove()
{
	int nRet_1, nRet_2=0, nRet_3=0, nJobFlag = 0;	

	Func.ThreadFunctionStepTrace(4, mn_RunStep);	

	switch( mn_RunStep )
	{
	case -1:
		mn_RunStep = 0;
		break;

	case 0:
		m_nFindLotNo_Flag = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( ( g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES") && g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{				
				m_nFindLotNo_Flag = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
				mn_RunStep = 100;
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( ( g_lotMgr.GetLotAt(1).GetStrLastModule() != "YES") && g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
				{
					m_nFindLotNo_Flag = 1;
					m_strLotNo = g_lotMgr.GetLotAt(1).GetLotID();
					m_strPartNo = g_lotMgr.GetLotAt(1).GetPartID();
					mn_RunStep = 100;
				}
				else
				{
					return;
				}
			}
		}
		break;

	case  100:
		Set_Tray_Grip_Vacuum_OnOff(IO_OFF);
		mn_RunStep = 200;
		break;

	case 200:
		nRet_1 = Chk_Tray_Grip_Vacuum_OnOff(IO_OFF );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 300;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2100, dWARNING, m_strAlarmCode);
			mn_RunStep = 100;
		}
		break;

	case 300:
		Set_Tray_Remover_Z_UpDown(IO_OFF);
		mn_RunStep = 400;
		break;

	case 400:
		nRet_1 = Chk_Tray_Remover_Z_UpDown(IO_OFF );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 500;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2101, dWARNING, m_strAlarmCode);
			mn_RunStep = 100;
		}
		break;

	case 500:
		st_tray_info[THD_WORK_TRANSFER].nTrayExist = CTL_NO;
		m_nTransfer_WaitPosMove_Flag = CTL_NO;
		mn_RunStep = 1000;
		break;

	case 1000:
		if( st_handler.mn_removetray == CTL_REQ || st_handler.mn_removetray == CTL_CHANGE)
		{
		}
		else
		{
			if(st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_REQ) //Load Tray PlateƮ������ �� Ʈ,���� ��ü�� ��û 
			{
				nJobFlag += (int) pow((double)2, 0);// �۾��� ��ġ ���; //1
			}		
		}		

		if( st_handler.mn_removetray == CTL_NO && (nJobFlag == 0 || g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" ) )
		{//lot end Ȯ�� 
			if(st_tray_info[THD_WORK_TRANSFER].nTrayExist == CTL_YES) 
			{
				m_nTransferJobFlag[0] = THD_EMPTY_STACKER; 
				m_nTransferJobFlag[1] = WORK_PLACE; 

				//�̹� ��� �ִ� Ʈ���̸� ������ empty site�� ���� �غ��϶�� ��û�Ѵ� 
				st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_REQ;
				st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] = WORK_PLACE;

				mn_RunStep = 2000; //Ʈ���̸� ������ empty site�� �̵��Ѵ� 
				break;
			}			 
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//Ʈ���̸� ������ �ִ��� / ������ Ȯ���� ������ �����Ѵ� 
		/////////////////////////////////////////////////////////////////////////////////////////////////
		if(st_tray_info[THD_WORK_TRANSFER].nTrayExist == CTL_YES)
		{
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check, IO_ON);  
			if(nRet_1 == IO_OFF)
			{	//Ʈ���̰� ��� �����߻�  
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(2102, dWARNING, m_strAlarmCode);
				break;
			}	

			if(nJobFlag == 1)
			{ 
				m_nTransferJobFlag[0] = THD_EMPTY_STACKER; 
				m_nTransferJobFlag[1] = WORK_PLACE; 

				//�̹� ��� �ִ� Ʈ���̸� ������ empty site�� ���� �غ��϶�� ��û�Ѵ� 
				st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_REQ;
				st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][1] = WORK_PLACE;
			}
		}
		else  
		{
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check, IO_OFF);  				
			if(nRet_1 == IO_ON)
			{//Ʈ���� ������ �˶� 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(2103, dWARNING, m_strAlarmCode);
				break;
			}

			if(nJobFlag == 1)
			{ // �ε� �� Ʈ���̸� ������ �̵��Ѵ� 
				m_nTransferJobFlag[0] = THD_LD_TRAY_PLATE; 
				m_nTransferJobFlag[1] = WORK_PICK; 
			}
		}

		if(nJobFlag > 0) //�۾� �� Ʈ���̰� �ִ�
		{
			m_nTransfer_WaitPosMove_Flag = CTL_NO;
			mn_RunStep = 2000;
		}

		if( st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_TRAY_PLATE] == LOTEND )
		{
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_WORK_TRANSFER] = LOTEND;
		}
		break;

	case 1100:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Up_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Down_Check,	IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			mn_RunStep = 1200;
		}
		else 
		{
			mn_RunStep = 1110;		
		}
		break;

	case 1110://transfer up 
		Set_Tray_Remover_Z_UpDown(IO_OFF);
		mn_RunStep = 1120;
		break;

	case 1120:
		nRet_1 = Chk_Tray_Remover_Z_UpDown(IO_OFF );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 1200;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2104, dWARNING, m_strAlarmCode);
			mn_RunStep = 1110;
		}
		break;

	case 1200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_INIT_POS], COMI.mn_runspeed_rate);  //left�� �̸� �̵��� �ִ´� 
		if (nRet_1 == BD_GOOD) //�������� �̵�  
		{			
			m_nTransfer_WaitPosMove_Flag = CTL_YES;

			mn_RunStep = 1000;

			//2017.0109
			if( st_tray_info[THD_WORK_TRANSFER].nStartStatus  == 1)
			{
				st_tray_info[THD_WORK_TRANSFER].nStartStatus = 0;

				clsLog.LogTransfer(_T("EMPTY_TRAY_TRANSFER"),
					_T("MOVE_EMPTY_TRAY"),
					1,
					st_tray_info[THD_WORK_TRANSFER].strMaterial,
					_T("EMPTY_TRAY"),
					st_tray_info[THD_WORK_TRANSFER].strMovePos[1], 
					st_tray_info[THD_WORK_TRANSFER].strMovePos[0], 
					1, 
					st_tray_info[THD_WORK_TRANSFER].strLogKey,
					st_tray_info[THD_WORK_TRANSFER].strLogData);
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 1200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�  
			CTL_Lib.Alarm_Error_Occurrence(2105, dWARNING,  alarm.mstr_code);
			mn_RunStep = 1200;
		}
		break; 

		//////////////////////////////////////////////////////////////////////
		// LOAD TRAY �ش� �۾� ��ġ�� ���� place �ϴ� ��ƾ 
		//////////////////////////////////////////////////////////////////////
	case 2000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Up_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Down_Check,	IO_OFF); 
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{ //��Ŀ�� ��� ���� 
			mn_RunStep = 2100;			
		}
		else 
		{
			mn_RunStep = 2010;			
		}
		break;

	case 2010:
		Set_Tray_Remover_Z_UpDown(IO_OFF);
		mn_RunStep = 2020;
		break;

	case 2020:
		nRet_1 = Chk_Tray_Remover_Z_UpDown(IO_OFF );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 2000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2106, dWARNING, m_strAlarmCode);
			mn_RunStep = 2010;
		}
		break;

	case 2100:
		if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)
		{
			m_dpTargetPos = st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_PICK_POS];
		}
		else if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
		{
			m_dpTargetPos = st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_PLACE_POS];
		}
		else
		{
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, m_dpTargetPos, COMI.mn_runspeed_rate);  //left�� �̸� �̵��� �ִ´� 
		if (nRet_1 == BD_GOOD) //�������� �̵�  
		{			
			mn_RunStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�  
			CTL_Lib.Alarm_Error_Occurrence(2107, dWARNING, alarm.mstr_code);
			mn_RunStep = 2000;
		}
		break; 

	//TRANSFER ���� 
	case 3000:
		if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)
		{
			nRet_1 = COMI.Get_MotIOSensor(THD_LD_STACKER, MOT_SENS_SD); 	
			if(nRet_1 != BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
			{
				m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(2108, dWARNING, m_strAlarmCode);
				break;
			}	
		}
		else if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
		{	 //����Ŀ�� ���¸� üũ�� �ʿ䵵 ���� 		
			if(m_nTransferJobFlag[1] == WORK_PLACE)
			{
				if(st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] != CTL_READY) //empty���� Ʈ���̸� ���� �� �� �غ�Ǿ����� ��ٸ��� 
				{
					break;
				}
			}
		}
		mn_RunStep = 3100;
		break;

	case 3100:
		nRet_1 = COMI.Check_MotPosRange(m_nAxisNum, m_dpTargetPos, COMI.md_allow_value[m_nAxisNum]); 			
		if (nRet_1 != BD_GOOD)
		{
			mn_RunStep = 2000;
			break;
		}

		Set_Tray_Remover_Z_UpDown(IO_ON);

		if(m_nTransferJobFlag[1] == WORK_PICK)
		{  //pick ���� , �̸����� 
			Set_Tray_Grip_Vacuum_OnOff(IO_ON);
			//air sol �� �ִ��� Ȯ�� �ʿ�
		}
		else
		{  //place ���� 
		}
		mn_RunStep = 3110;

		break;

	case 3110:
		nRet_1 = Chk_Tray_Remover_Z_UpDown(IO_ON); 
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 3200;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2109, dWARNING, m_strAlarmCode);
			mn_RunStep = 3100;
		}
		break;

	case 3200:
		if(m_nTransferJobFlag[1] == WORK_PICK)
		{  //pick ���� 
			Set_Tray_Grip_Vacuum_OnOff(IO_ON);
		}
		else //if(m_nTransferJobFlag[1] == WORK_PLACE)
		{  //place ���� 
			Set_Tray_Grip_Vacuum_OnOff(IO_OFF);
		}
		mn_RunStep = 3210;
		break;

	case 3210:
		if(m_nTransferJobFlag[1] == WORK_PICK)
		{  //pick ���� 				 
			nRet_1 = Chk_Tray_Grip_Vacuum_OnOff(IO_ON);
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 3300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				Set_Tray_Remover_Z_UpDown(IO_OFF); 
				CTL_Lib.Alarm_Error_Occurrence(2110, dWARNING, m_strAlarmCode);
				mn_RunStep = 2000;
			}
		}
		else //if(m_nTransferJobFlag[1] == TRAY_PLACE)
		{  //place ���� 
			nRet_1 = Chk_Tray_Grip_Vacuum_OnOff(IO_OFF);
			if(nRet_1 == RET_GOOD)
			{
				Set_Tray_Remover_Z_UpDown(IO_OFF);
				mn_RunStep = 3300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				Set_Tray_Remover_Z_UpDown(IO_OFF); 
				CTL_Lib.Alarm_Error_Occurrence(2111, dWARNING, m_strAlarmCode);
				mn_RunStep = 2000;
			}
		} 
		break;

	case 3300: 
		Set_Tray_Remover_Z_UpDown(IO_OFF); 
		mn_RunStep = 3310;
		break;

	case 3310:
		nRet_1 = Chk_Tray_Remover_Z_UpDown(IO_OFF );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 3400;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2112, dWARNING, m_strAlarmCode);
			mn_RunStep = 3300;
		}
		break;

	case 3400:
		if(m_nTransferJobFlag[1] == WORK_PICK)
		{  //pick		 
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check,	IO_ON);  
			if(nRet_1 == IO_ON)
			{
				mn_RunStep = 4000;
			}
			else
			{	//Ʈ���� �־�� �Ѵ� 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(2113, dWARNING, m_strAlarmCode);
				break;
			}	
		}
		else //if(m_nTransferJobFlag[1] == WORK_PLACE)
		{  //place
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check,	IO_OFF);
			if(nRet_1 == IO_OFF)
			{//���� 					
				mn_RunStep = 4000;
			}
			else
			{
				//Ʈ���� ����� �Ѵ� 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(2114, dWARNING, m_strAlarmCode); 
			}	
		} 
		break;

	case 4000: //Ʈ���� ���� ���� 
		if(m_nTransferJobFlag[1] == WORK_PICK)
		{
			if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)//Ʈ������ Ʈ���� ���� 
			{
				Func.Handler_Tray_DataInfo_Shift(1, CTL_YES, THD_LD_TRAY_PLATE, THD_WORK_TRANSFER); //Ʈ���� ������ load plate -> transfer�� ����Ʈ �Ѵ� // Handler_Tray_DataInfo_Shift(int nMode, int nDvc_Yes_No, int nSend_SIte, int nRcv_Site)

				st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_READY;
				st_tray_info[THD_WORK_TRANSFER].nTrayExist = CTL_YES;


				st_tray_info[THD_WORK_TRANSFER].strLogKey[0]	= _T("EMPRT_TRAY_TRANSFER");
				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() != _T("") && g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() != "YES")
				{
					st_tray_info[THD_WORK_TRANSFER].strLogData[0]	= g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID();
				}
				else
				{
					st_tray_info[THD_WORK_TRANSFER].strLogData[0]	= _T("$");
				}

				st_tray_info[THD_WORK_TRANSFER].strMaterial = _T("$");
				st_tray_info[THD_WORK_TRANSFER].nStartStatus = 1;
				st_tray_info[THD_WORK_TRANSFER].strMovePos[0] = clsLog.GetDeviceID( THD_LD_TRAY_PLATE );
				st_tray_info[THD_WORK_TRANSFER].strMovePos[1] = clsLog.GetDeviceID( THD_EMPTY_STACKER );


				clsLog.LogTransfer(_T("EMPTY_TRAY_TRANSFER"),
					_T("MOVE_EMPTY_TRAY"),
					0,
					st_tray_info[THD_WORK_TRANSFER].strMaterial,
					_T("EMPTY_TRAY"),
					st_tray_info[THD_WORK_TRANSFER].strMovePos[0], 
					st_tray_info[THD_WORK_TRANSFER].strMovePos[1], 
					1, 
					st_tray_info[THD_WORK_TRANSFER].strLogKey,
					st_tray_info[THD_WORK_TRANSFER].strLogData);

				mn_RunStep = 4010;

			}
			else// if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
			{
				//error
			}
		}
		else if(m_nTransferJobFlag[1] == WORK_PLACE) //Ʈ������ Ʈ���� ���� 
		{				
			if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
			{
				Func.Handler_Tray_DataInfo_Shift(1, CTL_YES, THD_WORK_TRANSFER, THD_EMPTY_STACKER); //Ʈ���� ������ transfer -> empty stacker�� ����Ʈ �Ѵ� 

				st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_FREE;
				st_tray_info[THD_WORK_TRANSFER].nTrayExist =  CTL_NO;
				mn_RunStep = 4100;
			}
		}
		else
		{
			//error
			break;
		}
// 		mn_RunStep = 1000; 			 
		break;

		
	case 4010:
		if( st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_FREE )
		{
			st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_CLEAR;
			mn_RunStep = 1000; 			 
		}
		break;
		
	case 4100:
		if( st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_CLEAR)
		{
			mn_RunStep = 1000;
		}
		break;

	default:
		if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_EmotyTrayTransfer  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		break;
	}
}


void CRun_EmptyTrayTransfer::Set_Tray_Grip_Vacuum_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag[0]	= false;
	m_dwClampOnOff[0][0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Tray_Vacuum_On_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Tray_Vacuum_Off_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMON"),0,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),0,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
	}
}

int CRun_EmptyTrayTransfer::Chk_Tray_Grip_Vacuum_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag[0] == false && g_ioMgr.get_in_bit( st_io.i_Tray_Vacuum_On_Check, IO_OFF ) == IO_OFF )
		{
			m_bClampOnOffFlag[0]	= true;
			m_dwClampOnOff[0][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[0] == true && g_ioMgr.get_in_bit( st_io.i_Tray_Vacuum_On_Check, IO_OFF ) == IO_OFF )
		{
			m_dwClampOnOff[0][1] = GetCurrentTime();
			m_dwClampOnOff[0][2] = m_dwClampOnOff[0][1] - m_dwClampOnOff[0][0];

			if (m_dwClampOnOff[0][2] <= 0)
			{
				m_dwClampOnOff[0][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[0][2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[0][1] = GetCurrentTime();
			m_dwClampOnOff[0][2] = m_dwClampOnOff[0][1] - m_dwClampOnOff[0][0];

			if (m_dwClampOnOff[0][2] <= 0)
			{
				m_dwClampOnOff[0][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[0][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Tray_Vacuum_On_Check); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag[0] == false && g_ioMgr.get_in_bit( st_io.i_Tray_Vacuum_On_Check, IO_ON ) == IO_ON )
		{
			m_bClampOnOffFlag[0]	= true;
			m_dwClampOnOff[0][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[0] == true && g_ioMgr.get_in_bit( st_io.i_Tray_Vacuum_On_Check, IO_ON ) == IO_ON )
		{
			m_dwClampOnOff[0][1]	= GetCurrentTime();
			m_dwClampOnOff[0][2]	= m_dwClampOnOff[0][1] - m_dwClampOnOff[0][0];

			if (m_dwClampOnOff[0][2] <= 0)
			{
				m_dwClampOnOff[0][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwClampOnOff[0][2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMON"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[0][1]	= GetCurrentTime();
			m_dwClampOnOff[0][2]	= m_dwClampOnOff[0][1] - m_dwClampOnOff[0][0];

			if (m_dwClampOnOff[0][2] <= 0)
			{
				m_dwClampOnOff[0][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[0][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Tray_Vacuum_On_Check); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMON"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}


void CRun_EmptyTrayTransfer::Set_Tray_Remover_Z_UpDown(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag[1] = false;
	m_dwClampOnOff[1][0] = GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Tray_Remover_Z_Up_Sol, !OnOff);
	g_ioMgr.set_out_bit( st_io.o_Tray_Remover_Z_Down_Sol, OnOff);

	if (OnOff == IO_OFF)//down
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("ZUP"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else//up
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("ZDOWN"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_EmptyTrayTransfer::Chk_Tray_Remover_Z_UpDown(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;
	int nRet_1 = g_ioMgr.get_in_bit( st_io.i_Tray_Remover_Z_Up_Check, IO_ON );
	int nRet_2 = g_ioMgr.get_in_bit( st_io.i_Tray_Remover_Z_Down_Check, IO_OFF );

	if (OnOff == IO_OFF)//up
	{
		if (m_bClampOnOffFlag[1] == false &&  nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			m_bClampOnOffFlag[1] = true;
			m_dwClampOnOff[1][0] = GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[1] == true && nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			m_dwClampOnOff[1][1] = GetCurrentTime();
			m_dwClampOnOff[1][2] = m_dwClampOnOff[1][1] - m_dwClampOnOff[1][0];

			if (m_dwClampOnOff[1][2] <= 0)
			{
				m_dwClampOnOff[1][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[1][2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[1][1] = GetCurrentTime();
			m_dwClampOnOff[1][2] = m_dwClampOnOff[1][1] - m_dwClampOnOff[1][0];

			if (m_dwClampOnOff[1][2] <= 0)
			{
				m_dwClampOnOff[1][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[1][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Tray_Remover_Z_Up_Check); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag[1] == false && nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			m_bClampOnOffFlag[1]			= true;
			m_dwClampOnOff[1][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[1] == true && nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			m_dwClampOnOff[1][1]	= GetCurrentTime();
			m_dwClampOnOff[1][2]	= m_dwClampOnOff[1][1] - m_dwClampOnOff[1][0];

			if (m_dwClampOnOff[1][2] <= 0)
			{
				m_dwClampOnOff[1][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwClampOnOff[1][2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMON"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[1][1]	= GetCurrentTime();
			m_dwClampOnOff[1][2]	= m_dwClampOnOff[1][1] - m_dwClampOnOff[1][0];

			if (m_dwClampOnOff[1][2] <= 0)
			{
				m_dwClampOnOff[1][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[1][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Tray_Remover_Z_Up_Check); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMON"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}