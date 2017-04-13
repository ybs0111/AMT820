// IO_Manager.cpp: implementation of the IO_Manager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "IO_Manager.h"
#include "FAS_HSSI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "..\common\ShareMem.h"
#include "FastechPublic_IO.h"
#include "Srcbase\ALocalization.h"

st_io_param st_io;
IO_Manager g_ioMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IO_Manager::IO_Manager()
{

}

IO_Manager::~IO_Manager()
{

}

BOOL IO_Manager::Get_In_Bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_In_Bit( iIoNum );
	}

	return bRtn;
}

int IO_Manager::Get_In_Bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_In_Bit( iIoNum );
	}

// 	return Get_In_Bit( iIoNum ); //2012,1229
	return bRtn;
}

int IO_Manager::Set_Out_Bit( int nIONo, BOOL bOnOff ) 
{
	int iRtn = CTLBD_RET_GOOD;

	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strOnOff = "0";
		if( bOnOff == TRUE )
			strOnOff = "1";

		g_sm.SetData( (SM_TYPE)nIONo, strOnOff );
	}
	else
	{
		iRtn = FAS_IO.Set_Out_Bit( nIONo, bOnOff );
	}
	return iRtn;
}

int IO_Manager::Set_IO_PortAutoEnable(int nMasterID, int nPortNo)
{
	int iRtn = CTL_YES;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = FAS_IO.Set_IO_PortAutoEnable( nMasterID, nPortNo );
	}

	return iRtn;
}

int IO_Manager::Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable)
{
	int iRtn = CTL_YES;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = FAS_IO.Set_IO_SlaveEnable( nMasterID, nPortNo, nSlaveNo, bEnable );
	}

	return iRtn;
}

BOOL IO_Manager::Get_Out_Bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_Out_Bit( iIoNum );
	}
	
	return bRtn;	
}

BOOL IO_Manager::Get_Out_Bit( int iIoNum, int iDummy )
{
	return Get_Out_Bit( iIoNum );
}

WORD IO_Manager::Get_In_Word(int nIONo)
{
	WORD wRtn = 0x00;
	if( st_handler.mn_virtual_mode == 1 )
	{
		for( int i=0; i<16; i++ )
		{
			std::string strGet = g_sm.GetData( (SM_TYPE) (nIONo * 100 + i) );
			if( strGet == "1" )
				wRtn |= (1 << i);
		}
	}
	else
	{
		wRtn = FAS_IO.Get_In_Word( nIONo );
	}
	return wRtn;
}

WORD IO_Manager::Get_Out_Word(int nIONo)
{
	WORD wRtn = 0x00;
	if( st_handler.mn_virtual_mode ==  1 )
	{
		for( int i=0; i<16; i++ )
		{
			std::string strGet = g_sm.GetData( (SM_TYPE) (nIONo * 100 + i) );
			if( strGet == "1" )
				wRtn |= (1 << i);
		}
	}
	else
	{
		wRtn = FAS_IO.Get_Out_Word( nIONo );
	}
	return wRtn;
}


int IO_Manager::set_out_bit( int nIONo, BOOL bOnOff )
{
	int iRtn = 0;
	if( st_handler.mn_virtual_mode == 1 )
		iRtn = Set_Out_Bit( nIONo, bOnOff );
	else
		iRtn = FAS_IO.set_out_bit( nIONo, bOnOff );

	return iRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_in_bit( iIoNum, iDummy );

	return bRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum, int opt, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.Get_In_Bit( iIoNum, iDummy );
	
	return bRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	int iDummy = IO_OFF;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_in_bit( iIoNum, iDummy );

	return bRtn;
}

BOOL IO_Manager::get_out_bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_Out_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_out_bit( iIoNum, iDummy );

	return bRtn;
}

