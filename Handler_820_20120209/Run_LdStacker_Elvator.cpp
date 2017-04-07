// Run_LdStacker_Elvator.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_LdStacker_Elvator.h"
#include "AMTLotManager.h"
#include "FastechPublic_IO.h"
#include "LogFromat.h"
#include "IO_Manager.h"

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
	if (st_sync.n_init_flag[THD_LD_STACKER] != INIT_CLEAR)		return;		//INIT_CLEAR �϶��� �ʱ�ȭ �۾��� �Ѵ�. �ʱ�ȭ�� ������ INIT_CLEAR -> INIT_READY�� �Ǳ� ������...

	switch( mn_InitStep )
	{
	case -1:
		if( st_sync.n_init_flag[THD_WORK_TRANSFER] < INIT_READY ) break;
		mn_InitStep = 0;

	case 0:
		break;
	}
}

void CRun_LdStacker_Elvator::RunMove()
{
	int nRet_1,nRet_2;

	switch( mn_RunStep )
	{
	case -1:
		mn_RunStep = 0;
		break;

	case 0:
		m_nFindLotNo_Flag = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{
				//load plate�� ���� ��û
				m_nFindLotNo_Flag = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
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
		if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
		{
			m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
			CTL_Lib.Alarm_Error_Occurrence(6060, dWARNING, m_strAlarmCode);
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
			CTL_Lib.Alarm_Error_Occurrence(6050, dWARNING, alarm.mstr_code);
			mn_RunStep = 300;
		}
		break; 

	case 400:
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
		break;

	case 1100:
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		if( m_dwWaitTime[2] <= 0 ) m_dwWaitTime[0] = GetCurrentTime();
		nRet_1 = Ready_Stacker_Move_Check(1);
		if( 	nRet_1 == RET_GOOD )
		{
			st_tray_info[THD_LD_STACKER].nTrayExist = CTL_YES; //Ʈ�������� ���� ��
			mn_RunStep = 1200;
		}
		else
		{
			if( m_dwWaitTime[2] > 15000 )
			{
				//�������
				sprintf(st_msg.c_abnormal_msg, "[LDSTACKER_ELV] There is no tray in load stacker elevator. Please check sensor.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				m_dwWaitTime[0] = GetCurrentTime();
			}
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
			CTL_Lib.Alarm_Error_Occurrence(6120, dWARNING, m_strAlarmCode);
		}
		break;

	case 1300:
		nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
		if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
		{
			mn_retry_cnt++;
			if (mn_retry_cnt > 2)
			{
				mn_retry_cnt = 0;
				m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(6110, dWARNING, m_strAlarmCode);
			}
			mn_retry_cnt = 1310;
			break;
		}

		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_OFF); //plate off üũ  
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON); //plate off üũ			
		if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			mn_RunStep = 2000;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else						 m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(6120, dWARNING, m_strAlarmCode);
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
		if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
		{
			mn_retry_cnt++;
			if (mn_retry_cnt > 2)
			{
				mn_retry_cnt = 0;
				m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(6110, dWARNING, m_strAlarmCode);
			}
			mn_retry_cnt = 1310;
			break;
		}

		//Tray lock cyclinder üũ �ʿ�
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
		if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			mn_RunStep = 2100;
		}
		else
		{
			if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
			else						 m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(6120, dWARNING, m_strAlarmCode);
		}
		break;

	case 2100://6200~6300
		nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET);
		if(nRet_1 == RET_GOOD)
		{
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_ON); //load stacker tray check    
			nRet_2 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == IO_OFF && nRet_2 != BD_GOOD) //Ʈ���̰� ����Ŀ�� ���� ����, SD ������ �������� ���� ����, Ʈ���̰� ���� ����  
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
		if(nRet_1 == BD_ERROR) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
		{
			m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
			CTL_Lib.Alarm_Error_Occurrence(6140, dWARNING, m_strAlarmCode);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		//load stacker elevator�� Ʈ���� ������ load plate�� ����Ʈ �Ѵ� 
		Func.Handler_Tray_DataInfo_Shift(0, CTL_YES, THD_LD_STACKER, THD_LD_TRAY_PLATE);  
		//��� Ʈ���̸� �����ϵ� �ݺ��Ǿ�� �ϴ�, Ʈ���� �����Ѥ� ���ø� �ϰ� Ʋ���������ʴ´� 			
		//////////////////////////////////////////////////////////////////////////

		st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_READY;
		mn_RunStep = 3000;
		break;

	case 3000:
		mn_RunStep = 1000;
		break; 

	case 5000://7000
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_OFF);
		if(nRet_1 == IO_ON)
		{	
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Exist_Check); 			
			CTL_Lib.Alarm_Error_Occurrence(6150, dWARNING, m_strAlarmCode);
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);   //������ �̵��Ѵ� 
		if (nRet_1 == BD_GOOD) //���� �̵� �ϸ鼭 SD�� ã�´�  
		{		
			mn_RunStep = 5010;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			mn_RunStep = 5000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{ 
			CTL_Lib.Alarm_Error_Occurrence(6150, dWARNING, alarm.mstr_code);

			if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2000"));
				//st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			mn_RunStep = 5000;
		}
		break;

	case 5010:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //2015.0407 james  //SD ��ġ���� �̵��ߴµ� SD�� �������� ������ Ʈ���̰� ���°��̴�
		if (nRet_1 == BD_GOOD)   
		{	 
			mn_RunStep = 5100;  			 
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			mn_RunStep = 5010;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{ 
			CTL_Lib.Alarm_Error_Occurrence(6160, dWARNING, alarm.mstr_code);
			mn_RunStep = 5010;
		}
		break;

	case 5100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_OFF); //stacker tary �� ����� �Ѵ�  
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Ready_Check, IO_OFF);
			if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
			{
				st_tray_info[THD_LD_STACKER].nTrayExist = CTL_NO; //Ʈ�������� ���� Ŭ����
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
			{//Ʈ���� ���簡 �����Ѵ�  		

				if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo )
				{
					if( g_lotMgr.GetLotAt( m_nFindLotNo_Flag ).GetStrLastModule() == "YES" )
					{
						mn_RunStep = 0;
						break;
					}
				}

				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Exist_Check); 				 
				CTL_Lib.Alarm_Error_Occurrence(6180, dWARNING, m_strAlarmCode);
			}			 
			break;

	default:
		if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_LdStacker_Elvator  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		break;
	}
}


