// Run_UldPicker.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_UldPicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_UldPicker
CRun_UldPicker Run_UldPicker;
IMPLEMENT_SERIAL(CRun_UldPicker, CObiect, 1);

CRun_UldPicker::CRun_UldPicker()
{
	m_nPickerPara = PICKER_PARA;//1
	m_nRobotSite  = THD_UNLOAD_WORK_RBT;

	m_nRobot_X = M_UNLOADER_TRANSFER_X;
	m_nRobot_Y = M_UNLOADER_TRANSFER_Y;
	m_nRobot_Z = M_UNLOADER_TRANSFER_Z; 

	m_dwCycleTime[0][0] = 0;
	m_dwCycleTime[1][0] = 0;
	m_strLotNo[0] = m_strPartNo[0] = _T("");
	m_strLotNo[1] = m_strPartNo[1] = _T("");

	mn_RunStep = 0;
	mn_RunSafetyStep = 0;
	mn_InitStep = 0;
	mn_Pick_Step = 0;
	mn_Place_Step = 0;
	mn_reinstate_step = 0;
}

CRun_UldPicker::~CRun_UldPicker()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_UldPicker message handlers
void CRun_UldPicker::ThreadRun()
{
	switch( st_work.mn_run_status)
	{
	case dINIT:
		break;

	case dRUN:
		RunMove();
		break;

	case dSTOP:
		if(m_nRobot_Z_Stop_Flag == 0)
		{
			nRet_1 = COMI.Check_Motion_State(m_nRobot_Z, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotorNum, &dwMotionDone);
			if (nRet_1 != BD_GOOD) 
			{//���� ���°� mMST_STOP �̴� 
				if(CTL_Lib.m_nStep_Single[m_nRobot_Z] > 0)
				{
					COMI.Set_MotStop(1, m_nRobot_Z) ; //������� 
					CTL_Lib.m_nStep_Single[m_nRobot_Z] = 0;
					m_nRobot_Z_Stop_Flag = 100; 
				}
				else
				{
					m_nRobot_Z_Stop_Flag = 1;
				}					
			}
			else
			{
				m_nRobot_Z_Stop_Flag = 1;
			}
		}

		m_dwWaitTime[0] = GetCurrentTime();
		break;

	case dWARNING:
		break;

	default:
		break;
	}
}

void CRun_UldPicker::RunInit()
{
}

void CRun_UldPicker::RunMove()
{
	int nRet_1,nRet_2,nRet_3;
	long lMotionDone=0;

	double m_MotCurrPos = COMI.Get_MotCurrentPos( m_nRobot_X );

	Func.ThreadFunctionStepTrace(30, mn_RunStep);
	switch(mn_RunStep)
	{
	case -1:
		mn_RunStep = 100;
		break;

	case 100:
		if( m_nRobot_Z > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_X].mn_allow) )
		{
			mn_RunStep = 110;
		}
		else
		{
			mn_RunStep = 200;
		}
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunStep = 110;
		}
		break;
		
	case 200:
		if( Move_MakeSafetyPosBeforeWork( POS_MOV_READY ) == RET_GOOD )
		{
			mn_RunStep = 1000;
		}
		break;

	case 1000:
		if( g_lotMgr.GetLotCount() > 0 )
		{
			mn_RunStep = 1100;
		}
		break;

	case 1100:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] == WORK_PICK )
		{
			mn_RunStep = 1200;
		}
		break;

	case 1200:
		if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y )  > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS]) + st_motor[M_LOADER_TRANSFER_Y].mn_allow ) )
		{
			break;
		}
		if( st_handler.mn_lduld_safety != LD_MOVING_SAFETY )
		{
			st_sync.nLdUldSateyflag = ULD_MOVING_SAFETY;
			mn_RunStep = 1300;
		}
		break;

	case 1300:
		nRet_1 = Process_DVC_Pickup( 0, THD_LDULD_CARRIER_BUFF, g_lotMgr.GetLotAt(0).GetLotID() );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 2000;
		}
		else if(nRet_1 == RET_PICKER_NOT_FIND)
		{
			mn_RunStep = 3000;			
		}
		else if(nRet_1 == RET_TRAY_NOT_FIND)
		{
			mn_RunStep = 3000;  
		}
		break;

	case 2000:
		if( Move_MakeSafetyPosBeforeWork( POS_MOV_PLACE ) == RET_GOOD )
		{
			mn_RunStep = 2100;
		}
		break;

	case 2100:

		if( m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[m_nRobot_X].mn_allow) )
		{
			COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nRobot_X );
			COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nRobot_Y );
			mn_RunSafetyStep = 2000;
		}
		if( st_handler.mn_lduld_safety != LD_MOVING_SAFETY )
		{
			if( st_sync.nLdUldSateyflag != MOVING_NOT_SAFETY;)
				st_sync.nLdUldSateyflag = MOVING_NOT_SAFETY;
		}

		nRet_1 = Process_DVC_Place( 0, THD_UNLD_ALIGN_BUFF, g_lotMgr.GetLotAt(0).GetLotID() );
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 3000;
		}
		else if(nRet_1 == RET_PICKER_NOT_FIND)
		{
			mn_RunStep = 2100;			
		}
		else if(nRet_1 == RET_TRAY_NOT_FIND)
		{
			mn_RunStep = 2100;  
		}
		break;

	case 3000:
		if( Move_MakeSafetyPosBeforeWork( POS_MOV_READY ) == RET_GOOD )
		{
			mn_RunStep = 1000;
		}
		break;

	default:
		if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_UldPicker  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		break;
	}
}

//nWorkSite: THD_LDULD_CARRIER_BUFF