st_io_param::st_io_param()
{
	int i;

	///////////////// ysyang
	st_io.i_Loading_Stacker_Tray_On_Check			=	0;						///PS0000
	st_io.i_Loading_Stacker_Tray_Ready_Check		=	1;						///PS0001
	st_io.i_Loading_Stacker_Tray_Limit_Check		=	2;						///PS0002
	st_io.i_Loading_Stacker_Tray_Lock_Check			=	3;						///PS0003
	st_io.i_Loading_Stacker_Tray_Unlock_Check		=	4;						///PS0004
	st_io.i_Loading_Stacker_Tray_Lock_Able_Check	=	7;						///PS0007
	st_io.i_UnLoading_Stacker_Tray_Lock_Able_Check	=	8;						///PS0008
	st_io.i_Loading_Stacker_Tray_Holder2_On_Check	=	9;						///PS0009
	st_io.i_Loading_Stacker_Tray_Holder2_Off_Check	=	10;						///PS0010
	st_io.i_Unloading_Stacker_Tray_On_Check			=	11;						///PS0011
	st_io.i_Unloading_Stacker_Tray_Ready_Check		=	12;						///PS0012
	st_io.i_Unloading_Stacker_Tray_Limit_Check		=	13;						///PS0013
	st_io.i_Unloading_Stacker_Tray_Lock_Check		=	14;						///PS0014
	st_io.i_Unloading_Stacker_Tray_Unlock_Check		=	15;						///PS0015


	//////////////////////카메라 추가 건 
	st_io.i_UV_Lamp_Over_Temp_Check					=	100;
	st_io.i_Camera_Y_Press_Up_Check					=	101;
	st_io.i_Camera_Y_Press_Down_Check				=	102;
	st_io.i_Camera_Y_Device_In_Check				=	103;
	st_io.i_Camera_Y_Jig_Press_Forward_Check		=	104;
	st_io.i_Camera_Y_Jig_Press_Backward_Check		=	105;


	//////////////////////
	
	st_io.i_Tray_Remover_Tray_In_Check				=	106;
	st_io.i_Tray_Remover_Z_Up_Check					=	108;					///PS0108
	st_io.i_Tray_Remover_Z_Down_Check				=	109;					///PS0109
	st_io.i_Tray_Vacuum_On_Check                    =   110;//i_Tray_Clamp_Forward_Check				=	110;					///PS0110
	st_io.i_Tray_Vacuum_Off_Check                   =   111;//i_Tray_Clamp_Backward_Check				=	111;					///PS0111
// 	st_io.i_Loading_Tr_Jig_Detect_Check				=	112;					///PS0114
	st_io.i_Loader_Transfer_Clamp_On_Check			=	112;					///PS0115
	st_io.i_Loader_Transfer_Clamp_Off_Check			=	113;					///PS0115
	st_io.i_Loader_Align_Forward_Check				=	114;					///PS0114
	st_io.i_Loader_Align_Backward_Check				=	115;					///PS0115

	st_io.i_HeatSink_Box1_OnOff_Check				=	200;
	st_io.i_HeatSink_Box2_OnOff_Check				=	201;
	st_io.i_HeatSink_Transfer_Vacuum_Check			=	202;
//모터교체
	st_io.i_HeatSink_Transfer_Clamp_On_Check		=	203;					///PS0202
	st_io.i_HeatSink_Transfer_Clamp_Off_Check		=	204;					///PS0203
	st_io.i_HeatSink_Transfer_Pad_Up_Check			=	205;
	st_io.i_HeatSink_Transfer_Pad_Down_Check		=	206;
	st_io.i_HeatSink_Reverse_0_Check				=	207;					///PS0204 비포켓 위치 
	st_io.i_HeatSink_Reverse_180_Check				=	208;					///PS0205 포켓 위치 
	st_io.i_HeatSink_Reverse_Up_Check				=	209;
	st_io.i_HeatSink_Reverse_Down_Check				=	210;
	st_io.i_HeatSink_Reverse_Clamp_Off_Check		=	211;
	st_io.i_HeatSink_Reverse_Clamp_On_Check			=	212;
	st_io.i_Dispenser_Spray_Forward_Check			=	213;
	st_io.i_Dispenser_Spray_Backward_Check			=	214;
	st_io.i_Dispenser_Device_Check					=	215;


	st_io.i_Dispenser_1_Level_Check					=	300;					///PS0206	//Dispenser부 Full Check
	st_io.i_Dispenser_2_Level_Check					=	301;					///PS0207	//Dispenser부 Empty Check
	st_io.i_Dispenser_Color_Check					=	302;
	st_io.i_Loader_Transfer_Left_Exist_Check		=	303;
	st_io.i_Loader_Transfer_Right_Exist_Check		=	304;					///PS0208

	st_io.i_Slide_Guide_Z1_Up_Check					=	305;					///PS0210
	st_io.i_Slide_Guide_Z1_Down_Check				=	306;					///PS0211
	st_io.i_Slide_Guide_X1_Forward_Check			=	307;					///PS0212
	st_io.i_Slide_Guide_X1_Backward_Check			=	308;					///PS0213
	st_io.i_Slide_Guide_Z2_Up_Check					=	309;					///PS0214
	st_io.i_Slide_Guide_Z2_Down_Check				=	310;					///PS0215
	st_io.i_Slide_Guide_X2_Up_Check					=	311;
	st_io.i_Slide_Guide_X2_Down_Check				=	312;
	st_io.i_HeatSink_Transfer_CellIn_Check			=	313;




	st_io.i_Carrier_1_Forward_Check					=	400;					///PS0300
	st_io.i_Carrier_1_Backward_Check				=	401;					///PS0301
	st_io.i_Carrier_Z_1_Up_Check					=	402;//Jig 감지			///PS0302
	st_io.i_Carrier_Z_1_Down_Check					=	403;//Jig 감지 			///PS0303	// 없음 
	st_io.i_Carrier_2_Forward_Check					=	404;					///PS0304
	st_io.i_Carrier_2_Backward_Check				=	405;					///PS0305
	st_io.i_Carrier_Z_2_Up_Check					=	406;//Jig 감지 			///PS0306
	st_io.i_Carrier_Z_2_Down_Check					=	407;//Jig 감지 			///PS0307
	st_io.i_Carrier_Z_2_PIN_Guide_Forward_Check		=	408;
	st_io.i_Carrier_Z_2_PIN_Guide_Backward_Check	=	409;
	st_io.i_Press_Up_Check							=	410;					///PS0314
	st_io.i_Press_Down_Check						=	411;					///PS0315
	st_io.i_Press_UpDown_CellIn_Check				=	412;
	st_io.i_Press_Carrier_CellIn_Check_1			=	413;
	st_io.i_Press_Carrier_CellIn_Check_2			=	414;

	st_io.i_Press_PIN_Guide_Forward_Check			=	500;
	st_io.i_Press_PIN_Guide_Backward_Check			=	501;
	st_io.i_Press_Carrier_Holder_Up_Check			=	502;
	st_io.i_Press_Carrier_Holder_Down_Check			=	503;

	st_io.i_Transfer1_Clamp_Forward_Check			=	505;					///PS0502
	st_io.i_Transfer1_Clamp_Backward_Check			=	506;					///PS0503
	st_io.i_Transfer1_Device_Check					=	507;					///PS0504

	st_io.i_Epoxy_Level_Top							=	508;
	st_io.i_Epoxy_Level_Middle						=	509;
	st_io.i_Epoxy_Level_Bottom						=	510;
	st_io.i_Epoxy_Reserve_4							=	511;

// 	st_io.i_EopxyGarbageContainerDetect				=	512;
	st_io.i_LoadCellContainerDetect					=	513;

	st_io.i_Main_Air1_Check							=	514;					///PS0514
	st_io.i_Main_Air2_Check							=	515;					///PS0515

	st_io.i_Carrier_1_On_Check						=	308;					///PS0308
	st_io.i_Carrier_2_On_Check						=	309;					///PS0309

//	st_io_def.i_Press_Forward_Check						=	312;				///PS0312
//	st_io_def.i_Press_Backward_Check					=	313;				///PS0313


	st_io.i_Loading_Tr_Jig_Detect_Check				=	512;//2017.0405 st_io.i_Device_Reverse_Input_Check				=	512;
	
	st_io.o_Loading_Stacker_Tray_Lock_Sol			=	600;					///S0600
	st_io.o_Loading_Stacker_Tray_Unlock_Sol			=	601;					///S0601


//	st_io_def.o_Loading_Stacker_Tray_Holder1_On_Sol		=	602;				///S0607
//	st_io_def.o_Loading_Stacker_Tray_Holder1_Off_Sol	=	603;				///S0608
//	st_io_def.o_Loading_Stacker_Tray_Holder2_On_Sol		=	604;				///S0609
//	st_io_def.o_Loading_Stacker_Tray_Holder2_Off_Sol	=	605;				///S0610
	st_io.o_Unloading_Stacker_Tray_Lock_Sol			=	606;					///S0611
	st_io.o_Unloading_Stacker_Tray_Unlock_Sol		=	607;					///S0612
///////////
	st_io.i_HeatSink_Box1_Guide_PIN_Forward_Sol		=   611;
	st_io.i_HeatSink_Box1_Guide_PIN_Backward_Sol	=   612;
	st_io.i_HeatSink_Box2_Guide_PIN_Forward_Sol		=   613;
	st_io.i_HeatSink_Box2_Guide_PIN_Backward_Sol	=   614;

	st_io.i_141B_Liquide_Level_Check				=	615;
	//	st_io_def.o_Epoxy_Dispensing_Sol					=	609;


	st_io.o_Tray_Remover_X_Forward_Sol				=	700;					///S0702
	st_io.o_Tray_Remover_X_Backward_Sol				=	701;					///S0703
	st_io.o_Tray_Remover_Z_Up_Sol					=	702;					///S0704
	st_io.o_Tray_Remover_Z_Down_Sol					=	703;					///S0705
	st_io.o_Tray_Vacuum_On_Sol						=   704;//2014.1013 o_Tray_Clamp_Forward_Sol					=	704;						///S0706
	st_io.o_Tray_Vacuum_Off_Sol						=   705;//2014.1013 o_Tray_Clamp_Backward_Sol					=	705;						///S0707
	st_io.o_Loader_Transfer_Clamp_On_Sol			=	706;					///S0708
	st_io.o_Loader_Transfer_Clamp_Off_Sol			=	707;					///S0709
	st_io.o_Loader_Align_Forward_Sol				=	708;					///S0710
	st_io.o_Loader_Align_Backward_Sol				=	709;					///S0711

	st_io.o_DisPenser_1_AirBlow_On_Sol				=	712;
//	st_io_def.o_Dispenser_1_AirBlow_Off_Sol				=	711;
//	st_io_def.o_Carrier_Device_Y_AirBlow_On_Sol			=	712;
//	st_io_def.o_Carrier_Device_Y_AirBlow_Off_Sol		=	713;

	st_io.o_PCB_Cleanning_Forward_Sol				=	1206;
	st_io.o_PCB_Cleanning_Backward_Sol				=	1207;
	st_io.o_PCB_Cleanning_Up_Sol				=	714;
	st_io.o_PCB_Cleanning_Down_Sol				=	715;
	st_io.o_PCB_Cleanning_Maint_Forward_Sol				=	1310;
	st_io.o_PCB_Cleanning_Maint_Backward_Sol				=	1311;

	st_io.o_Tray_Guide_Clamp_Forward_Sol			=	1300;
	st_io.o_Tray_Guide_Clamp_Backward_Sol			=	1301;


//	st_io_def.o_HeatSink_Box1_Sol						=	712;					///S0712
//	st_io.o_HeatSink_Box2_Sol						=	713;						///S0713


	st_io.o_HeatSink_Transfer_Clamp_On_Sol			=	1001;						///S0714
	st_io.o_HeatSink_Transfer_Clamp_Off_Sol			=	1002;						///S0715
	st_io.o_HeatSink_Reverse_0_Sol					=	1003;						///S1001
	st_io.o_HeatSink_Reverse_180_Sol				=	1004;						///S1002
	st_io.o_HeatSink_Reverse_Up_Sol					=	1005;
	st_io.o_HeatSink_Reverse_Down_Sol				=	1006;
	st_io.o_HeatSink_Reverse_Clamp_On_Sol			=	1007;
	st_io.o_HeatSink_Reverse_Clamp_Off_Sol			=	1008;
	st_io.o_HeatSink_Trasnfer_Pad_Up_Sol			=	1009;
	st_io.o_HeatSink_Trasnfer_Pad_Down_Sol			=	1010;
	st_io.o_Dispenser_Spread_Forward_Sol			=	1011;
	st_io.o_Dispenser_Spread_Backward_Sol			=	1012;
	st_io.o_Brush_Cyl_Up_Sol						=	1015;						///S1006
	st_io.o_Brush_Cyl_Down_Sol						=	1014;						///S1005	
	
	st_io.o_HeatSink_Box2_Guide_PIN_Forward_Sol		=	1100;
	st_io.o_HeatSink_Box2_Guide_PIN_Backward_Sol	=	1101;
	st_io.o_HeatSink_Box1_Guide_PIN_Forward_Sol		=	1104;
	st_io.o_HeatSink_Box1_Guide_PIN_Backward_Sol	=	1105;
	
	st_io.o_Slide_Guide_X2_Forward_Sol				=	1106;						///S1013	carrier jig 미는넘 Up/down
	st_io.o_Slide_Guide_X2_Backward_Sol				=	1107;						///S1014
	st_io.o_Carrier_Clamp_1_Forward_Sol				=	1108;						///S1100
	st_io.o_Carrier_Clamp_1_Backward_Sol			=	1109;						///S1101
	st_io.o_Carrier_Clamp_2_Forward_Sol				=	1110;
	st_io.o_Carrier_Clamp_2_Backward_Sol			=	1111;

	st_io.o_Carrier_Z_2_PIN_Guide_Forward_Sol		=	1112;	//찾을수 없음 ㅠㅠ 
	st_io.o_Carrier_Z_2_PIN_Guide_Backward_Sol		=	1113;

	st_io.o_Press_Up_Sol							=	1200;//device 눌러줌 		///S1114
	st_io.o_Press_Down_Sol							=	1201;						///S1115
	st_io.o_Press_PIN_Guide_Forward_Sol				=	1202;
	st_io.o_Press_PIN_Guide_Bakcward_Sol			=	1203;
	st_io.o_Press_Carrier_Holder_Up_Sol				=	1204;						///S1112
	st_io.o_Press_Carrier_Holder_Down_Sol			=	1205;						///S1113
				
	st_io.o_Dispenser_1_Liquide_On_Sol				=	1208;
	st_io.o_Dispenser_1_Liquide_Off_Sol				=	1209;
//	st_io.o_HeatSink_Vacuum_Off_Sol					=	1211;

	st_io.i_HeatSink_Type1_SFF_Check				=   1212;
	st_io.i_HeatSink_Type2_SFF_Check				=   1213;
	st_io.i_HeatSink_Reverse_Type1_Check			=   1214;
	st_io.i_HeatSink_Reverse_Type2_Check			=   1215;

	st_io.i_Brush_Liquid_Cyl_Forward_Check			=	1214;
	st_io.i_Brush_Liquid_Cyl_Backward_Check			=	1215;
// 	st_io.i_Brush_Liquid_Dispenser_On_Check;
// 	st_io.i_Brush_Liquid_Dispenser_Off_Check;


	st_io.o_PCB_Cleanning_Liquide_On_Sol			=	609;
	st_io.o_PCB_Cleanning_Liquide_Off_Sol			=	610;

	st_io.o_PCB_Cleanning_Air_Blower_Sol			=	1114;
	//////////////cleanning
	st_io.i_PCB_Cleanning_Forward_Check				=	1214;	
	st_io.i_PCB_Cleanning_Backward_Check			=	1215;		
	st_io.i_PCB_Cleanning_Up_Check					=	315;	
	st_io.i_PCB_Cleanning_Down_Check				=	314;		
	st_io.i_PCB_Cleanning_Maint_Forward_Check		=	1213;
	st_io.i_PCB_Cleanning_Maint_Backward_Check		=	1212;


	st_io.o_Transfer1_Clamp_Forward_Sol				=	1302;						///S1302
	st_io.o_Transfer1_Clamp_Backward_Sol			=	1303;						///S1303

//	st_io.o_Cleaner_Dispenser_Forward_Sol			=	1310;
//	st_io.o_Cleaner_Dispenser_Backward_Sol			=	1311;
	st_io.o_Cleaner_Maint_Forward_Sol				=	1310;
	st_io.o_Cleaner_Maint_Backward_Sol				=	1311;
	
	st_io.o_Slide_Guide_Z1_Up_Sol					=	1304;						///S1007
	st_io.o_Slide_Guide_Z1_Down_Sol					=	1305;						///S1008
	st_io.o_Slide_Guide_X1_Forward_Sol				=	1306;						///S1009
	st_io.o_Slide_Guide_X1_Backward_Sol				=	1307;						///S1010
	st_io.o_Slide_Guide_Z2_Up_Sol					=	1308;						///S1011
	st_io.o_Slide_Guide_Z2_Down_Sol					=	1309;						///S1012

	///////////////////////Camera 추가 
	/////확인 필요 1312~1315
	st_io.o_Camera_Y_Press_Up_Sol					=	1312;
	st_io.o_Camera_Y_Press_Down_Sol					=	1313;
	st_io.o_Camera_Y_Jig_Press_Forward_Sol			=	1314;
	st_io.o_Camera_Y_Jig_Press_Backward_Sol			=	1315;

	st_io.i_Auto_SwitchCheck						= 1400;							///PS1400
	st_io.i_Manual_SwitchCheck						= 1401;							///PS1401
	st_io.i_Start_SwitchCheck						= 1402;							///PS1402
	st_io.i_Stop_SwitchCheck						= 1403;							///PS1403
	st_io.i_AlarmClear_SwitchCheck					= 1404;							///PS1404
	st_io.i_Buzzer_SwitchCheck						= 1405;							///PS1405

	st_io.i_Loading_Stacker_Tray_Exist_Check		= 1406;
	st_io.i_Unloading_Stacker_Tray_Exist_Check		= 1407;

	st_io.o_Start_SwitchLamp						= 1408;							///S1400
	st_io.o_Stop_SwitchLamp							= 1409;							///S1401
	st_io.o_AlarmClear_SwitchLamp					= 1410;							///S1402
	st_io.o_Buzzer_SwitchLamp						= 1411;							///S1403
//	st_io.o_Auto_SwitchLamp;								///
//	st_io.o_Manual_SwitchLamp;							///
	st_io.o_Tower_GreenLamp							= 1412;							///S1404		
	st_io.o_Tower_YellowLamp						= 1413;							///S1405
	st_io.o_Tower_RedLamp							= 1414;							///S1406
	st_io.o_tower_lamp[0]							= 1412;
	st_io.o_tower_lamp[1]							= 1413;
	st_io.o_tower_lamp[2]							= 1414;

	st_io.o_Buzzer1									= 2012;							///S2004	
	st_io.o_Buzzer2									= 2013;							///S2005	
	st_io.o_Buzzer3									= 2014;							///S2006					
	st_io.o_Buzzer4									= 2015;							///S2007


	st_io.i_Light1_Curtain_Check					=	1500;						///PS1500
//	st_io.i_Light_Curtain_Reset_Switch;					///PS1501
	st_io.i_E1_Box_Door_Check						=	1502;						///PS1502
	st_io.i_E2_Box_Door_Check						=	1503;						///PS1503
	st_io.i_E3_Box_Door_Check						=	1504;						///PS1504
	st_io.i_E4_Box_Door_Check						=	1505;						///PS1505
	st_io.i_DoorCheck1								=	1506;						///PS1506
	st_io.i_DoorCheck2								=	1507;						///PS1507
	st_io.i_DoorCheck3								=	1508;						///PS1508
	st_io.i_DoorCheck4								=	1509;						///PS1509
	st_io.i_DoorCheck5								=	1510;						///PS1510
	st_io.i_DoorCheck6								=	1511;						///PS1511
	st_io.i_DoorCheck7								=	1512;						///PS1512
	st_io.i_DoorCheck8								=	1513;						///PS1513
	st_io.i_DoorCheck9								=	1514;						///PS1514
	st_io.i_DoorCheck10								=	1515;						///PS1515

	for ( i = 0; i < 10; i++)
	{
		st_io.i_door_openclose_chk[i]				=	1506;
	}


	st_io.i_Light2_Curtain_Check					=	1600;						///PS1500
	st_io.i_LoadingOn_SW_Check						=	1602;						///PS1602
	st_io.i_LoadingOff_SW_Check						=	1603;						///PS1602
	
	st_io.i_EMO_Switch1_Check						=	1604;						///PS1604
	st_io.i_EMO_Switch2_Check						=	1605;						///PS1605
	st_io.i_EMO_Switch3_Check						=	1606;						///PS1606
	st_io.i_MC2_Check								=	1608;						///PS1608
	st_io.i_MC3_Check								=	1609;						///PS1609

	st_io.i_HeatSinkLoadingOn_SW_Check				=	1607;							///PS1602
	st_io.i_HeatSinkLoadingOff_SW_Check				=	1610;							///PS1603
	st_io.o_Loading_On_Release						=	1611;						///S1600
	st_io.o_Safety_Door_Lock_Release				=	1612;						///S1601
	st_io.o_Loading_Off_Release						=	1613;						///S1602
	st_io.o_Loading_On_Lamp							=	1614;						///S1603
	st_io.o_Loading_Off_Lamp						=	1615;						///S1604

	st_io.i_GCP_SMD1_CP_Check						=	1700;							///PS1700
	st_io.i_GCP_SMD2_CP_Check						=	1701;							///PS1701
	st_io.i_GCP_SMD3_CP_Check						=	1702;							///PS1702
	st_io.i_GCP_SMD4_CP_Check						=	1703;							///PS1703
	st_io.i_GCP_SMD5_CP_Check						=	1704;							///PS1704
	st_io.i_GCP_SMD6_CP_Check						=	1705;							///PS1705
	st_io.i_GCP_SMD7_CP_Check						=	1706;							///PS1706
	st_io.i_GCP_SMD8_CP_Check						=	1707;							///PS1707
	st_io.i_GCP_SMD9_CP_Check						=	1708;							///PS1708
	st_io.i_GCP_SMD10_CP_Check						=	1709;							///PS1709

	st_io.o_Soft_Muting_HeatSink_Off				=	1711;
	st_io.o_Soft_Muting_HeatSink_On					=	1713;

//	st_io_def.i_MC4_Check								=	1715;							///PS1715



//	st_io_def.i_Ionizer1_On_Check						=	2000;							///PS2000
//	st_io_def.i_Ionizer1_Tip_Check						=	2001;							///PS2001
//	st_io_def.i_Ionizer2_On_Check						=	2002;							///PS2002
//	st_io_def.i_Ionizer2_Tip_Check						=	2003;							///PS2003
//	st_io_def.i_Ionizer3_On_Check						=	2004;							///PS2004
//	st_io_def.i_Ionizer3_Tip_Check						=	2005;							///PS2005

	///////////////////////Camera 추가 
	
	st_io.o_Camera_UV_Lamp_HW_On_RY1				=	2008;
	st_io.o_Camera_UV_Lamp_HW_On_RY2				=	2009;
	st_io.o_Camera_LED_Lamp_HW_On_RY1				=	2010;
	st_io.o_Camera_LED_Lamp_HW_On_RY2				=	2011;

//	st_io.o_Ionizer1_OnOff							=	2008;								///S2000
//	st_io_def.o_Ionizer2_OnOff							=	2009;								///S2001
//	st_io_def.o_Ionizer3_OnOff							=	2010;								///S2002

	//////interface
	st_io.i_Interface_Input_1						=	2100;	//InterLock
	st_io.i_Interface_Input_2						=	2101;	//HeartBit
	st_io.i_Interface_Input_3						=	2102;
	st_io.i_case_assembler_status					=	2103;

	st_io.o_Interface_Output_1						=	2108;
	st_io.o_Interface_Output_2						=	2109;
	
	st_io.o_HeatSink_Vacuum_On_Sol					=	2112;
	

	///////Timer
	st_io.o_Dispenser_Air_Timer_Start				=	2200;
	st_io.o_Dispenser_Air_Timer_Reset				=   2201;
	st_io.o_Dispenser_Air_Timer_Inhibit				=   2202;
	///////Timer
	st_io.o_Nova_Cleannign_Air_Timer_Start			=   2203;
	st_io.o_Nova_Cleannign_Air_Timer_Reset			=   2204;
	st_io.o_Nova_Cleannign_Air_Timer_Inhibit		=   2205;

////20130825유성준
	st_io.i_HeatSink_Garbage_Pos_Chk				=   2208;
	st_io.i_HeatSink_Garbage_Full_Chk_1				=   2209;
	st_io.i_HeatSink_Garbage_Full_Chk_2				=   2210;
	st_io.i_Unloader_Transfer_Device_Check			=   2212;//2015.0123

////




	st_io.o_Tray_remove_z2_up_sol					=	2300;
	st_io.o_Tray_remove_z2_dn_sol					=	2301;
	st_io.o_camera_y_clamp_sol						=	2302;
	st_io.o_camera_y_unclamp_sol					=	2303;
	
	st_io.i_Tray_remove_z2_up_chk					=	2304; 
	st_io.i_Tray_remove_z2_dn_chk					=	2305;
	st_io.i_camera_clamp_chk						=	2306;
	st_io.i_camera_unclamp_chk						=	2307;
	st_io.i_loading_stacker_tray1_type_chk			=	2308;//20130825유성준 로더 CD
	st_io.i_loading_stacker_tray2_type_chk			=	2309;//20130825유성준 로더 SND
	st_io.i_unloading_stacker_tray1_type_chk		=	2310;//20130825유성준 언로더 CD
	st_io.i_unloading_stacker_tray2_type_chk		=	2311;//20130825유성준 언로더 SND
	st_io.i_press_carrier_type1						=	2312;
	st_io.i_press_carrier_type2						=	2313;
	st_io.i_press_carrier_type3						=	2314;
	st_io.i_press_carrier_type4						=	2315;

	
	st_io.i_loading_rbt_glipper_sff_missalign_chk_1 =	2400;
	st_io.i_loading_rbt_glipper_sff_missalign_chk_2 =	2401;
	st_io.i_loading_rbt_glipper_sff_missalign_chk_3 =	2402;
	st_io.i_loading_rbt_glipper_sff_missalign_chk_4 =	2403;
	
	st_io.i_loading_rbt_glipper_tff_missalign_chk_1 =	2404;
	st_io.i_loading_rbt_glipper_tff_missalign_chk_2 =	2405;
	st_io.i_loading_rbt_glipper_tff_missalign_chk_3 =	2406;
	st_io.i_loading_rbt_glipper_tff_missalign_chk_4 =	2407;
	
	st_io.i_loading_buffer_sff_tilt_chk				=   2409;
	st_io.i_loading_buffer_tff_tilt_chk				=   2410;
}

