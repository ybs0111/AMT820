// Run_Device_Carrier_Robot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Device_Carrier_Robot.h"
#include "AMTLotManager.h"
#include "FastechPublic_IO.h"
#include "IO_Manager.h"
#include "LogFromat.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Device_Carrier_Robot
CRun_Device_Carrier_Robot Run_Device_Carrier_Robot;
IMPLEMENT_SERIAL(CRun_Device_Carrier_Robot, CObject, 1);

CRun_Device_Carrier_Robot::CRun_Device_Carrier_Robot()
{
	int i = 0, j = 0;
	mn_RunUpStep = 0;
	mn_RunDownStep = 0;
	mn_RunCarrierStatus = 0;


	m_nInspectAxis = M_HEATSINK_INSPECT_Y;
	m_nEpoxy[0] = M_EPOXY_TRANSFER_X;
	m_nEpoxy[1] = M_EPOXY_TRANSFER_Y;
	m_nEpoxy[2] = M_EPOXY_TRANSFER_Z;
	m_nEpoxy[3] = M_EPOXY_SCREW;

	m_nRobot_Y = M_LOADER_TRANSFER_Y;

	nBcrNum = 0;
	mn_Count = 0;

	for ( i = 0; i < THREAD_MAX_SITE; i++ )
	{
		for ( j = 0; j < 5; j++ )
		{
			st_sync.nCarrierRbt_Dvc_Req[i][j] = CTL_NO;
		}
	}
}

CRun_Device_Carrier_Robot::~CRun_Device_Carrier_Robot()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Device_Carrier_Robot message handlers

void CRun_Device_Carrier_Robot::Thread_Run()
{
	switch( st_work.mn_run_status)
	{
	case dINIT:
		RunInit();
		break;

	case dRUN:
		RunMove();
		RunReadBcr();
		break;

	case dSTOP:
		break;

	case dWARNING:
		break;

	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//초기화할때 유의사항
//1. 초기화가 끝났을때는 Left에서는 Carrier가 없어야 하고 Right에서는 Carrier가 있어야 한다
//2. 될 수 있는데로 센서가 즉 CDIMM carrier 감지 일때 멈춰야 한다.
//3. 최소 3번 ~5번 정고 회전시켜서 바코드를 읽어 현재 위치의 바코드번호를 알 수 있도록 하자.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRun_Device_Carrier_Robot::RunInit()
{
	int nRet_1 = 0, nRet_2 = 0;
	if( st_handler.mn_init_state[INIT_LD_ROBOT] == CTL_NO ) return;
 	if( st_handler.mn_init_state[INIT_ULD_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_CARRIER_ROBOT] != CTL_NO ) return;
	st_handler.mn_init_state[INIT_CARRIER_ROBOT] = CTL_YES;

	switch( mn_InitStep )
	{
	case 0:
		AllBufferClear();
		mn_InitStep = 100;
		break;

	case 100:
		Set_Device_Carrier_Clamp_FwdBwd( 0, IO_ON );
		Set_Device_Carrier_Clamp_FwdBwd( 1, IO_ON );
		mn_InitStep = 110;
		break;

	case 110:
		nRet_1 = Chk_Device_Carrier_Clamp_FwdBwd( 0, IO_ON );
		nRet_2 = Chk_Device_Carrier_Clamp_FwdBwd( 1, IO_ON );
		if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
		{
			mn_InitStep = 200;
		}
		else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
		{
			mn_InitStep = 900;
		}
		break;

	case 200:
		Set_Device_Carrier_Slide_Z_Cylinder_UpDown(0, IO_OFF);
		Set_Device_Carrier_Slide_Z_Cylinder_UpDown(1, IO_OFF);
		mn_InitStep = 210;
		break;

	case 210:
		nRet_1 = Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_OFF );
		nRet_2 = Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_OFF );
		if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
		{
			mn_InitStep = 300;
		}
		else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
		{
			mn_InitStep = 900;
		}
		break;

	case 300:
		Set_Device_Carrier_HolderPin_Fix(0, IO_OFF);
		Set_Device_Carrier_HolderPin_Fix(1, IO_OFF);
		Set_Device_Carrier_HolderPin_Fix(2, IO_OFF);
		mn_InitStep = 310;
		break;

	case 310:
		nRet_1 = Chk_Device_Carrier_HolderPin_Fix(1, IO_OFF);
		nRet_2 = Chk_Device_Carrier_HolderPin_Fix(2, IO_OFF);
		if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
		{
			mn_InitStep = 400;
		}
		else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 3990, dWARNING, m_strAlarmCode);
			mn_InitStep = 900;
		}
		break;

	case 400:
		Set_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF );
		mn_InitStep = 410;
		break;

	case 410:
		nRet_1 = Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF );
		if( nRet_1 == RET_GOOD )
		{
			mn_InitStep = 500;
		}
		else if( nRet_1 == RET_ERROR )
		{
			mn_InitStep = 900;
		}
		break;

	case 500:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_NO )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_REQ;
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PICK;
			mn_InitStep = 510;
		}
		break;

	case 510:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_READY && 
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
		{
			mn_InitStep = 520;
		}
		break;

	case 520://EPOXY 초기화 진행
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_REQ;
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = WORK_PICK;
			mn_InitStep = 530;
		}

		break;

	case 530://EPOXY 초기화 완료
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_READY && 
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] == WORK_PICK)
		{
			mn_InitStep = 600;
		}
		break;

	case 600://Vision 초기화 진행
		if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_NO )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_REQ;
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] = WORK_PICK;
			mn_InitStep = 610;
		}
		break;

	case 610://Vision 초기화 완료
		if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_READY && 
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] == WORK_PICK)
		{
			mn_InitStep = 620;
		}
		break;

	case 620://Unpress 초기화 
		if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_READY && 
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_CHANGE; 
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PICK;
			mn_InitStep = 700;
		}
		break;

	case 700:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_FREE && 
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
		{
			Set_Device_Carrier_HolderPin_Fix(1, IO_ON);
			Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			mn_InitStep = 710;
		}
		break;

	case 710:
		nRet_1 = Chk_Device_Carrier_HolderPin_Fix(1, IO_ON);
		nRet_2 = Chk_Device_Carrier_HolderPin_Fix(2, IO_ON);
		if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
		{
			mn_InitStep = 800;
		}
		else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
		{
			mn_InitStep = 900;
		}

	case 800:
		st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_NO; 
		st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] = CTL_NO;
		st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_NO; 
		st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = CTL_NO;
		st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_NO; 
		st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = CTL_NO;
		mn_InitStep = 1000;
		break;

	case 900:
		mn_InitStep = 0;
		st_handler.mn_initial_error = TRUE;
		break;
		
	case 1000:
		st_handler.mn_init_state[INIT_CARRIER_ROBOT] = CTL_YES;
		mn_InitStep = 0;
		mn_RunMove = -1;
		break;
	}
}

//초기화할때 3번 돌고 안맞으면 한번 더
//시작할때 2번 돌자. 않맞으면 1번더   최대 6번 => 12개 버퍼중 6번 돌아서 바코드 위치를 정한다.
//돌리기 전에 바코드를 읽을 수도있도 있고, 없을 수도 있다
void CRun_Device_Carrier_Robot::RunMove()
{
	int i,nRet_1;
	long lMotionDone=0;

	Func.ThreadFunctionStepTrace(11, mn_RunMove);
	switch(mn_RunMove)
	{
	case -1:
		mn_RunMove = 0;
		break;

	case 0:		
		if( g_lotMgr.GetLotCount() > 0 )
		{
			mn_RunMove = 100;
		}
		break;

		//현재의 Carrier가 CDIMM인지 먼저 체크한다.
		//바코드를 읽어야 한다.
	case 100:
		nRet_1 = Check_Carrier_Move_Enable(0);
		if( nRet_1 != RET_GOOD )//초기상태
		{
			break;
		}
		st_sync.nCarrierBcr_Req = CTL_REQ;
		m_dwBcrWaitTime[0] = GetCurrentTime();
		mn_RunMove = 110;
		break;

	case 110:
		m_dwBcrWaitTime[1] = GetCurrentTime();
		m_dwBcrWaitTime[2] = m_dwBcrWaitTime[1] - m_dwBcrWaitTime[0];
		if( m_dwBcrWaitTime[2] <= 0 ) m_dwBcrWaitTime[0] = GetCurrentTime();
		
		if( st_sync.nCarrierBcr_Req == CTL_READY )
		{
			mn_RunMove = 120;
		}
		else
		{
			if( m_dwBcrWaitTime[2] > 5000 )
			{//940000 1 A "BARCODE_IS_NOT_RESPONSE."
				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "940000");	
				mn_RunMove = 100;
			}
		}
		break;

	case 120:
		nRet_1 = atoi(st_msg.mstr_barcode);
		if( nRet_1 > 0 && nRet_1 < 13)
		{
			for ( i = 0; i < 3; i++ )
			{
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i], "%d",nRet_1);
			}			
			mn_RunMove = 130;
		}
		else
		{//940001 1 A "BARCODE_IS_NOT_CORRECT_BARCODE_NUMBER."
			CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "940001");
			mn_RunMove = 100;
		}
		break;

	case 130:
		if( Func.Check_RunAllSafety() != RET_GOOD )
		{
			break;
		}

		nRet_1 = CheckCarrierType();
		if( nRet_1 == RET_GOOD )
		{
			mn_RunMove = 1000;
		}
		else
		{
			m_dwWaitTime[0] = GetCurrentTime();
			mn_RunMove = 140;
		}
		break;

	case 140:
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		if( m_dwWaitTime[2] <= 0 ) m_dwWaitTime[0] = GetCurrentTime();
		if( CheckCarrierType() == RET_GOOD )
		{
			mn_RunMove = 1000;//작업공간이다.
		}
		else
		{
			if( m_dwWaitTime[2] > 500 )
			{
				if( CheckCarrierType() == RET_ERROR )
				{
					mn_RunMove = 200;//작업공간이 아니라서 한바퀴 돌아야 한다,
				}
				else
				{//930000 1 A "CARRIRER_TYPE_IS_WRONG(PS2308PS2309PS2310)
					//혹시 캐리어가 1~2칸 밀려서 서로 센서가 틀릴수도 있다
					CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "930000");		
					mn_RunMove = 100;//ERROR
				}
			}
		}
		break;

	case 200://top 한칸 민다
		nRet_1 = CarrierTopForward(1);
		if( nRet_1 == RET_GOOD )
		{
			mn_RunMove = 1000;//작업 영역
		}
		else if( nRet_1 == RET_SKIP )
		{
			mn_RunMove  = 210;
		}
		break;

	case 210:
		if( Check_Carrier_Move_Enable(1) != RET_GOOD )//한칸 민 상태
		{
			break;
		}
		mn_RunMove = 220;
		break;

	case 220://데이타 이동값 
		Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_LOADER_RECEIVE );		
		mn_RunMove = 230;		
		break;

	case 230:
		if( Top_Carrierbuffer_sensor_flag_status_check( 0, TOPSHIFT_BUFF_LOADER_RECEIVE ) == RET_GOOD )
		{
			Top_ShiftCarrierDataMoveRbt( 1, TOPSHIFT_BUFF_LOADER_RECEIVE );//clear
			mn_RunMove = 300;
		}
		break;

	case 300://TOP의 Unload Carrier를 아래로 이동한다.
		if( Func.Check_RunAllSafety() != RET_GOOD )
		{
			break;
		}
		if( CarrierMoveDown() != RET_GOOD )
		{
			break;
		}
		mn_RunMove = 310;
		break;

	case 310:
		nRet_1 = Check_Carrier_Move_Enable(2);
		if( nRet_1 != RET_GOOD )
		{
			break;
		}
		mn_RunMove = 320;
		break;

	case 320://다운체크
		Top_ShiftCarrierDataMoveRbt( 0, TOPSHIFT_BUFF_UNLOADER );//TOPSHIFT_BUFF_UNLOADER -> BTMSHIFT_BUFF_DOWN
		mn_RunMove = 330;
		break;

	case 330:
		if( Top_Carrierbuffer_sensor_flag_status_check( 2, TOPSHIFT_BUFF_UNLOADER ) == RET_GOOD )
		{
			Top_ShiftCarrierDataMoveRbt( 1, TOPSHIFT_BUFF_UNLOADER );//clear
			mn_RunMove = 400;
		}
		break;

	case 400:
		if( Func.Check_RunAllSafety() != RET_GOOD )
		{
			break;
		}
		if( CarrierMoveUp() != RET_GOOD )
		{
			break;
		}
		mn_RunMove = 410;
		break;

	case 410:
		nRet_1 = Check_Carrier_Move_Enable(3);
		if( nRet_1 != RET_GOOD )//한칸 UP 상태
		{
			break;
		}
		mn_RunMove = 420;
		break;

	case 420://up체크
		Btm_ShiftCarrierDataMoveRbt( 0, BTMSHIFT_BUFF_LOADER_DOWN );//BTMSHIFT_BUFF_LOADER_DOWN -> TOPSHIFT_BUFF_LOADER_RECEIVE
		mn_RunMove = 430;
		break;

	case 430:
		if( Btm_Carrierbuffer_sensor_flag_status_check( 2, TOPSHIFT_BUFF_UNLOADER ) == RET_GOOD )
		{
			Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_LOADER_DOWN );//clear
			mn_RunMove = 400;
		}
		break;

	case 500:
		if( Func.Check_RunAllSafety() != RET_GOOD )
		{
			break;
		}
		nRet_1 = CarrierBtmForwrad();
		if( nRet_1 == RET_GOOD )//하단 이동
		{
			mn_RunMove = 510;
		}
		break;

	case 510:
		Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_DOWN );//BTMSHIFT_BUFF_DOWN -> BTMSHIFT_BUFF_DOWNFORWARD//한칸 민다
		mn_RunMove = 530;
		break;

	case 530:
		if( Btm_Carrierbuffer_sensor_flag_status_check( 0, BTMSHIFT_IDBUFF_SEALING_SITE_ALL_CHK ) == RET_GOOD )
		{
			Btm_ShiftCarrierDataMoveRbt( 1, BTMSHIFT_BUFF_DOWN );//clear
			mn_RunMove = 0;
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		//START
		//자재 LOAD가 덜 되었는가?
		// 자재 LOAD가 끝났다면 멀타랏인가?
		//
		//1. N Y Y Y Y Y Y
		//////////////////////////////////////////////////////////////////////////
	case 1000:
		//[0][0] 전랏 로드 [0][1] 전랏 언로드 
		//[1][0] 후랏 로드 [1][1] 후랏 언로드
		m_nFindLotNo_Flag[0][0] = -1; m_nFindLotNo_Flag[0][1] = -1; m_nFindLotNo_Flag[1][0] = -1; m_nFindLotNo_Flag[1][1] = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			if( g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{
				m_nFindLotNo_Flag[0][0] = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
			}
			else  if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( ( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) +g_lotMgr.GetLotAt(1).GetFailCnt(PRIME) ) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
				{
					m_nFindLotNo_Flag[1][0] = 1;
					m_strLotNo = g_lotMgr.GetLotAt(1).GetLotID();
					m_strPartNo = g_lotMgr.GetLotAt(1).GetPartID();
				}
			}
			//양품과 불량을 UNLOAD 피커가 전부 다 픽업했을 경우
			if( ( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) + g_lotMgr.GetLotAt(0).GetFailCnt(PRIME) ) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
			{				
				m_nFindLotNo_Flag[0][1] = 0;
				m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
			}
			else if( g_lotMgr.GetLotCount() >= 2 )
			{
				if( ( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) +g_lotMgr.GetLotAt(1).GetFailCnt(PRIME) ) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
				{
					m_nFindLotNo_Flag[1][1] = 1;
					m_strLotNo = g_lotMgr.GetLotAt(1).GetLotID();
					m_strPartNo = g_lotMgr.GetLotAt(1).GetPartID();
				}
				else
				{
					return;
				}
			}
			mn_RunMove = 1100;
		}
		break;

		//현재 사이트에 자재기 있는지 비어 있는지 체크한다.
	case 1100:
		nRet_1 = Check_Carrier_Move_Enable(0);
		if( nRet_1 != RET_GOOD )//초기상태
		{
			break;
		}

		for ( i = 0 ; i < 3; i++ )
		{
			if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_skip_flag[i] != CTL_YES)
			{
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[i] != CTL_YES )
				{
					if( m_nFindLotNo_Flag[0][0] >= 0 || m_nFindLotNo_Flag[1][0] >= 1)//LOAD 자재가 있는가?
					{
						if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[i] != CTL_YES )
						{
							mn_BufferPos = i;
							mn_RunMove = 2000;//LOAD하자
							m_Thread_Flag[0] = m_Thread_Flag[1] = m_Thread_Flag[2] = m_Thread_Flag[3] = CTL_NO;
							return;
						}
					}
					if( m_nFindLotNo_Flag[0][1] >= 0 || m_nFindLotNo_Flag[1][1] >= 1)//UNLOAD 자재가 있는가?
					{
						if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[i] ==  BIN_GOOD )
						{
							st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][1] = GetCurrentTime();
							st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][0];
							if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][2] <= 0 ) st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][0] = GetCurrentTime();
							if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][2] > (15*60) )//15분 완료시간
							{
								mn_BufferPos = i;
								mn_RunMove = 4000;//UNLOAD하자		
							}
							return;
						}
						else
						{
							if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[i] ==  BIN_VISION )
							{
								st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][1] = GetCurrentTime();
								st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][0];
								if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][2] <= 0 ) st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][0] = GetCurrentTime();
								if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].dwBdTime[i][2] > (15*60) )//15분 완료시간
								{
									st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[i] = BIN_GOOD;
								}
							}
						}

					}
				}
			}
		}
		//mn_RunMove = 7000;
		break;

	case 2000:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_REQ;
			st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
			st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
		}
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_REQ;
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = WORK_PLACE;
		}