int CRun_UldPicker::Process_DVC_Pickup( int nMode, int nWorkSite, CString strLotNo)
{
	int nFuncRet = RET_PEOCEED;
	int nRet_1, nRet_2, nRet_3, nRet_4, nRet_5, nRet_6, nFlag = 0;

	CString strMsg;
	CString strLogKey[10];
	CString strLogData[10];

	Func.ThreadFunctionStepTrace(31, mn_Pick_Step);

	switch( mn_Pick_Step )
	{
	case 0:
		m_bDvcWaitChk_Falg =  false;
		m_nPickRetry_Cnt = 0;
		mn_Pick_Step = 100;
		break;

	case 100:
		m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if( m_dCurrentPos[m_nRobot_Z] > ( st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) ) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			mn_Pick_Step = 900;
		}
		else
		{
			mn_Pick_Step = 1000;
		}
		break;

	case 900:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_bDvcWaitChk_Falg =  false;
			mn_Pick_Step = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Pick_Step = 900;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 900;
		}
		break;

		//////////////////////////////////////////////////////
		//��Ŀ ���¸� Ȯ�� �� ������ ������ �۾�
		//////////////////////////////////////////////////////
	case 1000:
		m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			mn_Pick_Step = 900;
			break;
		}


		nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
		if(nRet_1 == RET_GOOD) //���� ������ ���� ��Ŀ ���¸� üũ 
		{			//��Ŀ ������ ���� ���°� �����̴�
		}
		else// if(nRet_1 == RET_ERROR)xx
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(1200, dWARNING, Func.m_strAlarmCode);
				m_bDvcWaitChk_Falg = false;
				break;
			}
		}

		nFlag = 0;
		m_n_FirstTray_Y_Num = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
		for(i = 0; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO)
			{
				m_n_FirstPicker_Num = i;
				i = m_nPickerPara;
			}
		}

		if( m_n_FirstPicker_Num === -1)
		{
			mn_Pick_Step = 0;
			nFuncRet = RET_PICKER_NOT_FIND;
			break;
		}

		nRet_1 = Func.Find_TrayWork_Pos(WORK_PICK, nWorkSite, CTL_YES, m_npFindWorkPosYXCPB, THD_UNLOAD_WORK_RBT, strLotNo);
		if( nRet_1 == RET_GOOD )
		{
			m_n_FirstTray_X_Num = m_npFindWorkPosYXCPB[0];
			m_n_FirstTray_Y_Num = m_npFindWorkPosYXCPB[1];
		}
		else if( nRet_1 == RET_SKIP )//no device
		{
			mn_Pick_Step = 0;
			nFuncRet = RET_TRAY_NOT_FIND;
			break;
		}

		nFlag = 0;
		if( m_n_FirstPicker_Num != -1 && m_n_FirstTray_Y_Num != -1 && m_n_FirstTray_X_Num != -1 )
		{
			for( i = 0; i < m_nPickerPara; i++ )
			{
				m_npPicker_YesNo[i] = CTL_NO;
				if( i < m_n_FirstPicker_Num ) continue;

				if( nWorkSite == THD_UNLOAD_WORK_RBT )
				{
					if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO)
					{
						if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nTrayY)
						{
							if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)].nYesNo == CTL_YES) //���ۿ� ���簡 ������ �Ⱦ������ϰ� �Ѵ�
							{
								m_npPicker_YesNo[i] = CTL_YES;
								nFlag++;
								st_picker[m_nRobotSite].st_pcb_info[i].nStatusChang = NO;
							}
						}
					}
				}
				else
				{
					if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
					{
						sprintf(st_msg.c_abnormal_msg, "There is no WorkSite in Process_DVC_Pickup. Run_UldPicker = %d", mn_Pick_Step);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
						st_work.mn_run_status = dSTOP;
					}
					break;
				}
			}
		}

		if( nFlag == 0 )
		{
			mn_Pick_Step = 0;
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "RET_PICKER_NOT_FIND in Process_DVC_Pickup. Run_UldPicker = %d", mn_Pick_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				st_work.mn_run_status = dSTOP;
			}
			nFuncRet = RET_PICKER_NOT_FIND;
			break;
		}

		m_npFindWorkPosYXCPB[0] = m_n_FirstTray_X_Num;
		m_npFindWorkPosYXCPB[1] = m_n_FirstTray_Y_Num;
		m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;

		m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;

		if( nWorkSite == THD_UNLOAD_WORK_RBT )
		{
			mn_Pick_Step = 1500;
		}
		break;
		

	case 1500:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			mn_Pick_Step = 1600;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Pick_Step = 1500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1201, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 1500;
		}
		break;

	case 1600:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[2] > st_motor_info[m_nRobot_Z].d_pos[P_LOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			mn_Pick_Step = 900;
			break;
		}
		mn_Pick_Step = 2000;
		break;

	//��Ŀ�� ��ġ�� PICK������ ��ġ�� �̵��Ѵ�.
	//�������� �Ǵ��� Ȯ���Ѵ�.
	case 2000:
		if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y )  > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS]) + st_motor[M_LOADER_TRANSFER_Y].mn_allow ) )
		{
			break;
		}
		if( st_sync.nLdUldSateyflag == LD_MOVING_SAFETY )
		{
			break;
		}

		if( Move_MakeSafetyPosBeforeWork(POS_MOV_PICK) == RET_GOOD )
		{
			m_motPickFlag[0] = CTL_NO;
			m_motPickFlag[1] = CTL_NO;
			mn_Pick_Step = 2100;
		}
		break;

	case 2100:
		//st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] �� ��ġ�� �´��� ���� ����.
		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWorkSite, WORK_PICK, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWorkSite, WORK_PICK, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);
		if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD  )
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1203, dWARNING, Func.m_strAlarmCode);
			break;
		}
		
		nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate );
		nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate );

		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_motPickFlag[0] = CTL_YES;
		}
		if( nRet_2 == BD_GOOD )
		{
			m_motPickFlag[1] = CTL_YES;
		}
		if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
		{
			m_bDvcWaitChk_Falg =  false;
			mn_Pick_Step = 2200;
		}
		else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY)
		{
			mn_Pick_Step = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1204, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 2100;
		}

	case 2200:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			mn_Pick_Step = 900;
			break;
		}

		mn_Pick_Step = 3000;
		break;

	case 2200:
		Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
		mn_Pick_Step = 2300;
		break;

	case 2300:
		nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_OFF );
		if( nRet_1 == RET_GOOD )
		{
			mn_Pick_Step = 3000;
		}
		else if( nRet_1 == RET_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1208, dWARNING, alarm.mstr_code);
		}
		break;

	case  3000:
		nRet_1 = FAS_IO.get_out_bit(st_io_info.o_Transfer1_Clamp_Forward_Sol,	IO_OFF); 
		nRet_2 = FAS_IO.get_out_bit(st_io_info.o_Transfer1_Clamp_Backward_Sol,	IO_ON);
		nRet_3 = FAS_IO.get_in_bit( st_io.i_Transfer1_Clamp_Forward_Check, IO_OFF );
		nRet_4 = FAS_IO.get_in_bit( st_io.i_Transfer1_Clamp_Backward_Check, IO_ON );
		nRet_5 = FAS_IO.get_in_bit( st_io.i_Transfer1_Device_Check, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_ON && nRet_3 == IO_OFF && nRet_4 == IO_ON && nRet_5 == IO_OFF )
		{  //�׸��۰� ������ ���� ������ ���翡 �������� �ֱ⶧���� �̸� ������ ������ �Ѵ� 
			mn_Pick_Step = 2200;
			break;
		}

		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], st_motor[m_nRobot_X].mn_allow);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], st_motor[m_nRobot_Y].mn_allow);
		if( nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			mn_Pick_Step = 2000;
			break;
		}

		if(nWorkSite == THD_LDULD_CARRIER_BUFF )
		{
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKTOP_POS + m_n_FirstTray_Y_Num];
		}
		else
		{
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{ 
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			mn_Pick_Step = 3100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Pick_Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1210, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 3000;
		}
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{ 			 
			mn_Pick_Step = 3110;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Pick_Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1211, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 3000;
		}		
		break;

	case 3110:
		nRet_1 = Chk_Loader_Transfer_Clamp_OnOff(IO_OFF); 
		if(nRet_1 == RET_GOOD )
		{
			mn_Pick_Step = 3200;
		}
		else if(nRet_1 == RET_ERROR || nRet_2 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1212, dWARNING, m_strAlarmCode);
			mn_Pick_Step = 3000;
		}
		break;

	case 3200:
		Set_Loader_Transfer_Clamp_OnOff( IO_ON );
		mn_Pick_Step = 3210;
		break;

	case 3210:
		nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_ON);
		if(nRet_1 == RET_GOOD )
		{
			mn_Pick_Step = 3300;
		}
		else if( nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 3200;
		}
		break;

	case 3300:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io. i_Loader_Transfer_Clamp_Off_Check, IO_OFF, IO_STABLE_WAIT, 1000); //IO_STABLE_WAIT == 100ms
		if(nRet_1 == RET_GOOD )
		{
			if( st_handler.bLoaderOnFlag == CTL_NO )
			{
				mn_Pick_Step = 3400;
			}
			else
			{
				m_nPickRetry_Cnt++;
				if( m_nPickRetry_Cnt < 3)
				{
					mn_Pick_Step = 3210;
					break;
				}
				m_nPickRetry_Cnt = 0;
				//900200 0 0 "REMOVE DEVIC. INPUT DEVUCE IS MUCH MORE THAN PRODUCT QTY."
				CTL_Lib.Alarm_Error_Occurrence(1250, dWARNING, _T("900200") );
			}
		}
		else if(nRet_1 == RET_PROCEED)
		{
			break;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC )
			{
				mn_Pick_Step = 3400;
				break;
			}
			if( st_handler.bLoaderOnFlag == CTL_YES )
			{
				mn_Pick_Step = 3400;
				/*nFuncRet = RET_TRAY_NOT_FIND;*/
				break;
			}
			//800113 0 A "LOADER_TRANSFER_CLAMP_OFF_����_OFF_CHECK_ERROR.
			m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io_info.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(1214, dWARNING, m_strAlarmCode);
		} 
		break;

	case 3400:
		if(nWorkSite == THD_LDULD_CARRIER_BUFF)
		{
			mn_Pick_Step = 4000;
		}
		else
		{
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "There is no PickupPos(CARRIER_BUFF) at Process_DVC_Pickup Run_UldPicker  = %d", mn_Pick_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				st_work.mn_run_status = dSTOP;
			}
			break;
		} 
		break;
	
	case  4000:
		m_dpTargetPosList[2] = st_motor[m_nRobot_Z].d_pos[P_UNLOADER_TRANSFER_Z_INIT_POS];
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{ 
			Set_Loader_Transfer_Clamp_OnOff(IO_ON); //������ ��Ŀ�� �ø���  
			mn_Pick_Step = 4100 ; //�ð� ������ ���� üũ�� �������� 
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Pick_Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1217, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 4000;
		}		
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK,m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{ 
			m_bDvcWaitChk_Falg = false;
			mn_Pick_Step = 5000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Pick_Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1218, dWARNING, alarm.mstr_code);
			mn_Pick_Step = 4000;
		}		
		break;

	case 5000:
		nRet_1 = Func.Check_PickerStatus(1, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);

		if (nRet_1 != RET_GOOD) //
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;

				CTL_Lib.Alarm_Error_Occurrence(1218, dWARNING, Func.m_strAlarmCode);
			}
		}

		for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
		{
			if(nWorkSite == THD_LDULD_CARRIER_BUFF)
			{
				if(i >= 0 && m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num) < st_recipe.nTrayY)
				{
					m_npPicker_YesNo[i] = CTL_NO; //�۾��� ��Ŀ ���� Ŭ����

					//�Ʒ� �Լ����� ������� ��ġ ������ �����Ѵ�
					m_npSet_WorkPosYXCPB[0]  = m_npFindWorkPosYXCPB[0]; //tray X Pos ��ġ
					m_npSet_WorkPosYXCPB[1]  = m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num); //tray Y Pos ��ġ
					m_npSet_WorkPosYXCPB[3]  = i; //picker ��ġ ����

					Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, m_nRobotSite, nWork_Site, m_npSet_WorkPosYXCPB); //��Ŀ �� Ʈ���� ���� ���� �Լ�	
					
					if (m_strLotNo[1] == _T(""))
					{
						m_strLotNo[1]	= st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
						m_strPartNo[1] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					}
				}	
			}
			else
			{
				break;
			}
		}

		if (nRet_1 == RET_GOOD) // 1000 step���� ������ ��Ŀ ������ �°� ��� ������ ������ ��� ��
		{
			//ȭ�� ���� ����
			m_nPickRetry_Cnt = 0;

			m_bDvcWaitChk_Falg = false;

			// jtkim 20150216 conveyor/loader tray ȭ�� ǥ��
			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_LD_PICKER_DISPLAY, 0);
				st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_LD_TRAY_DISPLAY, 0);
				st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_RETEST_BUF_DISPLAY, 0);
			}

			mn_Pick_Step = 6000;
		}
		else if (nRet_1 == RET_ERROR) //������ ��Ŀ�� �Ѱ��� ����� ���� �������� �̰����� �̵�
		{
			m_nPickRetry_Cnt++;
			CTL_Lib.Alarm_Error_Occurrence(1220, dWARNING, Func.m_strAlarmCode);
			//kwlee 2017.0214
			//mn_Pick_Step = 0;
		}
		break;

	case 6000:
		nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);
		if (nRet_1 == RET_GOOD) //
		{
			nFuncRet = RET_GOOD;
			mn_Pick_Step = 0;
		}
		else //if(nRet_1 == RET_ERROR)
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(1221, dWARNING, Func.m_strAlarmCode);
			}
		}
		break;

	default :
		if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Pick_Step at Process_DVC_Pickup Run_UldPicker  = %d", mn_Pick_Step);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		break;
	}

	return nFuncRet;
}