int IO_Manager::IO_Board_Initialize()
{
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;

	if( !st_handler.mn_virtual_mode &&
		FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING) != STATUS_SUCCESS )
		return FALSE;
	
	if( IO_Port_AutoEnable( 0, 0 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 0, 1 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 0, 2 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 0, 3 ) == FALSE )		return FALSE;

	if( IO_SlaveEnable( 0, 0, 8, TRUE ) == FALSE )	return FALSE;
	if( IO_SlaveEnable( 0, 1, 8, TRUE ) == FALSE )	return FALSE;
	if( IO_SlaveEnable( 0, 2, 8, TRUE ) == FALSE )	return FALSE;
	if( IO_SlaveEnable( 0, 3, 8, TRUE ) == FALSE )	return FALSE;


	for (int i = 0; i < 4; i++)
	{
		Ret = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_10);
		Ret = FAS_IO.Get_IO_HSSISpeed(0, i, &HSSI_speed);
	}


	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x0000); // 0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x0000); // 0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0x0000); // 0번 Master, 0번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0x0000); // 0번 Master, 0번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0x0000); // 0번 Master, 0번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x0000); // 0번 Master, 0번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0x0000); // 0번 Master, 0번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x07ff); // 0번 Master, 0번 포트, 7번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0xffff); // 0번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0xffff); // 0번 Master, 1번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0xffff); // 0번 Master, 1번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0xffff); // 0번 Master, 1번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 4, 0xff00); // 0번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 5, 0x0000); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 6, 0xf800); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 7, 0xf800); // 0번 Master, 1번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(0, 2, 0, 0xff00); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 1, 0xff00); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 2, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 3, 0x000f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 4, 0x0000); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅



	SetIOBoardInit( true );
	
	return TRUE;
}

