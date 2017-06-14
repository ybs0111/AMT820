// Run_LdTrayPlate.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_LdTrayPlate.h"
#include "AMTLotManager.h"
#include "LogFromat.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_LdTrayPlate
CRun_LdTrayPlate Run_LdTrayPlate;
IMPLEMENT_SERIAL(CRun_LdTrayPlate, CObject, 1);

CRun_LdTrayPlate::CRun_LdTrayPlate()
{
	mn_RunStep	= -1;
	mn_InitStep	= -1;
}

CRun_LdTrayPlate::~CRun_LdTrayPlate()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_LdTrayPlate message handlers
void CRun_LdTrayPlate::Thread_Run()
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

void CRun_LdTrayPlate::RunInit()
{
	int nRet_1=0;
	
	if( st_handler.mn_init_state[INIT_LD_PLATE] != CTL_NO ) return;
	switch(mn_InitStep)
	{
		case 0:
			mn_InitStep = 100;
			break;

		case 100:
			Set_Tray_Guide_Clamp_ForBackward(IO_OFF);
			mn_InitStep = 200;
			break;

		case 200:
			nRet_1 = Chk_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			if(nRet_1 == RET_GOOD)
			{
				mn_InitStep = 1000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(8001, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			break;

		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;

		case 1000:
			st_handler.mn_init_state[INIT_LD_PLATE] = CTL_YES;
			mn_InitStep = 0;
			break;

	}

}

void CRun_LdTrayPlate::RunMove()
{
	int nRet_1,x,y;
	long lMotionDone=0;

	Func.ThreadFunctionStepTrace(10, mn_RunStep);


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
			m_nFindLotNo_Flag = -1;
			if( g_lotMgr.GetLotCount() > 0 )
			{
				if( ( g_lotMgr.GetLotAt(0).GetStrLastModule() != "YES") && g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
				{
					//load plate�� ���� ��û
					m_nFindLotNo_Flag = 0;
					m_strLotNo = g_lotMgr.GetLotAt(0).GetLotID();
					m_strPartNo = g_lotMgr.GetLotAt(0).GetPartID();
					mn_RunStep = 200;
				}
				else if( g_lotMgr.GetLotCount() >= 2 )
				{
					if( ( g_lotMgr.GetLotAt(1).GetStrLastModule() != "YES") && g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
					{
						m_nFindLotNo_Flag = 1;
						m_strLotNo = g_lotMgr.GetLotAt(1).GetLotID();
						m_strPartNo = g_lotMgr.GetLotAt(1).GetPartID();
						mn_RunStep = 200;
					}
					else
					{
						return;
					}
				}
			}
			break;

		case 200:
			nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
			{
				//930001 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR"
				m_strAlarmCode.Format(_T("930001"));
				CTL_Lib.Alarm_Error_Occurrence(8002, dWARNING, m_strAlarmCode);
				mn_RunStep = 100;
			}
			else 
			{
				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo )
				{					
					if( st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LOAD_WORK_RBT] == LOTEND)//g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES")
					{
						Set_Tray_Guide_Clamp_ForBackward(IO_OFF);
						st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_TRAY_PLATE] = LOTEND;
						mn_RunStep = 9000;
					}
				}
				else
				{
					mn_RunStep = 300;
				}
			} 
			break;

		case 300:
			Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			mn_RunStep = 310;
			break;

		case 310:
			nRet_1 = Chk_Tray_Guide_Clamp_ForBackward(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 400;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(8003, dWARNING, m_strAlarmCode);
				mn_RunStep = 300;
			}
			break;

		case 400:
			mn_RunStep = 1000;
			break;

		case 1000:
			st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_REQ;
			mn_RunStep = 1100;
			break;

		case 1100:
			if(st_sync.nLdPlate_Tray_Supply_Req[THD_LD_STACKER] == CTL_READY)
			{
				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo) //2016.0907 
				{
					st_tray_info[THD_LD_TRAY_PLATE].nTrayExist = CTL_YES;		
					mn_RunStep = 2000; 
				}		

				if (st_handler.cwnd_main != NULL)
				{
					//st_handler.cwnd_main->SendMessage(WM_WORK_DISPLAY, MAIN_LOT_DISPLAY, 0);
				}
			}
			break;

		case 2000:
			nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
			{
				st_handler.n_partial_lotend = 0;
				mn_RunStep = 2100;
			}
			else 
			{
				m_strAlarmCode.Format(_T("910002")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(8004, dWARNING, m_strAlarmCode); 
			}
			break;

		case 2100:
			Set_Tray_Guide_Clamp_ForBackward(IO_ON); 
			mn_RunStep = 2110;
			break;

		case 2110:
			nRet_1 = Chk_Tray_Guide_Clamp_ForBackward(IO_ON );
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 2120;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(8605, dWARNING, m_strAlarmCode);
				mn_RunStep = 2100;
			}
			break;

		case 2120:
			st_handler.n_partial_lotend++;
			if( st_basic.n_count_retry < st_handler.n_partial_lotend)
			{
				st_handler.n_partial_lotend = 0;
				mn_RunStep = 2200;
			}
			else
			{
				mn_RunStep = 2130;
			}
			break;

		case 2130:
			Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			mn_RunStep = 2140;
			break;
			
		case 2140:
			nRet_1 = Chk_Tray_Guide_Clamp_ForBackward(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 2100;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(8607, dWARNING, m_strAlarmCode);
				mn_RunStep = 2130;
			}
			break;

		case 2200:
			nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //�δ� �÷���Ʈ�� Ʈ���̰� ���� �� ���� 
			{
				st_sync.nLdPlate_Tray_Supply_Req[THD_LD_TRAY_PLATE] = CTL_CLEAR;
				mn_RunStep = 5000;
			}
			else 
			{
				m_strAlarmCode.Format(_T("900001")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(8006, dWARNING, m_strAlarmCode);			 
			} 
			break;

		case 5000:
			if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_REQ)
			{
				if(st_tray_info[THD_LD_TRAY_PLATE].nTrayExist == CTL_YES)
				{
					nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
					if(nRet_1 == BD_GOOD)
					{
						mn_RunStep = 5100;
					}
					else 
					{
						m_strAlarmCode.Format(_T("910002")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
						CTL_Lib.Alarm_Error_Occurrence(8007, dWARNING, m_strAlarmCode);			 
					} 					
				}
				else //if(st_tray_info[THD_LD_TRAY_PLATE].nTrayExist == CTL_NO)
				{
					nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
					if(nRet_1 == BD_GOOD)
					{
						m_strAlarmCode.Format(_T("910002")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
						CTL_Lib.Alarm_Error_Occurrence(8008, dWARNING, m_strAlarmCode); 
					}
					else 
					{
						mn_RunStep = 1000;
					}
				}
			}  
			else
			{
				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo )
				{					
					if( st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LOAD_WORK_RBT] == LOTEND)//g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES")
					{
						Set_Tray_Guide_Clamp_ForBackward(IO_OFF);
						st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LD_TRAY_PLATE] = LOTEND;
						mn_RunStep = 9000;
					}
				}		 
			}
			break; 

		case 5100:
			nRet_1 = Func.Find_TrayWork_Pos(0, THD_LD_TRAY_PLATE, CTL_YES, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strLotNo);
			if(nRet_1 == RET_GOOD) //�۾� ������ ���簡 �ִ�
			{
				for (x=0; x<st_recipe.nTrayX; x++)
				{
					for (y=0; y<st_recipe.nTrayY; y++)
					{
						st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].strLotNo	= st_tray_info[THD_LD_STACKER].strLotNo;
						st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].strPartNo	= st_tray_info[THD_LD_STACKER].strPartNo;
					}
				}

				st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_READY;

				mn_RunStep = 5000;
			}
			else if(nRet_1 == RET_SKIP) //���̻� ���簡 ����
			{
				mn_RunStep = 6000;
			}
			break;		

		case 6000: //Ʈ�����۰� �۾��� ���� Ʈ���̸� empty�� ������ 
			Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			mn_RunStep = 6010;
			break;

		case 6010:
			nRet_1 = Chk_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 6100;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(8009, dWARNING, m_strAlarmCode);
				mn_RunStep = 6000;
			}
			break; 

			//load plate Ʈ���� ��ü�� ��û 
		case 6100:			
			if( st_sync.nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_CLEAR )
			{
				st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ; //Ʈ���� ��ü�� ��û 
				st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK; //Ʈ���� ����� ��û 
				mn_RunStep = 6200;
			}
			break;

		case 6200:
			if(st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_READY) //Ʈ�����۰� Ʈ���̸� ���� ���� 
			{
				nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
				if(nRet_1 == BD_GOOD)//BD_ERROR) //Ʈ���̰� ���� �ȵ� ���� 
				{
					st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_FREE;
					st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][1] = CTL_CLEAR; 
					mn_RunStep = 6300;
				}
				else 
				{
					m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
					CTL_Lib.Alarm_Error_Occurrence(8010, dWARNING, m_strAlarmCode);			 
				} 
			}
			break;

		case 6300:
			// 			if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo)
			// 			{
			// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" )
			// 				{
			// 					mn_RunStep = 9000;
			// 					break;
			// 				}
			// 			}
			if( st_sync.nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_CLEAR)
				mn_RunStep = 1000; //����Ŀ�� Ʈ���� ������ ��û���� ó������ ���� 
			break;

		case 9000:
			Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			mn_RunStep = 9010;
			break;

		case 9010:
			nRet_1 = Chk_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 9100;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(8011, dWARNING, m_strAlarmCode);
				mn_RunStep = 9000;
			}
			break; 

		case 9100:
// 			if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo)
// 			{
// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" )
// 				{
					mn_RunStep = 0;
					break;
// 				}
// 			}		
			break; 

		default:
			if (st_handler.cwnd_list != NULL)	// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_LdTrayPlate  = %d", mn_RunStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			break;
	}
}	


void CRun_LdTrayPlate::Set_Tray_Guide_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Tray_Guide_Clamp_Forward_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Tray_Guide_Clamp_Backward_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("LD_TRAY_PLATE"),_T("FORWARD"),0,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("LD_TRAY_PLATE"),_T("BACKWARD"),0,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

//������ ���� sol ��½ð���ŭ ������
int CRun_LdTrayPlate::Chk_Tray_Guide_Clamp_ForBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_TRAY_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true)
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
				clsLog.LogFunction(_T("LD_TRAY_PLATE"),_T("BACKWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.o_Tray_Guide_Clamp_Forward_Sol); 
				clsLog.LogFunction(_T("LD_TRAY_PLATE"),_T("BACKWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false)// &&	g_ioMgr.get_in_bit(st_io.i_LdUldPickDvcChk,	IO_ON)	== IO_ON )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true) //&&		 g_ioMgr.get_in_bit(st_io.i_LdUldPickDvcChk, IO_ON)	== IO_ON )
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
				clsLog.LogFunction(_T("LD_TRAY_PLATE"),_T("FORWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.o_Tray_Guide_Clamp_Forward_Sol); 
				clsLog.LogFunction(_T("LD_TRAY_PLATE"),_T("FORWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}
