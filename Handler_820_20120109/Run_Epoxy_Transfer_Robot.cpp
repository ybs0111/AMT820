// Run_Epoxy_Transfer_Robot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Epoxy_Transfer_Robot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Epoxy_Transfer_Robot
CRun_Device_Carrier_Robot Run_Device_Carrier_Robot;
IMPLEMENT_SERIAL(CRun_Epoxy_Transfer_Robot, CObject, 1);

CRun_Epoxy_Transfer_Robot::CRun_Epoxy_Transfer_Robot()
{
}

CRun_Epoxy_Transfer_Robot::~CRun_Epoxy_Transfer_Robot()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Epoxy_Transfer_Robot message handlers

void CRun_Epoxy_Transfer_Robot::ThreadRun()
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


void CRun_Epoxy_Transfer_Robot::RunInit()
{
	if (st_sync.n_init_flag[THD_EMPTY_STACKER] != INIT_CLEAR)		return;		//INIT_CLEAR 일때만 초기화 작업을 한다. 초기화가 끝나면 INIT_CLEAR -> INIT_READY가 되기 떄문에...

	switch( mn_InitStep )
	{
	case -1:
		if( st_sync.n_init_flag[THD_WORK_TRANSFER] < INIT_READY ) break;
		mn_InitStep = 0;

	case 0:
		break;
	}
}

void CRun_Epoxy_Transfer_Robot::RunMove()
{
	int nRet_1,nRet_2,nRet_3;
	long lMotionDone=0;

	Func.ThreadFunctionStepTrace(20, mn_RunStep);
	switch(mn_RunStep)
	{
	case -1:
		mn_RunStep = o;
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



	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_Epoxy_Transfer_Robot  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}
}