// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  íŒŒì¼ ì¡´ì¬ ìœ ë¬´ ê²€ì‚¬ í•¨ìˆ˜ í˜¸ì¶œí•˜ê¸° ìœ„í•´ì„œëŠ” ë°˜ë“œì‹œ í•„ìš”
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

}

CCtlBd_Library::~CCtlBd_Library()
{

}

CCtlBd_Library CTL_Lib;

//////////////////////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 
//ì•ŒëŒë°œìƒ ì²˜ë¦¬ë°©ë²•, ëª¨í„° ë° I/O ê´€ë ¨ ë¼ì´ë¸ŒëŸ¬ë¦¬ë¥¼ ì´ˆê¸°í™” ë° ì‘ìš©í•˜ì—¬ ì‹¤ì œì‚¬ìš©í•˜ëŠ” í•¨ìˆ˜ì •ë¦¬ 
//
//ì°¸ê³  ê¸°ë³¸í•¨ìˆ˜ 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{
	//ê·¸ê°„ì€ ì•ŒëŒì´ ë°œìƒí•˜ë©´ ê° ì“°ë ˆë“œì—ì„œ jamcode ë° run status ì •ë³´ë¥¼ ë°”ë€Œì–´ ì¥ë¹„ë¥¼ ì •ì§€í•˜ê³  
	//ë¬¸ì œë¥¼ í•´ê²°í–ˆìœ¼ë‚˜ ì•ìœ¼ë¡œëŠ” ì´ í•¨ìˆ˜ë¥¼ ëª¨ë“ ê³³ì—ì„œ ì‚¬ìš©í•˜ì—¬ ì•ŒëŒì •ë³´ ë° ê°€ë™ ìƒíƒœë¥¼ í‘œì¤€í•¨ìˆ˜ í•˜ë‚˜ë¥¼ 
	//ì‚¬ìš©í•˜ì—¬ ì¥ë¹„ ê°€ë™ ë° ì•ŒëŒ ë¬¸ì œë¥¼ ì²˜ë¦¬í•œë‹¤ 
	//
	//ê° ì“°ë ˆë“œì— ê°ê°ìˆë˜ ì•ŒëŒ ì²˜ë¦¬ë°©ë²•ì€ ì´ í•¨ìˆ˜ë¥¼ ëª¨ë‘ ì‚¬ìš©í•œë‹¤ 
   //
	int n_cur_num = -1;

	CTL_Lib.mn_jampos = n_jampos;
	CTL_Lib.mn_run_status = n_run_status;
	COMI.mn_run_status = n_run_status;
	st_work.mn_run_status = n_run_status;
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

	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	}
	

// 	st_handler.mn_dumping_alarm_count++;
// 
// 	sprintf( st_msg.c_normal_msg, "Dumping Test Pick & Place SSD : %d / %d" ,  st_handler.mn_dumping_alarm_count, st_handler.mn_dumping_test_count);
// 	st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG );

	//st_handler.m_tDMTBI = 0;
	
	CString mstr_temp;

	return BD_GOOD;
}

int CCtlBd_Library::Alarm_Error_Occurrence( int n_jampos, int n_run_status, CString strJamCode )
{
// 	//2013,0123
// 	if(COMI.mn_run_status == dINIT) alarm.stl_a .stl_alarm_start_time = GetCurrentTime();
	int n_cur_num = -1;
	CTL_Lib.mn_jampos = n_jampos; //jamÀÌ ¹ß»ıÇÑ À§Ä¡¸¦ ÀúÀåÇÑ´Ù 
	CTL_Lib.mn_run_status = n_run_status; //ÀåºñÀÇ °¡µ¿»óÅÂ¸¦ º¯°æÇÑ´Ù 
	COMI.mn_run_status = n_run_status;
	alarm.mstr_code = strJamCode;
	st_work.mn_run_status = n_run_status;
	alarm.stl_cur_alarm_time = GetCurrentTime();
	
	strcpy(mc_alarmcode, (LPCSTR)strJamCode);
	
	//	g_handler.AddAlarmCnt();

	
	//2012,1220
	// 	CtlBdFunc.Alarm_Error_Occurrence(CTL_Lib.mn_jampos, COMI.mn_run_status, COMI.mn_run_status, alarm.mc_code);
	MyJamData.On_Alarm_Info_Set_to_Variable(alarm.mstr_code);

	
	if (st_handler.cwnd_list != NULL)  // ¸®½ºÆ® ¹Ù È­¸é Á¸Àç
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	}
	
	
	
	return BD_GOOD;
}

int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor ë³´ë“œ ì´ˆê¸°í™”ì‹œ ì‚¬ìš©
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if( n_bd_type == 0 ) //0:fastec
	{
		COMI.mn_totalmotboard_number = 4;//¸ğÅÍº¸µå°¡ 3°³ ÀÖ´Ù 
		COMI.mn_totalmotoraxis_number= 21; //¸ğÅÍÀÇ ÃÑ ¼ö·®Àº 21°³ÀÌ´Ù  
		COMI.mn_bd_axis_number[0] = 8;//0¹ø ¸ğÅÍº¸µå´Â 8Ãà¿ë º¸µåÀÌ´Ù  
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
				{//í™œì„±
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//ë¹„í™œì„± 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}
	}

	//0¹ø º¸µå	Motor No., ±â¾îºñ (1È¸Àü½Ã ÆŞ½º :10000 / 1È¸Àü½Ã ÀÌµ¿°Å¸®)
	// 1mm ÀÌµ¿ÇÏ´Âµ¥ ÇÊ¿äÇÑ pulse
	COMI.Set_MotUnitDist(M_TRAY1_Z, 1000);
	// 10mm ÀÌµ¿ÇÏ´Âµ¥ ÇÊ¿äÇÑ pulse (¼Óµµ´Â 1Cm/ÃÊ·Î ¼¼ÆÃÇÏ°Ô µÈ´Ù.)
	COMI.Set_MotUnitSpeed(M_TRAY1_Z, 1000);
	// motor Home
	COMI.mn_homecheck_method[M_TRAY1_Z] = 6;

	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD ºñÈ°¼º, cmTRUE = 1 SD È°¼º 	
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : AÁ¢Á¡ ¹æ½Ä,1 (cmLOGIC_B) : BÁ¢Á¡ ¹æ½Ä
	COMI.Set_Motor_IO_Property(M_TRAY1_Z, cmSD_LATCH, cmFALSE);//16
	COMI.Set_MotorType_Init(M_TRAY1_Z, MOT_SERVO);


	
	COMI.Set_MotUnitDist(M_TRAY2_Z,	1000);
	COMI.Set_MotUnitSpeed(M_TRAY2_Z, 1000);
	COMI.mn_homecheck_method[M_TRAY2_Z] = 6;

	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD ºñÈ°¼º, cmTRUE = 1 SD È°¼º 	
	COMI.Set_Motor_IO_Property(M_TRAY2_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : AÁ¢Á¡ ¹æ½Ä,1 (cmLOGIC_B) : BÁ¢Á¡ ¹æ½Ä
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


	///1¹ø º¸µå 
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
				st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // µ¿ÀÛ ¿Ï·á Ãâ·Â ¿äÃ»
			}
		}
		
		COMI.Set_MotPower(i, TRUE);		

		COMI.Set_CMD_CTL_Mode(i, cmCTRL_SEMI_C);//20150429 james cmCTRL_SEMI_C , ¿£ÄÚ´õ,(ÇÇµå¹é±âÁØ) ±âÁØÀ¸·Î ¸ñÇ¥ÁÂÇ¥¸¦ ¼³Á¤ÇÏ¿© ÀÌ¼ÛÇÕ´Ï´Ù(Ä¿¸àµå Á¤º¸´Â ¹«½ÃµÇ°í ÃÖÁ¾ feedback(¿£ÄÚ´õ) À§Ä¡·Î Á¦¾î°¡ ÀÌ·ç¾îÁø´Ù.
	}

	return  BD_GOOD;
}

