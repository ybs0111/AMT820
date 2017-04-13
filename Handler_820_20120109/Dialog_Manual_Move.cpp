// Dialog_Manual_Move.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_Move.h"
#include "Public_Function.h"
#include "Run_LdStacker_Elvator.h"
#include "Run_EmptyStacker_Elvator.h"
#include "Run_EmptyTrayTransfer.h"
#include "IO_Manager.h"
#include <MATH.H>
#include "CmmsdkDef.h"
#include "Dialog_Message.h"
#include "Run_Device_Carrier_Robot.h"
#include "Run_UnPress_Robot.h"
#include "Run_Epoxy_Transfer_Robot.h"
#include "Run_HeatSinkVision_Transfer_Robot.h"
#include "Run_LdTrayPlate.h"
#include "Run_LdPicker.h"
#include "Run_DvcLdBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move dialog

CDialog_Manual_Move::CDialog_Manual_Move(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_Move::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Manual_Move)
	//}}AFX_DATA_INIT
}


void CDialog_Manual_Move::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_Move)
	DDX_Control(pDX, IDC_COMBO_MOVE_VISION, m_cbo_vision);
	DDX_Control(pDX, IDC_COMBO_MOVE_HEATSINK, m_cbo_hs_rub);
	DDX_Control(pDX, IDC_COMBO_MOVE_CARRIER, m_cbo_carrier);
	DDX_Control(pDX, IDC_COMBO_TRAY_TRANSFER, m_cbo_tray_transfer);
	DDX_Control(pDX, IDC_COMBO_TRAY2, m_cbo_tray2);
	DDX_Control(pDX, IDC_COMBO_TRAY1, m_cbo_tray1);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btn_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_Move, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_Move)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_ELEVATOR_GO1, OnElevatorGo1)
	ON_BN_CLICKED(IDC_ELEVATOR_GO2, OnElevatorGo2)
	ON_BN_CLICKED(IDC_ELEVATOR_GO3, OnElevatorGo3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_REQ_TRAY_CHANGE, OnReqTrayChange)
	ON_BN_CLICKED(IDC_BTN_ENDLESS_LOOP, OnBtnEndlessLoop)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_HEAT_SINK_GO, OnBtnHeatSinkGo)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BTN_VISION_GO, OnBtnVisionGo)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move message handlers

BOOL CDialog_Manual_Move::Create()
{
	return CDialog::Create( CDialog_Manual_Move::IDD );
}


BOOL CDialog_Manual_Move::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect_area;
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	GetWindowRect(&rect_area);
	MoveWindow(m_cp_coord.x, m_cp_coord.y, rect_area.Width(), rect_area.Height());

	Init_Data();
	Init_Button();
	Init_Group();
	Init_Show();

	st_handler.cwnd_manualmove = this;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Manual_Move::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	PostNcDestroy();
}

void CDialog_Manual_Move::Init_Data()
{
	m_nTray1 = 0;
	m_nTray2 = 0;
	m_nTrayTransfer = 0;
}

void CDialog_Manual_Move::Init_Show()
{
}

void CDialog_Manual_Move::Init_Button()
{
	short	shBtnColor = 30;
	
	m_btn_exit.SetFont(m_p_font);
	m_btn_exit.SetIcon(IDI_EXIT);
	m_btn_exit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_exit.SetButtonColor(1);
	m_btn_exit.SetTooltipText("IO Dialog Exit");
}

void CDialog_Manual_Move::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
}

void CDialog_Manual_Move::OnButtonControl(bool bflag)
{
	m_btn_exit.EnableWindow(bflag);
}



void CDialog_Manual_Move::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	KillTimer(TM_TRAY1);
	KillTimer(TM_TRAY2);
	KillTimer(TM_TRAY_TRANSFER);
	KillTimer(TM_CARRIER_MOVE);
	KillTimer(TM_UNPRESS_MOVE);
	KillTimer(TM_VISION_MOVE);
	KillTimer(TM_EPOXY_MOVE);
	KillTimer(TM_HEATSINK_MOVE);
	KillTimer(TM_HSVISION_MOVE);
	KillTimer(TM_DISPENSOR);
	KillTimer(TM_LOAD_PLATE);
	KillTimer(TM_LOADER_MOVE);
	KillTimer(TM_DVCBUFFER);
	
	delete this;
	Func.m_p_move = NULL;

	st_handler.cwnd_manualmove = NULL;

	CDialog::PostNcDestroy();
}

void CDialog_Manual_Move::OnElevatorGo1() 
{
// 	m_nTray1 = m_cbo_tray1.GetCurSel();
// 	Init_Timer();
// 	SetTimer( TM_TRAY1, 100, NULL );
// 
// 	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
// 	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_LATCH, cmFALSE);//16
 Func.SaveHeatSinkBoxPos();	

}

void CDialog_Manual_Move::OnElevatorGo2() 
{
	m_nTray2 = m_cbo_tray2.GetCurSel();
	Init_Timer();
	SetTimer( TM_TRAY2, 100, NULL );

	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_LATCH, cmFALSE);//16

}

void CDialog_Manual_Move::OnElevatorGo3() 
{
	m_nTrayTransfer = m_cbo_tray2.GetCurSel();
	Init_Timer();
	SetTimer( TM_TRAY1, 100, NULL );
	SetTimer( TM_TRAY2, 100, NULL );
	SetTimer( TM_TRAY_TRANSFER, 100, NULL );
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_LATCH, cmFALSE);//16
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_LATCH, cmFALSE);//16

}

void CDialog_Manual_Move::OnBtnEndlessLoop() 
{
	double dCurr = 0.0;
	//초기화가 완료되었는가?
	CDialog_Message msg_dlg;
	if( st_handler.n_initial == TRUE )
	{
		dCurr = COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X);
		if( dCurr < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) )
		{
			st_other.str_fallacy_msg = _T("[EPOXY_TRANSFER_X] X축이 safety위치가 아닙니다.");
			msg_dlg.DoModal();
			return;
		}

		dCurr = COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y);
		if( dCurr > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
		{
			st_other.str_fallacy_msg = _T("[EPOXY_TRANSFER_Y] Y축이 safety위치가 아닙니다.");
			msg_dlg.DoModal();
			return;
		}
		dCurr = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y);
		if( dCurr > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + st_motor[M_HEATSINK_TRANSFER_Y].mn_allow ) )
		{
			st_other.str_fallacy_msg = _T("[HEATSINK_TRANSFER_Y] Y축이 carrier safety 위치가 아닙니다.");
			msg_dlg.DoModal();
			return;
		}
		dCurr = COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y);
		if( dCurr > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) )
		{
			st_other.str_fallacy_msg = _T("[LOADER_TRANSFER_Y] Y축이 ready safety 위치가 아닙니다.");
			msg_dlg.DoModal();
			return;
		}
		dCurr = COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X);
		if( dCurr > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_X] ) )
		{
			st_other.str_fallacy_msg = _T("[UNLOADER_TRANSFER_X] X축이 ready safety 위치가 아닙니다.");
			msg_dlg.DoModal();
			return;
		}
		dCurr = COMI.Get_MotCurrentPos( M_CARRIER_X);
		if( dCurr < ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] - (10*COMI.md_allow_value[M_CARRIER_X]) ) )
		{
			st_other.str_fallacy_msg = _T("[CARRIER_X] X축이 ready safety 위치가 아닙니다.");
			msg_dlg.DoModal();
			return;
		}

		mn_move_carrier = 0;
		mn_move_carrier = m_cbo_carrier.GetCurSel();
		if( mn_move_carrier < 0 ) mn_move_carrier = 0;

		st_basic.n_mode_device = WITHOUT_DVC;
 		st_basic.n_mode_jig = WITH_JIG;
		int m,x,y;
		st_recipe.nTrayNum = 2;
		st_recipe.nHsTrayY = 5;
		st_recipe.nHsTrayX = 9;

		for(m = 0; m<st_recipe.nTrayNum; m++)
		{
			for(y = 0; y < st_recipe.nHsTrayY; y++) //Y 방향 정보
			{
				for(x = 0; x < st_recipe.nHsTrayX; x++) //X 방향 정보 
				{ 
					st_tray_info[THD_LD_HEATSINK_BUFF].st_pcb_info[y][x].nYesNo = DVC_YES;
				}
			}
		}

		st_recipe.nTrayY = 2;
		st_recipe.nTrayX = 1;
		for(y = 0; y<st_recipe.nTrayY; y++)
		{
			for(x = 0; x< st_recipe.nTrayX; x++) //Y 방향 정보
			{
				st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].nYesNo = CTL_YES;
			}
		}

		st_recipe.dTrayPitch_Y = 95;


		Init_Timer();
		if( mn_move_carrier == 0 )//무한반복 carrier + unpress
		{
			SetTimer( TM_CARRIER_MOVE, 100, NULL );
			SetTimer( TM_UNPRESS_MOVE, 100, NULL );
		}
		else if( mn_move_carrier == 1)//only epoxy
		{
 			SetTimer( TM_EPOXY_MOVE, 100, NULL );
		}
		else if(mn_move_carrier == 2)// carrier + epoxy
		{
			SetTimer( TM_CARRIER_MOVE, 100, NULL );
			SetTimer( TM_UNPRESS_MOVE, 100, NULL );
			SetTimer( TM_EPOXY_MOVE, 100, NULL );
		}
		else if(mn_move_carrier == 3)//only load rbt + ELV
		{
			SetTimer( TM_LOADER_MOVE, 100, NULL );
			SetTimer( TM_LOAD_PLATE, 100, NULL );
			SetTimer( TM_TRAY1, 100, NULL );
			SetTimer( TM_TRAY2, 100, NULL );
			SetTimer( TM_TRAY_TRANSFER, 100, NULL );
			SetTimer( TM_DVCBUFFER, 100, NULL );
		}
		else if(mn_move_carrier == 4)//carrier + load rbt + ELV
		{
			SetTimer( TM_CARRIER_MOVE, 100, NULL );
			SetTimer( TM_UNPRESS_MOVE, 100, NULL );
			SetTimer( TM_LOADER_MOVE, 100, NULL );
			SetTimer( TM_LOAD_PLATE, 100, NULL );
			SetTimer( TM_TRAY1, 100, NULL );
			SetTimer( TM_TRAY2, 100, NULL );
			SetTimer( TM_TRAY_TRANSFER, 100, NULL );
			SetTimer( TM_DVCBUFFER, 100, NULL );
		}
		else if( mn_move_carrier == 5)// Only HeatSink + vision
		{
			SetTimer( TM_HEATSINK_MOVE, 100, NULL );
			SetTimer( TM_DISPENSOR, 100, NULL);
			//SetTimer( TM_HSVISION_MOVE, 100, NULL );
		}
		if(mn_move_carrier == 7)//CRR+EPOXY+LOAD DEVICE
		{
			SetTimer( TM_CARRIER_MOVE, 100, NULL );
			SetTimer( TM_UNPRESS_MOVE, 100, NULL );
			SetTimer( TM_LOADER_MOVE, 100, NULL );
			SetTimer( TM_LOAD_PLATE, 100, NULL );
			SetTimer( TM_TRAY1, 100, NULL );
			SetTimer( TM_TRAY2, 100, NULL );
			SetTimer( TM_TRAY_TRANSFER, 100, NULL );
			SetTimer( TM_DVCBUFFER, 100, NULL );
			SetTimer( TM_EPOXY_MOVE, 100, NULL );
		}
		else if(mn_move_carrier == 8)//CRR+EPOXY+LOAD DEVICE
		{
			SetTimer( TM_HEATSINK_MOVE, 100, NULL );
			SetTimer( TM_DISPENSOR, 100, NULL);
			SetTimer( TM_CARRIER_MOVE, 100, NULL );
			SetTimer( TM_UNPRESS_MOVE, 100, NULL );
			SetTimer( TM_LOADER_MOVE, 100, NULL );
			SetTimer( TM_LOAD_PLATE, 100, NULL );
			SetTimer( TM_TRAY1, 100, NULL );
			SetTimer( TM_TRAY2, 100, NULL );
			SetTimer( TM_TRAY_TRANSFER, 100, NULL );
			SetTimer( TM_DVCBUFFER, 100, NULL );
			SetTimer( TM_EPOXY_MOVE, 100, NULL );
		}		
	}
}

void CDialog_Manual_Move::Init_Timer()
{
	int i = 0, j = 0, k = 0;

	for ( i = 0; i < M_MOTOR_COUNT; i++ )
	{
		COMI.Set_MotStop( 0, i );
	}

	KillTimer(TM_TRAY1);
	KillTimer(TM_TRAY2);
	KillTimer(TM_TRAY_TRANSFER);
	KillTimer(TM_CARRIER_MOVE);
	KillTimer(TM_UNPRESS_MOVE);
	KillTimer(TM_VISION_MOVE);
	KillTimer(TM_EPOXY_MOVE);
	KillTimer(TM_HEATSINK_MOVE);
	KillTimer(TM_HSVISION_MOVE);
	KillTimer(TM_DISPENSOR);
	KillTimer(TM_LOAD_PLATE);
	KillTimer(TM_LOADER_MOVE);
	KillTimer(TM_DVCBUFFER);


	for ( i = 0; i<100; i++)
	{
		m_move_step[i] = 0;
	}
	mn_RunTopFwdStep = 0;
	mn_RunDownStep = 0;
	mn_RunUpStep = 0;
	mn_RunBtmFwdStep = 0;
	mn_jig_rotator = 0;
	mn_BufferPos = 0;
	mn_SafetyStep = 0;
	mn_MoveStep = 0;
	mn_FirstSecond = 0;
	mn_StartEnd = 0;
	mn_dotPos = 0;
	m_nCarriorPos = 0;
	bHeatsinkMeasureCmp = FALSE;
	m_nVisCarriorPos = 0;
	m_nVisTeachPos = 0;
	m_n_FirstPicker_Num = 0;
	m_n_FirstTray_X_Num = 0;
	m_n_FirstTray_Y_Num = 0;
	m_nWork_Pos = 0;
	m_LoadBufferPos = 0;
	m_hs_rub = -1;
	m_nhsexystop = 0;
	m_nvisionstop = 0;
	m_hs_vision = -1;

	for( i = 0; i < 4; i++ )
	{
		m_npBuff_Info[i] = CTL_NO;
		m_npBuff_Status[i] = CTL_NO;
		m_npBuff_OutputStatus[i] = CTL_NO;

	}

	for( i = 0; i < 30; i++)
	{
		for( j = 0; j < 5; j++ )
		{
			m_Thread_Flag[j] = CTL_NO;
			m_npSet_WorkPosYXCPB[j] = CTL_NO;
			m_npFindWorkPosYXCPB[j] = CTL_NO;
			nWorkTransfer_Req[i][j] = CTL_NO;
			nCarrierRbt_Dvc_Req[i][j] = CTL_NO;
			nHeatSinkRbt_Dvc_Req[i][j] = CTL_NO;
			nLdWorkRbt_Dvc_Req[i][j] = CTL_NO;
			nUldWorkRbt_Dvc_Req[i][j] = CTL_NO;
		}
		nLdPlate_Tray_Supply_Req[i] = CTL_NO;
		m_nTransferJobFlag[i] = CTL_NO;
		nCarrierSateyflag[i] = CTL_NO;
	}
	for ( i = 0; i < THREAD_MAX_SITE; i++)
	{
		for( j = 0; j < MAX_BUF_SIZE; j++ )
		{			
			//memset(&st_manaualbuffer_info[i].st_pcb_info[j], 0x00, sizeof(st_manaualbuffer_info[i].st_pcb_info[j]) );
		}
		for( k = 0; k <1; k++ )
		{
			st_picker[i].st_pcb_info[k].nYesNo = CTL_NO;
			st_picker[i].st_pcb_info[k].nBin = CTL_NO;
			st_buffer_info[i].st_pcb_info[k].nYesNo = CTL_NO;
			st_buffer_info[i].st_pcb_info[k].nBin = CTL_NO;
		}

	}


}

int CDialog_Manual_Move::Move_TrayElv1()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = M_TRAY1_Z;

	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			nTrayExist = CTL_NO;
			m_move_step[m_nAxisNum] = 10;
			break;

		case 10:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //트레이를 받을 위치로 미리 이동한다 
			if (nRet_1 == BD_GOOD) //좌측으로 이동  
			{
				mn_retry_cnt = 0;
				m_move_step[m_nAxisNum] = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 10;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
				CTL_Lib.Alarm_Error_Occurrence(6050, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 10;
			}
			break; 			

		case 100:
			nRet_1 = Run_LdStacker_Elvator.Ready_Stacker_Move_Check(0);

			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 200;
			}
			else if( nRet_1 == RET_ERROR )
			{
				//동작중에는 자재없음 알람발생.
				//CTL_Lib.Alarm_Error_Occurrence(2100, dWARNING, Run_LdStacker_Elvator.m_strAlarmCode);
			}
			break;

		case 200:
			nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(6060, dWARNING, m_strAlarmCode);
			}
			else if(nRet_1 == BD_ERROR)//로더 플레이드에 트레이가 감지 되지않은 상태 
			{
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //트레이를 받을 위치로 미리 이동한다 
			if (nRet_1 == BD_GOOD) //좌측으로 이동  
			{
				mn_retry_cnt = 0;
				m_move_step[m_nAxisNum] = 400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 210;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
				CTL_Lib.Alarm_Error_Occurrence(6050, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 210;
			}
			break; 

		case 400:
			if( mn_move_carrier == 3 )
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			else//ONLY ELIVATOR
			{
				nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_REQ;
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 1000:
			if( nLdPlate_Tray_Supply_Req[THD_LD_STACKER] == CTL_REQ )
			{
				m_dwWaitTime[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 1100;
			}
			break;

		case 1100:
			m_dwWaitTime[1] = GetCurrentTime();
			m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
			if( m_dwWaitTime[2] <= 0 ) m_dwWaitTime[0] = GetCurrentTime();
			nRet_1 = Run_LdStacker_Elvator.Ready_Stacker_Move_Check(1);
			if(	nRet_1 == RET_GOOD )
			{
				nTrayExist = CTL_YES; //트레이존재 정보 셋
				m_move_step[m_nAxisNum] = 1200;
			}
			else
			{
				if( m_dwWaitTime[2] > 15000 )
				{
					//자재없음
// 					sprintf(st_msg.c_abnormal_msg, "[LDSTACKER_ELV] There is no tray in load stacker elevator. Please check sensor.");
// 					if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 					m_dwWaitTime[0] = GetCurrentTime();
					COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmFALSE);
					m_move_step[m_nAxisNum] = 1110;
				}
			}
			break;

		case 1110:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD)
			{
				sprintf(st_msg.c_abnormal_msg, "[LDSTACKER_ELV] There is no tray in load stacker elevator. Please check sensor.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				//m_dwWaitTime[0] = GetCurrentTime();
				//nFunRet = RET_ERROR;
				COMI.Set_Motor_IO_Property(m_nAxisNum, cmSD_EN, cmTRUE);
	
				m_move_step[m_nAxisNum] = 0;
			}
			else if (nRet_1 == BD_RETRY)
			{				 
				m_move_step[m_nAxisNum] = 1110;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(375, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1110;
			}
			break;


		case 1200:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
			if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
			{
				mn_retry_cnt = 0;
				m_move_step[m_nAxisNum] = 1300;
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
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				mn_retry_cnt++;
				if (mn_retry_cnt > 2)
				{
					mn_retry_cnt = 0;

					m_strAlarmCode.Format(_T("9100%d2"), m_nAxisNum); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
					CTL_Lib.Alarm_Error_Occurrence(6110, dWARNING, m_strAlarmCode);
				}
				m_move_step[m_nAxisNum] = 1310;
				break;
			}
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_OFF); //plate off 체크  
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON); //plate off 체크			
			if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
			{
				m_move_step[m_nAxisNum] = 2000;
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
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2000://6100
			nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 != BD_GOOD && g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_On_Check, IO_ON) != IO_ON) //로더 플레이트에 트레이가 감지 된 상태 
			{
				mn_retry_cnt++;
				if (mn_retry_cnt > 2)
				{
					mn_retry_cnt = 0;
					m_strAlarmCode.Format(_T("910002")); //910002 1 A "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
					CTL_Lib.Alarm_Error_Occurrence(6110, dWARNING, m_strAlarmCode);
				}
				m_move_step[m_nAxisNum] = 1310;
				break;
			}
			
			//Tray lock cyclinder 체크 필요
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_On_Check,	IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Holder2_Off_Check,	IO_ON);			
			if( nRet_1 == IO_OFF && nRet_2 == IO_ON )
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			else
			{
				if(nRet_1 == IO_ON) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_On_Check); 
				else				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Holder2_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(6120, dWARNING, m_strAlarmCode);
			}
			break;

		case 2100:
			nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET);
			if(nRet_1 == RET_GOOD)
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_ON); //load stacker tray check    
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Limit_Check, IO_ON);// COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
				
				if(nRet_1 == IO_OFF && nRet_2 == IO_OFF) //트레이가 스태커에 없는 상태, SD 센서도 감지되지 않은 상태, 트레이가 없는 상태  
				{
					m_move_step[m_nAxisNum] = 5000;
				}
				else
				{
					m_move_step[m_nAxisNum] = 2200;
				}
			}
			break;

		case 2200:
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Limit_Check, IO_ON);//COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == IO_OFF)//if(nRet_1 == BD_ERROR) //로더 플레이트에 트레이가 감지 된 상태 
			{
				m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(6140, dWARNING, m_strAlarmCode);
				break;
			}
			nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_READY;
			m_move_step[m_nAxisNum] = 3000;
			break;

		case 3000:
			m_move_step[m_nAxisNum] = 1000;
			break; 


		case 5000:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_ON);
			if(nRet_1 == IO_OFF)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Exist_Check);
				CTL_Lib.Alarm_Error_Occurrence(6150, dWARNING, m_strAlarmCode);
				break;
			}
			
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);   //밑으로 이동한다 
			if (nRet_1 == BD_GOOD) //위로 이동 하면서 SD를 찾는다  
			{		
				m_move_step[m_nAxisNum] = 5010;
			}
			else if (nRet_1 == BD_RETRY)
			{				 
				m_move_step[m_nAxisNum] = 5000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{ 
				CTL_Lib.Alarm_Error_Occurrence(6150, dWARNING, alarm.mstr_code);
				
				if (st_handler.cwnd_list != NULL)  
				{
					//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2000"));
					//st_handler_info.cWndList->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
				}
				m_move_step[m_nAxisNum] = 5000;
			}
			break;
			
		case 5010:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //2015.0407 james  //SD 위치까지 이동했는데 SD가 감지되지 않으면 트레이가 없는것이다
			if (nRet_1 == BD_GOOD)   
			{	 
				m_move_step[m_nAxisNum] = 5100;  			 
			}
			else if (nRet_1 == BD_RETRY)
			{		 
				m_move_step[m_nAxisNum] = 5010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{ 
				CTL_Lib.Alarm_Error_Occurrence(6160, dWARNING, alarm.mstr_code);
<<<<<<< HEAD:Handler_820_20120109/Dialog_Manual_Move.cpp
				m_move_step[m_nAxisNum] = 5010;
=======
				m_move_step[m_nAxisNum] = 2010;
>>>>>>> 03c9121054aa9555b02a1f7854d5a71699174b8b:Handler_820_20120109/Dialog_Manual_Move.cpp
			}
			break;
			
		case 5100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_OFF); //stacker tary 는 없어야 한다  
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Ready_Check, IO_OFF);
			if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
			{
				nTrayExist = CTL_NO; //트레이존재 정보 클리어
				m_move_step[m_nAxisNum] = 1000;
			}
			else
			{//트레이 존재가 존재한다  		
				
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Stacker_Tray_Exist_Check);
				CTL_Lib.Alarm_Error_Occurrence(6180, dWARNING, m_strAlarmCode);
			}			 
			break;
	}
	return nFunRet;
}