int CRun_UldPicker::Process_DVC_Place( int nMode, int nWorkSite, CString strLotNo)//THD_UNLD_ALIGN_BUFF
{
	int nFuncRet = RET_PROCEED;
	int nRet_1, nRet_2, i, nFlag, nTemp;

	Func.ThreadFunctionStepTrace(37, mn_Place_Step);

	switch(mn_Place_Step)
	{
	case 0:
		m_bDvcWaitChk_Falg =  false;
		m_nPickRetry_Cnt = 0;
		mn_Place_Step = 100;
		break;

	case 100:
		m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			mn_Place_Step = 200;
		}
		else
		{
			mn_Place_Step = 1000;
		}
		break;

	case 200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor_info[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 1000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1230, dWARNING, alarm.mstr_code);
			mn_Place_Step = 200;
		}
		break;

	case 1000:
		nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus); //���� ������ ���� ���� üũ 
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false; //��Ŀ ������ ���� ���°� �����̴�
		}
		else// if(nRet_1 == RET_ERROR)
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(1231, dWARNING, clsFunc.m_strAlarmCode);
				break;
			}
		}
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[2] > st_motor_info[m_nRobot_Z].d_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			mn_Place_Step = 200;
			break;
		}
		m_n_FirstTray_Y_Num = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
		for(i = 0; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES)//��Ŀ�� ���簡 ��� �ִٴ� ������ ����
			{
				m_n_FirstPicker_Num = i;
				i = m_nPickerPara;
			}
		}
		if(m_n_FirstPicker_Num == -1)
		{
			//��Ŀ�� ���̻� �۾��� ���� ����
			mn_Place_Step = 0;
			nFuncRet = RET_PICKER_NOT_FIND;
			break;
		}

		nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, nWork_Site, CTL_NO, m_npFindWorkPosYXCPB, THD_UNLOAD_WORK_RBT, m_strFindLotNo); //���������ۿ����� ���۾������� �ں��� ���´� (4 -> 1 ����) 
		if(nRet_1 == RET_GOOD) //�۾� ������ ���簡 �ִ�
		{
			m_n_FirstTray_X_Num = m_npFindWorkPosYXCPB[0]; m_n_FirstTray_Y_Num = m_npFindWorkPosYXCPB[1]; 
			//st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos �� ��ġ�� m_n_FirstTray_Y_Num�� �´��� Ȯ���ʿ�
		}
		else if(nRet_1 == RET_SKIP) //���̻� ���簡 ����
		{	//��� ������ �� ����
			mn_Place_Step = 0;
			nFuncRet = RET_TRAY_NOT_FIND;
			break;
		}
		nFlag = 0;
		if(m_n_FirstPicker_Num != -1 && m_n_FirstTray_Y_Num != -1 && m_n_FirstTray_X_Num != -1) //�۾��� ������ �� ������ ���� �۾��� �����Ѵ�
		{
			for(i = 0;  i < m_nPickerPara; i++)  //m_n_FirstPicker_Num; y < m_mPickerPara; y++)
			{
				m_npPicker_YesNo[i] = CTL_NO; //�۾��� ��Ŀ���� clear
				if(i < m_n_FirstPicker_Num) continue;

				if( nWork_Site == THD_UNLD_ALIGN_BUFF )
				{
					if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES)// && st_picker[m_nRobotSite].st_pcb_info[i].nEnable == CTL_YES)
					{
						if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe_info.nTrayY)
						{
							if(st_buffer_info[nWork_Site].st_pcb_info[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)][m_n_FirstTray_X_Num].nYesNo == CTL_NO)
							{
								m_npPicker_YesNo[i] = CTL_YES;
								nFlag++;
							}
						}
					} 
				}
				else
				{
					if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
					{
						sprintf(st_msg.c_abnormal_msg, "There is no Place_Step at Process_DVC_Place Place_Step = %d", Place_Step);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					Func.OnSet_IO_Port_Stop();
					break;
				}
			}
			if(nFlag == 0) //�۾��� ��Ŀ�� �ϳ��� �غ�� �ʾҴ�.
			{ //�̺κ��� �ƿ��� �ȵ� 
				mn_Place_Step = 0;
				nFuncRet = RET_PICKER_NOT_FIND;//��Ŀ�� ���̻� �۾��� ���� ����
				break;
			}

			//���� �۾� ��ġ�� ����
			m_npFindWorkPosYXCPB[0] = m_n_FirstTray_Y_Num; m_npFindWorkPosYXCPB[1] = m_n_FirstTray_X_Num; m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;

			m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;
			mn_Place_Step = 2000;
		}
		break;

	case 2000:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			m_nPlace_Step = 200;
			break;
		}

		if(nWork_Site == THD_UNLD_ALIGN_BUFF)
		{
			nRet_1 = Func.Check_Robot_Buffer_Clash( THD_UNLOAD_WORK_RBT, nWork_Site, m_n_FirstTray_Y_Num, m_n_FirstPicker_Num, m_nClash_ErrorInfoStatus); //��Ŀ�� ���۵��� ���� �浹 Ȯ�� 
			if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1232, dWARNING, Func.m_strAlarmCode);		
				break;
			}
		}
		else 
		{
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Place_Step at Process_DVC_Place Place_Step = %d", mn_Place_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			Func.OnSet_IO_Port_Stop();
			break;
		}
		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWork_Site, 0, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X); 
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, 0, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y); 		
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1233, dWARNING, Func.m_strAlarmCode);
			break;
		}
		m_motPlaceFlag[0] = CTL_NO;
		m_motPlaceFlag[1] = CTL_NO;
		m_nPlace_Step = 2100;
		break;

	case 2100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate); 
		nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_motPlaceFlag[0] = CTL_YES;
		}
		if( nRet_2 == BD_GOOD )
		{
			m_motPlaceFlag[1] = CTL_YES;
		}
		if( m_motPlaceFlag[0] == CTL_YES && m_motPlaceFlag[1] == CTL_YES )
		{
			m_motPlaceFlag[0] = CTL_NO;
			m_motPlaceFlag[1] = CTL_NO;
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 2200;
		}
		else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY )
		{
			mn_Place_Step = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1234, dWARNING, alarm.mstr_code);
			mn_Place_Step = 2100;
		}
		break; 	

	case 2200:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate); 
		nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_motPlaceFlag[0] = CTL_YES;
		}
		if( nRet_2 == BD_GOOD )
		{
			m_motPlaceFlag[1] = CTL_YES;
		}
		if( m_motPlaceFlag[0] == CTL_YES && m_motPlaceFlag[1] == CTL_YES )
		{
			m_motPlaceFlag[0] = CTL_NO;
			m_motPlaceFlag[1] = CTL_NO;
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_RETRY|| nRet_2 == BD_RETRY)
		{
			mn_Place_Step = 2200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1234, dWARNING, alarm.mstr_code);
			mn_Place_Step = 2200;
		}
		break; 	

	case 2500:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 2500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, alarm.mstr_code);
			mn_Place_Step = 2500;
		}
		break;

		////////////////////////////////////////////////////////
		// �۾� �� ���縦 ���´� 
		////////////////////////////////////////////////////////
	case 3000:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow)
		{
			mn_Place_Step = 2500;
			break;
		}

		nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false; //��Ŀ ������ ���� ���°� �����̴�
		}
		else// if(nRet_1 == RET_ERROR)
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(1237, dWARNING, clsFunc.m_strAlarmCode);
				break;
			}
		}

		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], st_motor_info[m_nRobot_X].mn_allow);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], st_motor_info[m_nRobot_Y].mn_allow);
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			mn_Place_Step = 2000;
			break;
		}

		if( nWork_Site == THD_UNLD_ALIGN_BUFF )
		{//UNLOAD JIG�� ����̽� ���� ������ �´°�?
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Unloader_Transfer_Device_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io_info.i_Loading_Tr_Jig_Detect_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			} 
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_ZIGPLACE_POS];
		}
		else
		{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
			CTL_Lib.Alarm_Error_Occurrence(1244, dWARNING, "920000");
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{ 			
			mn_Place_Step = 3100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1245, dWARNING, alarm.mstr_code);
			mn_Place_Step = 3000;
		}		
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{ 			 
			mn_Place_Step = 3200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_Place_Step = 3000;
		}		
		break;

	case 3200:
		if(nWork_Site == THD_UNLD_ALIGN_BUFF)
		{
			mn_Place_Step = 3300;
		}
		else
		{
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Place_Step at Process_DVC_Place Place_Step = %d", mn_Place_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			Func.OnSet_IO_Port_Stop();
		} 
		break;

		////////////////////////////////////////////////////////////
		//Unload buffer�� clamp�� �̰����� ���� 
		//���� �ܼ�ȭ�� ���� ��ε� ������ Ŭ������ �̰����� ���� �Ѵ� 
		////////////////////////////////////////////////////////////
	case 3300:
		FAS_IO.Set_IO_OnOff(st_io.o_Transfer1_Clamp_Forward_Sol, IO_OFF);
		FAS_IO.Set_IO_OnOff(st_io.o_Transfer1_Clamp_Backward_Sol , IO_ON);
		mn_Place_Step = 3310;
		break;

	case 3310:
		nRet_1 = FAS_IO.Wait_IO_OnOff(st_io.o_Transfer1_Clamp_Backward_Sol, IO_ON, st_wait.nOffWaitTime[WAIT_PICKER_CLAMP_ON_OFF]);
		if(nRet_1 == RET_GOOD) //�۾� ������ ���簡 �ִ�
		{
			mn_Place_Step = 3320;
		}
		break;

	case 3320:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.o_Transfer1_Clamp_Forward_Sol, IO_OFF, IO_STABLE_WAIT, 1000);
		if(nRet_1 == RET_GOOD) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
		{
			mn_Place_Step = 4000;
		} 
		else if(nRet_1 == RET_PROCEED)
		{
			break;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC)
			{
				mn_Place_Step = 3400;
				break;
			}
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(1248, dWARNING, m_strAlarmCode);
		} 
		break; 

	case 3400:
		Set_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
		mn_Place_Step = 3410;
		break;

	case 3410:
		nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff(IO_OFF); 			 
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false;
			mn_Place_Step = 3420;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC)
			{
				m_bDvcWaitChk_Falg = false;
				mn_Place_Step = 3420;
				break;
			}
			CTL_Lib.Alarm_Error_Occurrence(1247, dWARNING, m_strAlarmCode);
			mn_Place_Step = 3400;
		}
		break;

	case 3420:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Transfer1_Clamp_Backward_Check, IO_ON, st_wait.nOnWaitTime[WAIT_PICKER_CLAMP_ON_OFF], st_wait.nLimitWaitTime[WAIT_PICKER_CLAMP_ON_OFF]); 
		if(nRet_1 == RET_GOOD) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
		{
			mn_Place_Step = 4000;
		} 
		else if(nRet_1 == RET_ERROR)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(1250, dWARNING, m_strAlarmCode);
		} 
		break; 

		////////////////////////////////////////////////////////////
		//�κ�Ʈ�� ���� ��ġ�� �ø��� 
		//Carrier������ ���ڸ��� �ִ� üũ�Ѵ�.
		////////////////////////////////////////////////////////////
	case 4000:
		if( nWork_Site == THD_UNLD_ALIGN_BUFF )
		{
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //�������� �̵�
			{
				mn_Place_Step = 4100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Place_Step = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
				CTL_Lib.Alarm_Error_Occurrence(1251, dWARNING, alarm.mstr_code);
				mn_Place_Step = 4000;	
			}
		}
		else
		{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
			CTL_Lib.Alarm_Error_Occurrence(1291, dWARNING, "920000");
		}
		break;

		//carrirer buffer���� ����̽� ��ġ�� �´�ġ üũ����.
	case 4010:
		//nRet_1 = FAS_IO.Chk_IO_OnOff(st_io., IO_ON, IO_STABLE_WAIT, 1000);
		nRet_1 = RET_GOOD;//
		if(nRet_1 == RET_GOOD) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
		{
			mn_Place_Step = 4020;
		} 
		else if(nRet_1 == RET_PROCEED)
		{
			break;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC)
			{
				mn_Place_Step = 4020;
				break;
			}
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(1248, dWARNING, m_strAlarmCode);
		} 
		break; 

	case 4020:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_Place_Step = 4100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1251, dWARNING, alarm.mstr_code);
			mn_Place_Step = 4000;	
		}
		break;

	case 4100:
		if( nWork_Site == THD_UNLD_ALIGN_BUFF )
		{
			m_nPlace_Step = 4200;
		}
		else
		{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
			CTL_Lib.Alarm_Error_Occurrence(1291, dWARNING, "920000");
		}
		break;

	case 4200:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // ���� Ư�� ���� ���� ��ġ ���� �Լ�
		if(m_dCurrentPos[2] < st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //���� ��ġ�� ��� ������ ����ġ ��Ų��
		{
			m_nPlace_Step = 5000;
		}
		else
		{
			m_nPlace_Step = 4020;
		}
		break;

		////////////////////////////////////////////////////////
		// �۾��� �Ϸ��� ���·� ��Ŀ ���¸� Ȯ���Ѵ�
		// ��Ŀ �����͸� ����/Ʈ���̿� data exchange �Ѵ�
	case 5000:
		nRet_1 = Func.Check_PickerStatus(1, THD_UNLOAD_WORK_RBT, CTL_NO, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
		if (nRet_1 != RET_GOOD) //
		{//���� �ð��� �ش�
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] <= 0) m_dwWaitUntil[0] = GetCurrentTime();
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(1237, dWARNING, clsFunc.m_strAlarmCode);
				break;
			}
		}

		for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES && m_npPicker_YesNo[i] == CTL_YES
				&& (m_npPicker_Vacuum_Status[i] == IO_ON && m_npPicker_OutputStatus[i] == IO_ON))
			{
				if (m_strLotNo[1] == _T(""))
				{
					m_strLotNo[1]	= st_picker[m_nRobotSite].st_pcb_info[i].strLotNo;
					m_strPartNo[1] = st_picker[m_nRobotSite].st_pcb_info[i].strPartNo;
				}				 

				if(nWork_Site = THD_UNLD_ALIGN_BUFF)
				{
					if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nLdBuffer_Num)
					{
						m_npPicker_YesNo[i] = CTL_NO;	//�۾��� ��Ŀ ���� Ŭ����

						//�Ʒ� �Լ����� ������� ��ġ ������ �����Ѵ�
						m_npSet_WorkPosYXCPB[0] = m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num); //Y Pos ��ġ
						m_npSet_WorkPosYXCPB[3]  = i; //picker ��ġ ����

						Func.Data_Exchange_PickPlace(PICKER_PLACE_MODE, 1, m_nRobotSite, nWork_Site, m_npSet_WorkPosYXCPB); //��Ŀ �� Ʈ���� ���� ���� �Լ�
					}
					//2017.0109
					clsLog.LogTransfer(_T("LOADING_ROBOT"),
						_T("MOVE_CDIMM"),
						0,
						st_picker[m_nRobotSite].st_pcb_info[i].strMaterial,
						_T("CDIMM"),
						st_picker[m_nRobotSite].st_pcb_info[i].strMovePos[0],
						st_picker[m_nRobotSite].st_pcb_info[i].strMovePos[1],
						1, 
						st_picker[m_nRobotSite].st_pcb_info[i].strLogKey,
						st_picker[m_nRobotSite].st_pcb_info[i].strLogData);
				}
				else
				{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
					CTL_Lib.Alarm_Error_Occurrence(1291, dWARNING, "920000");	
					return nFuncRet;
				}
			}
		}

		if (nRet_1 == RET_GOOD) // 1000 step���� ������ ��Ŀ ������ �°� ��� ������ ������ ��� ��
		{
			//ȭ�� ���� ����
			m_bDvcWaitChk_Falg = false;

			// jtkim 20150303 conveyor/loader tray ȭ�� ǥ��
			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_LD_PICKER_DISPLAY, 0);
				st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_LD_BUF_PICK_DISPLAY, 0);
				st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_COUNT_DISPLAY, 0);
			}
			mn_Place_Step = 6000;
		}
		else if (nRet_1 == RET_ERROR) //������ ��Ŀ�� �Ѱ��� ����� ���� �������� �̰����� �̵�
		{
			CTL_Lib.Alarm_Error_Occurrence(1255, dWARNING, clsFunc.m_strAlarmCode);
		}
		break;

	case 6000:
		nRet_1 = clsFunc.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);

		if(nRet_1 == RET_GOOD)
		{
			nFuncRet = RET_GOOD;
			mn_Place_Step = 0;
		}
		if (nRet_1 == RET_ERROR) //
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) m_dwWaitUntil[0] = GetCurrentTime();
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
			}
			CTL_Lib.Alarm_Error_Occurrence(1256, dWARNING, Func.m_strAlarmCode);
		}
		break; 


		default::
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "There is no lace_Ste at Process_DVC_Place in RunLdUldRobot  = %d", mn_Place_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			break;
	}
	return nFuncRet;
}

