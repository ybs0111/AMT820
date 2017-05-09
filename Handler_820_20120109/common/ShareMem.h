// ShareMem.h: interface for the ShareMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_)
#define AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

enum SM_TYPE 
{
	//ASS'Y  : BIN STACKER1
	SMT_O_BIN_STACKER_RAIL_VARIABLE_FORWARD_ONOFF				= 0,		//S0000
	SMT_O_BIN_STACKER_RAIL_VARIABLE_BACKWARD_ONOFF				= 1,		//S0001
	SMT_O_BIN1_STACKER_RAIL_GUIDE_FORWARDBACKWARD_ONOFF			= 2,		//S0002
	SMT_O_BIN1_STACKER_RAIL_PUSHER_FORWARDBACKWARD_ONOFF		= 3,		//S0003
	SMT_O_BIN2_STACKER_RAIL_GUIDE_FORWARDBACKWARD_ONOFF			= 4,		//S0004
	SMT_O_BIN2_STACKER_RAIL_PUSHER_FORWARDBACKWARD_ONOFF		= 5,		//S0005
	SMT_O_BIN3_STACKER_RAIL_GUIDE_FORWARDBACKWARD_ONOFF			= 6,		//S0006
	SMT_O_BIN3_STACKER_RAIL_PUSHER_FORWARDBACKWARD_ONOFF		= 7,

	SMT_I_BIN3_STACKER_RAIL_PUSHER_FORWARD_CHK					= 12,
	SMT_I_BIN3_STACKER_RAIL_PUSHER_BACKWARD_CHK					= 13,

	//ASS'Y  : BIN STACKER2
	SMT_I_BIN_STACKER_RAIL_VARIABLE_FORWARD_CHK					= 100,		//PS0100
	SMT_I_BIN_STACKER_RAIL_VARIABLE_BACKWARD_CHK				= 101,		//PS0101
	SMT_I_BIN1_STACKER_RAIL_GUIDE_FORWARD_CHK					= 102,		//PS0102
	SMT_I_BIN1_STACKER_RAIL_GUIDE_BACKWARD_CHK					= 103,		//PS0103
	SMT_I_BIN1_STACKER_RAIL_PUSHER_FORWARD_CHK					= 104,		//PS0104
	SMT_I_BIN1_STACKER_RAIL_PUSHER_BACKWARD_CHK					= 105,		//PS0105
	SMT_I_BIN1_STACKER_TRAY_CHK									= 106,		//PS0106
	SMT_I_BIN2_STACKER_RAIL_GUIDE_FORWARD_CHK					= 107,		//PS0107
	SMT_I_BIN2_STACKER_RAIL_GUIDE_BACKWARD_CHK					= 108,		//PS0108
	SMT_I_BIN2_STACKER_RAIL_PUSHER_FORWARD_CHK					= 109,		//PS0109
	SMT_I_BIN2_STACKER_RAIL_PUSHER_BACKWARD_CHK					= 110,		//PS0110
	SMT_I_BIN2_STACKER_TRAY_CHK									= 111,		//PS0111
	SMT_I_BIN3_STACKER_RAIL_GUIDE_FORWARD_CHK					= 112,		//PS0112
	SMT_I_BIN3_STACKER_RAIL_GUIDE_BACKWARD_CHK					= 113,		//PS0113
	SMT_I_BIN3_STACKER_TRAY_CHK									= 114,		//PS0114