void IO_Manager::OnSet_IO_Out_Port_Clear()
{
	g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Buzzer2, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Buzzer3, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Buzzer4, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Buzzer_SwitchLamp, IO_OFF);
	
	if (g_ioMgr.Get_In_Bit(st_io.i_Main_Air1_Check) != IO_ON ||
		g_ioMgr.Get_In_Bit(st_io.i_Main_Air2_Check) != IO_ON)
	{

// 			810514 0 A "MAIN_AIR_1_OFF_CHECK_ERROR."
// 			810515 0 A "MAIN_AIR_1_OFF_CHECK_ERROR."
			
	}	
}


int IO_Manager::IO_Port_AutoEnable( int iMasterID, int iPortNo )
{
	int nRtn = Set_IO_PortAutoEnable(iMasterID, iPortNo);
	if ( nRtn != CTL_YES)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "%d Port, not enable", iPortNo);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}

		SetIOBoardInit( false );
		return FALSE;
	}

	return TRUE;
}

int IO_Manager::IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveCnt, BOOL bEnable)
{
	int nFailCnt = 0;
	for (int i = 0; i < nSlaveCnt; i++)
	{
		int nRet = Set_IO_SlaveEnable( nMasterID, nPortNo, i, bEnable);
		
		if (nRet != CTL_YES)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Slave #%d, not enable", i);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			i = -1;
			
			if (nFailCnt < 3)
			{
				nFailCnt++;
			}
			else
			{
				SetIOBoardInit( false );
				return FALSE;
			}
			continue;
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Slave #%d, enable success", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 성공 출력 요청
		}
		Sleep(100);
	}

	return TRUE;
}