int CDialog_Manual_Move::Move_TrayElv2()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = M_TRAY2_Z;
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 100;
			break;

		case 100:
			nRet_1 = Run_EmptyStacker_Elvator.Ready_Stacker_Move_Check(0);
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 200;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence(2100, dWARNING, Run_EmptyStacker_Elvator.m_strAlarmCode);
			}
			break;

		case 200:
			nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				m_strAlarmCode.Format(_T("910102")); //910102 1 A "EMPTY_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(6060, dWARNING, m_strAlarmCode);
			}
			else if(nRet_1 == BD_ERROR)//로더 플레이드에 트레이가 감지 되지않은 상태 
			{
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);  //트레이를 받을 위치로 미리 이동한다 
			if (nRet_1 == BD_GOOD) //좌측으로 이동  
			{			
				m_move_step[m_nAxisNum] = 400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
				CTL_Lib.Alarm_Error_Occurrence(6050, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 300;
			}
			break; 

		case 400:
			nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD)
			{
				m_move_step[m_nAxisNum] = 500;
				break;
				//m_strAlarmCode.Format(_T("910103")); //910103 1 A "EMPTY_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
				//CTL_Lib.Alarm_Error_Occurrence(4100, dWARNING, m_strAlarmCode);
			}
			else if(nRet_1 == BD_ERROR)
			{
				m_move_step[m_nAxisNum] = 500;
			}
			break;

		case 500:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Exist_Check, IO_OFF); //stacker tary 는 처음에는 없어야 한다  
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Unloading_Stacker_Tray_Ready_Check, IO_OFF); //stacker tary 는 처음에는 없어야 한다  
			if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			else
			{//트레이 존재 에러 
				m_move_step[m_nAxisNum] = 1000;//james 2016.0915 빈 트레이가 있어도 진행하자. 향후 고객요청 상황을 파악하여 에러 여/부 결정 필요 
				break;

				if( nRet_1 == IO_ON )
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Unloading_Stacker_Tray_Exist_Check);  
				else
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Unloading_Stacker_Tray_Ready_Check);  
				CTL_Lib.Alarm_Error_Occurrence(4110, dWARNING, m_strAlarmCode);
			}
			break;

		//////////////////////////////////////////////////////////////////////////////////
		// empty stacker와 연관하여 작업하는 영역
		//////////////////////////////////////////////////////////////////////////////////
		case 1000:
			if(nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_REQ) //트랜스퍼에서 empty stacker에 빈 트레이를 놓게 요청
			{
				if(nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)
				{
					m_move_step[m_nAxisNum] = 2000;
				}						
			}	
			else
			{
				m_move_step[m_nAxisNum] = 5000;
			}
			break;

		case 1100:
			nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //트레이를 트랜스퍼에서 받는다 

			if(nRet_1 == RET_GOOD)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Unloading_Stacker_Tray_Exist_Check); 
				CTL_Lib.Alarm_Error_Occurrence(4130, dWARNING, m_strAlarmCode); 

				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 2000:
			//if(st_sync_info.nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)//받는 동작으로 비어 있어도 됨 
			m_move_step[m_nAxisNum] = 3000;		
			break;

		case 3000:
			nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_RECEIVE_OFFSET); //트레이를 트랜스퍼에서 받는다 
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 3100;
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
			nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_READY;
			m_move_step[m_nAxisNum] = 4000;
			break;

		case 3200:
			nRet_1 = CTL_Lib.Elevator_Job_Move_Pos(0, m_nAxisNum,  P_ELV_SUPPLY_OFFSET); //트레이를 트랜스퍼에서 받는다 

			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			break;

		case 4000:
			if(nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_FREE) //트레스퍼 place 트레이 작업 완료 
			{
				if(nWorkTransfer_Req[THD_EMPTY_STACKER][1] == WORK_PLACE)
				{
					nRet_1 = COMI.Get_MotIOSensor(m_nAxisNum, MOT_SENS_SD); 	
					if(nRet_1 == BD_GOOD)//플레이트에 트레이가 감지 된 상태 
					{
						nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_CLEAR;
						nWorkTransfer_Req[THD_EMPTY_STACKER][1] = CTL_CLEAR;
						m_move_step[m_nAxisNum] = 5000;
					}
					if(nRet_1 == BD_ERROR) //로더 플레이트에 트레이가 감지 안된 상태 
					{
						m_nRetry++;
						if (m_nRetry > 2)
						{
							m_nRetry = 0;
							m_strAlarmCode.Format(_T("900004")); //900001 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR" //kwlee 2016.0902 "900000" ->"900004"
							CTL_Lib.Alarm_Error_Occurrence(4180, dWARNING, m_strAlarmCode);
						}
						m_move_step[m_nAxisNum] = 3200;
						

						break;
					}
				}				
			}
			break;

		case 5000:
			m_move_step[m_nAxisNum] = 1000;
			break;

	}

	return nFunRet;

}

int CDialog_Manual_Move::Move_TrayTransfer()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = M_TRAY_REMOVE_X;
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 100;
			break;

		case  100:
			Run_EmptyTrayTransfer.Set_Tray_Grip_Vacuum_OnOff(IO_OFF);
			m_move_step[m_nAxisNum] = 200;
			break;

		case 200:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Grip_Vacuum_OnOff(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 100;
			}
			break;

		case 300:
			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF);
			m_move_step[m_nAxisNum] = 400;
			break;

		case 400:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 500;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, Run_EmptyTrayTransfer.m_strAlarmCode);
				m_move_step[m_nAxisNum] = 100;
			}
			break;

		case 500:
			nTransferTrayExist = CTL_NO;
			m_nTransfer_WaitPosMove_Flag = CTL_NO;
			m_move_step[m_nAxisNum] = 1000;
			break;

		case 1000:
			nJobFlag = 0;
			if( st_handler.mn_removetray == CTL_REQ || st_handler.mn_removetray == CTL_CHANGE)
			{
			}
			else
			{
				if(nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_REQ) //Load Tray Plate트레이의 빈 트,레이 교체를 요청 
				{
					nJobFlag += (int) pow((double)2, 0);// 작업할 위치 계산; //1
				}		
			}		

			if( st_handler.mn_removetray == CTL_NO )
			{//lot end 확인 
				if(nTransferTrayExist == CTL_YES) 
				{
					m_nTransferJobFlag[0] = THD_EMPTY_STACKER; 
					m_nTransferJobFlag[1] = WORK_PLACE; 

					//이미 잡고 있는 트레이를 놓으러 empty site에 받을 준비하라고 요청한다 
					nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_REQ;
					nWorkTransfer_Req[THD_EMPTY_STACKER][1] = WORK_PLACE;

					m_move_step[m_nAxisNum] = 2000; //트레이를 놓으러 empty site로 이동한다 
					break;
				}			 
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////
			//트레이를 가지고 있는지 / 없는지 확인후 동작을 진행한다 
			/////////////////////////////////////////////////////////////////////////////////////////////////
			if(nTransferTrayExist == CTL_YES)
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check, IO_ON);  
				if(nRet_1 == IO_OFF)
				{	//트레이가 없어서 에러발생  
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Tray_Vacuum_On_Check); 
					CTL_Lib.Alarm_Error_Occurrence(4630, dWARNING, m_strAlarmCode);
					break;
				}	

				if(nJobFlag == 1)
				{ 
					m_nTransferJobFlag[0] = THD_EMPTY_STACKER; 
					m_nTransferJobFlag[1] = WORK_PLACE; 

					//이미 잡고 있는 트레이를 놓으러 empty site에 받을 준비하라고 요청한다 
					nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_REQ;
					nWorkTransfer_Req[THD_EMPTY_STACKER][1] = WORK_PLACE;
				}
			}
			else  
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check, IO_OFF);  				
				if(nRet_1 == IO_ON)
				{//트레이 있으면 알람 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Tray_Vacuum_On_Check); 
					CTL_Lib.Alarm_Error_Occurrence(4640, dWARNING, m_strAlarmCode);
					break;
				}

				if(nJobFlag == 1)
				{ // 로딩 빈 트레이를 집으러 이동한다 
					m_nTransferJobFlag[0] = THD_LD_TRAY_PLATE; 
					m_nTransferJobFlag[1] = WORK_PICK; 
				}
			}

			if(nJobFlag > 0) //작업 할 트레이가 있다
			{
				m_nTransfer_WaitPosMove_Flag = CTL_NO;
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 1100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Up_Check,	IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Down_Check,	IO_OFF);
			if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else 
			{
				m_move_step[m_nAxisNum] = 1110;		
			}
			break;

		case 1110://transfer up 
			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF);
			m_move_step[m_nAxisNum] = 1120;
			break;

		case 1120:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 1110;
			}
			break;

		case 1200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_INIT_POS], COMI.mn_runspeed_rate);  //left로 미리 이동해 있는다 
			if (nRet_1 == BD_GOOD) //좌측으로 이동  
			{			
				m_nTransfer_WaitPosMove_Flag = CTL_YES;

				m_move_step[m_nAxisNum] = 1000;

				//2017.0109
				if( st_tray_info[THD_WORK_TRANSFER].nStartStatus  == 1)
				{
					st_tray_info[THD_WORK_TRANSFER].nStartStatus = 0;
				}
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
				CTL_Lib.Alarm_Error_Occurrence(4660, dWARNING,  alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1200;
			}
			break; 

			//////////////////////////////////////////////////////////////////////
			// LOAD TRAY 해당 작업 위치로 가서 place 하는 루틴 
			//////////////////////////////////////////////////////////////////////
		case 2000:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Up_Check,	IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Tray_Remover_Z_Down_Check,	IO_OFF); 
			if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
			{ //피커를 들고 있음 
				m_move_step[m_nAxisNum] = 2100;			
			}
			else 
			{
				m_move_step[m_nAxisNum] = 2010;			
			}
			break;

		case 2010:
			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF);
			m_move_step[m_nAxisNum] = 2020;
			break;

		case 2020:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 2010;
			}
			break;

		case 2100:
			if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)
			{
				m_dpTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_PICK_POS];
			}
			else if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
			{
				m_dpTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_TRAY_REMOVE_X_PLACE_POS];
			}
			else
			{
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nAxisNum, m_dpTargetPos[m_nAxisNum], COMI.mn_runspeed_rate);  //left로 미리 이동해 있는다 
			if (nRet_1 == BD_GOOD) //좌측으로 이동  
			{			
				m_move_step[m_nAxisNum] = 3000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다  
				CTL_Lib.Alarm_Error_Occurrence(4680, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2000;
			}
			break; 

		//TRANSFER 동작 
		case 3000:
			if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Limit_Check, IO_ON); //COMI.Get_MotIOSensor(THD_LD_STACKER, MOT_SENS_SD); 	
				if(nRet_1 != IO_ON)//BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
				{
					m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
					CTL_Lib.Alarm_Error_Occurrence(4690, dWARNING, m_strAlarmCode);
					break;
				}	
			}
			else if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
			{	 //스테커의 상태를 체크할 필요도 있음 		
				if(m_nTransferJobFlag[1] == WORK_PLACE)
				{
					if(nWorkTransfer_Req[THD_EMPTY_STACKER][0] != CTL_READY) //empty에서 트레이를 받을 수 있 준비되었는지 기다린다 
					{
						break;
					}
				}
			}
			m_move_step[m_nAxisNum] = 3100;
			break;

		case 3100:
			nRet_1 = COMI.Check_MotPosRange(m_nAxisNum, m_dpTargetPos[m_nAxisNum], st_motor[m_nAxisNum].mn_allow); 			
			if (nRet_1 != BD_GOOD)
			{
				m_move_step[m_nAxisNum] = 2000;
				break;
			}

			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_ON);

			if(m_nTransferJobFlag[1] == WORK_PICK)
			{  //pick 동작 , 미리동작 
				Run_EmptyTrayTransfer.Set_Tray_Grip_Vacuum_OnOff(IO_ON);
				//air sol 이 있는지 확인 필요
			}
			else
			{  //place 동작 
			}
			m_move_step[m_nAxisNum] = 3110;

			break;

		case 3110:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown(IO_ON); 
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 3200;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(4710, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 3100;
			}
			break;

		case 3200:
			if(m_nTransferJobFlag[1] == WORK_PICK)
			{  //pick 동작 
				Run_EmptyTrayTransfer.Set_Tray_Grip_Vacuum_OnOff(IO_ON);
			}
			else //if(m_nTransferJobFlag[1] == WORK_PLACE)
			{  //place 동작 
				Run_EmptyTrayTransfer.Set_Tray_Grip_Vacuum_OnOff(IO_OFF);
			}
			m_move_step[m_nAxisNum] = 3210;
			break;

		case 3210:
			if(m_nTransferJobFlag[1] == WORK_PICK)
			{  //pick 동작 				 
				nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Grip_Vacuum_OnOff(IO_ON);
				if(nRet_1 == RET_GOOD)
				{
					m_move_step[m_nAxisNum] = 3300;
				}
				else if(nRet_1 == RET_ERROR)
				{
					Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF); 
					CTL_Lib.Alarm_Error_Occurrence(4720, dWARNING, m_strAlarmCode);
					m_move_step[m_nAxisNum] = 2000;
				}
			}
			else //if(m_nTransferJobFlag[1] == TRAY_PLACE)
			{  //place 동작 
				nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Grip_Vacuum_OnOff(IO_OFF);
				if(nRet_1 == RET_GOOD)
				{
					Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF);
					m_move_step[m_nAxisNum] = 3300;
				}
				else if(nRet_1 == RET_ERROR)
				{
					Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF); 
					CTL_Lib.Alarm_Error_Occurrence(4730, dWARNING, m_strAlarmCode);
					m_move_step[m_nAxisNum] = 2000;
				}
			} 
			break;

		case 3300: 
			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown(IO_OFF); 
			m_move_step[m_nAxisNum] = 3310;
			break;

		case 3310:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 3400;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1501, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 3300;
			}
			break;

		case 3400:
			if(m_nTransferJobFlag[1] == WORK_PICK)
			{  //pick		 
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check,	IO_ON);  
				if(nRet_1 == IO_ON)
				{
					m_move_step[m_nAxisNum] = 4000;
				}
				else
				{	//트레이 있어야 한다 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Tray_Vacuum_On_Check); 
					CTL_Lib.Alarm_Error_Occurrence(4750, dWARNING, m_strAlarmCode);
					break;
				}	
			}
			else //if(m_nTransferJobFlag[1] == WORK_PLACE)
			{  //place
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Tray_Vacuum_On_Check,	IO_OFF);
				if(nRet_1 == IO_OFF)
				{//정상 					
					m_move_step[m_nAxisNum] = 4000;
				}
				else
				{
					//트레이 없어야 한다 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Tray_Vacuum_On_Check); 
					CTL_Lib.Alarm_Error_Occurrence(4760, dWARNING, m_strAlarmCode); 
				}	
			} 
			break;

		case 4000: //트레이 정보 셋팅 
			if(m_nTransferJobFlag[1] == WORK_PICK)
			{
				if(m_nTransferJobFlag[0] == THD_LD_TRAY_PLATE)//트랜스퍼 트레이 집기 
				{
					nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_READY;
					nTransferTrayExist = CTL_YES;
					m_move_step[m_nAxisNum] = 4010;
				}
				else// if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
				{
					//error
				}
			}
			else if(m_nTransferJobFlag[1] == WORK_PLACE) //트랜스퍼 트레이 놓기 
			{				
				if(m_nTransferJobFlag[0] == THD_EMPTY_STACKER)
				{
					nTransferTrayExist =  CTL_NO;
					nWorkTransfer_Req[THD_EMPTY_STACKER][0] = CTL_FREE;
					m_move_step[m_nAxisNum] = 4100;
					break;
				}
			}
			else
			{
				//error
				break;
			}
//			m_move_step[m_nAxisNum] = 1000; 			 
			break;

		case 4010:
			if( nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_FREE )
			{
				nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_CLEAR;
				m_move_step[m_nAxisNum] = 1000; 			 
			}
			break;

		case 4100:
			if( nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_CLEAR)
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

	}

	return nFunRet;
}

int CDialog_Manual_Move::CheckCarrierType()
{
	int nFuncRet = RET_PROCEED;
	
	int nRet_1 = g_ioMgr.get_in_bit(st_io.i_press_carrier_type1, IO_OFF);
	int nRet_2 = g_ioMgr.get_in_bit(st_io.i_press_carrier_type2, IO_OFF);
	int nRet_3 = g_ioMgr.get_in_bit(st_io.i_press_carrier_type3, IO_OFF);
	
	if( mn_jig_rotator < 0 ) mn_jig_rotator = 0;
	mn_jig_rotator++;
	if( st_basic.n_mode_jig == WITHOUT_JIG)
	{
		if( mn_jig_rotator % 2 == 0) nFuncRet = RET_GOOD;
		else						 nFuncRet = RET_ERROR;
	}
	else if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && nRet_3 == IO_OFF)
	{
		if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[0] == CTL_YES &&
			st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[1] == CTL_YES &&
			st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[2] == CTL_YES)
		{
			nFuncRet = RET_ERROR;
		}
		else
		{
			nFuncRet = RET_GOOD;
		}
	}
	else if( nRet_1 == IO_ON && nRet_2 == IO_ON && nRet_3 == IO_ON)
	{
		nFuncRet = RET_ERROR;
	}
	return nFuncRet;
}

int CDialog_Manual_Move::CarrierTopForward( int nMode )
{
	int nFuncRet = RET_PROCEED;
	int nRet_1 = 0, nRet_2 = 0;

	switch( mn_RunTopFwdStep)
	{
		case 0:
			if( nMode < 0 || nMode > 1 )
			{//902000 1 A "THERE_IS_NO_MODE_IN_CARRIER_TOP_FORWARD."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "902000");
				//nFuncRet = RET_ERROR;
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
			else if( nRet_1 == RET_ERROR )//혹시 SND???
			{
				//if( nMode == 1 )
				//{
					mn_RunTopFwdStep = 3000;//전부 OFF이므로 CDIMM 동작 가능
					//작업 영역이 아니라서 한칸 민다.
				//}
				//else
				//{
				//	break;
				//}
			}
// 			else
// 			{//950000 1 A "PRESS_CARRIER_TYPE_CHECK_ERROR_PS2312_PS2313_PS2314."
// 				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, "950000");
// 				//nFuncRet = RET_ERROR;
// 				break;
// 			}
			break;

		case 1000:
			//if( nMode == 0)
			//{
				mn_RunTopFwdStep = 1100;
			//}
			//else
			//{
			//	mn_RunTopFwdStep = 3000;
			//}
			break;

		case 1100:
			//1. nMode = 0 UNPRESS
			if(mn_move_carrier == 0 )
			{
				mn_RunTopFwdStep = 3000;
			}
			else
			{
				if( nCarrierSateyflag[LDPICKER] == RET_GOOD && nCarrierSateyflag[ULDPICKER] == RET_GOOD &&
					nCarrierSateyflag[UNPRESS_Y] == RET_GOOD)
				{
					mn_RunTopFwdStep = 1200;
				}
				else
				{
					if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
					{
						break;
					}
					if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
						COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
					{
						break;
					}
					
					if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
					{
						break;
					}
					if( COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
					{
						break;
					}
					mn_RunTopFwdStep = 1200;
				}
			}

			break;

		case 1200:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_NO )
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_REQ;
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PICK;
				mn_RunTopFwdStep = 1300;
			}
			break;

		case 1300:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_READY && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_CHANGE;
				mn_RunTopFwdStep = 1400;
			}

			break;

		case 1400:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_FREE && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_NO;
				mn_RunTopFwdStep = 6000;
			}
			break;

		case 3000://한칸 민다.
			if( nCarrierSateyflag[LDPICKER] == RET_GOOD && nCarrierSateyflag[ULDPICKER] == RET_GOOD &&
				nCarrierSateyflag[UNPRESS_Y] == RET_GOOD)
			{
				mn_RunTopFwdStep = 3100;
			}
			else
			{
				if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
				{
					break;
				}
				if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
					COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
				{
					break;
				}
				if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
				{
					break;
				}
				if( COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
				{
					break;
				}
				mn_RunTopFwdStep = 3100;
			}
			break;

		case 3100:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_NO)
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_REQ;
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PLACE;
				mn_RunTopFwdStep = 3200;
			}
			break;

		case 3200:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_READY && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE)
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_CHANGE;
				mn_RunTopFwdStep = 3300;
			}
			break;

		case 3300:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_FREE && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE)
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_NO;
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