// 		if( st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] == CTL_NO )
// 		{
// 			st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] = CTL_REQ;
// 			st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][1] = WORK_PLACE;
// 		}
		if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_NO )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_REQ;
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] = WORK_PLACE;
		}

		if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY )
		{
			m_Thread_Flag[0] = CTL_YES;
		}
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_READY )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_CHANGE;
			m_Thread_Flag[1] = CTL_YES;
		}
// 		if( st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] == CTL_READY )
// 		{
// 			st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] = CTL_CHANGE;
// 			m_Thread_Flag[2] = CTL_YES;
// 		}
		if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_READY )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_CHANGE;
			m_Thread_Flag[2] = CTL_YES;
		}
		if( m_Thread_Flag[0] == CTL_YES && m_Thread_Flag[1] == CTL_YES && m_Thread_Flag[2] == CTL_YES)// && m_Thread_Flag[3] == CTL_YES)
		{
			m_Thread_Flag[0] = m_Thread_Flag[1] = m_Thread_Flag[2] = m_Thread_Flag[3] = CTL_NO;
			mn_RunMove = 2100;
		}
		break;

	case 2100:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_FREE ) st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_NO;
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_FREE )		st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_NO;
		if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_FREE )	st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_NO;
		/*st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] == CTL_NO && */
		if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO &&
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_NO )
		{
			mn_RunMove = 1100;
		}
		break;

	case 4000:
		st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] = CTL_REQ;
		st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] = WORK_PICK;
		st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
		mn_RunMove = 4100;
		break;

	case 4100:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] == CTL_READY )
		{
			mn_RunMove = 1100;
		}
		break;

	case 7000://작업이 끝았다.





	case 5000:
		for ( i = 0 ; i < 3; i++ )
		{
			if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_skip_flag[i] != CTL_YES)
			{
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[i] != CTL_YES )
				{
					if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[i] != CTL_YES )
					{
						mn_BufferPos = i;
						mn_RunMove = 1210;
						return;
					}
				}

			}
		}


		break;
		


	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Up at Run_Device_Carrier_Robot  = %d", mn_RunUpStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
//1. 랏이 존재하는가?
//2. 각 위치의 safety가 동작 가능한 상태인가?
//3. 
/////////////////////////////////////////////////////////////////////////////////
int CRun_Device_Carrier_Robot::CarrierMoveUp()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	long lMotionDone=0;

	Func.ThreadFunctionStepTrace(12, mn_RunUpStep);
	switch(mn_RunUpStep)
	{
		case 0:
			mn_RunUpStep = 100;
			break;

		case 100:
			Set_Device_Carrier_HolderPin_Fix( 0, IO_OFF);
			m_dwUpCarrierWaitTime[0] = GetCurrentTime();
			mn_RunUpStep = 110;
			break;

		case 110:
			m_dwUpCarrierWaitTime[1] = GetCurrentTime();
			m_dwUpCarrierWaitTime[2] = m_dwUpCarrierWaitTime[1] - m_dwUpCarrierWaitTime[0];
			if( m_dwUpCarrierWaitTime[2] <= 0 ) m_dwUpCarrierWaitTime[0] = GetCurrentTime();
			if( m_dwUpCarrierWaitTime[2] > 500 )
			{
				mn_RunUpStep = 1000;
			}
			break;

		case 1000:
			//clamp를 벌린다.
			//자재가 없는지 센서통해서 체크한다. fix clamp도 확인한다.
			Set_Device_Carrier_Clamp_FwdBwd( 1, IO_OFF );//LOAD의 clamp를 벌린다.
			mn_RunUpStep = 1100;
			break;

		case 1100:
			nRet_1 = Chk_Device_Carrier_Clamp_FwdBwd( 1, IO_OFF);
			if( nRet_1 == IO_ON )
			{
				mn_RunUpStep = 2000;
			}
			break;

		case 2000:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_OFF);

			if( nRet_1 == IO_ON && nRet_2 == IO_OFF )
			{
				Set_Device_Carrier_HolderPin_Fix( 0, IO_OFF);//혹시나
				Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_ON );//LOAD의 Z축을 올린다.
				mn_RunUpStep = 2100;
			}
			else
			{
				mn_RunUpStep = 1000;
			}
			break;

		case 2100:
			nRet_1 = Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_ON );
			if( nRet_1 == IO_ON )
			{
				mn_RunUpStep = 3000;
			}
			break;

		case 3000:
			Set_Device_Carrier_HolderPin_Fix( 0, IO_OFF);//혹시나
			Set_Device_Carrier_Clamp_FwdBwd( 1, IO_ON );//UNLOAD의 clamp를 오무린다
			mn_RunUpStep = 3100;
			break;

		case 3100:
			nRet_1 = Chk_Device_Carrier_Clamp_FwdBwd( 1, IO_ON);
			if( nRet_1 == IO_ON )
			{
				mn_RunUpStep = 4000;
			}
			break;

		case 4000:
			Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_OFF );
			mn_RunUpStep = 4100;
			break;

		case 4100:
			nRet_1 = Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_OFF );
			if( nRet_1 == IO_ON )
			{
				mn_RunUpStep = 5000;
			}
			break;

		case 5000:
			mn_RunUpStep = 0;
			nFuncRet = RET_GOOD;
			break;

		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Up at Run_Device_Carrier_Robot  = %d", mn_RunUpStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}
	return nFuncRet;
}

int CRun_Device_Carrier_Robot::CarrierMoveDown()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	long lMotionDone=0;

	Func.ThreadFunctionStepTrace(13, mn_RunDownStep);
	switch(mn_RunDownStep)
	{
	case 0:
		mn_RunDownStep = 100;
		nRetryCyl = 0;
		break;

	case 100: //btm forward backward 실린더가 back되어 있어야 한다.
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_OFF);
		if( nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			mn_RunDownStep = 1000;
		}
		else
		{
// 			nRetryCyl++;
// 			if( nRetryCyl > 3 )
// 			{
// 				nRetryCyl = 0;
// 				strTmp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X1_Backward_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, strTmp);
// 			}
// 			mn_RunDownStep = 200;
		}
		break;

	case 200:
		Set_Device_Carrier_Slide_Bottom_X_ForBackward(IO_OFF);
		mn_RunDownStep = 300;
		break;

	case 300:
		if( Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF ) == RET_GOOD )
		{
			mn_RunDownStep = 100;
		}
		break;

	case 1000:
		Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_ON );//UNLOAD의 Z축을 올린다.
		mn_RunDownStep = 1100;
		break;

	case 1100:
		nRet_1 = Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_ON );
		if( nRet_1 == IO_ON )
		{
			mn_RunDownStep = 2000;
		}
		break;

	case 2000:
		Set_Device_Carrier_Clamp_FwdBwd( 0, IO_OFF );//UNLOAD의 clamp를 벌린다.
		mn_RunDownStep = 2100;
		break;

	case 2100:
		nRet_1 = Chk_Device_Carrier_Clamp_FwdBwd( 0, IO_OFF);
		if( nRet_1 == IO_ON )
		{
			mn_RunDownStep = 3000;
		}
		break;

	case 3000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, IO_OFF);
		if( nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_OFF );
			mn_RunDownStep = 3100;
		}
		else
		{
			mn_RunDownStep = 2000;
		}
		break;

	case 3100:
		nRet_1 = Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_OFF );
		if( nRet_1 == IO_ON )
		{
			mn_RunDownStep = 4000;
		}
		break;

	case 4000:
		Set_Device_Carrier_Clamp_FwdBwd( 0, IO_ON );//UNLOAD의 clamp를 오무린다
		mn_RunDownStep = 4100;
		break;

	case 4100:
		nRet_1 = Chk_Device_Carrier_Clamp_FwdBwd( 0, IO_ON);
		if( nRet_1 == IO_ON )
		{
			mn_RunDownStep = 5000;
		}
		break;

	case 5000:
		mn_RunDownStep = 0;
		nFuncRet = RET_GOOD;
		break;

	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Down at Run_Device_Carrier_Robot  = %d", mn_RunDownStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}

	return nFuncRet;
}

