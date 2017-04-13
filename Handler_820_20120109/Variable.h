#ifndef _VARIABLE_h__
#define _VARIABLE_h__


#include "ctlbd_variable.h"
#include "ComizoaPublic.h"
#include "Alg_Mysql.h"
#include "SrcPart/PartFunction.h"
#include <string>
#include <vector>
// #include <map>

// 
// #ifndef PASCAL
// #define PASCAL						__stdcall
// #endif	// PASCAL
// 
// #ifndef EXPORT
// #define EXPORT
// #endif	// EXPORT

enum ENUM_INIT_SITE
{
	INIT_LD_ROBOT = 0,
	INIT_ULD_ROBOT,
	INIT_LD_PLATE,
	INIT_LDSTACKER_ELV,
	INIT_EMPTYSTACKER_ELV,
	INIT_EMPTYTRAY_TRANSFER,
	INIT_HEAT_SINK_ROBOT,
	INIT_CARRIER_ROBOT,
	INIT_UNPRESS_ROBOT,
	INIT_EPOXY_ROBOT,
	INIT_DISPENSOR_ROBOT,
	INIT_VISION_ROBOT,
	MAXSITE,
};

enum LOG_TYPE
{ 
	LOG_OPERATION = 0,
	LOG_TESTINTERFACE,
	LOG_TCPIP,
	LOG_MACHINE,
	LOG_DEBUG,
	LOG_BARCODE,
	LOG_TIME,
	LOG_AUTOSOCKETOFF,
	LOG_WORK,
	LOG_ADJUSTMENT,
	LOG_HISTORY,
	LOG_PICK_FAIL,
	LOG_RUN_STOP,
	LOG_MOTOR,
	LOG_DISABLE,
	LOG_TEST,
	LOG_TOTAL = 99
};

//int nCarrierSateyflag[9];
enum SAFETY_FLAG
{
	LDPICKER = 0,
	ULDPICKER,
	UNPRESS_Y,
	EPOXY,
	HEATSINK,
	VISION,
	UNLOADCARRIER,
	LOADCARRIER,
	CARRIERMOVE,
	MAX_SAFETY_POSITION
};

#define LANGUAGE_KOREA		0		// 한글 OS
#define LANGUAGE_ENGLISH	1		// 영어 OS

#define MAX_SITE_INFO		10
#define MACHINE_MANUAL			0
#define MACHINE_AUTO			1


// *****************************************************************************
//////////vision camera
#define MEASURE_RESULT_RCV_7387		0
#define MEASURE_RESULT_RCV_EPOXY	1
#define MEASURE_RESULT_RCV_HEATSINK	2

#define HOST_CMD_7387_MEASURE		1
#define HOST_CMD_EPOXY_MEASURE		2
#define HOST_CMD_HEATSINK_MEASURE	3
#define HOST_CMD_INSPECT_OS_VIEW	4
#define HOST_CMD_INSPECT_OS_DISABLE	5
#define HOST_CMD_RECIPE_CHANGE		6
#define HOST_CMD_HEATSINK_MEASURE2	7
#define HOST_CMD_HEATSINK_MEASURE3	8
#define HOST_CMD_NOBA_POS			9
#define HOST_CMD_HEATSINK_POS		10

#define HEATSINK_PICKER_PITCH_PORT  0//20130716


// *****************************************************************************
// 사용자 정의 메시지 ID 선언                                                   
// *****************************************************************************
#define WM_FORM_CHANGE			WM_USER+200   // 화면이 전환된 때 해당 전환 상태 정보를 전송하기 위한 사용자 정의 메시지 

#define WM_STATUS_CHANGE		WM_USER+201   // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 

#define WM_LIST_DATA			WM_USER + 6   // 통신 메시지 출력 리스트 대화 상자에 
                                              // 시리얼 포트를 통해 송/수신된 메시지를 출력하기 위한 사용자 정의 메시지 
#define WM_DATA_SEND			WM_USER + 7   // Data를 전송하기 위한 화면에서 정보를 전송하기 위해 정의한 사용자 정의 함수

#define WM_WORK_END				WM_USER + 8   // 테스트 작업 완료 시 정보를 메인 화면에 출력시키기 위한 사용자 정의 메시지 추가 

#define WM_BASIC_APPLY			WM_USER + 10  // BASIC DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 

#define WM_MAINTENANCE_APPLY	WM_USER + 11  // Maintenance DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가
#define WM_INTERFACE_APPLY		WM_USER + 13
#define	WM_WAITTIME_APPLY		WM_USER + 15  // Wait Time DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 

#define	WM_MODULESITE_APPLY		WM_USER + 16
#define	WM_HEATSINKSITE_APPLY	WM_USER + 17
#define	WM_MODULEROBOT_APPLY	WM_USER + 18
#define	WM_HEATSINKROBOT_APPLY	WM_USER + 19
#define	WM_DISPENSER_APPLY		WM_USER + 20
#define	WM_MOTORSPEED_APPLY		WM_USER + 21
#define WM_SERIAL_PORT			WM_USER + 25
#define	WM_TESTREFERENCE_MANUAL		WM_USER + 21

#define	WM_LOTOPEN_APPLY		WM_USER + 26
#define	WM_LOTSTART_APPLY		WM_USER + 27
#define WM_MAINFRAME_WORK		WM_USER + 28
#define	WM_MAIN_EVENT			WM_USER + 29

#define WM_DATA_INIT_SAVE       WM_USER + 300 // apply버튼시 저장까지, 해당 화면의 데이터 적용(Init)
#define WM_DATA_CHANGE          WM_USER + 301 // apply버튼시 저장까지, 해당 화면의 데이터 적용(Init)

#define WM_CLIENT_MSG			WM_USER + 500
#define WM_CLIENT_MSG_1			WM_USER + 500
#define WM_CLIENT_MSG_2			WM_USER + 501
#define WM_CLIENT_MSG_3			WM_USER + 502
#define WM_CLIENT_MSG_4			WM_USER + 503
#define WM_CLIENT_MSG_5			WM_USER + 504
#define WM_CLIENT_MSG_6			WM_USER + 505
#define WM_CLIENT_MSG_7			WM_USER + 506
#define WM_CLIENT_MSG_8			WM_USER + 507
#define WM_CLIENT_MSG_9			WM_USER + 508
#define WM_CLIENT_MSG_10		WM_USER + 509

#define WM_SERVER_MSG			WM_USER + 550
#define WM_SERVER_MSG_1			WM_USER + 550
#define WM_SERVER_MSG_2			WM_USER + 551
#define WM_SERVER_MSG_3			WM_USER + 552
#define WM_SERVER_MSG_4			WM_USER + 553
#define WM_SERVER_MSG_5			WM_USER + 554
#define WM_SERVER_MSG_6			WM_USER + 555
#define WM_SERVER_MSG_7			WM_USER + 556
#define WM_SERVER_MSG_8			WM_USER + 557
#define WM_SERVER_MSG_9			WM_USER + 558
#define WM_SERVER_MSG_10		WM_USER + 559

#define INITIAL_INDEX_DUMP		0	
#define INITIAL_TEST_DUMP		1
#define INITIAL_INDEX_TABLE		2
// *****************************************************************************

// *****************************************************************************
//  네트웍 메세지 정의 장비쪽에서 BPC로 보내는 메세지.
// *****************************************************************************
#define NW_LOT_START_RPY				1
#define NW_LOT_CANCEL_RPY				2
#define NW_PCBOX_START_RPY				3
#define NW_PCBOX_END_RPY				4
#define NW_PCBOX_CHANGE_RPY				5
#define NW_ENABLE_RPY					6
#define NW_DISABLE_RPY					7
#define NW_SLOT_MASK_RPY				8
#define NW_LOT_START_ENABLE_RPY			9
#define NW_PCBOX_REMOVE_RPY				10
#define NW_PCBOX_ERROR_RPY				11

#define NW_PCBOX_OFF					12
#define NW_UNLOAD_COMPT					13
#define NW_CANCEL_END					14
#define NW_LOT_END						15
#define NW_PCBOX_DISABLE				16
#define NW_VIDEO_TIMEOUT				17
#define NW_HOLD_TIMEOUT					18
#define NW_PCBOX_LOT_INFO				19
#define NW_FAIL_INFO_SEND				20
#define NW_FAIL_REGNUMBER_REQUEST		21
#define NW_FAIL_INFO_SERACH				22
#define NW_FAIL_INFO_SAVE				23
#define NW_FAIL_INFO_DELETE				24
// 새로 추가 됨.
#define NW_LOT_DISPLAY					25
#define NW_EQP_LOT_START				26
#define NW_EQP_LOT_CANCEL				27
#define NW_EQP_LOT_END					28

// *****************************************************************************
//  네트웍 메세지 정의 BPC쪽에서 장비로 보내는 메세지.
// *****************************************************************************
#define NW_LOT_START					29
#define NW_LOT_CANCEL					30
#define NW_PCBOX_START					31
#define NW_PCBOX_END					32
#define NW_PCBOX_CHANGE					33
#define NW_ENABLE						34
#define NW_DISABLE						35
#define NW_SLOT_MASK					36
#define NW_LOT_START_ENABLE				37
#define NW_PCBOX_REMOVE					38
#define NW_PCBOX_ERROR					39

#define NW_PCBOX_LOT_INFO_RPY			40
#define NW_FAIL_INFO_SEND_RPY			41
#define NW_FAIL_REGNUMBER_REQUEST_RPY	42
#define NW_FAIL_INFO_SERACH_RPY			43
#define NW_FAIL_INFO_SAVE_RPY			44
#define NW_FAIL_INFO_DELETE_RPY			45
// 새로 추가 됨
#define NW_LOT_DISPLAY_RPY				46
#define NW_EQP_LOT_START_RPY			47
#define NW_EQP_LOT_CANCEL_RPY			48
#define NW_EQP_LOT_END_RPY				49
// *****************************************************************************

#define NW_PCBOX_ENABLE					50
#define NW_PGM_START					60
//#define NW_FAIL_REGNUMBER_REQUEST		61
// *****************************************************************************
//  List Box 출력용 메세지 정의                                                 
// *****************************************************************************
#define RECEIVE_MSG			1
#define SEND_MSG			2
#define NORMAL_MSG			3
#define ABNORMAL_MSG		4
#define MACHINE_INFO		5
#define DEVICE_INFO			6

// *****************************************************************************
#define MAX_RETEST_COUNT	10

enum ENUM_WM_MESSAGE
{
	WM_DRAW_UI_MAIN			= WM_USER + 30,
	WM_DRAW_DATA_MAIN,
	WM_DRAW_DATA_LOT,
	WM_MOTOR_COMPLETION,
};

// *****************************************************************************
//  Main Screen 출력용 메세지 정의                                              
// *****************************************************************************
#define MAIN_TIMEINFO		9
#define MAIN_COUNTINFO		10
#define MAIN_LOTINFO		21
#define MAIN_CYCLETIME		22
#define MAIN_FTPDOWN        23
//kwlee 2017.0413
#define MAIN_TOP_INFO       24
#define MAIN_BTM_INFO       25
// *****************************************************************************


// *****************************************************************************
//  Title Bar 출력용 메세지 정의                                                
// *****************************************************************************
#define FORM_NAME			1
#define MACHINE_STATUS		2
#define DEVICE_MODE			3
#define INTERFACE_MODE		4
#define PRESS_MODE			5
#define FILE_MODE			6
#define MODEL_MODE			7
//kwlee 2017.0406
#define MODE_7387           8
#define MODE_3874           9
#define MODE_BCR           10
// *****************************************************************************

// *****************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                         
// *****************************************************************************
#define TIMECOUNT			5
#define ELEVATOR_STRATUMS	6
// *****************************************************************************

// *************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                          
// *************************************************************************
#define dSTOP				0
#define dRUN				1
#define dJAM				2
#define dLOTEND				3
#define dINIT				4
#define dWARNING			5
#define dLOCK				6
#define dSELFCHECK			7
#define dMAINT				8
#define dREINSTATE			9	//복귀동작중
#define dUNLOAD				10
#define dRECOVERY			11

#define eWARNING			0
#define eRUNNING			1
#define eSTOP				2
#define eERROR				3

#define IO_CLEAR			-1
#define NO					0
#define YES					1
#define READY				2

#define IO_ON				1
#define IO_OFF				0
#define IO_ERROR			2
#define IO_CHK_ERROR		4

#define IO_IN				1
#define IO_OUT				0
#define IO_NONE				2

#define IO_CLAMP			1
#define IO_UNCLAMP			0

#define IO_HOLD				0
#define IO_RELEASE			1

#define IO_DOWN				1
#define IO_UP				0

#define IO_PROCEED			2
#define IO_TIMEOUT			3

#define IO_LEFT				0
#define IO_RIGHT			1

#define IO_CLOSE			0
#define IO_OPEN				1

#define IO_GOOD				10
#define IO_FAIL				11

#define RET_PROCEED			0
#define RET_GOOD			1
#define RET_ERROR			2
#define RET_RETRY			3
#define RET_ERR_SAFE		4
#define RET_SKIP				5
#define RET_ABORT			6

#define RET_DOOROPEN		9
#define RET_ERROR_SAFETY	10
#define RET_CASSETTE		11
#define RET_DANGER			12
#define RET_CYLINDER		13
#define RET_POWER_ON		14
#define RET_POWER_OFF		15

#define FUN_ING				0
#define FUN_END				1
#define FUN_ERR				2
#define FUN_CTU				3

#define PICKER				0
#define FINGER				1

#define TIMECOUNT			5
#define ELEVATOR_STRATUMS	6

#define RET_PICKER_NOT_FIND		100  //피커에 더이상 작업할 공간이 없으면 
#define RET_TRAY_NOT_FIND		200  //트레이에 더이상 작업할 공간이 없으면 

// *************************************************************************

// *************************************************************************
// RS-232C 시리얼 통신 시 사용되는 메시지 정의                                   
// -> MAX_PORT : 기본 포트 2개로 구성되어 있다						             
// *************************************************************************
#define MAX_PORT				10	// 생성할 시리얼 포트 갯수
#define COM_ERROR				-1	// 에러 메시지
#define LOT_BARCODE_PORT		1		// 현재 BCR READ 포트 번호
#define DEVICE_BARCODE_PORT		2		// 현재 BCR READ 포트 번호

#define BCR_PORT	        2 //kwlee 201704.12
#define COM_PORT			0
#define COM_BAUDRATE		1
#define COM_DATA			2
#define COM_STOP			3
#define COM_PARITY			4

//#define TESTER_PORT	2		// 현재 Tester Communication 포트 번호
//#define COM_PORT   2	// 현재 사용 중인 시리얼 포트 번호
 
#define BUF_SIZE						1000	// 버퍼 크기
#define MAX_TRAY_ROW			30
#define MAX_TRAY_COL			30
// *************************************************************************

// *************************************************************************
//  Motor 번호 정의                                                         
// *************************************************************************
#define MAXMOTOR		21
#define MAXBOARD		4
#define MOTTIMEOUT		60000

//////////////////////////////////////////////////////////
//MOTOR 정의 
//모터 수 총 12개 
//모터보드 : 총 2개 모터보드 사용 ==> 8축 1개, 4축 1개 
//////////////////////////////////////////////////////////

// *************************************************************************

// *************************************************************************
//  각 화면 모터 이동 명령 처리 타이머 선언
// *************************************************************************

// *************************************************************************
// *************************************************************************
// I/O 모듈 갯수 및 포트 최대 갯수 선언한다                                      
// *************************************************************************
#define MAX_IO				2415

#define MAX_MASTER_IO		1
#define MAX_PORT_IO			4
#define MAX_SLAVE_IO		8
// *************************************************************************
#define M_BARCODE_PORT		0
#define C_BARCODE_PORT		1
// *************************************************************************
//  MOTOR 보드 관련 변수 선언한다                                                
// *************************************************************************
#define MOT_GOOD			1
#define MOT_PROGRESS		2
#define MOT_RETRY			3
#define MOT_ERROR			4

#define PLUS				1
#define MINUS				0
#define RUN_MODE			1
#define MANUAL_MODE			2