//nMode = 0 safety �������� �ƴ��� Ȯ���Ѵ� �׸��� �ƴϸ� �̵��Ѵ�.
//nMode = 1 reay��ġ�� �̵�
//nMode = 2 pick �κ�Ʈ�� �ϴ� safety��ġ�� �̵��Ѵ�. LOAD�κ��� ������ �� �ֵ���  //,�ε� �κ� �� unpress�κ� ��ġ�� �ľ��Ѵ�.
//nMode = 3 place �κ�Ʈ�� Pickeup�� ���� carrier�κ����� ���� ���� ����Ѵ�.
//nWorkSite - ��� ��ġ, Pick��ġ, Place��ġ
int CRun_UldPicker::Move_MakeSafetyPosBeforeWork( int nMode )
{
	int nFuncRet = RET_PEOCEED, nRet_1 = 0, nRet_2 = 2;
	double d_MotorPos[3], d_MotorLoaderY;

	d_MotorPos[0] = COMI.Get_MotCurrentPos( m_nRobot_X );
	d_MotorPos[1] = COMI.Get_MotCurrentPos( m_nRobot_Y );
	d_MotorPos[2] = COMI.Get_MotCurrentPos( m_nRobot_Z );
	d_MotorLoaderY = COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y );
	//unpress�� �Ǹ��� Ȯ���� �ؾ��ϴµ�

	Func.ThreadFunctionStepTrace(33, mn_RunSafetyStep);
	switch( mn_RunSafetyStep )
	{
	case 0:
		mn_RunSafetyStep = 100;
		break;

	case 100:
		if( m_nRobot_Z > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_X].mn_allow) )
		{
			mn_RunSafetyStep = 110;
		}
		else
		{
			mn_RunSafetyStep = 1000;
		}
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 110;
		}
		break;

	case 1000:
		if( nMode < POS_CHK_READY || nMode > POS_MOV_PLACE)
		{
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Mode at Move_MakeSafetyPosBeforeWork  RunSafetyStep= %d", mn_RunSafetyStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			st_work.mn_run_status = dSTOP;
			break;
		}
		if( nMode == POS_CHK_READY )
		{
			mn_RunSafetyStep = 2000;
		}
		else if( nMode == POS_MOV_READY)
		{
			mn_RunSafetyStep = 3000;
		}
		else if( nMode == POS_MOV_PICK)
		{
			mn_RunSafetyStep = 4000;
		}
		else//POS_MOV_PLACE
		{
			mn_RunSafetyStep = 5000;
		}
		break;

	case 2000:
		if( m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[m_nRobot_X].mn_allow) ) nFuncRet = RET_ERROR;
		else                                                                                                                                                                         	nFuncRet = RET_GOOD;
		mn_RunSafetyStep = 0;
		break;

	//1. x��ġ�� READY ��ġ���� ũ�� X,Y ���� Ready��ġ�� �̵��Ѵ�.
	//1. x��ġ�� READY ��ġ�� ���̿� ������ �ȵȴ�. Ȥ�� y��ġ�� ������ ��ġ�� ������ Y���� safety�� ���������Ѵ�.
	//x��ġ�� ready���� ũ�� �ȵȴ�,
	case 3000:
		if( m_nRobot_X < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[m_nRobot_X].mn_allow) )
		{
			mn_RunSafetyStep = 3100;
		}//üũ�غ���
		else if ( ( m_nRobot_X >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[m_nRobot_X].mn_allow) ) &&
			( m_nRobot_X < st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*st_motor[m_nRobot_X].mn_allow) ) )
		{
			mn_RunSafetyStep = 3200;
		}
		else if(  m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*st_motor[m_nRobot_X].mn_allow) ) )
		{
			mn_RunSafetyStep = 3200;
		}
		else
		{
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 6000;
		}
		break;

	case 3100:
		if( Move_ReadySafeyXY() == RET_GOOD )
		{
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 6000;
		}
		break;

	case 3200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 3300;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 3200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 3200;
		}
		break;

	case 3300:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 3300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 3300;
		}
		break;

	//POS_MOV_PICK
	case 4000:
		if( m_nRobot_X < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[m_nRobot_X].mn_allow) )
		{
			mn_RunSafetyStep = 4100;
		}//üũ�غ���
		else if ( ( m_nRobot_X >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[m_nRobot_X].mn_allow) ) &&
			( m_nRobot_X < st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*st_motor[m_nRobot_X].mn_allow) ) )
		{
			mn_RunSafetyStep = 4200;
		}
