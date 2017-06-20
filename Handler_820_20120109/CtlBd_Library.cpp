// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"

#include "io.h"	
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "IO_Manager.h"
#include "SrcPart/APartHandler.h"

#include "Srcbase\ALocalization.h"
#include "CmmsdkDef.h"
#include "MyJamData.h"
#include "Cmmsdk.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCtlBd_Library::CCtlBd_Library()
{
	m_nMcEmg = NO;
}

CCtlBd_Library::~CCtlBd_Library()
{

}

CCtlBd_Library CTL_Lib;
//////////////////////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{
	int n_cur_num = -1;
	CString strTemp = "";

	CTL_Lib.mn_jampos = n_jampos;
	CTL_Lib.mn_run_status = n_run_status;
	COMI.mn_run_status = n_run_status;
	st_work.mn_run_status = n_run_status;
	strTemp = c_alarmcode;
	alarm.mstr_code = c_alarmcode;
	alarm.stl_cur_alarm_time = GetCurrentTime();

	strcpy(mc_alarmcode, c_alarmcode);

// 	for(int n_cnt=0; n_cnt<2000; n_cnt++)
// 	{  
// 		if ((st_alarm.mstr_e_code[n_cnt]).Compare(alarm.mstr_code) == 0)
// 		{
// 			n_cur_num = n_cnt; 
// 			break;
// 		}
// 	}


	MyJamData.On_Alarm_Info_Set_to_Variable(alarm.mstr_code);

	
	if (st_handler.cwnd_list != NULL)  // ∏ÆΩ∫∆Æ πŸ »≠∏È ¡∏¿Á
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg, strTemp);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	}	

// 	st_handler.mn_dumping_alarm_count++;
// 
// 	sprintf( st_msg.c_normal_msg, "Dumping Test Pick & Place SSD : %d / %d" ,  st_handler.mn_dumping_alarm_count, st_handler.mn_dumping_test_count);
// 	st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG );

	//st_handler.m_tDMTBI = 0;
	

	return BD_GOOD;
}

int CCtlBd_Library::Alarm_Error_Occurrence( int n_jampos, int n_run_status, CString strJamCode )
{
// 	//2013,0123
// 	if(COMI.mn_run_status == dINIT) alarm.stl_a .stl_alarm_start_time = GetCurrentTime();
	int n_cur_num = -1;
	CString strTemp = "";
	CTL_Lib.mn_jampos = n_jampos; //jam¿Ã πﬂª˝«— ¿ßƒ°∏¶ ¿˙¿Â«—¥Ÿ 
	CTL_Lib.mn_run_status = n_run_status; //¿Â∫Ò¿« ∞°µøªÛ≈¬∏¶ ∫Ø∞Ê«—¥Ÿ 
	COMI.mn_run_status = n_run_status;
	strTemp = strJamCode;
	alarm.mstr_code = strJamCode;
	st_work.mn_run_status = n_run_status;
	alarm.stl_cur_alarm_time = GetCurrentTime();
	
	strcpy(mc_alarmcode, (LPCSTR)strJamCode);
	
	//	g_handler.AddAlarmCnt();
	
	//2012,1220
	// 	CtlBdFunc.Alarm_Error_Occurrence(CTL_Lib.mn_jampos, COMI.mn_run_status, COMI.mn_run_status, alarm.mc_code);
	MyJamData.On_Alarm_Info_Set_to_Variable(alarm.mstr_code);

	
	if (st_handler.cwnd_list != NULL)  // ∏ÆΩ∫∆Æ πŸ »≠∏È ¡∏¿Á
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg, strTemp);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	}
	
	
	
	return BD_GOOD;
}

int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor Î≥¥Îìú Ï¥àÍ∏∞ÌôîÏãú ÏÇ¨Ïö©
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if( n_bd_type == 0 ) //0:fastec
	{
		COMI.mn_totalmotboard_number = 4;//∏≈Õ∫∏µÂ∞° 3∞≥ ¿÷¥Ÿ 
		COMI.mn_totalmotoraxis_number= 21; //∏≈Õ¿« √— ºˆ∑Æ¿∫ 21∞≥¿Ã¥Ÿ  
		COMI.mn_bd_axis_number[0] = 8;//0π¯ ∏≈Õ∫∏µÂ¥¬ 8√‡øÎ ∫∏µÂ¿Ã¥Ÿ  
		COMI.mn_bd_axis_number[1] = 8;
		COMI.mn_bd_axis_number[2] = 4;
		COMI.mn_bd_axis_number[3] = 4;
 	}
	else if( n_bd_type == 1 )//1: comizoa
	{
		nRet = COMI.Initialize_MotBoard(s_filename);
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD]Initialize");
			LogFile_Write("C:\\AMT_LOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 4; //motion board -> 4ea
		COMI.mn_totalmotoraxis_number= 21;//motor 21ea
		COMI.mn_bd_axis_number[0] = 8;
		COMI.mn_bd_axis_number[1] = 8;
		COMI.mn_bd_axis_number[2] = 4;
		COMI.mn_bd_axis_number[3] = 4;

		if(COMI.mn_motorbd_init_end == BD_YES)
		{			
			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
			{//n_simul_mode => 0:

				if(COMI.mn_simulation_mode == 1)
				{//ÌôúÏÑ±
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//ÎπÑÌôúÏÑ± 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}
	}

	//0π¯ ∫∏µÂ	Motor No., ±‚æÓ∫Ò (1»∏¿¸Ω√ ∆ﬁΩ∫ :10000 / 1»∏¿¸Ω√ ¿Ãµø∞≈∏Æ)
	// 1mm ¿Ãµø«œ¥¬µ• « ø‰«— pulse
	COMI.Set_MotUnitDist(M_TRAY1_Z, 1000);
	// 10mm ¿Ãµø«œ¥¬µ• « ø‰«— pulse (º”µµ¥¬ 1Cm/√ ∑Œ ºº∆√«œ∞‘ µ»¥Ÿ.)
	COMI.Set_MotUnitSpeed(M_TRAY1_Z, 1000);
	// motor Home
	COMI.mn_homecheck_method[M_TRAY1_Z] = 6;

	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD ∫Ò»∞º∫, cmTRUE = 1 SD »∞º∫ 	
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A¡¢¡° πÊΩƒ,1 (cmLOGIC_B) : B¡¢¡° πÊΩƒ
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_LATCH, cmFALSE);//16
	COMI.Set_MotorType_Init(M_TRAY1_Z, MOT_SERVO);


	
	COMI.Set_MotUnitDist(M_TRAY2_Z,	1000);
	COMI.Set_MotUnitSpeed(M_TRAY2_Z, 1000);
	COMI.mn_homecheck_method[M_TRAY2_Z] = 6;

	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD ∫Ò»∞º∫, cmTRUE = 1 SD »∞º∫ 	
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A¡¢¡° πÊΩƒ,1 (cmLOGIC_B) : B¡¢¡° πÊΩƒ
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_LATCH, cmFALSE);//16
	COMI.Set_MotorType_Init(M_TRAY2_Z, MOT_SERVO);


	COMI.Set_MotUnitDist(M_PRESS_Y,	90.7325748);//110.2
	COMI.Set_MotUnitSpeed(M_PRESS_Y, 90.7325748);//110.2
	COMI.mn_homecheck_method[M_PRESS_Y] = 6;
	COMI.Set_MotorType_Init(M_PRESS_Y, MOT_SERVO);

	COMI.Set_MotUnitDist(M_EPOXY_TRANSFER_X,	90.7325748);
	COMI.Set_MotUnitSpeed(M_EPOXY_TRANSFER_X,	90.7325748);
	COMI.mn_homecheck_method[M_EPOXY_TRANSFER_X] = 6;
	COMI.Set_MotorType_Init(M_EPOXY_TRANSFER_X, MOT_SERVO);


	COMI.Set_MotUnitDist(M_EPOXY_TRANSFER_Y,	90.7325748);//90.7358679//// 110.214
	COMI.Set_MotUnitSpeed(M_EPOXY_TRANSFER_Y,	90.7325748);
	COMI.mn_homecheck_method[M_EPOXY_TRANSFER_Y] = 6;
	COMI.Set_MotorType_Init(M_EPOXY_TRANSFER_Y, MOT_SERVO);


	COMI.Set_MotUnitDist(M_EPOXY_TRANSFER_Z,	1000);
	COMI.Set_MotUnitSpeed(M_EPOXY_TRANSFER_Z,	1000);
	COMI.mn_homecheck_method[M_EPOXY_TRANSFER_Z] = 6;
	COMI.Set_MotorType_Init(M_EPOXY_TRANSFER_Z, MOT_SERVO);


	COMI.Set_MotUnitDist(M_EPOXY_SCREW,			10000);
	COMI.Set_MotUnitSpeed(M_EPOXY_SCREW,		10000);
	COMI.mn_homecheck_method[M_EPOXY_SCREW] = 6;
	COMI.Set_MotorType_Init(M_EPOXY_SCREW, MOT_SERVO);


	COMI.Set_MotUnitDist(M_CARRIER_X,			90.7325748);
	COMI.Set_MotUnitSpeed(M_CARRIER_X,			90.7325748);
	COMI.mn_homecheck_method[M_CARRIER_X] = 6;
	COMI.Set_MotorType_Init(M_CARRIER_X, MOT_SERVO);


	///1π¯ ∫∏µÂ 
	COMI.Set_MotUnitDist(M_LOADER_TRANSFER_Y,	90.7325748); //110.214
	COMI.Set_MotUnitSpeed(M_LOADER_TRANSFER_Y,	90.7325748); //110.214
	COMI.mn_homecheck_method[M_LOADER_TRANSFER_Y] = 6;
	COMI.Set_MotorType_Init(M_LOADER_TRANSFER_Y, MOT_SERVO);

	COMI.Set_MotUnitDist(M_LOADER_TRANSFER_Z,	1000);			//109.9314
	COMI.Set_MotUnitSpeed(M_LOADER_TRANSFER_Z,	1000);			//109.9314
	COMI.mn_homecheck_method[M_LOADER_TRANSFER_Z] = 6;
	COMI.Set_MotorType_Init(M_LOADER_TRANSFER_Z, MOT_SERVO);

	COMI.Set_MotUnitDist(M_HEATSINK_TRANSFER_X,	90.96582);
	COMI.Set_MotUnitSpeed(M_HEATSINK_TRANSFER_X,	90.96582);
	COMI.mn_homecheck_method[M_HEATSINK_TRANSFER_X] = 6;
	COMI.Set_MotorType_Init(M_HEATSINK_TRANSFER_X, MOT_SERVO);

	COMI.Set_MotUnitDist(M_HEATSINK_TRANSFER_Y,	90.96582);
	COMI.Set_MotUnitSpeed(M_HEATSINK_TRANSFER_Y,	90.96582);
	COMI.mn_homecheck_method[M_HEATSINK_TRANSFER_Y] = 6;
	COMI.Set_MotorType_Init(M_HEATSINK_TRANSFER_Y, MOT_SERVO);


	COMI.Set_MotUnitDist(M_HEATSINK_TRANSFER_Z,	1000);
	COMI.Set_MotUnitSpeed(M_HEATSINK_TRANSFER_Z,	1000);
	COMI.mn_homecheck_method[M_HEATSINK_TRANSFER_Z] = 6;
	COMI.Set_MotorType_Init(M_HEATSINK_TRANSFER_Z, MOT_SERVO);

	COMI.Set_MotUnitDist(M_UNLOADER_TRANSFER_X,	90.7325748);
	COMI.Set_MotUnitSpeed(M_UNLOADER_TRANSFER_X,	90.7325748);
	COMI.mn_homecheck_method[M_UNLOADER_TRANSFER_X] = 6;
	COMI.Set_MotorType_Init(M_UNLOADER_TRANSFER_X, MOT_SERVO);

	COMI.Set_MotUnitDist(M_UNLOADER_TRANSFER_Y,	90.7325748);
	COMI.Set_MotUnitSpeed(M_UNLOADER_TRANSFER_Y,	90.7325748);
	COMI.mn_homecheck_method[M_UNLOADER_TRANSFER_Y] = 6;
	COMI.Set_MotorType_Init(M_UNLOADER_TRANSFER_Y, MOT_SERVO);

	COMI.Set_MotUnitDist(M_UNLOADER_TRANSFER_Z,	1000);
	COMI.Set_MotUnitSpeed(M_UNLOADER_TRANSFER_Z,	1000);
	COMI.mn_homecheck_method[M_UNLOADER_TRANSFER_Z] = 6;
	COMI.Set_MotorType_Init(M_UNLOADER_TRANSFER_Z, MOT_SERVO);

	COMI.Set_MotUnitDist(M_DISPENSER_Y,		1094.4032);	//91.374
	COMI.Set_MotUnitSpeed(M_DISPENSER_Y,		1094.4032);	//91.374
	COMI.mn_homecheck_method[M_DISPENSER_Y] = 6;
	COMI.Set_MotorType_Init(M_DISPENSER_Y, MOT_SERVO);

	COMI.Set_MotUnitDist(M_TRAY_REMOVE_X,	90.7325748);
	COMI.Set_MotUnitSpeed(M_TRAY_REMOVE_X,	90.7325748);
	COMI.mn_homecheck_method[M_TRAY_REMOVE_X] = 6;
	COMI.Set_MotorType_Init(M_TRAY_REMOVE_X, MOT_SERVO);


	COMI.Set_MotUnitDist(M_HEATSINK_INSPECT_Y,	90.7325748);
	COMI.Set_MotUnitSpeed(M_HEATSINK_INSPECT_Y,	90.7325748);
	COMI.mn_homecheck_method[M_HEATSINK_INSPECT_Y] = 6;
	COMI.Set_MotorType_Init(M_HEATSINK_INSPECT_Y, MOT_SERVO);


	COMI.Set_MotUnitDist(M_HEATSINK_INSPECT_Z, 1000);//20130716
	COMI.Set_MotUnitSpeed(M_HEATSINK_INSPECT_Z, 1000);//20130716
	COMI.mn_homecheck_method[M_HEATSINK_INSPECT_Z] = 6;
	COMI.Set_MotorType_Init(M_HEATSINK_INSPECT_Z, MOT_SERVO);

	COMI.Set_MotUnitDist(M_HEATSINK_PICKER_PITCH, 500.3411417);
	COMI.Set_MotUnitSpeed(M_HEATSINK_PICKER_PITCH, 500.3411417);
	COMI.mn_homecheck_method[M_HEATSINK_PICKER_PITCH] = 6;
	COMI.Set_MotorType_Init(M_HEATSINK_PICKER_PITCH, MOT_SERVO);


	for (i = 0; i < M_MOTOR_COUNT; i++)
	{
		st_motor[i].d_limit_position[0] = COMI.md_limit_position[i][0];
		st_motor[i].d_limit_position[1] = COMI.md_limit_position[i][1]; 
		
		st_motor[i].md_spd_vel			= COMI.md_spd_vel[i][0]; 
		//2016.1209
		st_motor[i].md_spd_acc			= COMI.md_spd_vel[i][1];
		st_motor[i].md_spd_dec			= COMI.md_spd_vel[i][2];
		
		st_motor[i].md_spd_home			= COMI.md_spd_home[i];
		st_motor[i].md_spd_jog			= COMI.md_spd_jog[i];
		
		st_motor[i].mn_allow			= COMI.md_allow_value[i];
		
		//		COMI.mn_homecheck_method[i]		= 6;	//el on-> stop-> back-> el off - > stop 
		nRet = COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		if (nRet != BD_GOOD)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,_T("Set_HomeSet_Config Error [MotNum[%d]]"), i);
				st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µø¿€ øœ∑· √‚∑¬ ø‰√ª
			}
		}
		
		COMI.Set_MotPower(i, TRUE);		

		COMI.Set_CMD_CTL_Mode(i, cmCTRL_SEMI_C);//20150429 james cmCTRL_SEMI_C , ø£ƒ⁄¥ı,(««µÂπÈ±‚¡ÿ) ±‚¡ÿ¿∏∑Œ ∏Ò«•¡¬«•∏¶ º≥¡§«œø© ¿Ãº€«’¥œ¥Ÿ(ƒø∏‡µÂ ¡§∫∏¥¬ π´Ω√µ«∞Ì √÷¡æ feedback(ø£ƒ⁄¥ı) ¿ßƒ°∑Œ ¡¶æÓ∞° ¿Ã∑ÁæÓ¡¯¥Ÿ.
	}

	return  BD_GOOD;
}

