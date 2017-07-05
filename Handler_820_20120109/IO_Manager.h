// IO_Manager.h: interface for the IO_Manager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_)
#define AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct st_io_param
{
	///////////////// ysyang
	int	i_Loading_Stacker_Tray_On_Check;				///PS0000
	int	i_Loading_Stacker_Tray_Ready_Check;				///PS0001
	int	i_Loading_Stacker_Tray_Limit_Check;				///PS0002
	int	i_Loading_Stacker_Tray_Lock_Check;				///PS0003
	int	i_Loading_Stacker_Tray_Unlock_Check;			///PS0004

	///////역삽입 체크 
	int	i_Loading_Stacker_Tray_Reverse_Input_Check;

	int	i_Loading_Stacker_Tray_Lock_Able_Check;		///PS0007
	int	i_UnLoading_Stacker_Tray_Lock_Able_Check;		///PS0008
	int	i_Loading_Stacker_Tray_Holder2_On_Check;		///PS0009
	int	i_Loading_Stacker_Tray_Holder2_Off_Check;		///PS0010
	int	i_Unloading_Stacker_Tray_On_Check;				///PS0011
	int	i_Unloading_Stacker_Tray_Ready_Check;			///PS0012
	int	i_Unloading_Stacker_Tray_Limit_Check;			///PS0013
	int	i_Unloading_Stacker_Tray_Lock_Check;			///PS0014
	int	i_Unloading_Stacker_Tray_Unlock_Check;			///PS0015


	//////////////////////카메라 추가 건 
	int	i_UV_Lamp_Over_Temp_Check;
	int	i_Camera_Y_Press_Up_Check;
	int	i_Camera_Y_Press_Down_Check;
	int	i_Camera_Y_Device_In_Check;
	int	i_Camera_Y_Jig_Press_Forward_Check;
	int i_Camera_Y_Jig_Press_Backward_Check;
	//////////////////////

	int	i_Tray_Remover_Tray_In_Check;
	int	i_Tray_Remover_Z_Up_Check;						///PS0108
	int	i_Tray_Remover_Z_Down_Check;					///PS0109
	int	i_Tray_Vacuum_On_Check;//2014.1013 i_Tray_Clamp_Forward_Check;						///PS0110
	int	i_Tray_Vacuum_Off_Check;//2014.1013 i_Tray_Clamp_Backward_Check;					///PS0111
//	int	i_Loader_Transfer_Clamp_On_Check;				///PS0114
	int	i_Loading_Tr_Jig_Detect_Check;
	int i_Loader_Transfer_Clamp_On_Check;
	int	i_Loader_Transfer_Clamp_Off_Check;				///PS0115
	int	i_Loader_Align_Forward_Check;					///PS0114
	int	i_Loader_Align_Backward_Check;					///PS0115

	//////////////cleanning
	int	i_PCB_Cleanning_Forward_Check;	
	int	i_PCB_Cleanning_Backward_Check;		
	int	i_PCB_Cleanning_Up_Check;	
	int	i_PCB_Cleanning_Down_Check;		
	int	i_PCB_Cleanning_Maint_Forward_Check;
	int	i_PCB_Cleanning_Maint_Backward_Check;

	int	i_Device_Reverse_Input_Check;


	int	i_HeatSink_Box1_OnOff_Check;					///PS0200
	int	i_HeatSink_Box2_OnOff_Check;					///PS0201
	int	i_HeatSink_Transfer_Vacuum_Check;				///PS0202
	int	i_HeatSink_Transfer_Clamp_On_Check;				///PS0203
	int	i_HeatSink_Transfer_Clamp_Off_Check;			///PS0204
	int i_HeatSink_Transfer_Pad_Up_Check;				///PS0205
	int i_HeatSink_Transfer_Pad_Down_Check;				///PS0206
	int	i_HeatSink_Reverse_0_Check;						///PS0207
	int	i_HeatSink_Reverse_180_Check;					///PS0208
	int	i_HeatSink_Reverse_Up_Check;					///PS0209
	int i_HeatSink_Reverse_Down_Check;					///PS0210
	int	i_HeatSink_Reverse_Clamp_On_Check;				///PS0211
	int i_HeatSink_Reverse_Clamp_Off_Check;				///PS0212
	int i_Dispenser_Spray_Forward_Check;				///PS0213
	int i_Dispenser_Spray_Backward_Check;				///PS0214
	int i_Dispenser_Device_Check;						///PS0215	///reverse pocket에 cell check 


	/////////////양이성 
	int	i_Brush_Liquid_Cyl_Forward_Check;
	int	i_Brush_Liquid_Cyl_Backward_Check;
	int	i_Brush_Liquid_Dispenser_On_Check;
	int	i_Brush_Liquid_Dispenser_Off_Check;


	int	i_Dispenser_1_Level_Check;	//경화제.7387					///PS0300
	int	i_Dispenser_2_Level_Check;						///PS0301
	int i_Dispenser_Color_Check;		
	int	i_Loader_Transfer_Left_Exist_Check;							///PS0302
	int	i_Loader_Transfer_Right_Exist_Check;							///PS0303
	int	i_Slide_Guide_Z1_Up_Check;						///PS0304
	int	i_Slide_Guide_Z1_Down_Check;					///PS0305
	int	i_Slide_Guide_X1_Forward_Check;					///PS0306
	int	i_Slide_Guide_X1_Backward_Check;				///PS0307
	int	i_Slide_Guide_Z2_Up_Check;						///PS0308
	int	i_Slide_Guide_Z2_Down_Check;					///PS0309
	int i_Slide_Guide_X2_Up_Check;						///PS0310
	int i_Slide_Guide_X2_Down_Check;					///PS0311
	int i_HeatSink_Transfer_CellIn_Check;

	int	i_Carrier_1_Forward_Check;						///PS0400
	int	i_Carrier_1_Backward_Check;						///PS0401
	int	i_Carrier_Z_1_Up_Check;							///PS0402
	int	i_Carrier_Z_1_Down_Check;						///PS0403
	int	i_Carrier_2_Forward_Check;						///PS0404
	int	i_Carrier_2_Backward_Check;						///PS0405
	int	i_Carrier_Z_2_Up_Check;							///PS0406
	int	i_Carrier_Z_2_Down_Check;						///PS0407
	int i_Carrier_Z_2_PIN_Guide_Forward_Check;			///PS0408
	int i_Carrier_Z_2_PIN_Guide_Backward_Check;			///PS0409
	int	i_Carrier_1_On_Check;							///PS0408
	int	i_Carrier_2_On_Check;							///PS0409
	int	i_Press_Up_Check;								///PS0314
	int	i_Press_Down_Check;								///PS0315

	int	i_Press_UpDown_CellIn_Check;			
	int	i_Press_Carrier_CellIn_Check_1;
	int	i_Press_Carrier_CellIn_Check_2;

	int	i_Press_PIN_Guide_Forward_Check;
	int	i_Press_PIN_Guide_Backward_Check;
	int i_Press_Carrier_Holder_Up_Check;
	int i_Press_Carrier_Holder_Down_Check;
	int i_Epoxy_Y_Axis_Safety_Check;

	int	i_Transfer1_Clamp_Forward_Check;				///PS0502
	int	i_Transfer1_Clamp_Backward_Check;				///PS0503
	int	i_Transfer1_Device_Check;						///PS0504
	
	int	i_Epoxy_Level_Top;
	int i_Epoxy_Level_Middle;
	int i_Epoxy_Level_Bottom;
	int i_Epoxy_Reserve_4;

	int	i_EopxyGarbageContainerDetect;
	int	i_LoadCellContainerDetect;

	int	i_Main_Air1_Check;								///PS0514
	int	i_Main_Air2_Check;								///PS0515

	int	i_Press_Forward_Check;							///PS0312
	int	i_Press_Backward_Check;							///PS0313



	int	o_Loading_Stacker_Tray_Lock_Sol;				///S0600
	int	o_Loading_Stacker_Tray_Unlock_Sol;				///S0601

	///////////////////////Camera 추가 
	int	o_Camera_Y_Press_Up_Sol;
	int	o_Camera_Y_Press_Down_Sol;
	int	o_Camera_Y_Jig_Press_Forward_Sol;
	int	o_Camera_Y_Jig_Press_Backward_Sol;

	int	o_Camera_UV_Lamp_HW_On_RY1;
	int	o_Camera_UV_Lamp_HW_On_RY2;
	int	o_Camera_LED_Lamp_HW_On_RY1;
	int	o_Camera_LED_Lamp_HW_On_RY2;

	int	o_Loading_Stacker_Tray_Holder1_On_Sol;			///S0607
	int	o_Loading_Stacker_Tray_Holder1_Off_Sol;			///S0608
	int	o_Loading_Stacker_Tray_Holder2_On_Sol;			///S0609
	int	o_Loading_Stacker_Tray_Holder2_Off_Sol;			///S0610

	int	o_Unloading_Stacker_Tray_Lock_Sol;				///S0606
	int	o_Unloading_Stacker_Tray_Unlock_Sol;			///S0607

	int	i_141B_Liquide_Level_Check;
	int o_Epoxy_Dispensing_Sol;


	int	o_Tray_Remover_X_Forward_Sol;					///S0700
	int	o_Tray_Remover_X_Backward_Sol;					///S0701
	int	o_Tray_Remover_Z_Up_Sol;						///S0702
	int	o_Tray_Remover_Z_Down_Sol;						///S0703
	int	o_Tray_Vacuum_On_Sol;							//2014.1013 o_Tray_Clamp_Forward_Sol;		///S0704
	int	o_Tray_Vacuum_Off_Sol;							//2014.1013 o_Tray_Clamp_Backward_Sol;		///S0705
	int	o_Loader_Transfer_Clamp_On_Sol;					///S0706
	int	o_Loader_Transfer_Clamp_Off_Sol;				///S0707
	int	o_Loader_Align_Forward_Sol;						///S0708
	int	o_Loader_Align_Backward_Sol;					///S0709
	int	o_HeatSink_Box1_Sol;							///S0710
	int	o_HeatSink_Box2_Sol;							///S0711
	int	o_HeatSink_Transfer_Clamp_On_Sol;				///S0712
	int	o_HeatSink_Transfer_Clamp_Off_Sol;				///S0713

	int o_HeatSink_Box1_Clamp_On_Sol;
	int o_HeatSink_Box1_Clamp_Off_Sol;
	int o_HeatSink_Box2_Clamp_On_Sol;
	int o_HeatSink_Box2_Clamp_Off_Sol;

	int	o_HeatSink_Reverse_0_Sol;						///S1001
	int	o_HeatSink_Reverse_180_Sol;						///S1002
	int o_HeatSink_Reverse_Up_Sol;
	int o_HeatSink_Reverse_Down_Sol;
	int o_HeatSink_Reverse_Clamp_On_Sol;
	int o_HeatSink_Reverse_Clamp_Off_Sol;
	int o_HeatSink_Trasnfer_Pad_Up_Sol;
	int o_HeatSink_Trasnfer_Pad_Down_Sol;
	int o_Dispenser_Spread_Forward_Sol;
	int o_Dispenser_Spread_Backward_Sol;

	int	o_DisPenser_1_AirBlow_On_Sol;						///S1003	// Dispenser Y 토출 
	int o_Dispenser_1_AirBlow_Off_Sol;
	int	o_Brush_Cyl_Down_Sol;							///S1005
	int	o_Brush_Cyl_Up_Sol;								///S1006



	int	o_HeatSink_Vacuum_On_Sol;			
	int	o_HeatSink_Vacuum_Off_Sol;

	int	i_HeatSink_Box1_Guide_PIN_Forward_Sol;
	int	i_HeatSink_Box1_Guide_PIN_Backward_Sol;
	int	i_HeatSink_Box2_Guide_PIN_Forward_Sol;
	int	i_HeatSink_Box2_Guide_PIN_Backward_Sol;
	int	o_HeatSink_Box1_Guide_PIN_Forward_Sol;
	int	o_HeatSink_Box1_Guide_PIN_Backward_Sol;
	int	o_HeatSink_Box2_Guide_PIN_Forward_Sol;
	int	o_HeatSink_Box2_Guide_PIN_Backward_Sol;

	int	o_Slide_Guide_Z1_Up_Sol;						///S1007
	int	o_Slide_Guide_Z1_Down_Sol;						///S1008
	int	o_Slide_Guide_X1_Forward_Sol;					///S1009
	int	o_Slide_Guide_X1_Backward_Sol;					///S1010
	int	o_Slide_Guide_Z2_Up_Sol;						///S1011
	int	o_Slide_Guide_Z2_Down_Sol;						///S1012
	int	o_Slide_Guide_X2_Forward_Sol;					///S1013
	int	o_Slide_Guide_X2_Backward_Sol;					///S1014

	int	o_Carrier_Clamp_1_Forward_Sol;						///S1100
	int	o_Carrier_Clamp_1_Backward_Sol;						///S1101
	int	o_Carrier_Clamp_2_Forward_Sol;
	int	o_Carrier_Clamp_2_Backward_Sol;
	int	o_Carrier_Z_2_PIN_Guide_Forward_Sol;
	int	o_Carrier_Z_2_PIN_Guide_Backward_Sol;

	int	o_Press_Up_Sol;									///S1114
	int	o_Press_Down_Sol;								///S1115
	int	o_Press_PIN_Guide_Forward_Sol;
	int	o_Press_PIN_Guide_Bakcward_Sol;
	int	o_Press_Carrier_Holder_Up_Sol;							///S1112
	int	o_Press_Carrier_Holder_Down_Sol;							///S1113
	int	o_Dispenser_1_Liquide_On_Sol;
	int	o_Dispenser_1_Liquide_Off_Sol;

	int	o_Carrier_Device_Y_AirBlow_On_Sol;						//// Dry
	int	o_Carrier_Device_Y_AirBlow_Off_Sol;						//// Dry
	int o_PCB_Cleanning_Liquide_On_Sol;
	int o_PCB_Cleanning_Liquide_Off_Sol;
	int	o_PCB_Cleanning_Air_Blower_Sol;

	int i_HeatSink_Type1_SFF_Check;
	int i_HeatSink_Type2_SFF_Check;
	int i_HeatSink_Reverse_Type1_Check;
	int i_HeatSink_Reverse_Type2_Check;

	int	o_PCB_Cleanning_Forward_Sol;
	int	o_PCB_Cleanning_Backward_Sol;
	int	o_PCB_Cleanning_Up_Sol;
	int	o_PCB_Cleanning_Down_Sol;
	int	o_PCB_Cleanning_Maint_Forward_Sol;
	int	o_PCB_Cleanning_Maint_Backward_Sol;

	int o_Tray_Guide_Clamp_Forward_Sol;
	int o_Tray_Guide_Clamp_Backward_Sol;

	int	o_Transfer1_Clamp_Forward_Sol;					///S1302
	int	o_Transfer1_Clamp_Backward_Sol;					///S1303
	
	int o_Cleaner_Dispenser_Forward_Sol;
	int o_Cleaner_Dispenser_Backward_Sol;
	int o_Cleaner_Maint_Forward_Sol;
	int o_Cleaner_Maint_Backward_Sol;

	int i_Auto_SwitchCheck;								///PS1400
	int i_Manual_SwitchCheck;							///PS1401
	int i_Start_SwitchCheck;							///PS1402
	int i_Stop_SwitchCheck;								///PS1403
	int i_AlarmClear_SwitchCheck;						///PS1404
	int i_Buzzer_SwitchCheck;							///PS1405

	int	i_Loading_Stacker_Tray_Exist_Check;				///PS1406
	int i_Unloading_Stacker_Tray_Exist_Check;			///PS1407

	int o_Start_SwitchLamp;								///S1400
	int o_Stop_SwitchLamp;								///S1401
	int o_AlarmClear_SwitchLamp;						///S1402
	int o_Buzzer_SwitchLamp;							///S1403

//	int o_Auto_SwitchLamp;								///
//	int o_Manual_SwitchLamp;							///
	int o_Tower_GreenLamp;								///S1412		
	int o_Tower_YellowLamp;								///S1413
	int o_Tower_RedLamp;								///S1414
	int o_tower_lamp[3];

	int o_Buzzer1;										///S2004	
	int o_Buzzer2;										///S2005	
	int o_Buzzer3;										///S2006					
	int o_Buzzer4;										///S2007


	int	i_Light1_Curtain_Check;							///PS1500

	int	i_E1_Box_Door_Check;							///PS1502
	int	i_E2_Box_Door_Check;							///PS1503
	int	i_E3_Box_Door_Check;							///PS1504
	int	i_E4_Box_Door_Check;							///PS1505
	int i_DoorCheck1;									///PS1506
	int i_DoorCheck2;									///PS1507
	int i_DoorCheck3;									///PS1508
	int i_DoorCheck4;									///PS1509
	int i_DoorCheck5;									///PS1510
	int i_DoorCheck6;									///PS1511
	int i_DoorCheck7;									///PS1512
	int i_DoorCheck8;									///PS1513
	int i_DoorCheck9;									///PS1514
	int i_DoorCheck10;									///PS1515

	int i_door_openclose_chk[10];

	int	i_LoadingOn_SW_Check;							///PS1602
	int	i_LoadingOff_SW_Check;							///PS1603

	int	i_HeatSinkLoadingOn_SW_Check;							///PS1602
	int	i_HeatSinkLoadingOff_SW_Check;							///PS1603

	int	i_EMO_Switch1_Check;							///PS1604
	int	i_EMO_Switch2_Check;							///PS1605
	int	i_EMO_Switch3_Check;							///PS1606
	int	i_MC2_Check;									///PS1608
	int	i_MC3_Check;									///PS1609

	int	o_Loading_On_Release;							///S1600	//// 1초 on/off muting 활성화 
	int	o_Safety_Door_Lock_Release;						///S1601
	int	o_Loading_Off_Release;							///S1602	//// 1초 on/off muting 비활성화 
	int o_Loading_On_Lamp;								///S1603
	int	o_Loading_Off_Lamp;								///S1604

	int	o_Safety_By_Pass_Relay;							///S1602

	int	i_GCP_SMD1_CP_Check;							///PS1700
	int	i_GCP_SMD2_CP_Check;							///PS1701
	int	i_GCP_SMD3_CP_Check;							///PS1702
	int	i_GCP_SMD4_CP_Check;							///PS1703
	int	i_GCP_SMD5_CP_Check;							///PS1704
	int	i_GCP_SMD6_CP_Check;							///PS1705
	int	i_GCP_SMD7_CP_Check;							///PS1706
	int	i_GCP_SMD8_CP_Check;							///PS1707
	int	i_GCP_SMD9_CP_Check;							///PS1708
	int	i_GCP_SMD10_CP_Check;							///PS1709

	int	o_Soft_Muting_HeatSink_Off;		///감지시 서보 오프 
	int	o_Soft_Muting_HeatSink_On;		///감지시 서보 오프 안됨

	int i_Light2_Curtain_Check;
	int	o_Loading_Switch_HeatSink;
	int	o_Unloading_Switch_HeatSink;

	int	i_Ionizer1_On_Check;							///PS2000
	int	i_Ionizer1_Tip_Check;							///PS2001
	int	i_Ionizer2_On_Check;							///PS2002
	int	i_Ionizer2_Tip_Check;							///PS2003
	int	i_Ionizer3_On_Check;							///PS2004
	int	i_Ionizer3_Tip_Check;							///PS2005
	int	o_Ionizer1_OnOff;								///S2000
	int	o_Ionizer2_OnOff;								///S2001
	int	o_Ionizer3_OnOff;								///S2002

	//////interface
	int	i_Interface_Input_1;	//InterLock
	int	i_Interface_Input_2;	//HeartBit
	int	i_Interface_Input_3;	//HeartBit
	int	i_case_assembler_status;	//Case Status			// 2K13/03/12/ViboX

	int	o_Interface_Output_1;
	int	o_Interface_Output_2;
	

	///////Timer
	int	o_Dispenser_Air_Timer_Start;
	int	o_Dispenser_Air_Timer_Reset;
	int	o_Dispenser_Air_Timer_Inhibit;
	///////Timer
	int	o_Nova_Cleannign_Air_Timer_Start;
	int	o_Nova_Cleannign_Air_Timer_Reset;
	int	o_Nova_Cleannign_Air_Timer_Inhibit;

	int o_Tray_remove_z2_up_sol;
	int o_Tray_remove_z2_dn_sol;
	int o_camera_y_clamp_sol;
	int o_camera_y_unclamp_sol;

//////20130825유성준
	int i_HeatSink_Garbage_Pos_Chk;
	int i_HeatSink_Garbage_Full_Chk_1;
	int i_HeatSink_Garbage_Full_Chk_2;
	int i_Unloader_Transfer_Device_Check;//2015.0123
//////

	int i_Tray_remove_z2_up_chk;
	int i_Tray_remove_z2_dn_chk;
	int i_camera_clamp_chk;
	int i_camera_unclamp_chk;
	int i_loading_stacker_tray1_type_chk;
	int i_loading_stacker_tray2_type_chk;
	int i_unloading_stacker_tray1_type_chk;
	int i_unloading_stacker_tray2_type_chk;
	int i_press_carrier_type1;
	int i_press_carrier_type2;
	int i_press_carrier_type3;
	int i_press_carrier_type4;

	int i_loading_rbt_glipper_sff_missalign_chk_1;
	int i_loading_rbt_glipper_sff_missalign_chk_2;
	int i_loading_rbt_glipper_sff_missalign_chk_3;
	int i_loading_rbt_glipper_sff_missalign_chk_4;

	int i_loading_rbt_glipper_tff_missalign_chk_1;
	int i_loading_rbt_glipper_tff_missalign_chk_2;
	int i_loading_rbt_glipper_tff_missalign_chk_3;
	int i_loading_rbt_glipper_tff_missalign_chk_4;

	int i_loading_buffer_sff_tilt_chk;
	int i_loading_buffer_tff_tilt_chk;

	st_io_param();
};
extern st_io_param st_io;