#define POWER_OFF			0
#define POWER_ON			1

#define ELM					0
#define ELP					1
#define HOME				2

#define MOT_ACC				0
#define MOT_DEC				1
#define MOT_JOG				2
#define MOT_ORG				3
#define MOT_FEEDRATE		4
#define MOT_INTERACC		5
#define MOT_ORGMETHOD		6
#define MOT_ORGACCDEC		7

#define MOT_FAST			10
#define MOT_SLOW			20

#define DBIT0		0x0001 
#define DBIT1		0x0002 
#define DBIT2		0x0004 
#define DBIT3		0x0008 
#define DBIT4		0x0010 
#define DBIT5		0x0020 
#define DBIT6		0x0040 
#define DBIT7		0x0080 

#define DBIT8		0x0100 
#define DBIT9		0x0200 
#define DBIT10	0x0400 
#define DBIT11	0x0800 
#define DBIT12	0x1000 
#define DBIT13	0x2000 
#define DBIT14	0x4000 
#define DBIT15	0x8000 

#define DBIT16	0x00010000 
#define DBIT17	0x00020000 
#define DBIT18	0x00040000 
#define DBIT19	0x00080000 
#define DBIT20	0x00100000 
#define DBIT21	0x00200000 
#define DBIT22	0x00400000 
#define DBIT23	0x00800000 

#define DBIT24	0x01000000 
#define DBIT25	0x02000000 
#define DBIT26	0x04000000 
#define DBIT27	0x08000000 
#define DBIT28	0x10000000 
#define DBIT29	0x20000000 
#define DBIT30	0x40000000 
#define DBIT31	0x80000000 
// *************************************************************************

///// test bin  result
#define BIN_NOTUESD			0
#define BIN_LDBUFFERBIN		1
#define BIN_RETESTBIN		2
#define BIN_GOODBIN			3
#define BIN_REJECTBIN		4
#define BIN_OSFAILBIN		5
#define BIN_FUNCFAILBIN		6
#define BIN_CHECKOFF		7  //bin 정보를 체크하지 않는다 
#define BIN_REBOOT			8
#define BIN_GOODBIN1		9
#define BIN_GOODBIN2		10
#define BIN_CLEAR			-1
#define BIN_INS				11
#define BIN_END				12
#define BIN_RETEST			13
#define BIN_NONE			14
#define BIN_HOLD			15
#define BIN_READY			16
#define BIN_REJECTBIN_R		17
#define BIN_GOODBIN_R		18

#define TRAY_NOTUSED		0
#define TRAY_LOADER			1
#define TRAY_UNLOADER		2

#define TRAYCOL				0
#define TRAYROW				1

#define TRAY_X				0
#define TRAY_Y				1

#define PICKERSTATUS		10
#define PICKERSTATUS_ON		11
#define PICKERSTATUS_OFF	12

#define MODULETRAY_X		25
#define MODULETRAY_Y		25
#define HEATSINKTRAY_X		25
#define HEATSINKTRAY_Y		25

#define PICKERNUM			2

#define PICKERINFO			4

#define LINEAR_PULSE		0
#define MULTI_PULSE			1

#define	MLDPICKER			0
#define	MULDPICKER			1
#define	HLDPICKER			2
#define	HLDGRIPPER			3

#define BASE_LOAD			0
#define BASE_WORK			2

////// Tray Data 관리용
#define MODULETRAY			1
#define HEATSINKTRAY		2

#define COUNT_NO			0	// 처음 디바이스 갯수 및 공간 갯수를 체크 한다 
#define COUNT_YES			1   // 전체 스 갯수 및 공간 갯수를 체크 한다 

#define DVC_NO				0
#define DVC_YES				1

#define DVC_TEST_START		0
#define DVC_TEST_END		1

#define INFO_NO				0
#define INFO_YES			1

#define PICKDVC				0
#define PLACEDVC			1

#define CASSETTE_PARTIAL	2
////// Tray or Buffer Data 관리용
#define MLOADTRAY			1
#define MUNLOADTRAY			2
#define HLOADTRAY			3
#define MODULEBASE			4

#define SITE_FRONT			0
#define SITE_REAR			1

///////////////////
#define RET_NOWHERE			-1000		// 해당 데이터가 존재하지 않을때 
#define RET_NOSPACE			-2000		// 공간이 없을때 
#define RET_EMPTYDEVICE		-3000		// 디바이스가 모두 없을때 
#define RET_FULLDEVICE		-4000		// 디바이스가 FULL
#define RET_LOADTRAYSKIP	-10000		// loader key skip

#define RET_PICKERALLYES 	-5000	// 피커 정보가 문제일때 
#define RET_PICKERALLNO 	-6000	// 피커 정보가 문제일때 

#define ELEVATOR_NO			0
#define ELEVATOR_READY		1
#define ELEVATOR_FULL		2
#define ELEVATOR_EMPTY		3

#define MODULE_EL			0
#define HEATSINK_EL			1

#define WITH_DVC			0
#define WITHOUT_DVC			1
#define WITHOUT_TRAY		2
#define WITHOUT_CASSETTE	3

#define WITH_JIG			0
#define WITHOUT_JIG			1

#define MODULE_NOTUSE		0		// 없음.
#define MODULE_READY		1		// 작업전.
#define MODULE_WORKEND		2		// 작업후.

#define SINGLE_MODE			0
#define DOUBLE_MODE			1

#define OFF_LINE_MODE		0
#define ON_LINE_MODE		1

#define NOT_USE             0
#define USE                 1

// 모듈 엘레베이터 Type
#define TYPE_LOAD			0
#define TYPE_LOADUNLOAD		1
#define TYPE_UNLOAD			2

#define GUIDE_OPEN_START	4
#define GUIDE_OPEN_END		5
#define GUIDE_CLOSE_START	6
#define GUIDE_CLOSE_END		7

#define WHIRLER_START		8
#define WHIRLER_END			9
#define WHIRLER_RELEASE		10
#define WHIRLER_WORKEND		11

#define DISPENSER_START		12
#define DISPENSER_END		13
#define DISPENSER_WORKEND	14

#define CLEANER_START		15
#define CLEANER_END			16

#define LOTEND_READY		17

#define BUTTON_MOUSE_DN		1100
#define BUTTON_MOUSE_UP		1000

#define TRAYROW_LINE		(PICKER_NUM * 4) - 1	// 15
////// Tray Data 관리용
#define LOADTRAY			1				// 추가 및 변경 됨. 2K8/09/19/ViboX
#define UNLOADTRAY			2
#define REJECTTRAY			3
#define LBUFFER				4
#define UBUFFER				5
#define SAFETYPOSITION		6

#define T_BUFFER_MAIN			0
#define T_BUFFER_SIDE			1

#define PICKERNUM			2
#define PICKER_NUM			4
#define PICKING_GAP			1

#define SORTER_PICKER_NUM	10				// 2K10/02/09/ViboX		

#define PICKER_LOAD			0
#define PICKER_UNLOAD		1
#define FINGER_LOAD			2
#define FINGER_UNLOAD		3

//#define BUFFER_SIZE			4
#define BUFFER_LOAD			0
#define BUFFER_UNLOAD		1

//buffer 관리 
#define BUF_NOTUSED		0
#define BUF_LOADER		3
#define BUF_UNLOADER	4

// test 횟수설정
#define T_NOTTEST			0
#define T_PRIMETEST			1
#define T_RETEST			2

#define TRAYROW_ON			1
#define TRAYROW_OFF			4		// 트레이의 row을 사용하지 않는다는 의미로 사용함

#define TRAYCOL				0
#define TRAYROW				1
#define OFF_ALIVE			1		//살아있다...
#define OFF_CONTIN			100		//지정된 연속 fail로 죽었다
#define OFF_YIELD			200		//지정된 yield로 인해서 죽었다 (절대, 상대)
#define OFF_YIELD_A			210		//지정된 yield로 인해서 죽었다 (절대)
#define OFF_YIELD_R			220		//지정된 yield로 인해서 죽었다 (상대)
#define OFF_MANUAL			300		//사용자가 직접 off
#define OFF_PICKER			400		//Picker off로 인해서 죽었다...

#define BACK_CAL_POS		700		//각정정보를 통해 계산되어질 위치다.!
#define BACK_SET_POS		800		//해당 위치를 티칭한 데이터다..

///////////////////
#define RET_NOWHERE			-1000		// 해당 데이터가 존재하지 않을때 
#define RET_NOSPACE			-2000		// 공간이 없을때 
#define RET_EMPTYDEVICE		-3000		// 디바이스가 모두 없을때 
#define RET_FULLDEVICE		-4000		// 디바이스가 FULL
#define RET_LOADTRAYSKIP	-10000		// loader key skip

#define RET_PICKERALLYES 	-5000		// 피커 정보가 문제일때 
#define RET_PICKERALLNO 	-6000		// 피커 정보가 문제일때 
//#define RET_PICKERNOTFIND	-7000		// 빈에 해당하는 정보가 없다
#define RET_PICKERNOTMOVE	-5400		// 작업할 피커는 있지만, 간섭구간으로 인하여 이동이 불가한 위치다..

// Tray 및 작업위치를 세분화 하기 위함
#define LEFT_SITE			0
#define RIGHT_SITE			1
#define CENTER_SITE			2
#define UNKNOWN_SITE		3

#define MAX_IO_CYLINDER				40
// *************************************************************************
// handler 동작 관련 피킹 / 자재 정보 저장 플래그
#define D_INFOSIZE		5		//몇가지의 정보를 사용할건지..
#define D_EXIST			0		//자재 유무 관리
#define D_BIN			1		//빈 등급 관리
#define D_RE_CNT		2		//리데트스 횟수 관리
#define D_SCR_BIN		3		//화면에 표시할 빈 등급 관리
#define D_DC_DUT		4		//테스트거친 dc사이트의 dut정보

#define SKIPMODE				3

#define MAX_MOTOR				18


#define INIT_CLEAR					0
#define INIT_READY					1
#define INIT_COMPLETE				2

#define WORK_CLEAR					0

//kwlee 2017.0406
#define EPOXY_VEL                    0
#define EPOXY_ACC                    1
#define EPOXY_DEC                    2
/*
//0번 보드 
#define 	M_TRAY1_Z			0
#define 	M_TRAY2_Z			1
#define 	M_TRANSFER1_X		2
#define 	M_TRANSFER1_Y		3
#define 	M_TRANSFER1_Z		4
#define 	M_ALIGNER			5
#define 	M_TRANSFER2_X		6
//1번 보드 
#define 	M_TRANSFER2_Z		7
#define 	M_ARM_PITCH			8
#define 	M_ARM_X1			9
#define 	M_ARM_X2			10
#define 	M_ARM_Y				11
#define 	M_CASSETTE1_Z		12
#define 	M_CASSETTE2_Z		13
#define 	M_CASSETTE1_O		14
#define 	M_CASSETTE2_O		15

#define 	M_TRANSFER1_PITCH	16
#define 	M_TRANSFER2_PITCH	17
*/


// *****************************************************************************
// 화면 동작 관련 기본 데이터 저장 구조체 변수 선언                             
// *****************************************************************************

#define MAX_BUF_SIZE							20	
#define MAX_LD_BUFF_PARA					1

#define MAX_WAIT_TIME_NUM				40
#define PICKER_PARA							1		
#define MAX_PICKER							4

#define LD_MOVING_SAFETY				1
#define ULD_MOVING_SAFETY				2
#define MOVING_NOT_SAFETY				3

//kwlee 2017.0411
// 0 : 제어->검사 , 1 : 경화액 도포 불량 , 2: 점 불량 , 3: 선 두께 불량, 4: 중심점 편심 
// 5: 삐침 불량 , 6 : 모양 불량 7, 선 짧음 , 8:  heatSink MisAlign
#define  ERROR_7387_NOT_DISPENSING			1
#define  ERROR_EPOXY_DOT_DEFECT				2
#define  ERROR_EPOXY_LINE_THICK1_DEFECT		3
#define  ERROR_EPOXY_CENTER_DEFECT			4
#define  ERROR_EPOXY_LINE_THICK2_DEFECT		5
#define  ERROR_EPOXY_LINE_THICK3_DEFECT		6
#define  ERROR_EPOXY_LINE_THICK4_DEFECT		7
#define  ERROR_HEATSINK_MISALIGN_DEFECT		8
#define  ERROR_RESERVE_DEFECT				9
#define  ERROR_DEVICE_MARK_DEFECT			10

#define	 ERROR_PASS							1
#define  ERROR_DEFECT						2
enum PICKER_PICKPLACE_INFO
{
	PICKER_PICK_MODE	= 0,
	PICKER_PLACE_MODE	   ,
};
//0번 보드 


// *************************************************************************
// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언                            
// *************************************************************************
#define MAX_WAIT_TIME 40

enum WAIT_TIME
{
	WAIT_PICKER_CLAMP_ON_OFF = 0,//LOAD/UNLOAD
	WAIT_BUFFER_ALIGN_CLAMP,
	WAIT_TRAY_ALIGN_CLAMP,
	WAIT_CARRIER_CLAMP_FWDBWD,
	WAIT_CARRIER_PRESS_UPDN,
	WAIT_CARRIER_SLIDE_UPDN,
	WAIT_CARRIER_HOLDER_UPDN,
	WAIT_CAMERA_CLAMP_FWDBWD,
	WAIT_CAMERA_CLAMP_UPDN,
	WAIT_HEATSINK_PICKER_UPDN,
	WAIT_HEATSINK_PICKER_VACUUM,
	WAIT_DISPENSOR_REVERSE,
	WAIT_DISPENSOR_REVERSE_FWDBWD,
	WAIT_DISPENSOR_REVERSE_UPDN,
	WAIT_DISPENSOR_AIRBLOW,
};

struct st_io_wait_param
{
	int	nOnWaitTime[MAX_WAIT_TIME];
	int nOffWaitTime[MAX_WAIT_TIME];
	int nLimitWaitTime[MAX_WAIT_TIME];

};
extern  st_io_wait_param  st_wait;


enum THREAD_SYNC_VARIBLE_SITE_INFO  //위치별 트레이 존재 유무를 위치별로 정의해 놓음  
{
	THD_LD_STACKER			 = 0,
	THD_LD_TRAY_PLATE,
	THD_LD_ALIGN_BUFF,
	THD_ULD_ALIGN_BUFF,
	THD_UNLD_ALIGN_BUFF,
	THD_LDULD_CARRIER_BUFF,
	THD_UNPRESS_RBT,
	THD_EPOXY_RBT,//	THD_BILLIARD,RBT,
	THD_HEATSINK_RBT,
	THD_HEATSINK_PRBT,
	THD_VISION_RBT,
	THD_DISPENSOR_RBT,//경화제
	THD_DISPENSOR_PRBT,//경화제
	THD_LD_HEATSINK_BUFF,
	THD_PLACE_HEATSINK_DVC,
	THD_PICK_REVERSE_DVC,
	THD_PICK_HEATSINK_DVC,
	THD_PLACE_CARRIER_DVC,
	THD_LD_HSALIGN_BUFF,
	THD_LD_HSPICK_BUFF,
	THD_ULD_STACKER,
	THD_ULD_HSALIGN_BUFF,
	THD_EMPTY_STACKER,
	THD_WORK_TRANSFER,
	THD_LOAD_WORK_RBT,
	THD_UNLOAD_WORK_RBT,
	THD_CARRIER_RBT,


	THD_LOADHS_WORK_RBT,
	THD_HARDENER_RBT,
	THREAD_MAX_SITE
};

struct st_handler_param
{
	int mn_menu_num;				// 각 화면에 설정된 화면 번호 저장 변수 
	int mn_alarm_screen;			// 현재 알람 화면이 출력되어져 있는지에 대한 정보 저장 변수 

	BOOL b_program_exit;			// 프로그램을 빠져나가는건지 확인한다.
	
	// *************************************************************************
	// 화면에 대한 핸들 정보 저장 변수 선언                                     
	// *************************************************************************
	HWND hWnd;						// 메인 프레임 핸들 저장 변수
	CWnd *cwnd_title;				// 타이틀 출력 화면 핸들 저장 변수
	CWnd *cwnd_list;				// 리스트 출력 화면 핸들 저장 변수