	//ASS'Y  : BIN1
	SMT_O_BIN1_TRAY_ALIGN_FORWARDBACKWARD_ONOFF					= 200,		//S0200
	SMT_O_BIN1_STOPPER_UPDN_ONOFF								= 201,		//S0201
	SMT_O_BIN1_ELEVATOR_TRAY_CLAMPUNCLAMP_ONOFF					= 202,		//S0202
	SMT_I_BIN1_TRAY_IN_CHK										= 204,		//PS0200
	SMT_I_BIN1_TRAY_DIRECTION_CHK								= 205,		//PS0201
	SMT_I_BIN1_TRAY_STOP_POSITION_CHK							= 206,		//PS0202
	SMT_I_BIN1_TRAY_ALIGN_FORWARD_CHK							= 207,		//PS0203
	SMT_I_BIN1_TRAY_ALIGN_BACKWARD_CHK							= 208,		//PS0204
	SMT_I_BIN1_STOPPER_UP_CHK									= 209,		//PS0205
	SMT_I_BIN1_STOPPER_DN_CHK									= 210,		//PS0206
	SMT_I_BIN1_ELEVATOR_TRAY_CLAMP_CHK							= 211,		//PS0207
	SMT_I_BIN1_ELEVATOR_TRAY_UNCLAMP_CHK						= 212,		//PS0208
	SMT_I_REJECT_ELEVATOR_TRAY_CHK								= 215,		//PS0211

	//ASS'Y  : BIN2
	SMT_O_BIN2_TRAY_ALIGN_FORWARDBACKWARD_ONOFF					= 300,		//S0300
	SMT_O_BIN2_STOPPER_UPDN_ONOFF								= 301,		//S0301
	SMT_O_BIN2_ELEVATOR_TRAY_CLAMPUNCLAMP_ONOFF					= 302,		//S0302
	//SMT_I_BIN2_TRAY_DIRECTION_CHK								= 304,		//PS0300
	SMT_I_BIN2_TRAY_STOP_POSITION_CHK							= 304,		//PS0301
	SMT_I_BIN2_TRAY_ALIGN_FORWARD_CHK							= 306,		//PS0302
	SMT_I_BIN2_TRAY_ALIGN_BACKWARD_CHK							= 307,		//PS0303
	SMT_I_BIN2_STOPPER_UP_CHK									= 308,		//PS0304
	SMT_I_BIN2_STOPPER_DN_CHK									= 309,		//PS0305
	SMT_I_BIN2_ELEVATOR_TRAY_CLAMP_CHK							= 310,		//PS0306
	//SMT_I_BIN2_ELEVATOR_TRAY_UNCLAMP_CHK						= 311,		//PS0307
	SMT_I_BOTTOM_SAFETY_DOOR1_CHK								= 311,
	SMT_I_BOTTOM_SAFETY_DOOR2_CHK								= 312,

	//ASS'Y  : BIN3
	SMT_O_BIN3_TRAY_ALIGN_FORWARDBACKWARD_ONOFF					= 400,		//S0400
	SMT_O_BIN3_STOPPER_UPDN_ONOFF								= 401,		//S0401
	SMT_O_BIN3_ELEVATOR_TRAY_CLAMPUNCLAMP_ONOFF					= 402,		//S0402
	SMT_O_REJECT_TRANSFER_UPDN_ONOFF							= 403,		//S0403
	SMT_I_BIN3_TRAY_DIRECTION_CHK								= 404,		//PS0400
	SMT_I_BIN3_TRAY_STOP_POSITION_CHK							= 405,		//PS0401
	SMT_I_BIN3_TRAY_OUT_CHK										= 406,		//PS0402
	SMT_I_BIN3_TRAY_ALIGN_FORWARD_CHK							= 407,		//PS0403
	SMT_I_BIN3_TRAY_ALIGN_BACKWARD_CHK							= 408,		//PS0404
	SMT_I_BIN3_STOPPER_UP_CHK									= 409,		//PS0405
	SMT_I_BIN3_STOPPER_DN_CHK									= 410,		//PS0406
	SMT_I_BIN3_ELEVATOR_TRAY_CLAMP_CHK							= 411,		//PS0407
	SMT_I_BIN3_ELEVATOR_TRAY_UNCLAMP_CHK						= 412,		//PS0408
	SMT_I_REJECT_TRANSFER_TRAY_CHK								= 413,		//PS0409
	SMT_I_REJECT_TRANSFER_UP_CHK								= 414,		//PS0410
	SMT_I_REJECT_TRANSFER_DN_CHK								= 415,		//PS0411