// 		else if(  m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*st_motor[m_nRobot_X].mn_allow) ) )
// 		{
// 			mn_RunSafetyStep = 4300;
// 		}
		else
		{
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 6000;
		}
		break;

	case 4100:
		if( Move_ReadySafeyXY() == RET_GOOD )
		{
			mn_RunSafetyStep = 4000;
		}
		break;

	case 4200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 4210;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 4200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 4200;
		}
		break;

	case 4210:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 4000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 4210;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 4210;
		}
		break;

	//POS_MOV_PLACE ����
	case 5000:
		if( m_nRobot_X < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] - st_motor[m_nRobot_X].mn_allow) )
		{//���������� ������Ƿ� XY, ���õ������� JIG�� �����δ�.
			m_motPickFlag[0] = CTL_NO;
			m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5500;
		}
		else if( m_nRobot_X >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[m_nRobot_X].mn_allow) &&
				  m_nRobot_X < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS] - st_motor[m_nRobot_X].mn_allow))
		{//���������̹Ƿ� Y�� ���� �����δ�.
			mn_RunSafetyStep = 5100;//��������
		}
		else //if(m_nRobot_X >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS] - st_motor[m_nRobot_X].mn_allow))
		{
			m_motPickFlag[0] = CTL_NO;
			m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5600;
		}
		break;

	case 5100:
		//ASEMBLER�ʿ� ��ġ�� �ִ°�?
		if( m_nRobot_X <  st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS]  + st_motor[m_nRobot_Y].mn_allow )
		{//X�� ���� P_UNLOADER_TRANSFER_X_INREADY_POS�� �̵�
			mn_RunSafetyStep = 5110;
		}
		else
		{//X�� ���� P_UNLOADER_TRANSFER_X_OUTREADY_POS�� �̵�
			mn_RunSafetyStep = 5150;
		}
		break;

	case 5110:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 5000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5110;
		}
		break;
		
	case 5150:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			mn_RunSafetyStep = 5000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5150;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5150;
		}
		break;

		//asembler��ġ���� Inready��ġ�� �̵��Ѵ�
	case 5500:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
		nRet_2 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_motPickFlag[0] = CTL_YES;
		}
		if(nRet_2 == BD_GOOD)
		{
			m_motPickFlag[1] = CTL_YES;
		}
		if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
		{
			m_motPickFlag[0] = CTL_NO; m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5700;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5500;
		}
		break;

		//asembler��ġ���� outready��ġ�� �̵��Ѵ�
	case 5600:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS], COMI.mn_runspeed_rate);
		nRet_2 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //�������� �̵�
		{
			m_motPickFlag[0] = CTL_YES;
		}
		if(nRet_2 == BD_GOOD)
		{
			m_motPickFlag[1] = CTL_YES;
		}
		if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
		{
			m_motPickFlag[0] = CTL_NO; m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5700;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
			CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5600;
		}
		break;

	case 5700:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], st_motor[m_nRobot_Y].mn_allow);
		if(nRet_1 != BD_GOOD)
		{
			mn_RunSafetyStep = 5000;
		}
		else
		{
			if( m_nRobot_X < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] - st_motor[m_nRobot_X].mn_allow) )
			{
				mn_RunSafetyStep = 5800;
			}
			else
			{
				nFuncRet = RET_GOOD;
				mn_RunSafetyStep = 6000;
			}
		}
		break;

	case 5800:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], st_motor[m_nRobot_Y].mn_allow);
		if(nRet_1 != BD_GOOD)
		{
			COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nRobot_X );
			COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nRobot_Y );
			mn_RunSafetyStep = 5000;
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //�������� �̵�
			{
				nFuncRet = RET_GOOD;
				mn_RunSafetyStep = 6000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunSafetyStep = 5800;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_RunSafetyStep = 5800;
			}
			break;
		}

	case 6000:
		mn_RunStep = 0;
		break;


	default:
		if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "There is no RunSafetyStep at Move_MakeSafetyPosBeforeWork in Run_UldPicker  = %d", mn_RunSafetyStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		break;
	}

	return nFuncRet;
}