	CWnd *cwnd_main;				// 메인 화면 핸들 저장 변수
	CWnd *cwnd_basic;				// BASIC 화면 핸들 저장 변수
	CWnd *cwnd_maintenance;			// Maintenance 화면 핸들 저장 변수
	CWnd *cwnd_testreference;		// Test Reference 화면 핸들 저장 변수
	CWnd *cwnd_interface;			// Interface 화면 핸들 저장 변수
	CWnd *cwnd_waittime;			// Wait Time 화면 핸들 저장 변수
	CWnd *cwnd_init;

	CWnd *cwnd_rack_info;

	CWnd *cwnd_motorteach;			// motor teaching 화면 핸들 저장 변수
	CWnd *cwnd_motorspeed;			// Motor Speed 화면 핸들 저장 변수
	CWnd *cwnd_diagnose;

	CWnd *cwnd_motor;

	CWnd *cwnd_motorAxis;
	CWnd *cwnd_motor_with_io;

	CWnd* cwnd_data_lot;
	CWnd* cwnd_data_testsite;
	CWnd* cwnd_data_xgem;// 2013,1028
	// *************************************************************************		

	// *************************************************************************
	// 장비 상태 정보 설정 변수 선언                                            
	// *************************************************************************
	int mn_initial;					// 초기화 작업 완료 플래그
	int mn_manual;
	
	int mn_menu_lock;				// 메뉴 사용 가능 상태 플래그 
	int mn_system_lock;				// SYSTEM LOCK 상태 플래그
	int mn_switch_lock;				// Dialog가 떠 있는 상태 플래그 (Start Switch Lock)

	CString mstr_pass_level;
	int mn_level_maint;				// 메인트 레벨(Level 1) 설정 플래그
	int mn_level_teach;				// 티칭 레벨(Level 2) 설정 플래그
	int mn_level_speed;				// 스피드 레벨(Level 3) 설정 플래그
	int mn_level_admin;				// Admin 레벨(Level 4) 설정 플래그

	int mn_initial_error;

	int mn_status;					// 장비가 가동중이냐
	int mn_reinstatement;

	int mn_lduld_safety;

	CTimeSpan m_tR;		//Run Time
	CTimeSpan m_tJ;		//Jam Time
	CTimeSpan m_tS;		//Stop Time
	CTimeSpan m_tM;		//Maint Time
	CTimeSpan m_tT;		//Test Time
	CTimeSpan m_tPT;	//Prev Test Time
	CTimeSpan m_tIT;	//Index Time
	CTimeSpan m_tPIT;	//Prev Index Time
	CTimeSpan m_tCT;	//Cycle Time
	CTimeSpan m_tPCT;	//Prev Cycle Time
	CTime m_tRef;
	
	CTimeSpan m_tDR;	//Run Time
	CTimeSpan m_tDRB;	//Run Time
	CTimeSpan m_tDRW;	//Run Time
	CTimeSpan m_tDJ;	//Jam Time
	CTimeSpan m_tDS;	//Stop Time
	CTimeSpan m_tDM;	//Maint Time
//	CTimeSpan m_tLotR[10];	//Lot Run Time //2013,1105


	
	//2015.0914
	CTimeSpan m_tDRef;
	//CTimeSpan m_tDRdown[2][10];	
	CTimeSpan m_tDRdown[20];
	//2015.0914
	CTimeSpan m_tDUserStop[20];	//Stop Time
	CTimeSpan m_tDStop[20];	//Stop Time
	CTimeSpan m_tDInStandby[20];
	///////
	CTimeSpan m_tDtest[2][10];
	CTimeSpan m_tDSokOff[2][10];
	//CTimeSpan m_tDInStandby[2][10];
	CTimeSpan m_tDOutStandby[2][10];
	
	int				mn_Pass[20];
	int				mn_Fail[20];
	int				mn_Input[20];
	int             mn_Output[20];
	int             mn_Yield[20];

	int mn_motor_init_check;
	int n_initiailze;
	
	int mn_interval; //kwlee 2015.1005
	CString mstr_DirPath;

	int mnSerial_Port_Creating[MAX_PORT];

	CString mstrSelectMessage;
	int mnSelectMessage;

	CString mstr_lot_name[3];		// 현재 장비 안에 있는 LOT NAME과 BUFFER_READY에서 날라오는 LOT NAME과 비교한다.
	int mn_comm_lot_name_chk;

	int mn_virtual_mode;
	int mn_virtual_supply;
	double md_picker_position[4];
	double md_tray_place_offset[2];

	//2016.0329
	double md_safty_rbty[2];//0:tray 1:reject

	int mn_retest_cnt;
	int mn_beat_cnt;
	int mn_AutoSocketOff_Cnt;

	CString str_main_lot;
	CString str_main_part;
	CString str_main_last;
	CString str_main_bypass;
	
	int mn_emptyout_cnt;	
	int mn_emptyin_cnt;
	int mn_emptynext_cnt;
	int mn_buffer_in;
	BOOL mb_rej_3stacker;
	int mn_cok_change;

	int mn_temp_pogo_type;
	int mn_temp_alignBuffer_type;
	int mn_pogo_type;

	//2013,0213
	int mn_cok_chgstate[3];
	int mn_mirror_type;

	int m_nfront_smema;
	int m_nfront_request;

	int mn_robot_motorX_safety;
	int mn_robot_motorRej_safety;

	//2013,0715
	int mn_fullinline_mode;
	CString m_strProcessID;
	CString m_strRPropEnd;
	int m_iPLotInfo;		// 2013,0910

	int m_bRestore;

	int m_nRemote;
	CString m_xgemstrErrMsg;
	CString m_strRecipe[100];
	int m_nTotal;
	CString m_strToolInform;

	////2015.0518
	int mn_multi_retest_cnt;
	int m_nhifix_mode_cnt;
	CString mstr_hifix_mode[100];
	////

	////2015.0604
	int m_nScrap_code_cnt;
	CString mstr_scrap_code[100];
	int mn_MultiLot_cnt[100];
	int mn_1Lot_cnt[100];
	int mn_version_mode;
	int mn_scrap_code_retry[100];

	int m_nRecipe_rule_cnt;

	//2015.0914
	int mn_allnewlot;
	int mn_InstandyBy[20];
	int mn_InstandyBy_1[20];
	int mn_OutstandyBy[20];

	bool isTestRun[20];

	bool mn_xgem_connect;
	CString mstr_device_name;

	bool mn_Abort;//
	//2016.0709


	int n_menu_num;					// 각 화면에 설정된 화면 번호 저장 변수 
	int n_alarm_screen;				// 현재 알람 화면이 출력되어져 있는지에 대한 정보 저장 변수 
	int n_load_state;				// 현재 로딩 버튼 관련 대화 상자 출력 여부 플래그 저장 변수 
	int n_retest_request;			// 리젝 자재로 리테스트 하는건지 물어본다.



	// *************************************************************************
	// 원래 위치 복귀 기능 사용 시 사용되는 변수 선언                           
	// *************************************************************************
	int n_homing_request[MAXMOTOR];	// 강제 Home Check 요청 플래그 설정 변수 

	double d_cur_cmd_pos[MAXMOTOR];	// 현재 모터 위치 정보 저장 변수 
	// *************************************************************************

	// **************************************************************************
	// LOT 처리 관련 플래그 설정 멤버 변수                                       
	// **************************************************************************
	bool LoaderOnFlag;		// 로더 On : True    로더 Off : False
	bool AllLotStartFlag;	// LOT 처음 시작 시 설정 됨
	// **************************************************************************

	// *************************************************************************
	// 화면에 대한 핸들 정보 저장 변수 선언                                     
	// *************************************************************************
	CWnd *cwnd_initial;
	CWnd *cwnd_io;					// IO 화면 핸들 저장 변수
	CWnd *cwnd_dataview;
	CWnd *cwnd_lot_info;
	CWnd *cwnd_recipe;
	CWnd *cwnd_lotstart;
	CWnd *cwnd_manualmove;
	// *************************************************************************

	// *************************************************************************
	// 알람 정보 저장 파일 생성 관련 플래그 선언                                
	// *************************************************************************
	BOOL b_chk_load;				// 응용 프로그램 로딩 시 파일 생성 플래그
	BOOL b_chk_run;					// 06시 기준 파일 생성 플래그

	int n_chk_load_conti;			// 응용 프로그램 로딩 시 파일 생성 플래그
	int n_chk_load_conti_dlg;

	int n_chk_unload_conti;			// 응용 프로그램 로딩 시 파일 생성 플래그
	int n_chk_unload_conti_dlg;

	int n_chk_hload_conti;			// 응용 프로그램 로딩 시 파일 생성 플래그
	int n_chk_hload_conti_dlg;

	int n_chk_dump_conti;			// dump 위험지역에서 체크 요청 
	int n_chk_dump_conti_dlg;		// 
	
	int n_chk_index_table_conti;			// dump 위험지역에서 체크 요청 
	int n_chk_index_table_conti_dlg;		// 

	int n_calculate;				// 파일 생성 완료 플래그
	// *************************************************************************

	// *************************************************************************
	// 장비 상태 정보 설정 변수 선언                                            
	// *************************************************************************
	int n_initial;					// 초기화 작업 완료 플래그
	
	int n_menu_lock;				// 메뉴 사용 가능 상태 플래그 
	int n_system_lock;				// SYSTEM LOCK 상태 플래그
	int n_switch_lock;				// Dialog가 떠 있는 상태 플래그 (Start Switch Lock)



	int n_maint_mode;				// 메인트 모드 설정 플래그
	
	int n_level_maint;				// 메인트 레벨(Level 1) 설정 플래그
	int n_level_teach;				// 티칭 레벨(Level 2) 설정 플래그
	int n_level_admin;				// 관리자 레벨(Level 3) 설정 플래그
	int n_level_master;	

	CString	str_user_id;


	CString str_programmer_level;	// 프로그래머 레벨(Level Administrator) 설정 플래그
	
	int n_io_board_initial;			// IO 보드 초기화 작업 완료 플래그
	int n_mot_board_initial;		// 모션 보드 초기화 작업 완료 플래그
	int n_matrox_board_initial;     // matrox 초기화
	
	int n_home_state[MAX_MOTOR];		// 모터 HOMING 완료 플래그
	int n_init_flag[MAX_MOTOR];
	int n_elevator_status[2][ELEVATOR_STRATUMS];		//Elevator Status [0]:Module, [1]:Heat Sink
	
	int n_lot_start;				// LOT 시작 플래그
	
	int n_lot_request;
	
	int n_lot_start_check;

	int reinstatement;
	
	CString mstr_handler_version;	// 핸들러 버젼
	CString mstr_handler_date;		// 핸들러 버젼 수정 날짜.

	double md_run_speed;				// 장비 동작시 속도비
	double md_manual_speed;			// 메뉴얼 동작시 속도
	// *************************************************************************



	CTimeSpan m_tDMTBI;	//Run Time


	CTime m_lot_start_time;		//Lot Start 시간
	CTime m_lot_end_time;		//Lot End 시간

	int n_lot_total;			//Lot 총 갯수.
	CTime m_surmise_time;		//끝날 예상 시간.
	int n_loading_time;

	int n_safety[8];

	int n_mode_manual;

	int threadcount;

	CString str_last_alarm;

	int n_m_loader_type;		// 현재 모듈 Loader가 어떤 방식으로 사용되어지냐..
	int n_m_elevator_type;		// 현재 모듈 Elevator가 어떤 방식으로 사용되어지냐..

	CString str_lot_name;

	int n_site_status[2];

	double md_screw_90;

	double md_dot_amount;
	double md_line_amount;
/////////////////////////////////////////////////////////////////////////////////////////////////////
	int n_lot_end_check[MAXMOTOR+4];	// LOT END CHECK FLAG

	int n_partial_error;		// Load Tray Partial Error.
	int n_partial_expect_left;	// Load Tray Partial Error.
	int n_partial_check;		// Load Tray가 partial 일까요?
	
	//로더트레이 자재없음 선택시 나머지층은 확인하지 않고 lotend 들어가기 위한 변수 선언
	int n_partial_lotend;	// Load Tray가 partial 일까요?
	
	int n_sync_reinstate;	// 복귀동작 진행상황 확인 변수
	
	int	i_checking_test_setting;	// lot 시작시 테스트 횟수 (리테스트) 설정을 확인하는 창이 popup상태인지..
	int n_checking_load_continue;	// loader부의 lot end진행 여부를 물어 보고 있음을 구분
	int n_flag_load_continue;		// lot 진행또는 LOT END 선택결과를 저장하는 변수
	
	int n_tray_set_request;  //elev 내의 트레이가 empty/full이다 램프를 동작 시키자.
	int Serial_Port_Creating[2];		// 0 : 바코드 리더기, 1 : 테스터

	//////////////////////////////////2K10/02/09/ViboX////////////////////////////////////
	int n_run_status;					// 장비 동작. 2K10/02/09/ViboX
	int n_run_EpoxyStatus;
	int n_lot_flag;
	int n_initial_flag;

	/////////////////////////////////// 20100329 /////////////////////////////////////////
	int n_lot_end;


	int n_allhomecheck;					// 전체 홈체크 플래그.
	int i_main_screen_start_req;

	int n_pick_error_conti;

	int n_ad_board_create;
	int n_motor_port_create;

	float f_ad_resol;					// ad board 해상도....
	float f_ad_device;

	bool bLoaderOnFlag;				// 로더 On : True    로더 Off : False
	int mn_removetray;
 	int n_without_idbuffer;

	int mn_init_state[MAXSITE];		// 초기화 완료 플래그
	int n_MutingOn;
	int n_MutingOff;
	bool m_bAlarmMcPowerOff;

	int n_InterfaceConnectOK;
	int n_CaseAssemblyUnloadingAbleOn;
	DWORD nInterLockTimeCheck;
	int n_HSAbleOn;
	int n_HSLowerPlaceAbleCheck;

	////Light Curtain Check	
	int	nStackerLightCurtainFlag;
	int	nHeatSinkLightCurtainFlag;
	int n_HeatSinkMutingOn;

};
extern  st_handler_param  st_handler;
// *****************************************************************************

struct st_initial_param
{
	int n_manual;			//프로그램 시작시 초기화냐? 아니면 메뉴얼 초기화냐.

	int n_loader_elevator_init;
	int n_loader_transfer_init;

	int n_unloader_elevator_init;
	int n_unloader_transfer_init;

	int n_xyz_robot_init;
	int n_z_axis_safety;

	int n_test_site_init;

	int n_initial_error;
	int n_initial_count;
};
extern st_initial_param st_init;

// *************************************************************************
// RS-232C 시리얼 통신 관련 저장 구조체 선언                                     
// *************************************************************************
struct st_serial_param 
{
	int	n_connect[MAX_PORT];
	int n_snd_chk[MAX_PORT];         // 메시지 송신 완료 플래그
	int n_rec_chk[MAX_PORT];         // 메시지 수신 완료 플래그
	
	CString str_port_chk[MAX_PORT];  // 시리얼 포트 생성 정보 저장 변수

	CString str_snd[MAX_PORT];       // 송신 메시지 저장 변수
	CString str_rec[MAX_PORT];       // 수신 메시지 저장 변수

	CString comm_snd[MAX_PORT];      // 리스트 화면 출력 송신 메시지 저장 변수
	CString comm_rec[MAX_PORT];      // 리스트 화면 출력 수신 메시지 저장 변수
	CString comm_err[MAX_PORT];      // 리스트 화면 출력 에러 메시지 저장 변수

	int n_serial_port[10];
	int n_serial_baudrate[10];
	int n_serial_data[10];
	int n_serial_parity[10];
	int n_serial_stop[10];
};
extern  st_serial_param  st_serial;
// *************************************************************************