void IO_Manager::set_out_reverse( int nIONo )
{
	BOOL bOn = get_out_bit( nIONo, IO_ON );

	if( bOn == IO_ON )
	{
		/*if( nIONo == st_io.o_bin_stacker1_rail_guide_fwdbwd &&
			g_ioMgr.get_in_bit( st_io.i_bin_stacker1_tray_chk ) == IO_ON )
		{
			if ( g_local.GetLocalType() == LOCAL_ENG )//20131029
			{
				DoModal_Msg( "Tray Check." );
			}
			else
			{
				DoModal_Msg( "트레이가 있습니다." );
			}
			return;
		}
		else if( nIONo == st_io.o_bin_stacker2_rail_guide_fwdbwd &&
			g_ioMgr.get_in_bit( st_io.i_bin_stacker2_tray_chk ) == IO_ON )
		{
			if ( g_local.GetLocalType() == LOCAL_ENG )//20131029
			{
				DoModal_Msg( "Tray Check." );
			}
			else
			{
				DoModal_Msg( "트레이가 있습니다." );
			}
			return;
		}
		else if( nIONo == st_io.o_bin_stacker3_rail_guide_fwdbwd &&
			g_ioMgr.get_in_bit( st_io.i_bin_stacker3_tray_chk ) == IO_ON )
		{
			if ( g_local.GetLocalType() == LOCAL_ENG )//20131029
			{
				DoModal_Msg( "Tray Check." );
			}
			else
			{
				DoModal_Msg( "트레이가 있습니다." );
			}
			return;
		}*/
	}
	
	set_out_bit( nIONo, !bOn );
}

void IO_Manager::set_out_reverse( int nFwd, int nBwd )
{
	BOOL bFwd = get_out_bit( nFwd, IO_ON );
	BOOL bBwd = get_out_bit( nBwd, IO_ON );

	if( bFwd && !bBwd )
	{
		set_out_bit( nFwd, IO_OFF );
		set_out_bit( nBwd, IO_ON );
	}
	else
	{
		set_out_bit( nFwd, IO_ON );
		set_out_bit( nBwd, IO_OFF );
	}
}