int CCtlBd_Library::Initialize_io_Board(int n_bd_type)
{
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	

	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	if (Ret == FALSE)
	{
		return BD_ERROR;
	}

	FAS_IO.mn_io_board_initial_flag = BD_YES;
	
	return BD_GOOD;
}


int CCtlBd_Library::Motor_LinearSafety(int n_MapIndex, double *dp_PosList)
{
// 	double dCurrX = g_comiMgr.Get_MotCurrentPos( MOTOR_ROBOT_X );
// 	double dCurrY = g_comiMgr.Get_MotCurrentPos( MOTOR_ROBOT_Y );
// 	double dCurrT = g_comiMgr.Get_MotCurrentPos( MOTOR_TRAY_TR_Y );
// 
// 	if( n_MapIndex == M_ROBOT_XY_INDEX )
// 	{
// 		//2016.0410
// 		if(st_handler.mn_mirror_type == CTL_YES )
// 		{
// 			if( dCurrX < st_handler.md_safty_rbtx + COMI.md_allow_value[MOTOR_ROBOT_X] )
// 			{
// 				if( dp_PosList[0] < st_handler.md_safty_rbtx - COMI.md_allow_value[MOTOR_ROBOT_X] )
// 				{
// 					if( dCurrY > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 					{
// 						if( dp_PosList[1] >= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety0] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 							
// 						}
// 						else if( dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 
// 					}
// 					else if( dCurrY <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] )
// 					{
// 						if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_1_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety3_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if(  dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety4] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) && 
// 								dCurrT > ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety5] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{//2016.0410
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety6] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 					{
// 						if(  dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y]) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - 100*COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety10] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}//2016.0414
// // 						else if( dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + 100  )
// // 						{
// // 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// // 							{
// // 								alarm.mstr_code = _T("450002");
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// // 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear XY axis robot move error!" );
// // 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// // 								return BD_ERROR;
// // 							}
// // 						}
// 						else if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) &&
// 							dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety8] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else// if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT])
// 					{					
// 						if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 						{
// 							alarm.mstr_code = _T("450002");
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 							if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety9] Linear XY axis robot move error!" );
// 							CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 							return BD_ERROR;
// 						}					
// 					}
// 				}
// 			}
// 			else// if( dCurrX > st_handler.md_safty_rbtx + COMI.md_allow_value[MOTOR_ROBOT_X] )
// 			{
// 				if( dp_PosList[0] < st_handler.md_safty_rbtx - COMI.md_allow_value[MOTOR_ROBOT_X] )
// 				{
// 					if( dCurrY > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 					{
// 						if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety0_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety1_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety2_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety3_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] )
// 					{
// 						if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_1_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety4_1_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if(  dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety4_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) && 
// 								dCurrT > ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety5_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{//2016.0410
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety6_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 					{
// 						if(  dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y]) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - 100*COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety10_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}//2016.0414
// // 						else if(  dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + 100 )
// // 						{
// // 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// // 							{
// // 								alarm.mstr_code = _T("450002");
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// // 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7_1] Linear XY axis robot move error!" );
// // 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// // 								return BD_ERROR;
// // 							}
// // 						}
// 						else if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) &&
// 							dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety8_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else// if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT])
// 					{					
// 						if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 						{
// 							alarm.mstr_code = _T("450002");
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 							if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety9_1] Linear XY axis robot move error!" );
// 							CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 							return BD_ERROR;
// 						}					
// 					}
// 				}
// 
// 			}
// 
// 		}
// 		else// if(st_handler.mn_mirror_type == CTL_NO)
// 		{
// 			if( dCurrX > st_handler.md_safty_rbtx + COMI.md_allow_value[MOTOR_ROBOT_X] )
// 			{
// 				if( dp_PosList[0] > st_handler.md_safty_rbtx + COMI.md_allow_value[MOTOR_ROBOT_X] )
// 				{
// 					if( dCurrY > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 					{
// 						if( dp_PosList[1] >= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety0] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}								
// 							
// 						}
// 						else if( dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety1_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety2_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety3_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] )
// 					{
// 						if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_2_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety4_2_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if(  dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety4_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) && 
// 								dCurrT > ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety5_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{//2016.0410
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety6_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 					{
// 						if(  dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y]) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - 100*COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety10_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}//20160414
// // 						else if(  dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + 100 ) )
// // 						{
// // 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] && 
// // 								dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y])
// // 							{
// // 								alarm.mstr_code = _T("450002");
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// // 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7_2] Linear XY axis robot move error!" );
// // 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// // 								return BD_ERROR;
// // 							}
// // 						}
// 						else if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) &&
// 							dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety8_2] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else// if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT])
// 					{					
// 						if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 						{
// 							alarm.mstr_code = _T("450002");
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9_2] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 							if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety9_2] Linear XY axis robot move error!" );
// 							CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 							return BD_ERROR;
// 						}					
// 					}
// 				}
// 			}
// 			else// if( dCurrX < st_handler.md_safty_rbtx + COMI.md_allow_value[MOTOR_ROBOT_X] )
// 			{
// 				if( dp_PosList[0] > st_handler.md_safty_rbtx + COMI.md_allow_value[MOTOR_ROBOT_X] )
// 				{
// 					if( dCurrY > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 					{
// 						if( dp_PosList[1] >= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0_1] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety0_1] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 							
// 						}
// 						else if( dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety1_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety2_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety3_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] )
// 					{
// 						if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety0_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] > st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] < ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + 300.0f ) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety4_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) && 
// 								dCurrT > ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety5_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{//2016.0410
// 							if( dCurrT < ( st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] ) )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety6_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] && dCurrY > st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 					{
// 						if(  dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y]) )
// 						{
// 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - 100*COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety10_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}//2016.0414
// // 						else if(  dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + 100 ) )
// // 						{
// // 							if( dCurrT > st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_BIN] + COMI.md_allow_value[MOTOR_TRAY_TR_Y] && 
// // 								dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y])
// // 							{
// // 								alarm.mstr_code = _T("450002");
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// // 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7_3] Linear XY axis robot move error!" );
// // 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// // 								return BD_ERROR;
// // 							}
// // 						}
// 						else if( dp_PosList[1] > ( st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) &&
// 							dp_PosList[1] <= ( st_handler.md_safty_rbty[P_XY_SAFETY_REJ_LIMIT] + COMI.md_allow_value[MOTOR_ROBOT_Y] ) )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_SAFETY] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety7_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 						else// if( dp_PosList[1] <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT] )
// 						{
// 							if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 							{
// 								alarm.mstr_code = _T("450002");
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 								if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety8_3] Linear XY axis robot move error!" );
// 								CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 								return BD_ERROR;
// 							}
// 						}
// 					}
// 					else// if( dCurrY <= st_handler.md_safty_rbty[P_XY_SAFETY_TRAY_LIMIT])
// 					{					
// 						if( dCurrT < st_motor[MOTOR_TRAY_TR_Y].d_pos[P_TRANSFER_Y_REJ] - COMI.md_allow_value[MOTOR_TRAY_TR_Y] )
// 						{
// 							alarm.mstr_code = _T("450002");
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9_3] Linear ∑Œ∫ø X√‡ ¿Ãµø ø°∑Ø - ∆Æ∑£Ω∫∆€ ∞¯±ﬁ ¿ßƒ° æ∆¥‘ !!!" );	
// 							if ( g_local.GetLocalType() == LOCAL_ENG ) CtlBdFunc.ms_ErrMsg.Format("[LSafety9_3] Linear XY axis robot move error!" );
// 							CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, MOTOR_ROBOT_Y, "450002", CtlBdFunc.ms_ErrMsg);
// 							return BD_ERROR;
// 						}					
// 					}
// 				}
// 			}
// 		}
// 	}
 	return BD_GOOD;
}