// *************************************************************************
// 발생한 알람 관련 데이터 저장 구조체 선언                                      
// *************************************************************************
struct st_alarm_param 
{
	CStringList str_list_jam;	// 발생한 Jam Code의 리스트
	CString mstr_code;			// 발생한 알람 코드 저장 변수
	CString mstr_pcode;			// 발생한 알람 코드 저장 변수
	char mc_code[10];
	int mn_count_mode;			// 알람 카운트 플래그 (0:알람 카운트 미진행    1:알람 카운트 진행)
	int mn_type_mode;			// 알람 종류 저장 변수 (0:경고성    1:진행성    2:장비 정지)

	long stl_cur_alarm_time;	// 알람 발생 시간 저장 변수
	int stl_prev_code;			// 바로 전 발생한 알람 코드 저장 변수
	long stl_prev_alarm_time;	// 바로 전 발생한 알람 발생 시간 저장 변수

	int mn_alarm_occured;		// 알람 발생 횟수 저장 변수
	int mn_alarm_occured_day;	// 알람 발생 횟수 저장 변수
	int mn_arm_lot_continue;		// LOT 계속 진행 플래그
	int mn_reset_status;			// Alarm Reset Status
	int mn_emo_alarm;            // EMO관련 알람은 한번만 발생하도록 하기 위함!!
	
	int mn_emo_set;
	int n_alarm_assign_section;		//알람이 발생한 곳을 조금더 쉽게 찾기 위함

	int n_pickerstatus[SORTER_PICKER_NUM + 2];
};	
extern  st_alarm_param  alarm;
// *************************************************************************

// *************************************************************************
// 알람 데이터 저장 구조체 선언                                                  
// *************************************************************************
struct st_dsp_alarm_param 
{
	// **************************************************************************
	// 알람 부위 표시 마크 [출력 위치], [크기], [칼라] 저장 변수                 
	// **************************************************************************
	int n_Offset;
	int n_Cy;
	int n_Cx;
	int n_LineWidth;
	int n_R;
	int n_G;
	int n_B;
	// **************************************************************************

	// **************************************************************************
	// 화면에 출력할 알람 정보 저장 변수                                         
	// **************************************************************************
	int n_action_cnt;            // 출력할 알람 조치 사항 갯수 저장 변수
	CString str_style;           // 알람 출력 형태 저장 변수
	CString str_bmp_file;        // 알람 표시 그림 파일 저장 변수
	CString str_repair_msg;      // 알람 조치 사항 저장 변수
	// **************************************************************************

	// **************************************************************************
	// 파일에 저장된 알람 정보 저장 변수                                         
	// **************************************************************************
	CString mstr_e_content[2000];  // 파일에서 로딩한 한 라인 정보 저장 변수
	CString mstr_e_code[2000];     // 알람 코드 저장 변수
	CString mstr_e_state[2000];    // 알람 상태 저장 변수
	CString mstr_e_part[2000];     // 알람 발생 부위 저장 변수
	CString mstr_e_msg[2000];      // 공백 제거 알람 메시지 저장 변수
	// **************************************************************************

	// **************************************************************************
	// 현재 발생한 알람 저장 변수                                                
	// **************************************************************************
	int mn_cur_state;             // 알람 저장 여부 플래그
	CString mstr_cur_msg;         // 발생한 알람 메시지 저장 변수
	// **************************************************************************

	int n_picking_error;
};
extern  st_dsp_alarm_param  st_alarm;

#define D_EXIST_1			0		//자재 유무 관리
#define D_EXIST_2			1		//자재 유무 관리
#define D_BIN_1				2
#define D_RE_CNT_1			3
#define D_BIN_2				4
#define D_RE_CNT_2			5
// ==============================================================

#define LOADTRAY_X			25				// 추가 및 변경 됨. 2K8/09/19/ViboX
#define LOADTRAY_Y			3
#define UNLOADTRAY_X		25
#define UNLOADTRAY_Y		3
#define REJECTTRAY_X		25
#define REJECTTRAY_Y		3
#define MAXOPERATORCNT      50 //kwlee 2017.0404

struct st_modulemap_size
{
	
};
extern  st_modulemap_size  st_modulemap;

// ******************************************************************************
// 베이직 화면 환경 설정 정보 저장 구조체 선언                                   
// ******************************************************************************
struct st_basic_param 
{
	int nManualSpeed;
	int nRunSpeed;
	
	int n_tray_x;
	int n_tray_y;

//	int n_mode_device;
	int n_mode_pogo;//2012,1225

	int mn_alignBuffer_type;//2013,0215
//	int n_mode_run;
	int n_retry_cnt;

	int n_mode_jig;
	int n_mode_recovery;

	int n_mode_product;
	int n_mode_product_site1;
	int n_mode_product_site2;
	int n_mode_product_site1_mode;
	int n_mode_product_site2_mode;

	int n_cok_type;
	int n_degree;
	int n_pick_type;

	int mn_mode_run;			// 장비 동작 모드 저장 변수		(1:ON LINE	0:OFF LINE)
	int mn_mode_xgem;
	int mn_mode_rcmd;
	
	int mn_bypass_mode;

	int mn_device_type;
	
	int n_day_start_time;		// Data 기록용.. 하루의 시작 시간...
	int n_day_shift_time;		// Data 기록용.. Shift 시간...
			
	CString mstr_device_name;	// 선택된 디바이스 종류 저장 변수
	int mn_alarm_delay_time;
	CString mstr_revipe_name;	// 선택된 디바이스 종류 저장 변수
	
	CString mstr_Interval_Cnt; //kwlee 2015.1005;
	int mn_electrostatic_check_gap;
	double md_electrostatic_value;

	int mn_network_wait_time;		// Network 대기 시간
	int mn_network_retry_cnt;		// Network Retry 횟수

	int mn_time_out;
	int mn_time_out_per;
	int mn_empty_tray_out;
	int mn_robot_init_time;
		
	long l_handler_recovery_flag[4];
	//[0]=>복구모드 저장 모드(0:저장되지 않은 상태, 1:복구모드가 저장 상태(이때만 초기화시 복구여부를 물어볼 수  있다) 
	//[1]=>초기화시 복구모드 선택 유/무 확인 (0:복구모드 선택안함, 1:복구모드 선택)
	//[2]=>복구 모드 저장 시간 저장 
	//[3]=>초기화시 복구모드 선택시간  


	int mn_hotfix_pos;//2013,0504
	int mn_socket_max_count;

	int m_bFDC_Data;//2013,1105 FDC Data

	
	CString n_str_hifix_mode;//2015.0518
	
	////2015.0604
	CString mstr_FtpIP;
	CString mstr_FtpName;
	CString mstr_FtpPort;
	CString mstr_mode_ftp;
	int mn_mode_ftp;
	////

	int n_retest_jig;//2015.0914
	int n_recipe_mode;

	CString m_strScrapHead;

	int n_cserial2_mode;

	int n_auto_timeout_mode;//2016.0112

	int n_file_save;

	//kwlee 2017.0405
	//int n_mode_dc_test;			//
	//int n_mode_work;			// 
	//int n_mode_run;
	//int n_mode_turn;
	//int n_conveyor_move_mode;
	//	int n_mode_off_line;		// off-line 결과 설정 (0 : random 1 : All Good)	
	//kwlee 2017.0405 추가 예정.
	// 20100517 tae 수정
	int n_mode_interface;		// 장비 동작 모드 저장 변수		(1:ON LINE	0:OFF LINE)
	int n_mode_device;			// 자재 사용 모드 저장 변수		(0:With		1:with out device 2:With out c-tray)
	int n_mode_retest;			// RETEST 모드 저장 변수		(1:사용		0:미사용)
	int n_mode_tester;			// 테스터 종류 저장 변수
	int n_mode_bcr;
	int n_mode_module;

	int n_count_retry;
	int n_count_pick_retry;		// PICK 시도 횟수 저장 변수
	int n_count_partial;		// PARTIAL 횟수 저장 변수
	int n_count_elevator_partial;	// 엘레베이터 PARTIAL 횟수 저장 변수

	int n_mode_tray_type;//TYPE1,TYPE2

//	CString mstr_device_name;	// 선택된 디바이스 종류 저장 변수

	double dEpoxyXLineOffSet;
	double dEpoxyYLineOffSet;
	
	double dHSCarrierSpreadMoveOffset;
	double dHSCarrierSpreadMoveDistance;
	double dHeatSinkCarrierSpreadMove1Offset;
	double dHeatSinkCarrierSpreadMove2Offset;
	double dHeatSinkCarrierSpreadMove3Offset;
	
	int n_rubb_count;
	int n_Light_Curtain_Mode;
	int n_mode_case_assembly;

	int n_mode_7387;
	int n_7387InspectAlarmSkipMode;
	int n_3874InspectAlarmSkipMode; //kwlee 2017.0411
	int n7387AlarmCount;
	int n_3874InspectMode;
};
extern  st_basic_param  st_basic;
// ******************************************************************************

// *************************************************************************
//  TRAY 설정 정보 저장 구조체 선언                                     
// *************************************************************************
struct st_traybuffer_param 
{
	int n_loader_x;
	int n_loader_y;
	
	int n_unloader_x;
	int n_unloader_y;

	int n_reject_x;
	int n_reject_y;

	int n_tray_x;
	int n_tray_y;
};
extern  st_traybuffer_param  st_traybuffer;
// *************************************************************************

// ******************************************************************************
// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언                            
// ******************************************************************************
struct st_time_param 
{
	int m_iWaitTime[MAX_WAIT_TIME_NUM][2];
	int m_iLimitTime[MAX_WAIT_TIME_NUM];
} ;
extern  st_time_param  st_time;
// ******************************************************************************

// *****************************************************************************
//  Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언                           
// *****************************************************************************
struct st_lamp_param
{
	int mn_lamp_r[8];			// RED 램프 상태 저장 변수
	int mn_lamp_y[8];			// YELLOW 램프 상태 저장 변수
	int mn_lamp_g[8];			// GREEN 램프 상태 저장 변수
	
	int mn_interest;			// BLUE 램프 상태 저장 변수
	
	CString mstr_equip_no;		// 장비 호기 저장 변수
	CString mstr_equip_id;		// 장비 코드 저장 변수
	CString mstr_ec_equip_id;	// 장비 코드 저장 변수	// 20130822 kjh

	CString mstr_equip_area;	// 장비 공정 저장 변수	// 2014.12.24 - Bredmin.
	
	int mn_buzzer_mode;			// 부저 사용 모드 저장 변수
	
	int mn_lamp_wait_time;		// 램프 FLACKER 시간 저장 변수
	int mn_lamp_change_time;	// 램프 변
};
extern  st_lamp_param  st_lamp;
// *****************************************************************************


// *************************************************************************
// Home Check 시 사용될 구조체 선언                                              
// *************************************************************************
struct st_home_param
{
	int n_flag_home;		// Home Check 사용될 리턴 플래그 설정 변수 
	long n_step_home;		// Home Check 시 사용될 스텝 정보 저장 변수 

	int n_para_stop;		// Home Chek 동작 완료 시 정지 방법 정보 설정 변수 

	__int16 n_home_axis;	// Home Check 작업 진행 축 정보 저장 변수 
	double n_home_vel;		// Home Check 시 사용될 정속 정보 저장 변수 
	double n_home_acc;		// Home Check 시 사용될 가속 정보 저장 변수 
	long n_home_limit_time; // Home Check 시 사용될 Time Out 시간 정보 저장 변수 
	long n_home_start_time; // Home Check 시 사용될 시작 시간 정보 저장 변수
	long n_home_end_time;	// Home Check 시 사용될 종료 시간 정보 저장 변수
	long n_home_chk_time;	// Home Check 시 사용될 시간 간격 정보 저장 변수 

	unsigned short n_chk_io;  // ATX 모터 보드 센서 상태 정보(Bit) 저장 변수 
};
extern  st_home_param  st_home[MAX_MOTOR];
// *************************************************************************

// *****************************************************************************
// I/O 상태 데이터 저장 구조체 변수 선언                                        
// *****************************************************************************
#define BIT_NO				0
#define BIT_INPUT			1
#define BIT_OUTPUT			2


// *****************************************************************************
//  메세지 관련
// *****************************************************************************
struct st_message_param
{
	CString mstr_infor_msg;
	
	int mn_pad_dialog;
	int mn_dot_use;
	CString mstr_pad_high_limit;
	CString mstr_pad_low_limit;
	CString mstr_keypad_val;

	CString mstr_keypad_msg;
	CString mstr_keyboard_msg;

	CString str_fallacy_msg;		// 오류 메세지.
	CString mstr_confirm_msg;		// 확인 메세지
	CString mstr_typing_msg;		// 키보드 메세지

	CString mstr_parent_menu;

	CString mstr_password;

	CString mstr_barcode;

	// List에 출력 하다가 죽는 현상을 막기 위해서... 2K11/09/01/ViboX
	char c_msg[5000];
	char c_abmsg[5000];
	char c_normal_msg[5000];
	char c_abnormal_msg[5000];

	char c_NGBuffer_Msg[5000];		// 2014.12.04 - Bredmin.

	CString mstr_normal_prev_msg;
	CString mstr_abnormal_prev_msg;

	CString mstr_send;
	CString mstr_recive;

	int mn_event_msg_list;
	CString mstr_event_msg[3];

	CString str_step_savefile_name;
	CString str_step_savefile_path;

	char n_select_buffer;

	std::vector< CString > m_vecMsg;
} ;
extern  st_message_param  st_msg;
// *****************************************************************************

// *************************************************************************
// 파일 경로명 저장 구조체 변수 선언                                             
// *************************************************************************
struct st_path_param 
{
	/*CString str_basic;       // 기본 셋팅 정보 저장 폴더+파일명 설정 변수

	CString str_path_dvc;    // 디바이스별 티칭 정보 저장 파일 생성 폴더 설정 변수
	CString str_socket;      // 소켓별...
	CString str_path_alarm;  // 출력 알람 정보 존재 폴더 설정 변수
	CString str_path_op;     // 오퍼레이터 정보 저장 폴더 설정 변수
	CString str_path_tray;	 // Tray별 정보 저장 파일 생성 폴더 설정 변수

	CString str_path_file;   // 파일 생성 폴더 설정 변수
	CString str_path_back;   // BACKUP 파일 생성 폴더 설정 변수

	CString str_file_basic;  // 기본 셋팅 정보 저장 파일명 설정 변수
	CString str_file_wait;   // 대기 시간 정보 저장 파일명 설정 변수

	CString str_operation;	//Operation Log 파일 폴더 설정.
	CString str_interface;	//Interface Log 파일 폴더 설정.
	CString str_machine;	//Machine Log 파일 폴더 설정.
	CString str_total;		//Total Log 파일 폴더 설정.

	CString str_path_dctest;	//DC Test 파일 폴더 설정.

	CString	str_tempLotInfor;

	CString str_path_result;
	CString str_test_result;
	CString str_motor_pos;
	CString str_error_log;

	CString str_io_map;
	CString str_io_part_map;
	CString str_motor_axis_map;
	CString str_motor_part_map;
	CString str_wait_time_map;*/

	CString mstr_run_state;		// 20130830 kjh;
	CString mstr_basic;			// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
	CString mstr_basic_folder;	// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
	CString mstr_datalogger;	// Data Logger의 폴더+파일명 설정 변수
	
	CString mstr_path_Model;	//kwlee 2017.0403
	CString mstr_path_dvc;		// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정 변수
	CString mstr_socket_lot;	// 소켓별...
	CString mstr_socket_day;	// 소켓별...
	CString mstr_socket_month;	// 소켓별...
	CString mstr_socket_shift;	// 소켓별...
	CString mstr_path_alarm;	// 출력 알람 정보 존재 폴더 설정 변수
	CString mstr_path_op;		// 오퍼레이터 정보 저장 폴더 설정 변수
	CString mstr_path_tray;		// Tray별 정보 저장 파일 생성 폴더 설정 변수
	CString mstr_path_serial;	// Socket 정보 저장 파일 생성 폴더 설정 변수
	
	CString mstr_path_file;		// 파일 생성 폴더 설정 변수
	CString mstr_path_back;		// BACKUP 파일 생성 폴더 설정 변수
	
	CString mstr_file_basic;	// 기본 셋팅 정보 저장 파일명 설정 변수
	CString mstr_file_wait;		// 대기 시간 정보 저장 파일명 설정 변수
	
