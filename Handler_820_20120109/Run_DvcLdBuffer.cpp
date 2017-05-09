// Run_DvcLdBuffer.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_DvcLdBuffer.h"
#include "LogFromat.h"
#include "AMTLotManager.h"
#include "Run_LdPicker.h"
#include "FastechPublic_IO.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_DvcLdBuffer
CRun_DvcLdBuffer Run_DvcLdBuffer;
IMPLEMENT_SERIAL(CRun_DvcLdBuffer, CObject, 1);

CRun_DvcLdBuffer::CRun_DvcLdBuffer()
{
}

CRun_DvcLdBuffer::~CRun_DvcLdBuffer()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_DvcLdBuffer message handlers
void CRun_DvcLdBuffer::Thread_Run()
{
	switch( st_work.mn_run_status)
	{
	case dINIT:
		break;

	case dRUN:
		OnRunMove();
		break;

	case dSTOP:
	

		m_dwWaitTime[0] = GetCurrentTime();
		break;

	case dWARNING:
		break;

	default:
		break;
	}
}

void CRun_DvcLdBuffer::OnRunInit()
{

}

void CRun_DvcLdBuffer::OnRunMove()
{
	int i = 0, nRet_1, nCount = 0;

	Func.ThreadFunctionStepTrace(5, mn_RunStep);
	switch(mn_RunStep)
	{
	case -1:
		mn_RunStep = 0;
		break;

	case 0:
		if( g_lotMgr.GetLotCount() > 0 )
		{
			mn_RunStep = 100;
		}		
		break;

	case 100:
		st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CLEAR; //clear

		mn_RunStep = 200;
		break;

	case 200:
		mn_RunStep = 300;	
		break;

	case 300:
		for( i = 0; i < st_recipe.nLdBuffer_Num; i++ )
		{
			m_npBuff_Info[i] = CTL_YES;			 
		}
		Set_Loader_Buffer_Align_OnOff(IO_OFF);
		for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
		{
			st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo = CTL_NO;
			m_npBuff_Info[i] = CTL_NO;			 
		}
		mn_RunStep = 400;
		break;

	case 400:
		nRet_1 = Chk_Loader_Buffer_Align_OnOff( IO_OFF );

		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 500;
		}
		else if(nRet_1 == RET_ERROR)
		{			 
			CTL_Lib.Alarm_Error_Occurrence(5001, dWARNING, m_strAlarmCode);
			mn_RunStep = 300;
		}
		break;

	case 500:
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( ( g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES" ) && g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{
				//load plate�� ���� ��û
				st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
				st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;
				m_nFindLotNo_Flag = 0;
				mn_RunStep = 900;
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
				{
					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;

					m_nFindLotNo_Flag = 1;
					mn_RunStep = 900;
				}
				else
				{
					return;
				}
			}
		}
		break;

	case 900:
		mn_RunStep = 1000;
		break;


		////////////////////////////////////////////////////////////////////////////
		//1. �ε� ���۴� CarrierRbt�κ�Ʈ�� ��û�� �����ϸ鼭 �����δ� 
		//2. Carrier�� �̵��ؾ� �ϴ��� �Ǵ� �������� �ִ��� �ε���ä���� ��ε������ε� üũ�ϸ鼭 ���縦 ��û
		//3. �ε� ���縦 �ε��� Ŭ������ off�� ���¸� üũ�Ѵ�.
		//4. Ȥ�� ��ε��ϴ� ���簡 �ִ���, ���� �ε� ��ε��� �ʿ����� üũ�Ѵ�.
		//    �����߿� �ε� ��ε尡 ����, �ε��� ���� �̸� ����� �����ϴ°� ����.
		////////////////////////////////////////////////////////////////////////////
	case 1000:
		if( st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LOAD_WORK_RBT] == LOTEND)
		{
			st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_NO;
			st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = CTL_NO;
			st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_ALIGN_BUFF] = LOTEND;
			mn_RunStep = 0;
		}
		else if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && 
			st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_PLACE)  
		{
			mn_RunStep = 1100; 
		}
		//UNLOAD Picker ����
// 		else if(st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_DVC_UNLOAD)  
// 		{
// 			mn_RunStep = 1200; 
// 		}
		break;

	case 1100:
		nRet_1 = Func.Check_BufferStatus(100, THD_LD_ALIGN_BUFF, CTL_NO, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus); //���۴� ������ ����� �־�� �Ѵ� 
		if(nRet_1 == RET_GOOD)
		{
			//���� ����, ó������ work robot ����Ʈ�� �̵��ϴ� ��ƾ����, �ε� ���ۿ��� ���簡 �ϳ��� ����� �Ѵ� 
			mn_RunStep = 2000; 
		}
		else if(nRet_1 == RET_ERROR)
		{//���簡 �����ִٸ�, ���� ���������� �����ִ� �����̴� �̋��� �ך� �κ��� ��û�� ��� �ٷ� ���� �� �ְ� ó���Ѵ�  

			mn_RunStep = 3000; 
			//CTL_Lib.Alarm_Error_Occurrence(5002, dWARNING, Func.m_strAlarmCode);
		}
		break;

	case 1200://�̋� ���۴� ������ ����� �־�� �Ѵ� 
		nRet_1 = Func.Check_BufferStatus(100, THD_LD_ALIGN_BUFF, CTL_NO, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus); //���۴� ������ ����� �־�� �Ѵ� 
		if(nRet_1 == RET_GOOD)
		{
			//���� ����, 
			mn_RunStep = 6000; //test robot�� �����ִ� �ε� ���� ���� ���縦 ���� �����̴�. �� ���縦 ���� �׽�Ʈ ����Ʈ�� �̵��Ͽ� �۾��Ѵ�
		}
		else if(nRet_1 == RET_ERROR)
		{//���簡 �����ִٸ� ���� 
			CTL_Lib.Alarm_Error_Occurrence(5003, dWARNING, Func.m_strAlarmCode);
		}			 
		break;

	case 2000:
		m_strFindLotNo = "0";//LotNo�� �Ű澲�� �ʴ´�. LDPicker�� �Ű澴��
		nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LD_ALIGN_BUFF, CTL_NO, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strFindLotNo);
		if(nRet_1 == RET_GOOD) //�۾� ������ ���簡 �ִ�, ���� �۾��� �ڿ���(0��) ���� ���´� 
		{
			m_nWork_Pos = m_npFindWorkPosYXCPB[0]; 
		}
		else if(nRet_1 == RET_SKIP) //���̻� ���� ������ ���� 
		{	//��� ������ �� ����
			mn_RunStep = 1000;	 
			break;
		}