int CCtlBd_Library::Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos, double d_jogcurrentpos) 	//Î™®ÌÑ∞ ÏïàÏ†Ñ Ï†ïÎ≥¥ ÏÖãÌåÖÏãú ÏÇ¨Ïö© 
{
	// Ïòà) 
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog, 4:Length Change
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet_1 = 0, nRet_2 = 0, nRet_3 = 0, nRet_4 = 0, nRet_5 = 0, nRet_6 = 0;
	double d_CurPos[M_MOTOR_COUNT] = {0,};
	double d_Pos[4] = {0,};
	double d_GapCheck = 0;
	int i = 0, nExyPos = 0;


	if(Func.DoorOpenCheckSpot() != CTLBD_RET_GOOD)
	{
		return CTL_DOOROPEN;
	}

// 	nRet = g_handler.OnLightCurtainCheck();
// 	
// 	if (nRet == BD_ERROR)
// 	{
// 		if (m_nMcEmg == NO)
// 		{
// 			m_nMcEmg = YES;
// 		}
// 		
// 		return BD_ERROR;
// 	}


	nRet = g_handler.OnEmoCheck();
	
	if (nRet == RET_ERROR)
	{
		if (m_nMcEmg == NO)
		{
			m_nMcEmg = YES;
		}
		
		return BD_ERROR;
	}


	if (d_jogcurrentpos != -1000)
	{
		d_CurPos[n_axis] = d_jogcurrentpos;
	}
	// **************************************************************************
	//software limit Í∞í ÏÖãÌåÖ Ï≤¥ÌÅ¨ ÏóêÎü¨ 
	if(d_targetpos < COMI.md_limit_position[n_axis][0] && d_targetpos < d_jogcurrentpos ) //
	{
		//sprintf(cJamcode, "%02d0004", n_axis);
		sprintf(cJamcode, "0%02d010", n_axis);			// 2014.12.16 - Bredmin.
		Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
		
// 		if (st_handler.cwnd_list != NULL)
// 		{  
// 			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ÎèôÏûë Ïã§Ìå® Ï∂úÎ†• ÏöîÏ≤≠
// 		}
		// 2014.11.29 - Bredmin.
		CString strTemp = "";
		strTemp.Format( "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos );
		Func.On_LogFile_Add( LOG_TOTAL, strTemp );

		return BD_ERROR;
	}	

	if(d_targetpos > COMI.md_limit_position[n_axis][1] && d_targetpos > d_jogcurrentpos ) //
	{
		//sprintf(cJamcode, "%02d0004", n_axis);
		sprintf(cJamcode, "0%02d011", n_axis);			// 2014.12.16 - Bredmin.
		Alarm_Error_Occurrence(1, CTL_dWARNING, cJamcode);
		
// 		if (st_handler.cwnd_list != NULL)
// 		{ 
// 			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ÎèôÏûë Ïã§Ìå® Ï∂úÎ†• ÏöîÏ≤≠
// 		}
		// 2014.11.29 - Bredmin.
		CString strTemp = "";
		strTemp.Format( "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos );
		Func.On_LogFile_Add( LOG_TOTAL, strTemp );
		return BD_ERROR;
	}

	if (g_comiMgr.Get_MotPower(n_axis) == CTL_ERROR ) 
	{
		if (COMI.Set_MotPower(n_axis, CTL_ON) == CTLBD_RET_GOOD)
		{
// 			if (st_handler.cwnd_list != NULL)
// 			{  // 
// 				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ÎèôÏûë Ïã§Ìå® Ï∂úÎ†• ÏöîÏ≤≠
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotPower(n_axis) == CTL_ERROR)  // Î™®ÌÑ∞ POWER ÏÉÅÌÉú Í≤ÄÏÇ¨ Ìï®Ïàò 
			{
				if (st_motor[n_axis].n_retry_cnt > (MOT_RTY_CNT))
				{
					//sprintf(cJamcode, "%02d0005", n_axis);
					sprintf(cJamcode, "0%02d004", n_axis);				// 2014.12.16 - Bredmin.
					Alarm_Error_Occurrence(2, CTL_dWARNING, cJamcode);	
					return BD_ERROR;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if (st_motor[n_axis].n_retry_cnt > MOT_RTY_CNT)
			{
				//printf(cJamcode, "%02d0005", n_axis);
				sprintf(cJamcode, "0%02d004", n_axis);				// 2014.12.16 - Bredmin.
				Alarm_Error_Occurrence(3, CTL_dWARNING, cJamcode);		
				return BD_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return BD_RETRY;
			}
		}
	}

	if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR)
	{
		if (COMI.Set_MotAlarmClear(n_axis) == CTL_GOOD)
		{
			//091119 james			Sleep(1000);  //
// 			if (st_handler.cwnd_list != NULL)  //
// 			{
// 				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis] );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR )  // Î™®ÌÑ∞ ALARM ÏÉÅÌÉú Í≤ÄÏÇ¨ Ìï®Ïàò 
			{
				if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
				{			
					//sprintf(cJamcode, "%02d0002",  n_axis); 
					sprintf(cJamcode, "0%02d005",  n_axis);		// 2014.12.16 - Bredmin.
					alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					//sprintf(alarm.mc_code, "%02d0002", n_axis);
					sprintf(alarm.mc_code, "0%02d005", n_axis);		// 2014.12.16 - Bredmin.
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, cJamcode, CtlBdFunc.ms_ErrMsg);
					//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  //
					return BD_RETRY;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
			{
				//sprintf(cJamcode, "%02d0002",  n_axis);		// 2014.12.16 - Bredmin.
				sprintf(cJamcode, "0%02d005",  n_axis); 
				alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
				//sprintf(alarm.mc_code, "%02d0002", n_axis);
				sprintf(alarm.mc_code, "0%02d005", n_axis);		// 2014.12.16 - Bredmin.
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Alarm Return Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, cJamcode, CtlBdFunc.ms_ErrMsg);
				
				//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // ÏïåÎûå Ìï¥Ï†ú ÏãúÎèÑ ÌöüÏàò 
				return BD_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return BD_RETRY;
			}
		}
	}
	// **************************************************************************

	// **************************************************************************
	switch( n_axis )
	{
		case M_HEATSINK_TRANSFER_X:
		case M_HEATSINK_TRANSFER_Y:
			if( ( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_PLACE_TOPPOS] - 5 ) )&& 
				( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y) > (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_BOTPOS] - 5 ) ) )
			{

			}
			else
			{
				if( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Z) > st_motor[M_HEATSINK_TRANSFER_Z].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + 1)
				{
					COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_X);
					COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_Y);
					sprintf(cJamcode, "%02d0008", n_axis);
					Alarm_Error_Occurrence(74, CTL_dWARNING, cJamcode);
					return RET_ERROR;
				}
			}

			if( n_axis == M_HEATSINK_TRANSFER_X )
			{
				if( d_targetpos > ( st_motor[n_axis].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + COMI.md_allow_value[n_axis] ) )
				{
					//æ»¿¸¿ßƒ°∫∏¥Ÿ 20mm≥ª∑¡ø¿∏È √Êµπ¿ßƒ°
					if( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y)  < ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - 20 ) )
					{
						//if( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X)  < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] + 10 ) )
						//2017.0616
						if( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X)  > ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - 100 ) )
						{
							COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_X);
							COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_Y);
							sprintf(cJamcode, "%02d0008", n_axis);
							Alarm_Error_Occurrence(74, CTL_dWARNING, cJamcode);
							return RET_ERROR;
						}
					}
				}
			}
			if( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + 5 ) )
			{
				if( ( COMI.Get_MotCurrentPos(M_HEATSINK_INSPECT_Y)  > ( st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + 50*COMI.md_allow_value[n_axis] ) ) )
				{
					COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_X);
					COMI.Set_MotStop( 0, M_HEATSINK_TRANSFER_Y);
					sprintf(cJamcode, "%02d0008", M_HEATSINK_INSPECT_Y);
					Alarm_Error_Occurrence(75, CTL_dWARNING, cJamcode);
					return RET_ERROR;
				}
			}

			if( n_axis == M_HEATSINK_TRANSFER_Y )
			{
				if( d_targetpos == st_motor[n_axis].md_pos[P_HEATSINK_TRASNFER_Y_TURN_PLACE_POS] ||
					d_targetpos == st_motor[n_axis].md_pos[P_HEATSINK_TRASNFER_Y_TURN_PICK_POS] ||
					d_targetpos == st_motor[n_axis].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] ||
					d_targetpos == st_motor[n_axis].md_pos[P_HEATSINK_TRASNFER_Y_DISPENSER_POS] )
				{
					if( COMI.Get_MotCurrentPos( M_DISPENSER_Y) > ( st_motor[M_DISPENSER_Y].md_pos[P_DISPENSOR_Y_INIT_POS] + 5) )
					{
						COMI.Set_MotStop( 0, n_axis);
						sprintf(cJamcode, "%02d0008", M_DISPENSER_Y);
						Alarm_Error_Occurrence(64, CTL_dWARNING, cJamcode);
						return RET_ERROR;
					}
				}
			}
			break;
		
		case M_HEATSINK_INSPECT_Y:
// 			if( g_ioMgr.get_in_bit(st_io.i_camera_unclamp_chk, IO_ON) == IO_OFF ||
// 				g_ioMgr.get_in_bit(st_io.i_camera_clamp_chk, IO_OFF) == IO_OFF )
// 			{
// 				COMI.Set_MotStop( 0, n_axis);
// 				sprintf(cJamcode, "%02d0008", n_axis);
// 				Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
// 				return RET_ERROR;
// 			}

			if( ( d_targetpos > ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + 100*COMI.md_allow_value[n_axis] ) ) ||
				( COMI.Get_MotCurrentPos(n_axis)  > ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + 100*COMI.md_allow_value[n_axis] ) ) )
			{
				if( ( COMI.Get_MotCurrentPos(n_axis) < ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_PRESS_START_POS] + COMI.md_allow_value[n_axis] ) ) &&
					( COMI.Get_MotCurrentPos(n_axis) > ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_PRESS_END_POS] - COMI.md_allow_value[n_axis] ) ) )
				{
				}
				else if( ( COMI.Get_MotCurrentPos(n_axis) < ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_PRESS_START_POS] + st_recipe.dLoaderTransferTrayDeviceGap + COMI.md_allow_value[n_axis] ) ) &&
					( COMI.Get_MotCurrentPos(n_axis) > ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_PRESS_END_POS] + st_recipe.dLoaderTransferTrayDeviceGap - COMI.md_allow_value[n_axis] ) ) )
				{
				}
				else if( ( COMI.Get_MotCurrentPos(n_axis) < ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_PRESS_START_POS] + 2*st_recipe.dLoaderTransferTrayDeviceGap + COMI.md_allow_value[n_axis] ) ) &&
					( COMI.Get_MotCurrentPos(n_axis) > ( st_motor[n_axis].md_pos[P_HEATSINK_INSPECT_Y_PRESS_END_POS] + 2*st_recipe.dLoaderTransferTrayDeviceGap - COMI.md_allow_value[n_axis] ) ) )
				{
				}				
				else if( g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, IO_ON) == IO_ON ||
					g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, IO_OFF) == IO_OFF )
				{
					COMI.Set_MotStop( 0, n_axis);
					sprintf(cJamcode, "%02d0008", n_axis);
					Alarm_Error_Occurrence(4, CTL_dWARNING, cJamcode);
					return RET_ERROR;
				}
			}
			if( COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + 10*COMI.md_allow_value[M_HEATSINK_TRANSFER_Y]) )
			{
				COMI.Set_MotStop( 0, n_axis);
				sprintf(cJamcode, "%02d0008", n_axis);
				Alarm_Error_Occurrence(5, CTL_dWARNING, cJamcode);
				return RET_ERROR;
			}
			if( COMI.Get_MotCurrentPos( M_HEATSINK_INSPECT_Z) > ( st_motor[M_HEATSINK_INSPECT_Z].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Z] ) )
			{
				COMI.Set_MotStop( 0, n_axis);
				sprintf(cJamcode, "%02d0008", n_axis);
				Alarm_Error_Occurrence(67, CTL_dWARNING, cJamcode);
				return RET_ERROR;
			}
			break;

		case M_LOADER_TRANSFER_Y:
			if( d_targetpos > st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] ||
				( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) >= ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) ) )
			{
				if( COMI.Get_MotCurrentPos(M_CARRIER_X)  <= ( st_motor[n_axis].md_pos[P_CARRIER_X_INIT_POS] - COMI.md_allow_value[M_CARRIER_X] ) )
				{
					COMI.Set_MotStop( 0, M_LOADER_TRANSFER_Y);
					sprintf(cJamcode, "%02d0008", n_axis);
					Alarm_Error_Occurrence(51, CTL_dWARNING, cJamcode);
					return RET_ERROR;
				}
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Z ) > (st_motor[M_LOADER_TRANSFER_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Z]))
			{
				COMI.Set_MotStop( 0, M_LOADER_TRANSFER_Y);
				return RET_ERROR;
			}
			break;
		
		case M_CARRIER_X:
			if( d_targetpos == st_motor[M_LOADER_TRANSFER_Y].md_pos[P_CARRIER_X_PUSH_POS] )
			{				
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);		// Left ≥°¥‹ø°º≠ Jig ∞…æÓ¡‹ 
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, IO_OFF);
				if( nRet_1 == IO_OFF || nRet_2 == IO_ON)
				{//st_io.i_Carrier_1_Forward_Check					=	400;
					CTL_Lib.Alarm_Error_Occurrence(59, dWARNING, "810400");
					COMI.Set_MotStop( 0, M_CARRIER_X);
					return RET_ERROR;
				}
				if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) >= ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
				{
					sprintf(cJamcode, "%02d0008", M_EPOXY_TRANSFER_Y);
					Alarm_Error_Occurrence(53, CTL_dWARNING, cJamcode);
					COMI.Set_MotStop( 0, M_CARRIER_X);
					return RET_ERROR;
				}			

			}
			if( d_targetpos != st_motor[n_axis].md_pos[P_CARRIER_X_INIT_POS])
			{
				if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) >= ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) )
				{
					sprintf(cJamcode, "%02d0008", M_LOADER_TRANSFER_Y);
					Alarm_Error_Occurrence(52, CTL_dWARNING, cJamcode);
					COMI.Set_MotStop( 0, M_CARRIER_X);
					return RET_ERROR;
				}
// 				if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) >= ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
// 				{
// 					sprintf(cJamcode, "%02d0008", M_EPOXY_TRANSFER_Y);
// 					Alarm_Error_Occurrence(53, CTL_dWARNING, cJamcode);
// 					COMI.Set_MotStop( 0, M_CARRIER_X);
// 					return RET_ERROR;
// 				}

			}
// 			P_CARRIER_X_INIT_POS = 0,
// 			P_CARRIER_X_PUSHER_DOWN,
// 			P_CARRIER_X_PUSH_POS,//«—π¯ πŒ ∞≈∏Æ
// 			P_CARRIER_X_BACK_PUSH_POS,
// 			P_CARRIER_X_PRESS_POS,//¿·±Ò µ⁄∑Œ ñA ∞≈∏Æ
// 			P_CARRIER_X_UNPRESS_POS,/
			if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_PUSHER_DOWN])
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_ON || nRet_4 != IO_OFF )
				{
					if	  ( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);
					else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
					else if( nRet_3 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Up_Check);
					else						alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(100, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			else if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_PUSH_POS])
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_OFF);
				nRet_4 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);
				nRet_5 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_6 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_OFF || nRet_4 != IO_ON || nRet_5 != IO_ON || nRet_6 != IO_OFF )
				{
					if		( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Carrier_Holder_Up_Check);
					else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
					else if( nRet_3 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Camera_Y_Jig_Press_Forward_Check);
					else if( nRet_4 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Camera_Y_Jig_Press_Backward_Check);
					else if( nRet_5 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check);
					else if( nRet_6 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Down_Check);
					else								alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Carrier_Holder_Up_Check);//ÌòπÏãúÎÇò
					CTL_Lib.Alarm_Error_Occurrence(101, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			else if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_BACK_PUSH_POS] )
			{
// 				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
// 				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
// 				if( nRet_1 != IO_ON || nRet_2 != IO_OFF )
// 				{
// 					if( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check);
// 					else/* if( nRet_2 != IO_OFF )*/ alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Down_Check);
// 					CTL_Lib.Alarm_Error_Occurrence(102, dWARNING, alarm.mstr_code);
// 					return CTL_ERROR;
// 				}
			}
			else if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_PRESS_POS] )
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF)
				{
					if ( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);
					else				   alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Down_Check);					
					CTL_Lib.Alarm_Error_Occurrence(103, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			else if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_UNPRESS_POS] )
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_ON || nRet_4 != IO_OFF )
				{
					if	  ( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);
					else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
					else if( nRet_3 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Up_Check);
					else						alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Down_Check);					
					CTL_Lib.Alarm_Error_Occurrence(104, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}	
			}			
			else
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_ON || nRet_4 != IO_OFF)
				{
					if      ( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);
					else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
					else if( nRet_3 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check);
					else if( nRet_4 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Down_Check);
					else								 alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);
					CTL_Lib.Alarm_Error_Occurrence(105, dWARNING, alarm.mstr_code);
					return CTL_ERROR;

				}
			}
			break;

		case M_PRESS_Y://2017.0415
			if( n_mode == 0 )
			{
				if( COMI.Get_MotCurrentPos(M_PRESS_Y) >= ( st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_INIT_POS] + 10*COMI.md_allow_value[M_PRESS_Y] ) )
				{
					CTL_Lib.Alarm_Error_Occurrence(106, dWARNING, "020008");
					return CTL_ERROR;
				}

			}
			else
			{
				if( ( d_targetpos > ( st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[M_PRESS_Y] ) ) || 
					( COMI.Get_MotCurrentPos(M_PRESS_Y) >= ( st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[M_PRESS_Y] ) ) )
				{
					// 				if( g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_OFF) == IO_ON || 
					// 					g_ioMgr.get_in_bit( st_io.i_Press_Down_Check , IO_ON) == IO_OFF )
					// 				{
					// 					alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check);
					// 					CTL_Lib.Alarm_Error_Occurrence(107, dWARNING, alarm.mstr_code);
					// 					return CTL_ERROR;
					// 				}
					
				}
				if( COMI.Get_MotCurrentPos( M_CARRIER_X) >= ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_PRESS_POS] - COMI.md_allow_value[M_CARRIER_X] ) &&
					COMI.Get_MotCurrentPos( M_CARRIER_X) <= ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_PRESS_POS] + COMI.md_allow_value[M_CARRIER_X] ) ) 
				{
					
				}
				else
				{
					if( COMI.Get_MotCurrentPos( M_CARRIER_X) >= ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[M_CARRIER_X] ) &&
						COMI.Get_MotCurrentPos( M_CARRIER_X) <= ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[M_CARRIER_X] ) ) 
					{
					}
					else
					{
						//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
						CTL_Lib.Alarm_Error_Occurrence(108, dWARNING, "020008");
						return CTL_ERROR;
					}
				}			
			}
			break;

		case M_EPOXY_TRANSFER_X:
		case M_EPOXY_TRANSFER_Y:
			if( n_mode == 3) break;//√ ±‚»≠
			if( st_work.mn_run_status == dINIT ) break;
			if( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Z) > ( st_motor[M_EPOXY_TRANSFER_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Z] ) )
			{
				if( ( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X) > (st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_FIRST_START_POS] - (10*COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ) ) &&
					( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X) < (st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_FIRST_END_POS] + (10*COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ) ) )
				{
				}
				else
				{
					COMI.Set_MotStop( 0, M_EPOXY_TRANSFER_X);
					COMI.Set_MotStop( 0, M_EPOXY_TRANSFER_Y);
					sprintf(cJamcode, "%02d0008", n_axis);
					Alarm_Error_Occurrence(109, CTL_dWARNING, cJamcode);
					return RET_ERROR;
				}
				for( nExyPos = 0; nExyPos < 3; nExyPos++ )
				{
					if( ( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y) < ( (nExyPos*st_recipe.dLoaderTransferTrayDeviceGap) + st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_FIRST_START_POS] + (10*COMI.md_allow_value[M_EPOXY_TRANSFER_Y]) ) ) &&
						( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y) > ( (nExyPos*st_recipe.dLoaderTransferTrayDeviceGap) + st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_FIRST_END_POS] - (10*COMI.md_allow_value[M_EPOXY_TRANSFER_Y]) ) ))
					{
					}
					else
					{
// 						COMI.Set_MotStop( 0, n_axis);
// 						sprintf(cJamcode, "%02d0008", n_axis);
// 						Alarm_Error_Occurrence(110, CTL_dWARNING, cJamcode);
// 						return RET_ERROR;
					}
				}				
			}
			else if( n_axis == M_EPOXY_TRANSFER_X )
			{
				if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) >= ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) ) 
				{
					//if( COMI.Get_MotCurrentPos(M_CARRIER_X)  <= ( st_motor[n_axis].md_pos[P_CARRIER_X_INIT_POS] - COMI.md_allow_value[M_CARRIER_X] ) )
					if( COMI.Get_MotCurrentPos(M_CARRIER_X)  > ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] + (1+COMI.md_allow_value[M_CARRIER_X]) ) )
					{
						COMI.Set_MotStop( 0, M_EPOXY_TRANSFER_X);
						return RET_ERROR;
					}
				}			
				if(COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) >= ( st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + (5* COMI.md_allow_value[M_HEATSINK_TRANSFER_X]) ) )
				{
					if( ( d_targetpos >= ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] + 10 ) ) &&
						( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) < ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - (20* COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) ) ) )
					{
						COMI.Set_MotStop( 0, n_axis);
						sprintf(cJamcode, "%02d0008", n_axis);
						Alarm_Error_Occurrence(110, CTL_dWARNING, cJamcode);
						return RET_ERROR;
					}
				}

			}
			else if( n_axis == M_EPOXY_TRANSFER_Y )
			{
				if( d_targetpos > st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] ||
					( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) >= ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) ) )
				{
					//if( COMI.Get_MotCurrentPos(M_CARRIER_X)  <= ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] - COMI.md_allow_value[M_CARRIER_X] ) )
					if( COMI.Get_MotCurrentPos(M_CARRIER_X)  > ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] + COMI.md_allow_value[M_CARRIER_X] ) )
					{
						COMI.Set_MotStop( 0, M_EPOXY_TRANSFER_Y);
						return RET_ERROR;
					}
				}

				if( d_targetpos < ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - (20*COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) ) )
				{
					if( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - ( 20*COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ) )
					{
						COMI.Set_MotStop( 0, n_axis);
						sprintf(cJamcode, "%02d0008", n_axis);
						Alarm_Error_Occurrence(191, CTL_dWARNING, cJamcode);
						return RET_ERROR;
					}
					else if( COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X) > ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] + ( 10*COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ) ) 
					{
						if( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) >= ( st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + (5* COMI.md_allow_value[M_HEATSINK_TRANSFER_X]) ) )
						{
							COMI.Set_MotStop( 0, n_axis);
							sprintf(cJamcode, "%02d0008", n_axis);
							Alarm_Error_Occurrence(180, CTL_dWARNING, cJamcode);
							return RET_ERROR;
						}
					}
				}
			}
			break;

		case M_DISPENSER_Y:
			if( ( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + COMI.md_allow_value[M_HEATSINK_TRANSFER_X]) ) &&
				( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y) < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - COMI.md_allow_value[M_HEATSINK_TRANSFER_Y]) ) )
			{
				sprintf(cJamcode, "%02d0008", n_axis);
				Alarm_Error_Occurrence(111, CTL_dWARNING, cJamcode);
				return RET_ERROR;
			}
			break;

		case M_EPOXY_TRANSFER_Z:
			if(st_work.nEpoxyBiliardThreadRunMode == 1)
			{
				d_CurPos[0]		= COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y);

				//Let's check manual teaching pos