	CString mstr_heater;		// Operation Log 파일 폴더 설정.
	CString mstr_operation;		// Operation Log 파일 폴더 설정.
	CString mstr_interface;		// Interface Log 파일 폴더 설정.
	CString mstr_machine;		// Machine Log 파일 폴더 설정.
	CString mstr_barcode;		// Barcode Log 파일 폴더 설정.
	CString mstr_barcode_fail;	// Barcode Log 파일 폴더 설정.
	CString mstr_gpib;			// Total Log 파일 폴더 설정.
	CString mstr_total;			// Total Log 파일 폴더 설정.
	CString mstr_ngbuffer;		// NG Buffer Log 파일 폴더 설정.	2014.12.19 - Bredmin.
	CString mstr_galmg;
	CString str_log_path;
	CString str_lot_data_path;
	CString str_lot_pass_data_path;
	CString str_daily_data_path;
	CString str_daily_pass_count;	//20130531 kjh

	CString mstr_tcpip;
	CString mstr_tcpip_tc_server;
	CString mstr_tcpip_his;
	CString mstr_tcpip_vision;
	CString mstr_tcpip_bpc;
	CString mstr_tcpip_cim;
	CString mstr_tcpip_rfid;
	CString mstr_tcpip_bp;
	
	CString mstr_io_map;		// IO MAP 파일 위치 폴더.
	CString mstr_motor;
	
	CString mstr_handler;

	CString mstr_io_part_map;
	CString mstr_motor_part_map;
	CString mstr_motor_axis_map;
	CString mstr_wait_time_map;

	CString mstr_trackout_lot;
	CString mstr_auto_line;// auto line 저장 폴더 생성 20130913 kjh
	CString mstr_recipe_path;
	CString mstr_ppid;
	CString mstr_ppid_del;

	CString mstr_partno_match;	// Part No Match용. 2K14/05/21/ViboX

	CString mstr_user_control_map;

	CString m_strStationLog;
	CString m_strPcBoxIDSave;
	CString m_strMars;			// 삼성 로그 표준 폴더.

	CString m_strBoardLog;
	CString m_strIndexLog;
	CString m_strEventLog;
	CString m_strViboXLog;
	CString m_strPickFailLog;
	CString m_strLastModuleLog;

	CString mstr_xgem_cfg_path;
	CString m_strFtpPath;

	CString strPathCycle;
};
extern  st_path_param  st_path;
// *************************************************************************

// *************************************************************************
// 파일 정보 저장 구조체 변수 선언                                               
// *************************************************************************
typedef struct tagITEMINFO 
{
    CString     strFileName; 
    DWORD       nFileSizeLow; 
    FILETIME    ftLastWriteTime; 
} ITEMINFO;

const MAX_BUF_LEN = 25600;
// *************************************************************************

// *************************************************************************
// 장비 동작 제어 플래그 정보 저장 구조체 선언                                   
// *************************************************************************
#define TESTSIZE		4
#define MAXMAINBOARD	100
#define SITEMAINBOARD	25
#define LEFTSITE		0
#define RIGHTSITE		1

#define ELEVATOR_ROW	2
#define ELEVATOR_COL	8

#define DEV_BOOT		1
#define DEV_TEST		2
#define LOADER_CNT		20

#define CASSETTE_NO				0
#define CASSETTE_IN				1
#define CASSETTE_OUT			2

#define TEST_DUMP_NO			0
#define TEST_DUMP_PICK			1
#define TEST_DUMP_PICK_MOVE		2
#define TEST_DUMP_PLACE			3
#define TEST_DUMP_FREE			4
#define TEST_DUMP_HOME			5

#define TEST_SITE_NO			0
#define TEST_SITE_PICK			1
#define TEST_SITE_PLACE			2

#define INDEX_DUMP_NO			0
#define INDEX_DUMP_PICK			1
#define INDEX_DUMP_PLACE		2
#define INDEX_DUMP_PLACE_MOVE	3
#define INDEX_DUMP_ROTATOR		4
#define INDEX_DUMP_HOME			5

#define INDEX_TABLE_NO			0
#define INDEX_TABLE_SAFETY		1

#define SORTER_NO				0
#define SORTER_PICK				1

struct st_work_param 
{
	// **************************************************************************
	int mn_run_status;  // 장비 동작 상태 정보 저장 변수 (O:STOP   1:RUN    2:ALARM    3:MAINT    4:INIT)
	int mn_run_EpoxyStatus;
	// **************************************************************************
	int mn_tl_status;					// 타워램프 상태 정보 저장 변수 (O:STOP 1:RUN(IN PCB) 2:ALARM 3:MAINT 4:INIT 5:RUN(NO PCB))

	int mn_Box1[MAX_TRAY_ROW][MAX_TRAY_COL];
	int mn_Box2[MAX_TRAY_ROW][MAX_TRAY_COL];

	double d_Heatsink1Xpos[100];
	double d_Heatsink1Ypos[100];
	double d_Heatsink2Xpos[100];
	double d_Heatsink2Ypos[100];

	double d_X_Box1Pos;
	double d_Y_Box1Pos;
	double d_X_Box1Deg;
	double d_Y_Box1Deg;

	double d_X_Box2Pos;
	double d_Y_Box2Pos;
	double d_X_Box2Deg;
	double d_Y_Box2Deg;

	bool b_load_key_flag;		// 로더 키 ON/OFF 플래그 (TRUE:ON    FALSE:OFF)
	bool b_trayload_flag;		// 트레이에서 로딩할 자재가 있느냐.
	bool b_lot_start_flag;		// LOT 시작 플래그
	bool b_load_off_flag;		// 강제 Lot End

	int n7387BlowCheck;

	// **************************************************************************
	// 장비 동작 중 발생하는 시간 정보 저장 관련 멤버 변수                       
	// **************************************************************************
	long MachineTime[4];  // 장비 동작 및 정지 시간 정보 저장 변수 (0:STOP TIME    1:RUN TIME    2:ALARM TIME    3:MAINT TIME)
	// **************************************************************************
	
	int		mn_prev_step[200];			// 이전 step (변경 되었는지 보기 위함)
	CString	mstr_history_step[200];		// step_history 약 50개에 해당하는 step을 저장
	CString	mstr_history_step_back_up[200];		// step_history 약 50개에 해당하는 step을 저장
	CString mstr_last_change_date[200];	// 최종 step으로 변경되는 시점의 시간
	
	// **************************************************************************
	//  동작 관련 멤버 변수                                                      
	// **************************************************************************	// 추가 및 변경 됨. 2K8/09/19/ViboX	
	//  Unloader Elevator_Left부 ===============================================================================================
	// Main Robot부 ===============================================================================================
	int n_init_sorter_robot;			//XYZ Robot 초기화 변수
	int n_end_main_robot;			//XYZ Robot 동작 완료 변수
	int n_end_xyz_LoadTray;			//XYZ Robot이 loader부에 더이상에 자재가 없음을 확인하기 위함
	int n_end_xyz_LoadDevice;		//XYZ Robot이 장비내부에 더이상에 자재가 없음을 확인하기 위함
	int n_sync_xyz_to_ld_tr;		//XYZ Robot이 transfer에게 카세트를 이동해 달라고 요청
	int n_sync_xyz_to_ld_el;		//XYZ Robot이 elevator에게 카세트를 가져 가라고 요청
	int n_sync_xyz_to_retest_ld;	//XYZ Robot이 retest buffer에서 작업을 마무리 했다 언로더 위치로 이동해라...
	int n_robot_final_work_buffer;	//XYZ Robot이 buffer에 마지막으로 공급한 site를 구분.. (LEFT_SITE, RIGHT_SITE)
	int n_robot_device_loading;		//XYZ robot이 자재를 1회 이상 로딩했다.
	int n_loading_after_first_run;  //Loader on이후 RUN되기 이전을 확인하기 위함..
	int n_end_uld_robot;			//XYZ Robot 동작 완료 변수
	int n_sync_xyz_to_uld_tr;		//XYZ Robot이 transfer에게 카세트를 이동해 달라고 요청
	int n_sync_xyz_to_uld_el;		//XYZ Robot이 elevator에게 카세트를 가져 가라고 요청
	int n_sync_xyz_to_retest_uld;	//XYZ Robot이 retest buffer에서 작업을 마무리 했다 로더 위치로 이동해라...
	// WORK_READY -> WORK_START -> WORK_END
	
	int n_reinstate_main_robot;		//XYZ Robot 복귀동작 완료변수	

	//  안전상태관련  ===============================================================================================
	int n_safety_main_xyz;
	int n_safety_conveyor_z;
	int n_safety_test_site_z;
	int n_safety_test_dump_z;
	
	// WORK_READY -> WORK_START -> WORK_END
	int n_reinstate_buffer[2];		// Buffer 복귀동작 완료변수
	//  Buffer 부 ===============================================================================================
	bool b_door_open;
	bool b_door_close;
	// **************************************************************************

	int i_FirstPickerNum[10];
	int i_FirstX_Num[10];		//이동할 위치의(tray 또는 auto buff) X pos
	int i_FirstY_Num[10];		//이동할 위치의(tray 또는 auto buff) Y pos

	int n_FirstPickerNum[10];
	int n_FirstX_Num[10];		//이동할 위치의(tray 또는 auto buff) X pos
	int n_FirstY_Num[10];		//이동할 위치의(tray 또는 auto buff) Y pos

	long ThreadCount;			//thread 동작 상황을 체크하자 

	int n_area_sen_chk_flag;	//Door와 Area Sensor 상황을 체크하자.

	int DeviceInitializeEnd;	//0:초기화 중, 1:초기화 완료

	int n_dvc_bin_retestcnt;
	int n_dvc_retestbin_retestcnt;
	// **************************************************************************

	//EPOXY
	int nEpoxyBiliardThreadRunMode;
	int nJigEpoxyWorkCount;

	CTimeSpan m_boardtime[TESTSIZE][SITEMAINBOARD];

	int    n_text_r[2][2];
	int    n_text_g[2][2];
	int    n_text_b[2][2];

	int    n_grid_r[2][2];
	int    n_grid_g[2][2];
	int    n_grid_b[2][2];

	// daily cycle time
	double	dDailyCycle;
	int mn_machine_mode;
	CString strLotInputID;

	//kwlee 2017.0406
	int n_barcode_recive;
	CString strBarcodeRecive;
};
extern  st_work_param  st_work;

// *************************************************************************
// 기타 정보 저장 구조체 선언                                                    
// *************************************************************************
struct st_other_param 
{
	CString str_op_msg;			// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_normal_msg;		// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_abnormal_msg;	// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_snd_msg;		// 리스트 화면 테스터 SEND 메시지 저장 변수
	CString str_rcv_msg;		// 리스트 화면 테스터 RECIVE 메시지 저장 변수
	CString str_bcr_rcv_msg;	// 리스트 화면 바코드 RECIVE 메시지 저장 변수
	CString str_n_snd_msg;		// 리스트 화면 NETWORK SEND 메시지 저장 변수
	CString str_n_rcv_msg;		// 리스트 화면 NETWORK RECIVE 메시지 저장 변수

	CString str_fallacy_msg;	// 오류 메시지 대화 상자 타이틀 저장 변수
	CString str_confirm_msg;	// 확인 메시지 대화 상자 타이틀 저장 변수
	CString str_typing_msg;		// 키보드 대화 상자 타이틀 저장 변수

	CString str_keypad_msg;		// 키패드 대화 상자 타이틀 저장 변수
	CString str_pad_val;		// 키패드 대화 상자에 출력할 데이터 저장 변수

	CString str_pad_high_limit;
	CString str_pad_low_limit;

	CString str_pass_level;		// 암호 레벨 저장 변수
	CString str_password;		// 암호 저장 변수
	
	CString str_parent_menu;	// 주메뉴 이름 저장 변수
	
	CString m_TCP_S_Msg;		// 보낼 메세지
	CString m_TCP_R_Msg;		// 받은 메세지
	
	int n_teaching_pendant_axis;
	
	int n_Firstpicker_num[10];
	int n_dot_use;				// 소수점을 사용하느냐?
	
	int n_picker_site;
	
	int n_pad_dialog;

	int nBuzYesNo;
	char c_normal_msg[200];
	char c_abnormal_msg[200];

	////////////////////////////////2K10/02/09/ViboX////////////////////////////////////////
	int		n_prev_step[100];			// 이전 step (변경 되었는지 보기 위함)
	CString	str_history_step[100];		// step_history 약 50개에 해당하는 step을 저장
	CString str_last_change_date[100];	// 최종 step으로 변경되는 시점의 시간
};
extern  st_other_param  st_other;
// *************************************************************************

// *************************************************************************

#define	CVY_MODULE_PICK_NO			0
#define CVY_MODULE_PICK_1			1
#define CVY_MODULE_PICK_2			2


// *************************************************************************
//  Thread간 Sync.
// *************************************************************************
struct st_sync_param
{
	int nLotEndFlag[30];			// lot end 여부를 판가름 한다 YES이면 해당 쓰레드 LOT END
	int n_init_flag[20];

	int nLdUldSateyflag;
	int nLdUld_LightChk;

	int nCarrierSateyflag[9];//[0]LDPICKER [1]ULDPICKER[2]UNPRESS_Y[3]EPOXY[4]HEATSINK[5]VISION[6]UNLOADCARRIER[7]LOADCARRIER[8]CARRIERMOVE

	//work transfer에서 요청 
	int nWorkTransfer_Req[THREAD_MAX_SITE][2]; //work 트랜스퍼에 동작을 요청한다 , [0]:요청정보, [1]:1:트레이를 집기, 2:트레이를 놓기

	//load plate에서 요청 
	int nLdPlate_Tray_Supply_Req[THREAD_MAX_SITE]; //load plate에서 load elevator에 트레이 공급을 요청한다 

	///////////////////////////////////////////////////////////////////////////////////////////
	// Work Robot 사용 사이트 
	///////////////////////////////////////////////////////////////////////////////////////////
	//load plate, load buff, carrier place	
	int nLdWorkRbt_Dvc_Req[THREAD_MAX_SITE][5]; //[*][1]: WORK_PICK(디바이스를 집기 가능한 영역 관리), [*][1]:WORK_PLACE(/디바이스를 놓기 가능한 영역 관리)
											  //[*][2]: 로딩/언로딩 버퍼의 방향(left turn((right test site 작업가능), right turn(left test 작업가능) 명령),  -> 중요한 정보로 임의로 다른곳에서 바꿔서는 안된다 
											  //[*][3]: 작업할 테스트 사이트 번호 위치 정보 -> 중요한 정보로 임의로 다른곳에서 바꿔서는 안된다 
	int nUldWorkRbt_Dvc_Req[THREAD_MAX_SITE][5];

	//CARRIER ROBOT 동작
	int nCarrierRbt_Dvc_Req[THREAD_MAX_SITE][5]; 
	int nCarrierRbt_UpDnPress_Req[3];//[0]UP:LOAD Carrier [1]DOWN:UNLOAD Carrier [2]M_PRESS_Y
	int nCarrierBcr_Req;

	//HEATSINK ROBOT 동작
	int nHeatSinkRbt_Dvc_Req[THREAD_MAX_SITE][5]; //vision, Dispensor에 동작 전달

	//VISION ROBOT 동작

	//if Epoxy is going to safety, it may be conflict with Hearsink.
	//so Heatsink transfer robot have to check where is it(heatsinkrobot may be able to work in turn position )
	int nHeatsinkEpoxySateyflag;

	int nDisPensorFlag;
	int n_barcode_read_serial_num[2][2]; //kwlee 2017.0406

};
extern st_sync_param	st_sync;

// *************************************************************************
/////////////////////////////////////////////////////////////////////////////
#define	WM_DRAWRECT				1
#define IMG_WIDTH				640
#define IMG_HEIGHT				480

#define BAD_WIDTH				200
#define BAD_HEIGHT				200