int CDialog_Manual_Move::CarrierMoveDown()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	long lMotionDone=0;

	switch(mn_RunDownStep)
	{
	case 0:
		mn_RunDownStep = 100;
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
		Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Bottom_X_ForBackward(IO_OFF);
		mn_RunDownStep = 300;
		break;

	case 300:
		if( Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF ) == RET_GOOD )
		{
			mn_RunDownStep = 100;
		}
		break;

	case 1000:
		Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_ON );//UNLOAD의 Z축을 올린다.
		mn_RunDownStep = 1100;
		break;

	case 1100:
		nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_ON );
		if( nRet_1 == IO_ON )
		{
			mn_RunDownStep = 2000;
		}
		break;

	case 2000:
		Run_Device_Carrier_Robot.Set_Device_Carrier_Clamp_FwdBwd( 0, IO_OFF );//UNLOAD의 clamp를 벌린다.
		mn_RunDownStep = 2100;
		break;

	case 2100:
		nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Clamp_FwdBwd( 0, IO_OFF);
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
			Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_OFF );
			mn_RunDownStep = 3100;
		}
		else
		{
			mn_RunDownStep = 2000;
		}
		break;

	case 3100:
		nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 0, IO_OFF );
		if( nRet_1 == RET_GOOD )
		{
			mn_RunDownStep = 4000;
		}
		else if( nRet_1 == RET_ERROR)
		{
// 			nFuncRet = RET_ERROR;
		}
		break;

	case 4000:
		Run_Device_Carrier_Robot.Set_Device_Carrier_Clamp_FwdBwd( 0, IO_ON );//UNLOAD의 clamp를 오무린다
		mn_RunDownStep = 4100;
		break;

	case 4100:
		nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Clamp_FwdBwd( 0, IO_ON);
		if( nRet_1 == IO_ON )
		{
			mn_RunDownStep = 5000;
		}
		else if( nRet_1 == RET_ERROR)
		{
// 			nFuncRet = RET_ERROR;
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

int CDialog_Manual_Move::CarrierMoveUp()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	long lMotionDone=0;

	switch(mn_RunUpStep)
	{
		case 0:
			mn_RunUpStep = 100;
			break;

		case 100:
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix( 0, IO_OFF);
			m_dwUpCarrierWaitTime[0] = GetCurrentTime();
			mn_RunUpStep = 110;
			break;

		case 110:
			m_dwUpCarrierWaitTime[1] = GetCurrentTime();
			m_dwUpCarrierWaitTime[2] = m_dwUpCarrierWaitTime[1] - m_dwUpCarrierWaitTime[0];
			if( m_dwUpCarrierWaitTime[2] <= 0 ) m_dwUpCarrierWaitTime[0] = GetCurrentTime();
			if( m_dwUpCarrierWaitTime[2] > 1000 )
			{
				mn_RunUpStep = 1000;
			}
			break;

		case 1000:
			//clamp를 벌린다.
			//자재가 없는지 센서통해서 체크한다. fix clamp도 확인한다.
			Run_Device_Carrier_Robot.Set_Device_Carrier_Clamp_FwdBwd( 1, IO_OFF );//LOAD의 clamp를 벌린다.
			mn_RunUpStep = 1100;
			break;

		case 1100:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Clamp_FwdBwd( 1, IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_RunUpStep = 2000;
			}
			else if( nRet_1 == RET_ERROR )
			{
// 				nFuncRet = RET_ERROR;
			}
			break;

		case 2000:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_OFF);

			if( nRet_1 == IO_ON && nRet_2 == IO_OFF )
			{
				Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix( 0, IO_OFF);//혹시나
				Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_ON );//LOAD의 Z축을 올린다.
				mn_RunUpStep = 2100;
			}
			else
			{
				mn_RunUpStep = 1000;
			}
			break;

		case 2100:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_ON );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunUpStep = 3000;
			}
			break;

		case 3000:
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix( 0, IO_OFF);//혹시나
			Run_Device_Carrier_Robot.Set_Device_Carrier_Clamp_FwdBwd( 1, IO_ON );//UNLOAD의 clamp를 오무린다
			mn_RunUpStep = 3100;
			break;

		case 3100:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Clamp_FwdBwd( 1, IO_ON);
			if( nRet_1 == IO_ON )
			{
				mn_RunUpStep = 4000;
			}
			break;

		case 4000:
			Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_OFF );
			mn_RunUpStep = 4100;
			break;

		case 4100:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Z_Cylinder_UpDown( 1, IO_OFF );
			if( nRet_1 == IO_ON )
			{
				mn_RunUpStep = 5000;
			}
			break;

		case 5000:
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix( 0, IO_ON);
			m_dwUpCarrierWaitTime[0] = GetCurrentTime();
			mn_RunUpStep = 5100;
			break;
			
		case 5100:
			m_dwUpCarrierWaitTime[1] = GetCurrentTime();
			m_dwUpCarrierWaitTime[2] = m_dwUpCarrierWaitTime[1] - m_dwUpCarrierWaitTime[0];
			if( m_dwUpCarrierWaitTime[2] <= 0 ) m_dwUpCarrierWaitTime[0] = GetCurrentTime();
			if( m_dwUpCarrierWaitTime[2] > 1000 )
			{
				mn_RunUpStep = 6000;
			}
			break;

		case 6000:
			mn_RunUpStep = 0;
			nFuncRet = RET_GOOD;
			break;
	}
	return nFuncRet;
}



int CDialog_Manual_Move::CarrierBtmForwrad()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1 = 0, nRet_2 = 0;
	CString strTmp;
	
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
		nRet_1 = IO_ON;
		if(st_basic.n_mode_jig == WITHOUT_JIG)
		{
			nRet_1 = IO_ON; nRet_2 = IO_OFF;
		}
		if( nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			mn_RunBtmFwdStep = 2000;
		}
		else
		{
			//if( nRet_1 == IO_OFF ) strTmp.Format("8%d%04d", IO_OFF, st_io.i_Carrier_Z_1_Down_Check);
			//else                          strTmp.Format("8%d%04d", IO_ON, st_io.i_Carrier_Z_2_Down_Check);
			//CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, strTmp);
// 			nFuncRet = RET_ERROR;
		}
		break;
		
	case 2000:
		Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Bottom_X_ForBackward( IO_ON );
		mn_RunBtmFwdStep = 2100;
		break;
		
	case 2100:
		nRet_1 =  Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_ON);
		if( nRet_1 == IO_ON )
		{
			mn_RunBtmFwdStep = 3000;
		}
		break;
		
	case 3000:
		nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);//하단 지그체크
		nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);
		if(st_basic.n_mode_jig == WITHOUT_JIG)
		{
			nRet_1 = IO_OFF; nRet_2 = IO_ON;
		}
		if( nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			mn_RunBtmFwdStep = 4000;
		}
		else
		{
			//if( nRet_1 == IO_OFF ) strTmp.Format("8%d%04d", IO_ON, st_io.i_Carrier_Z_1_Down_Check);
			//else                          strTmp.Format("8%d%04d", IO_OFF, st_io.i_Carrier_Z_2_Down_Check);
			//CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, strTmp);
		}
		break;
		
	case 4000:
		Run_Device_Carrier_Robot.Set_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF );
		mn_RunBtmFwdStep = 4100;
		break;
		
	case 4100:
		nRet_1 =  Run_Device_Carrier_Robot.Chk_Device_Carrier_Slide_Bottom_X_ForBackward( IO_OFF);
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

int CDialog_Manual_Move::Check_Carrier_Move_Enable( int nMode)
{
	int nFuncRet = RET_ERROR;
	CString stralarm=_T("");
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
	nRet[12] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_OFF);		// Right 끝단에서 Jig 걸어줌 
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
		nRet[16] = IO_ON;
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_OFF;	nRet[15] = IO_OFF;	nRet[16] = IO_ON; nRet[17] = IO_ON;
		}
		
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON && nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF && 
			nRet[9] == IO_ON && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON && nRet[13] == IO_OFF && nRet[14] == IO_OFF && nRet[15] == IO_OFF && nRet[16] == IO_ON && nRet[17] == IO_ON )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if     ( nRet[0] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Press_Up_Check);
			else if( nRet[1] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Press_Down_Check);
			else if( nRet[2] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Slide_Guide_X1_Forward_Check);
			else if( nRet[3] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Slide_Guide_X1_Backward_Check);
			else if( nRet[4] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Slide_Guide_X2_Up_Check);
			else if( nRet[5] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Slide_Guide_X2_Down_Check);
			else if( nRet[6] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Slide_Guide_Z1_Up_Check);
			else if( nRet[7] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Slide_Guide_Z1_Down_Check);
			else if( nRet[8] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Slide_Guide_Z2_Up_Check);
			else if( nRet[9] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Slide_Guide_Z2_Down_Check);
			else if( nRet[10] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Carrier_1_Forward_Check);
			else if( nRet[11] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Carrier_1_Backward_Check);
			else if( nRet[12] == IO_OFF ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Carrier_2_Forward_Check);
			else if( nRet[13] == IO_ON ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Carrier_2_Backward_Check);
			else if( nRet[14] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Carrier_Z_1_Up_Check);
			else if( nRet[15] == IO_ON ) stralarm.Format("8%d%04d",IO_OFF,st_io.i_Carrier_Z_1_Down_Check);
			else if( nRet[16] == IO_OFF ) stralarm.Format("8%d%04d",IO_ON,st_io.i_Carrier_Z_2_Up_Check);
			else/* if( nRet[17] != IO_ON )*/ stralarm.Format("8%d%04d",IO_ON,st_io.i_Carrier_Z_2_Down_Check);
			
			CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, stralarm);
		}
	}
	else if ( nMode == 1 )//상단 민 상태
	{
		nRet[14] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_ON);	
		nRet[15] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_OFF);	
		nRet[16] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_OFF);	
		nRet[17] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_ON);
		nRet[14] = IO_ON;
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_ON;	nRet[15] = IO_OFF;	nRet[16] = IO_OFF; nRet[17] = IO_ON;
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
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_OFF;	nRet[15] = IO_ON;	nRet[16] = IO_OFF; nRet[17] = IO_ON;
		}

		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_OFF && nRet[3] == IO_ON && nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF && 
			nRet[9] == IO_ON && nRet[10] == IO_OFF && nRet[11] == IO_ON && nRet[12] == IO_ON && nRet[13] == IO_OFF && nRet[14] == IO_OFF && nRet[15] == IO_ON && nRet[16] == IO_OFF && nRet[17] == IO_ON )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "931002");
		}
	}
	else if(nMode == 3)//3 한다 업 상태
	{
		nRet[14] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Up_Check, IO_OFF);	
		nRet[15] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_1_Down_Check, IO_ON);	
		nRet[16] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Up_Check, IO_ON);	
		nRet[17] = g_ioMgr.get_in_bit(st_io.i_Carrier_Z_2_Down_Check, IO_OFF);
		if( st_basic.n_mode_jig == WITHOUT_JIG )
		{
			nRet[14] = IO_OFF;	nRet[15] = IO_ON;	nRet[16] = IO_ON; nRet[17] = IO_OFF;
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

int CDialog_Manual_Move::Move_CarrierTransfer()
{
	int nRet_1 = 0, i =0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = 99;
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 100;
			break;

		case 100:
			if( Check_Carrier_Move_Enable(0) != RET_GOOD )//초기상태
			{
				break;
			}
			m_move_step[m_nAxisNum] = 200;
			break;

		case 200:
			nRet_1 = CarrierTopForward(1);
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum]  = 1000;//작업 영역
			}
			else if( nRet_1 == RET_SKIP )
			{
				m_move_step[m_nAxisNum]  = 210;
			}
			break;
		case 210:
			if( Check_Carrier_Move_Enable(1) != RET_GOOD )//한칸 민 상태
			{
				break;
			}
			m_move_step[m_nAxisNum] = 300;
			break;

		case 300:
			if( CarrierMoveDown() != RET_GOOD )
			{
				break;
			}
			m_move_step[m_nAxisNum] = 310;
			break;


		case 310:
			if( Run_Device_Carrier_Robot.Check_Carrier_Move_Enable(2) != RET_GOOD )
			{
				break;
			}
			m_move_step[m_nAxisNum] = 400;
			break;

		case 400:
			if( CarrierMoveUp() != RET_GOOD )
			{
				break;
			}
			m_move_step[m_nAxisNum] = 410;
			break;

		case 410:
			if( Run_Device_Carrier_Robot.Check_Carrier_Move_Enable(3) != RET_GOOD )//한칸 UP 상태
			{
				break;
			}
			m_move_step[m_nAxisNum] = 420;
			break;

		case 420:
			if( CarrierBtmForwrad() == RET_GOOD )//하단 이동
			{
				if( mn_move_carrier == 0 ) m_move_step[m_nAxisNum] = 0;
				else					   m_move_step[m_nAxisNum] = 0;
				mn_BufferPos = 0;
			}
			break;

		case 1000:
			m_move_step[m_nAxisNum] = 1100;
			break;

			//현재 사이트에 자재기 있는지 비어 있는지 체크한다.
		case 1100:
			if( Check_Carrier_Move_Enable(0) != RET_GOOD )//초기상태
			{
				break;
			}
			m_move_step[m_nAxisNum] = 2000;//LOAD하자
			//mn_RunMove = 7000;
			break;

		case 2000:
			if( mn_move_carrier < 2 ) break;
			if( mn_move_carrier == 2)
			{
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = WORK_PLACE;
				}
			}
			if( mn_move_carrier == 4 )
			{
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
				}
			}
			else if( mn_move_carrier == 7 )
			{
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = WORK_PLACE;
				}
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
				}
			}
			else if( mn_move_carrier == 8 )
			{
				if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] = WORK_PLACE;
				}
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = WORK_PLACE;
				}
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO )
				{
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_REQ;
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
				}
			}
			else if(mn_move_carrier >= 3)
			{
// 				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO )
// 				{
// 					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_REQ;
// 					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
// 					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
// 				}
// 				if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_NO )
// 				{
// 					nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_REQ;
// 					nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] = WORK_PLACE;
// 				}
			}
			if( mn_move_carrier == 2)
			{
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_READY )
				{
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_CHANGE;
					m_Thread_Flag[1] = CTL_YES;
				}
				m_Thread_Flag[0] = CTL_YES;/* m_Thread_Flag[1] = CTL_YES; */
				m_Thread_Flag[2] = CTL_YES; m_Thread_Flag[3] = CTL_YES;
			}
			else if(mn_move_carrier == 4)
			{
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY )
				{
					m_Thread_Flag[0] = CTL_YES;
				}
				m_Thread_Flag[1] = CTL_YES; m_Thread_Flag[2] = CTL_YES; m_Thread_Flag[3] = CTL_YES;
			}
			else if(mn_move_carrier == 7)
			{
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_READY )
				{
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_CHANGE;
					m_Thread_Flag[1] = CTL_YES;
				}
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY )
				{
					m_Thread_Flag[0] = CTL_YES;
				}
				m_Thread_Flag[2] = CTL_YES; m_Thread_Flag[3] = CTL_YES;
			}
			else if(mn_move_carrier == 8)
			{
				if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_READY )
				{
					nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_CHANGE;
					m_Thread_Flag[2] = CTL_YES;
				}
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_READY )
				{
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_CHANGE;
					m_Thread_Flag[1] = CTL_YES;
				}
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY )
				{
					m_Thread_Flag[0] = CTL_YES;
				}
				m_Thread_Flag[3] = CTL_YES;
			}

			else
			{
// 				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY )
// 				{
// 					m_Thread_Flag[0] = CTL_YES;
// 				}
// 				if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_READY )
// 				{
// 					st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_CHANGE;
// 					m_Thread_Flag[2] = CTL_YES;
// 				}
// 				if( nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_READY )
// 				{
// 					nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_CHANGE;
// 					m_Thread_Flag[3] = CTL_YES;
// 				}
				break;
			}

			if( m_Thread_Flag[0] == CTL_YES && m_Thread_Flag[1] == CTL_YES && m_Thread_Flag[2] == CTL_YES && m_Thread_Flag[3] == CTL_YES)
			{
				m_Thread_Flag[0] = m_Thread_Flag[1] = m_Thread_Flag[2] = m_Thread_Flag[3] = CTL_NO;
				m_move_step[m_nAxisNum] = 2100;
			}
			break;

		case 2100:
// 			if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO &&
// 				st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_NO )
// 			{
// 				mn_RunMove = 1100;
// 			}
			if( mn_move_carrier == 2)
			{
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO)
				{
					m_move_step[m_nAxisNum] = 0;
				}

			}
			else if( mn_move_carrier == 4 )
			{
				if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO)
				{
					m_move_step[m_nAxisNum] = 0;
				}
			}
			else if( mn_move_carrier == 7 )
			{
				if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO && 
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO)
				{
					m_move_step[m_nAxisNum] = 0;
				}

			}
			else if( mn_move_carrier == 8 )
			{
				if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_NO &&
					nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_NO && 
					nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_NO)
				{
					m_move_step[m_nAxisNum] = 0;
				}
				
			}

			else
			{
				break;
			}
			break;

		case 4000:
			nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] = CTL_REQ;
			nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] = WORK_PICK;
			nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2] = mn_BufferPos;//Buffer position
			m_move_step[m_nAxisNum] = 4100;
			break;

		case 4100:
			if( nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] == CTL_READY )
			{
				m_move_step[m_nAxisNum] = 1100;
			}
			break;

	}

	return nFunRet;

}

int CDialog_Manual_Move::Move_UnPressTransfer()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0, nRet_3 = 0, nRet_4 = 0;
	int m_nAxisNum = M_CARRIER_X;
	int m_nPressAxisY = M_PRESS_Y;
	int nRet[10]={0,};

	double mCurrPosX = COMI.Get_MotCurrentPos(m_nAxisNum);
	double mCurrPosY = COMI.Get_MotCurrentPos(m_nPressAxisY);
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 100;
			break;
			
		case 100:
			if( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				m_move_step[m_nAxisNum] = 210;
				break;
			}
			else
			{
				if( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_INIT_POS] + st_motor[m_nAxisNum].mn_allow ) )
				{
					m_move_step[m_nAxisNum] = 300;
					break;
				}				
			}
			m_move_step[m_nAxisNum] = 500;
			break;

		case 210:
// 			if( Func.Check_RunAllSafety() != RET_GOOD || Chk_PressClamp_Safety(0) != RET_GOOD )
// 			{
// 				m_move_step[m_nAxisNum] = 100;
// 				COMI.Set_MotStop(0 , m_nPressAxisY);
// 				COMI.Set_MotStop(0 , m_nAxisNum);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisY, st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 210;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 210;
			}
			break;

		case 300:
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisY, st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 210;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 210;
			}
			break;


		case 500:
			m_move_step[m_nAxisNum] = 1000;
			break;


		//////////////////////////////////////////////////////////////////////////
		//1. 작업영역이라서 UNPRESS 한다.
		//2. 한칸 미는 동작을 수행한다.
		//////////////////////////////////////////////////////////////////////////
		case 1000:
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}

			//작업영역이라서 UNPRESS 한다.
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK )
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
				m_move_step[m_nAxisNum] = 1100;
			}//한칸 민다
			else if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE )
			{
				nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
				m_move_step[m_nAxisNum] = 3000;
			}
			break;

		case 1100:
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(1, IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			m_move_step[m_nAxisNum] = 1110;
			break;

		case 1110:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 1, IO_ON );
			nRet_2 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 2, IO_ON );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 1120;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
				nFunRet = RET_GOOD;
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 1120:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK )
			{
				nCarrierSateyflag[UNPRESS_Y] = RET_ERROR;
				m_nTrayDeviceGap = 0;
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2000:
			//nMode = 0 전부 업하여 모터가 움직일때
// 			if( m_nTrayDeviceGap == 0 && Chk_PressClamp_Safety(0) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				COMI.Set_MotStop( 0 , m_nAxisNum);
// 				break;
// 			}
// 			else
// 			{//nMode = 3 Holder down상태로 움직일때
// 				if( Chk_PressClamp_Safety(3) == RET_ERROR )
// 				{
// 					COMI.Set_MotStop( 0 , m_nAxisNum);
// 					COMI.Set_MotStop( 0 , m_nPressAxisY);
// 					break;
// 				}
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}

			if( m_nTrayDeviceGap > 0 && ( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nAxisNum].mn_allow ) ||
				mCurrPosX < ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nAxisNum].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( m_nTrayDeviceGap > 0 )
			{
				if( ( mCurrPosX < ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nAxisNum].mn_allow ) ) &&
					( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nAxisNum].mn_allow ) ) )
				{
					m_move_step[m_nAxisNum] = 2100;
					break;
				}
			}		

			m_dTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nAxisNum, m_dTargetPos[m_nAxisNum], COMI.mn_manualspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2010;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2010:
			m_dTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nAxisNum, m_dTargetPos[m_nAxisNum], COMI.mn_manualspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		//UNPRESS 실린더 다운시켜서 움직인다.
		case 2100:
// 			if( Chk_PressClamp_Safety(0) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nAxisNum);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			Run_UnPress_Robot.Set_Device_Carrier_Holder(IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, IO_OFF);//Pusher
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, IO_ON);//Press
			m_move_step[m_nAxisNum] = 2200;
			break;

		case 2200:
			nRet_1 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(1, IO_ON);
			if( nRet_1 != RET_GOOD || nRet_2 != RET_GOOD) break;
			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_ON );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_OFF );
			nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_ON );
			nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_OFF );
			if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON)
			{
				nRet_1 = Run_UnPress_Robot.Chk_Device_Carrier_Holder(IO_ON);
				nRet_2 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix(2, IO_ON);
				if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
				{
					m_move_step[m_nAxisNum] = 2300;
				}
				else if(nRet_1 == RET_ERROR || nRet_2 == RET_ERROR)
				{
					//CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
					//m_move_step[m_nAxisNum] = 2200;
				}
			}
			else
			{
				if( nRet[0] != IO_ON || nRet[2] != IO_ON)
				{
					m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, m_strAlarmCode);
				}
				else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
				{
					m_strAlarmCode.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, m_strAlarmCode);
				}
			}
			break;
			
		case 2300:
			//UNPRESS 실린더가 다운상태에서 움직여야 한다.
