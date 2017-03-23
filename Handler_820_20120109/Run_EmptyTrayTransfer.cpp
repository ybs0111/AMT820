// Run_EmptyTrayTransfer.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_EmptyTrayTransfer.h"
#include "Public_Function.h"

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

void CRun_EmptyTrayTransfer::ThreadRun()
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


void CRun_EmptyTrayTransfer::RunInit()
{

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
			if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetLotCount )
			{				
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
			CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
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
			CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
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
			nRet_1 = FAS_IO.get_in_bit(st_io.i_Tray_Vacuum_On_Check, IO_ON);  
			if(nRet_1 == IO_OFF)
			{	//Ʈ���̰� ��� �����߻�  
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(4630, dWARNING, m_strAlarmCode);
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
			nRet_1 = FAS_IO.get_in_bit(st_io.i_Tray_Vacuum_On_Check, IO_OFF);  				
			if(nRet_1 == IO_ON)
			{//Ʈ���� ������ �˶� 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(4640, dWARNING, m_strAlarmCode);
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
		break;

	case 1100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_Tray_Remover_Z_Up_Check,	IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_Tray_Remover_Z_Down_Check,	IO_OFF);
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
			CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
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
			CTL_Lib.Alarm_Error_Occurrence(4660, dWARNING, st_alarm_info.strCode);
			mn_RunStep = 1200;
		}
		break; 

		//////////////////////////////////////////////////////////////////////
		// LOAD TRAY �ش� �۾� ��ġ�� ���� place �ϴ� ��ƾ 
		//////////////////////////////////////////////////////////////////////
	case 2000;
		nRet_1 = FAS_IO.get_in_bit(st_io.i_Tray_Remover_Z_Up_Check,	IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_Tray_Remover_Z_Down_Check,	IO_OFF); 
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
			CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
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
			CTL_Lib.Alarm_Error_Occurrence(4680, dWARNING, st_alarm_info.strCode);
			m_nRunStep = 2000;
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
				CTL_Lib.Alarm_Error_Occurrence(4690, dWARNING, m_strAlarmCode);
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
		nRet_1 = COMI.Check_MotPosRange(m_nAxisNum, m_dpTargetPos, st_motor[m_nAxisNum].mn_allow); 			
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
			CTL_Lib.Alarm_Error_Occurrence(4710, dWARNING, m_strAlarmCode);
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
				CTL_Lib.Alarm_Error_Occurrence(4720, dWARNING, m_strAlarmCode);
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
				CTL_Lib.Alarm_Error_Occurrence(4730, dWARNING, m_strAlarmCode);
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
			CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
			mn_RunStep = 3300;
		}
		break;

	case 3400:
		if(m_nTransferJobFlag[1] == WORK_PICK)
		{  //pick		 
			nRet_1 = FAS_IO.get_in_bit(st_io.i_Tray_Vacuum_On_Check,	IO_ON);  
			if(nRet_1 == IO_ON)
			{
				mn_RunStep = 4000;
			}
			else
			{	//Ʈ���� �־�� �Ѵ� 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(4750, dWARNING, m_strAlarmCode);
				break;
			}	
		}
		else //if(m_nTransferJobFlag[1] == WORK_PLACE)
		{  //place
			nRet_1 = FAS_IO.get_in_bit(st_io.i_Tray_Vacuum_On_Check,	IO_OFF);
			if(nRet_1 == IO_OFF)
			{//���� 					
				mn_RunStep = 4000;
			}
			else
			{
				//Ʈ���� ����� �Ѵ� 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Tray_Vacuum_On_Check); 
				CTL_Lib.Alarm_Error_Occurrence(4760, dWARNING, m_strAlarmCode); 
			}	
		} 
		break;

	case 4000: //Ʈ���� ���� ���� 
		if(m_nTransferJobFlag[1] == WORK_PICK)
		{
			if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)//Ʈ������ Ʈ���� ���� 
			{
				clsFunc.Handler_Tray_DataInfo_Shift(1, CTL_YES, THD_LD_TRAY_PLATE, THD_WORK_TRANSFER); //Ʈ���� ������ load plate -> transfer�� ����Ʈ �Ѵ� // Handler_Tray_DataInfo_Shift(int nMode, int nDvc_Yes_No, int nSend_SIte, int nRcv_Site)

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
				clsFunc.Handler_Tray_DataInfo_Shift(1, CTL_YES, THD_WORK_TRANSFER, THD_EMPTY_STACKER); //Ʈ���� ������ transfer -> empty stacker�� ����Ʈ �Ѵ� 

				st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_FREE;
				st_tray_info[THD_WORK_TRANSFER].nTrayExist =  CTL_NO;
			}
		}
		else
		{
			//error
			break;
		}
		mn_RunStep = 1000; 			 
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

	FAS_IO.set_out_bit( st_io.o_Tray_Vacuum_On_Sol, nOnOff);
	FAS_IO.set_out_bit( st_io.o_Tray_Vacuum_Off_Sol, !nOnOff);

	if (nOnOff == IO_ON)
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER""),_T("VACUUMON"),0,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER),_T("VACUUMOFF"),0,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
	}
}

int CRun_EmptyTrayTransfer::Chk_Tray_Grip_Vacuum_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (nOnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag[0] == false && FAS_IO.set_out_bit( st_io.i_Tray_Vacuum_On_Check, IO_OFF ) == IO_OFF )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[0] == true && FAS_IO.set_out_bit( st_io.i_Tray_Vacuum_On_Check, IO_OFF ) == IO_OFF )
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
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.o_Tray_Guide_Clamp_Forward_Sol); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag[0] == false && FAS_IO.set_out_bit( st_io.i_Tray_Vacuum_On_Check, IO_ON ) == IO_ON )
		{
			m_bClampOnOffFlag[0]			= true;
			m_dwClampOnOff[0][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[0] == true && FAS_IO.set_out_bit( st_io.i_Tray_Vacuum_On_Check, IO_ON ) == IO_ON )
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
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.o_Tray_Guide_Clamp_Forward_Sol); 
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

	FAS_IO.set_out_bit( st_io.o_Tray_Remover_Z_Up_Sol, nOnOff);
	FAS_IO.set_out_bit( st_io.o_Tray_Remover_Z_Down_Sol, !nOnOff);

	if (nOnOff == IO_ON)
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER""),_T("ZDOWN"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER),_T("ZUP"),0,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_EmptyTrayTransfer::Chk_Tray_Remover_Z_UpDown(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (nOnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag[1] == false && FAS_IO.set_out_bit( st_io.i_Tray_Remover_Z_Down_Check, IO_ON ) == IO_ON )
		{
			m_bClampOnOffFlag[1]		= true;
			m_dwClampOnOff[1][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[1] == true && FAS_IO.set_out_bit( st_io.i_Tray_Remover_Z_Down_Check, IO_ON ) == IO_ON )
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
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.o_Tray_Guide_Clamp_Forward_Sol); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMOFF"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag[1] == false && FAS_IO.set_out_bit( st_io.i_Tray_Remover_Z_Up_Check, IO_ON ) == IO_ON )
		{
			m_bClampOnOffFlag[1]			= true;
			m_dwClampOnOff[1][0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag[1] == true && FAS_IO.set_out_bit( st_io.i_Tray_Remover_Z_Up_Check, IO_ON ) == IO_ON )
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
				m_dwClampOnOff[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[1][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.o_Tray_Guide_Clamp_Forward_Sol); 
				clsLog.LogFunction(_T("EMPRT_TRAY_TRANSFER"),_T("VACUUMON"),1,_T("TRAY"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}