// 칼라 정의
#define SKY_C					RGB(187, 250, 255)
#define NOR_C					RGB(164, 164, 164)
#define NOR_L					RGB(190, 190, 190)
#define RED_C					RGB(255, 0, 0)
#define RED_L					RGB(255, 150, 150)
#define RED_D					RGB(80, 20, 0)
#define BLUE_C					RGB(0, 0, 255)
#define WHITE_C					RGB(255, 255, 255)
#define BLACK_C					RGB(25, 1, 1)
#define BLACK_L					RGB(62, 55, 55)
#define YELLOW_C				RGB(255,255,0)
#define YELLOW_D				RGB(128, 128, 0)
#define YELLOW_L				RGB(255, 255, 193)
#define GREEN_C					RGB(0,255,0)
#define GREEN_L					RGB(150,255,150)
#define GREEN_D					RGB(0,100,20)
#define DSERBLE_BC				RGB(210,210,230)
#define TEXT_BC					RGB(220,230,210)
#define OK_BC					RGB(0, 0, 255)
#define NG_BC					RGB(255, 0, 0)
#define RESULT_BC				RGB(40, 1, 1)
#define ORANGE_C				RGB(0xFF, 0x68, 0x20)
#define BLACK					RGB(0, 0, 0)
#define BLUE_D					RGB(0, 0, 128)
#define BLUE				    RGB(0, 0, 255)
#define PINK					RGB(255, 125, 225)
#define TEXT_BC_1				RGB(240, 240, 240210)
//#define BLUE_L					RGB(150, 150, 255)
#define BLUE_L					RGB(124, 139, 252)
#define ON_C					RGB(0, 255, 0)
#define OFF_C					RGB(0, 150, 0)
#define NG_C					0x0000FF
#define OK_C					0xFF00
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define TEST_BC					RGB(194, 231, 150)
#define IN_BC					RGB(204, 153, 255)
#define GRAY					RGB(90, 80, 80)
#define DVC_READY_C				RGB(153, 153, 255)
#define DVC_LOAD_C				RGB(239, 214, 198)
#define DVC_NOVIDEO_C			RGB(0,100,20)
#define DVC_PGM_C				RGB(255, 255, 0)
#define DVC_UNLOAD_C			RGB(255, 255, 204)
#define DVC_NONE_C				RGB(150, 150, 150)
#define DVC_START_C				RGB(150, 255, 150)
#define DVC_NO_C				RGB(128, 128, 0)

#define THICKNESS_FAIL			RGB(255,255,0)
#define LENGTH_FAIL				RGB(0xFF, 0x68, 0x20)
#define BARCODE_FAIL			RGB(255, 0, 0)

#define NORMAL_MODE				0
#define THRESHOLD_MODE			1

#define HOMING				1
#define MOVING				2
#define MULTI_MOVING		3
#define POS_MOVING			4


//Grid 정의 
#define DATE					0
#define EDIT					1
#define COMBO					2
#define BUTTON					3
#define STATIC					4
#define TIME					5
#define PERCENT					6
#define CHECK1					7
#define NUMBER					8
#define NUMBER1					9

#define IO_IN					1
#define IO_OUT					0
#define IO_NONE					2


struct st_admin_param
{
	int i_analyze;			//프로그램 시작시 초기화냐? 아니면 메뉴얼 초기화냐.
	int i_dry_run;			//장비를 테스터 및 안전센서등 센싱무시 모드 (0: 사용안함, 1: 사용함)
	int i_dry_run_area_off; //dry모드에서 elevator area sensor 사용여부 (0: 센싱, 1: 무조건 무시)
	int i_dry_run_door_off; //dry모드에서 doors(5개) 사용여부 (0: 센싱, 1: 무조건 무시)
	int i_dry_run_air_off;  //dry모드에서 air sensor 사용여부 (0: 센싱, 1: 무조건 무시)
	
	int i_skip_tray_direction;
};
extern st_admin_param st_admin;
	

#define DOWN_FAIL			1
#define DOWN_SUCCESS		2

#define NETWORK_SND_MSG		17
#define NETWORK_RCV_MSG		18

#define NO				0
#define YES				1
#define READY			2
#define SUPPLY			3
#define REQUEST			4
#define COMPLETE		5
#define WORKEND			6
#define EMPTY			7

#define N_ESTABLISHED		1
#define N_FAILED			2
#define N_ABANDONNED		3

#define PGET				0
#define TMVIN				1
#define TMVOU				2
#define IMVIN				3
#define IMVOU				4
#define LMVIN				5
#define LMVOU				6
#define TAMS_ERR			7
#define TAMS_ERR_REPLY		8

#define RECEIVE_MSG			1
#define SEND_MSG			2
#define NORMAL_MSG			3
#define ABNORMAL_MSG		4

#define CLIENT_CONNECT		0
#define CLIENT_CLOSE		1
#define CLIENT_SEND			2
#define CLIENT_REV			3

#define SERVER_CONNECT		0
#define SERVER_CLOSE		1
#define SERVER_SEND			2
#define SERVER_REV			3

struct st_client_param
{
	CString		str_ip;			// ip address......
	
	int			n_port;			// port.....
	int			n_connect;		// client 연결상태......
	int			n_send_length;  // 송신 문자 길이......
	int			n_rev_length;	// 수신 문자 길이......

	int			n_rev_info;		// client 종료 방법..... (0:바로 1: 수신후)

	char		ch_rev[1024];	// 수신내용....
	char		ch_send[1024];  // 송신내용....
};
extern st_client_param st_client[10];

struct st_server_param
{
	CString		str_ip;			// ip address......
	
	int			n_port;			// port.....
	int			n_connect;		// client 연결상태......
	int			n_send_length;  // 송신 문자 길이......
	int			n_rev_length;	// 수신 문자 길이......
	
	char		ch_rev[1024];	// 수신내용....
	char		ch_send[1024];  // 송신내용....
};
extern st_server_param st_server[10];

struct st_part_motor_info
{
	int				n_part_cnt;
	int				n_part_axis_cnt[20];
	int				n_part_axis[20][10];

	int				n_axis_pos_num[20][10];
	int				n_axis_minus_el[20][10];
	int				n_axis_plus_el[20][10];
	int				n_axis_sd[20][10];
	int				n_axis_home[20][10];
	int				n_axis_direction[20][10];
	int				n_axis_account[20][10];
	
	CString			str_part_info[20];
	CString			str_part_name[20];

	CString			str_part_axis_name[20][10];
	CString			str_axis_pos_name[20][10][M_MAX_POS];
	CString			str_axis_pos_info[20][10][M_MAX_POS];
};
extern struct st_part_motor_info st_motor_info;

struct st_serial_info
{
	int			n_serial_port[10];		// serial port....
	int			n_serial_baudrate[10];	// serial baudrate....
	int			n_serial_data[10];		// serial data....
	int			n_serial_parity[10];	// serial parity.....
	int			n_serial_stop[10];		// serial stop....
};
extern struct st_serial_info rs_232;


struct st_time_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrlot_no;
	CString			mstrpart_no;
	
	int				mnTime_Section;
	int				mnTime_Time;
	int				mnMode;
	
	int				mnSectionBuffer;
};
extern struct st_time_database st_timedb;

struct st_alarm_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrlot_no;
	CString			mstrpart_no;
	
	CString			mstrAlarmCode;
	CString			mstrAlarmMsg;
};
extern struct st_alarm_database st_alarmdb;

struct st_ground_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrChannel[5];
};
extern struct st_ground_database st_grounddb;

struct st_buffer_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	CString			mstrlot_no;
	CString			mstrpart_no;
	int				mnbuffer_pos;
	int				mnlast_module;
	int				mnbypass;
	int				mnmodule_cnt;
	int				mnretest;
};
extern struct st_buffer_database		st_bufferdb;

struct st_boat_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	CString			mstrlot_no;
	CString			mstrpart_no;

	int				mn_boat_id;
	int				mn_product_id;
	int				mn_pcb_cnt;
	int				mn_bin[8];		// 4 * 2 또는 2 * 4가 수량이 제일 많은 Boat임. Bin은 1(0), 2(1), 4(2), 8(3), 조립불량(4), 치수불량(5), UnKnown(6).
};
extern struct st_boat_database		st_boatdb;

struct st_scrap_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	char			mstrlot_no[50];
	char			mstrpart_no[50];
// 	CString			mstrlot_no;
// 	CString			mstrpart_no;
	char			mstrSerialNo[50];
	char			mstrScarpCode[50];
// 	CString			mstrSerialNo;
// 	CString			mstrScarpCode;
};
extern struct st_scrap_database st_scrapdb;

struct st_buffer_tray_param
{
	int nDeviceExistence[4][3][100];
	char cModuleData[3][90][50];
	int nArrayInfo[3][90];
	char cSerialNumber[3][90][20];
	char cLotNumber[3][90][20];
	char cTestResult[3][90][20];
	char cArrayInfo[3][90][20];

	char cTotalLotNumber[40];
	char cTotalPartNumber[40];
	char cModuleCnt[5];
	int nRetestMdlCnt[2];			// 0 : HTO, BTO, 1: NTO
	int nLastModule;
	int nByPass;

	int nTrayExistence;
};
extern st_buffer_tray_param	st_buffer_tray[MAX_SITE_INFO];

struct st_set_data
{
	CString m_strSetID;
	CString m_strMLotID;
	int m_iChkMerge;
	int m_nTrayCnt;
	int m_nDvcCnt;
	int m_nDvcTotalCnt;
	int m_nFailDvcCnt;
	int m_iFailDvcOver;			// 기준수량보다 Fail 자재가 많으면, 버리지 말고 Loading Fail Stacker로 보내버린다. 2014.12.12 - Bredmin.
	int m_nFailScrapCnt;		// Fail Device의 기준수량보다 많아지기전 디버터에 털어버리는 수량..2014.12.12 - Bredmin.
	int m_nSetCnt;
	int m_nSetCntCheck;
	int m_nSetEnd;				// 세트중간에 다른세트 들어올 경우, 처리를 위한 변수. 2014.11.06 - Bredmin.
	int m_nTCTrayOut;			// 서버랑 통신 중, 화면에 체크버튼 비활성화용 변수. 2014.11.06 - Bredmin.
	int m_iGotoNGBuffer;		// NG Buffer로 제거할 물량이냐?  2K14/11/02/ViboX
	int m_iRFIDReadWriteFail;	// RFID 쓰기를 실패했냐? 2014.12.02 - Bredmin.
	int m_iLotWaitOver;			// 세트의 Lot이 10분동안 안들어왔을때 사용할 변수.. 2014.12.18 - Bredmin.
	int m_iVisionSend;
	int m_iSetColor;			// Set의 color 표시용 변수..2014.12.24 - Bredmin.
	int m_nFirstLotTrayCnt;		// 세트의 첫번째 들어온 Lot의 Tray 수량..2014.12.29 - Bredmin.
	int m_nFirstLotDvcCnt;		// 세트의 첫번째 들어온 Lot의 Device 수량..2015.04.30 - Bredmin.

	int m_iVisionOCRResult;		// 비전 OCR 검사 결과 변수..2015.01.19 - Bredmin.
	int m_iVisionPass;			// 비전검사를 pass 여부 변수..2015.01.19 - Bredmin.
	int m_nMultiLotOut;			// Multi Lot 배출 횟수 변수..2015.01.27 - Bredmin.

	int m_iInDiffSet;			// 세트 중간에 다른 세트가 들어왔을때, 사용할 변수..2015.03.30 - Bredmin.

	CString m_strLotID;
	CString m_strEqpID;
	CString m_strRunID;
	CString m_strPartID;
	CString m_strQty;
	CString m_strStepSeq;
	CString m_strWeekCode;
	CString m_strAsySite;
	CString m_strMarking;
	CString m_strTraySepc;
	
	CString m_strArrayX;
	CString m_strArrayY;
	CString m_strPocketPitchX;
	CString m_strPocketPitchY;
	CString m_strEndPitchX;
	CString m_strEndPitchY;
	
	CString m_strStackingThick;
	CString m_strSeatingHeigth;
	CString m_strPkgHeight;
	CString m_strPkgName;
	CString m_strBodySize;
	CString m_strAllScrap;
	CString m_strScrapCode;			// 2014.10.16 - Bredmin.
	CString m_strDumpComplt;		// 덤핑완료 구분자..2014.12.24 - Bredmin.

	std::vector< CString > m_vecLotID;
	std::vector< CString > m_vecMergeNoLotID;

	//std::vector< int > m_vecLotCnt;	// lot단위 디바이스 수량 화면 표시용..2014.11.12 - Bredmin.

	CString m_strMergeFailLotID;		// 2015.06.23 - Bredmin.		
};
extern st_set_data st_setdata;

struct st_part_info
{
	int			n_part_cnt;
	int			n_part_info;
	int			n_motor_axis_cnt[20];
	int			n_motor_axis[20][10];
	int			n_io_input_cnt;
	int			n_io_output_cnt;
	int			n_io_input[32];
	int			n_io_output[32];
	
	CString		str_part_name[20];
	CString		str_part_info[20];
	CString		str_motor_axis[20][10];
	CString		str_io_input[32];
	CString		str_io_output[32];
};
extern struct st_part_info st_part;

struct tagPCB_INFO
{
	DWORD   dw_TestWaitTime[3][3];
	int		nQty;//PCB order

	DWORD	dwBdTime;	

	int nTestBdStart;						//테스트 시작과 끝을 알수 있다 
	int nEnable;							//해당 위치 사용유/무 
	int nTempEnable;                      //james 2016.0816 //테스트사이트의 소켓오프등의 해당 위치는 작업을 할 수 없는 상태, 피커를 잠시 사용하지 못하게 한다, 해당 테스트 위치 작업 완료후, 해당 정보 클리어. 

	int nYesNo;								// ssd 유무.....
	int nBin;								// ssd 테스트 결과.....
	int nRetestCnt;							// retest 테스트 횟수......
	int nRetest;
	int	nBinNum;							// ssd 테스트 결과.....
	int	nRetestSite[MAX_RETEST_COUNT];		// retest 실행한 site 번호....
	int nBdNum;								// 마지막 테스트 테스트 Board 번호.......
	int nScrCode;							// scrap code.....
	int nCOK_IDNum;							//cok 정보 1~7까지 
	int nCOK_YesNo;							//cok 존재 유/무 셋팅 
	int nStatusChang;						// 상태정보 변경.........
	int nOldBin;							// barcode....

	int nSkipMode;                        //kwlee 2016.0912 TestSite SkipMode
///////////////////////////////////////////////////////////////
	CString strSerialNo;					// Lot에서 pcb의 순서번호......
	CString strBarcode;					// pcb 2D barcode......

	CString strArrNo;						// array serial....
	CString strPPID;						// ppid.....
	CString strPSID;						// psid....
	CString strWWN;						// wwn.....
	CString strCSerialNo;				// c_serial......


	CString strLotNo;						// pcb Lot number.....
	CString strBarcode1D[2];			// pcb 1D barcode......
	// jtkim 20160929
	CString strBarcode2D[2];			// pcb 2D barcode......
	CString strPartNo;					// part no.....
	CString strBin;							// strBin......
	// jtkim 20150707
	CString strOldSerial;
	// jtkim 20160804
	CString strMsg;							// result message (h60 h55 c30 c40)....
	// jtkim 20150805
	CString strComment;

	COleDateTime tStart;					// 테스트 시작시간...
	// jtkim 20151027
	CTime tStandBy;
	// jtkim 20151028
	int nReworkNg;							// rework mode check......

	// jtkim 20160106
	int nCntInfo;							// count info......

	// jtkim 20160108
	DWORD dwCycle[10];						// log add...

	// jtkim 20160929
	int nTemp[10];
	int nFailCount;                       //kwlee 2016.1124

	//kwlee 2016.1228
	int nBinFailCnt[150];
	int nTotalFail;
	int nBinGoodCnt;

	CString strBdTime;


	//2017.0109
	CString			strMaterial;			// matrial id.....
	CString			strLogKey[10];
	CString			strLogData[10];
	CString			strMovePos[2];	
};

struct tag_PICKER_INFO
{
	tagPCB_INFO st_pcb_info[MAX_PICKER]; // pcb 구조체 변수.....