// 			if( Chk_PressClamp_Safety(3) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}

			if( m_nTrayDeviceGap >= 0 && ( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nAxisNum].mn_allow ) ||
				mCurrPosX < ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nAxisNum].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPos[m_nPressAxisY] = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_PRESS1_POS] + (m_nTrayDeviceGap*st_recipe.dLoaderTransferTrayDeviceGap);
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nPressAxisY, m_dTargetPos[m_nPressAxisY], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2310;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2300;
			}
			break;

		case 2310:
			//UNPRESS 실린더가 다운상태에서 움직여야 한다.
// 			if( Chk_PressClamp_Safety(3) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nAxisNum);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if( m_nTrayDeviceGap >= 0 && ( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nAxisNum].mn_allow ) ||
				mCurrPosX < ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nAxisNum].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPos[m_nPressAxisY] = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_PRESS1_POS] + (m_nTrayDeviceGap*st_recipe.dLoaderTransferTrayDeviceGap);
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nPressAxisY, m_dTargetPos[m_nPressAxisY], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2320;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2300;
			}
			break;

			//Site 하나씩 UNPRESS 한다.
		case 2320:
			if( g_ioMgr.get_in_bit(st_io.i_Press_UpDown_CellIn_Check, IO_ON) != IO_ON)
			{
				m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, m_strAlarmCode);
				break;
			}
			//Run_UnPress_Robot.Set_Device_Carrier_Holder(OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, IO_OFF);
			m_move_step[m_nAxisNum] = 2330;
			break;

		case 2330:
// 			if( Run_UnPress_Robot.Chk_Device_Carrier_Holder(OFF) == RET_GOOD)
// 			{
// 				m_move_step[m_nAxisNum] = 2400;
// 			}
			nRet_1 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 0, IO_OFF );
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 1, IO_OFF );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2400;
			}
			break;

			//nMode = 1 Holder 상태에서  Press2로 움직인다.
		case 2400:
// 			if( Chk_PressClamp_Safety(1) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				COMI.Set_MotStop( 0 , m_nAxisNum);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if( ( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nAxisNum].mn_allow ) ) ||
				( mCurrPosX < ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nAxisNum].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPos[m_nPressAxisY] = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_PRESS2_POS] + (m_nTrayDeviceGap*st_recipe.dLoaderTransferTrayDeviceGap);
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisY, m_dTargetPos[m_nPressAxisY], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2410;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2400;
			}
			break;	

			//Press 이동 후 Holder 실린더를 내린다.
		case 2410:
			Run_UnPress_Robot.Set_Device_Carrier_Holder(IO_ON);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, IO_ON);
			m_move_step[m_nAxisNum] = 2420;
			break;

		case 2420:
			nRet_1 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(1, IO_ON);
			if( nRet_1 != RET_GOOD || nRet_2 != RET_GOOD) break;

			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_OFF );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_ON );
			nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_OFF );
			nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_ON );
			if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON)
			{
				nRet_1 = Run_UnPress_Robot.Chk_Device_Carrier_Holder(ON);
				if(nRet_1 == RET_GOOD)
				{
					m_move_step[m_nAxisNum] = 2500;
				}
				else if(nRet_1 == RET_ERROR)
				{
// 					CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
// 					m_move_step[m_nAxisNum] = 2420;
				}
			}
			else
			{
				if( nRet[0] != IO_OFF || nRet[2] != IO_OFF)
				{
					m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, m_strAlarmCode);
				}
				else if( nRet[1] != IO_ON || nRet[3] != IO_ON)
				{
					m_strAlarmCode.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, m_strAlarmCode);
				}
			}
			break;

		case 2500:
			m_nTrayDeviceGap++;
			if( m_nTrayDeviceGap >= 3 )
			{// 전부 UNPRESS를 했으므로 원래 상태로돌린다
				m_nTrayDeviceGap = 0;
				m_move_step[m_nAxisNum] = 2550;
				Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, IO_OFF);
				Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, IO_ON);
			}
			else
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2510:
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, IO_ON);
			m_move_step[m_nAxisNum] = 2550;
			break;

		case 2550:
			nRet_1 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(1, IO_ON);
			if( nRet_1 != RET_GOOD || nRet_2 != RET_GOOD) break;
			m_move_step[m_nAxisNum] = 2600;
			break;


		case 2600:
// 			if( Chk_PressClamp_Safety(3) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_OFF );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_ON );
			nRet[2] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Up_Check, IO_ON );
			nRet[3] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Down_Check, IO_OFF );
			if( nRet[0] != IO_OFF || nRet[1] != IO_ON || nRet[2] != IO_ON || nRet[3] != IO_OFF)
			{
				m_move_step[m_nAxisNum] = 2510;
				break;
			}


			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}

			if( ( mCurrPosX > ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nAxisNum].mn_allow ) ) ||
				( mCurrPosX < ( st_motor[m_nAxisNum].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nAxisNum].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPos[m_nPressAxisY] = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisY, m_dTargetPos[m_nPressAxisY], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2610;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2600;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2600;
			}
			break;
		
		case 2610:
			//Run_UnPress_Robot.Set_Device_Carrier_Holder(OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, IO_OFF);
			m_move_step[m_nAxisNum] = 2620;
			break;

		case 2620:
			nRet_1 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(1, IO_OFF);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2700;
			}	
			break;

		case 2700:
			if( mCurrPosY > ( st_motor[m_nAxisNum].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nAxisNum].mn_allow )  )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_PRESS_Y_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nAxisNum, m_dTargetPos[m_nAxisNum], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2710;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2700;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2700;
			}
			break;

		case 2710:
			nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_FREE;
			m_move_step[m_nAxisNum] = 1000;
			nCarrierSateyflag[UNPRESS_Y] = RET_GOOD;
			break;

		case 3000:
			if( nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE )
			{
				st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_ERROR;
				m_move_step[m_nAxisNum] = 3100;
			}
			break;

		case 3100:
			//nMode = 0 전부 업하여 모터가 움직일때
// 			if( Chk_PressClamp_Safety(0) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nAxisNum);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}	

			m_dTargetPos[m_nAxisNum]  = st_motor[m_nAxisNum].md_pos[P_CARRIER_X_PUSHER_DOWN];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nAxisNum, m_dTargetPos[m_nAxisNum] , COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 3110;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3100;
			}
			break;

		case 3110:
// 			if( Chk_PressClamp_Safety(0) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisX);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_OFF);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(1, IO_OFF);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_OFF);

			Run_UnPress_Robot.Set_Device_Carrier_Holder( OFF);//UNPRESS
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(0, ON);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff(1, OFF);
			m_move_step[m_nAxisNum] = 3120;
			break;


		case 3120:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 1, IO_OFF );
			nRet_2 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 2, IO_OFF );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				nRet_1 = Run_UnPress_Robot.Chk_Device_Carrier_Holder(OFF);
				nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 0, ON );
				nRet_3 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 1, OFF );

				if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD && nRet_3 == RET_GOOD )
				{
					m_move_step[m_nAxisNum] = 3200;
				}
				else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR || nRet_3 == RET_ERROR )
				{
					m_move_step[m_nAxisNum] = 0;
				}
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
				nFunRet = RET_GOOD;
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 3200:
			//PRESS 실린더가 다운상태에서 움직여야 한다.
// 			if( Chk_PressClamp_Safety(2) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisX);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_CARRIER_X_PUSH_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nAxisNum, m_dTargetPos[m_nAxisNum], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 3300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3200;
			}
			break;

		case 3300:
			m_dwWaitUntil[1] = GetCurrentTime();
			m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
			if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
			if( m_dwWaitUntil[2] > 1000 )
			{
				m_move_step[m_nAxisNum] = 3310; //Carrier X를 조금 두;로 뺀다. 실린더를 업하기 위해
			}
			break;

		case 3310://실린더 고정
			Run_UnPress_Robot.Set_Device_Carrier_Holder(ON);
			m_move_step[m_nAxisNum] = 3320;
			break;

		case 3320:
			nRet_1 = Run_UnPress_Robot.Chk_Device_Carrier_Holder(ON);
			if( nRet_1 == RET_GOOD)
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 3400;
			}
			else if( nRet_1 == RET_ERROR)
			{
			}
			break;

		case 3400:
			m_dwWaitUntil[1] = GetCurrentTime();
			m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
			if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
			if( m_dwWaitUntil[2] < 500 ) break;

			//PRESS 실린더가 다운상태에서 움직여야 한다.
// 			if( Chk_PressClamp_Safety(2) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisX);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - st_motor[M_EPOXY_TRANSFER_X].mn_allow ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + st_motor[M_EPOXY_TRANSFER_Y].mn_allow ))
			{
				break;
			}
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_CARRIER_X_BACK_PUSH_POS];
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nAxisNum, m_dTargetPos[m_nAxisNum], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 3410;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3400;
			}
			break;

		case 3410:
			Run_UnPress_Robot.Set_Device_Carrier_Holder(IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff( 0 ,IO_OFF);
			Run_UnPress_Robot.Set_UnPress_PushClamp_OnOff( 1 ,IO_OFF);
			m_move_step[m_nAxisNum] = 3420;
			break;

		case 3420:
			nRet_1 = Run_UnPress_Robot.Chk_Device_Carrier_Holder(IO_ON);
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 0, IO_OFF );
			nRet_3 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 1, IO_OFF );
			nRet_4 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix(2, IO_ON);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD && nRet_3 == RET_GOOD && nRet_4 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 3500;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR || nRet_3 == RET_ERROR || nRet_4 == RET_ERROR)
			{
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 3500:
			//PRESS 실린더가 다운상태에서 움직여야 한다.
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nAxisNum);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPos[m_nAxisNum] = st_motor[m_nAxisNum].md_pos[P_CARRIER_X_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nAxisNum, m_dTargetPos[m_nAxisNum], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 3600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3500;
			}
			break;

		case 3600:
			nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_FREE;
			nCarrierSateyflag[UNPRESS_Y] = RET_GOOD;
			m_move_step[m_nAxisNum] = 1000;
			break;	



 	}
	return nFunRet;

}

int CDialog_Manual_Move::Robot_Move_Safety_Zone( int nMode, int n_site, int n_flag)
{
	int Ret=0, nRet_1=0, FuncRet = RET_PROCEED, i=0;
	double dCurrentPosX =0, dCurrentPosY=0;
	int m_nRobot_X = M_EPOXY_TRANSFER_X;
	int m_nRobot_Y = M_EPOXY_TRANSFER_Y;
	int m_nRobot_Z = M_EPOXY_TRANSFER_Z;
	double m_dpTargetPosList[3];
	double dp_SpdRatio[3];

	int m_nLinearMove_Index	= 1;	//현재 map index
	int m_lAxisCnt	= 2;				 //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개
	long	m_lpAxisNum[4];
	m_lpAxisNum[0] = M_EPOXY_TRANSFER_X; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = M_EPOXY_TRANSFER_Y;
	m_lpAxisNum[2] = M_EPOXY_TRANSFER_Z;

	//nMode == 0 => Carrier 쪽으로 이동
	//nMode == 1  ==> 작업 완료후 안전위치로 이동
	//nMode == 2 => 작업자 쪽으로이동 ->Clean  DISCHARGE_POS

	//n_site 아직
	//n_flag == 아직
	dCurrentPosX = COMI.Get_MotCurrentPos( m_nRobot_X );
	dCurrentPosY = COMI.Get_MotCurrentPos( m_nRobot_Y );
	switch(mn_SafetyStep)
	{		 
		case 0:
			mn_SafetyStep = 100;
			break;

		case 100:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.md_allow_value[m_nRobot_Z]);
			if( nRet_1 == RET_GOOD )
			{
				mn_SafetyStep = 1000;
			}
			else
			{
				mn_SafetyStep = 200;
			}
			break;

		case 200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_SafetyStep = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 200;
			}
			break;

		case 1000:			
			if( dCurrentPosY >= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - COMI.md_allow_value[m_nRobot_Y] ) )
			{
				if( dCurrentPosX >=  ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) )
				{//Carrier영역  이영역에서는 안전영역이므로 linear로 이동이 가능하다
					if(nMode == 0)  //로보트가 Carrier로 들어가는 동작 
					{
						mn_SafetyStep = 1100;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else if( nMode == 1 )
					{
						mn_SafetyStep = 1200;
					}
					else //if(nMode == 2) //로보트가 Carrier 이외의 작업을 동작 할 예정
					{
						mn_SafetyStep = 2000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
					}
				}
				else//약간 왼쪽 영역이므로 일단 약간 오른쪽으로 이동 후 동작한다.
				{
					if(nMode == 0 )  //로보트가 Carrier로 들어가는 동작 
					{
						mn_SafetyStep = 1100;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else if(  nMode == 1)//안전 위칠 빠진다.
					{
						mn_SafetyStep = 5000;
					}
					else //if(nMode == 2) //로보트가 Carrier 이외의 작업을 동작 할 예정
					{
						mn_SafetyStep = 4000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
					}
				}
			}
			else //if( dCurrentPosY < ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - st_motor[m_nRobot_Y].mn_allow ) )
			{
				if( dCurrentPosX >=  ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) )
				{//Carrier영역  이영역에서는 안전영역이므로 linear로 이동이 가능하다
					//10mm정도 떨어져 있어도 좋다고 본다 체크해야한다.
					if( dCurrentPosY > ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - ( 10 * COMI.md_allow_value[m_nRobot_Y] ) ) )
					{
						if(nMode == 0 || nMode == 1)  //로보트가 Carrier로 들어가는 동작 
						{
							mn_SafetyStep = 5000;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
						}
						else //if(nMode == 2) //로보트가 Carrier 이외의 작업을 동작 할 예정
						{
							mn_SafetyStep =6000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
						}
					}
					else
					{//알람 트레이 트랜스퍼와 충돌 우려가 있다
						COMI.Set_MotStop( 0 , m_nRobot_X );
						COMI.Set_MotStop( 0 , m_nRobot_Y );
						COMI.Set_MotPower( m_nRobot_X, IO_OFF);
						COMI.Set_MotPower( m_nRobot_Y, IO_OFF);
						//960000 1 A "EPOXY_MOTOR_IS_NOT_SAFETY_MOVE_MANUALLY."
						CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, "960000");
						mn_SafetyStep = 0;
					}					
				}
				else//약간 왼쪽 영역이므로 일단 약간 오른쪽으로 이동 후 동작한다.
				{
					if(nMode == 0 || nMode == 1)  //로보트가 Carrier로 들어가는 동작 
					{
						mn_SafetyStep = 7000;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else //if(nMode == 1) //로보트가 Carrier 이외의 작업을 동작 할 예정
					{
						mn_SafetyStep = 8000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
					}
				}
			}
			break;			

		case 1100:
			mn_SafetyStep = 0;
			FuncRet = RET_GOOD;			
			break;

		case 1200:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS];

			dp_SpdRatio[0] = (double)50;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 0;
				FuncRet = RET_GOOD;	
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 1200;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 2000://clean 영역으로 이동 DISCHARGE_POS
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY];

			dp_SpdRatio[0] = (double)50;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 2000;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 2100:
			if( dCurrentPosY >= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - (5*COMI.md_allow_value[m_nRobot_Y]) ) &&
				dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + (5*COMI.md_allow_value[m_nRobot_Y]) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 2200;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 2000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					mn_SafetyStep = 2000;
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 2200:
			if( 	dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y])  &&
				dCurrentPosX <= ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_Y]) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{					
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 2200;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 2200;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 3000://Y가 safety위이고 x가 safety보다 작은데 위치할 경우 우선 safety로 이동하고 Init위치로 이동한다
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY];

			dp_SpdRatio[0] = (double)st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 3000;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 3100:
			if( dCurrentPosY >= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - (5*COMI.md_allow_value[m_nRobot_Y] ) )&&
				dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + (5*COMI.md_allow_value[m_nRobot_Y] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 3100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					mn_SafetyStep = 3100;
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 4000://Y가 safety위이고 x가 safety보다 작은데 위치할 경우 우선 safety로 이동하고 Init위치로 이동한다
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY];

			dp_SpdRatio[0] = (double)50;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 4100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 4000;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 4100:
			if( 	( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y]) ) &&
				( dCurrentPosX <= ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_Y]) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{					
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 4100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 4100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 5000:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS];

			dp_SpdRatio[0] = (double)50;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 0;
				FuncRet = RET_GOOD;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 5000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 5000;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 6000:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS];

			dp_SpdRatio[0] = (double)50;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 6100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 6000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 6000;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 6100:
			if( 	dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y])  &&
				dCurrentPosX <= ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_X]) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{					
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 6100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 6100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 7000:
			if( 	( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 7100;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 7000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 7000;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 7100:
			if( 	( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 7200;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 7100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 7100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 7200:
			if( ( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS] + COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 7200;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 7200;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;


		case 8000:
			if( 	( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X]) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 8100;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 8000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 8000;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 8100:
			if( 	( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 8100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 8100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;
	}


	return FuncRet;
}

int CDialog_Manual_Move::Move_Billiard_Epoxy( int nMode, int nSite)//, int nStartSecond)//nMode: billard or Dot, nSite : site,  nStartSecond: start or second
{
	int nRet_1,nRet_2;
	long lMotionDone=0;
	bool bSkip = false;
	int FuncRet = RET_PROCEED;

	double m_dpTargetPosList[2];
	double dp_SpdRatio[3];

	int m_nLinearMove_Index	= 1;	//현재 map index
	int m_lAxisCnt	= 2;				 //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개
	long m_lpAxisNum[4];
	m_lpAxisNum[0] = M_EPOXY_TRANSFER_X; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = M_EPOXY_TRANSFER_Y;
	m_lpAxisNum[2] = M_EPOXY_TRANSFER_Z;

	COMI.ml_axiscnt[m_nLinearMove_Index] = 2;
	COMI.mp_axisnum[m_nLinearMove_Index][0] = M_EPOXY_TRANSFER_X;
	COMI.mp_axisnum[m_nLinearMove_Index][1] = M_EPOXY_TRANSFER_Y;
	COMI.mp_axisnum[m_nLinearMove_Index][2] = M_EPOXY_TRANSFER_Z;
	
	
	int m_nRobot_X = M_EPOXY_TRANSFER_X;
	int m_nRobot_Y = M_EPOXY_TRANSFER_Y;
	int m_nRobot_Z = M_EPOXY_TRANSFER_Z;
	int m_nRobot_S = M_EPOXY_SCREW;


	switch(mn_MoveStep)
	{
		case 0:
			mn_FirstSecond = 0;
			mn_StartEnd = 0;
			COMI.Set_MotStop(0, m_nRobot_S);
			mn_MoveStep = 100;
			break;

		case 100:
			if( COMI.Get_MotCurrentPos(m_nRobot_Z)  > ( st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z] ) )
			{
				mn_MoveStep = 200;
			}
			else
			{
				mn_MoveStep = 1000;
			}
			break;

		case 200:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 100;
			}	
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 200;
			}
			break;

		case 1000:
			if( st_recipe.dLoaderTransferTrayDeviceGap <= 0 ) st_recipe.dLoaderTransferTrayDeviceGap = 100.0;
			Run_Epoxy_Transfer_Robot.Get_Billard_Pos( nSite);

			m_dpTargetPosList[0] = Run_Epoxy_Transfer_Robot.md_TargetAxisXValue[mn_FirstSecond][0];
			m_dpTargetPosList[1] = Run_Epoxy_Transfer_Robot.md_TargetAxisYValue[mn_FirstSecond][0];

			dp_SpdRatio[0] = (double)50;////st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)300;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)300;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 


			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );		
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1000;
			}
			break;

		case 1100:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_BOT_DOWN_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				mn_MoveStep = 1200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1100;
			}
			break;

		case 1200:
			m_dwSatbleWaitTime[1] = GetCurrentTime();
			m_dwSatbleWaitTime[2] = m_dwSatbleWaitTime[1] - m_dwSatbleWaitTime[0];
			if( m_dwSatbleWaitTime[2] <= 0 ) m_dwSatbleWaitTime[0] = GetCurrentTime();
			if( m_dwSatbleWaitTime[2] > st_recipe.dSatbleTime )
			{
				//st_work.nEpoxyBiliardThreadRunMode = dRUN;
				Func.VppmSet();//미리하는데 체크해야한다.
				mn_MoveStep = 1300;
			}
			break;

		case 1300:
			Run_Epoxy_Transfer_Robot.Get_Billard_Pos( nSite);

			m_dpTargetPosList[0] = Run_Epoxy_Transfer_Robot.md_TargetAxisXValue[mn_FirstSecond][1];
			m_dpTargetPosList[1] = Run_Epoxy_Transfer_Robot.md_TargetAxisYValue[mn_FirstSecond][1];

			if(st_basic.n_mode_7387 == 0)
			{
				//dp_PosList[2] = st_recipe.nEpoxyCircleCount;
			}
			else
			{
				//dp_PosList[2] = 10;
			}

			dp_SpdRatio[0] = (double)30;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			
			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );

			if( nRet_1 == BD_GOOD) 
			{
				mn_MoveStep = 1400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1000;
			}
			break;

		case 1400:
			Func.VppmOff();			
			COMI.Set_MotStop(0, m_nRobot_S);
			mn_MoveStep = 1500;
			break;

		case 1500:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_S, -0.5 , st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				//st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				mn_MoveStep = 1600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1500;
			}
			break;

		case 1600:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_BOT_DOWN_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 1700;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1600;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1600;
			}
			break;

		case 1700:
			mn_FirstSecond++;
			if( mn_FirstSecond >= 2)
			{
				mn_FirstSecond = 0;
				mn_dotPos = 0;
				mn_MoveStep = 2000;
			}
			else
			{
				mn_MoveStep = 1000;
			}
			break;

			//design DOTDOTDOTDOT

		case 1900:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{	
				mn_MoveStep = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1900;
			}
			break;

		case 2000:
			Run_Epoxy_Transfer_Robot.Get_Billard_Pos( nSite);

			m_dpTargetPosList[0] = Run_Epoxy_Transfer_Robot.md_TargetDotXValue[mn_dotPos];
			m_dpTargetPosList[1] = Run_Epoxy_Transfer_Robot.md_TargetDotYValue[mn_dotPos];

			dp_SpdRatio[0] = (double)50;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)300;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)300;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );		
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 2000;
			}
			break;

		case 2100:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_BOT_DOWN_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{	
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				mn_MoveStep = 2200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 2100;
			}
			break;

		case 2200:
			m_dwSatbleWaitTime[1] = GetCurrentTime();
			m_dwSatbleWaitTime[2] = m_dwSatbleWaitTime[1] - m_dwSatbleWaitTime[0];
			if( m_dwSatbleWaitTime[2] <= 0 ) m_dwSatbleWaitTime[0] = GetCurrentTime();
			if( m_dwSatbleWaitTime[2] > st_recipe.dSatbleTime )
			{
				Func.VppmSet();
				//st_work.nEpoxyBiliardThreadRunMode = dRUN;
				ml_motflag[0] = CTL_NO;
				ml_motflag[1] = CTL_NO;
				mn_MoveStep = 2300;
			}
			break;

		case 2300:
			//x Axis - md_TargetDotXValue[mn_dotPos];
			//Y- Axis md_TargetDotYValue[mn_dotPos];
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, Run_Epoxy_Transfer_Robot.md_TargetDotXValue[mn_dotPos], 5*COMI.md_allow_value[m_nRobot_X]);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, Run_Epoxy_Transfer_Robot.md_TargetDotYValue[mn_dotPos], 5*COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				COMI.Set_MotStop( 0, m_nRobot_X);
				COMI.Set_MotStop( 0, m_nRobot_Y);
				mn_MoveStep = 1900;
				break;
			}

			Run_Epoxy_Transfer_Robot.Get_Billard_Pos( nSite);
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_X, Run_Epoxy_Transfer_Robot.md_TargetDotXValue[mn_dotPos], st_recipe.nEpoxyRunSpeed);
			nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_S, st_recipe.nEpoxyDotScrewCount, st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD ) ml_motflag[0] = CTL_YES;
			if( nRet_2 == BD_GOOD ) ml_motflag[1] = CTL_YES;
			if( ml_motflag[0] == CTL_YES && ml_motflag[1] == CTL_YES )
			{
				ml_motflag[0] = CTL_NO;
				ml_motflag[1] = CTL_NO;		
				mn_MoveStep = 2310;
			}
			else if( nRet_1 == BD_RETRY || nRet_2 == BD_RETRY )
			{
				mn_MoveStep = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 2300;
			}
			break;

		case 2310:
			Run_Epoxy_Transfer_Robot.Get_Billard_Pos( nSite);
			if( mn_dotPos < 2) nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_X, Run_Epoxy_Transfer_Robot.md_TargetDotXValue[mn_dotPos], st_recipe.nEpoxyRunSpeed);
			else                     nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_Y, Run_Epoxy_Transfer_Robot.md_TargetDotYValue[mn_dotPos] - st_recipe.dEpoxyYOffSet , st_recipe.nEpoxyRunSpeed);
			nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_S, st_recipe.nEpoxyDotScrewCount, st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD ) ml_motflag[0] = CTL_YES;
			if( nRet_2 == BD_GOOD ) ml_motflag[1] = CTL_YES;
			if( ml_motflag[0] == CTL_YES && ml_motflag[1] == CTL_YES )
			{
				Func.VppmOff();
				ml_motflag[0] = CTL_NO;
				ml_motflag[1] = CTL_NO;	
				COMI.Set_MotStop(0, m_nRobot_S);	
				mn_MoveStep = 2400;
			}
			else if( nRet_1 == BD_RETRY || nRet_2 == BD_RETRY )
			{
				mn_MoveStep = 2310;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 2310;
			}
			break;

		case 2400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_S, -0.5 , st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				//st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				mn_MoveStep = 2500;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 2400;
			}
			break;

		case 2500://Zaxis do a littlebit up
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_BOT_DOWN_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 2600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 2500;
			}
			break;

		case 2600:
			mn_dotPos++;
			if( mn_dotPos < 4 )
			{
				mn_MoveStep = 2000;
			}
			else
			{
				mn_dotPos = 0;
				mn_MoveStep = 3000;
			}
			break;

		case 3000:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				FuncRet = RET_GOOD;
				mn_MoveStep = 0;
			}	
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				mn_MoveStep = 200;
			}
			break;
		}

		//m_nBillardWorking = CTL_YES;//작업시작	

		return FuncRet;
}