class IO_Manager  
{
public:
	IO_Manager();
	virtual ~IO_Manager();

	BOOL Get_In_Bit( int iIoNum );
	BOOL Get_In_Bit( int iIoNum, int iDummy );
	BOOL Get_Out_Bit( int iIoNum );
	BOOL Get_Out_Bit( int iIoNum, int iDummy );
	int Set_Out_Bit( int nIONo, BOOL bOnOff ) ;
	int Set_IO_PortAutoEnable(int nMasterID, int nPortNo);
	int Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable);

	WORD Get_In_Word(int nIONo);
	WORD Get_Out_Word(int nIONo);

	int set_out_bit( int nIONo, BOOL bOnOff );
	BOOL get_in_bit( int iIoNum, int opt, int iDummy );
	BOOL get_in_bit( int iIoNum, int iDummy );
	BOOL get_in_bit( int iIoNum);
	BOOL get_out_bit( int iIoNum, int iDummy );

	void set_out_reverse( int nIONo );
	void set_out_reverse( int nFwd, int nBwd );

public:
	int IO_Board_Initialize();
	void OnSet_IO_Out_Port_Clear();


	void SetIOBoardInit( bool bVal )	{ m_bIOBoardInit = bVal; }
	bool GetIOBoardInit()	{ return m_bIOBoardInit; }

protected:
	int IO_Port_AutoEnable( int iMasterID, int iPortNo );
	int IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveCnt, BOOL bEnable);

protected:
	bool	m_bIOBoardInit;
};

extern IO_Manager g_ioMgr;

#endif // !defined(AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_)