	//ASS'Y  : ROBOT1
	SMT_O_PICKER_GRIPPER_UPDN_ONOFF								= 500,		//S0500
	SMT_O_PICKER_GRIPPER_ONOFF									= 501,		//S0501
	SMT_O_VACUUM_PICKER1_UPDN_ONOFF								= 502,		//S0502
	SMT_O_VACUUM_PICKER2_UPDN_ONOFF								= 503,		//S0503
	SMT_O_VACUUM_PICKER3_UPDN_ONOFF								= 504,		//S0504
	SMT_O_VACUUM_PICKER4_UPDN_ONOFF								= 505,		//S0505
	SMT_O_VACUUM_PICKER1_VACUUM_ONOFF							= 506,		//S0506
	SMT_O_VACUUM_PICKER2_VACUUM_ONOFF							= 507,		//S0507
	SMT_O_VACUUM_PICKER3_VACUUM_ONOFF							= 508,		//S0508
	SMT_O_VACUUM_PICKER4_VACUUM_ONOFF							= 509,		//S0509
	SMT_O_VACUUM_PICKER1_EJECT_ONOFF							= 510,		//S0510
	SMT_O_VACUUM_PICKER2_EJECT_ONOFF							= 511,		//S0511
	SMT_O_VACUUM_PICKER3_EJECT_ONOFF							= 512,		//S0512
	SMT_O_VACUUM_PICKER4_EJECT_ONOFF							= 513,		//S0513
	SMT_I_ROBOT_Z_JOG_UP_SWITCH_CHK								= 514,		//PS0500
	SMT_I_ROBOT_Z_JOG_DN_SWITCH_CHK								= 515,		//PS0501

	//ASS'Y  : ROBOT2
	SMT_I_PICKER_GRIPPER_UP_CHK									= 600,		//PS0600
	SMT_I_PICKER_GRIPPER_DN_CHK									= 601,		//PS0601
	SMT_I_PICKER_GRIPPER_ON_CHK									= 602,		//PS0602
	SMT_I_PICKER_GRIPPER_OFF_CHK								= 603,		//PS0603
	SMT_I_VACUUM_PICKER1_UP_CHK									= 604,		//PS0604
	SMT_I_VACUUM_PICKER1_DN_CHK									= 605,		//PS0605
	SMT_I_VACUUM_PICKER2_UP_CHK									= 606,		//PS0606
	SMT_I_VACUUM_PICKER2_DN_CHK									= 607,		//PS0607
	SMT_I_VACUUM_PICKER3_UP_CHK									= 608,		//PS0608
	SMT_I_VACUUM_PICKER3_DN_CHK									= 609,		//PS0609
	SMT_I_VACUUM_PICKER4_UP_CHK									= 610,		//PS0610
	SMT_I_VACUUM_PICKER4_DN_CHK									= 611,		//PS0611
	SMT_I_VACUUM_PICKER1_VACUUM_CHK								= 612,		//PS0612
	SMT_I_VACUUM_PICKER2_VACUUM_CHK								= 613,		//PS0613
	SMT_I_VACUUM_PICKER3_VACUUM_CHK								= 614,		//PS0614
	SMT_I_VACUUM_PICKER4_VACUUM_CHK								= 615,		//PS0615

	//ASS'Y  : TESTSITE1-1
	SMT_O_TESTSITE1_COK_PLATE_FORWARD_ONOFF						= 700,		//S0700
	SMT_O_TESTSITE1_COK_PLATE_BACKWARD_ONOFF					= 701,		//S0701
	SMT_O_TESTSITE1_COK_LOCKUNLOCK_ONOFF						= 702,		//S0702
	SMT_O_TESTSITE1_COK_INSERT1_FORWARDBACKWARD_ONOFF			= 703,		//S0703
	SMT_O_TESTSITE1_COK_INSERT2_FORWARDBACKWARD_ONOFF			= 704,		//S0704
	SMT_O_TESTSITE1_COK_INSERT3_FORWARDBACKWARD_ONOFF			= 705,		//S0705
	SMT_O_TESTSITE1_COK_INSERT4_FORWARDBACKWARD_ONOFF			= 706,		//S0706
	SMT_O_TESTSITE1_COK_INSERT5_FORWARDBACKWARD_ONOFF			= 707,		//S0707
	SMT_O_TESTSITE1_HI_FIX_CLAMPUNCLAMP_ONOFF					= 708,		//S0708
	SMT_O_REJECT_STACKER_GUIDE									= 709,