//mn_move_carrier : 1 무한 Epoxy 작업
int CDialog_Manual_Move::Move_EpoxyTransfer()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = M_EPOXY_TRANSFER_X;
	int m_nRobot_Y = M_EPOXY_TRANSFER_Y;
	int m_nRobot_Z = M_EPOXY_TRANSFER_Z;
	
	switch( m_move_step[m_nAxisNum] )
	{
	case 0:
		m_move_step[m_nAxisNum] = 10;
		break;

	case 10:		
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			Func.VppmOff();
			m_move_step[m_nAxisNum] = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_move_step[m_nAxisNum] = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
			m_move_step[m_nAxisNum] = 0;
		}
		break;

	case 100:
		nRet_1 = COMI.Check_MotPosRange(m_nAxisNum, st_motor[m_nAxisNum].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nAxisNum] );
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.md_allow_value[m_nRobot_Y] );
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			m_move_step[m_nAxisNum] = 200;
		}
		else
		{
			Func.VppmOff();
			m_move_step[m_nAxisNum] = 1000;
		}
		break;

	case 200:
		nRet_1 = Robot_Move_Safety_Zone( 0, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			m_move_step[m_nAxisNum] = 100;
		}
		break;

	case 1000:
		m_move_step[m_nAxisNum] = 1100;
		break;

	case 1100:

		if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_REQ && 
			nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] == WORK_PLACE )
		{
			nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_READY;
			m_move_step[m_nAxisNum] = 1200;
		}
		else if( mn_move_carrier == 1)//무한 동작
		{
			m_move_step[m_nAxisNum] = 2000;
			mn_BufferPos = 0;
		}
		break;

	case 1200:

		if( nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_CHANGE)
		{
			m_move_step[m_nAxisNum] = 2000;
			mn_BufferPos = 0;
		}
		break;

	case 2000:
// 		if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_idbuffer[0] == CTL_YES )
// 		{
// 			if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[0] == BIN_CDIMM )
// 			{
// 				mn_BufferPos = 0;
// 				m_move_step[m_nAxisNum] = 2100;
// 			}
// 		}
// 		else if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_idbuffer[1] == CTL_YES )
// 		{
// 			if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[1] == BIN_CDIMM )
// 			{
// 				mn_BufferPos = 1;
// 				m_move_step[m_nAxisNum] = 2100;
// 			}
// 		}		
// 		else if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_idbuffer[2] == CTL_YES )
// 		{
// 			if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[2] == BIN_CDIMM )
// 			{
// 				mn_BufferPos = 2;
// 				m_move_step[m_nAxisNum] = 2100;
// 			}
// 		}

		m_move_step[m_nAxisNum] = 2100;
		//else
		//{
		//	m_move_step[m_nAxisNum] = 4000;
		//}
		break;

	case 2100:
		nRet_1 = Robot_Move_Safety_Zone( 0, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			mn_MoveStep = 0;
			m_move_step[m_nAxisNum] = 2200;
			ml_motflag[0] = CTL_NO;
			ml_motflag[1] = CTL_NO;
		}
		break;

	case 2200:
		nRet_1 = Move_Billiard_Epoxy( 0, mn_BufferPos);
		if( nRet_1 == RET_GOOD )
		{
			//st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[mn_BufferPos] = BIN_EPOXY;
			mn_BufferPos++;
			if( mn_BufferPos >= 3 )//단순 테스트 ->  3
			{
				mn_BufferPos = 0;
				m_move_step[m_nAxisNum] = 4000;
			}
			else
			{
				m_move_step[m_nAxisNum] = 2000;
			}
		}
		else if( nRet_1 == RET_ERROR)
		{
		}
		break;

	case 4000:
		nRet_1 = Robot_Move_Safety_Zone( 1, 0, 0 );//nMode == 1  ==> 체크 필요 2017.0407작업 완료후 안전위치로 이동
		if( nRet_1 == RET_GOOD )
		{
			nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_NO;
			nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = CTL_NO;
			m_move_step[m_nAxisNum] = 4100;
		}
		break;

	case 4100:
// 		if( mn_move_carrier == 1)//무한 반복
// 		{
			m_move_step[m_nAxisNum] = 0;
// 		}
		break;

		
		
	}
	return nFunRet;
}


int CDialog_Manual_Move::Move_HeatSinkTransfer()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = M_HEATSINK_TRANSFER_X;
	int m_nRobot_X = M_HEATSINK_TRANSFER_X;
	int m_nRobot_Y = M_HEATSINK_TRANSFER_Y;
	int m_nRobot_Z = M_HEATSINK_TRANSFER_Z;
	int m_nRobot_P = M_HEATSINK_PICKER_PITCH;
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 10;
			break;

		case 10:		
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 100:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_P, st_motor[m_nRobot_P].md_pos[P_HEATSINK_PICKER_PITCH_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 100;
			}
			break;

		case 200:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INIT_POS], COMI.md_allow_value[m_nRobot_X]);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 300:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 1000:
// 			m_nlast_pick = CTL_NO;
// 			if( st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_NO || 
// 				st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_NO )
// 			{
// 				m_nlast_pick = CTL_READY;
// 			}
// 			else
// 			{
// 				m_nlast_pick = CTL_YES;//또는 lot이 다 됐다
// 			}
				//load plate에 자재 요청
			if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_REQ && 
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE)
			{
				m_move_step[m_nAxisNum] = 1100;
			}
			break;

			//미리 준비하기 위한 기본 준비는 무엇인가?

			//1. HEATSINK PICKUP
			//2. PLACE DISPENSOY slot  -> Rotator
			//3. Place dispensor slot one more.
			//4. Pickup heatsink that is on liquid
			//5. Inspection at UV Lamp vision
			//6. Carrier 대기
		case 1100:
			//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
// 				if( m_nlast_pick == CTL_YES ) m_move_step[m_nAxisNum] = 2000;
				m_move_step[m_nAxisNum] = 1200;
			}
			break;

		case 1200:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Pickup( 0, THD_LD_HEATSINK_BUFF );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			break;

		case 2000:
			//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			break;

		case 2100:
			if( g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON) == IO_ON && 
				g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF) == IO_OFF )
			{
				nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Place( 0, THD_PLACE_HEATSINK_DVC, 0 );
				if( nRet_1 == RET_GOOD )
				{
					m_move_step[m_nAxisNum] = 2200;
				}
				else if( nRet_1 == RET_PICKER_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 2100;
				}
				else if( nRet_1 == RET_TRAY_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 2100;
				}							
			}
			else
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Reverse_0_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
			}
			break;

		case 2200:
			//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				st_sync.nDisPensorFlag = CTL_YES;
// 				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
// 				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;

				m_move_step[m_nAxisNum] = 2300;
			}
			break;

		case 2300:
			if( st_sync.nDisPensorFlag == CTL_YES )
			{
				if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_CHANGE && 
					nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE)
				{
					m_move_step[m_nAxisNum] = 3000;
					break;
				}
			}
			if( st_sync.nDisPensorFlag == CTL_YES )
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 3000:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Pickup( 0, THD_DISPENSOR_PRBT );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 3110;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 3000;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 3000;
			}
			break;

		case 3110:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_NO;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = CTL_NO;
				m_move_step[m_nAxisNum] = 3120;
			}
			break;

		case 3120:
			if( st_sync.nDisPensorFlag == CTL_YES )
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
			}
			m_move_step[m_nAxisNum] = 3200;
			break;

		case 3200:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Measure_Vision();
			if( nRet_1 == RET_GOOD )
			{
// 				g_lotMgr.GetLotAt(m_nFindLotNo_Flag).AddHeatSinkInputCnt(PRIME);//히트싱크 픽업증가
				m_move_step[m_nAxisNum] = 4000;
			}
			else if(nRet_1 == RET_ERROR)//hestsink를 버렸다 다시 집는다.
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 4000://move carrier safety
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 3, 0 );
			if( nRet_1 == RET_GOOD )
			{
				//m_nCarriorPos = 0;
				m_move_step[m_nAxisNum] = 4100;
			}
			break;
			
		case 4100:
 			if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_REQ && 
 				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] == WORK_PLACE)
 			{
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_READY;
				m_move_step[m_nAxisNum] = 4200;
 			}
			else if( m_nCarriorPos> 0 && nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_CHANGE && 
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] == WORK_PLACE)
			{
				m_move_step[m_nAxisNum] = 4200;
			}
			break;

		case 4110:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 3, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 4200;
			}
			break;

		case 4200:
			if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_CHANGE && 
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] == WORK_PLACE)
			{
				m_move_step[m_nAxisNum] = 4210;
			}
			//임시
// 			m_move_step[m_nAxisNum] = 4210;
			break;

		case 4210:
			if( ( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + COMI.md_allow_value[m_nRobot_X] ) ) &&
				( COMI.Get_MotCurrentPos(m_nRobot_Y) > ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] - COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Place(0, THD_PLACE_CARRIER_DVC, m_nCarriorPos);
				if( nRet_1 == RET_GOOD )
				{
					m_move_step[m_nAxisNum] = 4300;
				}
				else if( nRet_1 == RET_PICKER_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 4200;
				}
				else if( nRet_1 == RET_TRAY_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 4200;
				}
			}
			else
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			break;

		case 4300:
			m_nCarriorPos++;
			if( m_nCarriorPos >= 3 )
			{
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_NO;
 				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] = CTL_NO;
				m_nCarriorPos = 0;
				m_move_step[m_nAxisNum] = 0;
			}
			else
			{
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 4900:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 5000;
			}
			break;

		case 5000:
			if( COMI.Get_MotCurrentPos(m_nRobot_Y) < ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + COMI.md_allow_value[m_nRobot_X] ) )
			{
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_NO;
				
				nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_REQ;
				nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] = WORK_PLACE;
				m_move_step[m_nAxisNum] = 1000;
			}
			else
			{
				m_move_step[m_nAxisNum] = 4900;
			}
			break;
	}
	return nFunRet;
}

// int CDialog_Manual_Move::Process_Measure_Vision()
// {
// 	int nRet_1 = 0;
// 	int nFunRet = RET_PROCEED, nRet_2 = 0;
// 	int m_nAxisNum = M_HEATSINK_TRANSFER_X;
// 	int m_nRobot_X = M_HEATSINK_TRANSFER_X;
// 	int m_nRobot_Y = M_HEATSINK_TRANSFER_Y;
// 	int m_nRobot_Z = M_HEATSINK_TRANSFER_Z;
// 	int m_nRobot_P = M_HEATSINK_PICKER_PITCH;
// 	
// 	switch( m_move_step[m_nAxisNum] )
// 	{
// 		case  0:
// 			m_nContinusVError = 0;
// 			mn_MoveMeasureStep = 1000;
// 			break;
// 
// 		case 1000:
// 			//nMode == 2 => Dispensor로 자재를 Pick할 수 있는 위치로 이동
// 			nRet_1 = Robot_AutoMove_Safety_Zone( 1, 0 );
// 			if( nRet_1 == RET_GOOD )
// 			{
// 				mn_MoveMeasureStep = 1100;
// 			}
// 			break;
// 
// 		case 1100:
// 			if( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
// 			{
// 
// 				m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INSPECT_POS];
// 				m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INSPECT_POS];
// 				
// 				m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
// 				m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
// 				m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
// 				
// 				nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio);
// 				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
// 				{
// 					mn_MoveMeasureStep = 1200;
// 					
// 				}
// 				else if (nRet_1 == BD_RETRY)
// 				{
// 					mn_MoveMeasureStep = 1100;
// 				}
// 				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 				{
// 					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
// 					mn_MoveMeasureStep = 1100;
// 				}
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 1000;
// 			}
// 			break;
// 
// 		case 1200:
// 			if( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
// 			{
// 				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INSPECT_POS], COMI.mn_runspeed_rate);
// 				if (nRet_1 == BD_GOOD) //좌측으로 이동
// 				{
// 					mn_MoveMeasureStep = 2000;
// 				}
// 				else if (nRet_1 == BD_RETRY)
// 				{
// 					mn_MoveMeasureStep = 1200;
// 				}
// 				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 					CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
// 					mn_MoveMeasureStep = 1200;
// 				}
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 1000;
// 			}
// 			break;
// 
// 		case 2000:
// 			if( st_basic.n_mode_7387 == CTL_YES )
// 			{
// 				st_vision.b7387MeasureCmp = false;
// 				st_vision.n7387MeasureResultFlag = -1;
// 				
// 				Func.Send7387MeasureStart();
// 				m_dwMCameraResWaitTime[0] = GetCurrentTime();
// 				mn_MoveMeasureStep = 2100;
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 2200;
// 			}
// 			break;
// 
// 		case 2100:
// 			m_dwMCameraResWaitTime[1] = GetCurrentTime();
// 			m_dwMCameraResWaitTime[2] = m_dwMCameraResWaitTime[1] - m_dwMCameraResWaitTime[0];
// 			if( m_dwMCameraResWaitTime[2] <= 0 ) m_dwMCameraResWaitTime[0] = GetCurrentTime();
// 
// 			if( st_vision.b7387MeasureCmp == TRUE )
// 			{
// 				mn_MoveMeasureStep = 2200;
// 			}
// 			else
// 			{
// 				if( m_dwMCameraResWaitTime[2] > 10000 )
// 				{//941000 1 A "MEASURE_VISION_IS_NOT_RESPONSE."
// 					CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, "941000");
// 				}
// 			}
// 			break;
// 
// 		case 2200:
// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
// 			if (nRet_1 == BD_GOOD) //좌측으로 이동
// 			{
// 				mn_MoveMeasureStep = 3000;
// 			}
// 			else if (nRet_1 == BD_RETRY)
// 			{
// 				mn_MoveMeasureStep = 2200;
// 			}
// 			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
// 				mn_MoveMeasureStep = 2200;
// 			}
// 			break;
// 
// 		case 3000:
// 			if( st_basic.n_mode_7387 == CTL_YES )
// 			{
// 
// 				if(st_vision.n7387MeasureResultFlag == CTL_NO || st_basic.n_7387InspectAlarmSkipMode == CTL_YES )
// 				{
// 					st_vision.n7387MeasureAlarmCnt = 0;
// 					mn_MoveMeasureStep = 4000;
// 				}
// 				else if(st_vision.n7387MeasureResultFlag > 0 && st_basic.n_7387InspectAlarmSkipMode == 0)
// 				{	
// 					st_vision.n7387MeasureAlarmCnt++;
// 					st_basic.n7387AlarmCount++;
// 					if(m_nContinusVError > st_basic.n7387AlarmCount)
// 					{
// 						m_nContinusVError = 0;
// 						//941001 1 A "VISION_MEASURE_7387_ERROR_HEATSINK_TRANSFER.";						
// 						CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, "941001");
// 
// 					}
// 					else if(m_nTotalVError > st_vision.n7387MeasureAlarmCnt)
// 					{
// 						m_nTotalVError = 0;
// 						sprintf(Jamcode, "110037");//20130825
// 						
// 						//941001 1 A "VISION_MEASURE_7387_ERROR_HEATSINK_TRANSFER.";						
// 						CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, "941001");
// 					}
// 					else
// 					{
// 						//HeatSink 버리러 가기
// 						mn_MoveMeasureStep = 3100;
// 						
// 					}
// 				}
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 4000;
// 			}
// 			break;
// 
// 		case 3010:
// 			//nMode == 2 => Dispensor로 자재를 Pick할 수 있는 위치로 이동
// 			nRet_1 = Robot_AutoMove_Safety_Zone( 1, 0 );
// 			if( nRet_1 == RET_GOOD )
// 			{
// 				mn_MoveMeasureStep = 3100;
// 			}
// 			break;
// 
// 		case 3100:
// 			if( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
// 			{
// 				
// 				m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INSPECT_GABAGE_POS];
// 				m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INSPECT_GABAGE_POS];
// 				
// 				m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
// 				m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
// 				m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
// 				
// 				nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio);
// 				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
// 				{
// 					mn_MoveMeasureStep = 3200;
// 					
// 				}
// 				else if (nRet_1 == BD_RETRY)
// 				{
// 					mn_MoveMeasureStep = 3100;
// 				}
// 				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 				{
// 					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
// 					mn_MoveMeasureStep = 3100;
// 				}
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 3010;
// 			}
// 			break;
// 
// 		case 3200:
// 			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INSPECT_GABAGE_POS], st_motor[m_nRobot_X].mn_allow);
// 			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INSPECT_GABAGE_POS], st_motor[m_nRobot_Y].mn_allow);
// 			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
// 			{
// 				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INSPECT_GABAGE_POS], COMI.mn_runspeed_rate);
// 				if (nRet_1 == BD_GOOD) //좌측으로 이동
// 				{
// 					mn_MoveMeasureStep = 3300;
// 				}
// 				else if (nRet_1 == BD_RETRY)
// 				{
// 					mn_MoveMeasureStep = 3200;
// 				}
// 				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 					CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
// 					mn_MoveMeasureStep = 3200;
// 				}
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 3010;
// 			}
// 			break;
// 
// 		case 3300:
// 			//COMM
// 			//피치를 벌린다
// 			mn_MoveMeasureStep = 3400;
// 			break;
// 
// 		case 3400:
// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
// 			if (nRet_1 == BD_GOOD) //좌측으로 이동
// 			{
// 				mn_MoveMeasureStep = 3500;
// 			}
// 			else if (nRet_1 == BD_RETRY)
// 			{
// 				mn_MoveMeasureStep = 3400;
// 			}
// 			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
// 				mn_MoveMeasureStep = 3400;
// 			}
// 			break;
// 
// 		case 3500://check GABAGE sensor
// 			nRet_1 = g_ioMgr.get_in_bit(st_io.i_HeatSink_Garbage_Full_Chk_1, IO_ON);
// 			nRet_2 = g_ioMgr.get_in_bit(st_io.i_HeatSink_Garbage_Full_Chk_2, IO_ON);
// 			if( nRet_1 == IO_OFF || nRet_2 == IO_OFF )
// 			{
// 				strAlarm.Empty();
// 				if( nRet_1 == IO_OFF ) strAlarm.Format("8%d%04d",IO_OFF,st_io.i_HeatSink_Garbage_Full_Chk_1);
// 				else				   strAlarm.Format("8%d%04d",IO_OFF,st_io.i_HeatSink_Garbage_Full_Chk_2);
// 				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, strAlarm);
// 			}
// 			else
// 			{
// 				mn_MoveMeasureStep = 3600;
// 			}
// 			break;
// 
// 		case 3600:
// 			nFuncRet = RET_ERROR;
// 			mn_MoveMeasureStep = 0;
// 			break;
// 
// 		case 4000:
// 			nFuncRet = RET_GOOD;
// 			mn_MoveMeasureStep = 0;
// 			break;
// 	}
// 	return nFuncRet;
// }