// 				if(st_work.nJigEpoxyWorkCount == 0)
// 				{
// 					if(d_CurPos[0] > (st_epoxy_pos.dEpoxy_Y_FirstStartPos + st_basic.dEpoxyYLineOffSet + 2.) && 
// 						d_CurPos[0] < (st_epoxy_pos.dEpoxy_Y_FirstEndPos + st_basic.dEpoxyYLineOffSet - 2.))
// 					{
// 						return CTLBD_RET_ERROR;
// 					}	
// 				}
// 
// 				if(st_work.nJigEpoxyWorkCount == 1)
// 				{
// 					if(d_CurPos[0] > (st_epoxy_pos.dEpoxy_Y_FirstStartPos + st_basic.dEpoxyYLineOffSet - st_recipe.dLoaderTransferTrayDeviceGap + 2.) && 
// 						d_CurPos[0] < (st_epoxy_pos.dEpoxy_Y_FirstEndPos + st_basic.dEpoxyYLineOffSet - st_recipe.dLoaderTransferTrayDeviceGap - 2.))
// 					{
// 						return CTLBD_RET_ERROR;
// 					}	
// 				}
// 
// 				if(st_work.nJigEpoxyWorkCount == 2)
// 				{
// 					if(d_CurPos[0] > (st_epoxy_pos.dEpoxy_Y_FirstStartPos + st_basic.dEpoxyYLineOffSet - st_recipe.dLoaderTransferTrayDeviceGap*2 + 2.) && 
// 						d_CurPos[0] < (st_epoxy_pos.dEpoxy_Y_FirstEndPos + st_basic.dEpoxyYLineOffSet - st_recipe.dLoaderTransferTrayDeviceGap*2 - 2.))
// 					{
// 						return CTLBD_RET_ERROR;
// 					}	
// 				}

			}
			break;

		case M_UNLOADER_TRANSFER_X:
		case M_UNLOADER_TRANSFER_Y:
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_Z ) > (st_motor[M_UNLOADER_TRANSFER_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_Z]))
			{
				COMI.Set_MotStop( 0, M_UNLOADER_TRANSFER_X);
				COMI.Set_MotStop( 0, M_UNLOADER_TRANSFER_Y);
				COMI.Set_MotStop( 0, n_axis);
				sprintf(cJamcode, "%02d0008", n_axis);
				Alarm_Error_Occurrence(68, CTL_dWARNING, cJamcode);
				return RET_ERROR;
			}
			break;

		case M_UNLOADER_TRANSFER_Z:
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X ) > (st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_ZIGPLACE_POS] - COMI.md_allow_value[M_UNLOADER_TRANSFER_X]) && 
				COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X ) < (st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_ZIGPLACE_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_X]) )
			{
				if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_Y ) > (st_motor[M_UNLOADER_TRANSFER_Y].md_pos[P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS] - COMI.md_allow_value[M_UNLOADER_TRANSFER_Y]) && 
					COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_Y ) < (st_motor[M_UNLOADER_TRANSFER_Y].md_pos[P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_Y]) )
				{
					if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_Z ) > (st_motor[M_UNLOADER_TRANSFER_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_Z]))
					{
						//2017.0614 Case Assembly¡ˆ±◊ µø¿€±◊∏∏ 
						g_ioMgr.set_out_bit(st_io.o_Interface_Output_1, IO_OFF);
					}
				}
			}
			break;

	}

	st_motor[n_axis].mn_retry_time_flag = CTL_NO;//2011.0201 Ï∂îÍ∞Ä 

	return BD_GOOD;
}

BOOL CCtlBd_Library::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	CString strTemp;
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

int CCtlBd_Library::FileSizeCheck(char * cp_filename, int n_size, int n_check)
{
	CFileFind finder;
	long nLen=0, nflag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(cp_filename))
	{	
		if (n_check == BD_YES)
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			if(nLen > n_size)
			{
				nflag = 1;
			}
			finder.Close();	
		}
		else
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			
			nflag = nLen;
			
			finder.Close();	
		}
	}
	
	return nflag;
}

int CCtlBd_Library::LogFile_Write(char * cp_dir_path, char * cp_msg)
{
//2011.0417	sing.Lock(); //2011.0417 

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour;
	CString mstr_file_name, mstr_dir_path, str_msg;
	CString mstr_create_file;
	CString mstr_list_name, mstr_temp_data;
	CString mstr_content;
	COleDateTime time_cur;
	CTime m_time_current;
	char chr_buf[20]={0,};
	int mn_existence, nlength;
	char cfileName[256]={0,};
	FILE  *fp ;

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0)
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();
	m_time_current = CTime::GetCurrentTime() ;
	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// ÌòÑÏû¨ ÏãúÍ∞Ñ Ï†ïÎ≥¥ ÏñªÎäîÎã§                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();
	mstr_cur_hour.Format("%d",time_cur.GetHour());
	mstr_cur_min.Format("%d",time_cur.GetMinute());
	mstr_cur_sec.Format("%d",time_cur.GetSecond());
	str_display_time = m_time_current.Format("%c");
	// **************************************************************************


	mstr_dir_path = cp_dir_path;
	mstr_file_name = mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = mstr_dir_path + mstr_file_name;

	mstr_create_file += ".TXT";

	sprintf(cfileName, "%s", mstr_create_file);
	mn_existence = access(cfileName,0) ;	
	if (mn_existence == -1)
	{
		CreateFolder(cp_dir_path); 
	}
	else 
	{
	}
	
	Ret = FileSizeCheck(cfileName, 1048576, BD_YES); //size and rename	
	if(Ret == 1) //file size over
	{
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		//AfxMessageBox("File open failure!..");
		sprintf(mc_normal_msg,"LogFile_Write a+ Error"); 
		
 		return BD_ERROR;
	}
	// **************************************************************************
	
	mstr_content += str_display_time;
	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content) ;
	if (ferror(fp))  
	{
		sprintf(mc_normal_msg,"LogFile_Write ferror return error");

		clearerr(fp); //2011.0418 

		fclose(fp) ;  //2011.0417

//2011.0417		sing.Unlock(); //2011.0417
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // 

//2011.0417	sing.Unlock(); //2011.0417 
	return BD_GOOD;
}

int CCtlBd_Library::Single_Move(int n_RunMethod, int n_MotNum, double d_MovePos, int n_SpeedRate) 
{
	int nFuncRet = BD_PROCEED;
	long dwMotionDone=0;
	int nRet_1;
	
	
	switch(mn_single_motmove_step[n_MotNum])
	{
	case 0:
		if(n_RunMethod == ONLY_MOVE_CHECK)
		{
			mn_single_motmove_step[n_MotNum] = 200;
			break;
		}

		nRet_1 = COMI.Check_MotPosRange(n_MotNum, d_MovePos, COMI.md_allow_value[n_MotNum]);
		if (nRet_1 == BD_GOOD)
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_mot_retry_cnt[n_MotNum] = 0;
		mn_mot_max_retry_cnt = 3; 
		mn_single_motmove_step[n_MotNum] = 100;
		m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		break;

	case 10:
		if(m_dwUntil_Wait[n_MotNum][0]  < 0) m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][1] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][2] = m_dwUntil_Wait[n_MotNum][1] - m_dwUntil_Wait[n_MotNum][0];

		nRet_1 = COMI.Check_Motion_State(n_MotNum, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotNum, &dwMotionDone);
		if (nRet_1 == BD_GOOD) 
		{//
			mn_single_motmove_step[n_MotNum] = 100;
		}
		else if(nRet_1 == BD_PROCEED)
		{ 
			if(m_dwUntil_Wait[n_MotNum][2] > 15000) //max 10sec wait
			{
				if (st_handler.cwnd_list != NULL)  
				{
					sprintf(st_msg.c_normal_msg, "CCtlBd_Library::Single_Move = 10 1");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µø¿€ Ω«∆– √‚∑¬ ø‰√ª
				}
				cmmSxStopEmg(n_MotNum);	
			
				mn_single_motmove_step[n_MotNum] = 100;
			} 
		}
		else if(nRet_1 == BD_ERROR)//
		{  
			if(m_dwUntil_Wait[n_MotNum][2] > 200)
			{
				if (st_handler.cwnd_list != NULL)  
				{
					sprintf(st_msg.c_normal_msg, "CCtlBd_Library::Single_Move = 10 2");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µø¿€ Ω«∆– √‚∑¬ ø‰√ª
				}
	
				cmmSxStopEmg(n_MotNum);	

				mn_single_motmove_step[n_MotNum] = 0;
			}
		}
		break;
		
	case 100:
		nRet_1 = COMI.Start_SingleMove(n_MotNum, d_MovePos, n_SpeedRate);  // Ìï¥Îãπ ÏúÑÏπòÎ°ú Ïù¥Îèô
		
		if (nRet_1 == BD_GOOD)
		{
			if(n_RunMethod == ONLY_MOVE_START)
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_GOOD;
				break;
			}
			else
			{
				mn_single_motmove_step[n_MotNum] = 200;
			}
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29898;
				nFuncRet = nRet_1;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet_1 == BD_RETRY)  // ÎèôÏûë Ïû¨ÏãúÎèÑ
		{
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29899;
				nFuncRet = BD_ERROR;
			}
			else 
			{
				if( n_MotNum == M_HEATSINK_PICKER_PITCH )
				{
					COMI.Set_MotPower(n_MotNum, IO_OFF);
					m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
					mn_single_motmove_step[n_MotNum] = 110;
				}
				else
				{
					mn_single_motmove_step[n_MotNum] = 100;
				}
			}
		}
		break;

	case 110:
		m_dwUntil_Wait[n_MotNum][1] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][2] = m_dwUntil_Wait[n_MotNum][1] - m_dwUntil_Wait[n_MotNum][0]; 
		if( m_dwUntil_Wait[n_MotNum][2] <= 0 ) m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		if( m_dwUntil_Wait[n_MotNum][2] > 500)
		{
			mn_single_motmove_step[n_MotNum] = 100;
		}
		break;
		
	case 200:
		if(n_RunMethod == ONLY_MOVE_START)
		{
			mn_single_motmove_step[n_MotNum] = 0;
			break;
		}

		nRet_1 = COMI.Check_SingleMove(n_MotNum, d_MovePos);
		if (nRet_1 == BD_GOOD)
		{
			mn_mot_retry_cnt[n_MotNum] = 0;
			mn_single_motmove_step[n_MotNum] = 0;
			nFuncRet = nRet_1;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29901;
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = nRet_1;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29902;
				mn_single_motmove_step[n_MotNum] = 0;
				mn_mot_retry_cnt[n_MotNum] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
	}
	return nFuncRet;
}