	SMT_I_REJECT_STACKER_GUIDE_FORWARD_CHK						= 713,
	SMT_I_REJECT_STACKER_GUIDE_BACKWARD_CHK						= 714,

	//ASS'Y  : TESTSITE1-2
	SMT_I_TESTSITE1_COK_PLATE_FORWARD_CHK						= 1000,		//PS1000
	SMT_I_TESTSITE1_COK_PLATE_BACKWARD_CHK						= 1001,		//PS1001
	SMT_I_TESTSITE1_COK_LOCK_CHK								= 1002,		//PS1002
	SMT_I_TESTSITE1_COK_UNLOCK_CHK								= 1003,		//PS1003
	SMT_I_TESTSITE1_COK_INSERT1_FORWARD_CHK						= 1004,		//PS1004
	SMT_I_TESTSITE1_COK_INSERT1_BACKWARD_CHK					= 1005,		//PS1005
	SMT_I_TESTSITE1_COK_INSERT2_FORWARD_CHK						= 1006,		//PS1006
	SMT_I_TESTSITE1_COK_INSERT2_BACKWARD_CHK					= 1007,		//PS1007
	SMT_I_TESTSITE1_COK_INSERT3_FORWARD_CHK						= 1008,		//PS1008
	SMT_I_TESTSITE1_COK_INSERT3_BACKWARD_CHK					= 1009,		//PS1009
	SMT_I_TESTSITE1_COK_INSERT4_FORWARD_CHK						= 1010,		//PS1010
	SMT_I_TESTSITE1_COK_INSERT4_BACKWARD_CHK					= 1011,		//PS1011
	SMT_I_TESTSITE1_COK_INSERT5_FORWARD_CHK						= 1012,		//PS1012
	SMT_I_TESTSITE1_COK_INSERT5_BACKWARD_CHK					= 1013,		//PS1013
	SMT_I_TESTSITE1_HI_FIX_CLAMP_CHK							= 1014,		//PS1014
	SMT_I_TESTSITE1_HI_FIX_UNCLAMP_CHK							= 1015,		//PS1015

	//ASS'Y  : TESTSITE1-3
	SMT_I_TESTSITE1_COK1_CHK									= 1100,		//PS1100
	SMT_I_TESTSITE1_COK2_CHK									= 1101,		//PS1101
	SMT_I_TESTSITE1_COK3_CHK									= 1102,		//PS1102
	SMT_I_TESTSITE1_COK4_CHK									= 1103,		//PS1103
	SMT_I_TESTSITE1_COK5_CHK									= 1104,		//PS1104
	SMT_I_TESTSITE1_COK6_CHK									= 1105,		//PS1105
	SMT_I_TESTSITE1_COK7_CHK									= 1106,		//PS1106
	SMT_I_TESTSITE1_COK8_CHK									= 1107,		//PS1107
	SMT_I_TESTSITE1_COK9_CHK									= 1108,		//PS1108
	SMT_I_TESTSITE1_COK10_CHK									= 1109,		//PS1109
	SMT_I_TESTSITE1_HI_FIX_CHK									= 1110,		//PS1110
	SMT_I_TESTSITE1_DOCKING_CHK									= 1111,		//PS1111