	int nPicker_Workinfo[MAX_PICKER]; //테스트 로봇에서 PICK&PLACE 작업할 시 작업하 피커 정보를 셋팅한다 

	//2017.0109
	int				nLogCount;
	CString			strMaterial;			// matrial id.....
	CString			strLogKey[50];
	CString			strLogData[50];
};
extern tag_PICKER_INFO st_picker[THREAD_MAX_SITE];

// **********************************************************************************
// 레시피 화면 환경 설정 정보 저장 구조체 선언                                   
// **********************************************************************************
struct tagRECIPE_INFO
{
	////////////////////////////////////////
	// 사용하는 변수 
	///////////////////////////////////////
	int nTrayNum;
	int	 nHsTrayY;						// HeatSink tray Y 정보.......
	int nHsTrayX;						// HeatSink tray X 정보.......
	int	 nTrayY;						// tray Y 정보.......
	int nTrayX;						// tray X 정보.......
	int nRetestBuffer_Num; //2016.0810
	int nLdBuffer_Num; //1
	int nCarrierBuffer_Num;				//Carrier buffer
	int nLdHsAlignBuffer_Num;
	double dLdUldPickerPitch;		//picker pitch 
	double dTestPickerPitch;		//picker pitch 
	double dTrayYOffset;			// tray Y pitch.......
	double dTrayXOffset;			// tray X pitch.......

	double dTrayPitch_Y;			//트레이 피치간격 
	double dTrayPitch_X;			//트레이 피치간격 

	double dLdUldBufferPitch;			//버퍼 피치간격 
	double dRetestBufferPitch;	//retest buffer pitch 값

	double dVisionXOffset; // Vision X Offset
	double dVisionYOffset; // Vision Y Offset

	int	nTestRetest_Count;
	int nBarcodeCnt;

	////////////////////////////////////////
	// 이하 사용하지 않는 변수 
	///////////////////////////////////////

	int		nBufferRotator_TestSite_Degrees; // ld/unld 같이 사용 . 0:0도, 1:90도, 2:180도, 3:270도 까지 셋팅 가능 
	int		nBufferRotator_PlateSite_Degrees; // ld/uld 같이 사용 . 무조건 0도로 셋팅이다 
	int		nFailCnt;							//연속 Fail일때 Socket Off 되는 카운트.
	//kwlee 2016.0809
	int		nTestSite_Top_Degrees; // CW/CCW
	int		nTestSite_Btm_Degrees; // CW/CCW

	// jtkim 20151017
	int		nBufferRotator_Barcode_Degrees; 
	int Buffer_DvcCheckSensor_Mode;    //버퍼 디바이스체크 센서 사용 유/무 
	int nCOKBuff_IDNum_Type;			////2015.0305 위부터 1로 변경 4.:2.5" reverse, 3:2.5" Front, 2:Msata, 1:pogo type //현재 작업할 COK ID 정보, buffer id와 공유한다, (COK Buff, Rotator Buff, Test site COK 정보가 이 번호와 같이 동일해야 한다 	

	int nTsiteUse[2][2][12];		// test site socket on/off 정보....[0]좌측(0)/우측[1] [0]하단(0)상단(1) [0]SLOT
	

	int nRetestTray_X; //2015.0222 retest tray를 사용시 X 방향으로 사용할 트레이의 정보
	int nRejectTray_X; //2015.0222 reject tray를 사용시 X 방향으로 사용할 트레이의 정보

	int nBufferY;					//james 2016.0808 사용하지 않음 
	int nBufferX;					//james 2016.0808 사용하지 않음 

	int nRetestBufferY;					//buffer y 정보 //james 2016.0808 사용하지 않음 
	int nRetestBufferX;					//buffer x 정보

	int nHifix;						// hifix 번호.....
	int	nAbortTime;					// test timeout......
	int nCokType[4];

	
	double dBufferPitch_Y;			//버퍼 피치간격 
	double dBufferPitch_X;			//버퍼 피치간격 
	double dTestPitch_X;			//버퍼 피치간격 

	//james 2016.0822
	

	double dCOKBuffPitch_X;			//COK 버퍼 피치간격 

	//2015.0206 모터를 X 방향으로 변경 double dLdUldPickerPitch_Y;		//picker pitch 
	

	
	double dTSiteOffsetX;			// test site x offset.....

	double dRejTrayPick;			// Reject Tray Pick Offset...
	double dRejTrayPlace;			// Reject Tray Place Offset...

	CString strHifix;				// hifix name.....
	CString strCokTypeName[8];

	bool	bCokChk[8][3];

	BOOL	bUnldBcrFlag[2];		// Unload 단 Barcode A, B 사용유무 플래그

	int		nBcrBank;				// barcode bank......

	double	dTsiteOffsetY[12];	// test site offset y.......

	CString strPartNo;

	// jtkim
	int		nAbortPer;
	int		nIndividualPick;

	double	d15TOffset;
 
	// jtkim 20160901
	int		nBarcodeCount;

	//UnPress 일때의 디바이스 간격
	double	dLoaderTransferTrayDeviceGap;
	DWORD dSatbleTime;

	double nEpoxyRunSpeed;
	double nEpoxyXYRunSpeed[3];//[0]//vel [1]:acc [2] dec
	int nEpoxyDotScrewCount;
	double dEpoxyXOffSet;
	double dEpoxyYOffSet;

	double nRubHSRunSpeed;

	int fDispenserVppmA;
};
extern tagRECIPE_INFO	st_recipe;



struct tagALL_TRAY_INFO
{
	int nNewLotIn;            
	int nLastModule;				// 마지막 자재인지 체크
	int nModuleCnt;					// tray 안에 있는 모듈수량.
	int nTrayExist;					//tray 존재 유/무 추가 2015.0226 james
	// jtkim 20150811
	int nReworkMode;				// rework mode......
	
	//2015.0226 추가 
	CString strLotNo;				// lot no......
	CString strPartNo;				// part no......
	CString strProcess;				// process id.....
	CString strOptCode;				// opt code (lot display에서 정보 가져오기)
	CString strFabSite;	          //2017.0105
	// jtkim 2015127
	CTime tStandBy;
	
	// jtkim 20160812
	int		nTestMode;				// ONESHOT, HOT, COLD, GATE......
	int		nInterface;				// ftp / local......
	int		nRetry;					// retry count.....
	int		nBarcodeMode;			// barcode 전송선택 (1개/2개)
	int		nQty;					// lot 수량.....
	int		nModuleCount;			//
	int		nPgmDown;				// pgm download 완료......
	
	//2017.0109
	int				nLogCount;
	int				nStartStatus;
	CString			strMaterial;			// matrial id.....
	CString			strLogKey[50];
	CString			strLogData[50];
	CString			strMovePos[2];
	
	tagPCB_INFO st_pcb_info[MAX_TRAY_ROW][MAX_TRAY_COL]; // pcb 구조체 변수..... 
	tagPCB_INFO st_dvc_info[2][MAX_TRAY_ROW][MAX_TRAY_COL]; //HEATSINK
};
extern tagALL_TRAY_INFO st_tray_info[THREAD_MAX_SITE];


struct tagTEST_SITE_INFO
{
	CString strBuffNo;
	CString strLotNo;				// test site lot no....
	CString strPartNo;				// test site part no.....
	
	int		nModuleCount;			//
	int		nPgmDown;				// pgm download 완료......
	int		nChamberDoor;			// chanber door open......
	// jtkim 20160806
	int		nTestBdStart;			// test site 통신체크 현재 진행상태........
	
	tagPCB_INFO st_pcb_info[3]; // pcb 구조체 변수.....
	
	int nSite_Pos; //1:TESTSITE_LEFT, 2:TESTSITE_RIGHT   //james 2016.0809 
	int nStart_FixPos; //작업 가능한 사이트 0~7 까지 의 소켓 정보(0번, 4번 소켓이 시작 위치이며, 체크 및 동작 위치로 FIX이다) //james 2016.0813 
	
	int nTester_Req[3];
	
};
extern tagTEST_SITE_INFO st_test_site_info[THREAD_MAX_SITE]; //2015.0216 [2][2];

struct tag_BUFFER_INFO
{
	int nBuffer_Req[3];
	
	CString strLotNo;				// test site lot no....
	CString strPartNo;				// test site part no.....
	CString strEqpID;				// test site epq id.....
	CString strOptCode;				// opt code (lot display에서 정보 가져오기)
	
	//james 2016.0813 
	int nTestSIte_No;	  //작업이 지정된 정보, Test Site #1,#2,#3,#4, #5,#6,#7,#8
	int nTestSite_LR_Pos; //작업이 지정된 정보, TESTSITE_LEFT(test #1,#2,#3,#4), TESTSITE_RIGHT(Test #5,#6,#7,#8)
	int nTestSite_Start_FixPos; //물리적으로 테스트사이트 작업시작위치 정보 (0 또는 4 이다.)
	///////
	
	//2017.0109
	int				nLogCount;
	CString			strMaterial;			// matrial id.....
	CString			strLogKey[50];
	CString			strLogData[50];
	
	tagPCB_INFO st_pcb_info[MAX_BUF_SIZE]; // pcb 구조체 변수.....
};
extern tag_BUFFER_INFO st_buffer_info[THREAD_MAX_SITE];
extern tag_BUFFER_INFO st_manaualbuffer_info[THREAD_MAX_SITE];
//////  2016.0806 


//TEACHING
enum TRAY_Z
{
	P_ELV_TRAY_Z_INITPOS	= 0, //안전 위치 (down limit 위치일것임)
	P_ELV_TRAY_Z_UPPOS,        //high  까지 빈 트레이 공급 또는 트레이를 받을떄  제일 윗쪽까지 올리면 체크
	P_ELV_TRAY_Z_READYPOS,   //sd sensor 감지 위치(트레이가 한장 이상 있을때 감지됨. 하나도 없으면 감지되지 않음 	

	P_ELV_SUPPLY_OFFSET, // SD pos + 트레이를 작업 가능하게 하는 위치 
	P_ELV_RECEIVE_OFFSET,//EMPTY에서 빈 트레이를 하나 받을 때의 높이
};

enum PRESS_Y
{
	P_PRESS_Y_INIT_POS	 = 0,
	P_PRESS_Y_PRESS1_POS, //PICK 1위치  각 위치 + st_recipe.dLoaderTransferTrayDeviceGap
	P_PRESS_Y_PRESS2_POS, //PLACE 1위치  각 위치 + st_recipe.dLoaderTransferTrayDeviceGap
};

enum WORK_TRANSFER
{
	P_TRAY_REMOVE_X_INIT_POS = 0,
	P_TRAY_REMOVE_X_PICK_POS,
	P_TRAY_REMOVE_X_PLACE_POS,
};

enum LOADER_TRANSFER_Y
{
	P_LOADER_TRANSFER_Y_INIT_POS = 0,
	P_LOADER_TRANSFER_Y_READY_POS,
	P_LOADER_TRANSFER_Y_PICK_POS,
	p_LOADER_TRANSFER_Y_ALIGN_PLACE_POS,
	P_LOADER_TRANSFER_Y_ALIGN_PICK_POS,
	P_LOADER_TRANSFER_Y_PLACE_TOP_POS,
	P_LOADER_TRANSFER_Y_PLACE_MID_POS,
	P_LOADER_TRANSFER_Y_PLACE_BOT_POS,
};

enum LOADER_TRANSFER_Z
{
	P_LOADER_TRANSFER_Z_INIT_POS = 0,
	P_LOADER_TRANSFER_Z_UP_POS,
	P_LOADER_TRANSFER_Z_ALIGN_PLACE_POS,//P_LOADER_TRANSFER_Z_ALIGN_POS,
	P_LOADER_TRANSFER_Z_ALIGN_PICKPOS,//P_LOADER_TRANSFER_Z_BRUSH_POS,
	P_LOADER_TRANSFER_Z_TRAY_POS,
	P_LOADER_TRANSFER_Z_CARRIER_PLACE_POS,
	P_LOADER_TRANSFER_Z_LASER_CHKPOS,
};

enum UNLOADER_TRANSFER_X
{
	P_UNLOADER_TRANSFER_X_INIT_POS	= 0,
	P_UNLOADER_TRANSFER_X_INREADY_POS,
	P_UNLOADER_TRANSFER_X_READY_POS,
	P_UNLOADER_TRANSFER_X_OUTREADY_POS,
	P_UNLOADER_TRANSFER_X_PICK_TOP_POS,
	P_UNLOADER_TRANSFER_X_PICK_MID_POS,
	P_UNLOADER_TRANSFER_X_PICK_BOT_POS,
	P_UNLOADER_TRANSFER_X_ZIGPLACE_POS,
};

enum UNLOADER_TRANSFER_Y
{
	P_UNLOADER_TRANSFER_Y_INIT_POS	= 0,
	P_UNLOADER_TRANSFER_Y_INREADY_POS, //아래위치
	P_UNLOADER_TRANSFER_Y_READY_POS,//중간위치
	P_UNLOADER_TRANSFER_Y_OUTREADY_POS, //위에 위치	//Y만 움직이면 기구물 충돌 위치이므로 일단 X축만 이동하여 안전 위치로 움직인다.
	P_UNLOADER_TRANSFER_Y_PICK_TOP_POS,	
	P_UNLOADER_TRANSFER_Y_PICK_MID_POS,	
	P_UNLOADER_TRANSFER_Y_PICK_BOT_POS,
	P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS,		
};

enum UNLOADER_TRANSFER_Z
{
	P_UNLOADER_TRANSFER_Z_INIT_POS	= 0,
	P_UNLOADER_TRANSFER_Z_PICKTOP_POS,
	P_UNLOADER_TRANSFER_Z_PICKMID_POS,
	P_UNLOADER_TRANSFER_Z_PICKBOT_POS,
	P_UNLOADER_TRANSFER_Z_ZIGPLACE_POS,	
};

enum CARRIER_X
{
	P_CARRIER_X_INIT_POS = 0,
	P_CARRIER_X_PUSHER_DOWN,
	P_CARRIER_X_PUSH_POS,//한번 민 거리
	P_CARRIER_X_BACK_PUSH_POS,
	P_CARRIER_X_PRESS_POS,//잠깐 뒤로 뺸 거리
	P_CARRIER_X_UNPRESS_POS,//밀거나, UNPRESS 하는 위치가 같다.밀기위해 Loaer위치로 이동한 거리
};


enum EPOXY_TRANSFER_X
{
	P_EPOXY_TRANSFER_X_INIT_POS = 0,	///// Epoxy Transfer Init Pos 
	P_EPOXY_TRANSFER_X_SAFETY,
	P_EPOXY_TRANSFER_X_FIRST_START_POS,
	P_EPOXY_TRANSFER_X_FIRST_END_POS,
	P_EPOXY_TRANSFER_X_SECOND_START_POS,
	P_EPOXY_TRANSFER_X_SECOND_END_POS,
	P_EPOXY_TRANSFER_X_DOT_POS1,
	P_EPOXY_TRANSFER_X_DOT_POS2,
	P_EPOXY_TRANSFER_X_DOT_POS3,
	P_EPOXY_TRANSFER_X_DOT_POS4,
	P_EPOXY_TRANSFER_X_DISCHARGE_POS,
	//P_EPOXY_TRANSFER_X_SUCKTION_POS,
};
//	P_EPOXY_TRANSFER_X_MEASURE_POS,
//	,

enum EPOXY_TRANSFER_Y
{
	P_EPOXY_TRANSFER_Y_INIT_POS	 = 0,	///// Epoxy Transfer Init Pos 
	P_EPOXY_TRANSFER_Y_SAFETY,
	P_EPOXY_TRANSFER_Y_FIRST_START_POS	,
	P_EPOXY_TRANSFER_Y_FIRST_END_POS,
	P_EPOXY_TRANSFER_Y_SECOND_START_POS,
	P_EPOXY_TRANSFER_Y_SECOND_END_POS,
	P_EPOXY_TRANSFER_Y_DOT_POS1,
	P_EPOXY_TRANSFER_Y_DOT_POS2,
	P_EPOXY_TRANSFER_Y_DOT_POS3,
	P_EPOXY_TRANSFER_Y_DOT_POS4,
	P_EPOXY_TRANSFER_Y_DISCHARGE_POS,
	//P_EPOXY_TRANSFER_Y_SUCKTION_POS,
};
//	P_EPOXY_TRANSFER_Y_MEASSURE_POS,
//	,