int CDialog_Manual_Move::Move_VisionTransfer()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = M_HEATSINK_INSPECT_Y;
	int m_nRobot_VisY = M_HEATSINK_INSPECT_Y;
	int m_nRobot_VisZ = M_HEATSINK_INSPECT_Z;
	long lMotionDone=0;
	bool bSkip = false;
	
	if( m_nvisionstop == 1 )
		return nFunRet;

	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisZ, st_motor[m_nRobot_VisZ].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 0;
			}
			break;

		case 100:
			Run_HeatSinkVision_Transfer_Robot.Set_Device_Carrier_Camera_Y_Press_UpDown(IO_OFF);
			m_move_step[m_nAxisNum] = 110;
			break;

		case 110:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_Device_Carrier_Camera_Y_Press_UpDown( IO_OFF);
			if( nRet_1 == IO_ON )
			{
				m_move_step[m_nAxisNum] = 200;
			}
			break;

		case 200:
			Run_HeatSinkVision_Transfer_Robot.Set_Device_CameraY_Jig_Press_ForwardBackward(IO_OFF);
			m_move_step[m_nAxisNum] = 210;
			break;

		case 210:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_Device_CameraY_Jig_Press_ForwardBackward( IO_OFF);
			if( nRet_1 == IO_ON )
			{
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 1000:
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
				m_move_step[m_nAxisNum] = 1100;
// 			}
			break;

		case 1100:
			if( nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_REQ && 
				nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][1] == WORK_PLACE )
			{
				nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_READY;
				m_move_step[m_nAxisNum] = 1200;
			}
			if( m_hs_vision >= 0 )
				m_move_step[m_nAxisNum] = 1200;
				
			break;

		case 1200:
			if( nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_CHANGE && 
				nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][1] == WORK_PLACE )
			{
				m_nVisCarriorPos = 0;
				m_nVisTeachPos = 0;
				m_move_step[m_nAxisNum] = 2000;
			}
			if( m_hs_vision >= 0 )
			{
				m_nVisCarriorPos = 0;
				m_nVisTeachPos = 0;
				m_move_step[m_nAxisNum] = 2000;
			}

			break;

		case 1800:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisZ, st_motor[m_nRobot_VisZ].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 1800;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1800;
			}
			break;

		case 1900:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 1900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1900;
			}
			break;


		case 2000:
			if( COMI.Get_MotCurrentPos(m_nRobot_VisZ) > (st_motor[m_nRobot_VisZ].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_VisZ]) )
			{
				m_move_step[m_nAxisNum] = 1800;
				break;
			}
			if( COMI.Get_MotCurrentPos(m_nRobot_VisY) > (st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_VisY]) )
			{
				m_move_step[m_nAxisNum] = 1900;
				break;
			}

			if(st_basic.n_3874InspectMode == 0)
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Device_Carrier_Camera_LED_LAMP_OnOff(IO_ON);
				Run_HeatSinkVision_Transfer_Robot.Set_Device_Carrier_Camera_Y_Press_UpDown(IO_ON);
			}
			m_move_step[m_nAxisNum] = 2100;
			break;

		case 2100:			
			if(st_basic.n_3874InspectMode == 0)
			{
				nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_Device_Carrier_Camera_Y_Press_UpDown(IO_ON);
				if( nRet_1 == RET_GOOD )
				{

				}
				else if( nRet_1 == RET_ERROR )
				{
					CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, m_strAlarmCode);
				}
				else
				{
					break;
				}				
			}
			m_move_step[m_nAxisNum] = 2110;
			break;

		case 2110:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_VISION_TOP_POS + m_nVisCarriorPos], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2110;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2110;
			}
			break;

		case 2200:
			st_vision.bHeatsinkMeasureCmp = FALSE;
			st_vision.bEpoxyMeasureCmp = FALSE;
			st_vision.nEpoxyMeasureResultFlag = -1;
			st_vision.nHeatSinkMeasureResultFlag = -1;
			Func.SendEpoxyMeasureStart();
			m_move_step[m_nAxisNum] = 2300;
			break;

		case 2300:
			if( st_vision.bEpoxyMeasureCmp == TRUE )
			{
				m_move_step[m_nAxisNum] = 2310;
			}
			break;

		case 2310:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2310;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2310;
			}
			break;

		case 2400:
			Run_HeatSinkVision_Transfer_Robot.Set_Device_Carrier_Camera_LED_LAMP_OnOff(IO_OFF);
			m_move_step[m_nAxisNum] = 2500;
			break;

		case 2500:
			st_vision.bHeatsinkMeasureCmp = FALSE;
			st_vision.bEpoxyMeasureCmp = FALSE;
			st_vision.nEpoxyMeasureResultFlag = -1;
			st_vision.nHeatSinkMeasureResultFlag = -1;
			Func.SendHeatsinkMeasureStart(m_nVisTeachPos);
			m_move_step[m_nAxisNum] = 2600;
			break;

		case 2600:
			if( bHeatsinkMeasureCmp == TRUE )
			{
				m_move_step[m_nAxisNum] = 2700;
			}
			break;

		case 2700:





			Func.SendHeatsinkMeasureStart(m_nVisTeachPos);
			m_move_step[m_nAxisNum] = 2800;
			break;

		case 2800:
			break;

		case 2900:
			break;

		case 5000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_PRESS_START_POS] + (m_nVisCarriorPos*1), COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 5100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 5000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 5000;
			}
			break;

		case 5100:
			Run_HeatSinkVision_Transfer_Robot.Set_Device_Carrier_Camera_Y_Press_UpDown(IO_OFF);
			break;


		case 6000:
			m_nVisCarriorPos++;
			if( m_nVisCarriorPos >= 3)
			{

			}
			else
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			break;


	}

	return nFunRet;
}
int CDialog_Manual_Move::Move_BuffDispensor()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0, nRet_3 = 0;
	int m_nAxisNum = 98;
	int m_nRobot_DisY = M_DISPENSER_Y;
	CString strAlarm = _T("");
	
	double dCurrHeatSinkX, dCurrHeatSinkY;
	
	dCurrHeatSinkX = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_X );
	dCurrHeatSinkY = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y );
	
	if(	m_nhsexystop == 1 )
		return nFunRet;

	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 1000;
			break;

		case 1000:
			if(m_hs_rub != 2)
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_REQ;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
			}
			m_move_step[m_nAxisNum] = 1100;

			break;
			
		case 1100:
			if( m_hs_rub == 2 ||
				( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_READY && nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE ) )
			{
				Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_UpDown(OFF);
				Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
				m_move_step[m_nAxisNum] = 1200;
			}
			break;

		case 1200:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_UpDown(OFF);
			nRet_2 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2000:
			if(	g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Up_Check, IO_OFF) == IO_OFF && 
				g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_On_Check, IO_OFF) == IO_OFF )
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			else
			{
				m_move_step[m_nAxisNum] = 1100;
			}
			break;

		case 2100:
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_UpDown(ON);
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			m_move_step[m_nAxisNum] = 2110;
			break;

		case 2110:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_UpDown(ON);
			nRet_2 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2120;
			}
			break;

		case 2120:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_ON);
			if( st_basic.n_mode_device == WITHOUT_DVC) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON )
			{
				if( ( dCurrHeatSinkX > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[M_HEATSINK_TRANSFER_X].mn_allow) ) &&
					( dCurrHeatSinkY < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - st_motor[M_HEATSINK_TRANSFER_Y].mn_allow) ) )
				{
					strAlarm.Format("%02d0008", m_nRobot_DisY );
					CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
					break;
				}

				Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_UpDown(ON);
				Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
				Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Turn(ON);
				m_move_step[m_nAxisNum] = 2130;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Device_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
			}
			break;

		case 2130:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_UpDown(ON);
			nRet_2 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			nRet_3 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Turn(ON);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD  && nRet_3 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2200;
			}
			break;

		case 2200:
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_UpDown(OFF);
			m_move_step[m_nAxisNum] = 2210;
			break;

		case 2210:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_UpDown(OFF);
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2220;
			}
			break;

		case 2220:
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Clamp_ForwardBackward(ON);
			m_move_step[m_nAxisNum] = 2230;
			break;

		case 2230:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Clamp_ForwardBackward(ON);
			if( 1 || nRet_1 == RET_GOOD )//check
			{
				m_move_step[m_nAxisNum] = 2240;
			}
			break;

		case 2240:
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_UpDown(ON);
			m_move_step[m_nAxisNum] = 2250;
			break;

		case 2250:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_UpDown(ON);
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2260;
			}
			break;

		case 2260:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_OFF);//B접점
			if( nRet_1 == IO_ON )
			{
				m_npSet_WorkPosYXCPB[0] = 0;
				m_npSet_WorkPosYXCPB[1] = 0;
				m_npSet_WorkPosYXCPB[3] = 0;

				if(m_hs_rub != 2)
					Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, THD_DISPENSOR_PRBT, THD_PLACE_HEATSINK_DVC, m_npSet_WorkPosYXCPB);
				Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Turn(OFF);
				m_move_step[m_nAxisNum] = 2300;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_OFF, st_io.i_Dispenser_Device_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
			}
			break;

		case 2300:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Turn(OFF);
			if( nRet_1 ==  RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2310;
			}
			break;

			// down to help heatsink robot picks up DVC
		case 2310:
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_UpDown(OFF);
			m_move_step[m_nAxisNum] = 2320;
			break;

		case 2320:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_UpDown(OFF);
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2400;
			}
			break;

		case 2400:
			st_sync.nDisPensorFlag = CTL_NO;
			if(m_hs_rub != 2)
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_CHANGE; 
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
			}
			m_move_step[m_nAxisNum] = 2500;
			break;

		case 2500://HEATSINK ROBOT pick dvc in reverse site
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_SORT && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE ) 
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_SORT; 
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;

				m_move_step[m_nAxisNum] = 2600;
			}
			else if( m_hs_rub == 2 ) m_move_step[m_nAxisNum] = 2600;
			break;
			
		case 2600:
			Run_HeatSinkVision_Transfer_Robot.Set_HeatSink_Reverse_Clamp_ForwardBackward(IO_OFF);
			m_move_step[m_nAxisNum] = 2610;
			break;
			
		case 2610:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Chk_HeatSink_Reverse_Clamp_ForwardBackward(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2700;
			}
			break;

		case 2700:
			if( m_hs_rub != 2 )
			{
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_FREE;
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_FREE;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
			}
			m_move_step[m_nAxisNum] = 2800;
			break;

		case 2800:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_FINAL && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE )
			{
				m_move_step[m_nAxisNum] = 2900;
			}
			if( m_hs_rub == 2 ) m_move_step[m_nAxisNum] = 2900;
			break;

		case 2900:
			if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_NO && 
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == CTL_NO )
			{
				m_move_step[m_nAxisNum] = 5000;
			}
			if( m_hs_rub == 2 ) m_move_step[m_nAxisNum] = 5000;
			break;

		case 5000:
			m_move_step[m_nAxisNum] = 0;
			if( m_hs_rub == 2 )
				nFunRet = RET_GOOD;
			break;
	}

	return nFunRet;
}

int CDialog_Manual_Move::Move_Dispensor()
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0, nRet_3 = 0;
	int m_nAxisNum = M_DISPENSER_Y;
	int m_nRobot_DisY = M_DISPENSER_Y;
	CString strAlarm = _T("");
	
	double dCurrDisY, dCurrHeatSinkX, dCurrHeatSinkY;
	
	dCurrHeatSinkX = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_X );
	dCurrHeatSinkY = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y );


	if( m_nhsexystop == 1 )
	{
		return nFunRet;
	}

	switch( m_move_step[m_nAxisNum] )
	{
	case 0:
		m_move_step[m_nAxisNum] = 5;
		break;

		case 5:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 10;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 0;
				m_nhsexystop = 1;
			}
			break;

		case 10:
			dCurrDisY = COMI.Get_MotCurrentPos(M_PRESS_Y);
			if( dCurrDisY > st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS] + st_motor[m_nRobot_DisY].mn_allow )
			{
				m_move_step[m_nAxisNum] = 0;
			}
			else
			{
				m_move_step[m_nAxisNum] = 100;
			}
			break;			

		case 100:

			if( m_hs_rub == 1 )//only dispensor 동작만 할 경우
			{
				m_move_step[m_nAxisNum] = 1000;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_REQ;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
			}
			else
			{
				m_move_step[m_nAxisNum] = 1000;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_REQ;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
			}
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{

// 			}
			break;

		case 1000://Picker ask disponsor if picker could place device
			//dispensor가 요청하면 heatsink는 투입한다.
			if( COMI.Get_MotCurrentPos(M_DISPENSER_Y) > st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_DisY] )
			{
				m_move_step[m_nAxisNum] = 10;
			}
			else
			{

				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_ON);//B접점
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF);
				if( nRet_1 != IO_ON ||  nRet_2 != IO_ON || nRet_3 != IO_OFF)
				{
					if( nRet_1 != IO_OFF) strAlarm.Format("8%d%04d", IO_OFF, st_io.i_Dispenser_Device_Check);
					else if( nRet_2 != IO_ON ) strAlarm.Format("8%d%04d", IO_ON, st_io.i_HeatSink_Reverse_0_Check);
					else								strAlarm.Format("8%d%04d", IO_OFF, st_io.i_HeatSink_Reverse_180_Check);						
					CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
				}
				else
				{
					m_move_step[m_nAxisNum] = 1100;
				}

			}
			break;

		case 1100:
			if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_READY && //PLACE complete!!
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{
				m_move_step[m_nAxisNum] = 1110;
			}
			else if( m_hs_rub == 1 )//only heatsink
			{
				m_move_step[m_nAxisNum] = 1110;
			}
			break;

		case 1110:
			if( ( dCurrHeatSinkX > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[M_HEATSINK_TRANSFER_X].mn_allow) ) &&
				( dCurrHeatSinkY < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - st_motor[M_HEATSINK_TRANSFER_Y].mn_allow) ) )
			{
				strAlarm.Format("%02d0008", m_nRobot_DisY );
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
				m_nhsexystop = 1;
				break;
			}


			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Color_Check, IO_ON);
			if( st_basic.n_mode_7387 == CTL_NO) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Color_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
				m_nhsexystop = 1;
			}
			break;

		case 1200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_GABAGE_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 1300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1200;
				m_nhsexystop = 1;
			}
			break;

		case 1300:
			Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_ON);
			m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			m_move_step[m_nAxisNum] = 1310;
			break;

		case 1310:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_OFF);
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_HEATSINK_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_ON);
				m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2000;
				m_nhsexystop = 1;
			}
			break;

		case 2100:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_OFF);
				m_move_step[m_nAxisNum] = 2200;
			}
			break;

		case 2200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_DISPENSING_END_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_ON);
				m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 2300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2200;
				m_nhsexystop = 1;
			}
			break;

		case 2300:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_OFF);
				m_move_step[m_nAxisNum] = 3000;
			}
			break;

		case 3000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_DISPENSING_END_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_ON);
				m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
				m_move_step[m_nAxisNum] = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3000;
				m_nhsexystop = 1;
			}
			break;

		case 3100:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Run_HeatSinkVision_Transfer_Robot.Set_Dispenser_Air_Blow(IO_OFF);
				m_move_step[m_nAxisNum] = 3200;
			}
			break;

		case 3200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_LIQUID_CHECK_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 3300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3200;
				m_nhsexystop = 1;
			}
			break;

		case 3300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 3400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 3300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 3300;
				m_nhsexystop = 1;
			}
			break;

		case 3400:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Color_Check, IO_ON);
			if( st_basic.n_mode_7387 == CTL_NO) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON)
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Color_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
				m_nhsexystop = 1;
			}
			break;


		case 4000:
			if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_REQ && nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE)
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_READY;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
				m_move_step[m_nAxisNum] = 4100;
			}
			if( m_hs_rub == 1 )
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_READY;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
				m_move_step[m_nAxisNum] = 4100;
			}
			break;

		case 4100:
			if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_CHANGE && nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE)
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_NO;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = CTL_NO;
				m_move_step[m_nAxisNum] = 0;
			}
			if( m_hs_rub == 1 )
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_NO;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = CTL_NO;
				m_move_step[m_nAxisNum] = 0;
				m_hs_rub = -1;
				nFunRet = RET_GOOD;
			}

			break;
	}
	return nFunRet;
}