int CCtlBd_Library::Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio)
{
	
	int nFuncRet = BD_PROCEED;
	int nRet[4] = {0,}, nCnt =0, i=0;
	
	switch(mn_linear_motmove_step[n_LinearMapIndex])
	{
	case 0:
		
		nCnt = 0;
		st_linearmot[n_LinearMapIndex].l_AxisCnt = l_AxisCnt;
		for(i=0; i<l_AxisCnt; i++)
		{		
			st_linearmot[n_LinearMapIndex].lp_AxisNum[i] = lp_AxisNum[i]; 
			st_linearmot[n_LinearMapIndex].dp_PosList[i] = dp_PosList[i];

			nRet[i] = COMI.Check_MotPosRange(lp_AxisNum[i], dp_PosList[i], COMI.md_allow_value[lp_AxisNum[i]]);
			if(nRet[i] == BD_GOOD) nCnt++;
		}

		if(nCnt == l_AxisCnt)
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3;
		mn_linear_motmove_step[n_LinearMapIndex] = 100;
		break;
		
	case 100:
		nRet[0] = COMI.Start_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList, dp_SpdRatio);
		if (nRet[0] == BD_GOOD)
		{
/*			CString strData1="";
			CString strData2="";
			
			m_vecStLogData_Add1.clear();
			m_vecStLogData_Add2.clear();

			strData1 = "TYPE";
			strData2 = "MOTOR";
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "ACT_NAME";
			strData2 = Func.Get_MotorName( n_LinearMapIndex, 1 );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[0] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[0] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );

			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[1] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[1] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			st_standard_log.strDeviceID		= Func.Get_MotorName( n_LinearMapIndex );
			st_standard_log.iLogType		= LOG_FNC;
			st_standard_log.strEventID.Format( "MOVE_%s_%s", Func.GetMotorPos( lp_AxisNum[0] ), Func.GetMotorPos( lp_AxisNum[0], dp_PosList[0] ) );
			strStLogStartLinearData[n_LinearMapIndex] = st_standard_log.strEventID;
			st_standard_log.strStatus		= "START";
			st_standard_log.strMatrialID	= "MODULE";
			st_standard_log.strMatrialType	= "MODULE_TYPE";
			
			Func.OnStandardLogWrite( st_standard_log, m_vecStLogData_Add1, m_vecStLogData_Add2 );
*/
			mn_linear_motmove_step[n_LinearMapIndex] = 200;
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)
		{
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		break;
		
	case 200:
		nRet[0] = COMI.Check_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList) ;
		if (nRet[0] == BD_GOOD)
		{
/*			CString strData1="";
			CString strData2="";
			
			m_vecStLogData_Add1.clear();
			m_vecStLogData_Add2.clear();

			strData1 = "TYPE";
			strData2 = "MOTOR";
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "ACT_NAME";
			strData2 = Func.Get_MotorName( n_LinearMapIndex, 1 );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[0] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[0] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			strData1 = "POS_AXIS_" + Func.Get_MotorName( lp_AxisNum[1] );
			strData2.Format( "%.3f", g_comiMgr.Get_MotCurrentPos( lp_AxisNum[1] ) );
			m_vecStLogData_Add1.push_back( strData1 );
			m_vecStLogData_Add2.push_back( strData2 );
			
			st_standard_log.strDeviceID		= Func.Get_MotorName( n_LinearMapIndex );
			st_standard_log.iLogType		= LOG_FNC;
			
			st_standard_log.strEventID = strStLogStartLinearData[n_LinearMapIndex];
			st_standard_log.strStatus		= "END";
			st_standard_log.strMatrialID	= "MODULE";
			st_standard_log.strMatrialType	= "MODULE_TYPE";
			
			Func.OnStandardLogWrite( st_standard_log, m_vecStLogData_Add1, m_vecStLogData_Add2 );
*/
			mn_linear_motmove_step[n_LinearMapIndex] = 0;
			nFuncRet = nRet[0];
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}	
		break;
	}
	return nFuncRet;
}


int CCtlBd_Library::SD_Sensor_Enable(int n_Mode, int n_AxisNum, int n_Enable)
{//Constant speed mode
	int nFuncRet = BD_PROCEED;

	if(n_Enable == CTL_YES) 
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmTRUE);  
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmFALSE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmTRUE);

		st_motor[n_AxisNum].n_sd_mv_chk = 1; //sd
	}
	else
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  
		//COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmTRUE);
		//COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmFALSE);
		st_motor[n_AxisNum].n_sd_mv_chk = 0;
	}

	return BD_GOOD;
}


int CCtlBd_Library::Elevator_Job_Move_Pos(int nMode, int n_AxisNum,  int n_TargetPos_Site)
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	double dCurrentPos;
	CString strlog;
	//	double dPos_1, dPos_2, dPos_3;

	Func.ThreadFunctionStepTrace(41+n_AxisNum, m_nSD_Elv_MoveStep[n_AxisNum]);


	//LIGHT CURTAIN
	if( st_sync.nLdUld_LightChk == YES )
	{
		if( n_AxisNum == M_TRAY1_Z || n_AxisNum == M_TRAY2_Z )
		{
			cmmSxStop(n_AxisNum, FALSE, FALSE); 
			return BD_RETRY;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	if(st_work.mn_run_status != dRUN)
	{
		if(m_bSD_MoveFlag[n_AxisNum] == true) m_bSD_MoveFlag[n_AxisNum] = false; 
	}

	switch(m_nSD_Elv_MoveStep[n_AxisNum])
	{
	case 0:	
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET || n_TargetPos_Site == P_ELV_RECEIVE_OFFSET)//Ìä∏Î†àÏù¥Î•º ÏûëÏóÖ Í∞ÄÎä•Ìïú ÏòÅÏó≠Ïóê UPÌïòÏó¨ Í≥µÍ∏âÌïòÎäî ÏúÑÏπò, SD pos ÏÑºÏÑú On Í∞êÏßÄ ÌõÑ ÏÑºÏÑú Í∏∞Ï§Ä - Î∞©Ìï≠ÏúºÎ°ú Î≤óÏñ¥ÎÇú ÌõÑ + Î∞©Ìñ•ÏúºÎ°ú P_ELV_SUPPLY_OFFSET Ìã∞Ïπ≠ÎßåÌÅº up ÌïúÌõÑ Î™®ÌÑ∞ ÎèôÏûëÏùÄ ÏôÑÎ£åÌïòÍ≥†, Ìä∏Î†àÏù¥Î•¥, Î∞õÎì†Îã§ 
		{
			st_sync.n_barcode_read_serial_num[0][0] = 0;
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}			
		else 
		{//910004 1 A "THERE_IS_NO_POSITION_THAT ELV MOVE_IN_LOAD_STACKER."
			CTL_Lib.Alarm_Error_Occurrence(201, dWARNING, "910004");
		}			
		break;
		///////////////////////////////////////////////////////////////////////////
	case 1000:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 	
		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear			

		if(nRet_1 == BD_GOOD)
		{
			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 6000;
		}
		else //if(nRet_1 == BD_ERROR)
		{
			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 2000; //traget pos
		}	 
		break;

		////////////////////////////////////////////////////////////
	case 2000: 
		if(m_bSD_MoveFlag[n_AxisNum] == false)
		{
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			m_bSD_MoveFlag[n_AxisNum] = true;
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_YES); //sd sensor set
		}
		else if(m_bSD_MoveFlag[n_AxisNum] == true)
		{
			m_dwSdWaitTime[n_AxisNum][1] = GetTickCount();
			m_dwSdWaitTime[n_AxisNum][2] = m_dwSdWaitTime[n_AxisNum][1] - m_dwSdWaitTime[n_AxisNum][0];
			if(m_dwSdWaitTime[n_AxisNum][2] <= 0 )
			{
				m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
				break;
			}
			if(m_dwSdWaitTime[n_AxisNum][2] > MOT_ELE_MOVE_WAITLIMIT)
			{
				COMI.Set_MotStop(1, n_AxisNum) ;
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
		}

		//nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);
		nRet_1 = COMI.Start_SingleMove( n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)
		{					 				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 2100;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(202, dWARNING, alarm.mstr_code);

			if (st_handler.cwnd_list != NULL)  
			{
				sprintf(st_msg.c_normal_msg, "UnLoader Alarm : 2000");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µø¿€ Ω«∆– √‚∑¬ ø‰√ª
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2100:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_GOOD)
		{
			COMI.Set_MotStop(1, n_AxisNum) ;

			st_motor[n_AxisNum].n_sd_mv_chk = 0; //clear
// 			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear

			COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE); 
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			m_nSD_Elv_MoveStep[n_AxisNum] = 2200;
			break;
		}
 		nRet_1 = COMI.Check_SingleMove( n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS]);
		//nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);   
		if (nRet_1 == BD_GOOD)   
		{
			if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)
			{
				if(n_AxisNum == M_TRAY1_Z)
				{
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check,	IO_ON); //load stacker tray check    
					if(nRet_1 == IO_OFF)
					{
						m_nSD_Elv_MoveStep[n_AxisNum] = 0;
						nFuncRet = RET_GOOD;
						break;
					}
					nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
					if(nRet_1 == BD_GOOD)
					{
						m_nSD_Elv_MoveStep[n_AxisNum] = 0;
						nFuncRet = RET_GOOD;
					}
					else
					{
						COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  
						m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
						m_nSD_Elv_MoveStep[n_AxisNum] = 2200;
					}
				}
				else
				{
					//james test m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
					m_nSD_Elv_MoveStep[n_AxisNum] = 0;
					nFuncRet = RET_GOOD;
				}
			}
			else if( n_TargetPos_Site == P_ELV_RECEIVE_OFFSET )
			{
				if(g_ioMgr.get_in_bit( st_io.i_Unloading_Stacker_Tray_Exist_Check, IO_ON ) == IO_ON )
				{
					m_nSD_Elv_MoveStep[n_AxisNum] = 0;
					nFuncRet = RET_GOOD;
				}
			}
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(203, dWARNING, alarm.mstr_code);

			////////if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2010"));
				//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2200:
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 != BD_GOOD)
			{
				COMI.Set_MotStop(1, n_AxisNum);

				m_bSD_MoveFlag[n_AxisNum] = false;
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
// 			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear			
// 			COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmFALSE);
// 			COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  
// 			COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmTRUE);
		}
		

		m_dwSdWaitTime[n_AxisNum][1] = GetTickCount();
		m_dwSdWaitTime[n_AxisNum][2] = m_dwSdWaitTime[n_AxisNum][1] - m_dwSdWaitTime[n_AxisNum][0];
		if(m_dwSdWaitTime[n_AxisNum][2] <= 0 )
		{
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			break;
		}
		if(m_dwSdWaitTime[n_AxisNum][2] < 1000) break;


		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)
		{
			m_dTargetPos[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_SUPPLY_OFFSET]);
		}
		else if( n_TargetPos_Site == P_ELV_RECEIVE_OFFSET)
		{
			m_dTargetPos[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_RECEIVE_OFFSET]);
		}
		else
		{
			break;
		}