enum EPOXY_TRANSFER_Z
{
	P_EPOXY_TRANSFER_Z_INIT_POS = 0,
	P_EPOXY_TRANSFER_Z_TOP_DOWN_POS,
	P_EPOXY_TRANSFER_Z_MID_DOWN_POS,
	P_EPOXY_TRANSFER_Z_BOT_DOWN_POS,
	P_EPOXY_TRANSFER_Z_CLEANNING_DOWN_POS,	
};
//	P_EPOXY_TRANSFER_Z_MEASURE_DOWN_POS,

enum EPOXY_SCREW
{
	P_EPOXY_SCREW_INIT_POS = 0,
};

enum HEATSINK_TRANSFER_X
{
	P_HEATSINK_TRANSFER_X_INIT_POS = 0,
/*	P_HEATSINK_TRANSFER_X_PICK_POS,*/
	P_HEATSINK_TRANSFER_X_CARRIER_SAFETY_POS,
	P_HEATSINK_TRANSFER_X_DISPENSOR_SAFETY_POS,
	P_HEATSINK_TRASNFER_X_TURN_PLACE_POS,
	P_HEATSINK_TRASNFER_X_TURN_PICK_POS,
	P_HEATSINK_TRASNFER_X_TURN_READY_POS,
	P_HEATSINK_TRASNFER_X_DISPENSER_POS,
	P_HEATSINK_TRANSFER_X_PLACE_TOPPOS,
	P_HEATSINK_TRANSFER_X_PLACE_MIDPOS,
	P_HEATSINK_TRANSFER_X_PLACE_BOTPOS,
/*	P_HEATSINK_TRASNFER_X_MEASURE_POS,*/
	P_HEATSINK_TRANSFER_X_INSPECT_POS,
	P_HEATSINK_TRANSFER_X_INSPECT_GABAGE_POS	,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_1,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_2,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_3,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_4,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_1,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_2,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_3,
	P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_4,
};

enum HEATSINK_TRANSFER_Y
{
	P_HEATSINK_TRANSFER_Y_INIT_POS = 0,
	P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS,
	P_HEATSINK_TRANSFER_Y_DISPENSOR_SAFETY_POS,
	P_HEATSINK_TRASNFER_Y_TURN_PLACE_POS,
	P_HEATSINK_TRASNFER_Y_TURN_PICK_POS,
	P_HEATSINK_TRASNFER_Y_TURN_READY_POS,
	P_HEATSINK_TRASNFER_Y_DISPENSER_POS,
	P_HEATSINK_TRANSFER_Y_PLACE_TOPPOS,
	P_HEATSINK_TRANSFER_Y_PLACE_MIDPOS,
	P_HEATSINK_TRANSFER_Y_PLACE_BOTPOS,
	P_HEATSINK_TRANSFER_Y_INSPECT_POS,
	P_HEATSINK_TRANSFER_Y_INSPECT_GABAGE_POS	,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_1,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_2,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_3,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_4,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_1,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_2,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_3,
	P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_4,
};

enum HEATSINK_TRANSFER_Z
{
	P_HEATSINK_TRANSFER_Z_INIT_POS = 0,
	P_HEATSINK_TRANSFER_Z_UP_POS,
	P_HEATSINK_TRANSFER_Z_BOX_DOWN_POS,
	P_HEATSINK_TRANSFER_Z_TURN_PLACE_POS,
	P_HEATSINK_TRANSFER_Z_TURN_PICK_POS,
	P_HEATSINK_TRANSFER_Z_CARRIER_TOP_DOWN_POS,
	P_HEATSINK_TRANSFER_Z_CARRIER_MID_DOWN_POS,
	P_HEATSINK_TRANSFER_Z_CARRIER_BOT_DOWN_POS,
	P_HEATSINK_TRANSFER_Z_INSPECT_POS,
	P_HEATSINK_TRANSFER_Z_INSPECT_GABAGE_POS,
};

enum DISPENSER_Y
{
	P_DISPENSOR_Y_INIT_POS = 0,
	P_DISPENSOR_Y_HEATSINK_POS,
	P_DISPENSOR_Y_DISPENSING_END_POS,
	P_DISPENSOR_Y_LIQUID_CHECK_POS,
	P_DISPENSOR_Y_GABAGE_POS,
};

enum HEATSINK_PICKER_PITCH
{
	P_HEATSINK_PICKER_PITCH_INIT_POS = 0,
	P_HEATSINK_PICKER_PITCH_CLAMP_POS,
	P_HEATSINK_PICKER_PITCH_UNCLAMP_POS,
};

enum HEATSINK_INSPECT_Y
{
	P_HEATSINK_INSPECT_Y_INIT_POS = 0,
	P_HEATSINK_INSPECT_Y_PRESS_START_POS,
	P_HEATSINK_INSPECT_Y_PRESS_END_POS,
	P_HEATSINK_INSPECT_Y_VISION_TOP_POS,
	P_HEATSINK_INSPECT_Y_VISION_MID_POS,
	P_HEATSINK_INSPECT_Y_VISION_BOT_POS,
	P_HEATSINK_INSPECT_Y_VISION_TOP2_POS,
	P_HEATSINK_INSPECT_Y_VISION_MID2_POS,
	P_HEATSINK_INSPECT_Y_VISION_BOT2_POS,
};

enum HEATSINK_INSPECT_Z
{
	P_HEATSINK_INSPECT_Z_INIT_POS = 0,
	P_HEATSINK_INSPECT_Z_VISION_POS,
	P_HEATSINK_INSPECT_Z_VISION_2_POS,
};


//Buffer_Info 3개의 사이트
#define MAX_SHIFT_DATA_NUM		12 //최대 데이타 
//TOP : 7개 사이트  [7][3]
//BTM : 7개 사이트  [7][3]

#define TOP			0
#define MIDDLE		1
#define BTM			2

//BIN_VALUE -> 0:Load_bin 1:Epoxy bin 2: Heat_sink bin 3:Vision_bin 4: 자재 없음
#define  BIN_CDIMM		1
#define  BIN_EPOXY		2
#define  BIN_HEATSINK	3
#define  BIN_VISION		4
#define  BIN_GOOD		5


enum carrier_top_buffer_move_info_shift //carrier buffer가 이동하면서 각각의 정보를 유지 및 생성하면서 쉬프트한다 
{
	TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE = 0,
	
	TOPSHIFT_BUFF_LOADER_RECEIVE,		//Loader에 carrier를 공급한 상태 또는 초기화 후 맨처음 상태
	TOPSHIFT_BUFF_INPUT_LOADER ,				//Epoxy 전 상태
	TOPSHIFT_BUFF_EPOXY,							//EPOXY
	TOPSHIFT_BUFF_WAIT_INDEX,					//Heatsink vison 대기 상태
	TOPSHIFT_BUFF_HEATSINK_VISION,			//Heatsink vision 상태
	TOPSHIFT_BUFF_OUTSEND,								//send 전상태
	TOPSHIFT_BUFF_UNLOADER,							//Unload 상태

	TOPSHIFT_DATA_TEMP_CHECK,
	TOPSHIFT_IDBUFF_SEALING_SITE_ALL_CHK
};

enum carrier_btm_buffer_move_info_shift
{
	BTMSHIFT_BUFF_DOWN,
	BTMSHIFT_BUFF_DOWNFORWARD,
	BTMSHIFT_BUFF_HEATSINK_DOWN,
	BTMSHIFT_BUFF_INDEX_DOWN,
	BTMSHIFT_BUFF_EPOXY_DOWN,
	BTMSHIFT_BUFF_INPUT_DOWN,
	BTMSHIFT_BUFF_LOADER_DOWN,

	BTMSHIFT_DATA_TEMP_CHECK,
	BTMSHIFT_IDBUFF_SEALING_SITE_ALL_CHK
};

struct st_carrier_buffer_info_param
{ 
	int n_idbuffer[3];
	int n_idbuff_num[3];

	int n_last_module[3];
	int n_skip_flag[3];

	int n_exist[3];
	int n_mode;
	//int n_bin[3]; //현재 기준 빈정보를 가진다, 필요하면 더 늘리자	
	int n_bypass[3];//1 : btpass//2014,1102 ybs

	//int nYesNo;										// ssd 유무.....*/
	int nBin[3];										// ssd 테스트 결과.....
	int nRetestCnt[3];								// retest 테스트 횟수......
	int nRetest[3];
	int	nBinNum[3];								// ssd 테스트 결과.....
	int nBdNum[3];								// 마지막 테스트 테스트 Board 번호.......
	int nScrCode[3];								// scrap code.....


	int nSkipMode[3];								//kwlee 2016.0912 TestSite SkipMode
	DWORD dwBdTime[3][3];							// test time......

	int n_totalcnt; //lot 시작시 받은 전체 수량 	
	int n_count[3]; //작업한 수량 //정보는 [0][*]: top [1][*]: middle [2][*]: bottm    [*][0]: load [*][1]: epoxy [*][2]: heat sink [*][3]: vision
	int n_out_count[3][3]; //작업한 수량 //정보는 [0][*]: top [1][*]: middle [2][*]: bottm [*][0]: load [*][1]: heat [*][2]: vison
	int n_buffer_skip_cnt[3];

	char c_lot_id[3][30];
	char c_part_num[3][30];
	char c_chSerialNo[3][30];
	char c_chBarcode[3][30];

	char c_chArrNo[3][30];							// array serial....
	char c_chPPID[3][30];							// ppid.....
	char c_chPSID[3][30];							// psid....
	char c_chWWN[3][30];							// wwn.....
	char c_chCSerialNo[3][30];					// c_serial......

};
extern st_carrier_buffer_info_param	st_carrier_buff_info[MAX_SHIFT_DATA_NUM];


struct st_variable_param
{
	int nBcrNum;//read barcode value

	int n_top_carrier_buffer_exist_status_check_info[40][3]; //총 30개의 정보 플레그 존재
	int n_temp_top_carrier_buffer_exist_status_check_info[40]; //총 30개의 정보 플레그 임시 저장 변수 
	int n_top_carrier_buffer_io_status_read_chk_data[40]; //총 30개의 i/o 상태정보 저장

	int n_btm_carrier_buffer_exist_status_check_info[30][3]; //총 30개의 정보 플레그 존재
	int n_temp_btm_carrier_buffer_exist_status_check_info[30]; //총 30개의 정보 플레그 임시 저장 변수 
	int n_btm_carrier_buffer_io_status_read_chk_data[30]; //총 30개의 i/o 상태정보 저장


	//bottom id buffer 정보 
	//int n_btm_carrier_buffer_exist_check_info[8]; //left gripper [0],[1], right gripper [2],[3], main fix clamp  [4],[5], init fix clamp [6],[7]
	//int n_btm_carrier_buffer_io_status_read_chk_data[8]; //총8개의 i/o 상태정보 저장

};
extern  st_variable_param  st_var;

typedef struct  _RCV_STRUCT
{	
	char sObjectID[20];	// ID : LotID_Count / LotID_Count_H
	int nCmdID;		//1 : 경화제 검사,	2 : Epoxy 도포 검사,	3 : Heatsink 부착 상태 
	int nJudge;		//0: 제어-> 검사 ,	1 : 양품 ,	2 : 불량 
	int nDetailInfo;	// 0 : 제어->검사 , 1 : 경화액 도포 불량 , 2: 점 불량 , 3: 선 두께 불량, 4: 중심점 편심 
	// 5: 삐침 불량 , 6 : 모양 불량 7, 선 짧음 , 8:  heatSink MisAlign
	double x;//2014,0408
	double y;
	
}RCV_STRUCT,*pRCV_STRUCT;

struct st_vision_camera
{
	int		nEpoxyMeasureFlag;
	int		nHeatsinkMeasureFlag;
	BOOL	b7387MeasureCmp;
	BOOL	bEpoxyMeasureCmp;
	BOOL	bHeatsinkMeasureCmp;
	
	int		n7387MeasureResultFlag;
	int		nEpoxyMeasureResultFlag;
	int		nHeatSinkMeasureResultFlag;
	
	int		n7387MeasureAlarmCnt;
	int		nEpoxyMeasureAlarmCnt;
	int		nHeatSinkMeasureAlarmCnt;
	int		nHeatSinkVisionCount;
	int		nHeatSinkGabageCount;
	
	int		nRecipeSelect;
	
	int		n7387MeasureAlarmCnt_B;
	int		nEpoxyMeasureAlarmCnt_B;
	int		nHeatSinkMeasureAlarmCnt_B;
	int		nHeatSinkVisionCount_B;
	int		nHeatSinkGabageCount_B;
	
	//2014,0408
	int		nNobaPosResultFlag;
	BOOL	bNobaPosMeasureCmp;
	
	int		nHeatSinkPosResultFlag;
	BOOL	bHeatSinkMeasureCmp;
	
};
extern	struct st_vision_camera st_vision;
					 
#define BCR_MAX_CNT 4		//barcode read max count
#define BCR_SND_WAIT (1000 * 5)
#define BCR_RCV_WAIT (1000 * 15)

#define BCR_READON	8881	//barcode read start
#define BCR_READOFF	8882	//barcode read finish
#define BCR_ECHO	8883	//barcode parsing echo
#define BCR_NONE	"A"

#define BAR_FULL_INFO			0     // READ한 BCR 데이터 FULL [16]
#define BAR_MINI_INFO			1     // READ된 BCR 데이터 MINI [1]
#define BAR_SET_DEFAULT			2     // 초기 공장 출하 기본 설정
#define BAR_SET_SWITCH			3     // BCR READ 작업의 시작/종료를 사용자 신호로 제어하는 모드
#define BAR_READ_MODE			11    // BCR READ 모드 [READING MODE]
#define BAR_PERCENT				12    // BCR 무한 반복 READ 모드
#define BAR_SETTING_MODE		13    // BCR 옵션 설정 모드
#define BAR_SELF_TEST			15    // SELF TEST MODE
#define BAR_READ_ON				21    // READING START SWITCH
#define BAR_READ_OFF			22    // READING END   SWITCH
#define BAR_VOLUME				40    // VOLUME CONTROL  
#define BAR_CODE_1				51    // Codabar 코드
#define BAR_CODE_2				52    // code 39 코드
#define BAR_CODE_3				53    // UPC 코드
#define BAR_CODE_4				54    // EAN 코드
#define BAR_CODE_5				55    // 2/5-Interleaved 코드
#define BAR_CODE_6				56    // code 93 코드
#define BAR_CODE_7				57    // code 128 코드
#define BAR_CODE_8				58    // EAN 128 코드
#define BAR_ADJUST_MODE			112   // ADJUSTING MODE ENABLES THE CENTER OF THE SCAN LINE
#define BAR_SHOW_LIMIT			113   // SHOW CP LIMIT
#define BAR_RS232				232   // RS232C 통신 방식
#define BAR_RS422				422   // RS422/485 통신 방식
#define BAR_READ_DATA			500

#define BAR_OK					10    // 정상 종료
#define BAR_READ_ERR			30    // READ 실패 [READ 데이터 존재 : 두개 이상의 값이 수신]
#define BAR_ERR					20    // 파싱 에러
#define BAR_READ_OK				40    // BCR READ OK
#define BAR_READ_NO				45    // BCR [NO CODE]
#define BAR_ECHO				50    // ECHO MESSAGE
#define BAR_NO_CODE				"A"

#define BAR_COMM_MODE			1     // 일반 설정 명령어 사용
#define BAR_STEP_MODE			2     // BCR READ 모드 STEP
#define BAR_FLY_MODE			3     // BCR READ 모드 연속
#define BAR_READING				4     // BCR 모드 연속 : 16개 모두 READ 못한 경우
#define BAR_SELFTEST_MODE		5     // SELF TEST 모드
#define BAR_READ_ON_MODE		6     // BCR READ 시작
#define BAR_READ_OFF_MODE		7     // BCR READ 종료

#endif