int CRun_LdStacker_Elvator::Ready_Stacker_Move_Check( int mode )
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

	nRet_1 = FAS_IO.Chk_IO_OnOff( st_io.i_loading_stacker_tray1_type_chk , OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	nRet_2 = FAS_IO.Chk_IO_OnOff( st_io.i_loading_stacker_tray2_type_chk , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	if( mode == 0 )//�Ʒ��� ���� ��� 
	{
		nRet_3 = FAS_IO.Chk_IO_OnOff( st_io.i_Loading_Stacker_Tray_On_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
		nRet_4 = FAS_IO.Chk_IO_OnOff( st_io.i_Loading_Stacker_Tray_Ready_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );
	}
	else
	{
		nRet_3 = IO_ON;
		nRet_4 = IO_ON;
	}
	nRet_5 = FAS_IO.Chk_IO_OnOff( st_io.i_Loading_Stacker_Tray_Exist_Check , !OnOff, IO_STABLE_WAIT, IO_STABLE_LIMIT );

	if( (nRet_1 == RET_PROCEED || nRet_2 == RET_PROCEED ) && st_basic.n_mode_device != WITHOUT_DVC ) 
	{
		nFuncRet = RET_GOOD;
	}
	else if( ( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR) && st_basic.n_mode_device != WITHOUT_DVC)
	{
		if( nRet_1 == RET_ERROR  )
		{
			m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_loading_stacker_tray1_type_chk);
		}
		else
		{
			m_strAlarmCode.Format(_T("8%d%04d"), !OnOff, st_io.i_loading_stacker_tray2_type_chk);
		}
		nFuncRet = RET_ERROR;		
	}
	else
	{
		//nFuncRet = RET_GOOD;
	}
	return nFuncRet;
}


void CRun_LdStacker_Elvator::Set_Tray_Guide_Clamp_ForBackward(int OnOff)
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

//������ ���� sol ��½ð���ŭ ������
int CRun_LdStacker_Elvator::Chk_Tray_Guide_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON)	== IO_ON )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true &&	g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON)	== IO_ON )
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
				clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("UNLOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON)	== IO_ON )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check, IO_ON)	== IO_ON )
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
				clsLog.LogFunction(_T("LD_TRAY_STACKER_ELEVATOR"),_T("LOCK"),1,_T("TRAY"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}
