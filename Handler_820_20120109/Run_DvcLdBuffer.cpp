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
				//load plate에 자재 요청
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
		//1. 로딩 버퍼는 CarrierRbt로보트가 요청를 시작하면서 움직인다 
		//2. Carrier가 이동해야 하는지 또는 놓을때가 있는지 로딩상채인지 언로딩상태인디 체크하면서 자재를 요청
		//3. 로딩 자재를 로딩후 클램프로 off후 상태를 체크한다.
		//4. 혹시 언로딩하는 자재가 있는지, 동시 로딩 언로디이 필요한지 체크한다.
		//    동작중에 로드 언로드가 동시, 로딩만 인지 미리 계산후 동작하는게 좋다.
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
		//UNLOAD Picker 동작
// 		else if(st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_DVC_UNLOAD)  
// 		{
// 			mn_RunStep = 1200; 
// 		}
		break;

	case 1100:
		nRet_1 = Func.Check_BufferStatus(100, THD_LD_ALIGN_BUFF, CTL_NO, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus); //버퍼는 완전히 비워져 있어야 한다 
		if(nRet_1 == RET_GOOD)
		{
			//정상 상태, 처음으로 work robot 사이트로 이동하는 루틴으로, 로딩 버퍼에는 자재가 하나도 없어야 한다 
			mn_RunStep = 2000; 
		}
		else if(nRet_1 == RET_ERROR)
		{//자재가 남아있다면, 소켓 오프등으로 남아있는 자재이니 이떄는 테슽 로봇이 요청한 대로 바로 집을 수 있게 처리한다  

			mn_RunStep = 3000; 
			//CTL_Lib.Alarm_Error_Occurrence(5002, dWARNING, Func.m_strAlarmCode);
		}
		break;

	case 1200://이떄 버퍼는 완전히 비워져 있어야 한다 
		nRet_1 = Func.Check_BufferStatus(100, THD_LD_ALIGN_BUFF, CTL_NO, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus); //버퍼는 완전히 비워져 있어야 한다 
		if(nRet_1 == RET_GOOD)
		{
			//정상 상태, 
			mn_RunStep = 6000; //test robot가 집고있던 로딩 소켓 오프 자재를 놓는 영역이다. 이 자재를 놓고 테스트 사이트로 이동하여 작업한다
		}
		else if(nRet_1 == RET_ERROR)
		{//자재가 남아있다면 에러 
			CTL_Lib.Alarm_Error_Occurrence(5003, dWARNING, Func.m_strAlarmCode);
		}			 
		break;

	case 2000:
		m_strFindLotNo = "0";//LotNo에 신경쓰지 않는다. LDPicker만 신경쓴다
		nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LD_ALIGN_BUFF, CTL_NO, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strFindLotNo);
		if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다, 놓는 작업은 뒤에서(0번) 부터 놓는다 
		{
			m_nWork_Pos = m_npFindWorkPosYXCPB[0]; 
		}
		else if(nRet_1 == RET_SKIP) //더이상 넣을 공간이 없다 
		{	//계속 진행할 수 없다
			mn_RunStep = 1000;	 
			break;
		}
// 		//work robot에 자재 공급을 요청 
// 		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ; //로딩 자재 요청
// 		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE; //자재 놓는 작업
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY;
		mn_RunStep = 2100;
		break;

	case 2100:
		//LD_WORKRBT	st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY
		if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_CHANGE)//자재를 로보트가 로딩버퍼에 모두 채워 놓은 상태
		{
			nCount = 0;
			m_strLotNo = "";
			m_strPartNo = "";
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{ //버퍼의 자재 정보를 셋팅한다
				m_npBuff_Info[i] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo; //버퍼의 자재 정보를 셋팅한다
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
			{//자재가 하나도 없는 상태 
				mn_RunStep = 0;
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "There is no DVC in Load buffer step  = %d", mn_RunStep);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
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
		//유무 체크
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
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
		//유무 체크
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
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
		//work robot에 자재 PICKUP을 요청 
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ; //로딩 자재 요청
		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PICK; //자재 픽업
		mn_RunStep = 3000;
		break;

	case 3000:
		//LOAD RBT이 PICKUP을 완료함.
		if( st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0]== CTL_CHANGE )
		{
			mn_RunStep = 3100;
		}
		break;

	case 3100:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
		if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
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
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_DVCLdBuffer  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
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