// 		//work robot�� ���� ������ ��û 
// 		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ; //�ε� ���� ��û
// 		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE; //���� ���� �۾�
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY;
		mn_RunStep = 2100;
		break;

	case 2100:
		//LD_WORKRBT	st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY
		if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_CHANGE)//���縦 �κ�Ʈ�� �ε����ۿ� ��� ä�� ���� ����
		{
			nCount = 0;
			m_strLotNo = "";
			m_strPartNo = "";
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{ //������ ���� ������ �����Ѵ�
				m_npBuff_Info[i] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo; //������ ���� ������ �����Ѵ�
				if( m_npBuff_Info[i] == CTL_YES && m_strLotNo == "")
				{
					m_strLotNo = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].strLotNo;
					m_strPartNo = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].strPartNo;
				}
				nCount++;
			}

			if(nCount > 0)
			{
				mn_RunStep = 2200;

				m_dwCycleTime[1] = GetTickCount();
				m_dwCycleTime[2] = m_dwCycleTime[1] - m_dwCycleTime[0];

				if (m_dwCycleTime[2] <= (DWORD)0)
				{
					m_dwCycleTime[0] = GetTickCount();
				}
				else
				{
					if( m_strLotNo != _T("") && m_strPartNo != _T("") )
					{
						Func.OnCycleTime(1, 
							m_strLotNo,
							m_strPartNo,
							m_dwCycleTime[2], 
							0, 
							0);
					}

					m_dwCycleTime[0] = GetTickCount();
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////
				st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount = 0;
				st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial = _T(""); 
				for (i=0; i<st_recipe.nLdBuffer_Num; i++)
				{
					if (st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo == YES)
					{
						st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].strSerialNo;
						break;
					}
				}
				for (i=0; i<st_recipe.nLdBuffer_Num; i++)
				{
					if (st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo == YES)
					{
						st_buffer_info[THD_LD_ALIGN_BUFF].strLogKey[st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount].Format(_T("%02d"), i + 1);
						st_buffer_info[THD_LD_ALIGN_BUFF].strLogData[st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].strSerialNo;
						st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount++;
					}						
				}

				clsLog.LogTransfer(_T("LOADING_BUFFER"), 
					_T("ALIGN_BUFFER"), 
					0, 
					st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial,
					_T("CDIMM"), 
					_T("PLCE_BUFFER"), 
					_T("CARRIER_SITE"),
					st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount,
					st_buffer_info[THD_LD_ALIGN_BUFF].strLogKey,
					st_buffer_info[THD_LD_ALIGN_BUFF].strLogData);

			}
			else
			{//���簡 �ϳ��� ���� ���� 
				mn_RunStep = 0;
				if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
				{
					sprintf(st_msg.c_abnormal_msg, "There is no DVC in Load buffer step  = %d", mn_RunStep);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}
			}
		}
		break;

	case 2200:
		Set_Loader_Buffer_Align_OnOff(IO_ON);
		mn_RunStep = 2300;
		break;

	case 2300:
		nRet_1 = Chk_Loader_Buffer_Align_OnOff( IO_ON );

		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 2310;
		}
		else if(nRet_1 == RET_ERROR)
		{			 
			CTL_Lib.Alarm_Error_Occurrence(5004, dWARNING, m_strAlarmCode);
			mn_RunStep = 2200;
		}
		break;

	case 2310:
		//���� üũ
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
		{
			break; 
		} 
		else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Tr_Jig_Detect_Check);
			CTL_Lib.Alarm_Error_Occurrence(5005, dWARNING, m_strAlarmCode);
			break;
		} 
		mn_RunStep = 2320;
		break;

	case 2320:
		Set_Loader_Buffer_Align_OnOff(IO_OFF);
		mn_RunStep = 2330;
		break;

	case 2330:
		nRet_1 = Chk_Loader_Buffer_Align_OnOff( IO_OFF );

		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 2340;
		}
		else if(nRet_1 == RET_ERROR)
		{			 
			CTL_Lib.Alarm_Error_Occurrence(5006, dWARNING, m_strAlarmCode);
			mn_RunStep = 2320;
		}
		break;

	case  2340:
		//���� üũ
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
		{
			break; 
		} 
		else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Tr_Jig_Detect_Check);
			CTL_Lib.Alarm_Error_Occurrence(5007, dWARNING, m_strAlarmCode);
			break;
		} 
		mn_RunStep = 2400;
		break;

	case 2400:
		nRet_1 = Func.Check_BufferStatus(0, THD_LD_ALIGN_BUFF, CTL_YES, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus);
		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 2500;

			clsLog.LogTransfer(_T("LOADING_BUFFER"), 
				_T("MOVE_BUFFER"), 
				1, 
				st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial,
				_T("CDIMM"), 
				_T("PICK_BUFFER"), 
				_T("TEST_SITE"),
				st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount,
				st_buffer_info[THD_LD_ALIGN_BUFF].strLogKey,
				st_buffer_info[THD_LD_ALIGN_BUFF].strLogData);
		}
		else if(nRet_1== RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(5008, dWARNING, Func.m_strAlarmCode);
		} 
		break;

	case 2500:
		//work robot�� ���� PICKUP�� ��û 
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ; //�ε� ���� ��û
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PICK; //���� �Ⱦ�
		mn_RunStep = 3000;
		break;

	case 3000:
		//LOAD RBT�� PICKUP�� �Ϸ���.
		if( st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0]== CTL_CHANGE )
		{
			mn_RunStep = 3100;
		}
		break;

	case 3100:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //������ �ȵǵ� ����ȭ �ð������� ��ٸ��� 
		{
			break; 
		} 
		else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loading_Tr_Jig_Detect_Check);
			CTL_Lib.Alarm_Error_Occurrence(5009, dWARNING, m_strAlarmCode);
			break;
		} 
		mn_RunStep = 3200;
		break;

	case 3200:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Align_Backward_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) 
		{
			break; 
		} 
		else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
		{
			mn_RunStep = 3210;
			break;
		}
		mn_RunStep = 3300;
		break;

	case 3210:
		Set_Loader_Buffer_Align_OnOff(IO_OFF);
		mn_RunStep = 3220;
		break;

	case 3220:
		nRet_1 = Chk_Loader_Buffer_Align_OnOff( IO_OFF );

		if(nRet_1 == RET_GOOD)
		{
			mn_RunStep = 3300;
		}
		else if(nRet_1 == RET_ERROR)
		{			 
			CTL_Lib.Alarm_Error_Occurrence(5010, dWARNING, m_strAlarmCode);
			mn_RunStep = 3210;
		}
		break;

	case 3300:
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CLEAR;		
		mn_RunStep =0;
		break;
		

	default:
		if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_DVCLdBuffer  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		break;
	}

}


void CRun_DvcLdBuffer::Set_Loader_Buffer_Align_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Loader_Align_Forward_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Loader_Align_Backward_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("LD_DVCBUFFER_ROBOT"),_T("FORWARD"),0,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("LD_DVCBUFFER_ROBOT"),_T("BACKWARD"),0,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_DvcLdBuffer::Chk_Loader_Buffer_Align_OnOff( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_BUFFER_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_ON) == IO_ON )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_ON) == IO_ON )
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
				clsLog.LogFunction(_T("LD_DVCBUFFER_ROBOT"),_T("BACKWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loader_Align_Forward_Check); 
				clsLog.LogFunction(_T("LD_DVCBUFFER_ROBOT"),_T("BACKWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_OFF)	== IO_OFF )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_OFF) == IO_OFF )
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
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loader_Align_Forward_Check); 
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}