int CDialog_Manual_Move::Move_DvcBuffer()
{
	int nRet_1 = 0, i = 0, nCount = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0, nRet_3 = 0;
	int m_nAxisNum = 95;
	
	switch( m_move_step[m_nAxisNum] )
	{

		case 0:
	// 		if( g_lotMgr.GetLotCount() > 0 )
	// 		{
				m_move_step[m_nAxisNum] = 100;
	// 		}		
			break;

		case 100:
			nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CLEAR; //clear

			m_move_step[m_nAxisNum] = 200;
			break;

		case 200:
			m_move_step[m_nAxisNum] = 300;	
			break;

		case 300:
			for( i = 0; i < st_recipe.nLdBuffer_Num; i++ )
			{
				m_npBuff_Info[i] = CTL_YES;			 
			}
			Run_LdPicker.Set_Loader_Buffer_Align_OnOff(IO_OFF);
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo = CTL_NO;
				m_npBuff_Info[i] = CTL_NO;			 
			}
			m_move_step[m_nAxisNum] = 400;
			break;

		case 400:
			nRet_1 = Run_LdPicker.Chk_Loader_Buffer_Align_OnOff( IO_OFF );

			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 500;
			}
			else if(nRet_1 == RET_ERROR)
			{			 
				CTL_Lib.Alarm_Error_Occurrence(3520, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 500:
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
// 				if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
// 				{
					//load plate에 자재 요청
					nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
					nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;
// 					m_nFindLotNo_Flag = 0;
// 				}
// 				else if( g_lotMgr.GetLotCount() >= 2 )
// 				{
// 					if( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
// 					{
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;
// 
// 						m_nFindLotNo_Flag = 1;
// 					}
// 					else
// 					{
// 						return;
// 					}
// 				}
				m_move_step[m_nAxisNum] = 900;
// 			}
			break;

		case 900:
			m_move_step[m_nAxisNum] = 1000;
			break;


			////////////////////////////////////////////////////////////////////////////
			//1. 로딩 버퍼는 CarrierRbt로보트가 요청를 시작하면서 움직인다 
			//2. Carrier가 이동해야 하는지 또는 놓을때가 있는지 로딩상채인지 언로딩상태인디 체크하면서 자재를 요청
			//3. 로딩 자재를 로딩후 클램프로 off후 상태를 체크한다.
			//4. 혹시 언로딩하는 자재가 있는지, 동시 로딩 언로디이 필요한지 체크한다.
			//    동작중에 로드 언로드가 동시, 로딩만 인지 미리 계산후 동작하는게 좋다.
			////////////////////////////////////////////////////////////////////////////
		case 1000:
			if(nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && 
				nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_PLACE)  
			{
				m_move_step[m_nAxisNum] = 1100; 
			}
			//UNLOAD Picker 동작
	// 		else if(st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_DVC_UNLOAD)  
	// 		{
	// 			m_move_step[m_nAxisNum] = 1200; 
	// 		}
			break;

		case 1100:
			nRet_1 = Func.Check_BufferStatus(100, THD_LD_ALIGN_BUFF, CTL_NO, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus); //버퍼는 완전히 비워져 있어야 한다 
			if(nRet_1 == RET_GOOD)
			{
				//정상 상태, 처음으로 work robot 사이트로 이동하는 루틴으로, 로딩 버퍼에는 자재가 하나도 없어야 한다 
				m_move_step[m_nAxisNum] = 2000; 
			}
			else if(nRet_1 == RET_ERROR)
			{//자재가 남아있다면, 소켓 오프등으로 남아있는 자재이니 이떄는 테슽 로봇이 요청한 대로 바로 집을 수 있게 처리한다  

				m_move_step[m_nAxisNum] = 3000; 
				//CTL_Lib.Alarm_Error_Occurrence(m_nRunStep, dWARNING, Func.m_strAlarmCode);
			}
			break;

		case 1200://이떄 버퍼는 완전히 비워져 있어야 한다 
			nRet_1 = Func.Check_BufferStatus(100, THD_LD_ALIGN_BUFF, CTL_NO, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus); //버퍼는 완전히 비워져 있어야 한다 
			if(nRet_1 == RET_GOOD)
			{
				//정상 상태, 
				m_move_step[m_nAxisNum] = 6000; //test robot가 집고있던 로딩 소켓 오프 자재를 놓는 영역이다. 이 자재를 놓고 테스트 사이트로 이동하여 작업한다
			}
			else if(nRet_1 == RET_ERROR)
			{//자재가 남아있다면 에러 
				CTL_Lib.Alarm_Error_Occurrence(3100, dWARNING, Func.m_strAlarmCode);
			}			 
			break;

		case 2000:
// 			m_strFindLotNo = "0";//LotNo에 신경쓰지 않는다. LDPicker만 신경쓴다
			nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LD_ALIGN_BUFF, CTL_NO, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, _T("0"));
			if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다, 놓는 작업은 뒤에서(0번) 부터 놓는다 
			{
				m_nWork_Pos = m_npFindWorkPosYXCPB[0]; 
			}
			else if(nRet_1 == RET_SKIP) //더이상 넣을 공간이 없다 
			{	//계속 진행할 수 없다
				m_move_step[m_nAxisNum] = 1000;	 
				break;
			}
	// 		//work robot에 자재 공급을 요청 
	// 		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ; //로딩 자재 요청
	// 		st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE; //자재 놓는 작업
			nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY;
			m_move_step[m_nAxisNum] = 2100;
			break;

		case 2100:
			//LD_WORKRBT	st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY
			if(nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_CHANGE)//자재를 로보트가 로딩버퍼에 모두 채워 놓은 상태
			{
				nCount = 0;
// 				m_strLotNo = "";
// 				m_strPartNo = "";
				for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
				{ //버퍼의 자재 정보를 셋팅한다
					m_npBuff_Info[i] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo; //버퍼의 자재 정보를 셋팅한다
// 					if( m_npBuff_Info[i] == CTL_YES && m_strLotNo == "")
// 					{
// 						m_strLotNo = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].strLotNo;
// 						m_strPartNo = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].strPartNo;
// 					}
					nCount++;
				}

				if(nCount > 0)
				{
					m_move_step[m_nAxisNum] = 2200;

// 					m_dwCycleTime[1] = GetTickCount();
// 					m_dwCycleTime[2] = m_dwCycleTime[1] - m_dwCycleTime[0];
// 
// 					if (m_dwCycleTime[2] <= (DWORD)0)
// 					{
// 						m_dwCycleTime[0] = GetTickCount();
// 					}
// 					else
// 					{
// 						if( m_strLotNo != _T("") && m_strPartNo != _T("") )
// 						{
// 							Func.OnCycleTime(1, 
// 								m_strLotNo,
// 								m_strPartNo,
// 								m_dwCycleTime[2], 
// 								0, 
// 								0);
// 						}
// 
// 						m_dwCycleTime[0] = GetTickCount();
// 					}
					/////////////////////////////////////////////////////////////////////////////////////////////////
// 					st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount = 0;
// 					st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial = _T(""); 
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

// 					clsLog.LogTransfer(_T("LOADING_BUFFER"), 
// 						_T("ALIGN_BUFFER"), 
// 						0, 
// 						st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial,
// 						_T("CDIMM"), 
// 						_T("PLCE_BUFFER"), 
// 						_T("CARRIER_SITE"),
// 						st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount,
// 						st_buffer_info[THD_LD_ALIGN_BUFF].strLogKey,
// 						st_buffer_info[THD_LD_ALIGN_BUFF].strLogData);

				}
				else
				{//자재가 하나도 없는 상태 
					m_move_step[m_nAxisNum] = 0;
					if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
					{
						sprintf(st_msg.c_abnormal_msg, "There is no DVC in Load buffer step  = %d", m_move_step[m_nAxisNum]);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
				}
			}
			break;

		case 2200:
			Run_DvcLdBuffer.Set_Loader_Buffer_Align_OnOff(IO_ON);
			m_move_step[m_nAxisNum] = 2300;
			break;

		case 2300:
			nRet_1 = Run_LdPicker.Chk_Loader_Buffer_Align_OnOff( IO_ON );

			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 2310;
			}
			else if(nRet_1 == RET_ERROR)
			{			 
				CTL_Lib.Alarm_Error_Occurrence(3520, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 2200;
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
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			} 
			m_move_step[m_nAxisNum] = 2320;
			break;

		case 2320:
			Run_DvcLdBuffer.Set_Loader_Buffer_Align_OnOff(IO_OFF);
			m_move_step[m_nAxisNum] = 2330;
			break;

		case 2330:
			nRet_1 = Run_DvcLdBuffer.Chk_Loader_Buffer_Align_OnOff( IO_OFF );

			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 2340;
			}
			else if(nRet_1 == RET_ERROR)
			{			 
				CTL_Lib.Alarm_Error_Occurrence(3520, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 2320;
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
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			} 
			m_move_step[m_nAxisNum] = 2400;
			break;

		case 2400:
			nRet_1 = Func.Check_BufferStatus(0, THD_LD_ALIGN_BUFF, CTL_YES, m_npBuff_Info, m_npBuff_Status, m_npBuff_OutputStatus);
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 2500;

// 				clsLog.LogTransfer(_T("LOADING_BUFFER"), 
// 					_T("MOVE_BUFFER"), 
// 					1, 
// 					st_buffer_info[THD_LD_ALIGN_BUFF].strMaterial,
// 					_T("CDIMM"), 
// 					_T("PICK_BUFFER"), 
// 					_T("TEST_SITE"),
// 					st_buffer_info[THD_LD_ALIGN_BUFF].nLogCount,
// 					st_buffer_info[THD_LD_ALIGN_BUFF].strLogKey,
// 					st_buffer_info[THD_LD_ALIGN_BUFF].strLogData);
			}
			else if(nRet_1== RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(3120, dWARNING, Func.m_strAlarmCode);
			} 
			break;

		case 2500:
			//work robot에 자재 PICKUP을 요청 
			nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ; //로딩 자재 요청
			nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PICK; //자재 픽업
			m_move_step[m_nAxisNum] = 3000;
			break;

		case 3000:
			//LOAD RBT이 PICKUP을 완료함.
			if( nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0]== CTL_CHANGE )
			{
				m_move_step[m_nAxisNum] = 3100;
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
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			} 
			m_move_step[m_nAxisNum] = 3200;
			break;

		case 3200:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Align_Backward_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_move_step[m_nAxisNum] = 3210;
				break;
			}
			m_move_step[m_nAxisNum] = 3300;
			break;

		case 3210:
			Run_DvcLdBuffer.Set_Loader_Buffer_Align_OnOff(IO_OFF);
			m_move_step[m_nAxisNum] = 3220;
			break;

		case 3220:
			nRet_1 = Run_DvcLdBuffer.Chk_Loader_Buffer_Align_OnOff( IO_OFF );

			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 3300;
			}
			else if(nRet_1 == RET_ERROR)
			{			 
				CTL_Lib.Alarm_Error_Occurrence(3520, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 3210;
			}
			break;

		case 3300:
			nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CLEAR;		
			m_move_step[m_nAxisNum] =0;
			break;
			

		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_DVCLdBuffer  = %d", m_move_step[m_nAxisNum]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}

	return nFunRet;

}

int CDialog_Manual_Move::Move_LoadPlate()
{
	int nRet_1 = 0, x = 0, y = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0, nRet_3 = 0;
	int m_nAxisNum = 97;

	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
				m_move_step[m_nAxisNum] = 100;
// 			}
			break;

		case 100:
			m_move_step[m_nAxisNum] = 200;
			break;

		case 200:
			nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				//930001 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR"
				m_strAlarmCode.Format(_T("930001"));
				CTL_Lib.Alarm_Error_Occurrence(21001, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 100;
			}
			else 
			{
				m_move_step[m_nAxisNum] = 300;
			} 
			break;

		case 300:
			Run_LdTrayPlate.Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			m_move_step[m_nAxisNum] = 310;
			break;

		case 310:
			nRet_1 = Run_LdTrayPlate.Chk_Tray_Guide_Clamp_ForBackward(IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 400;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 400:
			m_move_step[m_nAxisNum] = 1000;
			break;

		case 1000:
			nLdPlate_Tray_Supply_Req[THD_LD_STACKER] = CTL_REQ;
			m_move_step[m_nAxisNum] = 1100;
			break;

		case 1100:
			if(nLdPlate_Tray_Supply_Req[THD_LD_STACKER] == CTL_READY)
			{
// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo) //2016.0907 
// 				{
					st_tray_info[THD_LD_TRAY_PLATE].nTrayExist = CTL_YES;
					for (x=0; x<st_recipe.nTrayX; x++)
					{
						for (y=0; y<st_recipe.nTrayY; y++)
						{
							st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].nYesNo = YES;
							st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].strLotNo = _T("");
							st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].strPartNo = _T("");
						}
					}
					m_move_step[m_nAxisNum] = 2000; 
// 				}		
// 
// 				if (st_handler.cwnd_main != NULL)
// 				{
// 					//st_handler.cwnd_main->SendMessage(WM_WORK_DISPLAY, MAIN_LOT_DISPLAY, 0);
//				}
			}
			break;

		case 2000:
			nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			else 
			{
				m_strAlarmCode.Format(_T("910002")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(5530, dWARNING, m_strAlarmCode); 
			}
			break;

		case 2100:
			Run_LdTrayPlate.Set_Tray_Guide_Clamp_ForBackward(IO_ON); 
			m_move_step[m_nAxisNum] = 2110;
			break;

		case 2110:
			nRet_1 = Run_LdTrayPlate.Chk_Tray_Guide_Clamp_ForBackward(IO_ON );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 2200;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 2100;
			}
			break;

		case 2200:
			nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
			if(nRet_1 == BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
			{
				nLdPlate_Tray_Supply_Req[THD_LD_TRAY_PLATE] = CTL_CLEAR;
				m_move_step[m_nAxisNum] = 5000;
			}
			else 
			{
				m_strAlarmCode.Format(_T("900001")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
				CTL_Lib.Alarm_Error_Occurrence(5550, dWARNING, m_strAlarmCode);			 
			} 
			break;

		case 5000:
			if(nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_REQ)
			{
				if(st_tray_info[THD_LD_TRAY_PLATE].nTrayExist == CTL_YES)
				{
					nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
					if(nRet_1 == BD_GOOD || g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check, IO_ON))
					{
						m_move_step[m_nAxisNum] = 5100;
					}
					else 
					{
						m_strAlarmCode.Format(_T("910002")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
						CTL_Lib.Alarm_Error_Occurrence(5560, dWARNING, m_strAlarmCode);			 
					} 					
				}
				else //if(st_tray_info[THD_LD_TRAY_PLATE].nTrayExist == CTL_NO)
				{
					nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
					if(nRet_1 == BD_GOOD)
					{
						m_strAlarmCode.Format(_T("910002")); //910002 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR."
						CTL_Lib.Alarm_Error_Occurrence(5570, dWARNING, m_strAlarmCode); 
					}
					else 
					{
						m_move_step[m_nAxisNum] = 1000;
					}
				}
			}  
			else
			{
// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo )
// 				{
// 					if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES")
// 					{
					//	m_move_step[m_nAxisNum] = 9000;
// 					}
// 				}		 
			}
			break; 

		case 5100:
			nRet_1 = Func.Find_TrayWork_Pos(0, THD_LD_TRAY_PLATE, CTL_YES, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, _T("0") );
			if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
			{
				// jtkim 20160831
				for (x=0; x<st_recipe.nTrayX; x++)
				{
					for (y=0; y<st_recipe.nTrayY; y++)
					{
// 						st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].strLotNo	= st_tray_info[THD_LD_STACKER].strLotNo;
// 						st_tray_info[THD_LD_TRAY_PLATE].st_pcb_info[y][x].strPartNo	= st_tray_info[THD_LD_STACKER].strPartNo;
					}
				}

				nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_READY;

				m_move_step[m_nAxisNum] = 5000;
			}
			else if(nRet_1 == RET_SKIP) //더이상 자재가 없다
			{
				m_move_step[m_nAxisNum] = 6000;
			}
			break;		

		case 6000: //트랜스퍼가 작업이 끝난 트레이를 empty에 버린다 
			Run_LdTrayPlate.Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			m_move_step[m_nAxisNum] = 6010;
			break;

		case 6010:
			nRet_1 = Run_LdTrayPlate.Chk_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 6100;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(5580, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 6000;
			}
			break; 

			//load plate 트레이 교체를 요청 
		case 6100:
			if( nWorkTransfer_Req[THD_EMPTY_STACKER][0] == CTL_CLEAR )
			{
				nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ; //트레이 교체를 요청 
				nWorkTransfer_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK; //트레이 집어가라 요청 
				m_move_step[m_nAxisNum] = 6200;
			}
			break;

		case 6200:
			if(nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_READY) //트랜스퍼가 트레이를 집고 셋팅 
			{
				nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
				if(nRet_1 == BD_GOOD)//BD_ERROR) //트레이가 감지 안된 상태 
				{
					nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_FREE;
					nWorkTransfer_Req[THD_LD_TRAY_PLATE][1] = CTL_CLEAR; 
					m_move_step[m_nAxisNum] = 6300;
				}
				else 
				{
					m_strAlarmCode.Format(_T("910003")); //910003 1 A "LOAD_STACKER_PLATE_SD_TRAY_OFF_CHECK_ERROR."
					CTL_Lib.Alarm_Error_Occurrence(5590, dWARNING, m_strAlarmCode);			 
				} 
			}
			break;

		case 6300:
// 			if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo)
// 			{
// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" )
// 				{
// 					m_move_step[m_nAxisNum] = 9000;
// 					break;
// 				}
// 			}
			if( nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] == CTL_CLEAR)
				m_move_step[m_nAxisNum] = 1000; //스태커에 트레이 공급을 요청위해 처음으로 간다 
			break;

		case 9000:
			Run_LdTrayPlate.Set_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			m_move_step[m_nAxisNum] = 9010;
			break;

		case 9010:
			nRet_1 = Run_LdTrayPlate.Chk_Tray_Guide_Clamp_ForBackward(IO_OFF); 
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 9100;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(5580, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 9000;
			}
			break; 

		case 9100:
// 			if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetLotID() == st_tray_info[THD_LD_STACKER].strLotNo)
// 			{
// 				if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" )
// 				{
					m_move_step[m_nAxisNum] = 0;
					break;
// 				}
// 			}		
// 			break; 

		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_LdTrayPlate  = %d", m_move_step[m_nAxisNum]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}

	return nFunRet;
}	

int CDialog_Manual_Move::Move_LoadTransfer()
{
	int nRet_1 = 0, i = 0, nCount = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0, nRet_3 = 0;
	int m_nAxisNum = M_LOADER_TRANSFER_Y;
	int m_nRobot_Y = M_LOADER_TRANSFER_Y;
	int m_nRobot_Z = M_LOADER_TRANSFER_Z;
	CString strAlarm = _T("");

	int m_nPickerPara = PICKER_PARA;//1
	int m_nRobotSite  = THD_LOAD_WORK_RBT;
	

	double dCurrX, dCurrY;
	
	dCurrX = COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y );
	dCurrY = COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Z );
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:		
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
				m_move_step[m_nAxisNum] = 10;
				m_LoadBufferPos = 0;
// 			}
			break;

		case 10:
			Run_LdPicker.Set_Loader_Transfer_Clamp_OnOff( IO_OFF);
			m_move_step[m_nAxisNum] = 20;
			break;

		case 20:
			nRet_1 = Run_LdPicker.Chk_Loader_Transfer_Clamp_OnOff( IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 100;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1022, dWARNING, Run_LdPicker.m_strAlarmCode);
			}
			break;


		case 100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON);
			nRet_2 = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_On_Sol,	IO_OFF);
			nRet_3 = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_Off_Sol, IO_ON);
			if(nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_ON) 
			{
				//810113 0 A "LOADER_PICKER_CLAMP_ON_ERROR."
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);	
			}
			else
			{
				st_picker[m_nRobotSite].st_pcb_info[0].nYesNo = CTL_NO; //james 2016.0909 
				m_move_step[m_nAxisNum] = 200;				 
			}
			break;

		case 200:
			Run_LdPicker.Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			m_move_step[m_nAxisNum] = 210;
			break;

		case 210:
			nRet_1 = Run_LdPicker.Chk_Loader_Transfer_Clamp_OnOff( IO_OFF ); 			 
			if(nRet_1 == RET_GOOD)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
				m_move_step[m_nAxisNum] = 100;
			}
			break;

		case 300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 300;
			}
			break;

		case 400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 500;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 400;
			}
			break;

		case 500:
// 			if( g_lotMgr.GetLotCount() > 0 )
// 			{
// 				if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
// 				{
					//load plate에 자재 요청

			if(nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_READY )
			{
// 					nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 					nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;

					nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ;
					nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE;
			
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
// // 						m_nFindLotNo_Flag = 1;
// 					}
// 					else
// 					{
// 						if( COMI.Get_MotCurrentPos(m_nRobot_Y) >= ( st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] + st_motor[m_nRobot_Y].mn_allow ) )
// 						{
// 							nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
// 							if (nRet_1 == BD_GOOD)
// 							{
// 								m_move_step[m_nAxisNum] = 500;
// 							}
// 							else if (nRet_1 == BD_RETRY)
// 							{
// 								m_move_step[m_nAxisNum] = 500;
// 							}
// 							else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 							{
// 								CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
// 								m_move_step[m_nAxisNum] = 500;
// 							}
// 						}					
// 
// 						return;
// 					}
// 				}
				m_move_step[m_nAxisNum] = 1000;
			}
// 			}
			break;


		case 1000:
// 			m_nFindLotNo_Flag = -1;
// 			m_strFindLotNo = "";
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
// 						return;
// 					}
// 				}
// 				else
// 				{
// 					return;
// 				}
// 
				//Carrier가 Unload 제품이 발생하여 unload하라고 Unload picker에게 요청했을때 load 피커에게 알려준다.
				if( nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ)//There is PCB to unload.
				{
					m_move_step[m_nAxisNum] = 1100;
				}
				else
				{
					int nCount = 0;
					for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
					{
						if(st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo == CTL_YES)
						{
							nCount++;			
						} 
					}
					if(nCount > 0)
					{
						if( nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_READY )
						{
							//m_nLoadBuff_Job_Continue_Flag = CTL_NO; //clear

							m_move_step[m_nAxisNum] = 3000;
							break;
						}
					}

					if(nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_READY)
					{
// 						if(m_strFindLotNo != st_tray_info[THD_LD_STACKER].strLotNo)
// 						{						
// 							//메세지 출력 
// 							break;
// 						}
						if( nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] != CTL_READY ) break;
						m_move_step[m_nAxisNum] = 2000;
					}
				}
// 			}
// 			else
// 			{
// 				m_move_step[m_nAxisNum] = 0;
// 			}
			break;
		
		case 1100:
			if( COMI.Get_MotCurrentPos(m_nRobot_Y) <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] )
			{
				m_move_step[m_nAxisNum] = 1400;
			}
			else
			{
				nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], st_motor[m_nRobot_Y].mn_allow);
				nRet_2 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], st_motor[m_nRobot_Z].mn_allow);
				if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
				{
					m_move_step[m_nAxisNum] = 1200;
				}
				else
				{
					m_move_step[m_nAxisNum] = 1000;
				}
			}
			break;

		case 1200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 1300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1200;
			}
			break;

		case 1300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 1400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 1300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1105, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 1300;
			}
			break;

		case 1400:
			nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_READY;
			m_move_step[m_nAxisNum] = 1500;
			break;

		case 1500:
			if(nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_FREE)
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			else if(nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ)
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 2000://tray pick up
			nRet_1 = Run_LdPicker.Process_DVC_Pickup( 0, THD_LD_TRAY_PLATE, _T("0") );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2010;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			break;

		case 2010:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 2010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 2010;
			}
			break;


		case 2100:
// 			if( m_strFindLotNo == st_tray_info[THD_LD_STACKER].strLotNo )
// 			{
				nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
				m_move_step[m_nAxisNum] = 2200;
// 			}
// 			else
// 			{
// 				//900100 1 00 "IT_IS DIFFERENT_TO_LOT_NAME_BETWEEN_LOT_LOAD_STACKER."
// 				CTL_Lib.Alarm_Error_Occurrence(1105, dWARNING, "900100");
// 			}
			break;

		case 2200:
			if( st_picker[m_nRobotSite].st_pcb_info[0].nYesNo == CTL_NO )
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			else
			{
				m_move_step[m_nAxisNum] = 3000;
			}
			break;

		case 3000:
			if( nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_READY && 
				nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_PLACE )
			{
				m_move_step[m_nAxisNum] = 3100;
			}
			break;

		case 3100:
			nRet_1 = Run_LdPicker.Process_DVC_Place( 0, THD_LD_ALIGN_BUFF, _T("0") );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 3200;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 3100;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 3100;
			}
			break;
		
		case 3200:
			nCount = 0;
			for(i = 0; i < PICKER_PARA; i++)
			{					 
				if(st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].nYesNo == CTL_YES)
				{
// 					m_strLotNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
// 					m_strPartNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					nCount++;
				} 
			}
			if(nCount > 0) //자재가 남아있으면 
			{
				m_move_step[m_nAxisNum] = 3100;   
			}
			else
			{
				m_move_step[m_nAxisNum] = 3300;   
			}
			break;

		case 3300:
			nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LD_ALIGN_BUFF, CTL_NO, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, _T("0") );		 			
			if(nRet_1 == RET_SKIP) //더이상 작업할 공간이 없다 
			{	
				nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CHANGE; //버퍼에 위치를 잡도록 함(실린더 동작)


				if( st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].nYesNo == CTL_YES)
				{
// 					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strLotNo;
// 					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strPartNo;
				}

				m_move_step[m_nAxisNum] = 3400;

				////////////////////////////////////////////////////////////////////////////////
				if(m_npFindWorkPosYXCPB[2] == 0)
				{//버퍼에는 더이상 놓은 공간이 없다
// 					m_dwCycleTime[0][1] = GetTickCount();
// 					m_dwCycleTime[0][2] = m_dwCycleTime[0][1] - m_dwCycleTime[0][0];
// 
// 					if (m_dwCycleTime[0][2] <= (DWORD)0)
// 					{
// 						m_dwCycleTime[0][0] = GetTickCount();
// 					}
// 					else
// 					{
// 						if (m_dwCycleTime[0][2] > (DWORD)0)
// 						{
// 							if( m_strLotNo[0] != _T("") && m_strPartNo[0] != _T("") )
// 							{
// 								Func.OnCycleTime(0, 
// 									m_strLotNo[0],
// 									m_strPartNo[0],
// 									m_dwCycleTime[0][2], 
// 									0, 
// 									0);
// 							}
// 
// 							m_strLotNo[0] = m_strPartNo[0] = _T("");
// 						}
// 						m_dwCycleTime[0][0] = GetTickCount();
// 					}
				}
				////////////////////////////////////////////////////////////////////////////////

			}
			else
			{
				m_move_step[m_nAxisNum] = 3400;
			}
			break;

			//////////////////////////////////////////////////////////////
			//LD ALIGN BUFF에서 제대로 안착이 됐는지 확인한다.
			//////////////////////////////////////////////////////////////
		case 3400:
			if( nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && 
				nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_PICK )
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			break;

		case 4000:
			nRet_1 = Run_LdPicker.Process_DVC_Pickup( 0, THD_LD_ALIGN_BUFF, _T("0") );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 4100;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			break;

		case 4100:
			nCount = 0;
			for(i = 0; i < PICKER_PARA; i++)
			{					 
				if(st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].nYesNo == CTL_YES)
				{
// 					m_strLotNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
// 					m_strPartNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					nCount++;
				} 
			}
			if(nCount > 0) //자재가 남아있으면 
			{
				m_move_step[m_nAxisNum] = 4200;   
			}
			else
			{
				m_move_step[m_nAxisNum] = 4000;//에러 들어오면 알람 추가 기능 
			}
			break;

		case 4200:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loading_Tr_Jig_Detect_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			}

			m_move_step[m_nAxisNum] = 4300;
			break;

		case 4300:
			//디바이스 픽업 센서체크함
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loading_Tr_Jig_Detect_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			}

			m_move_step[m_nAxisNum] = 4400;
			break;

		case 4400:
			nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CHANGE;
			m_move_step[m_nAxisNum] = 5000;

			break;

		////////////////////////////////
		//Carrier Buffer Place
		////////////////////////////////
		case 5000:
			if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ && 
				nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
			{
				nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_READY;
				nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = WORK_PLACE;
				m_move_step[m_nAxisNum] = 5100;
			}
			else if( nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY && 
				nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
			{
				m_move_step[m_nAxisNum] = 5100;
			}
			else if(mn_move_carrier == 3 )
			{
				m_move_step[m_nAxisNum] = 5100;
			}
			break;

		case 5100:
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X )  > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_UNLOADER_TRANSFER_X].mn_allow ) )
			{
				break;
			}