void CRun_Device_Carrier_Robot::RunReadBcr()
{

	Func.ThreadFunctionStepTrace(17, mn_RunBcrStep);

	switch(mn_RunBcrStep)
	{
	case -1:
		mn_RunBcrStep = 0;
		break;

	case 0:
		mn_RunBcrStep = 1000;
		break;

	case 1000:
		if( st_sync.nCarrierBcr_Req == CTL_REQ )
		{
			if( st_basic.n_mode_device == CTL_NO )
			{
				nBcrNum++;
				if( nBcrNum > 12 ) nBcrNum = 1;
				st_var.nBcrNum = nBcrNum;
				st_sync.nCarrierBcr_Req = CTL_READY;
				mn_RunBcrStep = 2000;
			}
			//kwlee 2017.0412
			Func.OnBcrReader(); 
			m_dwWaitTime[0] = GetCurrentTime();
			mn_RunBcrStep = 1100;
		}
		break;

	case 1100:
// 		if( st_sync.nCarrierBcr_Req == CTL_READY )
// 		{
// 			mn_RunBcrStep = 2000;
// 		}
		//kwlee 2017.0412
		m_dwWaitTime[1] = GetCurrentTime();
		m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
		if (m_dwWaitTime[2] < 0)
		{
			m_dwWaitTime[0] = GetCurrentTime();
		}
		if (m_dwWaitTime[2] > 300)
		{		
			if( st_sync.nCarrierBcr_Req == CTL_READY )
			{	
				st_sync.nCarrierBcr_Req = CTL_CLEAR; //kwlee 2017.0412
				mn_RunBcrStep = 2000;	
			}
		}
		break;

	case 2000:
		if( st_sync.nCarrierBcr_Req == CTL_CLEAR )
		{
			st_sync.nCarrierBcr_Req = CTL_NO;
			mn_RunBcrStep = 0;
		}
		break;


	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no RunReadBcr at Run_Device_Carrier_Robot  = %d", mn_RunBcrStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
		}
		break;
	}
}

void CRun_Device_Carrier_Robot::AllBufferClear()
{
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE]));
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER]));
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY]));
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX]));
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION]));
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND]));
	memset(&st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER]));

	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN]));
	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD]));
	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN]));
	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN]));
	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN]));
	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN]));
	memset(&st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN]));
}

//////////////////////////////////////////////////////////////////////////
//carrier sata change shift
//nMode = 0  쉬프트하는 정보를 받았을 때 nMode = 1 정보를 저장
// N Y Y Y Y Y Y  : 평상시 이거나 Load carrier 상
// N Y Y Y Y Y Y  : 평상시 이거나 Unload carrier 하
//////////////////////////////////////////////////////////////////////////
int CRun_Device_Carrier_Robot::Top_ShiftCarrierDataMoveRbt( int nMode, int nSiftSide )
{
	int nFuncRet = CTL_GOOD;
	int nFlag = 0;

	if( nSiftSide == TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE )
	{
		if( nMode == 0 )
		{
			if(st_basic.n_mode_device == CTL_NO)
			{
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_skip_flag[TOP] = CTL_NO;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_skip_flag[MIDDLE] = CTL_NO;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_skip_flag[BTM] = CTL_NO;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[TOP] = CTL_YES;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[MIDDLE] = CTL_YES;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[BTM] = CTL_YES;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuff_num[TOP] =50; //바코드를 잃지 않아 가상번호 부여한다.
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuff_num[MIDDLE] =50; //바코드를 잃지 않아 가상번호 부여한다.
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuff_num[BTM] =50; //바코드를 잃지 않아 가상번호 부여한다.
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[TOP] = 1;//자재가 있으면
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[MIDDLE] = 1;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[BTM] = 1;

				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_count[TOP]	  = ++mn_Count;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_count[MIDDLE] = ++mn_Count;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_count[BTM]	  = ++mn_Count;


				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_totalcnt = 1000;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[TOP] = BIN_LDBUFFERBIN;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[MIDDLE] = BIN_LDBUFFERBIN;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[BTM] = BIN_LDBUFFERBIN;

				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id[TOP], "%s", g_lotMgr.GetLotAt(0).GetLotID() );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id[MIDDLE], "%s",  g_lotMgr.GetLotAt(0).GetLotID() );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id[BTM], "%s",  g_lotMgr.GetLotAt(0).GetLotID() );

				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_part_num[TOP], "%s", g_lotMgr.GetLotAt(0).GetPartID());
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_part_num[MIDDLE], "%s", g_lotMgr.GetLotAt(0).GetPartID());
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_part_num[BTM], "%s", g_lotMgr.GetLotAt(0).GetPartID());
			}
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], &st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE])); //clear 
			

			if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_idbuffer[TOP] == CTL_YES)
			{	
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_normal_msg, "[DATA SHIFT] TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else
			{
				AfxMessageBox("[datainfo_shift_error] -TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE");

				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_normal_msg, "[ERROR] TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE DATA");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}			
		}
		else// if(nMode == 1)
		{
			//memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_RECEIVE, 0);
		}
	}
	else if( nSiftSide == TOPSHIFT_BUFF_LOADER_RECEIVE )
	{
		if(nMode == 0) //data shift 
		{
// 			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], &st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], sizeof(st_carrier_buffer_info_param) ); //data copy
	
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER], &st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND])); //clear 

			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], &st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION])); //clear 

			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], &st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX])); //clear 

			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], &st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY])); //clear 

			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], &st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER])); //clear 

			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], &st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE])); //clear 

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_INPUT_LOADER");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE])); //clear  
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_LOADER_RECEIVE DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_RECEIVE, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_INPUT_LOADER, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_INPUT_LOADER, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_EPOXY, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_WAIT_INDEX, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_HEATSINK_VISION, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_OUTSEND, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_UNLOADER, 0);
		}
	}
	else if(nSiftSide == TOPSHIFT_BUFF_INPUT_LOADER)
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], &st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_EPOXY");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_INPUT_LOADER DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_INPUT_LOADER, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_EPOXY, 0);
		}
	}
	else if( nSiftSide == TOPSHIFT_BUFF_EPOXY )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], &st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_EPOXY");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_EPOXY DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_WAIT_INDEX, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_EPOXY, 0);
		}
	}
	else if( nSiftSide == TOPSHIFT_BUFF_WAIT_INDEX )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], &st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_WAIT_INDEX");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_WAIT_INDEX DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_HEATSINK_VISION, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_WAIT_INDEX, 0);
		}
	}
	else if( nSiftSide == TOPSHIFT_BUFF_HEATSINK_VISION )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], &st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_OUTSEND");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_HEATSINK_VISION DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_OUTSEND, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_HEATSINK_VISION, 0);
		}
	}
	else if( nSiftSide == TOPSHIFT_BUFF_OUTSEND )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER], &st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_OUTSEND");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_OUTSEND DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_UNLOADER, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_OUTSEND, 0);
		}
	}
	else if( nSiftSide == TOPSHIFT_BUFF_UNLOADER )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWN], &st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] TOPSHIFT_BUFF_UNLOADER");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] TOPSHIFT_BUFF_UNLOADER DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_UNLOADER, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_DOWN, 0);
		}
	}
	return nFuncRet;
}

/////////////////////////////////////////////////////////////////////////////////////
// 동작 flag와 그와 연관된 I/O의 상태를 체크하는 함수 
//n_mode = 0 밀기 전과 평상시
//n_mode = 1 밀었을 때의 상태
//n_mode = 2 다운때의 상태
// N Y Y Y Y Y Y  밀기전 0 ( mode 0 )
// Y Y Y Y Y Y N  밀때 와 밀고 난뒤 1 Top_Carrierbuffer_sensor_flag_status_check ( mode1 )
// N Y Y Y Y Y Y  밀고 올라온뒤 == 밀기전과 같다 2 == 0 ( mode1 )
/////////////////////////////////////////////////////////////////////////////////////
int CRun_Device_Carrier_Robot::Top_Carrierbuffer_sensor_flag_status_check(int n_mode, int n_check_site)
{
	int i=0, j =0;
	int nFunRet = CTL_GOOD;
	int nRet_1=0, nRet_2=0;
//	int nFixClamp_OnOff_1, nFixClamp_OnOff_2, nFixClamp_OnOff_3;

// 	TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE = 0,
// 
// 	TOPSHIFT_BUFF_LOADER_RECEIVE,		//Loader에 carrier를 공급한 상태 또는 초기화 후 맨처음 상태
// 	TOPSHIFT_BUFF_INPUT_LOADER ,				//Epoxy 전 상태
// 	TOPSHIFT_BUFF_EPOXY,							//EPOXY
// 	TOPSHIFT_BUFF_WAIT_INDEX,					//Heatsink vison 대기 상태
// 	TOPSHIFT_BUFF_HEATSINK_VISION,			//Heatsink vision 상태
// 	TOPSHIFT_BUFF_OUTSEND,								//send 전상태
// 	TOPSHIFT_BUFF_UNLOADER,							//Unload 상태
// 
// 	TOPSHIFT_DATA_TEMP_CHECK,
// 	TOPSHIFT_IDBUFF_SEALING_SITE_ALL_CHK

	if( Func.Check_BeforeMoveUnPressRbt( n_mode ) != RET_GOOD )
		return CTL_ERROR;

	for( i=0; i<40; i++ )
	{
		if(i == 0)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_idbuffer[j];

		else if(i == 1)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].n_idbuffer[j];

		else if(i == 2)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_idbuffer[j];

		else if(i == 3) 
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].n_idbuffer[j];

		else if(i == 4) 
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_idbuffer[j];

		else if(i == 5) 
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].n_idbuffer[j];

		else if(i == 6) 
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].n_idbuffer[j];

		else if(i == 10)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[j];

		else if(i == 11)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].n_exist[j];

		else if(i ==12)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[j];

		else if(i==13)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].n_exist[j];

		else if( i == 14)
			for( j = 0; j < 3; j++ )
				 st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[j];

		else if(i == 15)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].n_exist[j];

		else if(i == 16)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].n_exist[j];		

		else if(i ==20)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_idbuff_num[j];

		else if(i == 21)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_INPUT_LOADER].n_idbuff_num[j];

		else if(i ==22)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_idbuff_num[j];

		else if(i==23)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_WAIT_INDEX].n_idbuff_num[j];

		else if(i==24)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_idbuff_num[j];

		else if(i==25)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_OUTSEND].n_idbuff_num[j];

		else if(i==26)
			for( j = 0; j < 3; j++ )
				st_var.n_top_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[TOPSHIFT_BUFF_UNLOADER].n_idbuff_num[j];

		//load unload carrier 센서
		if( i == 0 )		st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);
		else if(i == 6)	st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);		
		else if(i == 30)//type센서
		{
			if( st_handler.n_without_idbuffer == CTL_YES)
			{
				if( st_var.nBcrNum % 2 == 0)
					st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = IO_OFF;
				else
					st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = IO_ON;
			}
			else
				st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_press_carrier_type3, IO_OFF);//type

		}
		else if(i == 32)
		{//만약 바코드가 홀수이면 구멍이 있는 carrier이므로 센서가 off
			if( st_handler.n_without_idbuffer == CTL_YES)
			{
				if( st_var.nBcrNum % 2 == 0)
					st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = IO_OFF;
				else
					st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = IO_ON;
			}
			else
				st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_press_carrier_type2, IO_OFF);//type
		}
		else if(i == 34)
		{
			if( st_handler.n_without_idbuffer == CTL_YES)
			{
				if( st_var.nBcrNum % 2 == 0)
					st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = IO_OFF;
				else
					st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = IO_ON;
			}
			else
				st_var.n_top_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_press_carrier_type1, IO_OFF);//type
		}
	}
		
	if		(n_check_site == TOPSHIFT_IDBUFF_SEALING_SITE_ALL_CHK) 
	{
		for ( i = 0; i < 40; i++)
		{
			if( i == 30 || i == 32 || i == 34)
			{
// 				if( (st_var.n_top_carrier_buffer_exist_status_check_info[i]) != st_var.n_top_carrier_buffer_io_status_read_chk_data[i])
// 				{
// 					if(st_handler.n_without_idbuffer == CTL_YES || st_basic.n_mode_bcr == CTL_NO)
// 					{
// 						continue;
// 					}
// 					else
// 					{
// 						if ( i == 30 )
// 							sprintf(mc_jamcode, "81%04d", st_io.i_press_carrier_type3); 
// 						else if( i == 32)
// 							sprintf(mc_jamcode, "81%04d", st_io.i_press_carrier_type2); 
// 						else
// 							sprintf(mc_jamcode, "81%04d", st_io.i_press_carrier_type1); 
// 
// 						nFunRet = CTL_ERROR;
// 						break;
// 					}
// 				}			
			}
			if( i == 0 || i == 6 )
			{
				for( j = 0; j < 3; j++ )
				{
					if(st_var.n_top_carrier_buffer_exist_status_check_info[i][j] != st_var.n_top_carrier_buffer_io_status_read_chk_data[i])
					{
						if(st_handler.n_without_idbuffer == CTL_YES)
						{
							if(i == 0 || i == 6) continue;
						}
						if(st_basic.n_mode_device == 0)//
						{
							if(i == 0 || i == 6) continue;
						}

						if(i == 0 || i == 6)
						{
							if( n_mode == 2 )//다운 했을 때
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);
									nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_ON);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_2_Up_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_1_Up_Check );
								}
							}
							else if( n_mode == 1 )//밀었을 시기
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_2_Up_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_1_Up_Check );
								}
							}
							else//밀기전
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_2_Up_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_1_Up_Check );
								}
							}
						}
						nFunRet = CTL_ERROR;
						return nFunRet;
					}
				}
			}
		}
	}
	else if(n_check_site == TOPSHIFT_BUFF_LOADER_RECEIVE)
	{
		for ( i = 0; i < 40; i++ )
		{
			if(i == 0 || i == 6)// || i == 10 || i == 16)
			{
				for( j = 0; j < 3; j++ )
				{
					if(st_var.n_top_carrier_buffer_exist_status_check_info[i][j] != st_var.n_top_carrier_buffer_io_status_read_chk_data[i])
					{
						if(st_handler.n_without_idbuffer == CTL_YES)
						{
							if(i == 0 || i == 6) continue;
						}
						if(st_basic.n_mode_device == 0)
						{
							if(i== 10 || i == 16)continue;
						}

						if(i == 0 || i == 6)
						{
							if( n_mode == 1 )//밀었을 시기
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_2_Up_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_1_Up_Check );
								}
							}
							else//밀기전
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_2_Up_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_1_Up_Check );
								}
							}

						}
						nFunRet = CTL_ERROR;
						return nFunRet;
					}
				}
			}
		}
	}
	return nFunRet;
}