int CRun_UldPicker::Move_ReadySafeyXY()
{
	int nRe_1 = 0, nRet_2 = 0;
	int nFuncRet = RET_PROCEED;

	switch( mn_MoveStep)
	{
		case 0:
			m_motFlag[0] = CTL_NO;
			m_motFlag[1] = CTL_NO;
			mn_MoveStep = 1000;
			break;

		case 1000
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS], COMI.mn_runspeed_rate);
			nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //�������� �̵�
			{
				m_motFlag[0] = CTL_YES;
			}
			if (nRet_2 == BD_GOOD) //�������� �̵�
			{
				m_motFlag[1] = CTL_YES;
			}
			if(m_motFlag[0] == CTL_YES  && m_motFlag[1] == CTL_YES)
			{
				m_motFlag[0] = CTL_NO;
				m_motFlag[1] = CTL_NO;
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1000;
			}
			break;

		case 1100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS], COMI.mn_runspeed_rate);
			nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //�������� �̵�
			{
				m_motFlag[0] = CTL_YES;
			}
			if (nRet_2 == BD_GOOD) //�������� �̵�
			{
				m_motFlag[1] = CTL_YES;
			}
			if(m_motFlag[0] == CTL_YES  && m_motFlag[1] == CTL_YES)
			{
				mn_RunSafetyStep = 1200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunSafetyStep = 1100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//���� �˶��� �̹� ó�������� �̰������� �� ���¸� �ٲٸ� �ȴ�
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_RunSafetyStep = 1100;
			}
			break;

		case 1200:
			m_motFlag[0] = CTL_NO;
			m_motFlag[1] = CTL_NO;
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 0;
			break;
	}
	return nFuncRet;
}

void CRun_UldPicker::Set_UnLoader_Transfer_Clamp_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	FAS_IO.set_out_bit( st_io.o_Transfer1_Clamp_Forward_Sol, nOnOff);
	FAS_IO.set_out_bit( st_io.o_Transfer1_Clamp_Backward_Sol, !nOnOff);

	if (nOnOff == IO_ON)
	{
		clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("FORWARD"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT),_T("BACKWARD"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_UldPicker::Chk_UnLoader_Transfer_Clamp_OnOff( int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_PICKER_CLAMP_ON_OFF;

	if (nOnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false &&	FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_ON)	== IO_ON &&
			FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_OFF)	== IO_OFF )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_ON)	== IO_ON &&
			FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_OFF)	== IO_OFF )
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
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("BACKWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Transfer1_Clamp_Backward_Check); 
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("BACKWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_ON)	== IO_ON )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && FAS_IO.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_OFF)	== IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_ON)	== IO_ON )
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
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Transfer1_Clamp_Forward_Check); 
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}