	//ASS'Y  : TESTSITE2-1
	SMT_O_TESTSITE2_COK_PLATE_FORWARD_ONOFF						= 1200,		//S1200
	SMT_O_TESTSITE2_COK_PLATE_BACKWARD_ONOFF					= 1201,		//S1201
	SMT_O_TESTSITE2_COK_LOCKUNLOCK_ONOFF						= 1202,		//S1202
	SMT_O_TESTSITE2_COK_INSERT1_FORWARDBACKWARD_ONOFF			= 1203,		//S1203
	SMT_O_TESTSITE2_COK_INSERT2_FORWARDBACKWARD_ONOFF			= 1204,		//S1204
	SMT_O_TESTSITE2_COK_INSERT3_FORWARDBACKWARD_ONOFF			= 1205,		//S1205
	SMT_O_TESTSITE2_COK_INSERT4_FORWARDBACKWARD_ONOFF			= 1206,		//S1206
	SMT_O_TESTSITE2_COK_INSERT5_FORWARDBACKWARD_ONOFF			= 1207,		//S1207
	SMT_O_TESTSITE2_HI_FIX_CLAMPUNCLAMP_ONOFF					= 1208,		//S1208

	//ASS'Y  : TESTSITE2-2
	SMT_I_TESTSITE2_COK_PLATE_FORWARD_CHK						= 1300,		//PS1300
	SMT_I_TESTSITE2_COK_PLATE_BACKWARD_CHK						= 1301,		//PS1301
	SMT_I_TESTSITE2_COK_LOCK_CHK								= 1302,		//PS1302
	SMT_I_TESTSITE2_COK_UNLOCK_CHK								= 1303,		//PS1303
	SMT_I_TESTSITE2_COK_INSERT1_FORWARD_CHK						= 1304,		//PS1304
	SMT_I_TESTSITE2_COK_INSERT1_BACKWARD_CHK					= 1305,		//PS1305
	SMT_I_TESTSITE2_COK_INSERT2_FORWARD_CHK						= 1306,		//PS1306
	SMT_I_TESTSITE2_COK_INSERT2_BACKWARD_CHK					= 1307,		//PS1307
	SMT_I_TESTSITE2_COK_INSERT3_FORWARD_CHK						= 1308,		//PS1308
	SMT_I_TESTSITE2_COK_INSERT3_BACKWARD_CHK					= 1309,		//PS1309
	SMT_I_TESTSITE2_COK_INSERT4_FORWARD_CHK						= 1310,		//PS1310
	SMT_I_TESTSITE2_COK_INSERT4_BACKWARD_CHK					= 1311,		//PS1311
	SMT_I_TESTSITE2_COK_INSERT5_FORWARD_CHK						= 1312,		//PS1312
	SMT_I_TESTSITE2_COK_INSERT5_BACKWARD_CHK					= 1313,		//PS1313
	SMT_I_TESTSITE2_HI_FIX_CLAMP_CHK							= 1314,		//PS1314
	SMT_I_TESTSITE2_HI_FIX_UNCLAMP_CHK							= 1315,		//PS1315

	//ASS'Y  : TESTSITE2-3
	SMT_I_TESTSITE2_COK1_CHK									= 1400,		//PS1400
	SMT_I_TESTSITE2_COK2_CHK									= 1401,		//PS1401
	SMT_I_TESTSITE2_COK3_CHK									= 1402,		//PS1402
	SMT_I_TESTSITE2_COK4_CHK									= 1403,		//PS1403
	SMT_I_TESTSITE2_COK5_CHK									= 1404,		//PS1404
	SMT_I_TESTSITE2_COK6_CHK									= 1405,		//PS1405
	SMT_I_TESTSITE2_COK7_CHK									= 1406,		//PS1406
	SMT_I_TESTSITE2_COK8_CHK									= 1407,		//PS1407
	SMT_I_TESTSITE2_COK9_CHK									= 1408,		//PS1408
	SMT_I_TESTSITE2_COK10_CHK									= 1409,		//PS1409
	SMT_I_TESTSITE2_HI_FIX_CHK									= 1410,		//PS1410
	SMT_I_TESTSITE2_DOCKING_CHK									= 1411,		//PS1411