int CRun_Device_Carrier_Robot::Btm_ShiftCarrierDataMoveRbt( int nMode, int nSiftSide )
{
	int nFuncRet = CTL_GOOD;
	int nFlag = 0;

	if( nSiftSide == BTMSHIFT_BUFF_DOWN )//btm에서 한칸 민다.
	{
		if( nMode == 0 )
		{
			if(st_basic.n_mode_device == CTL_NO)
			{
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_skip_flag[TOP] = CTL_NO;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_skip_flag[MIDDLE] = CTL_NO;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_skip_flag[BTM] = CTL_NO;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuffer[TOP] = CTL_YES;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuffer[MIDDLE] = CTL_YES;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuffer[BTM] = CTL_YES;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuff_num[TOP] =50; //바코드를 잃지 않아 가상번호 부여한다.
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuff_num[MIDDLE] =50; //바코드를 잃지 않아 가상번호 부여한다.
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuff_num[BTM] =50; //바코드를 잃지 않아 가상번호 부여한다.
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[TOP] = 1;//자재가 있으면
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[MIDDLE] = 1;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[BTM] = 1;

				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_count[TOP]	 = ++mn_Count;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_count[MIDDLE] = ++mn_Count;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_count[BTM]	 = ++mn_Count;


				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_totalcnt = 1000;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[TOP] = BIN_LDBUFFERBIN;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[MIDDLE] = BIN_LDBUFFERBIN;
				st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].nBin[BTM] = BIN_LDBUFFERBIN;

				sprintf(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_lot_id[TOP], "%s", g_lotMgr.GetLotAt(0).GetLotID() );
				sprintf(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_lot_id[MIDDLE], "%s",  g_lotMgr.GetLotAt(0).GetLotID() );
				sprintf(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_lot_id[BTM], "%s",  g_lotMgr.GetLotAt(0).GetLotID() );

				sprintf(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_part_num[TOP], "%s", g_lotMgr.GetLotAt(0).GetPartID());
				sprintf(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_part_num[MIDDLE], "%s", g_lotMgr.GetLotAt(0).GetPartID());
				sprintf(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].c_part_num[BTM], "%s", g_lotMgr.GetLotAt(0).GetPartID());
			}

			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN])); //clear 

			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN])); //clear 

			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN])); //clear 

			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN])); //clear 

			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD])); //clear 

			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD], &st_carrier_buff_info[BTMSHIFT_BUFF_DOWN], sizeof(st_carrier_buff_info)); //data copy
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN])); //clear 

			if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_idbuffer[TOP] == CTL_YES)
			{	
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_normal_msg, "[DATA SHIFT] BTMSHIFT_BUFF_DOWN");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else
			{
				AfxMessageBox("[datainfo_shift_error] -BTMSHIFT_BUFF_DOWN");

				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_normal_msg, "[ERROR] BTMSHIFT_BUFF_DOWN DATA");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}			
		}
		else// if(nMode == 1)
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_DOWN])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_DOWN DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_DOWNFORWARD, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_HEATSINK_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INDEX_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_EPOXY_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INPUT_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_LOADER_DOWN, 0);
		}
	}
	else if( nSiftSide == BTMSHIFT_BUFF_DOWNFORWARD )
	{
		if(nMode == 0) //data shift 
		{
			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD], sizeof(st_carrier_buffer_info_param) ); //data copy

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] BTMSHIFT_BUFF_DOWNFORWARD");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD])); //clear  
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_DOWNFORWARD DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_DOWNFORWARD, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_HEATSINK_DOWN, 0);
		}
	}
	else if(nSiftSide == BTMSHIFT_BUFF_HEATSINK_DOWN)
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] BTMSHIFT_BUFF_HEATSINK_DOWN");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //2014.1108 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_HEATSINK_DOWN DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_HEATSINK_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INDEX_DOWN, 0);
		}
	}
	else if( nSiftSide == BTMSHIFT_BUFF_INDEX_DOWN )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] BTMSHIFT_BUFF_INDEX_DOWN");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_INDEX_DOWN DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INDEX_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_EPOXY_DOWN, 0);
		}
	}
	else if( nSiftSide == BTMSHIFT_BUFF_EPOXY_DOWN )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] BTMSHIFT_BUFF_EPOXY_DOWN");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_EPOXY_DOWN DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_EPOXY_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INPUT_DOWN, 0);
		}
	}
	else if( nSiftSide == BTMSHIFT_BUFF_INPUT_DOWN )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN], &st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] BTMSHIFT_BUFF_INPUT_DOWN");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_INPUT_DOWN DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_LOADER_DOWN, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, BTMSHIFT_BUFF_INPUT_DOWN, 0);
		}
	}
	else if( nSiftSide == BTMSHIFT_BUFF_LOADER_DOWN )
	{
		if(nMode == 0)
		{
			memcpy(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], &st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN], sizeof(st_carrier_buffer_info_param)); //data copy
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[shift data] BTMSHIFT_BUFF_LOADER_DOWN");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else //if(n_mode == 1) //이 사이트의 정보를 클리어 한다 
		{
			memset(&st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN], 0x00, sizeof(st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN])); //clear 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[CLEAR DATA] BTMSHIFT_BUFF_LOADER_DOWN DATA CLEAR");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}	
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE, 0);
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_OUTSEND, 0);
		}
	}

	return nFuncRet;
}

/////////////////////////////////////////////////////////////////////////////////////
// 동작 flag와 그와 연관된 I/O의 상태를 체크하는 함수 
//n_mode = 0 밀었을 때의 상태
//n_mode = 1 밀었을 때의 상태
//n_mode = 2 up  //down은 top에서 담당한다.
// N Y Y Y Y Y Y  밀기전 0 ( mode 0 )
// Y Y Y Y Y Y N  밀때 와 밀고 난뒤 1 Top_Carrierbuffer_sensor_flag_status_check ( mode1 )
// N Y Y Y Y Y Y  밀고 올라온뒤 == 밀기전과 같다 2 == 0 ( mode1 )
/////////////////////////////////////////////////////////////////////////////////////
int CRun_Device_Carrier_Robot::Btm_Carrierbuffer_sensor_flag_status_check(int n_mode, int n_check_site)
{
	int i=0, j =0;
	int nFunRet = CTL_GOOD;
	int nRet_1=0, nRet_2=0;
//	int nFixClamp_OnOff_1, nFixClamp_OnOff_2, nFixClamp_OnOff_3;

// 	BTMSHIFT_BUFF_DOWN,
// 	BTMSHIFT_BUFF_DOWNFORWARD,
// 	BTMSHIFT_BUFF_HEATSINK_DOWN,
// 	BTMSHIFT_BUFF_INDEX_DOWN,
// 	BTMSHIFT_BUFF_EPOXY_DOWN,
// 	BTMSHIFT_BUFF_INPUT_DOWN,
// 	BTMSHIFT_BUFF_LOADER_DOWN,
// 
// 	BTMSHIFT_DATA_TEMP_CHECK,
// 	BTMSHIFT_IDBUFF_SEALING_SITE_ALL_CHK

// 	if( Func.Check_BeforeMoveUnPressRbt( n_mode ) != RET_GOOD )
// 		return CTL_ERROR;

	for(i=0; i<30; i++)
	{
		if(i == 0)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuffer[j];

		else if(i == 1)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_idbuffer[j];

		else if(i == 2)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_idbuffer[j];

		else if(i == 3) 
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_idbuffer[j];

		else if(i == 4) 
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_idbuffer[j];

		else if(i == 5) 
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_idbuffer[j];

		else if(i == 6) 
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_idbuffer[j];

		else if(i == 10)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_exist[j];

		else if(i ==11)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_exist[j];

		else if(i==12)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_exist[j];

		else if( i == 13)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_exist[j];

		else if(i == 14)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_exist[j];

		else if(i == 15)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_exist[j];

		else if(i == 16)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_exist[j];
		
		else if(i ==20)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWN].n_idbuff_num[j];

		else if(i == 21)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_DOWNFORWARD].n_idbuff_num[j];

		else if(i ==22)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_HEATSINK_DOWN].n_idbuff_num[j];

		else if(i==23)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_INDEX_DOWN].n_idbuff_num[j];

		else if(i==24)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_EPOXY_DOWN].n_idbuff_num[j];

		else if(i==25)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_INPUT_DOWN].n_idbuff_num[j];

		else if(i==26)
			for( j = 0; j < 3; j++ )
				st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] = st_carrier_buff_info[BTMSHIFT_BUFF_LOADER_DOWN].n_idbuff_num[j];

		//load unload carrier 센서
		if( i == 0 )		st_var.n_btm_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
		else if(i == 6)	st_var.n_btm_carrier_buffer_io_status_read_chk_data[i] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);		
	}



	if		(n_check_site == BTMSHIFT_IDBUFF_SEALING_SITE_ALL_CHK) 
	{
		for ( i = 0; i < 10; i++)
		{
			if( i == 0 || i == 6 )//DOWN 캐리어 인식 센서
			{
				for( j = 0; j < 3; j++ )
				{
					if(st_var.n_btm_carrier_buffer_exist_status_check_info[i][j] != st_var.n_btm_carrier_buffer_io_status_read_chk_data[i])//BTM의 carrier가 양쪽에 있는지 체크한다.
					{
						if(st_handler.n_without_idbuffer == CTL_YES)
						{
							if(i == 0 || i == 6) continue;
						}
						if(st_basic.n_mode_device == 0)//
						{
							if(i == 0 || i == 6) continue;
						}

						if(i == 0 || i == 6)
						{
							if( n_mode == 2 )//up carrior일때
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);//down 쪽 미는곳
									nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);//down 쪽 미는곳
									if(nRet_1 == IO_OFF && nRet_2 == IO_ON && st_var.n_btm_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_1_Down_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);//load 쪽 Up하는곳
									nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
									if(nRet_1 == IO_ON && nRet_2 == IO_OFF && st_var.n_btm_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_2_Down_Check );
								}
							}
							else if( n_mode == 1 )//밀었을 시기
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);//down 쪽 미는곳
									if(nRet_1 == IO_OFF && st_var.n_btm_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_1_Down_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);//load 쪽 Up하는곳
									if(nRet_1 == IO_ON && st_var.n_btm_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_2_Down_Check );
								}
							}
							else//밀기전
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_1_Down_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_2_Down_Check );
								}
							}
						}
						nFunRet = CTL_ERROR;
						return nFunRet;
					}
				}
			}
		}

	}
	else if(n_check_site == BTMSHIFT_BUFF_DOWN)
	{
		for ( i = 0; i < 10; i++)
		{
			if(i == 0 || i == 6)// || i == 10 || i == 16)
			{
				for( j = 0; j < 3; j++ )
				{
					if(st_var.n_top_carrier_buffer_exist_status_check_info[i][j] != st_var.n_top_carrier_buffer_io_status_read_chk_data[i])
					{
						if(st_handler.n_without_idbuffer == CTL_YES)
						{
							if(i == 0 || i == 6) continue;
						}
						if(st_basic.n_mode_device == 0)
						{
							if(i== 10 || i == 16)continue;
						}

						if(i == 0 || i == 6)
						{
							if( n_mode == 1 )//밀었을 시기
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_1_Down_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_2_Down_Check );
								}
							}
							else//밀기전
							{
								if(i == 0)
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);
									if(nRet_1 == IO_ON && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_YES)
										continue;
									sprintf(mc_jamcode, "81%04d", st_io.i_Carrier_Z_1_Down_Check );
								}
								else
								{
									nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
									if(nRet_1 == IO_OFF && st_var.n_top_carrier_buffer_io_status_read_chk_data[i] == CTL_NO)
										continue;
									sprintf(mc_jamcode, "80%04d", st_io.i_Carrier_Z_2_Down_Check );
								}
							}

						}
						nFunRet = CTL_ERROR;
						return nFunRet;
					}
				}
			}
		}
	}
	return nFunRet;
}