// 		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);
		nRet_1 = COMI.Start_SingleMove( n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 2210;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 2200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_sync.n_barcode_read_serial_num[0][0]++;
			if( st_sync.n_barcode_read_serial_num[0][0] > 3)
			{
				st_sync.n_barcode_read_serial_num[0][0] = 0;
				CTL_Lib.Alarm_Error_Occurrence(205, dWARNING, alarm.mstr_code);

// 				if (st_handler.cwnd_list != NULL)  
// 				{
// 					sprintf(st_msg.c_normal_msg, "UnLoader Alarm : 2100");
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µø¿€ Ω«∆– √‚∑¬ ø‰√ª
// 				}
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 2200;
		}
		break;

	case 2210:
		if( n_TargetPos_Site == P_ELV_RECEIVE_OFFSET )
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 != BD_GOOD)
			{
				COMI.Set_MotStop(1, n_AxisNum);				
				m_bSD_MoveFlag[n_AxisNum] = false;
				m_nSD_Elv_MoveStep[n_AxisNum] = 2300;
				break;
			}			
		}
// 		else if( n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)
// 		{
// // 			COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  
// // 			COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmTRUE);
// 		}
// // 		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear			
// 		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmFALSE);  

		
		//nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);
		nRet_1 = COMI.Check_SingleMove( n_AxisNum, m_dTargetPos[n_AxisNum]);//, COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)   
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 2300;
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(206, dWARNING, alarm.mstr_code);

			if (st_handler.cwnd_list != NULL)  
			{
				sprintf( st_msg.c_abnormal_msg,"Motor:%d Target: %.3f Feedback: %.3f", n_AxisNum, m_dTargetPos[n_AxisNum], COMI.Get_MotCurrentPos(n_AxisNum));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , ABNORMAL_MSG);
			}

			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2300:
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET || n_TargetPos_Site == P_ELV_RECEIVE_OFFSET)
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 == BD_GOOD)
			{
				m_nSD_Elv_MoveStep[n_AxisNum] = 2400;
			}
			else
			{//error
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
			}
		}
		break;

	case 2400:
		m_nSD_Elv_MoveStep[n_AxisNum] = 2500;
		break;

	case 2500:
		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)
		{
			m_dSD_Supply_Pos_Backup[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_SUPPLY_OFFSET]);
		}
		else if( n_TargetPos_Site == P_ELV_RECEIVE_OFFSET)
		{
			m_dSD_Supply_Pos_Backup[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_RECEIVE_OFFSET]);
		}

		//m_dReference_Pos_Backup

		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear
		m_nSD_Elv_MoveStep[n_AxisNum] = 0;
		nFuncRet = RET_GOOD;
		break; 

		//
		////////////////////////////////////////////////////////////
	case 6000: 
		if(m_bSD_MoveFlag[n_AxisNum] == false)
		{
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			m_bSD_MoveFlag[n_AxisNum] = true;
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor set
		}
		else if(m_bSD_MoveFlag[n_AxisNum] == true)
		{
			m_dwSdWaitTime[n_AxisNum][1] = GetTickCount();
			m_dwSdWaitTime[n_AxisNum][2] = m_dwSdWaitTime[n_AxisNum][1] - m_dwSdWaitTime[n_AxisNum][0];
			if(m_dwSdWaitTime[n_AxisNum][2] <= 0)
			{
				m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
				break;
			}
			if(m_dwSdWaitTime[n_AxisNum][2] > MOT_ELE_MOVE_WAITLIMIT)
			{
				COMI.Set_MotStop(1, n_AxisNum) ;
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate/3);
		if (nRet_1 == BD_GOOD)
		{					 				 
			m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
			m_nSD_Elv_MoveStep[n_AxisNum] = 6100;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(207, dWARNING, alarm.mstr_code);
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 6100:
		m_dwSdWaitTime[n_AxisNum][1] = GetTickCount();
		m_dwSdWaitTime[n_AxisNum][2] = m_dwSdWaitTime[n_AxisNum][1] - m_dwSdWaitTime[n_AxisNum][0];
		if(m_dwSdWaitTime[n_AxisNum][2] <= 0) m_dwSdWaitTime[n_AxisNum][0] = GetTickCount();
		if(m_dwSdWaitTime[n_AxisNum][2] <= 5000) break;

		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if( n_AxisNum == M_TRAY1_Z)
			nRet_2 = g_ioMgr.get_in_bit( st_io.i_Loading_Stacker_Tray_Limit_Check, IO_OFF);
		else
			nRet_2 = g_ioMgr.get_in_bit( st_io.i_Unloading_Stacker_Tray_Limit_Check, IO_OFF);
		if(nRet_1 == BD_ERROR && nRet_2 == IO_OFF)
		{ 
			COMI.Set_MotStop(1, n_AxisNum);
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear

			m_nSD_Elv_MoveStep[n_AxisNum] = 7000;
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)   
		{	 
			m_nSD_Elv_MoveStep[n_AxisNum] = 6200;
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(208, dWARNING, alarm.mstr_code);
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 6200:
		/////////////////////////////////////////////////////////////////////
		// TRAY FULL Error
		/////////////////////////////////////////////////////////////////////
		nFuncRet = RET_ABORT;
		m_nSD_Elv_MoveStep[n_AxisNum] = 6100; 

		if (st_handler.cwnd_list != NULL)
		{
			st_other.nBuzYesNo = YES;
			if( n_AxisNum == M_TRAY1_Z )
			{
				st_msg.mstr_event_msg[0] = _T("[ TRAY ELV#1 FULL] Tray Elv#1 is Full. \r\n Please Remove it.");
			}
			else if( n_AxisNum == M_TRAY2_Z )
			{
				st_msg.mstr_event_msg[0] = _T("[ TRAY ELV#2 FULL] Tray Elv#2 is Full. \r\n Please Remove it.");
			}			
			else
			{
				st_msg.mstr_event_msg[0] = _T("[TRAY FULL] Warnning : Tray is Full. \r\n Please Remove it.");
			}
			
			if( st_handler.cwnd_list != NULL )
			{
				sprintf(st_msg.c_abnormal_msg, "%s", st_msg.mstr_event_msg[0]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			Func.OnSet_IO_Port_Stop();
		}
		break;


	case 7000:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_ERROR)
		{
			COMI.Set_MotStop(1, n_AxisNum);

			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;
	}	 

	if(m_nSD_Elv_MoveStep[n_AxisNum] == 0 || nFuncRet == RET_GOOD)
	{
		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear
		m_nSd_Motor_MoveStart_Flag[n_AxisNum] = 0;
	}

	return nFuncRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//									MOTOR ERROR CODE DEFINITIONs								//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCtlBd_Library::Motor_Error_Occurrence(int n_Mode, long l_Axis, long l_ErrorIDNum, CString strErrorMsg)
{
	CString strMsg, strMsg_1, strMsg_2, strMsg_ErrIDNum;	 
	 

	strMsg = _T("");
	strMsg_1 = _T("");
	strMsg_2 = _T("");
	strMsg_ErrIDNum = _T("");

	
	////////////////////
	switch(l_Axis)
	{
	case -1:
		l_Axis = 0;
		strMsg_1 = _T("");		
		break;
	case 0:
		strMsg_1.Format(_T("M_TRAY1_Z[%02ld]"), l_Axis);
		break;
	case 1:
		strMsg_1.Format(_T("M_TRAY2_Z[%02ld]"), l_Axis);
		break;
	case 2:
		strMsg_1.Format(_T("M_PRESS_Y[%02ld]"), l_Axis);
		break;
	case 3:   
		strMsg_1.Format(_T("M_EPOXY_TRANSFER_X[%02ld]"), l_Axis);
		break;
	case 4:   
		strMsg_1.Format(_T("M_EPOXY_TRANSFER_Y[%02ld]"), l_Axis);
		break;
	case 5:   
		strMsg_1.Format(_T("M_EPOXY_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 6:   
		strMsg_1.Format(_T("M_EPOXY_SCREW[%02ld]"), l_Axis);
		break;
	case 7:   
		strMsg_1.Format(_T("M_EPOXY_SCREW[%02ld]"), l_Axis);
		break;
	case 8:   
		strMsg_1.Format(_T("M_CARRIER_X[%02ld]"), l_Axis);
		break;
	case 9:   
		strMsg_1.Format(_T("M_LOADER_TRANSFER_Y[%02ld]"), l_Axis);
		break;
	case 10:   
		strMsg_1.Format(_T("M_LOADER_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 11:   
		strMsg_1.Format(_T("M_HEATSINK_TRANSFER_X[%02ld]"), l_Axis);
		break;
	case 12:   
		strMsg_1.Format(_T("M_HEATSINK_TRANSFER_Y[%02ld]"), l_Axis);
		break;
	case 13:   
		strMsg_1.Format(_T("M_HEATSINK_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 14:   
		strMsg_1.Format(_T("M_UNLOADER_TRANSFER_X[%02ld]"), l_Axis);
		break;
	case 15:   
		strMsg_1.Format(_T("M_UNLOADER_TRANSFER_Y[%02ld]"), l_Axis);
		break;
		
	case 16:   
		strMsg_1.Format(_T("M_UNLOADER_TRANSFER_Z[%02ld]"), l_Axis);
		break;
	case 17:   
		strMsg_1.Format(_T("M_DISPENSER_Y[%02ld]"), l_Axis);
		break;
	case 18:   
		strMsg_1.Format(_T("M_TRAY_REMOVE_X[%02ld]"), l_Axis);
		break;
	case 19:   
		strMsg_1.Format(_T("M_HEATSINK_INSPECT_Y[%02ld]"), l_Axis);
		break;
	case 20:   
		strMsg_1.Format(_T("M_HEATSINK_DISPENSOR_Z[%02ld]"), l_Axis);
		break;
	case 21:   
		strMsg_1.Format(_T("M_HEATSINK_PICKER_PITCH[%02ld]"), l_Axis);
		break;

	default :
		strMsg_1.Format(_T("MOTOR_NUM_UNKNOWN_DEFAULT_ERROR[%02ld]"), l_Axis);
		l_Axis = 0;
		break;  

	}
		 

	//////////////////////////////////////////////////////////////////
	switch(l_ErrorIDNum)
	{
	case -1:
		l_ErrorIDNum = 1;
		strMsg_2.Format(_T(""));
		 break;	

	case 1:
		 strMsg_2.Format(_T("MOTOR_POWER_OFF_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 2:
		 strMsg_2.Format(_T("MOTOR_ALARM_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 3:
		 strMsg_2.Format(_T("MOTOR_HOME_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 4:
		 strMsg_2.Format(_T("MOTOR_MOVE_START_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 5:
		 strMsg_2.Format(_T("MOTOR_MOVE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 6:
		 strMsg_2.Format(_T("MOTOR_MOVE_END_TIME_OUT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 7:
		 strMsg_2.Format(_T("MOTOR_SAFETY_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	case 10: 
		strMsg_2.Format(_T("MOTOR_SET_IO_PROPERTY_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 11: 
		strMsg_2.Format(_T("MOTOR_PARAMETER_SETTING_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 12: 
		strMsg_2.Format(_T("MOTOR_MOVE_POS_MINUS_LIMIT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 13: 
		strMsg_2.Format(_T("MOTOR_MOVE_POS_PLUS_LIMIT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 14: 
		strMsg_2.Format(_T("MOTOR_CMDPOS_FEEDPOS_DEVITION_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 15: 
		strMsg_2.Format(_T("MOTOR_ALARM_CLEAR_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 16: 
		strMsg_2.Format(_T("MOTOR_ALARM_STATUS_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 17: 
		strMsg_2.Format(_T("MOTOR_ESTOP_RETURN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 18: 
		strMsg_2.Format(_T("MOTOR_GETCURRENTPOS_RETURN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 19: 
		strMsg_2.Format(_T("MOTOR_INPOSITION_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 20: 
		strMsg_2.Format(_T("MOTOR_MOTION_DONE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 21: 
		strMsg_2.Format(_T("MOTOR_MOTION_DONE_ZERO_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 22: 
		strMsg_2.Format(_T("MOTOR_HOME_CONFIG_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 23: 
		strMsg_2.Format(_T("MOTOR_HOME_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 24: 
		strMsg_2.Format(_T("MOTOR_OVERRIDE_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 25: 
		strMsg_2.Format(_T("MOTOR_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	case 26: 
		strMsg_2.Format(_T("MOTOR_UNIT_SPEED_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 27: 
		strMsg_2.Format(_T("MOTOR_INTERRUPT_STATUS_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 28: 
		strMsg_2.Format(_T("MOTOR_TRIGGERSET_ONE_COMPARE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 29: 
		strMsg_2.Format(_T("MOTOR_UNIT_DISTANCE_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 30: 
		strMsg_2.Format(_T("MOTOR_SPEED_RATIO_RETURN_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 31: 
		strMsg_2.Format(_T("MOTOR_MOTION_STATE_CHECK_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	case 41: 
		strMsg_2.Format(_T("MOTOR_LIST_MOTION_LMMAPAXIS_CREATE_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 42: 
		strMsg_2.Format(_T("MOTOR_LINEAR_MOTION_IXMAPAXIS_CREATE_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	case 50: 
		strMsg_2.Format(_T("MOTOR_BOARD_LOAD_DLL_OPEN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 51: 
		strMsg_2.Format(_T("MOTOR_BOARD_LOAD_DEVICE_OPEN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 52: 
		strMsg_2.Format(_T("MOTOR_BOARD_CME_FILE_OPEN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
 

	////////////////////////////////////////////////////////////////////////////////////////////////
    //Motor dll return error id num
	////////////////////////////////////////////////////////////////////////////////////////////////
	case cmERR_NONE: //0:
		 strMsg_2 = _T("[%04ld]"), abs(l_ErrorIDNum);
		 break;
	case cmERR_UNKNOWN:
		strMsg_2.Format(_T("MOTOR_UNKNOWN_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;	
	case cmERR_MEM_ALLOC_FAIL://-290
		 strMsg_2.Format(_T("Memory allocation fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_GLOBAL_MEM_FAIL://-292
		 strMsg_2.Format(_T("Global memory allocation fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_ISR_CONNEC_FAIL://-310
		 strMsg_2.Format(_T("ISR(Interrupt Service Routine) registration fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_DIVIDE_BY_ZERO://-400
		 strMsg_2.Format(_T("Cause divide by zero error[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_WORNG_NUM_DATA://-500
		 strMsg_2.Format(_T("Number of data is too small or too big[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_VER_MISMATCH://-600
		 strMsg_2.Format(_T("Version(of file or device) mismatch[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_DEVICE_ID://-1010
		 strMsg_2.Format(_T("User set invalid device id. Refer to ‚ÄúDeviceId‚Äù property.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_HANDLE://-1020
		 strMsg_2.Format(_T("Device handle is not valid. This means that loading a device has been failed or not performed. Refer to ‚ÄúGnLoadDevcie‚Äù function[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_UNSUPORTED_FUNC://-1030
		 strMsg_2.Format(_T("User called an unsupported function for the specified product.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_PARAMETER://-1101
		 strMsg_2.Format(_T("Some of the function parameters are invalid.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_CHANNEL://-1105
		 strMsg_2.Format(_T("UThe channel setting parameter(s) is(are) invalid.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_INPUT_RANGE://-1111
		 strMsg_2.Format(_T("Invalid range value (AI, AO).[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_FREQ_RANGE://-1121
		 strMsg_2.Format(_T("User selected invalid frequency range.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_FILE_CREATE_FAIL://-1501
		 strMsg_2.Format(_T("File creation has been failed.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_FILE_OPEN_FAIL://-1511
		 strMsg_2.Format(_T("File opening has been failed.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_FILE_READ_FAIL://-1522
		 strMsg_2.Format(_T("File reading fail.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_EVENT_CREATE_FAIL://-1550
		 strMsg_2.Format(_T("Event handle creation has been failed.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_DITHREAD_CRE://-1570
		 strMsg_2.Format(_T("D/I state change monitor thread creation fail[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_BUFFER_SMALL://-1580
		 strMsg_2.Format(_T("Buffer size is too small[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_HIGH_TIMER_UNSUPP://-1090
		 strMsg_2.Format(_T("The installed hardware doesn‚Äüt support a high-resolution performance counter.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_OUT_OF_RANGE://-1600
		 strMsg_2.Format(_T("The range of some parameter is out of range that it is occured.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_ON_MOTION://-5001
		 strMsg_2.Format(_T("This code is just a symbolic code. This error will never occur.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SLP://-5002
		 strMsg_2.Format(_T("Abnormally stopped by positive soft limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SLN://-5003
		 strMsg_2.Format(_T("Abnormally stopped by negative soft limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CMP3://-5004
		 strMsg_2.Format(_T("Abnormally stopped by comparator3[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CMP4://-5005
		 strMsg_2.Format(_T("Abnormally stopped by comparator4[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CMP5://-5006
		 strMsg_2.Format(_T("Abnormally stopped by comparator5[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_ELP://-5007
		 strMsg_2.Format(_T("Abnormally stopped by (+) external limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_ELN://-5008
		 strMsg_2.Format(_T("Abnormally stopped by (-) external limit[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_ALM://-5009
		 strMsg_2.Format(_T("Abnormally stopped by alarm input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CSTP://-5010
		 strMsg_2.Format(_T("Abnormally stopped by CSTP input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_CEMG://-5011
		 strMsg_2.Format(_T("Abnormally stopped by CEMG input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SD://-5012
		 strMsg_2.Format(_T("Abnormally stopped by SD input signal[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_DERROR://-5013
		 strMsg_2.Format(_T("Abnormally stopped by operation data error[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_IP://-5014
		 strMsg_2.Format(_T("Abnormally stopped by other axis error during interpolation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_PO://-5015
		 strMsg_2.Format(_T("An overflow occurred in the PA/PB input buffer[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_AO://-5016
		 strMsg_2.Format(_T("Out of range position counter during interpolation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_EE://-5017
		 strMsg_2.Format(_T("An EA/EB input error occurred (does not stop)[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_PE://-5018
		 strMsg_2.Format(_T("An PA/PB input error occurred (does not stop)[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SLVERR://-5019
		 strMsg.Format(_T("Abnormally stopped because slave axis has been stopped[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_STOP_BY_SEMG://-5120
		 strMsg_2.Format(_T("Abnormally stopped by software emergency setting[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_MOT_MAOMODE://-5110
		 strMsg_2.Format(_T("Master output mode is not CW/CCW mode during Master/Slave operation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_MOT_SLAVE_SET://-5120
		 strMsg_2.Format(_T("Slave start has been failed during Master/Slave operation[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_SPEED_RANGE_OVER://-5130
		 strMsg_2.Format(_T("Speed setting value exceeds setting range[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_SPEED_SET://-5140
		 strMsg_2.Format(_T("Speed setting value is not valid[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_IXMAP://-5150
		 strMsg_2.Format(_T("Invalid Interpolation Map[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_LMMAP://-5160
		 strMsg_2.Format(_T("Invalid List-Motion Map[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_MOT_SEQ_SKIPPED://-5170
		 strMsg_2.Format(_T("Invalid List-Motion Map[%04ld]"), abs(l_ErrorIDNum));
		 break;
// 	case cmERR_CMPIX_INVALID_MAP://-5180
// 		 strMsg_2.Format(_T("Interpolated position compare output map is not valid[%04ld]"), abs(l_ErrorIDNum));
// 		 break;
// 	case cmERR_INVALID_ARC_POS://-5190
// 		 strMsg_2.Format(_T("Position data for circular interpolation is invalid[%04ld]"), abs(l_ErrorIDNum));
// 		 break;
// 	case cmERR_LMX_ADD_ITEM_FAIL://-5200
// 		 strMsg_2.Format(_T("failed to add an job item to extend list motion[%04ld]"), abs(l_ErrorIDNum));
// 		 break;
// 	case cmERR_LMX_IS_NOT_ACTIVE://-5300
// 		 strMsg_2.Format(_T("Extended ListMotion' is not active extend list motion[%04ld]"), abs(l_ErrorIDNum));
// 		 break;
	default:
		strMsg_2.Format(_T("MOTOR_UNKNOWN_DEFAULT_ERROR[%04ld]"), abs(l_ErrorIDNum));
		l_ErrorIDNum = 1;
		break;
	 }

	 strMsg = strMsg_1 + _T("") + strMsg_2 + _T("") + strErrorMsg;	
	 strMsg_ErrIDNum.Format(_T("%02ld%04ld"), l_Axis, abs(l_ErrorIDNum)); //ÌïÑÏöîÏãú ÏÇ¨Ïö©ÏúÑÌï¥ 6ÏûêÎ¶¨Ïùò ÏóêÎü¨ ÏΩîÎìú Ï†ïÏùò 
	
	 if(n_Mode > 0)
	 {

	 }
	 else if(n_Mode == 0)
	 {
		 if(str_Motor_Msg == strMsg)
		 {
			 return;
		 }
	 }

	 sprintf(mc_alarmcode, strMsg_ErrIDNum);

	 if(COMI.mn_errormsg_debug_mode)
	 {
		//OnStringToChar(strMsg, mc_normal_msg);
		sprintf( mc_normal_msg, strMsg);
		COMI.Debug_File_Create(0, mc_normal_msg);
	 }
	 //∏∂¡ˆ∏∑ ªÁøÎ«— ∏ﬁ§ƒ¡ˆ ¡§∫∏ ¿˙¿Â
	 str_Motor_Msg = strMsg;

// 	 if (st_handler.cwnd_list != NULL)
// 	 {
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 	 }
}