//ì•„ë˜ ë‚´ìš©ì€ I/O ë³´ë“œ ì´ˆê¸°í™”ì‹œ í•„ìš”í•œ ë‚´ìš©ì„ ì˜ˆë¡œ ë“¤ì€ê²ƒì´ë‹ˆ 
//ì¥ë¹„ë³„ í•„ìš”í•œ ë‚´ìš©ì„ ì…‹íŒ…í•˜ì—¬ ì‚¬ìš©í•˜ë©´ ëœë‹¤.
int CCtlBd_Library::Initialize_io_Board(int n_bd_type)
{
	//if(n_bd_type == 0) //íŒŒìŠ¤í… HSSI I/O board 
	//{
	//}
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	

	/////////////////ì´ˆê¸°ì— ì´ˆê¸°í™”ì‹œ //////////////////////////////////////////////////////////////////////
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_1_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_1_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_2_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9_2] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0_1] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety1_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety2_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety3_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety0_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety4_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety5_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety6_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety10_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// // 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety7_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 								CtlBdFunc.ms_ErrMsg.Format("[LSafety8_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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
// 							CtlBdFunc.ms_ErrMsg.Format("[LSafety9_3] Linear ·Îº¿ XÃà ÀÌµ¿ ¿¡·¯ - Æ®·£½ºÆÛ °ø±Ş À§Ä¡ ¾Æ´Ô !!!" );	
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


int CCtlBd_Library::Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos, double d_jogcurrentpos) 	//ëª¨í„° ì•ˆì „ ì •ë³´ ì…‹íŒ…ì‹œ ì‚¬ìš© 
{
	// ì˜ˆ) 
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog, 4:Length Change
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet_1 = 0, nRet_2 = 0, nRet_3 = 0, nRet_4 = 0, nRet_5 = 0, nRet_6 = 0;
	double d_CurPos[M_MOTOR_COUNT] = {0,};
	double d_Pos[4] = {0,};
	double d_GapCheck = 0;
	int i = 0;


	if(Func.DoorOpenCheckSpot() != CTLBD_RET_GOOD)
	{
		return CTL_DOOROPEN;
	}


	if (d_jogcurrentpos != -1000)
	{
		d_CurPos[n_axis] = d_jogcurrentpos;
	}
	// **************************************************************************
	// ëª¨í„° ì•ŒëŒ ìƒíƒœ ê²€ì‚¬í•œë‹¤                                                   
	// -> ì•ŒëŒ ë°œìƒí•œ ê²½ìš° ì•ŒëŒ í•´ì œí•œë‹¤                                         
	// **************************************************************************
	//software limit ê°’ ì…‹íŒ… ì²´í¬ ì—ëŸ¬ 
	if(d_targetpos < COMI.md_limit_position[n_axis][0] && d_targetpos < d_jogcurrentpos ) //
	{
		//sprintf(cJamcode, "%02d0004", n_axis);
		sprintf(cJamcode, "0%02d010", n_axis);			// 2014.12.16 - Bredmin.
		Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
		
// 		if (st_handler.cwnd_list != NULL)
// 		{  // ë¦¬ìŠ¤íŠ¸ ë°” í™”ë©´ ì¡´ì¬
// 			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ë™ì‘ ì‹¤íŒ¨ ì¶œë ¥ ìš”ì²­
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
// 		{  // ë¦¬ìŠ¤íŠ¸ ë°” í™”ë©´ ì¡´ì¬
// 			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos);
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ë™ì‘ ì‹¤íŒ¨ ì¶œë ¥ ìš”ì²­
// 		}
		// 2014.11.29 - Bredmin.
		CString strTemp = "";
		strTemp.Format( "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos );
		Func.On_LogFile_Add( LOG_TOTAL, strTemp );
		return BD_ERROR;
	}

	// **************************************************************************
	// ëª¨í„° íŒŒì›Œ ìƒíƒœ ê²€ì‚¬í•œë‹¤                                                   
	// -> ëª¨í„° POWER OFF ì‹œ POWER ON ìƒíƒœë¡œ ë§Œë“ ë‹¤                               
	// **************************************************************************
	if (g_comiMgr.Get_MotPower(n_axis) == CTL_ERROR )          // ëª¨í„° POWER ìƒíƒœ ê²€ì‚¬ í•¨ìˆ˜ 
	{
		if (COMI.Set_MotPower(n_axis, CTL_ON) == CTLBD_RET_GOOD)       // ëª¨í„° POWER ON ì„¤ì • í•¨ìˆ˜ 
		{
// 			if (st_handler.cwnd_list != NULL)
// 			{  // ë¦¬ìŠ¤íŠ¸ ë°” í™”ë©´ ì¡´ì¬
// 				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ë™ì‘ ì‹¤íŒ¨ ì¶œë ¥ ìš”ì²­
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotPower(n_axis) == CTL_ERROR)  // ëª¨í„° POWER ìƒíƒœ ê²€ì‚¬ í•¨ìˆ˜ 
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

	if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR)      // ëª¨í„° ALARM ìƒíƒœ ê²€ì‚¬ í•¨ìˆ˜ 
	{
		if (COMI.Set_MotAlarmClear(n_axis) == CTL_GOOD)       // ëª¨í„° ALARM CLEAR í•¨ìˆ˜ 
		{
			//091119 james			Sleep(1000);  // ì¼ì • ì‹œê°„ í›„ì— ìƒíƒœ í™•ì¸í•˜ê¸° ìœ„í•´ SLEEP ì‚¬ìš©í•œë‹¤ 
// 			if (st_handler.cwnd_list != NULL)  // ë¦¬ìŠ¤íŠ¸ ë°” í™”ë©´ ì¡´ì¬
// 			{
// 				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis] );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR )  // ëª¨í„° ALARM ìƒíƒœ ê²€ì‚¬ í•¨ìˆ˜ 
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
					//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // ì•ŒëŒ í•´ì œ ì‹œë„ íšŸìˆ˜ 
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
				
				//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // ì•ŒëŒ í•´ì œ ì‹œë„ íšŸìˆ˜ 
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
	// ëª¨í„°ì˜ ì•ˆì „ ìœ„ì¹˜ë¥¼ ì²´í¬í•œë‹¤. ì¥ë¹„ì— ë§ê²Œ ë„£ëŠ”ê²ƒ ë³´ë‹¤. ë”´ìª½ì— ì§€ì •í•´ ë†“ê³  ë¶ˆëŸ¬ë‹¤ ì¨ì•¼ í• ë“¯.                                           
	//***************************************************************************
	switch( n_axis )
	{
		case M_CARRIER_X:

			if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_PUSH_POS])
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_OFF);
				nRet_4 = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);
// 				nRet_5 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, nOnOff);
// 			    nRet_6 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, !nOnOff);
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
					else								alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Carrier_Holder_Up_Check);//í˜¹ì‹œë‚˜
					CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			if(d_targetpos == st_motor[n_axis].md_pos[P_CARRIER_X_PRESS_POS])
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if( nRet_1 != IO_ON || nRet_2 != IO_OFF )
				{
					if( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check);
					else/* if( nRet_2 != IO_OFF )*/ alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
					return CTL_ERROR;
				}
			}
			else
			{
				nRet_1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
				if      ( nRet_1 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);
				else if( nRet_2 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
				else if( nRet_3 != IO_ON ) alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Up_Check);
				else if( nRet_4 != IO_OFF ) alarm.mstr_code.Format(_T("8%d%04d"), IO_OFF, st_io.i_Press_Down_Check);
				else								 alarm.mstr_code.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check);//í˜¹ì‹œë‚˜
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				return CTL_ERROR;
			}
			break;

		case M_PRESS_Y:
// 			if( Chk_PressClamp_Safety(3) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nPressAxisY);
// 				COMI.Set_MotStop( MOTSTOP_SLOWDOWN , m_nPressAxisX);
// 				break;
// 			}
// 			if( COMI.Get_MotCurrentPos( M_CARRIER_X ) < ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) &&
// 				COMI.Get_MotCurrentPos( M_CARRIER_X ) > ( st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS] + st_motor[m_nPressAxisX].mn_allow ))
// 			{
// 				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
// 				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
// 				return CTL_ERROR;
// 			}
// 			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y ) > (st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
// 			{//080008 1 0 "M_LOADER_TRANSFER_Y_MOTOR_IS_NON_SAFETY_POS."
// 				alarm.mstr_code = _T("080008");
// 				CtlBdFunc.ms_ErrMsg.Format("[Safety] Loader_Y_Motor is not safety." );				
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "080008", CtlBdFunc.ms_ErrMsg);
// 				return CTL_ERROR;
// 			}
// 			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X ) > (st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
// 			{//130008 1 0 "M_UNLOADER_TRANSFER_X_MOTOR_IS_NON_SAFETY_POS."
// 				alarm.mstr_code = _T("130008");
// 				CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "130008", CtlBdFunc.ms_ErrMsg);
// 				return CTL_ERROR;
// 			}
// 			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X ) > (st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
// 			{//130008 1 0 "M_UNLOADER_TRANSFER_X_MOTOR_IS_NON_SAFETY_POS."
// 				alarm.mstr_code = _T("130008");
// 				CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "130008", CtlBdFunc.ms_ErrMsg);
// 				return CTL_ERROR;
// 			}
// 			if( COMI.Get_MotCurrentPos( M_HEATSINK_INSPECT_Y ) > (st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + st_motor[M_LOADER_TRANSFER_Y].mn_allow)  )
// 			{//130008 1 0 "M_UNLOADER_TRANSFER_X_MOTOR_IS_NON_SAFETY_POS."
// 				alarm.mstr_code = _T("130008");
// 				CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, "130008", CtlBdFunc.ms_ErrMsg);
// 				return CTL_ERROR;
// 			}
			break;

		case M_EPOXY_TRANSFER_X:
			if(COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y) <= st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - st_motor[M_EPOXY_TRANSFER_Y].mn_allow)
			{
				d_CurPos[0]	= COMI.Get_MotCurrentPos(n_axis);
				if( d_targetpos > ( st_motor[n_axis].md_pos[P_EPOXY_TRANSFER_X_SAFETY] + st_motor[n_axis].mn_allow ) )
				{	
					alarm.mstr_code.Format(_T("%02d0008"),n_axis );
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Unloader_X_Motor is not safety." );				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg);
					return CTLBD_RET_ERROR;
				}
			}
			else if(COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y) > (st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - st_motor[M_EPOXY_TRANSFER_Y].mn_allow))
			{
				d_CurPos[0]	= COMI.Get_MotCurrentPos(M_CARRIER_X);
				if(d_CurPos[0] > (st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS]+(double)10))
				{
					if(d_targetpos < st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS])
					{
						return CTLBD_RET_ERROR;
					}
				}
			}

			if(COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) >= st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_PLACE_POS] - 5)
			{
				if(d_targetpos >= st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS])
				{
					return CTLBD_RET_ERROR;
				}
			}

			break;

		case M_EPOXY_TRANSFER_Y:
			if(COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_Y) < st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - st_motor[M_EPOXY_TRANSFER_Y].mn_allow)
			{
				d_CurPos[0]	= COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X);
				if(d_CurPos[0] < st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS])
				{
					return CTLBD_RET_ERROR;
				}
			}
			else
			{//P_EPOXY_TRANSFER_X_SUCKTION_POSì€ ìƒê°í•˜ì§€ ë§ì
// 				if (d_TargetPos < st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - st_motor[M_EPOXY_TRANSFER_Y].n_allow)
// 				{
// 					if(COMI.Get_MotCurrentPos(M_EPOXY_TRANSFER_X) < (st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SUCKTION_POS] - st_motor[M_EPOXY_TRANSFER_X].mn_allow))
// 					{
// 						return CTLBD_RET_ERROR;
// 					}
// 				}
			}

			if(d_targetpos > (st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_X_INIT_POS] + (double)10) )
			{
				d_CurPos[1]	= COMI.Get_MotCurrentPos(M_CARRIER_X);
				if(d_CurPos[1] > (st_motor[M_CARRIER_X].md_pos[P_CARRIER_X_INIT_POS]+(double)10))
				{
					return CTLBD_RET_ERROR;
				}

			}
			break;

		case M_DISPENSER_Y:
			if( ( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_X) > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[M_HEATSINK_TRANSFER_X].mn_allow) ) &&
				( COMI.Get_MotCurrentPos(M_HEATSINK_TRANSFER_Y) < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - st_motor[M_HEATSINK_TRANSFER_Y].mn_allow) ) )
			{
				sprintf(cJamcode, "%02d0008", n_axis);
				Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
				return CTLBD_RET_ERROR;
			}
			break;

		case M_EPOXY_TRANSFER_Z:
			// ë‹¹êµ¬ì¥ ë¬´ë¹™ì‹œ ì•ŒëŒ ê´€ë ¨ ..
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

	}

	st_motor[n_axis].mn_retry_time_flag = CTL_NO;//2011.0201 ì¶”ê°€ 

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
		if (n_check == BD_YES)			// Sizeë¥¼ ì²´í¬í•˜ëŠ”ê²ƒì´ë©´...
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
		else						// Sizeë¥¼ í™•ì¸í•˜ëŠ”ê²ƒì´ë©´...
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

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // í˜„ì¬ ë…„, ì›”, ì¼ ì •ë³´ ë¬¸ìí˜•ìœ¼ë¡œ ë³€í™˜í•˜ì—¬ ì €ì¥í•  ë³€ìˆ˜ 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // í˜„ì¬ ë…„, ì›”, ì¼ ì •ë³´ ì €ì¥ ë³€ìˆ˜ 
	CString mstr_file_name, mstr_dir_path, str_msg;		// ë§ˆì§€ë§‰ìœ¼ë¡œ ìƒì„±ëœ íŒŒì¼ ì´ë¦„ ì €ì¥ ë³€ìˆ˜ 
	CString mstr_create_file;	// ì•ŒëŒ ì •ë³´ ì €ì¥í•  íŒŒì¼ì— ëŒ€í•œ [í´ë”]+[íŒŒì¼ëª…]+[í™•ì¥ì] ì„¤ì • ë³€ìˆ˜ 
	CString mstr_list_name, mstr_temp_data;  // ê° ë¶€ë¶„ë³„ ì•ŒëŒ ë°œìƒ íšŸìˆ˜ ì •ë³´ ì €ì¥í•  ë¡œê·¸ íŒŒì¼ ì •ë³´ ì €ì¥ ë³€ìˆ˜ 
	CString mstr_content;		// í•œ ë¼ì¸ì— ì¶œë ¥í•  ë¬¸ìì—´ ì •ë³´ ì €ì¥ ë³€ìˆ˜ 
	COleDateTime time_cur;		// ê²€ì‚¬í•  ì‹œê°„ ì •ë³´ ì €ì¥ ë³€ìˆ˜ 
	CTime m_time_current;		// ê°„ëµí•œ í—ì‹ì˜ í˜„ì¬ ì‹œê°„ ì •ë³´ ì €ì¥ ë³€ìˆ˜
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// íŒŒì¼ ì¡´ì¬ ìœ ë¬´ ì„¤ì • í”Œë˜ê·¸ 
	char cfileName[256]={0,};			// ê²€ìƒ‰í•  íŒŒì¼ ì •ë³´ ì„¤ì • í•¨ìˆ˜ 
	FILE  *fp ;					// íŒŒì¼ì— ëŒ€í•œ í¬ì¸í„° ì„¤ì • ë³€ìˆ˜ 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //ì €ì¥í•  ë©”ì„¸ì§€ê°€ ì—†ë‹¤ 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// íŒŒì¼ ì´ë¦„ìœ¼ë¡œ ì‚¬ìš©í•  ë‚ ì§œ ì •ë³´ë¥¼ ì–»ëŠ”ë‹¤                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // í˜„ì¬ ì‹œê°„ ì •ë³´ë¥¼ ì–»ëŠ”ë‹¤. 

	m_time_current = CTime::GetCurrentTime() ;  // ê°„ëµí•œ í˜•ì‹ì˜ í˜„ì¬ ì‹œê°„ ì •ë³´ ì–»ëŠ”ë‹¤. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// ë‚ ì§œ ì •ë³´ë¥¼ ë¬¸ìí˜•ìœ¼ë¡œ ë³€í™˜í•˜ì—¬ ë³€ìˆ˜ì— ì„¤ì •í•œë‹¤                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// í˜„ì¬ ì‹œê°„ ì •ë³´ ì–»ëŠ”ë‹¤                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// í˜„ì¬ ì‹œê°„ ì •ë³´ë¥¼ ì„¤ì •í•œë‹¤. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// í˜„ì¬ ì‹œê°„ ì •ë³´ë¥¼ ì„¤ì •í•œë‹¤.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// í˜„ì¬ ë¶„ ì •ë³´ë¥¼ ì„¤ì •í•œë‹¤. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// í˜„ì¬ ì´ˆ ì •ë³´ë¥¼ ì„¤ì •í•œë‹¤. 
	str_display_time = m_time_current.Format("%c");	// ë¦¬ìŠ¤íŠ¸ íŒŒì¼ì— ì¶œë ¥í•  íƒ€ì´í‹€ ì‹œê°„ ì •ë³´ ì„¤ì • 
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
	{//í•´ë‹¹ íŒŒì¼ì´ ì¡´ì¬í•˜ì§€ ì•ŠëŠ” ìƒíƒœì´ë‹¤ 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//í•´ë‹¹ íŒŒì¼ì´ ì´ë¯¸ ì¡´ì¬í•œë‹¤.
		//ì •ìƒ 
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

		fclose(fp) ;  //2011.0417  íŒŒì¼ ì¢…ë£Œ

//2011.0417		sing.Unlock(); //2011.0417 ì¶”ê°€ 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // íŒŒì¼ì„ ì¢…ë£Œí•œë‹¤.

//2011.0417	sing.Unlock(); //2011.0417 ì¶”ê°€ 
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
		if (nRet_1 == BD_GOOD) //ì´ë¯¸ í•´ë‹¹ ìœ„ì¹˜ì— ì™€ ìˆìœ¼ë©´ ë™ì‘í•˜ì§€ ì•Šê³  ëª¨í„° ì´ë™ì„ ëë‚¸ë‹¤ 
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_mot_retry_cnt[n_MotNum] = 10;
		mn_mot_max_retry_cnt = 3; //ìµœëŒ€ 3íšŒ ë¦¬íŠ¸ë¼ì´ ë™ì‘ì„ ìˆ˜í–‰í•œë‹¤ 
		mn_single_motmove_step[n_MotNum] = 100;
		m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		break;

	case 10:
		//20150428 james ï¿½ï¿½ï¿½ï¿½ stateï¿½ï¿½ ï¿½Ğ¾ï¿½ ï¿½ï¿½ï¿½Í»ï¿½ï¿½Â¸ï¿½ Ã¼Å©ï¿½ï¿½ Ã³ï¿½ï¿½
		if(m_dwUntil_Wait[n_MotNum][0]  < 0) m_dwUntil_Wait[n_MotNum][0] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][1] = GetCurrentTime();
		m_dwUntil_Wait[n_MotNum][2] = m_dwUntil_Wait[n_MotNum][1] - m_dwUntil_Wait[n_MotNum][0];

		//20150428 james ï¿½ï¿½ï¿½ï¿½ stateï¿½ï¿½ ï¿½Ğ¾ï¿½ ï¿½ï¿½ï¿½Í»ï¿½ï¿½Â¸ï¿½ Ã¼Å©ï¿½ï¿½ Ã³ï¿½ï¿½
		nRet_1 = COMI.Check_Motion_State(n_MotNum, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotNum, &dwMotionDone);
		if (nRet_1 == BD_GOOD) 
		{//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Â°ï¿½ mMST_STOP ï¿½Ì´ï¿½ 
			mn_single_motmove_step[n_MotNum] = 100;
		}
		else if(nRet_1 == BD_PROCEED) //ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Â°ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì´ï¿½ 
		{ 
			if(m_dwUntil_Wait[n_MotNum][2] > 15000) //max 10sec wait
			{
				if (st_handler.cwnd_list != NULL)  
				{
					sprintf(st_msg.c_normal_msg, "CCtlBd_Library::Single_Move = 10 1");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆĞ Ãâ·Â ¿äÃ»
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
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆĞ Ãâ·Â ¿äÃ»
				}
	
				cmmSxStopEmg(n_MotNum);	

				mn_single_motmove_step[n_MotNum] = 0;
			}
		}
		break;
		
	case 100:
		nRet_1 = COMI.Start_SingleMove(n_MotNum, d_MovePos, n_SpeedRate);  // í•´ë‹¹ ìœ„ì¹˜ë¡œ ì´ë™
		
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
		else if (nRet_1 == BD_RETRY)  // ë™ì‘ ì¬ì‹œë„
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
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
		
	case 200:
		if(n_RunMethod == ONLY_MOVE_START)
		{
			mn_single_motmove_step[n_MotNum] = 0;
			break;
		}

		nRet_1 = COMI.Check_SingleMove(n_MotNum, d_MovePos);  // ì´ë™ ì™„ë£Œ í™•ì¸
		if (nRet_1 == BD_GOOD)  // ì •ìƒ ì™„ë£Œ
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
		else if (nRet_1 == BD_RETRY)  // ë™ì‘ ì¬ì‹œë„
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				alarm.mstr_code.Format(_T("%02d0006"), n_MotNum);
				alarm.n_alarm_assign_section = 29902;
				mn_single_motmove_step[n_MotNum] = 0;
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
		{//ì´ë¯¸ ëª¨ë“  ëª¨í„°ì˜ í˜„ì¬ ìœ„ì¹˜ê°€ ì´ë™í•˜ë ¤ëŠ” ìœ„ì¹˜ì™€ í—ˆìš©ì˜¤ì°¨ ì•ˆì— ìˆìœ¼ë©´ ë™ì‘í•  í•„ìš”ê°€ ì—†ìœ¼ë‹ˆ ëë‚´ì.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //ìµœëŒ€ 3íšŒ ë¦¬íŠ¸ë¼ì´ ë™ì‘ì„ ìˆ˜í–‰í•œë‹¤ 
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
		else if (nRet[0] == BD_RETRY)  // ë™ì‘ ì¬ì‹œë„
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
		if (nRet[0] == BD_GOOD)  // ì •ìƒ ì™„ë£Œ
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
		else if (nRet[0] == BD_RETRY)  // ë™ì‘ ì¬ì‹œë„
		{
			// ë¬´í•œë£¨í”„ì— ë¹ ì§ˆìˆ˜ë„ ìˆì§€ë§Œ íšŸìˆ˜ë¥¼ ì§€ì •í•´ì„œ ì‚¬ìš©í•˜ë©´ ë¬¸ì œ ì—†ë‹¤
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
{//Constant speed mode ì—ì„œëŠ” SD ì‹ í˜¸ ì…ë ¥ì´ ë¬´ì‹œë©ë‹ˆë‹¤.
	int nFuncRet = BD_PROCEED;

	if(n_Enable == CTL_YES) 
	{
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_EN, cmTRUE);  
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LOGIC,cmFALSE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_MODE,cmTRUE);
		COMI.Set_Motor_IO_Property(n_AxisNum, cmSD_LATCH, cmTRUE);

		st_motor[n_AxisNum].n_sd_mv_chk = 1; //sdë¥¼ ì¡°ê±´ì„ í™•ì¸í•˜ê¸° ìœ„í•´ ì´ë™í•œë‹¤ 
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
	int nRet_1;
	double dCurrentPos;
	CString strlog;
	//	double dPos_1, dPos_2, dPos_3;

	Func.ThreadFunctionStepTrace(30, m_nSD_Elv_MoveStep[n_AxisNum]);


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
	//SD ì„¼ì„œê°€ ê¸°ì¤€ì ì„ ì¡ëŠ” ì„¼ì„œ ì´ë‹¤ 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(st_work.mn_run_status != dRUN)
	{   //time out ì‹œê°„ ì²´í¬ ë¬¸ì œ ë³´ì™„ì„ ìœ„í•´  
		if(m_bSD_MoveFlag[n_AxisNum] == true) m_bSD_MoveFlag[n_AxisNum] = false; 
	}

	switch(m_nSD_Elv_MoveStep[n_AxisNum])
	{
	case 0:	
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//íŠ¸ë ˆì´ë¥¼ ì‘ì—… ê°€ëŠ¥í•œ ì˜ì—­ì— UPí•˜ì—¬ ê³µê¸‰í•˜ëŠ” ìœ„ì¹˜, SD pos ì„¼ì„œ On ê°ì§€ í›„ ì„¼ì„œ ê¸°ì¤€ - ë°©í•­ìœ¼ë¡œ ë²—ì–´ë‚œ í›„ + ë°©í–¥ìœ¼ë¡œ P_ELV_SUPPLY_OFFSET í‹°ì¹­ë§Œí¼ up í•œí›„ ëª¨í„° ë™ì‘ì€ ì™„ë£Œí•˜ê³ , íŠ¸ë ˆì´ë¥´, ë°›ë“ ë‹¤ 
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}			
		else 
		{//910004 1 A "THERE_IS_NO_POSITION_THAT ELV MOVE_IN_LOAD_STACKER."
			CTL_Lib.Alarm_Error_Occurrence(272, dWARNING, "910004");
		}			
		break;
		///////////////////////////////////////////////////////////////////////////
		// SD Sensor ê¸°ì¤€ ìœ„ ë˜ëŠ” ì•„ë˜ì¸ì§€ í™•ì¸í•˜ì—¬, ìœ„ë©´ ë‚´ë ¤ê°€ê³ , ì•„ë˜ë©´ ì˜¬ë¼ê°„ë‹¤ 
		///////////////////////////////////////////////////////////////////////////
	case 1000: //ê¸°ì¤€ì  ìœ„ì¹˜ í™•ì¸ (SD Sensor ëŒ€ë¹„ high or ê°™ì€ ìœ„ì¹˜ì¸ì§€ì¸ì§€, lowì¸ì§€ ë‘ê°€ì§€ ìœ„ì¹˜ë¥¼ í™•ì¸í•œë‹¤)			 
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 	
		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear			

		if(nRet_1 == BD_GOOD)
		{//SDê°€ ê°ì§€ë˜ë©´ ìœ„ì— ìˆëŠ” ìƒíƒœ 
			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 6000; //ê¸°ì¤€ì ë³´ë‹¤ ìœ„ì— ìˆëŠ” ìƒíƒœë¡œ ì•„ë˜ë¡œ ë‚´ë¦¬ë©´ì„œ SDë¥¼ ì§€ë‚˜ê°€ê¸°ë¥¼ ê¸°ë‹¤ë¦¬ì 
		}
		else //if(nRet_1 == BD_ERROR)
		{//SDë„ ê°ì§€ë˜ì§€ ì•Šì•˜ê³ , SD ìœ„ì¹˜ë³´ë‹¤ ë‚®ì€ ìƒíƒœë¡œ, ìœ„ë¡œ ì˜¬ë¼ê°€ë©´ì„œ SDë¥¼ ì°¾ëŠ”ë‹¤ 
			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 2000; //traget posë¥¼ ì´ë™í•˜ëŠ” ë£¨í‹´ 
		}	 
		break;

		////////////////////////////////////////////////////////////
		//SD ì„¼ì„œë¥¼ ì°¾ìœ¼ë©´ì„œ ìœ„ë¡œ ì´ë™í•˜ë‹¤ 
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
			{//limit ì‹œê°„ ì§€ë‚¨, ì—ëŸ¬ ì²˜ë¦¬í›„ ì¬ ì‹œë„í•˜ì 
				COMI.Set_MotStop(1, n_AxisNum) ; //ê¸´ê¸‰ì •ì§€ 
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);   //ì œì¼ ë°‘ê¹Œì§€ ì´ë™í•œë‹¤ 
		if (nRet_1 == BD_GOOD) //ìœ„ë¡œ ì´ë™ í•˜ë©´ì„œ SDë¥¼ ì°¾ëŠ”ë‹¤  
		{					 				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 2100;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(272, dWARNING, alarm.mstr_code);

			if (st_handler.cwnd_list != NULL)  
			{
				sprintf(st_msg.c_normal_msg, "UnLoader Alarm : 2000");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆĞ Ãâ·Â ¿äÃ»
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2100:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_GOOD)
		{//ì •ìƒì ìœ¼ë¡œ ì²´í¬í–ˆë‹¤. 
			COMI.Set_MotStop(1, n_AxisNum) ; //ê¸´ê¸‰ì •ì§€ 

			st_motor[n_AxisNum].n_sd_mv_chk = 0; //clear
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear

			m_nSD_Elv_MoveStep[n_AxisNum] = 2200;
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_READYPOS], COMI.mn_runspeed_rate);  //2015.0407 james  //SD ìœ„ì¹˜ê¹Œì§€ ì´ë™í–ˆëŠ”ë° SDê°€ ê°ì§€ë˜ì§€ ì•Šìœ¼ë©´ íŠ¸ë ˆì´ê°€ ì—†ëŠ”ê²ƒì´ë‹¤
		if (nRet_1 == BD_GOOD)   
		{	//íŠ¸ë ˆì´ê°€ ì—†ëŠ” ìƒíƒœì¼ê²ƒì´ë‹¤, ë¯¸ë¦¬ íŠ¸ë ˆì´ë¥¼ ì²´í¬ê°€ëŠ¥í•˜ì—¬ íŠ¸ë ˆì´ê°€ ì—†ìœ¼ë©´ ì˜¬ë¼ ê°ˆ í•„ìš”ê°€ ì—†ë‹¤ 
			//ì´ê³³ì— ì˜¤ë©´ ë¬¸ì œê°€ ìˆìŒ 
			if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//íŠ¸ë ˆì´ë¥¼ ì‘ì—… ê°€ëŠ¥í•œ ì˜ì—­ì— UPí•˜ì—¬ ê³µê¸‰í•˜ëŠ” ìœ„ì¹˜, SD pos ì„¼ì„œ On ê°ì§€ í›„ ì„¼ì„œ ê¸°ì¤€ - ë°©í•­ìœ¼ë¡œ ë²—ì–´ë‚œ í›„ + ë°©í–¥ìœ¼ë¡œ P_ELV_SUPPLY_OFFSET í‹°ì¹­ë§Œí¼ up í•œí›„ ëª¨í„° ë™ì‘ì€ ì™„ë£Œí•˜ê³ , íŠ¸ë ˆì´ë¥´, ë°›ë“ ë‹¤ 
			{
				if(n_AxisNum == M_TRAY1_Z)
				{
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loading_Stacker_Tray_Exist_Check,	IO_ON); //load stacker tray check    
					if(nRet_1 == IO_OFF)
					{
						m_nSD_Elv_MoveStep[n_AxisNum] = 0;
						nFuncRet = RET_GOOD;
					}
				}
				else
				{
					//james test m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //íŠ¸ë ˆì´ê°€ ì—†ëŠ” ìƒíƒœ  
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
		{//ëª¨í„° ì•ŒëŒì€ ì´ë¯¸ ì²˜ë¦¬í–ˆìœ¼ë‹ˆ ì´ê³³ì—ì„œëŠ” ëŸ° ìƒíƒœë§Œ ë°”ê¾¸ë©´ ëœë‹¤  
			CTL_Lib.Alarm_Error_Occurrence(273, dWARNING, alarm.mstr_code);

			////////if (st_handler.cwnd_list != NULL)  
			{
				//clsMem.OnNormalMessageWrite(_T("UnLoader Alarm : 2010"));
				//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2200:
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//íŠ¸ë ˆì´ë¥¼ ì‘ì—… ê°€ëŠ¥í•œ ì˜ì—­ì— UPí•˜ì—¬ ê³µê¸‰í•˜ëŠ” ìœ„ì¹˜, SD pos ì„¼ì„œ On ê°ì§€ í›„ ì„¼ì„œ ê¸°ì¤€ - ë°©í•­ìœ¼ë¡œ ë²—ì–´ë‚œ í›„ + ë°©í–¥ìœ¼ë¡œ P_ELV_SUPPLY_OFFSET í‹°ì¹­ë§Œí¼ up í•œí›„ ëª¨í„° ë™ì‘ì€ ì™„ë£Œí•˜ê³ , íŠ¸ë ˆì´ë¥´, ë°›ë“ ë‹¤ 
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 != BD_GOOD)
			{//SD ì„¼ì„œê°€ ê°ì§€ë˜ì§€ ì•Šìœ¼ë©´ ì—ëŸ¬ 
				COMI.Set_MotStop(1, n_AxisNum) ; //ê¸´ê¸‰ì •ì§€ 

				m_bSD_MoveFlag[n_AxisNum] = false;
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //ì¬ì‹œë„  ë£¨í‹´  
				break;
			}
		}

		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//íŠ¸ë ˆì´ë¥¼ ì‘ì—… ê°€ëŠ¥í•œ ì˜ì—­ì— UPí•˜ì—¬ ê³µê¸‰í•˜ëŠ” ìœ„ì¹˜, SD pos ì„¼ì„œ On ê°ì§€ í›„ ì„¼ì„œ ê¸°ì¤€ - ë°©í•­ìœ¼ë¡œ ë²—ì–´ë‚œ í›„ + ë°©í–¥ìœ¼ë¡œ P_ELV_SUPPLY_OFFSET í‹°ì¹­ë§Œí¼ up í•œí›„ ëª¨í„° ë™ì‘ì€ ì™„ë£Œí•˜ê³ , íŠ¸ë ˆì´ë¥´, ë°›ë“ ë‹¤ 
		{
			m_dTargetPos[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_SUPPLY_OFFSET]); //í˜„ì¬ ìœ„ì¹˜ì— + P_ELV_SUPPLY_OFFSET 
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);   
		if (nRet_1 == BD_GOOD) //ì¡°ê±´ì— ë”°ë¼ ìœ„ë¡œ/ì•„ë˜ë¡œ  ì´ë™  
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 2210;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//ëª¨í„° ì•ŒëŒì€ ì´ë¯¸ ì²˜ë¦¬í–ˆìœ¼ë‹ˆ ì´ê³³ì—ì„œëŠ” ëŸ° ìƒíƒœë§Œ ë°”ê¾¸ë©´ ëœë‹¤  
			//st_sync_info.nSmema_Tray_Output_Req++;
			//if( st_sync_info.nSmema_Tray_Output_Req > 3)
			//{
				//st_sync_info.nSmema_Tray_Output_Req = 0;
				CTL_Lib.Alarm_Error_Occurrence(274, dWARNING, alarm.mstr_code);

				if (st_handler.cwnd_list != NULL)  
				{
					sprintf(st_msg.c_normal_msg, "UnLoader Alarm : 2100");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // µ¿ÀÛ ½ÇÆĞ Ãâ·Â ¿äÃ»
				}
			//}
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2210:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, m_dTargetPos[n_AxisNum], COMI.mn_runspeed_rate);  //2015.0407 james  //SD ìœ„ì¹˜ê¹Œì§€ ì´ë™í–ˆëŠ”ë° SDê°€ ê°ì§€ë˜ì§€ ì•Šìœ¼ë©´ íŠ¸ë ˆì´ê°€ ì—†ëŠ”ê²ƒì´ë‹¤
		if (nRet_1 == BD_GOOD)   
		{	//íŠ¸ë ˆì´ê°€ ì—†ëŠ” ìƒíƒœì¼ê²ƒì´ë‹¤, ë¯¸ë¦¬ íŠ¸ë ˆì´ë¥¼ ì²´í¬ê°€ëŠ¥í•˜ì—¬ íŠ¸ë ˆì´ê°€ ì—†ìœ¼ë©´ ì˜¬ë¼ ê°ˆ í•„ìš”ê°€ ì—†ë‹¤ 
			//ì´ê³³ì— ì˜¤ë©´ ë¬¸ì œê°€ ìˆìŒ 
			m_nSD_Elv_MoveStep[n_AxisNum] = 2300; //íŠ¸ë ˆì´ê°€ ìˆëŠ” ìƒíƒœ 				 
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//ëª¨í„° ì•ŒëŒì€ ì´ë¯¸ ì²˜ë¦¬í–ˆìœ¼ë‹ˆ ì´ê³³ì—ì„œëŠ” ëŸ° ìƒíƒœë§Œ ë°”ê¾¸ë©´ ëœë‹¤  
			//2016.1117
// 			st_sync_info.nSmema_Tray_Output_Req++;
// 			if( st_sync_info.nSmema_Tray_Output_Req > 3)
// 			{
				/*st_sync_info.nSmema_Tray_Output_Req = 0;*/
				CTL_Lib.Alarm_Error_Occurrence(275, dWARNING, alarm.mstr_code);

				if (st_handler.cwnd_list != NULL)  
				{
					sprintf( st_msg.c_abnormal_msg,"Motor:%d Target: %.3f Feedback: %.3f", n_AxisNum, m_dTargetPos[n_AxisNum], COMI.Get_MotCurrentPos(n_AxisNum));
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , ABNORMAL_MSG);
				}
//			}

			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 2300:
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//íŠ¸ë ˆì´ë¥¼ ì‘ì—… ê°€ëŠ¥í•œ ì˜ì—­ì— UPí•˜ì—¬ ê³µê¸‰í•˜ëŠ” ìœ„ì¹˜, SD pos ì„¼ì„œ On ê°ì§€ í›„ ì„¼ì„œ ê¸°ì¤€ - ë°©í•­ìœ¼ë¡œ ë²—ì–´ë‚œ í›„ + ë°©í–¥ìœ¼ë¡œ P_ELV_SUPPLY_OFFSET í‹°ì¹­ë§Œí¼ up í•œí›„ ëª¨í„° ë™ì‘ì€ ì™„ë£Œí•˜ê³ , íŠ¸ë ˆì´ë¥´, ë°›ë“ ë‹¤ 
		{
			nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
			if(nRet_1 == BD_GOOD)
			{//ì •ìƒì ìœ¼ë¡œ ì²´í¬í–ˆë‹¤. 					
				m_nSD_Elv_MoveStep[n_AxisNum] = 2400; // ë£¨í‹´   
			}
			else
			{//error
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //ì¬ì‹œë„  ë£¨í‹´   
			}
		}
		break;

	case 2400:
		m_nSD_Elv_MoveStep[n_AxisNum] = 2500;
		break;

	case 2500:
		dCurrentPos = COMI.Get_MotCurrentPos(n_AxisNum); 
		if(n_TargetPos_Site == P_ELV_SUPPLY_OFFSET)//íŠ¸ë ˆì´ë¥¼ ì‘ì—… ê°€ëŠ¥í•œ ì˜ì—­ì— UPí•˜ì—¬ ê³µê¸‰í•˜ëŠ” ìœ„ì¹˜, SD pos ì„¼ì„œ On ê°ì§€ í›„ ì„¼ì„œ ê¸°ì¤€ - ë°©í•­ìœ¼ë¡œ ë²—ì–´ë‚œ í›„ + ë°©í–¥ìœ¼ë¡œ P_ELV_SUPPLY_OFFSET í‹°ì¹­ë§Œí¼ up í•œí›„ ëª¨í„° ë™ì‘ì€ ì™„ë£Œí•˜ê³ , íŠ¸ë ˆì´ë¥´, ë°›ë“ ë‹¤ 
		{
			m_dSD_Supply_Pos_Backup[n_AxisNum] = fabs(dCurrentPos + st_motor[n_AxisNum].md_pos[P_ELV_SUPPLY_OFFSET]); //í˜„ì¬ ìœ„ì¹˜ì— + P_ELV_SUPPLY_OFFSET 
		}

		//m_dReference_Pos_Backup

		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear
		m_nSD_Elv_MoveStep[n_AxisNum] = 0;
		nFuncRet = RET_GOOD;
		break; 

		//
		////////////////////////////////////////////////////////////
		//SD ì„¼ì„œë¥¼ ê°ì§€í•˜ê³  ìˆìœ¼ë‹ˆ ì•„ë˜ë¡œ ë‚´ë ¤ê°€ì 
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
			{//limit ì‹œê°„ ì§€ë‚¨, ì—ëŸ¬ ì²˜ë¦¬í›„ ì¬ ì‹œë„í•˜ì 
				COMI.Set_MotStop(1, n_AxisNum) ; //ê¸´ê¸‰ì •ì§€ 
				m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
				break;
			}
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate/3);   //ë°‘ìœ¼ë¡œ ì´ë™í•œë‹¤ 
		if (nRet_1 == BD_GOOD) //ìœ„ë¡œ ì´ë™ í•˜ë©´ì„œ SDë¥¼ ì°¾ëŠ”ë‹¤  
		{					 				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 6100;
		}
		else if (nRet_1 == BD_RETRY)
		{				 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//ëª¨í„° ì•ŒëŒì€ ì´ë¯¸ ì²˜ë¦¬í–ˆìœ¼ë‹ˆ ì´ê³³ì—ì„œëŠ” ëŸ° ìƒíƒœë§Œ ë°”ê¾¸ë©´ ëœë‹¤  
			CTL_Lib.Alarm_Error_Occurrence(375, dWARNING, alarm.mstr_code);
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		break;

	case 6100:
		nRet_1 = COMI.Get_MotIOSensor(n_AxisNum, MOT_SENS_SD); 
		if(nRet_1 == BD_ERROR)
		{//ì •ìƒì ìœ¼ë¡œ ì²´í¬í–ˆë‹¤. 
			COMI.Set_MotStop(1, n_AxisNum) ; //ê¸´ê¸‰ì •ì§€ 			
			CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear

			m_nSD_Elv_MoveStep[n_AxisNum] = 7000;
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, n_AxisNum, st_motor[n_AxisNum].md_pos[P_ELV_TRAY_Z_INITPOS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)   
		{	 
			m_nSD_Elv_MoveStep[n_AxisNum] = 6200;  //ì™„ì „íˆ ë‚´ë ¸ìœ¼ë‚˜ ì•„ì§ë„ SDë£° ê°ì§€í•˜ê³  ìˆëŠ” ìƒíƒœë¡œ , TRAY FULL ìƒíƒœì´ë‹¤ 				 
		}
		else if (nRet_1 == BD_RETRY)
		{		 
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//ëª¨í„° ì•ŒëŒì€ ì´ë¯¸ ì²˜ë¦¬í–ˆìœ¼ë‹ˆ ì´ê³³ì—ì„œëŠ” ëŸ° ìƒíƒœë§Œ ë°”ê¾¸ë©´ ëœë‹¤  
			CTL_Lib.Alarm_Error_Occurrence(276, dWARNING, alarm.mstr_code);
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
		{//SD ì„¼ì„œê°€ ê°ì§€
			COMI.Set_MotStop(1, n_AxisNum) ; //ê¸´ê¸‰ì •ì§€ 

			m_bSD_MoveFlag[n_AxisNum] = false;
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //ì´ì œ ë‹¤ì‹œ ìœ„ë¡œ ì˜¬ë¼ê°€ ì •í•´ì§„ ë£¨í‹´ìœ¼ë¡œ SD ê¸°ì¤€ ìœ„ì¹˜ì— ë§ì¶˜ë‹¤    
		}
		else
		{
			m_nSD_Elv_MoveStep[n_AxisNum] = 1000; //ì´ì œ ë‹¤ì‹œ ìœ„ë¡œ ì˜¬ë¼ê°€ ì •í•´ì§„ ë£¨í‹´ìœ¼ë¡œ SD ê¸°ì¤€ ìœ„ì¹˜ì— ë§ì¶˜ë‹¤    
		}
		break;
	}	 

	if(m_nSD_Elv_MoveStep[n_AxisNum] == 0 || nFuncRet == RET_GOOD)
	{
		CTL_Lib.SD_Sensor_Enable(0, n_AxisNum, CTL_NO); //sd sensor clear
		m_nSd_Motor_MoveStart_Flag[n_AxisNum] = 0; //ëª¨í„° ì´ë™ í´ë¦¬ì–´ 
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
	//ëª¨í„° ì´ë¦„ ì •ì˜ 
	////////////////////
	switch(l_Axis)
	{//AMT8562ëŠ” 29ê°œì˜ ëª¨í„°ë¥¼ ì‚¬ìš©í•œë‹¤ 
	case -1: //ì‚¬ìš©í•˜ì§€ ì•ŠëŠ” ë£¨í‹´
		l_Axis = 0; //ì•ŒëŒ ì½”ë“œ ìƒì„±ì„ ìœ„í•´ ì´ˆê¸°í™” ì¶”ê°€ 
		strMsg_1 = _T("");		
		break;
	case 0: //0ë²ˆëª¨í„° 
		strMsg_1.Format(_T("M_TRAY1_Z[%02ld]"), l_Axis);
		break;
	case 1:  //1ë²ˆëª¨í„° 
		strMsg_1.Format(_T("M_TRAY2_Z[%02ld]"), l_Axis);
		break;
	case 2:  //2ë²ˆëª¨í„° 
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

	}//ëª¨í„° ì •ë³´ ì´ë¦„ 
		 

	//////////////////////////////////////////////////////////////////
	//ì»¤ë¯¸ì¡°ì•„ ëª¨í„° í•¨ìˆ˜ ì—ëŸ¬ ì •ë³´ ì·¨í•©
	//////////////////////////////////////////////////////////////////
	switch(l_ErrorIDNum)
	{
	case -1:        //ì‚¬ìš©í•˜ì§€ ì•ŠëŠ” ë£¨í‹´ 
		l_ErrorIDNum = 1; //ì•ŒëŒ ì½”ë“œ ìƒì„±ì„ ìœ„í•´ ì´ˆê¸°í™” ì¶”ê°€  
		strMsg_2.Format(_T(""));
		 break;	

	//ëª¨í„° êµ¬ë™ì‘ì—…ì¤‘ ì•ŒëŒ ì •ë³´
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

	//ëª¨í„° êµ¬ë™ ê´€ë ¨ ìƒíƒœ ë° ë¦¬í„´ ê´€ë ¤ ì»¤ë¯¸ì¡°ì•„ ì‚¬ìš© ì•ŒëŒ ì •ë³´
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

	//List Motion êµ¬ë™ ë™ì‘ ê´€ë ¨, ëª¨í„° ë²ˆí˜¸ ë˜ëŠ” ì´ë¦„ì— ì´ ë‚´ìš©ì„ ë”í•´ì„œ ë©”ì„¸ì§€ë¥¼ ì¶œë ¥í•œë‹¤  
	case 41: 
		strMsg_2.Format(_T("MOTOR_LIST_MOTION_LMMAPAXIS_CREATE_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case 42: 
		strMsg_2.Format(_T("MOTOR_LINEAR_MOTION_IXMAPAXIS_CREATE_ERROR[%04ld]"), abs(l_ErrorIDNum));
		 break;

	//Motor board ê´€ë ¨ ì—ëŸ¬ 
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
	case cmERR_NONE: //0: //ì •ìƒì¸ ìƒíƒœ 
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
		 strMsg_2.Format(_T("User set invalid device id. Refer to â€œDeviceIdâ€ property.[%04ld]"), abs(l_ErrorIDNum));
		 break;
	case cmERR_INVALID_HANDLE://-1020
		 strMsg_2.Format(_T("Device handle is not valid. This means that loading a device has been failed or not performed. Refer to â€œGnLoadDevcieâ€ function[%04ld]"), abs(l_ErrorIDNum));
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
		 strMsg_2.Format(_T("The installed hardware doesnâ€Ÿt support a high-resolution performance counter.[%04ld]"), abs(l_ErrorIDNum));
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
	 strMsg_ErrIDNum.Format(_T("%02ld%04ld"), l_Axis, abs(l_ErrorIDNum)); //í•„ìš”ì‹œ ì‚¬ìš©ìœ„í•´ 6ìë¦¬ì˜ ì—ëŸ¬ ì½”ë“œ ì •ì˜ 
	
	 if(n_Mode > 0)
	 {//ì¥ë¹„ì— ì•ŒëŒ ì •ë³´ ì²˜ë¦¬ 

		 //ì“°ë ˆë“œì—ì„œ ì•ŒëŒ ê´€ë ¨ ì¥ë¹„ ìŠ¤í†±ì€ ì²˜ë¦¬í•˜ê²Œ í•˜ì st_handler_info.nRunStatus = dWARNING;
	 }
	 else if(n_Mode == 0)
	 {//ëª¨í„° ê´€ë ¨ ë©”ì„¸ì§€ë§Œ ë¡œê¹…í•˜ì—¬ íŒŒì¼ì— ì“°ê³ , ì¥ë¹„ëŠ” ì •ìƒ ê°€ë™ì‹œí‚¨ë‹¤ 
		 //ê°™ì€ ë©”ì„¸ì§€ë¥¼ ê³„ì† ì“°ì§€ ëª»í•˜ê²Œ í• ê²ƒ 
		 if(str_Motor_Msg == strMsg)
		 {
			 return;
		 }
	 }

	 sprintf(mc_alarmcode, strMsg_ErrIDNum);

	 if(COMI.mn_errormsg_debug_mode)
	 {
		//OnStringToChar(strMsg, mc_normal_msg);  //msg í™”ë©´ ì¶œë ¥ 
		sprintf( mc_normal_msg, strMsg);
		COMI.Debug_File_Create(0, mc_normal_msg);    //fileë¡œ log	  
	 }

	 str_Motor_Msg = strMsg; //¸¶Áö¸· »ç¿ëÇÑ ¸Ş¼¼Áö Á¤º¸¸¦ ÀúÀå

	 if (st_handler.cwnd_list != NULL)
	 {
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	 }
}