//////////////////////////////////////////////////////////////////////////
//TOPSHIFT_BUFF_LOADER_RECEIVE,TOPSHIFT_BUFF_UNLOADER에 Carrier가 있는지
//BTMSHIFT_BUFF_DOWN,LOADER_DOWN에 Carrier가 있는지
//////////////////////////////////////////////////////////////////////////
// int CRun_Device_Carrier_Robot::CheckCarrierStatus( int nCycle)
// {
// 	int i = 0, nFuncRet = RET_PROCEED, nRet_1 = 0;
// 	int nRet[8] = {0,};
// 	
// 	Func.ThreadFunctionStepTrace(14, mn_RunCarrierStatus);
// 	switch( mn_RunCarrierStatus)
// 	{
// 		case 0:
// 			mn_RunCarrierStatus = 10;
// 			break;
// 
// 		case 10:
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
// 				mn_RunCarrierStatus = 100;
// 			}
// 			break;
// 
// 		//일단, 처음에는 TOP와 BTM이 아래 상황이라고 생각한다. 
// 		// N Y Y Y Y Y Y
// 		// N Y Y Y Y Y Y
// 		case 100:
// 			//N에 clamp가 On되어 있는가?
// 			nRet[0] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, IO_OFF); // Left 끝단에서 Jig 걸어줌 
// 			nRet[1] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);
// 			nRet[2] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_OFF);		// Right 끝단에서 Jig 걸어줌 
// 			nRet[3] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);
// 
// 			if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON )
// 			{
// 				mn_RunCarrierStatus = 200;
// 			}
// 			else
// 			{
// 				if( nRet[0] != IO_OFF) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_1_Backward_Check); 
// 				else if(nRet[1] != IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Carrier_1_Forward_Check); 
// 				else if(nRet[2] != IO_OFF) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_2_Forward_Check); 
// 				else /*if(nRet[3] != IO_ON)*/ m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Carrier_2_Backward_Check); 
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);
// 			}
// 			break;
// 
// 		case 200:
// 			nRet[0] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);//vision heatsink fix// 상단 중간에서 JIg 잡아줌 
// 			nRet[1] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_OFF);
// 			nRet[2] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, IO_ON);
// 			nRet[3] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, IO_OFF);
// 			if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF )
// 			{
// 				mn_RunCarrierStatus = 300;
// 			}
// 			else
// 			{
// 				if( nRet[0] != IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Camera_Y_Jig_Press_Backward_Check); 
// 				else if(nRet[1] != IO_OFF) m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Camera_Y_Jig_Press_Forward_Check); 
// 				else if(nRet[2] != IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Camera_Y_Press_Up_Check); 
// 				else /*if(nRet[3] != IO_ON)*/ m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Camera_Y_Press_Down_Check); 
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);
// 			}
// 			break;
// 
// 		case 300:
// 			nRet[0] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Up_Check, IO_OFF);		// Left z
// 			nRet[1] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
// 			nRet[2] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Up_Check, IO_OFF);		// Right Z
// 			nRet[3] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
// 			if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON )
// 			{
// 				mn_RunCarrierStatus = 400;
// 			}
// 			else
// 			{
// 				if( nRet[0] != IO_OFF) m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Slide_Guide_Z1_Up_Check); 
// 				else if(nRet[1] != IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_Z1_Down_Check); 
// 				else if(nRet[2] != IO_OFF) m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Slide_Guide_Z2_Up_Check); 
// 				else /*if(nRet[3] != IO_ON)*/ m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_Z2_Down_Check); 
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);
// 			}
// 			break;
// 
// 		case 400:
// 			nRet[0] = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);	// Device 눌러줌 
// 			nRet[1] = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
// 			nRet[2] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_OFF);	// 상단 중간에서 JIg 잡아줌 
// 			nRet[3] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_ON);	
// 			if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON )
// 			{
// 				mn_RunCarrierStatus = 500;
// 			}
// 			else
// 			{
// 				if( nRet[0] != IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check); 
// 				else if(nRet[1] != IO_OFF) m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Down_Check); 
// 				else if(nRet[2] != IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Carrier_Holder_Up_Check); 
// 				else /*if(nRet[3] != IO_ON)*/ m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check); 
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);
// 			}
// 			break;
// 
// 		case 500:
// 			nRet[0] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_ON);	// 하단에서 Jig 밀어줌 
// 			nRet[1] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_OFF);
// 			nRet[2] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);		// 상단에서 Jig 밀어줌 
// 			nRet[3] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
// 			nRet[4] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Forward_Check, IO_ON);		// 상단 중간에서 jig 앞뒤로 눌러서 jig 잡아줌 
// 			nRet[5] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Backward_Check, IO_OFF);
// 			if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON/* && nRet[3] == IO_OFF && nRet[4] == IO_OFF*/)
// 			{
// 				mn_RunCarrierStatus = 600;
// 			}
// 			break;
// 
// 		case 600:
// 			//Loader의 Up down의 carrier 유무 체크
// // 			st_io.i_Carrier_Z_2_Up_Check					=	406;//Jig 감지 			///PS0306
// // 			st_io.i_Carrier_Z_2_Down_Check					=	407;//Jig 감지 			///PS0307
// 			nRet[0] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_ON);
// 			nRet[1] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);
// 			nRet[2] = g_ioMgr.get_in_bit( st_io.i_Carrier_Z_2_Up_Check, IO_ON );
// 			nRet[3] = g_ioMgr.get_in_bit( st_io.i_Carrier_Z_2_Down_Check, IO_ON );
// 			if( nRet[0] == IO_OFF && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_ON )
// 			{
// 				mn_RunCarrierStatus = 1000;
// 			}
// 			else
// 			{
// 				if( nRet[0] == IO_ON )	m_strAlarmCode.Format("8%d04d", IO_ON, st_io.i_Carrier_Z_1_Up_Check);
// 				else if( nRet[1] == IO_ON ) m_strAlarmCode.Format("8%d04d", IO_ON, st_io.i_Carrier_Z_1_Down_Check);
// 				else if( nRet[2] == IO_ON )	m_strAlarmCode.Format("8%d04d", IO_OFF, st_io.i_Carrier_Z_2_Up_Check);
// 				else/* if( nRet[3] == IO_ON )*/ m_strAlarmCode.Format("8%d04d", IO_OFF, st_io.i_Carrier_Z_2_Down_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);
// 			}
// 			break;
// 
// 		case 1000:
// 			st_sync.nCarrierBcr_Req = CTL_REQ;
// 			m_dwBcrWaitTime[0] = GetCurrentTime();
// 			mn_RunCarrierStatus = 1100;
// 			break;
// 
// 		case 1100:
// 			m_dwBcrWaitTime[1] = GetCurrentTime();
// 			m_dwBcrWaitTime[2] = m_dwBcrWaitTime[1] - m_dwBcrWaitTime[0]
// 			if( m_dwBcrWaitTime[2] <= 0 ) m_dwBcrWaitTime[0] = GetCurrentTime();
// 
// 			if( st_sync.nCarrierBcr_Req == CTL_READY )
// 			{
// 				mn_RunCarrierStatus = 1200;
// 			}
// 			else
// 			{
// 				if(  m_dwBcrWaitTime[2] > 5000 )
// 				{//940000 1 A "BARCODE_IS_NOT_RESPONSE."
// 					CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "940000");	
// 					mn_RunCarrierStatus = 1000;
// 				}
// 			}
// 			break;
// 
// 		case 1200:
// 			nRet_1 = atoi(st_msg.mstr_barcode);
// 			if( nRet_1 > 0 && nRet_1 < 13)
// 			{
// 				for ( i = 0; i < 3; i++ )
// 				{
// 					sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i], "%d",nRet_1);
// 				}			
// 				mn_RunCarrierStatus = 1300;
// 			}
// 			else
// 			{//940001 1 A "BARCODE_IS_NOT_CORRECT_BARCODE_NUMBER."
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "940001");
// 				mn_RunCarrierStatus = 1000;
// 			}
// 			break;
// 
// 		case 1300:
// 			if( CheckCarrierType() == RET_GOOD )
// 			{
// 				mn_RunCarrierStatus = 2000;
// 			}
// 			else if( CheckCarrierType() == RET_ERROR )
// 			{
// 				m_dwWaitTime[0] = GetCurrentTime();
// 				mn_RunCarrierStatus = 4000;
// 			}
// 			else
// 			{//950000 1 A "PRESS_CARRIER_TYPE_CHECK_ERROR_PS2312_PS2313_PS2314."
// 				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "950000");
// 			}
// 			break;
// 
// 		//1. nMode = 0 UNPRESS
// 		case 2000:
// 			nRet[0] = CarrierTopForward( 0 );
// 			if( nRet[0] == RET_GOOD )
// 			{
// 				mn_RunCarrierStatus = 2100;
// 			}
// 			break;
// 
// 			//랏이 있는지와 공급할 랏이 있는지 체크
// 		case 2100:
// 			m_nFindLotNo_Flag = -1;
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
// 				if( g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
// 				{
// 					m_nFindLotNo_Flag = 0;
// 					m_strFindLotNo = g_lotMgr.GetLotAt(0).GetLotID();
// 				}
// 				else if( g_lotMgr.GetLotCount() >= 2 )
// 				{
// 					if( g_lotMgr.GetLotAt(1).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
// 					{
// 						m_nFindLotNo_Flag = 1;
// 						m_strFindLotNo = g_lotMgr.GetLotAt(0).GetLotID();
// 					}
// 					else
// 					{
// 						//
// 					}
// 				}
// 
// 			}
// 			else
// 			{
// 				return nFuncRet;
// 			}
// 
// 			if( m_nFindLotNo_Flag >= 0)
// 			{//언로드 자재가 존재하는지 체크한다.
// 				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[TOP] == CTL_YES )
// 				{
// 					if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[TOP] != BIN_LDBUFFERBIN)
// 					{
// 					}
// 					else
// 					{
// 					}
// 				}
// 				else
// 				{
// 					//
// 				}
// 			else if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[MIDDLE] == CTL_YES )
// 			{
// 				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[MIDDLE] != BIN_LDBUFFERBIN )
// 				{
// 				}
// 				else
// 				{
// 				}
// 
// 			}
// 			else if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[BTM] == CTL_YES )
// 			{
// 				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[BTM] != BIN_LDBUFFERBIN)
// 				{
// 				}
// 				else
// 				{
// 				}
// 			}
// 			}
// 			else//언로드 자재만 존재한다,
// 			{
// 
// 			}
// 
// 
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
// 				if( g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
// 				{
// 					m_nFindLotNo_Flag = 0;
// 					m_strFindLotNo = g_lotMgr.GetLotAt(0).GetLotID();
// 				}
// 				else if( g_lotMgr.GetLotCount() >= 2 )
// 				{
// 					if( g_lotMgr.GetLotAt(1).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
// 					{
// 						m_nFindLotNo_Flag = 1;
// 						m_strFindLotNo = g_lotMgr.GetLotAt(0).GetLotID();
// 					}
// 					else
// 					{
// 						return nFuncRet;
// 					}
// 				}
// 			}
// 
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
// 				if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
// 				{
// 					//load plate에 자재 요청
// 					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;
// 
// 					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ;
// 					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE;
// 					m_nFindLotNo_Flag = 0;
// 				}
// 				else if( g_lotMgr.GetLotCount() >= 2 )
// 				{
// 					if( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
// 					{
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;
// 
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ;
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE;
// 						m_nFindLotNo_Flag = 1;
// 					}
// 					else
// 					{
// 						if( COMI.Get_MotCurrentPos(m_nRobot_Y) >= ( st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] + st_motor[m_nRobot_Y].mn_allow ) )
// 						{
// 							nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
// 							if (nRet_1 == BD_GOOD)
// 							{
// 								mn_RunCarrierStatus = 500;
// 							}
// 							else if (nRet_1 == BD_RETRY)
// 							{
// 								mn_RunCarrierStatus = 500;
// 							}
// 							else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 							{
// 								CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
// 								mn_RunCarrierStatus = 500;
// 							}
// 						}					
// 
// 					}
// 
// 
// 
// 
// 		case 1400:
// 			st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_REQ;
// 			st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
// 			st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
// 			mn_RunCarrierStatus = 2100;
// 			break;
// 
// 		case 1500:
// 			if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY )
// 			{
// 				mn_RunCarrierStatus = 1100;
// 			}
// 			break;
// 
// 
// 	}
// 
// 	return nFuncRet;
// }