	//ASS'Y  : COK STACKER
	SMT_O_COK_STACKER_LOCKUNLOCK_ONOFF							= 1500,		//S1500
	SMT_I_COK_STACKER_LOCK_CHK									= 1501,		//PS1500
	SMT_I_COK_STACKER_UNLOCK_CHK								= 1502,		//PS1501
	SMT_I_COK_STACKER_POSITION_CHK								= 1503,		//PS1502
	SMT_I_COK_STACKER1_TOP_CHK									= 1504,		//PS1503
	SMT_I_COK_STACKER1_BOTTOM_CHK								= 1505,		//PS1504
	SMT_I_COK_STACKER2_TOP_CHK									= 1506,		//PS1505
	SMT_I_COK_STACKER2_BOTTOM_CHK								= 1507,		//PS1506
	SMT_I_COK_STACKER3_TOP_CHK									= 1508,		//PS1507
	SMT_I_COK_STACKER3_BOTTOM_CHK								= 1509,		//PS1508
	SMT_I_COK_STACKER4_TOP_CHK									= 1510,		//PS1509
	SMT_I_COK_STACKER4_BOTTOM_CHK								= 1511,		//PS1510
	SMT_I_COK_STACKER5_TOP_CHK									= 1512,		//PS1511
	SMT_I_COK_STACKER5_BOTTOM_CHK								= 1513,		//PS1512
	SMT_I_COK_STACKER6_TOP_CHK									= 1514,		//PS1513
	SMT_I_COK_STACKER6_BOTTOM_CHK								= 1515,		//PS1514

	//ASS'Y  : SWITCH
	SMT_O_START_SWITCH_LAMP										= 1600,		//S1600
	SMT_O_STOP_SWITCH_LAMP										= 1601,		//S1601
	SMT_O_ALARM_CLEAR_LAMP										= 1602,		//S1602
	SMT_O_BUZZER_OFF_LAMP										= 1603,		//S1603
	SMT_I_START_SWITCH_CHK										= 1608,		//PS1600
	SMT_I_STOP_SWITCH_CHK										= 1609,		//PS1601
	SMT_I_ALARM_CLEAR_CHK										= 1610,		//PS1602
	SMT_I_BUZZER_OFF_CHK										= 1611,		//PS1603
	SMT_I_AUTO_MODE_SWITCH_CHK									= 1612,		//PS1604
	SMT_I_MANUAL_MODE_SWITCH_CHK								= 1613,		//PS1605
	SMT_I_FRONT_SELECT_SWITCH_CHK								= 1614,		//PS1606
	SMT_I_REAR_SELECT_SWITCH_CHK								= 1615,		//PS1607

	//ASS'Y  : TL & DOOR
	SMT_O_MAIN_AIR_ONOFF										= 1700,		//S1700
	SMT_O_TOWER_GREEN_LAMP										= 1701,		//S1701
	SMT_O_TOWER_YELLOW_LAMP										= 1702,		//S1702
	SMT_O_TOWER_RED_LAMP										= 1703,		//S1703
	SMT_O_BUZZER1_ONOFF											= 1704,		//S1704
	SMT_O_BUZZER2_ONOFF											= 1705,		//S1705
	SMT_O_DOOR_LOCK_ONOFF										= 1706,		//S1706
	SMT_I_MAIN_AIR_CHK											= 1708,		//PS1700
	SMT_I_DOOR1_OPENCLOSE_CHK									= 1709,		//PS1701
	SMT_I_DOOR2_OPENCLOSE_CHK									= 1710,		//PS1702
	SMT_I_DOOR3_OPENCLOSE_CHK									= 1711,		//PS1703
	SMT_I_DOOR4_OPENCLOSE_CHK									= 1712,		//PS1704