// 			if( st_handler.mn_lduld_safety == ULD_MOVING_SAFETY)
// 			{
// 				break;
// 			}
			st_handler.mn_lduld_safety = LD_MOVING_SAFETY;
			m_move_step[m_nAxisNum] = 5200;
			break;


		case 5200:
			nRet_1 = Run_LdPicker.Process_DVC_Place( 0, THD_LDULD_CARRIER_BUFF, _T("0") );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 5300;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 5200;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 5200;
			}
			break;

		case 5300:
			nCount = 0;
			for(i = 0; i < PICKER_PARA; i++)
			{					 
				if(st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].nYesNo == CTL_YES)
				{
// 					m_strLotNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
// 					m_strPartNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					nCount++;
				} 
			}
			if(nCount > 0) //자재가 남아있으면 
			{
				m_move_step[m_nAxisNum] = 5200;   
			}
			else
			{
				m_move_step[m_nAxisNum] = 5400;   
			}
			break;

		case 5400:
			//nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LDULD_CARRIER_BUFF, CTL_YES, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strFindLotNo );	
			//if(nRet_1 == RET_SKIP) //더이상 작업할 공간이 없다 
			
			//어느 carrier에 넣었는지 체크해서 확인 필요
			if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[m_LoadBufferPos] == CTL_YES )// && (st_buffer_info[THD_LDULD_CARRIER_BUFF].st_pcb_info[m_n_FirstTray_Y_Num].strLotNo == m_strFindLotNo) )
			{			
				nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY;

				//Carrier
				//Carrier site 의 버퍼 위치 
				//nPos
				int nPos = 0;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[m_n_FirstTray_Y_Num] == CTL_YES)
				{
// 					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strLotNo;
// 					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strPartNo;
				}
				m_move_step[m_nAxisNum] = 5500;
				////////////////////////////////////////////////////////////////////////////////
				if(m_npFindWorkPosYXCPB[2] == 0)
				{//버퍼에는 더이상 놓은 공간이 없다
// 					m_dwCycleTime[0][1] = GetTickCount();
// 					m_dwCycleTime[0][2] = m_dwCycleTime[0][1] - m_dwCycleTime[0][0];
// 
// 					if (m_dwCycleTime[0][2] <= (DWORD)0)
// 					{
// 						m_dwCycleTime[0][0] = GetTickCount();
// 					}
// 					else
// 					{
// 						if (m_dwCycleTime[0][2] > (DWORD)0)
// 						{
// 							if( m_strLotNo[0] != _T("") && m_strPartNo[0] != _T("") )
// 							{
// 								Func.OnCycleTime(0, 
// 									m_strLotNo[0],
// 									m_strPartNo[0],
// 									m_dwCycleTime[0][2], 
// 									0, 
// 									0);
// 							}
// 
// 							m_strLotNo[0] = m_strPartNo[0] = _T("");
// 						}
// 						m_dwCycleTime[0][0] = GetTickCount();
// 					}
				}
				////////////////////////////////////////////////////////////////////////////////

			}
			else
			{
				m_move_step[m_nAxisNum] = 5500;
			}
			break;

		case 5500:
			//높이 체크
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loader_Transfer_Clamp_Off_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			}

			m_move_step[m_nAxisNum] = 5600;
			break;

		case 5510:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 5600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 5510;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1194, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 5510;
			}
			break;

		case 5600:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], st_motor[m_nRobot_Z].mn_allow);
			if(nRet_1 != BD_GOOD )
			{
				m_move_step[m_nAxisNum] = 5510;
			}
			else
			{
				m_move_step[m_nAxisNum] = 5700;
			}
			break;

		case 5700:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 6000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 5700;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 5700;
			}
			break;

		case 6000:
			m_LoadBufferPos++;
			if( m_LoadBufferPos >= 3 )
			{
				m_LoadBufferPos = 0;
				nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_NO; 
				nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = CTL_NO;
			}
			m_move_step[m_nAxisNum] = 500;
			break;


		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_LdPicker  = %d", m_move_step[m_nAxisNum]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;		
	}

	return nFunRet;
}

//m_hs_rub_pick == 0 pick &place
//m_hs_rub_pick == 3 reverse pick
//m_hs_rub_pick == 4,5,6 carrier#1#2#3
int CDialog_Manual_Move::Move_HSPickHeatSinkTransfer( int m_hs_rub_pick )
{
	int nRet_1 = 0;
	int nFunRet = RET_PROCEED, nRet_2 = 0;
	int m_nAxisNum = 89;
	int m_nRobot_X = M_HEATSINK_TRANSFER_X;
	int m_nRobot_Y = M_HEATSINK_TRANSFER_Y;
	int m_nRobot_Z = M_HEATSINK_TRANSFER_Z;
	int m_nRobot_P = M_HEATSINK_PICKER_PITCH;

	if( m_nhsexystop == 1)
		return nFunRet;
	
	switch( m_move_step[m_nAxisNum] )
	{
		case 0:
			m_move_step[m_nAxisNum] = 10;
			if( m_hs_rub_pick == 3 )
				m_move_step[m_nAxisNum] = 3000;
			else if( m_hs_rub_pick == 4 || m_hs_rub_pick == 5 || m_hs_rub_pick == 6 )
				m_move_step[m_nAxisNum] = 4000;
			break;

		case 10:		
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 0;
				m_nhsexystop = 1;
			}
			break;

		case 100:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_P, st_motor[m_nRobot_P].md_pos[P_HEATSINK_PICKER_PITCH_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_move_step[m_nAxisNum] = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				m_move_step[m_nAxisNum] = 100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				m_move_step[m_nAxisNum] = 100;
				m_nhsexystop = 1;
			}
			break;

		case 200:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INIT_POS], COMI.md_allow_value[m_nRobot_X]);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				m_move_step[m_nAxisNum] = 300;
			}
			else
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 300:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 1000:
			//load plate에 자재 요청
			if( m_hs_rub_pick >= 0 || ( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_REQ && 
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE ) )
			{
				m_move_step[m_nAxisNum] = 1100;
			}
			break;

		case 1100:
			//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
// 				if( m_nlast_pick == CTL_YES ) m_move_step[m_nAxisNum] = 2000;
				m_move_step[m_nAxisNum] = 1200;
			}
			break;

		case 1200:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Pickup( 0, THD_LD_HEATSINK_BUFF );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2000;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 1200;
			}
			break;

		case 2000:
			//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 2100;
			}
			break;

		case 2100:
			if( g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON) == IO_ON && 
				g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF) == IO_OFF )
			{
				nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Place( 0, THD_PLACE_HEATSINK_DVC, 0 );
				if( nRet_1 == RET_GOOD )
				{
					m_move_step[m_nAxisNum] = 2200;
				}
				else if( nRet_1 == RET_PICKER_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 2100;
				}
				else if( nRet_1 == RET_TRAY_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 2100;
				}							
			}
			else
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Reverse_0_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				m_nhsexystop = 1;
			}
			break;

		case 2200:
			//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				st_sync.nDisPensorFlag = CTL_YES;
// 				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
// 				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;

				m_move_step[m_nAxisNum] = 2300;
			}
			break;

		case 2300:
			if( m_hs_rub_pick == 0 )
				nFunRet = RET_GOOD;
// 			if( st_sync.nDisPensorFlag == CTL_YES )
// 			{
// 				if( nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_CHANGE && 
// 					nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE)
// 				{
// 					m_move_step[m_nAxisNum] = 3000;
// 					break;
// 				}
// 			}
// 			if( st_sync.nDisPensorFlag == CTL_YES )
// 			{
// 				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
// 				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
// 				m_move_step[m_nAxisNum] = 0;
// 			}
			break;

		case 3000:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Pickup( 0, THD_DISPENSOR_PRBT );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 3110;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				m_move_step[m_nAxisNum] = 3000;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				st_buffer_info[THD_DISPENSOR_PRBT].st_pcb_info[0].nYesNo = CTL_YES;
				m_move_step[m_nAxisNum] = 3000;
			}
			break;

		case 3110:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				if( m_hs_rub_pick >= 0 )
				{
					nFunRet = RET_GOOD;
				}
				else
				{
					nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_NO;
					nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = CTL_NO;
					m_move_step[m_nAxisNum] = 3120;
				}
			}
			break;

		case 3120:
			if( st_sync.nDisPensorFlag == CTL_YES )
			{
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
				nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
			}
			m_move_step[m_nAxisNum] = 3200;
			break;

		case 3200:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Measure_Vision();
			if( nRet_1 == RET_GOOD )
			{
// 				g_lotMgr.GetLotAt(m_nFindLotNo_Flag).AddHeatSinkInputCnt(PRIME);//히트싱크 픽업증가
				m_move_step[m_nAxisNum] = 4000;
			}
			else if(nRet_1 == RET_ERROR)//hestsink를 버렸다 다시 집는다.
			{
				m_move_step[m_nAxisNum] = 1000;
			}
			break;

		case 4000://move carrier safety
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 3, 0 );
			if( nRet_1 == RET_GOOD )
			{
				//m_nCarriorPos = 0;
				m_move_step[m_nAxisNum] = 4100;
				if( m_hs_rub_pick >= 4 )
					m_move_step[m_nAxisNum] = 4110;
			}
			break;
			
		case 4100:
 			if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_REQ && 
 				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] == WORK_PLACE)
 			{
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] = CTL_READY;
				m_move_step[m_nAxisNum] = 4200;
 			}
			else if( m_nCarriorPos> 0 && nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_CHANGE && 
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] == WORK_PLACE)
			{
				m_move_step[m_nAxisNum] = 4200;
			}
			break;

		case 4110:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 3, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 4200;
				if( m_hs_rub_pick >= 4 || m_hs_rub_pick >= 3)
				{
					m_nCarriorPos = m_hs_rub_pick-4;
					m_move_step[m_nAxisNum] = 4210;
				}

			}
			break;

		case 4200:
			if( nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][0] == CTL_CHANGE && 
				nCarrierRbt_Dvc_Req[THD_HEATSINK_RBT][1] == WORK_PLACE)
			{
				m_move_step[m_nAxisNum] = 4210;
			}
			//임시
// 			m_move_step[m_nAxisNum] = 4210;
			break;

		case 4210:
			if( ( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + COMI.md_allow_value[m_nRobot_X] ) ) &&
				( COMI.Get_MotCurrentPos(m_nRobot_Y) > ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] - COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = Run_HeatSinkVision_Transfer_Robot.Process_Dvc_Place(0, THD_PLACE_CARRIER_DVC, m_nCarriorPos);
				if( nRet_1 == RET_GOOD )
				{
					m_move_step[m_nAxisNum] = 4300;
				}
				else if( nRet_1 == RET_PICKER_NOT_FIND)
				{
					st_picker[THD_HEATSINK_PRBT].st_pcb_info[0].nYesNo = CTL_YES;
					m_move_step[m_nAxisNum] = 4210;
				}
				else if( nRet_1 == RET_TRAY_NOT_FIND)
				{
					m_move_step[m_nAxisNum] = 4210;
				}
			}
			else
			{
				m_move_step[m_nAxisNum] = 4000;
			}
			break;

		case 4300:
			nRet_1 = Run_HeatSinkVision_Transfer_Robot.Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				m_move_step[m_nAxisNum] = 0;
				nFunRet = RET_GOOD;
			}
			break;
	}
	return nFunRet;
}

void CDialog_Manual_Move::OnTimer(UINT nIDEvent) 
{
	int nRet_1 = 0, nRet_2 = 0;
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == TM_TRAY1)
	{
		nRet_1 = Move_TrayElv1();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	else if( nIDEvent == TM_TRAY2)
	{
		nRet_1 = Move_TrayElv2();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	else if( nIDEvent == TM_TRAY_TRANSFER)
	{
		nRet_1 = Move_TrayTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if( nIDEvent == TM_CARRIER_MOVE)
	{
		nRet_1 = Move_CarrierTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent);
			Init_Timer();
		}
	}
	if( nIDEvent == TM_UNPRESS_MOVE )
	{
		nRet_1 = Move_UnPressTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent);
			Init_Timer();
		}
	}
	if( nIDEvent == TM_EPOXY_MOVE)
	{
		nRet_1 = Move_EpoxyTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if( nIDEvent == TM_LOAD_PLATE )
	{
		nRet_1 = Move_LoadPlate();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_LOADER_MOVE )
	{
		nRet_1 = Move_LoadTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_DVCBUFFER)
	{
		nRet_1 = Move_DvcBuffer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_HEATSINK_MOVE )
	{
		nRet_1 = Move_HeatSinkTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_HSVISION_MOVE )
	{
		nRet_1 = Move_VisionTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_DISPENSOR )
	{
		nRet_1 = Move_Dispensor();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
		nRet_2 = Move_BuffDispensor();
		if( nRet_2 == RET_GOOD || nRet_2 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_ONLYDISPENSOR )
	{
		nRet_1 = Move_Dispensor();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if ( nIDEvent == TM_ONLYREVERSE )
	{
		nRet_1 = Move_BuffDispensor();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}
	if( nIDEvent == TM_ONLYHSPICKPLACE )
	{
		if( m_hs_rub != 0 && m_hs_rub != 3 && m_hs_rub != 4 && m_hs_rub != 5 && m_hs_rub != 6)
		{
			KillTimer( nIDEvent);
		}
		else
		{
			nRet_1 = Move_HSPickHeatSinkTransfer(m_hs_rub);
			if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
			{
				KillTimer( nIDEvent );
				Init_Timer();
			}
		}

	}
	if ( nIDEvent == TM_VISIONCHK )
	{
		nRet_1 = Move_VisionTransfer();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{
			KillTimer( nIDEvent );
			Init_Timer();
		}
	}


	CDialog::OnTimer(nIDEvent);
}


void CDialog_Manual_Move::OnReqTrayChange() 
{
// 	nWorkTransfer_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 				SetTimer( TM_LOADER_MOVE, 100, NULL );
// 				SetTimer( TM_LOAD_PLATE, 100, NULL );
// 				SetTimer( TM_TRAY1, 100, NULL );
// 				SetTimer( TM_TRAY2, 100, NULL );
// 				SetTimer( TM_TRAY_TRANSFER, 100, NULL );
// 				SetTimer( TM_DVCBUFFER, 100, NULL );

}


void CDialog_Manual_Move::OnBtnStop() 
{
	m_nhsexystop = 1;
	m_nvisionstop = 1;
	for ( int i = 0; i < M_MOTOR_COUNT; i++ )
	{
		COMI.Set_MotStop( 0, i );
	}

	if( m_hs_rub >= 0 && m_hs_rub < 7)
	{
		return;
	}
	if( m_hs_vision >= 0 && m_hs_vision < 3)
	{
		return;
	}
	Init_Timer();
// 	SetTimer( TM_LOADER_MOVE, 100, NULL );
// 	SetTimer( TM_LOAD_PLATE, 100, NULL );
// 	SetTimer( TM_TRAY1, 100, NULL );
// 	SetTimer( TM_TRAY2, 100, NULL );
// 	SetTimer( TM_TRAY_TRANSFER, 100, NULL );
// 	SetTimer( TM_DVCBUFFER, 100, NULL );

}

void CDialog_Manual_Move::OnBtnHeatSinkGo() 
{

	int m,x,y;
	st_recipe.nTrayNum = 2;
	st_recipe.nHsTrayY = 5;
	st_recipe.nHsTrayX = 9;
<<<<<<< HEAD:Handler_820_20120109/Dialog_Manual_Move.cpp
	
	for(m = 0; m<st_recipe.nTrayNum; m++)
	{
		for(y = 0; y < st_recipe.nHsTrayY; y++) //Y 방향 정보
		{
			for(x = 0; x < st_recipe.nHsTrayX; x++) //X 방향 정보 
			{ 
				st_tray_info[THD_LD_HEATSINK_BUFF].st_pcb_info[y][x].nYesNo = DVC_YES;
			}
		}
	}

	Init_Timer();

    m_hs_rub = m_cbo_hs_rub.GetCurSel();
	if( m_hs_rub <= 0)  m_hs_rub = 0;

	
	if( m_hs_rub == 0)//hs pick and dispensor place
	{
		SetTimer( TM_ONLYHSPICKPLACE, 100, NULL );
	}
	else if(m_hs_rub == 1)//dispensor and reverse
	{
		SetTimer( TM_ONLYDISPENSOR, 100, NULL);
	}
	else if(m_hs_rub == 2)//reverse
	{
		SetTimer( TM_ONLYREVERSE, 100, NULL);
	}
	else if(m_hs_rub == 3)//reverse pick and mve safety
	{
		SetTimer( TM_ONLYHSPICKPLACE, 100, NULL );
	}
	else if(m_hs_rub == 4 || m_hs_rub == 5 || m_hs_rub == 6 )//carrier 1,2,3 rubbing
	{
		SetTimer( TM_ONLYHSPICKPLACE, 100, NULL );

	}

}

void CDialog_Manual_Move::OnButton1() 
{
	m_nhsexystop = 0;	
}

void CDialog_Manual_Move::OnBtnVisionGo() 
{
	Init_Timer();

	m_hs_vision = m_cbo_vision.GetCurSel();
	if( m_hs_vision <= 0 ) m_hs_vision = 0;

	if( m_hs_vision >= 0 && m_hs_vision < 3)
	{
		SetTimer( TM_VISIONCHK, 100, NULL );
		//SetTimer( TM_HSVISION_MOVE, 100, NULL );

	}
=======
>>>>>>> 03c9121054aa9555b02a1f7854d5a71699174b8b:Handler_820_20120109/Dialog_Manual_Move.cpp
	
	for(m = 0; m<st_recipe.nTrayNum; m++)
	{
		for(y = 0; y < st_recipe.nHsTrayY; y++) //Y 방향 정보
		{
			for(x = 0; x < st_recipe.nHsTrayX; x++) //X 방향 정보 
			{ 
				st_tray_info[THD_LD_HEATSINK_BUFF].st_pcb_info[y][x].nYesNo = DVC_YES;
			}
		}
	}

	Init_Timer();

    m_hs_rub = m_cbo_hs_rub.GetCurSel();
	if( m_hs_rub <= 0)  m_hs_rub = 0;

	
	if( m_hs_rub == 0)//hs pick and dispensor place
	{
		SetTimer( TM_ONLYHSPICKPLACE, 100, NULL );
	}
	else if(m_hs_rub == 1)//dispensor and reverse
	{
		SetTimer( TM_ONLYDISPENSOR, 100, NULL);
	}
	else if(m_hs_rub == 2)//reverse
	{
		SetTimer( TM_ONLYREVERSE, 100, NULL);
	}
	else if(m_hs_rub == 3)//reverse pick and mve safety
	{
		SetTimer( TM_ONLYHSPICKPLACE, 100, NULL );
	}
	else if(m_hs_rub == 4 || m_hs_rub == 5 || m_hs_rub == 6 )//carrier 1,2,3 rubbing
	{
		SetTimer( TM_ONLYHSPICKPLACE, 100, NULL );

	}

}

<<<<<<< HEAD:Handler_820_20120109/Dialog_Manual_Move.cpp
=======
void CDialog_Manual_Move::OnButton1() 
{
	m_nhsexystop = 0;	
}

void CDialog_Manual_Move::OnBtnVisionGo() 
{
	Init_Timer();

	m_hs_vision = m_cbo_vision.GetCurSel();
	if( m_hs_vision <= 0 ) m_hs_vision = 0;

	if( m_hs_vision >= 0 && m_hs_vision < 3)
	{
		SetTimer( TM_VISIONCHK, 100, NULL );
		//SetTimer( TM_HSVISION_MOVE, 100, NULL );

	}
	
}

>>>>>>> 03c9121054aa9555b02a1f7854d5a71699174b8b:Handler_820_20120109/Dialog_Manual_Move.cpp
void CDialog_Manual_Move::OnButton15() 
{
	m_nvisionstop = 0;	
	
}