int CRun_Device_Carrier_Robot::CarrierBtmForwrad()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1 = 0, nRet_2 = 0;
	CString strTmp;

	Func.ThreadFunctionStepTrace(14, mn_RunBtmFwdStep);
	switch( mn_RunBtmFwdStep)
	{
		case 0:
			mn_RunBtmFwdStep = 1000;
			break;

		case 1000:
			//N Y Y Y Y Y Y
			//Y Y Y Y Y Y N
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);//하단 지그체크
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
			if( nRet_1 == IO_ON && nRet_2 == IO_OFF)
			{
				mn_RunBtmFwdStep = 2000;
			}
			else
			{
				if( nRet_1 == IO_OFF ) strTmp.Format("8%d%04d", IO_OFF, st_io.i_Carrier_Z_1_Down_Check);
				else                          strTmp.Format("8%d%04d", IO_ON, st_io.i_Carrier_Z_2_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, strTmp);
			}
			break;

		case 2000:
			Set_Device_Carrier_Slide_Bottom_X_ForBackward( IO_ON );
			mn_RunBtmFwdStep = 2100;
			break;

		case 2100:
			nRet_1 =  Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_ON);
			if( nRet_1 == IO_ON )
			{
				mn_RunBtmFwdStep = 3000;
			}
			break;

		case 3000:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);//하단 지그체크
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);
			if( nRet_1 == IO_OFF && nRet_2 == IO_ON)
			{
				mn_RunBtmFwdStep = 4000;
			}
			else
			{
				if( nRet_1 == IO_OFF ) strTmp.Format("8%d%04d", IO_ON, st_io.i_Carrier_Z_1_Down_Check);
				else                          strTmp.Format("8%d%04d", IO_OFF, st_io.i_Carrier_Z_2_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, strTmp);
			}
			break;

		case 4000:
			Set_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF );
			mn_RunBtmFwdStep = 4100;
			break;

		case 4100:
			nRet_1 =  Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF);
			if( nRet_1 == IO_ON )
			{
				mn_RunBtmFwdStep = 5000;
			}
			break;

		case 5000:
			mn_RunBtmFwdStep = 0;
			nFuncRet = RET_GOOD;
			break;
		}

	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
//CarrierTopForward()함수
//1. nMode = 0 UNPRESS
//2 nMode = 1  PUSH FORWARD
//////////////////////////////////////////////////////////////////////////
int CRun_Device_Carrier_Robot::CarrierTopForward( int nMode )
{
	int nFuncRet = RET_PROCEED;
	int nRet_1 = 0, nRet_2 = 0;

	Func.ThreadFunctionStepTrace(15, mn_RunTopFwdStep);
	switch( mn_RunTopFwdStep)
	{
		case 0:
			if( nMode < 0 || nMode > 1 )
			{//902000 1 A "THERE_IS_NO_MODE_IN_CARRIER_TOP_FORWARD."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "902000");
				nFuncRet = RET_ERROR;
				break;
			}
			mn_RunTopFwdStep = 100;
			break;

		//덜 밀렸거나 중간이 비었거나 다른문제가 발생할 수 있음
		//에러시 센서 확인철저히 manual로 확인해야 함 think!think!think!
		case 100:
			nRet_1 = CheckCarrierType();
			if( nRet_1 == RET_GOOD )
			{
				//if( nMode == 0 )
				//{
					mn_RunTopFwdStep = 1000;//전부 OFF이므로 CDIMM 동작 가능
				//}
				//else
				//{
				//	break;
				//}
			}
			else if( CheckCarrierType() == RET_ERROR )//혹시 SND?
			{
				//if( nMode == 1 )
				//{
					mn_RunTopFwdStep = 3000;//전부 OFF이므로 CDIMM 동작 가능
				//}
				//else
				//{
				//	break;
				//}
			}
			else
			{//950000 1 A "PRESS_CARRIER_TYPE_CHECK_ERROR_PS2312_PS2313_PS2314."
// 				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, "950000");
// 				break;
			}
//			mn_RunTopFwdStep = 1000;			
			break;

		case 1000:
// 			if( nMode == 0)
// 			{
				mn_RunTopFwdStep = 1100;
// 			}
// 			else
// 			{
// 				mn_RunTopFwdStep = 3000;
// 			}
			break;

		case 1100:
			//1. nMode = 0 UNPRESS
			if( st_sync.nCarrierSateyflag[LDPICKER] == RET_GOOD && st_sync.nCarrierSateyflag[ULDPICKER] == RET_GOOD &&
				st_sync.nCarrierSateyflag[UNPRESS_Y] == RET_GOOD)
			{
				mn_RunTopFwdStep = 1200;
			}
			break;

		case 1200:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_NO )
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_REQ;
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PICK;
				mn_RunTopFwdStep = 1300;
			}
			break;

		case 1300:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_READY && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_CHANGE;
				mn_RunTopFwdStep = 1400;
			}
			break;

		case 1400:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_FREE && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_NO;
				mn_RunTopFwdStep = 6000;
			}
			break;

		case 3000://한칸 민다.
			if( st_sync.nCarrierSateyflag[LDPICKER] == RET_GOOD && st_sync.nCarrierSateyflag[ULDPICKER] == RET_GOOD &&
				st_sync.nCarrierSateyflag[UNPRESS_Y] == RET_GOOD)
			{
				mn_RunTopFwdStep = 3100;
			}
			break;

		case 3100:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_NO)
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_REQ;
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PLACE;
				mn_RunTopFwdStep = 3200;
			}
			break;

		case 3200:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_READY && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE)
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_CHANGE;
				mn_RunTopFwdStep = 3300;
			}
			break;

		case 3300:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_FREE && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE)
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_NO;
				mn_RunTopFwdStep = 7000;
			}
			break;

		case 6000:
			mn_RunTopFwdStep = 0;
			nFuncRet = RET_GOOD;
			break;

		case 7000:
			mn_RunTopFwdStep = 0;
			nFuncRet = RET_SKIP;
			break;

		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no RunTopFwdStep at CarrierTopForward in Run_Device_Carrier_Robot  = %d", mn_RunTopFwdStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}

	return nFuncRet;
}


int CRun_Device_Carrier_Robot::CheckCarrierType()
{
	int nFuncRet = RET_PROCEED;

	int nRet_1 = g_ioMgr.get_in_bit(st_io.i_press_carrier_type1, IO_OFF);
	int nRet_2 = g_ioMgr.get_in_bit(st_io.i_press_carrier_type2, IO_OFF);
	int nRet_3 = g_ioMgr.get_in_bit(st_io.i_press_carrier_type3, IO_OFF);

	if( st_basic.n_mode_jig == WITHOUT_JIG )
	{
		nFuncRet = RET_GOOD;
	}
	else if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && nRet_3 == IO_OFF)
	{
		nFuncRet = RET_GOOD;
	}
	else if( nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON)
	{
		nFuncRet = RET_ERROR;
	}
	return nFuncRet;
}



//위에서 캐리어를 밀때의 동작 상태를 체크한다
//1.캐리어 down위치에 하나가 비어 있어야 하고
//2.디바이스 로드 위치에 케리어가 있어야 한다.
//3.각 위치의 모터는 safety 한다.
//4.down 위치의 캐리어가 움지이고 잇는 상태는 괜찮은데 캐리어 버퍼를 올리거나 내리는 동작을 할려거나 하고 있으면 안된다.
//nMode = 0 초기 상태            N Y Y Y Y Y Y
//                                          N Y Y Y Y Y Y
//nMode = 1 상단 민 상태        Y Y Y Y Y Y N
//								            N Y Y Y Y Y Y
//nMode = 2 상단 다운 상태     N Y Y Y Y Y N
//										    Y Y Y Y Y Y Y
//nMode = 3 한다 업 상태        N Y Y Y Y Y Y
//										    Y Y Y Y Y Y N
//다시 초기 상태	하단 민 상태	N Y Y Y Y Y Y
//										    N Y Y Y Y Y Y
int CRun_Device_Carrier_Robot::Check_Carrier_Move_Enable( int nMode)
{
	int nFuncRet = RET_ERROR;
	int nRet[20] = {0,};
	nRet[0] = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);	// Press Unpress
	nRet[1] = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
	nRet[2] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_OFF);	// 하단에서 Jig 밀어줌 
	nRet[3] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_ON);
	nRet[4] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);		// 상단에서 Jig 밀어줌 
	nRet[5] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
	nRet[6] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Up_Check, IO_OFF);		// Left z
	nRet[7] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
	nRet[8] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Up_Check, IO_OFF);		// Right Z
	nRet[9] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
	nRet[10] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);		// Left 끝단에서 Jig 걸어줌 
	nRet[11] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, IO_OFF);
	nRet[12] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_ON);		// Right 끝단에서 Jig 걸어줌 
	nRet[13] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);		
// 	nRet[14] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check);	// 상단 중간에서 Jig Hole에 집어넣어 JIg 잡아줌 
// 	nRet[15] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check);	
// 	nRet[16] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Forward_Check);		// 상단 중간에서 jig 앞뒤로 눌러서 jig 잡아줌 
// 	nRet[17] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Backward_Check);