	//ASS'Y  : EMO & CONVEYOR
	SMT_O_BIN1_CONVEYOR_ONOFF									= 2000,		//S2000
	SMT_O_BIN2_CONVEYOR_ONOFF									= 2001,		//S2001
	SMT_O_BIN3_CONVEYOR_ONOFF									= 2002,		//S2002
	SMT_I_MC1_CHK												= 2008,		//PS2000
	SMT_I_MC2_CHK												= 2009,		//PS2001
	SMT_I_EMO_SWITCH1_CHK										= 2012,		//PS2004
	SMT_I_EMO_SWITCH2_CHK										= 2013,		//PS2005
	SMT_I_EMO_SWITCH3_CHK										= 2014,		//PS2006
	SMT_I_EMO_SWITCH4_CHK										= 2015,		//PS2007

	//ASS'Y  : SMEMA
	SMT_O_FRONT_SHUTTLE_REQUEST_SIGNAL							= 2100,		//S2100
	SMT_O_REAR_SHUTTLE_TRANSFER_SIGNAL							= 2101,		//S2101
	SMT_I_FRONT_SHUTTLE_TRANSFER_SIGNAL_CHK						= 2108,		//PS2100
	SMT_I_REAR_SHUTTLE_REQUEST_SIGNAL_CHK						= 2109,		//PS2101

	//MOTOR TARGET
	SMT_MOTOR_BIN_1_Z_TARGET,
	SMT_MOTOR_BIN_1_R_TARGET,
	SMT_MOTOR_BIN_2_Z_TARGET,
	SMT_MOTOR_BIN_2_R_TARGET,
	SMT_MOTOR_REJECT_Z_TARGET,
	SMT_MOTOR_REJECT_R_TARGET,
	SMT_MOTOR_TRAY_TR_Y_TARGET,
	SMT_MOTOR_TRAY_TR_CLAMP_TARGET,
	SMT_MOTOR_TRAY_STACKER_TARGET,
	SMT_MOTOR_ROBOT_Z_TARGET,
	SMT_MOTOR_ROBOT_X_TARGET,
	SMT_MOTOR_ROBOT_Y_TARGET,
	SMT_MOTOR_TESTSITE_1_Z_TARGET,
	SMT_MOTOR_TESTSITE_2_Z_TARGET,
	SMT_MOTOR_COK_ROBOT_Y_TARGET,
	SMT_MOTOR_COK_ROBOT_Z_TARGET,

	// MOTOR POSITION
	SMT_MOTOR_BIN_1_Z_POSITION,
	SMT_MOTOR_BIN_1_R_POSITION,
	SMT_MOTOR_BIN_2_Z_POSITION,
	SMT_MOTOR_BIN_2_R_POSITION,
	SMT_MOTOR_REJECT_Z_POSITION,
	SMT_MOTOR_REJECT_R_POSITION,
	SMT_MOTOR_TRAY_TR_Y_POSITION,
	SMT_MOTOR_TRAY_TR_CLAMP_POSITION,
	SMT_MOTOR_TRAY_STACKER_POSITION,
	SMT_MOTOR_ROBOT_Z_POSITION,
	SMT_MOTOR_ROBOT_X_POSITION,
	SMT_MOTOR_ROBOT_Y_POSITION,
	SMT_MOTOR_TESTSITE_1_Z_POSITION,
	SMT_MOTOR_TESTSITE_2_Z_POSITION,
	SMT_MOTOR_COK_ROBOT_Y_POSITION,
	SMT_MOTOR_COK_ROBOT_Z_POSITION,

	SMT_COUNT,
};

struct st_shareMem
{
	SM_TYPE smt;

	HANDLE hFMap;
	char* pInFile;
};

class ShareMem  
{
public:
	ShareMem();
	virtual ~ShareMem();

public:
	void Initialize();
	void UnInitialize();
	void SetData( SM_TYPE smt, std::string strData );
	std::string GetData( SM_TYPE smt );

protected:
	std::vector<st_shareMem> m_vecSM;
};

extern ShareMem g_sm;

#endif // !defined(AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_)