// 	931000 1 A "CARRIRER_IS_NOT_INIT_STATUS."
// 	931001 1 A "CARRIRER_IS_NOT_PUSHED_STATUS_ON_TOP."
// 	931002 1 A "CARRIRER_IS_NOT_DOWN_STATUS_ON_BTM."
// 	931003 1 A "CARRIRER_IS_NOT_UP_STATUS_ON_BTM."
//	931004 1 A "CARRIRER_IS_NOT_STATUS_MODE."
	if( nMode == 0)// 초기 상태
	{
		nRet[14] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);	
		nRet[15] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);	
		nRet[16] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_ON);	
		nRet[17] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);	
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_OFF; nRet[15] = IO_OFF; nRet[16] = IO_ON;	nRet[17] = IO_ON;
		}
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON && nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF && 
			nRet[9] == IO_ON && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON && nRet[13] == IO_OFF && nRet[14] == IO_OFF && nRet[15] == IO_OFF && nRet[16] == IO_ON && nRet[17] == IO_ON )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "931000");
		}
	}
	else if ( nMode == 1 )//상단 민 상태
	{
		nRet[14] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_ON);	
		nRet[15] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);	
		nRet[16] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);	
		nRet[17] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);
		nRet[14] = IO_ON; nRet[15] = IO_OFF;

		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_ON; nRet[15] = IO_OFF; nRet[16] = IO_OFF;	nRet[17] = IO_ON;
		}	
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON && nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF && 
			nRet[9] == IO_ON && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON && nRet[13] == IO_OFF && nRet[14] == IO_ON && nRet[15] == IO_OFF && nRet[16] == IO_OFF && nRet[17] == IO_ON )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "931001");
		}
	}
	else if( nMode == 2 )//2 상단 다운 상태
	{
		nRet[14] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);	
		nRet[15] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);	
		nRet[16] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);	
		nRet[17] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);
		nRet[14] = IO_OFF; nRet[15] = IO_ON;
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_OFF; nRet[15] = IO_ON; nRet[16] = IO_OFF;	nRet[17] = IO_ON;
		}
		nRet[17] = IO_ON;
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON && nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF && 
			nRet[9] == IO_ON && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON && nRet[13] == IO_OFF && nRet[14] == IO_OFF && nRet[15] == IO_ON && nRet[16] == IO_OFF && nRet[17] == IO_ON )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			//CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "931002");
		}
	}
	else if(nMode == 3)//3 한다 업 상태
	{
		nRet[14] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);	
		nRet[15] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);	
		nRet[16] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_ON);	
		nRet[17] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
		nRet[14] = IO_OFF; nRet[15] = IO_ON;
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_OFF; nRet[15] = IO_ON; nRet[16] = IO_ON;	nRet[17] = IO_OFF;
		}
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON && nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF && 
			nRet[9] == IO_ON && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON && nRet[13] == IO_OFF && nRet[14] == IO_OFF && nRet[15] == IO_ON && nRet[16] == IO_ON && nRet[17] == IO_OFF )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "931003");
		}
	}
	else
	{
		CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "931004");
	}

	return nFuncRet;
}


//로보트가 Top의 Carrier Slide를 밀기 위해 로보트의 UPDOWN실린더
// void CRun_Device_Carrier_Robot::Set_Device_Carrier_Slide_Top_X_UpDown(int OnOff)
// {
// 	CString strLogKey[10];
// 	CString	strLogData[10];
// 
// 	strLogKey[0] = _T("Mode Start");
// 	strLogData[0].Format(_T("%d"),0);
// 
// 	m_bClampOnOffFlag	= false;
// 	m_dwClampOnOff[0]	= GetCurrentTime();
// 
// 	g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, OnOff);
// 	g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, !OnOff);
// 
// 	if (OnOff == IO_ON)//다운
// 	{
// 		clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("TOP_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
// 	}
// 	else
// 	{
// 		clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),0,_T("TOP_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
// 	}
// 
// }
// 
// int CRun_Device_Carrier_Robot::Chk_Device_Carrier_Slide_Top_X_UpDown( int OnOff )
// {
// 	CString strLogKey[10];
// 	CString	strLogData[10];
// 
// 	strLogKey[0] = _T("Mode End");
// 	strLogData[0].Format(_T("%d"),0);
// 
// 	int nWaitTime = WAIT_CARRIER_CLAMP_FWDBWD;
// 
// 	if (OnOff == IO_OFF)//DOWN
// 	{
// 		if (m_bClampOnOffFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_ON)	== IO_ON &&
// 			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_OFF) == IO_OFF)
// 		{
// 			m_bClampOnOffFlag		= true;
// 			m_dwClampOnOff[0]	= GetCurrentTime();
// 		}
// 		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_ON)	== IO_ON &&
// 			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_OFF) == IO_OFF)
// 		{
// 			m_dwClampOnOff[1] = GetCurrentTime();
// 			m_dwClampOnOff[2] = m_dwClampOnOff[1] - m_dwClampOnOff[0];
// 
// 			if (m_dwClampOnOff[2] <= 0)
// 			{
// 				m_dwClampOnOff[0] = GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if (m_dwClampOnOff[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
// 			{
// 				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),1,_T("TOP_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
// 				return RET_GOOD;
// 			}
// 		}
// 		else
// 		{
// 			m_dwClampOnOff[1] = GetCurrentTime();
// 			m_dwClampOnOff[2] = m_dwClampOnOff[1] - m_dwClampOnOff[0];
// 
// 			if (m_dwClampOnOff[2] <= 0)
// 			{
// 				m_dwClampOnOff[0] = GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if (m_dwClampOnOff[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
// 			{
// 				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Slide_Guide_X2_Down_Check); 
// 				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),1,_T("TOP_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
// 				return RET_ERROR;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (m_bClampOnOffFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF)	== IO_OFF &&
// 			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON) == IO_ON)
// 		{
// 			m_bClampOnOffFlag			= true;
// 			m_dwClampOnOff[0]	= GetCurrentTime();
// 		}
// 		else if (m_bClampOnOffFlag == true &&	g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF)	== IO_OFF &&
// 			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON) == IO_ON)
// 		{
// 			m_dwClampOnOff[1]	= GetCurrentTime();
// 			m_dwClampOnOff[2]	= m_dwClampOnOff[1] - m_dwClampOnOff[0];
// 
// 			if (m_dwClampOnOff[2] <= 0)
// 			{
// 				m_dwClampOnOff[0]	= GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if(m_dwClampOnOff[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
// 			{
// 				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),1,_T("TOP_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
// 				return RET_GOOD;
// 			}
// 		}
// 		else
// 		{
// 			m_dwClampOnOff[1]	= GetCurrentTime();
// 			m_dwClampOnOff[2]	= m_dwClampOnOff[1] - m_dwClampOnOff[0];
// 
// 			if (m_dwClampOnOff[2] <= 0)
// 			{
// 				m_dwClampOnOff[0]	= GetCurrentTime();
// 				return RET_PROCEED;
// 			}
// 
// 			if (m_dwClampOnOff[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
// 			{
// 				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Slide_Guide_X2_Down_Check); 
// 				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),1,_T("TOP_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
// 				return RET_ERROR;
// 			}
// 		}
// 	}
// 
// 	return RET_PROCEED;
// }

void CRun_Device_Carrier_Robot::Set_Device_Carrier_Slide_Bottom_X_ForBackward(int OnOff)	
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bBtmFwdBwdFlag	= false;
	m_dwBtmFwdBwd[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X1_Forward_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X1_Backward_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),0,_T("BTM_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),0,_T("BTM_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
// 	switch(OnOff)
// 	{
// 	case ON://Up
// 		g_ioMgr.Set_Out_Bit(st_io.o_Slide_Guide_X1_Forward_Sol, IO_ON);
// 		g_ioMgr.Set_Out_Bit(st_io.o_Slide_Guide_X1_Backward_Sol, IO_OFF);
// 		PublicFunction.MessageDisplay("		Carrier Bottom X Forward");
// 		break;
// 	case OFF://Down
// 		g_ioMgr.Set_Out_Bit(st_io.o_Slide_Guide_X1_Forward_Sol, IO_OFF);
// 		g_ioMgr.Set_Out_Bit(st_io.o_Slide_Guide_X1_Backward_Sol, IO_ON);
// 		PublicFunction.MessageDisplay("		Carrier Bottom X Backward");
// 		break;
// 	}
}


int CRun_Device_Carrier_Robot::Chk_Device_Carrier_Slide_Bottom_X_ForBackward( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CARRIER_CLAMP_FWDBWD;

	if (OnOff == IO_OFF)
	{
		if (m_bBtmFwdBwdFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_OFF)	== IO_OFF)
		{
			m_bBtmFwdBwdFlag		= true;
			m_dwBtmFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bBtmFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_OFF)	== IO_OFF)
		{
			m_dwBtmFwdBwd[1] = GetCurrentTime();
			m_dwBtmFwdBwd[2] = m_dwBtmFwdBwd[1] - m_dwBtmFwdBwd[0];

			if (m_dwBtmFwdBwd[2] <= 0)
			{
				m_dwBtmFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwBtmFwdBwd[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),1,_T("BTM_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwBtmFwdBwd[1] = GetCurrentTime();
			m_dwBtmFwdBwd[2] = m_dwBtmFwdBwd[1] - m_dwBtmFwdBwd[0];

			if (m_dwBtmFwdBwd[2] <= 0)
			{
				m_dwBtmFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwBtmFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Slide_Guide_X1_Backward_Check); 
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),1,_T("BTM_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bBtmFwdBwdFlag == false && g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_ON)	== IO_ON)
		{
			m_bBtmFwdBwdFlag			= true;
			m_dwBtmFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bBtmFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_ON)	== IO_ON)
		{
			m_dwBtmFwdBwd[1]	= GetCurrentTime();
			m_dwBtmFwdBwd[2]	= m_dwBtmFwdBwd[1] - m_dwBtmFwdBwd[0];

			if (m_dwBtmFwdBwd[2] <= 0)
			{
				m_dwBtmFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwBtmFwdBwd[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),1,_T("BTM_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwBtmFwdBwd[1]	= GetCurrentTime();
			m_dwBtmFwdBwd[2]	= m_dwBtmFwdBwd[1] - m_dwBtmFwdBwd[0];

			if (m_dwBtmFwdBwd[2] <= 0)
			{
				m_dwBtmFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwBtmFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Slide_Guide_X1_Backward_Check); 
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),1,_T("BTM_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}


//Left Carrier Clamp 벌림 오무림 : 0
//Right Carrier Clamp 벌림 오무림 : 1
void CRun_Device_Carrier_Robot::Set_Device_Carrier_Clamp_FwdBwd(int nMode, int OnOff)	
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0].Format(_T("Mode%02d Start"), nMode);
	strLogData[0].Format(_T("%d"),0);

	m_bCarrierClampFlag[nMode]	= false;
	m_dwCarrierClampFwdBwd[nMode][0] = GetCurrentTime();

	//OnOff -> ON://Up
	if( nMode == 0 )//LEFT
	{
		g_ioMgr.set_out_bit(st_io.o_Carrier_Clamp_1_Forward_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Carrier_Clamp_1_Backward_Sol, !OnOff);
	}
	else
	{
		g_ioMgr.set_out_bit(st_io.o_Carrier_Clamp_2_Forward_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Carrier_Clamp_2_Backward_Sol, !OnOff);
	}

	if (OnOff == IO_ON)
	{
		if( nMode == 0 )
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("OPEN"),0,_T("LEFT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
		else
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("OPEN"),0,_T("RIGHT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		if( nMode == 0 )
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("CLOSE"),0,_T("LEFT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
		else
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("CLOSE"),0,_T("RIGHT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_Device_Carrier_Robot::Chk_Device_Carrier_Clamp_FwdBwd( int nMode, int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0].Format(_T("Mode%02d End"), nMode);
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CARRIER_CLAMP_FWDBWD;

	int nRet_1 = 0, nRet_2 = 0;

	//Left Carrier Clamp 벌림 오무림 : 0
	//Right Carrier Clamp 벌림 오무림 : 1
	if( nMode == 0)
	{
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, !OnOff);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, OnOff);
	}
	else
	{
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, !OnOff);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, OnOff);
	}

// 	nRet[0] = g_ioMgr.Get_In_Bit(st_io.i_Carrier_1_Forward_Check);
// 	nRet[1] = g_ioMgr.Get_In_Bit(st_io.i_Carrier_1_Backward_Check);
	if (OnOff == IO_OFF)
	{
		if (m_bCarrierClampFlag[nMode] == false && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_bCarrierClampFlag[nMode] = true;
			m_dwCarrierClampFwdBwd[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCarrierClampFlag[nMode]== true && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_dwCarrierClampFwdBwd[nMode][1] = GetCurrentTime();
			m_dwCarrierClampFwdBwd[nMode][2]= m_dwCarrierClampFwdBwd[nMode][1] - m_dwCarrierClampFwdBwd[nMode][0];

			if (m_dwCarrierClampFwdBwd[nMode][2] <= 0)
			{
				m_dwCarrierClampFwdBwd[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierClampFwdBwd[nMode][2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				if( nMode == 0)
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("CLOSE"),1,_T("LEFT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				else
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("CLOSE"),1,_T("RIGHT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierClampFwdBwd[nMode][1] = GetCurrentTime();
			m_dwCarrierClampFwdBwd[nMode][2] = m_dwCarrierClampFwdBwd[nMode][1] - m_dwCarrierClampFwdBwd[nMode][0];

			if (m_dwCarrierClampFwdBwd[nMode][2] <= 0)
			{
				m_dwCarrierClampFwdBwd[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierClampFwdBwd[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				if( nMode == 0)
				{
					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Carrier_1_Backward_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("CLOSE"),1,_T("LEFT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Carrier_2_Backward_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("CLOSE"),1,_T("RIGHT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCarrierClampFlag[nMode] == false && nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			m_bCarrierClampFlag[nMode] = true;
			m_dwCarrierClampFwdBwd[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCarrierClampFlag[nMode] == true && nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			m_dwCarrierClampFwdBwd[nMode][1]	= GetCurrentTime();
			m_dwCarrierClampFwdBwd[nMode][2]	= m_dwCarrierClampFwdBwd[nMode][1] - m_dwCarrierClampFwdBwd[nMode][0];

			if (m_dwCarrierClampFwdBwd[nMode][2] <= 0)
			{
				m_dwCarrierClampFwdBwd[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCarrierClampFwdBwd[nMode][2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				if( nMode == 0)
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("OPEN"),1,_T("LEFT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				else
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("OPEN"),1,_T("RIGHT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierClampFwdBwd[nMode][1]	= GetCurrentTime();
			m_dwCarrierClampFwdBwd[nMode][2]	= m_dwCarrierClampFwdBwd[nMode][1] - m_dwCarrierClampFwdBwd[nMode][0];

			if (m_dwCarrierClampFwdBwd[nMode][2] <= 0)
			{
				m_dwCarrierClampFwdBwd[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierClampFwdBwd[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				if( nMode == 0)
				{
					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Carrier_1_Forward_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("OPEN"),1,_T("LEFT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Carrier_2_Forward_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("OPEN"),1,_T("RIGHT_CARRIER"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}


//LEFT CARRIER UP DOWN 0
//RIGHT CARRIER UP DOWN 1
void CRun_Device_Carrier_Robot::Set_Device_Carrier_Slide_Z_Cylinder_UpDown(int nMode, int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0].Format(_T("Mode%02d Start"), nMode);
	strLogData[0].Format(_T("%d"),0);

	m_bCarrierSlideGuide[nMode]	= false;
	m_dwCarrierGuideUpDn[nMode][0] = GetCurrentTime();

	//OnOff -> ON://Up

	if( nMode == 0 )
	{
		g_ioMgr.set_out_bit(st_io.o_Slide_Guide_Z1_Up_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Slide_Guide_Z1_Down_Sol, !OnOff);
	}
	else
	{
		g_ioMgr.set_out_bit(st_io.o_Slide_Guide_Z2_Up_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Slide_Guide_Z2_Down_Sol, !OnOff);
	}

	if (OnOff == IO_ON)
	{
		if( nMode == 0 )
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),0,_T("SLIDE_GUIDE1"),_T("CYLINDER"),1,strLogKey,strLogData);
		else
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),0,_T("SLIDE_GUIDE2"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		if(nMode == 0)
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("SLIDE_GUIDE1"),_T("CYLINDER"),1,strLogKey,strLogData);
		else
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("SLIDE_GUIDE2"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_Device_Carrier_Robot::Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( int nMode, int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0].Format(_T("Mode%02d End"), nMode);
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CARRIER_SLIDE_UPDN;

	int nRet_1 = 0, nRet_2 = 0;

	if( nMode == 0 )
	{
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Up_Check, nOnOff) ;
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Down_Check, !nOnOff);
	}
	else
	{
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Up_Check, nOnOff) ;
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Down_Check, !nOnOff);
	}

	if (nOnOff == IO_OFF)//DOWN
	{
		if (m_bCarrierSlideGuide[nMode] == false && nRet_1 == IO_OFF &&	nRet_2 == IO_ON )
		{
			m_bCarrierSlideGuide[nMode]		= true;
			m_dwCarrierGuideUpDn[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCarrierSlideGuide[nMode] == true && nRet_1 == IO_OFF &&	nRet_2 == IO_ON )
		{
			m_dwCarrierGuideUpDn[nMode][1] = GetCurrentTime();
			m_dwCarrierGuideUpDn[nMode][2] = m_dwCarrierGuideUpDn[nMode][1] - m_dwCarrierGuideUpDn[nMode][0];

			if (m_dwCarrierGuideUpDn[nMode][2] <= 0)
			{
				m_dwCarrierGuideUpDn[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierGuideUpDn[nMode][2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				if( nMode == 0 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("SLIDE_GUIDE1"),_T("CYLINDER"),1,strLogKey,strLogData);
				else
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("SLIDE_GUIDE2"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierGuideUpDn[nMode][1] = GetCurrentTime();
			m_dwCarrierGuideUpDn[nMode][2] = m_dwCarrierGuideUpDn[nMode][1] - m_dwCarrierGuideUpDn[nMode][0];

			if (m_dwCarrierGuideUpDn[nMode][2] <= 0)
			{
				m_dwCarrierGuideUpDn[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierGuideUpDn[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				if( nMode == 0 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Slide_Guide_Z1_Down_Check); 
					clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("DOWN"),1,_T("SLIDE_GUIDE1"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Slide_Guide_Z2_Down_Check); 
					clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("DOWN"),1,_T("SLIDE_GUIDE2"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCarrierSlideGuide[nMode] == false && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_bCarrierSlideGuide[nMode]			= true;
			m_dwCarrierGuideUpDn[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCarrierSlideGuide[nMode] == true && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_dwCarrierGuideUpDn[nMode][1]	= GetCurrentTime();
			m_dwCarrierGuideUpDn[nMode][2]	= m_dwCarrierGuideUpDn[nMode][1] - m_dwCarrierGuideUpDn[nMode][0];

			if (m_dwCarrierGuideUpDn[nMode][2] <= 0)
			{
				m_dwCarrierGuideUpDn[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCarrierGuideUpDn[nMode][2]> (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				if( nMode == 0 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("SLIDE_GUIDE1"),_T("CYLINDER"),1,strLogKey,strLogData);
				else
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("SLIDE_GUIDE2"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierGuideUpDn[nMode][1]	= GetCurrentTime();
			m_dwCarrierGuideUpDn[nMode][2]	= m_dwCarrierGuideUpDn[nMode][1] - m_dwCarrierGuideUpDn[nMode][0];

			if (m_dwCarrierGuideUpDn[nMode][2] <= 0)
			{
				m_dwCarrierGuideUpDn[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierGuideUpDn[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				if( nMode == 0 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Slide_Guide_Z1_Down_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("SLIDE_GUIDE1"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Slide_Guide_Z2_Down_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("SLIDE_GUIDE2"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}


//mode 0 1번째 - sol만 있고 센서는 없다
//mode 1 2,3,6번째
//mode 2 5번째
void CRun_Device_Carrier_Robot::Set_Device_Carrier_HolderPin_Fix(int nMode, int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0].Format(_T("Mode%02d Start"), nMode+1);
	strLogData[0].Format(_T("%d"),0);

	if( nMode < 0 || nMode > 2)
	{
		return;
	}

	m_bCarrierPinFix[nMode]	= false;
	m_dwCarrierPinFix[nMode][0] = GetCurrentTime();
	

	if( nMode == 0 )
	{		
		g_ioMgr.set_out_bit(st_io.o_Press_PIN_Guide_Forward_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Press_PIN_Guide_Bakcward_Sol, !OnOff);
	}
	else if( nMode == 1 )//ON->DOWN OFF->UP
	{		
		g_ioMgr.set_out_bit(st_io.o_Press_Carrier_Holder_Down_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Press_Carrier_Holder_Up_Sol, !OnOff);
	}
	else if( nMode == 2 )//ON->fwd OFF->bwd
	{		
		g_ioMgr.set_out_bit(st_io.o_Camera_Y_Jig_Press_Forward_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Camera_Y_Jig_Press_Backward_Sol, !OnOff);
	}
	else if( nMode == 3 )
	{		
		g_ioMgr.set_out_bit(st_io.o_Camera_Y_Press_Down_Sol, OnOff);
		g_ioMgr.set_out_bit(st_io.o_Camera_Y_Press_Up_Sol, !OnOff);
	}
	else if( nMode == 4 )
	{
		g_ioMgr.set_out_bit( st_io.o_Press_Down_Sol, OnOff);
		g_ioMgr.set_out_bit( st_io.o_Press_Up_Sol, !OnOff);
	}

	if (OnOff == IO_ON)//DOWN FWD
	{
		if( nMode == 2 )
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),0,_T("CAMERA_Y_JIG_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
		else
		{
			if( nMode == 0 )
			{
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
			}
			else if( nMode == 1 )
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
			else if( nMode == 3 )
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("CAMERA_Y_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
			else if( nMode == 4 )
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);		
		}
	}
	else
	{
		if( nMode == 2 )
			clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),0,_T("CAMERA_Y_JIG_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
		else
		{
			if( nMode == 0 )
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);	
			else if( nMode == 3 )
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CAMERA_Y_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
			else if( nMode == 4 )
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),0,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);		
		}
	}
}

int CRun_Device_Carrier_Robot::Chk_Device_Carrier_HolderPin_Fix(int nMode, int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime;

	int nRet_1 = 0, nRet_2 = 0;

	if( nMode == 1 )//ON->down OFF->up
	{
		nWaitTime = WAIT_CARRIER_HOLDER_UPDN;
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, !nOnOff);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, nOnOff);
	}
	else if( nMode == 2 )//ON->Fwd OFF->Bwd
	{
		nWaitTime = WAIT_CARRIER_HOLDER_UPDN;
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, !nOnOff);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, nOnOff);
	}
	else if( nMode == 3 )//ON->down OFF->up
	{
		nWaitTime = WAIT_CARRIER_PRESS_UPDN;
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, !nOnOff);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, nOnOff);
	}
	else if( nMode == 4 )//ON->down OFF->up
	{
		nWaitTime = WAIT_CARRIER_PRESS_UPDN;
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, !nOnOff);
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, nOnOff);
	}

	if (nOnOff == IO_OFF)//DOWN BWD
	{
		if (m_bCarrierPinFix[nMode] == false && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_bCarrierPinFix[nMode] = true;
			m_dwCarrierPinFix[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCarrierPinFix[nMode] == true && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_dwCarrierPinFix[nMode][1] = GetCurrentTime();
			m_dwCarrierPinFix[nMode][2] = m_dwCarrierPinFix[nMode][1] - m_dwCarrierPinFix[nMode][0];

			if (m_dwCarrierPinFix[nMode][2] <= 0)
			{
				m_dwCarrierPinFix[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierPinFix[nMode][2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				if( nMode == 1 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				else if( nMode == 3 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CAMERA_Y_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				else if( nMode == 2 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),0,_T("CAMERA_Y_JIG_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				else// if( nMode == 4 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),0,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierPinFix[nMode][1] = GetCurrentTime();
			m_dwCarrierPinFix[nMode][2] = m_dwCarrierPinFix[nMode][1] - m_dwCarrierPinFix[nMode][0];

			if (m_dwCarrierPinFix[nMode][2] <= 0)
			{
				m_dwCarrierPinFix[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierPinFix[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				if( nMode == 1 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Press_Carrier_Holder_Up_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else if( nMode == 3 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Press_Up_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CAMERA_Y_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else if( nMode == 4 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Press_Up_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Jig_Press_Backward_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("FORWARD"),0,_T("CAMERA_Y_JIG_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCarrierPinFix[nMode] == false && nRet_2 == IO_ON && nRet_1 == IO_OFF )
		{
			m_bCarrierPinFix[nMode] = true;
			m_dwCarrierPinFix[nMode][0]	= GetCurrentTime();
		}
		else if (m_bCarrierPinFix[nMode] == true && nRet_2 == IO_ON && nRet_1 == IO_OFF )
		{
			m_dwCarrierPinFix[nMode][1]	= GetCurrentTime();
			m_dwCarrierPinFix[nMode][2]	= m_dwCarrierPinFix[nMode][1] - m_dwCarrierPinFix[nMode][0];

			if (m_dwCarrierPinFix[nMode][2] <= 0)
			{
				m_dwCarrierPinFix[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCarrierPinFix[nMode][2]> (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				if( nMode == 1 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				else if( nMode == 3 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CAMERA_Y_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				else if( nMode == 4 )
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				else
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),0,_T("CAMERA_Y_JIG_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierPinFix[nMode][1]	= GetCurrentTime();
			m_dwCarrierPinFix[nMode][2]	= m_dwCarrierPinFix[nMode][1] - m_dwCarrierPinFix[nMode][0];

			if (m_dwCarrierPinFix[nMode][2] <= 0)
			{
				m_dwCarrierPinFix[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierPinFix[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				if( nMode == 1 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Press_Carrier_Holder_Down_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else if( nMode == 3 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Press_Down_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CAMERA_Y_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else if( nMode == 4 )
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Press_Down_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Jig_Press_Forward_Check); 
					clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("BACKWARD"),0,_T("CAMERA_Y_JIG_PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				}
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}







