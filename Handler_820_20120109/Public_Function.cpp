// Public_Function.cpp: implementation of the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Public_Function.h"
#include "FastechPublic_IO.h"
#include "DataAPI.h"
#include <nb30.h>
#include "Run_Device_Carrier_Robot.h"
#include "SrcPart/APartHandler.h"

typedef struct _ASTAT_ 
{ 
	ADAPTER_STATUS adapt; 
	NAME_BUFFER NameBuff[30]; 
	
}ASTAT, * PASTAT; 

ASTAT Adapter; 

#include "Variable.h"				// 전역 변수 정의 Class

// ******************************************************************************
// 대화 상자 클래스 추가                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Message.h"
#include "Dialog_Select.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************

#include "math.h"
#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "FAS_HSSI.h"
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "MyBasicData.h"
#include "Cmmsdk.h"
#include "Alg_Mysql.h"
#include "Dialog_Motor_Part.h"
#include "IO_Manager.h"
#include "AMTLotManager.h"
#include "SrcPart/APartHandler.h"

CMyBasicData     mcls_p_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPublic_Function::CPublic_Function()
{
//	int i;
	m_pInternetFile = NULL;

}

CPublic_Function::~CPublic_Function()
{

}

int CPublic_Function::DoorOpenCheck()
{
	int Ret = CTLBD_RET_GOOD, n_check = 0, i = 0;
	int nDoorState = CTLBD_RET_PROCEED;
	char Jamcode[10];
	
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck1, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980000");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck2, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980001");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck3, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980002");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck4, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980003");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck5, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980004");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck6, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980005");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck7, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980006");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck8, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980007");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck9, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980008");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck10, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980009");
		alarm.mstr_code = _T(Jamcode); 
		
		return CTLBD_RET_ERROR;
	}
	
	return CTLBD_RET_GOOD;
}

void CPublic_Function::OnSet_IO_Port_Stop()
{
// 	if ( st_work.mn_run_status == dSTOP )
// 		return;

	st_work.mn_run_status = dSTOP;
	COMI.mn_run_status = dSTOP;
	
	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_ON);

	FAS_IO.Set_Out_Bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io.o_Dispenser_1_Liquide_Off_Sol, IO_ON);
		
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);

	g_ioMgr.set_out_bit(st_io.o_Safety_Door_Lock_Release, IO_ON);

// 	sprintf(st_msg.c_normal_msg, "[Button] STOP.");
// 	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);

	Func.On_LogFile_Add( LOG_TOTAL, "[Button] STOP." );



	if(st_basic.n_Light_Curtain_Mode == 0)
	{
		/////////스타트 시작시 뮤팅 시작 ..
		st_handler.n_MutingOn = 1;//파워오프 O
		g_handler.nLightCurtainMutingOnStep = 0;
		st_handler.n_HeatSinkMutingOn = 1;
	}

	CString mstr_temp = "";

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDRW);
	:: WritePrivateProfileString("BasicData", "Daily_Run_With_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);	

}

int CPublic_Function::PickerDeviceStatusCheck(int Robot, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM], int Result[PICKER_NUM])
{
//	int i, FuncRet;
//	int nPickerStats;
/*
	if (st_basic.n_mode_device != WITH_DVC) //without device
	//if (Robot == PICKER_UNLOAD || st_basic.i_mode_device == 1 || st_basic.i_mode_device == 2 || st_basic.i_mode_device == 3) //without device
	{
		for (i = 0 ; i < PICKER_NUM ; i++) 
		{
			Result[i] = YES;
		}
		return RET_GOOD;
	}
	
	FuncRet = RET_GOOD;
	
	for (i = 0 ; i < PICKER_NUM ; i++)
	{
		Result[i] = NO;

		if (Robot == PICKER_LOAD)
		{
			nPickerStats = g_ioMgr.Get_In_Bit(st_io_def.i_main_robot_gripper_on_check_sensor[i]);
		}
		else
		{
			
		}

		if (OnOff == IO_ON)		// 디바이스를 집은후  
		{
			if (nPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)  //동작 시킨 피컨데 센싱이 된다면 자재가 없다는 의미!!
			{		
				if (OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

				Result[i] = NO; 
				FuncRet = RET_ERROR;
				
				if (Robot == PICKER_LOAD)
				{
					//400200 1 40 "Loading Picker #1 Module On Check Error."
					sprintf(Jamcode, "40020%d",  i); //자재가 감지가 되야하는데 센싱이 안된다!
				}
				else
				{

				}
				alarm.mstr_code = _T(Jamcode); 
				alarm.mn_count_mode = 0;
				alarm.mn_type_mode = eWARNING;

				alarm.n_alarm_assign_section = 201;
			}	
			else if (nPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)
			{
				Result[i] = YES; 
			}
		}
		else if (OnOff == IO_OFF) // 디바이스를 놓은 후 체크 
		{					
			if (nPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)	//나올리 없겠지.. (나오면 그리퍼가 동작이 안된거다)
			{
				if (OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

				Result[i] = NO;
				FuncRet = RET_ERROR;
				
				if (Robot == PICKER_LOAD)
				{
					//400300 1 40 "Unloading Picker #1 Module Off Check Error."
					sprintf(Jamcode, "40030%d",  i); //피커가 벌렸는데 센서가 ON 되었다..
				}
				else
				{

				}				
				alarm.mstr_code = _T(Jamcode);
				alarm.mn_count_mode = 0;
				alarm.mn_type_mode = eWARNING;

				alarm.n_alarm_assign_section = 202;
			}
			else if (nPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)
			{				 
				Result[i] = YES;
			}
		}
		else					// 피커의 상태를 파악한다 
		{		
			if (nPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
			{	//디바이스가 있어야 하는데 없을때
				if (OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다. (door를 열었다 닫아야 한다!)

				Result[i] = NO;
				FuncRet = RET_ERROR;

				if (Robot == PICKER_LOAD)
				{
					//400200 1 40 "Loading Picker #1 Module On Check Error."
					sprintf(Jamcode, "40020%d",  i); //on check error
				}
				else
				{
				
				}

				alarm.mstr_code = _T(Jamcode);
				alarm.mn_count_mode = 0;
				alarm.mn_type_mode = eWARNING;

				alarm.n_alarm_assign_section = 203;
			}
		}
	}
*/	
	return 0;
}


void CPublic_Function::OnSet_IO_Out_Port_Clear()
{
	int i;

/*	OnSet_IO_Port_Sound(IO_OFF);							// Buzzer를 Off 시킨다.

//	g_ioMgr.Set_Out_Bit(st_io_def.o_main_air, IO_ON);

	g_ioMgr.Set_Out_Bit(st_io_def.o_start_switch_lamp, IO_OFF);
	g_ioMgr.Set_Out_Bit(st_io_def.o_stop_switch_lamp, IO_OFF);
	g_ioMgr.Set_Out_Bit(st_io_def.o_reset_switch_lamp, IO_OFF);
//	g_ioMgr.Set_Out_Bit(st_io_def.o_buzzer_stop_lamp, IO_OFF);
//	g_ioMgr.Set_Out_Bit(st_io_def.o_loader_lamp, IO_OFF);
	g_ioMgr.Set_Out_Bit(st_io_def.o_loader_switch_lamp, IO_OFF);
//	g_ioMgr.Set_Out_Bit(st_io_def.o_dispenser_airblow_sol, IO_OFF);

	g_ioMgr.Set_Out_Bit(st_io_def.o_tower_lamp_blue, IO_OFF);
	g_ioMgr.Set_Out_Bit(st_io_def.o_tower_lamp_red, IO_OFF);
	g_ioMgr.Set_Out_Bit(st_io_def.o_tower_lamp_green, IO_OFF);
	g_ioMgr.Set_Out_Bit(st_io_def.o_tower_lamp_yellow, IO_OFF);

	g_ioMgr.Set_Out_Bit(st_io.o_safety_door_free, IO_ON);*/
//	for (i = 0; i < 3; i++)
//	{
//		g_ioMgr.Set_Out_Bit(st_io_def.o_towerlamp[i], IO_OFF);
//	}
// jong

	OnSet_IO_Port_Sound(IO_OFF);							// Buzzer를 Off 시킨다.

	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);

	for (i = 0; i < 3; i++)
	{
		g_ioMgr.set_out_bit(st_io.o_tower_lamp[i], IO_OFF);
	}

}

void CPublic_Function::OnSet_IO_Port_Sound(int n_status)
{
	switch (n_status)
	{
		case 0:
			g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Buzzer2, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Buzzer3, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Buzzer4, IO_OFF);
			break;
			
		case 1:
			if (st_lamp.mn_buzzer_mode == YES)			// Buzzer 사용 모드에서만 켠다.
			{
				g_ioMgr.set_out_bit(st_io.o_Buzzer1, IO_ON);
				g_ioMgr.set_out_bit(st_io.o_Buzzer2, IO_ON);
				g_ioMgr.set_out_bit(st_io.o_Buzzer3, IO_ON);
				g_ioMgr.set_out_bit(st_io.o_Buzzer4, IO_ON);
			}
			break;
	}
}

void CPublic_Function::PublicVariableDataInitialize()
{
	// **************************************************************************
	// 스위치 및 로딩 LOT 제어 플래그 초기화 부분                                
	// **************************************************************************
	st_work.b_load_key_flag = false;		// 로더 키 ON/OFF 플래그 초기화
//	st_work.b_lot_start_flag = false;		// LOT 시작 플래그 초기화
//	st_handler.n_lot_flag	= FALSE;
	st_work.b_load_off_flag = false;		// 강제 Lot End가 아님.
	// **************************************************************************
	
	st_other.n_pad_dialog = NO;
}

void CPublic_Function::On_IOFlagReset()
{
	int i;
	
	for (i = 0; i < MAX_IO; i++)
	{
		CYLFlag[i] = false;
		IOFlag[i] = false;
		SiteFlag[i] = false;
		IOYFlag[i] = false;
	}
}



void CPublic_Function::OnSet_IO_Port_Alarm()
{
	OnSet_IO_Port_Sound(IO_ON);											// Buzzer를 ON 시킨다.
	
	if (st_work.mn_run_status != dLOTEND)		
	{
		g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_ON);
	}
	
	OnSet_Door_Open();
}

void CPublic_Function::On_LogFile_Add(int n_msg_select, CString str_msg)
{
	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	char chr_buf[20];
	int mn_existence;			// 파일 존재 유무 설정 플래그 
	char fileName[256];			// 검색할 파일 정보 설정 함수 
	FILE  *fp ;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret;

	if (str_msg.IsEmpty() == TRUE)  
	{
		return;
	}

	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime() ;  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// 현재 시간 정보를 설정한다. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");	// 리스트 파일에 출력할 타이틀 시간 정보 설정 
	// **************************************************************************

	switch(n_msg_select)			// Operation Setting
	{		
	case 0:
	case 1:
		mstr_file_name = "OP" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_operation + mstr_file_name;
		break;
	case 2:							// 장비 동작 관련.
		mstr_file_name = "MA" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case 3:							// 장비 디버깅 관련.
		mstr_file_name = "DEBUG" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case 4:							// 장비 디버깅 관련.
		mstr_file_name = "IT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_interface + mstr_file_name;
		break;

	case LOG_LOT_PRO:
		mstr_file_name = "LOT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.str_daily_data_path + mstr_file_name;
		break;

	case 99:						// 전체 메세지.
		mstr_file_name = "TOT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_total + mstr_file_name;
		break;
	}

	mstr_create_file += ".TXT";

	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0) ;
	
	if (mn_existence == -1)
	{
		switch(n_msg_select)			// Operation Setting
		{		
		case 0:
		case 1:
			mstr_file_name = "OP" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_operation + mstr_file_name;
			break;

		case 2:							// 장비 동작 관련.
			mstr_file_name = "MA" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;

		case 3:							// 장비 디버깅 관련.
			mstr_file_name = "DEBUG" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;

		case 4:							// 장비 디버깅 관련.
			mstr_file_name = "IT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_interface + mstr_file_name;
			break;

		case LOG_LOT_PRO:
			mstr_file_name = "LOT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.str_daily_data_path + mstr_file_name;
			break;

		case 99:						// 전체 메세지.
			mstr_file_name = "TOT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_total + mstr_file_name;
			break;
		}
		mstr_create_file += ".TXT";
		// **********************************************************************
	}

	Ret = FileSizeCheck(mstr_create_file, 1048576, YES); //size and rename
	
	if(Ret == 1) //file size over
	{
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	// **************************************************************************
	// 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성              
	// **************************************************************************
	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		AfxMessageBox("File open failure!..");
		return;
	}
	// **************************************************************************

	// **************************************************************************
	// 로그 파일에 현재 발생한 알람 정보 저장한다                                
	// **************************************************************************
	if (st_handler.str_user_id != "")
	{
		mstr_content += "[" + st_handler.str_user_id + "] ";
	}
	
	mstr_content += str_display_time;

	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content) ;

	if (ferror(fp))  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_op_msg = _T("File save failure.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, 7);
		}
		return ;
	}
	// **************************************************************************
	fclose(fp);  // 파일을 종료한다.
}

int CPublic_Function::FileSizeCheck(CString FileName, long size, int n_check)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(FileName))
	{	
		if (n_check == YES)			// Size를 체크하는것이면...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			if(Len > size)
			{
				flag = 1;
			}
			finder.Close();	
		}
		else						// Size를 확인하는것이면...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			
			flag = Len;
			
			finder.Close();	
		}
	}
	
	return flag;
}

void CPublic_Function::OnMot_Speed_Setting()
{
// 	int i;
// 
// 	for (i = 0; i < M_MOTOR_COUNT; i++)
// 	{
// 		if (COMI.md_spd_vel[i][1] < 1)
// 		{
// 			COMI.md_spd_vel[i][1] = 10000;
// 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// //				st_msg.mstr_abnormal_msg.Format("%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
// 				sprintf(st_msg.c_abnormal_msg, "%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
// 			}
// 		}
// 		
// 		if (COMI.md_spd_vel[i][2] < 1)
// 		{
// 			COMI.md_spd_vel[i][2] = 10000;
// 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "%s Motor Dec is under 1 -> 10000 Set up", Get_MotorName(i));
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
// 			}
// 		}
// 
// 		COMI.Set_MotSpeed(MOT_SPD_VEL, i, cmSMODE_T, COMI.md_spd_vel[i][0], COMI.md_spd_vel[i][1], COMI.md_spd_vel[i][2]);
// 		COMI.Set_MotSpeed(MOT_SPD_ORG, i, cmSMODE_T, st_motor[i].md_spd_home, COMI.md_spd_vel[i][1]/10, COMI.md_spd_vel[i][2]/10); //st_motor[i].d_spd_home[0] * 10, st_motor[i].d_spd_home[0] * 10);
// 
// 		COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
// 		Sleep(100);
// 	}
// 
// 	int num;
// 
// 	//0번 보드	Motor No., 기어비 (1회전시 펄스 :10000 / 1회전시 이동거리)
// 	COMI.Set_MotUnitDist(M_TRAY1_Z,				1000); //20mm
// 	COMI.Set_MotUnitSpeed(M_TRAY1_Z,			1000); //20mm
// 
// 	COMI.Set_MotUnitDist(M_TRAY2_Z,				1000);
// 	COMI.Set_MotUnitDist(M_PRESS_Y,				90.7325748);//110.2
// 	COMI.Set_MotUnitDist(M_EPOXY_TRANSFER_X,	90.7325748);
// 	COMI.Set_MotUnitDist(M_EPOXY_TRANSFER_Y,	90.7325748);//90.7358679//// 110.214
// 	COMI.Set_MotUnitDist(M_EPOXY_TRANSFER_Z,	1000);
// 	COMI.Set_MotUnitDist(M_EPOXY_SCREW,			10000);
// 	COMI.Set_MotUnitDist(M_CARRIER_X,			90.7325748);
// 	///1번 보드 
// 	COMI.Set_MotUnitDist(M_LOADER_TRANSFER_Y,	90.7325748); //110.214
// 	COMI.Set_MotUnitDist(M_LOADER_TRANSFER_Z,	1000);			//109.9314
// 	COMI.Set_MotUnitDist(M_HEATSINK_TRANSFER_X,	90.96582);
// 	COMI.Set_MotUnitDist(M_HEATSINK_TRANSFER_Y,	90.96582);
// 	COMI.Set_MotUnitDist(M_HEATSINK_TRANSFER_Z,	1000);
// 	COMI.Set_MotUnitDist(M_UNLOADER_TRANSFER_X,	90.7325748);
// 	COMI.Set_MotUnitDist(M_UNLOADER_TRANSFER_Y,	90.7325748);
// 	COMI.Set_MotUnitDist(M_UNLOADER_TRANSFER_Z,	1000);
// 	COMI.Set_MotUnitDist(M_DISPENSER_Y,		1094.4032);	//91.374
// 	COMI.Set_MotUnitDist(M_TRAY_REMOVE_X,	90.7325748);
// 	COMI.Set_MotUnitDist(M_HEATSINK_INSPECT_Y,	90.7325748);
// 
// 	COMI.Set_MotUnitDist(M_HEATSINK_INSPECT_Z, 1000);//20130716
// 	COMI.Set_MotUnitDist(M_HEATSINK_PICKER_PITCH, 500.3411417);//1.);//3.1825);//1.273);//39.9722);//20130716 20131010 20131216 
// 	////
// 
// 	num = 0;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 1;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 2;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 3;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 4;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 5;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 6;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 7;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 8;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 9;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 10;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 11;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 12;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 
// 	num = 13;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 14;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 
// 	num = 15;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 16;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 17;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 
// 	num = 18;
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 
// 	//////////////////20130716
// 	num = 19;//camera z
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	
// 	num = 20;//picker pitch
// 	cmmCfgSetCtrlMode(num, 1);
// 	st_motor[num].mn_homecheck_method = 6;
// 	//////////////////


}

void CPublic_Function::OnSet_IO_Port_Run()
{	
	if ( st_work.mn_run_status == dRUN )
		return;

	st_work.mn_run_status = dRUN;
	COMI.mn_run_status = dRUN;

	//레벨 초기화.
	st_handler.n_level_teach = FALSE;
	st_handler.n_level_maint = FALSE;
	
	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp,	IO_ON);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp,	IO_OFF);
// 	g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_ON);
// 	g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_Off_Sol, IO_OFF);

	// 도어를 잠근다.
	g_ioMgr.set_out_bit( st_io.o_Safety_Door_Lock_Release, IO_OFF );
	
// 	sprintf(st_msg.c_normal_msg, "[Button] RUN.");
// 	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);

	Func.On_LogFile_Add( LOG_TOTAL, "[Button] RUN." );

	if(st_handler.cwnd_title != NULL)
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
	}


	st_handler.n_MutingOff = 1;//파워오프 O
	//st_handler.n_HeatSinkMutingOff = 1; //no use
	//Func.nLightCurtainHeatSinkMutingOffStep = 0;//no use
	st_handler.nHeatSinkLightCurtainFlag = 0;
	st_handler.nStackerLightCurtainFlag = 0;
}

void CPublic_Function::OnSet_IO_Port_Init()
{
	st_work.mn_run_status = dINIT;	
	OnSet_Door_Close();
}

CString CPublic_Function::GetLocalMacAddress()
{
	NCB Ncb; 
	UCHAR uRetCode; 
	LANA_ENUM lenum; 
	int i; 
	CString str_value; 
	
	memset( &Ncb, 0, sizeof(Ncb) ); 
	Ncb.ncb_command = NCBENUM; 
	Ncb.ncb_buffer = (UCHAR *)&lenum; 
	Ncb.ncb_length = sizeof(lenum); 
	uRetCode = Netbios( &Ncb ); 
	
	for (i = 0; i < lenum.length; i++) 
	{ 
		memset(&Ncb, 0, sizeof(Ncb)); 
		Ncb.ncb_command = NCBRESET; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		uRetCode = Netbios( &Ncb ); 
		memset(&Ncb, 0, sizeof (Ncb)); 
		Ncb.ncb_command = NCBASTAT; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		strcpy((char*)Ncb.ncb_callname, "* "); 
		Ncb.ncb_buffer = (unsigned char *) &Adapter; 
		Ncb.ncb_length = sizeof(Adapter); 
		
		uRetCode = Netbios( &Ncb ); 
		
		if (uRetCode == 0) 
		{ 
			str_value.Format("%02x-%02x-%02x-%02x-%02x-%02x", 
				Adapter.adapt.adapter_address[0], 
				Adapter.adapt.adapter_address[1], 
				Adapter.adapt.adapter_address[2], 
				Adapter.adapt.adapter_address[3], 
				Adapter.adapt.adapter_address[4], 
				Adapter.adapt.adapter_address[5] ); 
		} 
	} 	
	return str_value;
}

int CPublic_Function::OnMotor_Go_Check(int n_motor, double d_pos)
{
	CString str_msg, str_motor;
	
	int Ret = NO, n_response;
	
	CDialog_Select	select_dlg;

	str_motor = Func.Get_MotorName(n_motor);
	str_msg.Format("%s Motor, Move value is %.3f, Go?", str_motor, d_pos);
	
	st_other.str_confirm_msg = _T(str_msg);
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK) 
	{
		Ret = YES;
	}
	else
	{
		Ret = NO;
	}

	return Ret;
}

int CPublic_Function::On_Encoder_Check(double d_pos)
{	
	CString str_msg;
	
	int n_response, Ret = NO;
	
	CDialog_Select	select_dlg;
		
	str_msg.Format("Now Reading Value is %ld, Setting?", (long)d_pos);
	st_other.str_confirm_msg = _T(str_msg);
	n_response = select_dlg.DoModal();
	if (n_response == IDOK) 
	{
		Ret = YES;
	}
	else
	{
		Ret = NO;
	}
	
	return Ret;
}

void CPublic_Function::OnSet_IO_Port_Load_On()
{
	/* ************************************************************************** */
	/* 로딩 관련 플래그 설정한다                                                  */
	/* -> st_work.b_load_key_flag : 로딩 작업 ON/OFF 상태 플래그                  */
	/* -> st_work.b_lot_start_flag : LOT 시작 플래그                              */
	/* ************************************************************************** */
	st_work.b_load_key_flag = true;
//	st_work.b_lot_start_flag = true;
//j	st_handler.n_lot_flag   = TRUE;
	/* ************************************************************************** */
//	g_ioMgr.Set_Out_Bit(st_io_def.o_loader_lamp, IO_ON);
//	g_ioMgr.Set_Out_Bit(st_io.o_loader, IO_ON);

//	g_ioMgr.Set_Out_Bit(st_io_def.o_dispenser_airblow_sol, IO_ON);
}

void CPublic_Function::OnSet_IO_Port_Load_Off()
{
	// **************************************************************************
	// 로딩 관련 플래그 설정한다                                                 
	// -> st_work.b_load_key_flag : 로딩 작업 ON/OFF 상태 플래그                 
	// -> st_work.b_lot_start_flag : LOT 시작 플래그                             
	// **************************************************************************
	st_work.b_load_key_flag = false;
	// **************************************************************************
//	g_ioMgr.Set_Out_Bit(st_io_def.o_loader_lamp, IO_OFF);
//	g_ioMgr.Set_Out_Bit(st_io.o_loader, IO_OFF);

//j	g_ioMgr.Set_Out_Bit(st_io_def.o_dispenser_airblow_sol, IO_OFF);
}



void CPublic_Function::OnDayDataCheck()
{
	CString mstr_chk_temp;
	int mn_chk_year, mn_chk_month, mn_chk_day, mn_chk_hour; // Data 저장 시간정보 저장 변수 
	COleDateTime time_cur;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 시간정보 저장 변수 
	
//	mstr_chk_temp.Format("%10d", st_work.n_data_save_time);
	
	mn_chk_year = atoi(mstr_chk_temp.Mid(0, 4));
	mn_chk_month = atoi(mstr_chk_temp.Mid(4, 2));
	mn_chk_day = atoi(mstr_chk_temp.Mid(6, 2));
	mn_chk_hour = atoi(mstr_chk_temp.Mid(8, 2));
	
	time_cur = COleDateTime::GetCurrentTime();
	
	mn_cur_year = time_cur.GetYear();  
	mn_cur_month = time_cur.GetMonth();  
    mn_cur_day = time_cur.GetDay();  
	mn_cur_hour = time_cur.GetHour();
	
	// 현재 시간이 Day Start Time이 되기 전이라면...
/*	if (mn_cur_hour < st_basic.n_day_start_time)
	{
		if (mn_chk_day == mn_cur_day)						// 저장된 날짜와 현재 날짜가 같다면...
		{
			
		}
		else												// 같지 않다면 지난 Data이다.
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// 저장된 시간이 Day Start 시간보다 이전이면.. Reset이다.
			{
				OnDayDataReset();							// Day Data Reset
			}
		}
	}
	else		// 현재 시간이 Day Start Time 이후라면...
	{
		if (mn_chk_day == mn_cur_day)						// 저장된 날짜와 현재 날짜가 같다면...
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// 저장된 시간이 Day Start 시간보다 이전이면.. Reset이다.
			{
				OnDayDataReset();							// Day Data Reset
			}
		}
		else												// 같지 않다면 지난 Data이다.
		{
			OnDayDataReset();								// Day Data Reset
		}
	}*/
}

void CPublic_Function::OnDayDataReset()
{
	CString mstr_temp;
// 	int k,j;
	
	st_handler.m_tDR = 0;
	st_handler.m_tDJ = 0;
	st_handler.m_tDM = 0;
	st_handler.m_tDS = 0;
	st_handler.m_tDRef = 0;
// 	st_handler.m_tDRdown = 0;
// 	for ( k = 0; k < 10; k++)
// 	{
// 		for ( j = 0; j < 2; j++)
// 		{
// 			st_handler.m_tDtest[j][k] = 0;
// 			st_handler.m_tDSokOff[j][k] = 0;
// 			st_handler.m_tDInStandby[j][k] = 0;
// 			st_handler.m_tDOutStandby[j][k] = 0;
// 		}
// 	}
	
	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	

	
	g_lotMgr.ClearCountData();
	g_handler.ClearAlarmCnt();
	
	if (st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
	}
}

BOOL CPublic_Function::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	
	CString strPath;// = szPath;
	
	strPath.Format("%s", szPath);
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            CString strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

int CPublic_Function::OnMotMove_SafetyCheck(int n_axis, int n_move)
{

	return CTLBD_RET_GOOD;
}



void CPublic_Function::OnSet_Door_Open()
{
	// 메뉴얼 모드라면 도어를 연다.
	if (st_handler.n_mode_manual == YES)
	{
		//도어 오픈
		g_ioMgr.set_out_bit(st_io.o_Safety_Door_Lock_Release, IO_ON);
//		g_ioMgr.set_out_bit(st_io.o_left_door_lock_cyl, IO_OFF);
//		g_ioMgr.set_out_bit(st_io.o_right_door_lock_cyl, IO_OFF);
	}
}

void CPublic_Function::OnSet_Door_Close()
{
	//도어 락
	g_ioMgr.set_out_bit(st_io.o_Safety_Door_Lock_Release, IO_OFF);
//	g_ioMgr.set_out_bit(st_io.o_left_door_lock_cyl, IO_ON);
//	g_ioMgr.set_out_bit(st_io.o_right_door_lock_cyl, IO_ON);
}

int CPublic_Function::Find_TrayRowColumns(int Site, int TrayNum, int RowColPos, int BinNum, int DVCYesNo, int tcountmode)
{
//	int i, j, Num, 
	int FuncRet = 0;
	 
//	Num = -1; 
/*
	switch(Site)
	{
	case LOADTRAY:
		for (j = 0; j < st_traybuffer.n_loader_y; j++)
		{
			for (i = 0; i < st_traybuffer.n_loader_x; i++)
			{
				if (st_modulemap.ModuleTray[TrayNum][j][i] == YES && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_READY)		// 작업전 디바이스가 있으면 
				{		
					if (tcountmode == COUNT_NO)					// 위치체크이고, 카운터 체크가 아니면 
					{
						if (RowColPos == TRAY_X) Num = j;		
						else Num = i;

						i = st_traybuffer.n_loader_x + 1; j = st_traybuffer.n_loader_y + 1;		// for문을 빠져 나가기 위함이다!
					}
					else
					{	// 카운터 체크 모드이면 
						if (RowColPos == TRAY_X) Num++;		
						else Num++;
					}
				}
			}
		*/
		// 역방향용. 2K10/08/04/ViboX
//		for (j = 0; j < st_traybuffer.n_loader_y; j++)
/*		for (j = (st_traybuffer.n_loader_y - 1); j >= 0; j--)
		{
			for (i = (st_traybuffer.n_loader_x - 1) ; i >= 0 ; i--)
			{
				if (st_modulemap.ModuleTray[TrayNum][j][i] == YES && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_READY)		// 작업전 디바이스가 있으면 
				{		
					if (tcountmode == COUNT_NO)					// 위치체크이고, 카운터 체크가 아니면 
					{
						if (RowColPos == TRAY_Y) Num = i;		
						else Num = j;
						
						i = -1 ; j = -1; // for문을 빠져 나가기 위함이다!
					}
					else
					{	// 카운터 체크 모드이면 
						if (RowColPos == TRAY_Y) Num++;		
						else Num++;
					}			
				}
			}
		}
		break;

 	case UNLOADTRAY:
// 		for (j = 0; j < st_traybuffer.n_loader_y; j++)
// 		{
// 			for (i = 0; i < st_traybuffer.n_loader_x; i++)
// 			{
// 				if (st_modulemap.ModuleTray[TrayNum][j][i] == NO && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_NOTUSE)		// 작업전 디바이스가 있으면 
// 				{		
// 					if (tcountmode == COUNT_NO)					// 위치체크이고, 카운터 체크가 아니면 
// 					{
// 						if (RowColPos == TRAY_X) Num = j;		
// 						else Num = i;
// 
// 						i = st_traybuffer.n_loader_x + 1; j = st_traybuffer.n_loader_y + 1;		// for문을 빠져 나가기 위함이다!
// 					}
// 					else
// 					{	// 카운터 체크 모드이면 
// 						if (RowColPos == TRAY_X) Num++;		
// 						else Num++;
// 					}
// 				}
// 			}
// 		}

		// 역방향용. 2K10/08/04/ViboX
		for (j = (st_traybuffer.n_loader_y - 1); j >= 0; j--)
		{
			for (i = (st_traybuffer.n_loader_x - 1) ; i >= 0 ; i--)
			{
				if (st_modulemap.ModuleTray[TrayNum][j][i] == NO && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_NOTUSE)		// 작업전 디바이스가 있으면 
				{		
					if (tcountmode == COUNT_NO)					// 위치체크이고, 카운터 체크가 아니면 
					{
						if (RowColPos == TRAY_Y) Num = i;		
						else Num = j;
						
						i = -1 ; j = -1; // for문을 빠져 나가기 위함이다!
					}
					else
					{	// 카운터 체크 모드이면 
						if (RowColPos == TRAY_Y) Num++;		
						else Num++;
					}			
				}
			}
		}
		break;

	case REJECTTRAY:
		for (j = 0; j < st_traybuffer.n_loader_y; j++)
		{
			for (i = 0; i < st_traybuffer.n_loader_x; i++)
			{
				if (st_modulemap.ModuleTray[TrayNum][j][i] == NO && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_NOTUSE)		// 작업전 디바이스가 있으면 
				{		
					if (tcountmode == COUNT_NO)					// 위치체크이고, 카운터 체크가 아니면 
					{
						if (RowColPos == TRAY_X) Num = j;		
						else Num = i;
						
						i = st_traybuffer.n_loader_x + 1; j = st_traybuffer.n_loader_y + 1;		// for문을 빠져 나가기 위함이다!
					}
					else
					{	// 카운터 체크 모드이면 
						if (RowColPos == TRAY_X) Num++;		
						else Num++;
					}
				}
			}
		}
		break;
	}
	
	if (tcountmode == COUNT_NO)  //카운터를 하지 않고, 위치만 찾는 모드이면 
	{	
		if (Num < 0)
		{
			FuncRet = RET_NOWHERE;		//해당 정보의 위치를 찾지 못했다..
		}
		else FuncRet = Num;
	}
	else
	{	// 카운터만 체크 
		if (Num < 0)
		{
			FuncRet = 0;				//해당 정보의 자재 수량이 0 (없다)
		}
		else FuncRet = Num + 1;
	}
*/
	return FuncRet;
}

int CPublic_Function::CheckPickBin()
{
//	int i,nNum= 0;
/*	for (i = 0 ; i < SORTER_PICKER_NUM ; i++) 
	{
		if (st_modulemap.nSorterRobotPicker[D_EXIST][i] == DVC_YES && st_modulemap.nSorterRobotPicker[D_BIN][i] == BIN_PICKBIN)
		{
			nNum++;
		}
	}
	return nNum;*/
	return 0;
}

int CPublic_Function::Find_WhereToGoRobotTrayPos(int Site, int TrayNum, int PickerInfo[D_INFOSIZE][SORTER_PICKER_NUM], int InfoSetFlag, int BinNum)
{
/*	int FirstPicker, FirstRowPos, TrayColumnPos, RobotPos = -1, i = 0;
	int FirstYPos = -1, TrayXPos = -1;
	int iLoadingLeftRight = LEFT_SITE, SecondPicker = -1, SecondTrayColumnPos = -1, SecondTrayRowPos = -1;
	TrayColumnPos = -1; FirstRowPos = -1; FirstPicker = -1;
	
	switch (Site)
	{
	case LOADTRAY:
		// 작업할 첫번째 피커를 결정하는 곳
		for (i = (SORTER_PICKER_NUM - 1) ; i >= 0 ; i--)	// 역방향. 2K10/08/04/ViboX
		{
			if (PickerInfo[D_EXIST][i] == DVC_NO && st_modulemap.nSorterPickerEnable[i] == YES)
			{
				FirstPicker = i;					
				st_work.n_FirstPickerNum[LOADTRAY] = FirstPicker;
				break;
			}
		}

		if (FirstPicker == -1) 
		{
			RobotPos = RET_PICKERNOTFIND;
			break;
		}

		TrayXPos = Find_TrayRowColumns(LOADTRAY, TrayNum, TRAY_X, BIN_NOTUESD, DVC_YES, COUNT_NO);
		FirstYPos = Find_TrayRowColumns(LOADTRAY, TrayNum, TRAY_Y, BIN_NOTUESD, DVC_YES, COUNT_NO);

		if (TrayXPos == RET_NOWHERE)
		{
			RobotPos = RET_NOSPACE; 
			break;
		}

		if (FirstYPos == RET_NOWHERE)
		{
			RobotPos = RET_NOSPACE;
			break;
		}

		RobotPos = FirstYPos - (FirstPicker * PICKING_GAP);

		if (InfoSetFlag == INFO_NO) break; //row,col 정보를 전역변수로 넘기지 않음

		if (TrayXPos >= 0 && TrayXPos < st_traybuffer.n_loader_y)
		{
			st_modulemap.CurrentX[Site] = TrayXPos;
		}

		if (FirstYPos >= 0 && FirstYPos < st_traybuffer.n_loader_x)
//		if (RobotPos >= 0 && RobotPos < st_traybuffer.n_loader_x)
		{
			st_modulemap.CurrentY[Site] = FirstYPos;
//			st_modulemap.CurrentY[Site] = RobotPos;
		}
		break;

	case UNLOADTRAY:
		//작업할 첫번째 피커를 결정하는 곳
// 		for (i = 0 ; i < SORTER_PICKER_NUM ; i++) 
// 		{
// 			if (PickerInfo[D_EXIST][i] == DVC_YES && PickerInfo[D_BIN][i] == BinNum) // 디바이스를 잡고있지않은 피커번호 (집으려고 찾는거니까..)
// 			{
// 				FirstPicker = i;
// 				st_work.n_FirstPickerNum[UNLOADTRAY] = FirstPicker;
// 				break;
// 			}
// 		}

		for (i = (SORTER_PICKER_NUM - 1) ; i >= 0 ; i--)	// 역방향. 2K10/08/04/ViboX
		{
			if (PickerInfo[D_EXIST][i] == DVC_YES && PickerInfo[D_BIN][i] == BinNum)
			{
				FirstPicker = i;					
				st_work.n_FirstPickerNum[UNLOADTRAY] = FirstPicker;
				break;
			}
		}
		
		if (FirstPicker == -1) 
		{
			RobotPos = RET_PICKERNOTFIND;
			break;
		}
		
		TrayXPos = Find_TrayRowColumns(UNLOADTRAY, TrayNum, TRAY_X, BIN_NOTUESD, DVC_YES, COUNT_NO);
		FirstYPos = Find_TrayRowColumns(UNLOADTRAY, TrayNum, TRAY_Y, BIN_NOTUESD, DVC_YES, COUNT_NO);
		
		if (TrayXPos == RET_NOWHERE)
		{
			RobotPos = RET_NOSPACE; 
			break;
		}
		
		if (FirstYPos == RET_NOWHERE)
		{
			RobotPos = RET_NOSPACE;
			break;
		}
		
		RobotPos = FirstYPos - (FirstPicker * PICKING_GAP);
		
		if (InfoSetFlag == INFO_NO) break; //row,col 정보를 전역변수로 넘기지 않음
		
		if (TrayXPos >= 0 && TrayXPos < st_traybuffer.n_loader_y)
		{
			st_modulemap.CurrentX[Site] = TrayXPos;
		}
		
		if (FirstYPos >= 0 && FirstYPos < st_traybuffer.n_loader_x)
		{
			st_modulemap.CurrentY[Site] = FirstYPos;
		}		
		break;
		
	case REJECTTRAY:
		for (i = 0 ; i < SORTER_PICKER_NUM ; i++) 
		{
			if (PickerInfo[D_EXIST][i] == DVC_YES && PickerInfo[D_BIN][i] == BinNum) // 디바이스를 잡고있지않은 피커번호 (집으려고 찾는거니까..)
			{
				FirstPicker = i;
				st_work.n_FirstPickerNum[REJECTTRAY] = FirstPicker;
				break;
			}
		}
		
		if (FirstPicker == -1) 
		{
			RobotPos = RET_PICKERNOTFIND;
			break;
		}
		
		TrayXPos = Find_TrayRowColumns(REJECTTRAY, TrayNum, TRAY_X, BIN_NOTUESD, DVC_YES, COUNT_NO);
		FirstYPos = Find_TrayRowColumns(REJECTTRAY, TrayNum, TRAY_Y, BIN_NOTUESD, DVC_YES, COUNT_NO);
		
		if (TrayXPos == RET_NOWHERE)
		{
			RobotPos = RET_NOSPACE; 
			break;
		}
		
		if (FirstYPos == RET_NOWHERE)
		{
			RobotPos = RET_NOSPACE;
			break;
		}
		
		RobotPos = FirstYPos - (FirstPicker * PICKING_GAP);
		
		if (InfoSetFlag == INFO_NO) break; //row,col 정보를 전역변수로 넘기지 않음
		
		if (TrayXPos >= 0 && TrayXPos < st_traybuffer.n_loader_y)
		{
			st_modulemap.CurrentX[Site] = TrayXPos;
		}
		
		if (FirstYPos >= 0 && FirstYPos < st_traybuffer.n_loader_x)
		{
			st_modulemap.CurrentY[Site] = FirstYPos;
		}	
		break;
	}
	
	return RobotPos;*/
	return 0;
}

void CPublic_Function::OnLotDataTempLoggin(int iMode)
{
	CString str_temp_data;
	CTime temp_endtime;
/*	
	if (iMode == 0)
	{
		str_temp_data = st_work.str_lot_name;
		::WritePrivateProfileString("TempLotInfor", "File_Name", str_temp_data, st_path.str_tempLotInfor);
		::WritePrivateProfileString("TempLotInfor", "Load_Device", "0", st_path.str_tempLotInfor);
		
		str_temp_data = st_work.m_lot_start_time.Format("%Y%m%d%H%M%S");
		::WritePrivateProfileString("TempLotInfor", "Start__Time", str_temp_data, st_path.str_tempLotInfor);
		::WritePrivateProfileString("TempLotInfor", "Finish_Time", "N/A", st_path.str_tempLotInfor);
		::WritePrivateProfileString("TempLotInfor", "Product_Name", "N/A", st_path.str_tempLotInfor);
		::WritePrivateProfileString("TempLotInfor", "Lot_Number", "N/A", st_path.str_tempLotInfor);
	}
	else if (iMode == 1)
	{
		str_temp_data = st_work.str_rcv_product_name;
		::WritePrivateProfileString("TempLotInfor", "Product_Name", str_temp_data, st_path.str_tempLotInfor);
		
		str_temp_data = st_work.str_rcv_lot_number;
		::WritePrivateProfileString("TempLotInfor", "Lot_Number", str_temp_data, st_path.str_tempLotInfor);
	}
	else if (iMode == 2)
	{
		//정상이라면 이미 저장된게 있을테니 일단 skip하자~~
		::WritePrivateProfileString("TempLotInfor", "Load_Device", "0", st_path.str_tempLotInfor);
		
		str_temp_data = st_work.m_lot_end_time.Format("%Y%m%d%H%M%S");
		::WritePrivateProfileString("TempLotInfor", "Finish_Time", str_temp_data, st_path.str_tempLotInfor);
	}
	else if (iMode == 3)
	{
		::WritePrivateProfileString("TempLotInfor", "Load_Device", "0", st_path.str_tempLotInfor);
		
		// 이전 Lot에 대해서 임시로 종료시간을 저장하기 위함..
		temp_endtime = CTime::GetCurrentTime();
		
		str_temp_data = temp_endtime.Format("%Y%m%d%H%M%S");
		::WritePrivateProfileString("TempLotInfor", "Finish_Time", str_temp_data, st_path.str_tempLotInfor);
	}
	else if (iMode == 4)
	{
		//작업한 물량이 있으니 정상적으로 마무리가 안되면 PGM최초 시작시 마무리 해야한다는 설정 : Load_Device = 1
		::WritePrivateProfileString("TempLotInfor", "Load_Device", "1", st_path.str_tempLotInfor);
	}*/
}

void CPublic_Function::OnTestResultData_Save(int n_mode, CString cpBindata)
{
	int iRet=1;
	int i=0;
	
//	char* cpMessage; 
//	char* cpFilename;
	
	CString mMessage;
	
	CTime tTime;
	CString mCurrenttime; 
	CString mFilename;
	
//j	if (st_handler.n_testresult_save != 1)		return;						// 1이 아니면 리턴 해버린다.
	
	tTime = CTime::GetCurrentTime();
	
	mMessage = tTime.Format("%Y%m%d"); //년,월,일
	
	/*
	mFilename = st_path.str_test_result + mMessage + ".log";
	
	FILE *fp;
	
	mCurrenttime = tTime.Format("%Y/%m/%d %H:%M:%S");	//2000.11.08 mJamtime.Format("%ld", tTime);
	
	switch (n_mode)
	{
	case 0:				// Lot Start
		mMessage = mCurrenttime;
		mMessage += "[" + cpBindata + "]";
		mMessage += " Lot Start";
		break;
		
	case 1:				// Lot End
		mMessage = mCurrenttime;
		mMessage += "[" + cpBindata + "]";
		mMessage += " Lot End";
		break;
		
	case 2:				// Test Result
		mMessage = mCurrenttime + " " + cpBindata;
		break;
	}
	
	cpMessage = (LPSTR)(LPCTSTR)mMessage;
	cpFilename = (LPSTR)(LPCTSTR)mFilename;
	
	if((fp = fopen(cpFilename,"a+")) != NULL)
	{
		fprintf(fp, "%s\n", cpMessage);	
		fclose(fp);
	}*/
}

int CPublic_Function::OnBCR_CheckData(CString strCheckData)
{
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	
//	char  cp_cmd[1024];
	
//	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if (nLength<3)
		return FALSE;
	// **************************************************************************
	
	strBody = strCheckData.Mid(1, nLength-2);  // [헤더+테일] 제거 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if (strBody.Find("ERR") >= 0)
	{
		return FALSE;
	}
	// **************************************************************************
		
	return TRUE;
}


int CPublic_Function::Check_BufferStatus(int nBufferMode, int nSite, int nYesNo, int *nBufferInfo, int *nBufferStatus, int *npBufferOutputStatus)
{
	int nFuncRet = RET_GOOD;
	int i, nRet[4]={0,};
	int nIO_Num_Info[MAX_BUF_SIZE];
	int nIO_Output_Num_Info[MAX_BUF_SIZE];

	int nCurrent_BufferInfo[MAX_BUF_SIZE];
	int nPara_Num;//

	if(nSite == THD_LD_ALIGN_BUFF) 
	{
		nPara_Num = st_recipe.nLdBuffer_Num;
		nIO_Num_Info[0] = st_io.i_Loading_Tr_Jig_Detect_Check; 

		//체크시에는 항상 OFF이다.
		if( nYesNo == CTL_YES)
		{
			nIO_Output_Num_Info[0] = st_io.o_Loader_Align_Backward_Sol; 
		}
		else
		{
			nIO_Output_Num_Info[0] = st_io.o_Loader_Align_Forward_Sol;
		}

		for(i = 0; i < nPara_Num; i++)
		{
			if(nBufferMode == 0)	 //최종적으로 체크할 전역 변수를 기준으로 센서 상태 상태 확인 
			{
				nCurrent_BufferInfo[0] = st_buffer_info[nSite].st_pcb_info[i].nYesNo;
			}
			else if(nBufferMode == 1) //함수 인자(멤버변수) 사용하여 센서 상태 상태 확인  
			{
				nCurrent_BufferInfo[0] = nBufferInfo[i];
			}
		}
	}
	else if(nSite == THD_LD_HSALIGN_BUFF) 
	{
		nPara_Num = st_recipe.nLdHsAlignBuffer_Num;

		nIO_Num_Info[0] = st_io.i_HeatSink_Transfer_CellIn_Check; 
		
		nIO_Output_Num_Info[0] = st_io.o_HeatSink_Reverse_180_Sol; //회전이 안됐을 경우
		//nIO_Output_Num_Info[1] = st_io.o_HeatSink_Reverse_180_Sol; 

		for(i = 0; i < nPara_Num; i++)
		{
			if(nBufferMode == 0)
			{
				nCurrent_BufferInfo[i] = st_buffer_info[nSite].st_pcb_info[i].nYesNo;
			}
			else if(nBufferMode == 1)
			{
				nCurrent_BufferInfo[i] = nBufferInfo[i];
			} 
		}
	}
	else if(nSite == THD_LD_HSPICK_BUFF) 
	{
		nPara_Num = st_recipe.nLdHsAlignBuffer_Num;

		nIO_Num_Info[0] = st_io.i_Dispenser_Spray_Forward_Check; 
		//nIO_Num_Info[0] = st_io.i_Dispenser_Spray_Backward_Check; 

		nIO_Output_Num_Info[0] = st_io.o_HeatSink_Reverse_180_Sol; //디바이스를 집고 원래로 복귀했을 경우 
		//nIO_Output_Num_Info[1] = st_io.o_HeatSink_Reverse_180_Sol; 

		for(i = 0; i < nPara_Num; i++)
		{
			if(nBufferMode == 0)
			{
				nCurrent_BufferInfo[i] = st_buffer_info[nSite].st_pcb_info[i].nYesNo;
			}
			else if(nBufferMode == 1)
			{
				nCurrent_BufferInfo[i] = nBufferInfo[i];
			} 
		}
	}
	else 
	{//이곳으로 오면 안됨 
		return RET_ERROR;
	}

	for(i = 0; i < nPara_Num; i++)
	{
		nBufferStatus[i] = g_ioMgr.get_in_bit(nIO_Num_Info[i],	IO_ON);  
		npBufferOutputStatus[i] = g_ioMgr.get_out_bit(nIO_Output_Num_Info[i],	IO_ON);
	}	

	if(st_basic.n_mode_device == WITHOUT_DVC) //with device 가 아니면 
	{
		for(i = 0; i < nPara_Num; i++)
		{
			if(nBufferMode == 100) //무조건 자재가 모두 없어야 한다 
			{
				if((nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) || st_buffer_info[nSite].st_pcb_info[i].nYesNo == CTL_YES)
				{  				 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
					return nFuncRet;
				}
				continue;
			}

			if(nYesNo == CTL_YES) //디바이스가 있는 상태를 체크시 
			{
				if(nCurrent_BufferInfo[i] == CTL_YES)
				{
					if(npBufferOutputStatus[i] == IO_OFF)
					{
						nBufferStatus[i] = IO_ON;
					}
					else
					{
						nBufferStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리 
						if(nSite == THD_LD_ALIGN_BUFF) //Dispenbuffer(B접점)와 반대 
						{
							if( st_basic.n_mode_device == WITHOUT_DVC )
							{
								if( nYesNo == CTL_YES) nBufferStatus[i] = CTL_YES;
							}
						}

					}
				}
			}
			else //디바이스가 없는 상태를 체크시 
			{
				if(nCurrent_BufferInfo[i] == CTL_YES) 
				{
					if(npBufferOutputStatus[i] == IO_ON)//james 2016.0919 그리퍼 벌린 상태 
					{
						nBufferStatus[i] = IO_ON; // open하고 있어 정상 처리함 
					}
					else
					{
						nBufferStatus[i] = IO_OFF; //clamp 하고 있는 상태로 문제가 있어 에러처리 
					}
				}
			}
		}
		return RET_GOOD;
	}

	//with device
	for (i = 0; i < nPara_Num; i++) 
	{
		if(nBufferMode == 100) //무조건 자재가 모두 없어야 한다 
		{
			if((nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) || st_buffer_info[nSite].st_pcb_info[i].nYesNo == CTL_YES)
			{  				 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
				nFuncRet = RET_ERROR;
				return nFuncRet;
			}
			continue;
		}

		if(nYesNo == CTL_YES)	//디바이스가 있는 상태를 체크할 때 
		{
			if(nCurrent_BufferInfo[i] == CTL_YES) //CTL_YES: 피커 정보가 디바이스가 있다, CTL_NO:피커 정보가 디바이스가 없다
			{			

				if(nBufferStatus[i] == IO_OFF || npBufferOutputStatus[i] == CTL_NO)
				{	//error   //감지 안됨 에러 					
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
				}
			}
			else //if((nCurrent_PickerInfo[i] == CTL_NO) //CTL_YES: 피커 정보가 디바이스가 있다, CTL_NO:피커 정보가 디바이스가 없다
			{//디바이스가 없어야 한다 
				if(nBufferMode == 0) //전역 변수의 상태를 체크하는 동작시 사용 
				{
					if(nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) //james 2016.0918 추가 nIO_Output_Num_Info[i] 
					{	 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
			}
		}
		else if(nYesNo == CTL_NO)	//사용하지 말자,. 디바이스가 없는 상태를 체크할 때,  
		{
			if(nCurrent_BufferInfo[i] == CTL_YES) //CTL_YES: 피커 정보가 작업하는 정보이다, CTL_NO:피커 정보가 작업하지 않는 정보이다
			{ 
				if(nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) //james 2016.0919 (npPickerStatus[i] == IO_OFF) //james 2016.0918 CTL_NO)
				{	//error   //감지 안됨 에러 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
				}
			}
			else //if((nCurrent_BufferInfo[i] == CTL_NO) //CTL_YES: 피커 정보가 디바이스가 있다, CTL_NO:피커 정보가 디바이스가 없다
			{//디바이스가 없어야 한다 
				if(nBufferMode == 0)//전역 변수의 상태를 체크하는 동작시 사용 
				{
					if(nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) //james 2016.0919 추가 nIO_Output_Num_Info[i] 
					{	//error   //감지 안됨 에러 
						//vacuum on/off 후 바로 체크 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}					 
				}
			}
		}
	}  

	return nFuncRet;
}

void CPublic_Function::ThreadFunctionStepTrace(int nPos, int nStep)
{
	if (nStep == st_work.mn_prev_step[nPos]) // step이 동일하군~
	{
		return;
	}

// 	CString str_temp;
// 	str_temp.Format("[Pos:%02d] Prev : %d -> now %d",nPos,st_work.mn_prev_step[nPos],nStep);
// 	Func.On_LogFile_Add(3, str_temp);
	
	st_work.mn_prev_step[nPos] = nStep;
	StepChangeTime[nPos] = CTime::GetCurrentTime();	 
	st_work.mstr_last_change_date[nPos] = StepChangeTime[nPos].Format("%y/%m/%d %H:%M:%S");	
	
	if (st_work.mstr_history_step[nPos].GetLength() == 120 || st_work.mstr_history_step[nPos].GetLength() == 276)
	{
		str_step_temp[nPos].Format("%05d|", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];
		st_work.mstr_history_step[nPos] += "\r\n|                          |";
	}
	else if (st_work.mstr_history_step[nPos].GetLength() == 432)
	{
		str_step_temp[nPos].Format("%05d|", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];

		st_work.mstr_history_step_back_up[nPos] = st_work.mstr_history_step[nPos];
		st_work.mstr_history_step[nPos] = "";
	}
	else
	{
		str_step_temp[nPos].Format("%05d,", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];
	}
}

CString CPublic_Function::OnBarcode_Send(int n_command)
{
	CString str_ret = _T("FAIL");
	CString str_command = _T("FAIL");

	switch(n_command)
	{
		case BAR_READ_MODE :  // READ 모드
			str_command = "11";
			break;

		case BAR_PERCENT : // 반복 READ 모드
			/* ********************************************************************** */
			/* 무한 연속 READ 모드                                                    */
			/* -> 이 모드 설정 시 특정한 설정 옵션 미적용 되어진다                    */
			/* ********************************************************************** */
			str_command = "12";
			break;

		case BAR_SETTING_MODE :  // 설정 모드 전환
			str_command = "13";
			break;

		case BAR_SELF_TEST :  // SELF TEST
			str_command = "15";
			break;

		case BAR_ADJUST_MODE :  // SCAN 최적 감지 구간 확인
			str_command = "1J";
			break;

		case BAR_SHOW_LIMIT :  // SCAN 최적 미감지 구간 확인
			str_command = "1K";
			break;

		case BAR_READ_ON :  // SWITCH ON BAR_FLY_MODE
			str_command = "21";
			break;

		case BAR_READ_OFF :  // SWITCH OFF
			str_command = "22";
			break;

		case BAR_FULL_INFO :  // 세부 데이터 리턴 [다중 데이터]
			str_command = "3BT2";
			break;

		case BAR_MINI_INFO :  // 최소 데이터 리턴 [1개 데이터]
			str_command = "3BT3";
			break;

		case BAR_SET_DEFAULT :  // 디폴트 설정
			str_command = "3DF";
			break;

		case BAR_SET_SWITCH :  // READ 시기 제어
			/* ********************************************************************** */
			/* TRIGGER READING GATE를 TIME OUT으로 제어하는 모드                      */
			/* -> SWITCH ON/OFF로 READING 시기 제어한다                               */
			/*  : 기본적으로 TIME OUT READING GATE 주기는 1000ms로 지정된다           */
			/*  : 잘못된 TIME OUT 주기 설정 시 기본값이 설정되게 된다                 */
			/*  : str_command = "3LTM3T050E0C0Z0100A0B4BS49F0L1D0010X0001"            */
			/*  : M = 1 : SENSOR INPUT(ACTIVE HIGH)                                   */
			/*  :     2 : SENSOR INPUT(ACTIVE LOW)                                    */
			/*  :     3 : SERIAL INTERFACE                                            */
			/*  :     4 : FREE RUNNING WITH TIMEOUT                                   */
			/*  :     8 : REFLECTOR POLING                                            */
			/*  : C = 0 : TRIGGER OFF 또는 BARCODE 읽을때까지 READ 대기 모드          */
			/*  :     1 : TIME OUT 기간 동안 READ 대기 모드                           */
			/*  : Z = 0001 ~ 9999 : 1000ms = 1SEC  (TIME OUT 시간 설정, 0001 = 10ms)  */
			/* ********************************************************************** */
			// TIME OUT 없이 신호만으로 READING GATE 제어
			str_command = "3LTM3C0";
			break;

		case BAR_RS232 :  // RS-232C 인터페이스 모드로 변경
			/* ********************************************************************** */
			/* RS-232C 인터페이스 모드로 변경한다                                     */
			/*  : str_command = "3HSB6D1S1H11Z0E0"                                    */
			/*  : 3HS [호스트 인터페이스 설정 모드]                                   */
			/*  : B6  [BAUD RATE 설정 : 9600]                                         */
			/*  : D1  [DATA BITS + PARITY 설정 : 8bits]                               */
			/*  : S1  [STOP BIT 설정 : 1bit]                                          */
			/*  : H11 [통신 방식 설정 : 11 (RS232),  01 (RS422/485)]                  */
			/* ********************************************************************** */
			str_command = "3HSH11";
			break;

		case BAR_RS422 :  // 422/485 인터페이스 모드로 변경
			/* ********************************************************************** */
			/* 422/485 인터페이스 모드로 변경한다                                     */
			/*  : str_command = "3HSB6D1S1H11Z0E0"                                    */
			/* ********************************************************************** */
			str_command = "3HSH01";
			break;

		case BAR_VOLUME :  // BEEP 음량 설정
			/* ********************************************************************** */
			/* BEEP 음량을 설정한다                                                   */
			/* str_command = "3ROA11W0B00X0G00V0L00000000O00000000M00N0P00Q0T10R0S0"  */
			/*  : V  [0:OFF    1:LOW    2:HIGH]                                       */
			/*  : S1 [STOP BIT 설정 : 1bit]                                           */
			/* ********************************************************************** */
/*			if (n_option == 1)      // 음량 : LOW
				str_command = "3ROV1";
			else if(n_option == 2)  // 음량 : HIGH
				str_command = "3ROV2";
			else                    // 음량 : OFF
				str_command = "3ROV0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_1 :  // CODABAR 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COaA1LFP1M03C000D0"                                 */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COaA1";
			else                // INACTIVE
				str_command = "3COaA0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_2 :  // 39 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3CObA1LFP1M03C0000D0X0"                              */
			/* ********************************************************************** */
/*			if(n_option != 0)  // ACTIVE
				str_command = "3CObA1";
			else               // INACTIVE
				str_command = "3CObA0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_3 :  // UPC 모드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COcA0LE00 00 00 00 00P1M03C110D1"                   */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COcC110";
			else                // INACTIVE
				str_command = "3COcC000";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_4 :  // EAN 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COdA0LE00 00 00 00 00P1M03C110D1"                   */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COdC110";
			else                // INACTIVE
				str_command = "3COdC000";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_5 :  // 2/5-Interleaved 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COeA1LI04 50P1M03C10D0"                             */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COeA1";
			else                // INACTIVE
				str_command = "3COeA0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_6 :  // 39 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COiA1LE02 06 07 10 00P0M03C0D1"                     */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COiA1";
			else                // INACTIVE
				str_command = "3COiA0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_7 :  // 128 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COjA1LFP0M03C0D1"                                   */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COjA1";
			else                // INACTIVE
				str_command = "3COjA0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		case BAR_CODE_8 :  // EAN 128 코드 모드 설정
			/* ********************************************************************** */
			/*  : str_command = "3COnA1LE00 00 00 00 00P0M03C05D43311D0000D1"         */
			/* ********************************************************************** */
/*			if (n_option != 0)  // ACTIVE
				str_command = "3COnA1";
			else                // INACTIVE
				str_command = "3COnA0";

			if (n_port == BCR_PORT1 || n_port == BCR_PORT3)
			{
				st_bcr[0].n_bcr_code_mode = BAR_COMM_MODE;
			}
			else
			{
				st_bcr[1].n_bcr_code_mode = BAR_COMM_MODE;
			}*/
			/* ********************************************************************** */
			break;
		default :
			AfxMessageBox("Scanner Command Error");
			break;
	}

	if(str_command != "FAIL")
	{
		str_ret.Format("%c%s%c", 0x02, str_command, 0x03);			// 포멧 : [STX+명령어+ETX] 형태로 변환

		st_serial.str_snd[DEVICE_BARCODE_PORT-1] = str_ret;                    // 송신 데이터 설정
		::SendMessage(st_handler.hWnd, WM_DATA_SEND, DEVICE_BARCODE_PORT, 0);  // 데이터 송신 요청
	}
	else 
	{
		st_serial.comm_err[DEVICE_BARCODE_PORT-1] = _T("[송신 메시지] 포맷 생성 에러 발생");
	}

	return str_ret;
}

int CPublic_Function::OnBarcode_Receive(int n_picker, int n_command, CString str_rev)
{
	CString str_body;  /* BODY 데이터 저장 변수 */
	CString str_temp_data_ea;
	int     n_length;  /* 데이터 총 길이 저장 변수 */
	int     n_start_index, n_end_index;  /* BCR 데이터 시작 및 종료 위치 저장 변수 */
	int		first_carr_position = 0, final_carr_position = 0;
	int     n_receive_data_ea = 0, n_test_no = 0;
	int		n_pos[5];
//	char cp_cmd[BUF_SIZE];  // 검사할 데이터 임시 저장 변수

//	memset(cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화

	/* ************************************************************************** */
	/* 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                           */
	/* ************************************************************************** */
	n_length = str_rev.GetLength();
	if(n_length<3)
	{
		return BAR_ERR;
	}

	str_body = str_rev.Mid(1, n_length-2);  // 헤더+TAIL 제거 데이터
	str_body.TrimLeft(' ');
	str_body.TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 에러 데이터 수신 유무 검사한다                                             */
	/* ************************************************************************** */
	if(str_body.Find("ERR") >= 0)
	{
		st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
		st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

		return BAR_ERR;
	}
	/* ************************************************************************** */
	if(n_length >= 1000)
	{
		st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
		st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

		return BAR_ERR;
	}

	sprintf(cp_cmd, "%s", str_rev);

	/* ************************************************************************** */
	/* 수신 데이터 헤더 검사한다                                                  */
	/* ************************************************************************** */
	if(cp_cmd[0] != 0x02)
	{
		st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
		st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

		return BAR_ERR;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 수신 데이터 TAIL 검사한다                                                  */
	/* ************************************************************************** */
	if(cp_cmd[n_length-1] != 0x03)
	{
		st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
		st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

		return BAR_ERR;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 수신 데이터 정상 유무 검사한다                                             */
	/* ************************************************************************** */
	switch(n_command)
	{
		case BAR_READ_ON:
			n_pos[0] = n_pos[1] = n_pos[2] = n_pos[3] = n_pos[4] = -1;
			n_pos[0] = str_body.Find("21");

			if(n_pos[0] < 0)
			{
				st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
				st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

				return BAR_ERR;
			}
			else
			{
				return BAR_OK;
			}
			break;

		case BAR_READ_OFF:
			n_pos[0] = n_pos[1] = n_pos[2] = n_pos[3] = n_pos[4] = -1;
			n_pos[0] = str_body.Find("22");
			
			if(n_pos[0] < 0)
			{
				st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
				st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

				return BAR_ERR;
			}
			else
			{
				return BAR_OK;
			}
			break;

		case BAR_READ_DATA:
			n_pos[0] = n_pos[1] = n_pos[2] = n_pos[3] = n_pos[4] = -1;
			n_pos[0] = str_body.Find("no  code");
			n_pos[1] = str_body.Find("n=_0");

			if(n_pos[0] > 0 || n_pos[1] > 0)  // READ된 BCR 데이터 없음
			{
				st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
				st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

				return BAR_ERR;	
			}
			else
			{
				n_pos[3] = str_body.Find("n=_1");

				if(n_pos[3] > -1)
				{
					n_start_index = str_body.Find(10, 0)+1;				// BCR 시작 위치 설정 [LF]
					n_end_index = str_body.Find(10, n_start_index)-1;	// BCR 종료 위치 설정 [LF]

					return BAR_OK;
				}
				else
				{
					st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
					st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

					return BAR_ERR;
				}
			}
			break;
	}

	st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
	st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

	return BAR_ERR;
}








CString CPublic_Function::Get_MotorName(int n_axis)
{
	CString str_tmp;

	return str_tmp;
}

int CPublic_Function::OnIO_Safety(int n_io, int n_onoff)
{


	return CTLBD_RET_GOOD;
}

void CPublic_Function::MsgLog( LPCTSTR strMsg )
{
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, strMsg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
}

void CPublic_Function::DeleteOldFiles( LPCTSTR lpDirPath, int iDays )
{
	if ( lpDirPath == NULL )
		return;
	
	CString strNextDirPath = "";
	CFileFind find;
	BOOL bFind = find.FindFile( lpDirPath );
	
	if ( bFind == FALSE )
		return;
	
	while( bFind )
	{
		bFind = find.FindNextFile();
		
		// . or .. pass
		if ( find.IsDots() == TRUE )
			continue;
		
		if ( find.IsDirectory() )
		{
			CString strDir;
			strDir.Format("%s\\*.*", find.GetFilePath() );
			DeleteOldFiles( strDir, iDays );
		}
		else
		{
			HANDLE hF = CreateFile(find.GetFileName(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			
			SYSTEMTIME st;
			GetLocalTime(&st);
			
			CTime tNow(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
			CTime tTime;
			find.GetLastWriteTime(tTime);
			
			CTimeSpan span = tNow - tTime;
			//int iDays = span.GetDays();
			
			if ( span.GetDays() > iDays )
			{
				DeleteFile( find.GetFilePath() );
				
				CString strLog;
				strLog.Format("Delete File - Lost %d Days : %s", iDays, find.GetFilePath() );
				Func.On_LogFile_Add(3, strLog);
			}
		}
	}
	
	find.Close();
}

int  CPublic_Function::Check_PickerStatus(int nPickerMode, int nSite, int nYesNo, int *npPickerInfo, int *npPickerStatus, int *npPickerOutputStatus)
{
	int nFuncRet = RET_GOOD;
	int i, nRet[4]={0,};
	int nIO_Num_Info[MAX_PICKER];  //[0]:fwd [1]:bwd
	int nIO_Output_Num_Info[MAX_PICKER];
	int nCurrent_PickerInfo[MAX_PICKER];
//	int nIO_ExistSenChk[MAX_PICKER];
	int nPara_Num;

	int dDvcCheck_Mode = 1;
	if(nSite == THD_LOAD_WORK_RBT) 
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			if( st_basic.n_mode_device == WITHOUT_DVC )
				nIO_Num_Info[i] = st_io.i_Loader_Transfer_Clamp_On_Check;				
			else
				nIO_Num_Info[i] = st_io.i_Loader_Transfer_Clamp_On_Check;  

			nIO_Output_Num_Info[i] = st_io.o_Loader_Transfer_Clamp_On_Sol; 
			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용, CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if(nSite == THD_UNLOAD_WORK_RBT) 
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_Unloader_Transfer_Device_Check;
			nIO_Output_Num_Info[i] = st_io.o_Transfer1_Clamp_Forward_Sol; 
			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용, CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			}
		}
	}
	else if(nSite == THD_HEATSINK_RBT) 
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_Vacuum_Check;  
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Vacuum_On_Sol; 
			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용, CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if(nSite == THD_HEATSINK_PRBT) 
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_CellIn_Check;  
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Trasnfer_Pad_Up_Sol; //어쩔수 없이 -_-;;
			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용, CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if(nSite == THD_LD_HEATSINK_BUFF)//picker
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_Vacuum_Check;
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Vacuum_On_Sol;

			if(nPickerMode == 0)	 //최종적으로 체크할 전역 변수 확인, 전체 피커 정보와 센서 상태를 체크한다  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용,  CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}  
	}
	else if(nSite == THD_DISPENSOR_RBT)
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Reverse_Clamp_Off_Check;
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Reverse_Clamp_Off_Sol;
			
			if(nPickerMode == 0)	 //최종적으로 체크할 전역 변수 확인, 전체 피커 정보와 센서 상태를 체크한다  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용,  CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if(nSite == THD_DISPENSOR_PRBT)//Heatsink robot picks up reverse cylinder
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_CellIn_Check;
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Reverse_Clamp_Off_Sol;
			
			if(nPickerMode == 0)	 //최종적으로 체크할 전역 변수 확인, 전체 피커 정보와 센서 상태를 체크한다  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용,  CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if(nSite == THD_PICK_REVERSE_DVC)//Heatsink robot picks up reverse cylinder
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_CellIn_Check;
			nIO_Output_Num_Info[i] = st_io.i_HeatSink_Transfer_CellIn_Check;

			if(nPickerMode == 0)	 //최종적으로 체크할 전역 변수 확인, 전체 피커 정보와 센서 상태를 체크한다  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용,  CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if( nSite == THD_PLACE_HEATSINK_DVC)//picker
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_Dispenser_Device_Check;//B접점 //st_io.i_HeatSink_Transfer_Vacuum_Check;
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Vacuum_On_Sol;

			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용,  CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}
	}
	else if( nSite == THD_PLACE_CARRIER_DVC)
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_CellIn_Check;
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Vacuum_On_Sol;

			if(nPickerMode == 0)	 //최종적으로 체크할 전역 변수 확인, 전체 피커 정보와 센서 상태를 체크한다  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //함수 인자(멤버변수) 사용,  CTL_YES인 부분만 백큠 센서상태 체크한다 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}  
	}
	else 
	{//이곳으로 오면 안됨 
		return RET_ERROR;
	}
	
	for(i = 0; i < nPara_Num; i++)
	{
		npPickerStatus[i]       = g_ioMgr.get_in_bit( nIO_Num_Info[i], IO_ON);
		npPickerOutputStatus[i] = g_ioMgr.get_out_bit(nIO_Output_Num_Info[i], IO_ON);
	}

	if(st_basic.n_mode_device == WITHOUT_DVC) //with device 가 아니면 
	{
		for(i = 0; i < nPara_Num; i++)
		{
			//npPickerStatus[i] = CTL_NO;
			if(nYesNo == CTL_YES)
			{			
				if(nSite == THD_LOAD_WORK_RBT)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF;
						}
					}
				}
				else if(nSite == THD_UNLOAD_WORK_RBT) 
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)//Backward
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_HEATSINK_RBT) 
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)//Backward
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_LD_HEATSINK_BUFF)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_PICK_HEATSINK_DVC)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_PLACE_HEATSINK_DVC)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_PLACE_CARRIER_DVC)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_HEATSINK_PRBT) 
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)//Backward
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_DISPENSOR_RBT)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_DISPENSOR_PRBT)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}

			}
			else //디바이스가 없는 상태를 체크시 
			{
				if(nSite == THD_LOAD_WORK_RBT)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)//forward
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //clamp 하고 있는 상태로 문제가 있어 에러처리 
						}
					}
				}
				else if(nSite == THD_UNLOAD_WORK_RBT)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)//forward
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //clamp 하고 있는 상태로 문제가 있어 에러처리 
						}
					}
				}
				else if(nSite == THD_LD_HEATSINK_BUFF)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_PICK_HEATSINK_DVC)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_PLACE_HEATSINK_DVC)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{					
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //Vacuum이 OFF이므로  without 이므로 항상 B접점이다 항상 ON
						}
					}
				}
				else if(nSite == THD_PLACE_CARRIER_DVC)
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_ON)
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
			}
		}
		return RET_GOOD;
	}

	for (i = 0; i < nPara_Num; i++) 
	{
		if(nYesNo == CTL_YES)	//디바이스가 있는 상태를 체크할 때 
		{
			if(nCurrent_PickerInfo[i] == CTL_YES) //CTL_YES: 피커 정보가 디바이스가 있다, 피커가 forwrad 상태
			{			
				if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
				{	//error   //감지 안됨 에러 	//실린더가 OFF(backward) 					
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
				}
			}
			else //if((nCurrent_PickerInfo[i] == CTL_NO) //CTL_YES: 피커 정보가 디바이스가 있다, CTL_NO:피커 정보가 디바이스가 없다
			{//디바이스가 없어야 한다 
				if(nPickerMode == 0) //전역 변수의 상태를 체크하는 동작시 사용 
				{
					if(nSite == THD_LOAD_WORK_RBT )
					{
						if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_HEATSINK_RBT )
					{
						if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_HEATSINK_PRBT )
					{//if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)
						if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)//2017.0420
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_UNLOAD_WORK_RBT)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_LD_HEATSINK_BUFF)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PICK_HEATSINK_DVC)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if( nSite == THD_PLACE_HEATSINK_DVC)
					{
						if(npPickerStatus[i] == IO_ON /*B접점*/|| npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지않됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if( nSite == THD_PLACE_CARRIER_DVC)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //감지됨 에러 	//실린더가 ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
				}
			}
		}
		else if(nYesNo == CTL_NO)	// 디바이스가 없는 상태를 체크할 때,  
		{
			if(nCurrent_PickerInfo[i] == CTL_YES) //CTL_YES: 피커 정보가 작업하는 정보이다, CTL_NO:피커 정보가 작업하지 않는 정보이다
			{
				if(nSite == THD_LOAD_WORK_RBT)
				{ 
					if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_HEATSINK_RBT)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_HEATSINK_PRBT)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_UNLOAD_WORK_RBT)
				{
					if(npPickerStatus[i] == IO_ON)
					{	//error   //감지 됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_LD_HEATSINK_BUFF)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_PICK_HEATSINK_DVC)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if( nSite == THD_PLACE_HEATSINK_DVC)
				{
					if(npPickerStatus[i] == IO_ON)//B접점
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if( nSite == THD_PLACE_CARRIER_DVC)
				{
					if(npPickerStatus[i] == IO_ON)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}	
			}
			else //if((nCurrent_PickerInfo[i] == CTL_NO) //CTL_YES: 피커 정보가 디바이스가 있다, CTL_NO:피커 정보가 디바이스가 없다
			{//디바이스가 없어야 한다 
				if(nPickerMode == 0)//전역 변수의 상태를 체크하는 동작시 사용 
				{
					if(nSite == THD_LOAD_WORK_RBT)
					{ 
						if(npPickerStatus[i] == IO_OFF && npPickerOutputStatus[i] == IO_OFF)
						{	//error   //감지 안됨 에러 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_UNLOAD_WORK_RBT)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //감지 안됨 에러 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_LD_HEATSINK_BUFF)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //감지 안됨 에러 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PICK_HEATSINK_DVC)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //감지 안됨 에러 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PLACE_HEATSINK_DVC)
					{//npPickerStatus[i] 디바이스를 Dispensor buffer에 놓았다 B접점
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //감지 안됨 에러 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PLACE_CARRIER_DVC)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //감지 안됨 에러 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
				}
			}
		}
	}

	return nFuncRet;
}

/////////////////////////////////////////////////////////////
// tray & Buffer 의 작업 가능한 공간의 정보를 찾는다 
//////////////////////////////////////////////////////////////2016.0814 
int CPublic_Function::Find_TrayWork_Pos(int nMode, int nSite, int nDVCYesNO, int *npFindPosYXCPB, int nRobotSite, CString str_LotNo) //first tray Y, first tray X, first picker num, first buff num, 작업 조건에 충족하는 빈 공간 또는 자재 집을 전체 수량
{

	int m, x, y, i, nFuncRet, nFlag;
	int nFirstY =0, nFirstX=0;
	int nPassCount = 0;
	bool bFlag = false;
	int nCount[2]={0,0};

	nFuncRet = RET_SKIP;

	npFindPosYXCPB[0] = 0;npFindPosYXCPB[1] = 0;npFindPosYXCPB[2] = 0;npFindPosYXCPB[3] = 0;npFindPosYXCPB[4] = 0; //clear
	nFlag = CTL_NO;

	switch(nSite)
	{
		case THD_LD_TRAY_PLATE: //load olate tray 자재의 정보
			nFirstY = 0; //st_recipe_info.nTrayY;
			nFirstX = 0;	
			for(y = nFirstY; y < st_recipe.nTrayY; y++) //Y 방향 정보
			{
				for(x = nFirstX; x < st_recipe.nTrayX; x++) //X 방향 정보 
				{ 
					if(st_tray_info[nSite].st_pcb_info[y][x].nYesNo == nDVCYesNO)// && (st_tray_info[nSite].st_pcb_info[y][x].strLotNo == str_LotNo || str_LotNo == "0") )
					{
						if(nFlag == CTL_NO)
						{
							npFindPosYXCPB[0] = x; 
							npFindPosYXCPB[1] = y;
							nFuncRet = RET_GOOD;
							nFlag = CTL_YES;
						}
						npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
					}
				}
			}
			break;

		case THD_LD_ALIGN_BUFF: //로딩 버퍼는 test robot이 작업하기로 선정한  test site의 소켓 정보에 따라 버퍼 각 위치의 사용 여부가 결정된다 
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if(st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )// && st_buffer_info[nSite].st_pcb_info[i].nEnable == CTL_YES
					//&& (st_buffer_info[nSite].st_pcb_info[i].strLotNo == str_LotNo || str_LotNo == "0") ) //nDVCYesNO == CTL_NO 조건에 충족한 정보만 체크, 사용가능한 소켓에만 작업가능
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

		case THD_ULD_ALIGN_BUFF:
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if(st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO)
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

		case THD_LDULD_CARRIER_BUFF:

			for ( i = 0 ; i < st_recipe.nCarrierBuffer_Num; i++ )//TOP1 MID 2 BTM 3
			{
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_skip_flag[i] != CTL_YES)
				{
					//if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[i] != CTL_YES )//나중에 바꿔야한다
					//{
						if( nDVCYesNO == CTL_NO )//디바이스 Place
						{
							if( nRobotSite == THD_LOAD_WORK_RBT && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == nDVCYesNO )
							{
								if(nFlag == CTL_NO)
								{
									npFindPosYXCPB[0] = 0; npFindPosYXCPB[1] = i;//Y축의 위치
									nFuncRet = RET_GOOD;
									nFlag = CTL_YES;
								}
								npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 	
							}
						}
						else//디바이스 Pick //if( nSite == THD_UNLOAD_WORK_RBT && nDVCYesNO == CTL_YES )
						{
							if( nRobotSite == THD_UNLOAD_WORK_RBT && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == nDVCYesNO && /*st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id*/
								st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] != BIN_CDIMM )
							{
								if(nFlag == CTL_NO)
								{
									npFindPosYXCPB[0] = 0; npFindPosYXCPB[1] = i;
									nFuncRet = RET_GOOD;
									nFlag = CTL_YES;
								}
								npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 	
							}
						}
					//}
				}
			}
			break;

		case  THD_LD_HEATSINK_BUFF:
			nFirstY = 0; //st_recipe_info.nTrayY;
			nFirstX = 0;	
			st_recipe.nTrayNum = 2;

			for(m = 0; m<st_recipe.nTrayNum; m++)
			{
				for(y = nFirstY; y < st_recipe.nHsTrayY; y++) //Y 방향 정보
				{
					for(x = nFirstX; x < st_recipe.nHsTrayX; x++) //X 방향 정보 
					{ 
						if(st_tray_info[nSite].st_dvc_info[m][y][x].nYesNo == nDVCYesNO )
						{
							if(nFlag == CTL_NO)
							{
								npFindPosYXCPB[0] = x; 
								npFindPosYXCPB[1] = y;
								nFuncRet = RET_GOOD;
								nFlag = CTL_YES;
								npFindPosYXCPB[4] = m;//Tray number
							}
							npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
						}
					}
				}
			}
			break;

		case THD_PLACE_HEATSINK_DVC:
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if( st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )//&& st_buffer_info[nSite].st_pcb_info[i].nEnable == CTL_YES && 
				   //( st_buffer_info[nSite].st_pcb_info[i].strLotNo == str_LotNo ) )
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

		case THD_DISPENSOR_PRBT:
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if( st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

		case THD_DISPENSOR_RBT:
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if( st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

		case THD_PICK_REVERSE_DVC:
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if( st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

		case THD_PICK_HEATSINK_DVC://REVERSE pickup dvc
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if( st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;


		case THD_PLACE_CARRIER_DVC://carrier dev place
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < 3/*st_recipe.nLdBuffer_Num*/; i++)
			{
				if( st_basic.n_mode_jig == WITHOUT_JIG ) st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[i] = nDVCYesNO;
				if( st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[i] == CTL_YES && 
					st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[i] == BIN_EPOXY)// &&	st_carrier_buff_info[nSite].n_skip_flag[i] == CTL_YES )
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //작업 가능한 count 수량 체크 			
				}
			}
			break;

	}

	return nFuncRet;
}

int CPublic_Function::Check_Robot_Buffer_Clash( int nRbtTHDSite, int BuffTHDSite, int nBuffStartPos, int nPickerStartPos, int *nErrorInfoStatus) //피커와 버퍼의 작업 위치간 공간의 작업 여부 가능 확인 한다
{//int nBuffStartPos 의 의미는 Buffer, Test site socket의 피커기준 버퍼등의 시작 번호이다 
	int nFuncRet = RET_GOOD;
	int i, nRet[4]={0,};
	int nPara_Num = st_recipe.nCarrierBuffer_Num;
	int nPos=0;
	int nFisrtPicker=0;

	for(i = 0; i < nPara_Num; i++) nErrorInfoStatus[i] = CTL_NO; //clear 
	 			
	if(nRbtTHDSite == THD_LOAD_WORK_RBT)
	{			
		for(i = 0; i < nPara_Num; i++) //nBuffStartPos을 기준으로 위치번호 큰쪽에 자재가 남아있으면 피커 헤드와 간섭이니 체크
		{
			nErrorInfoStatus[i] = CTL_NO; //에러 정보 클리어 				

			if(BuffTHDSite == THD_LD_ALIGN_BUFF) //자재를 only 놓는 작업을 한다, 작업 순서는 0->1->2->3 으로 뒤에서 부터 놓는다
			{					
				if(nBuffStartPos > i) continue;
				if(st_buffer_info[BuffTHDSite].st_pcb_info[i].nYesNo == CTL_YES)  
				{						  
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON,st_io.i_Loading_Tr_Jig_Detect_Check);//error 
					//에러 , 버퍼의 디바이스와 피커의 자재가 충돌 함
					nErrorInfoStatus[i] = CTL_YES; //에러 정보 셋팅
					nFuncRet = RET_ERROR; 
				} 
			}
			else if(BuffTHDSite == THD_LDULD_CARRIER_BUFF)
			{
				if(nBuffStartPos > i) continue; //버퍼 번호 순서로 작업할 영역보다 큰 위치 순서에 자재 있으면 충돌하다 // 방지해야 함 
				if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == CTL_YES)  
				{	//에러 , 버퍼의 디바이스와 피커의 자재가 충돌 함
					nErrorInfoStatus[i] = CTL_YES; //에러 정보 셋팅
					m_strAlarmCode.Format(_T("8%d500%d"), IO_ON,st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i]);//error 
					nFuncRet = RET_ERROR; 
				} 
			}
		}			
	}//체크 필요
	else if(nRbtTHDSite == THD_UNLOAD_WORK_RBT) //로보트 피커 방향과 , 버퍼/테스트 사이트의 번호 순서가 동일하다 
	{
		nFisrtPicker=0;
		for(i = 0; i < nPara_Num; i++)
		{
			if(BuffTHDSite == THD_ULD_ALIGN_BUFF)
			{
				if(nBuffStartPos > i) continue; //버퍼 번호 순서로 작업할 영역보다 큰 위치 순서에 자재 있으면 충돌하다 // 방지해야 함 
				if(st_buffer_info[BuffTHDSite].st_pcb_info[i].nYesNo == CTL_NO)  
				{	//815100 0 A "UNLOAD_BUFFER_DVC_EXIST_CHECK_ERROR."
					nErrorInfoStatus[i] = CTL_YES; //에러 정보 셋팅
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON,st_io.i_Unloader_Transfer_Device_Check);//error 
					nFuncRet = RET_ERROR; 
				} 
			}
		}//for
	}
	return nFuncRet;
}
////////////////////////////////////////////////////////////
// 로보트 Pos를 찾는다 
////////////////////////////////////////////////////////////
int CPublic_Function::Calculate_MovePos_Find(int nMode, int nMotNum, int nSite, int nWorkPart, int *npFindWorkPosYXCPB, double *dpGetTargetPos)
{
	//n_workpos[2]:[0]:x, [1]:y , n_refnum = 작업 시작할 피커 번호, 작업위치 지정 번호   
	double dBasicPos=0, dVarMveGapVal=0, dCompleteMveVal=0; 
	int nX=0, nY=0, workpoint=0, nID = 0;
// 	char Jcode[10];

	switch (nMotNum)
	{

	case M_LOADER_TRANSFER_Y:
		if(nSite == THD_LD_TRAY_PLATE)
		{
			dBasicPos = st_motor[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PICK_POS];
			dVarMveGapVal = (st_recipe.dTrayPitch_Y * npFindWorkPosYXCPB[1]);
			dCompleteMveVal =  (dBasicPos + dVarMveGapVal);		
		}
		else if(nSite == THD_LD_ALIGN_BUFF)//PLACE
		{
			//PLACE
			if( nWorkPart == WORK_PLACE) dBasicPos = st_motor[nMotNum].md_pos[p_LOADER_TRANSFER_Y_ALIGN_PLACE_POS];
			//PICK
			else dBasicPos = st_motor[nMotNum].md_pos[P_LOADER_TRANSFER_Y_ALIGN_PICK_POS];
			dVarMveGapVal = (st_recipe.dTrayPitch_Y * npFindWorkPosYXCPB[0]);
			dCompleteMveVal =  (dBasicPos + dVarMveGapVal);		
		}
		else if(nSite == THD_LDULD_CARRIER_BUFF)
		{
			if( nWorkPart == 0 )
				dBasicPos = st_motor[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PLACE_TOP_POS];
			else if( nWorkPart == 1 )
				dBasicPos = st_motor[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PLACE_MID_POS];
			else
				dBasicPos = st_motor[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PLACE_BOT_POS];
			dVarMveGapVal = 0.0;// npFindWorkPosYXCPB[0];//(st_recipe.dTrayPitch_Y * npFindWorkPosYXCPB[0]);
			dCompleteMveVal =  (dBasicPos + dVarMveGapVal);		
		}
		break;

	case M_UNLOADER_TRANSFER_Y:
		if(nSite == THD_LDULD_CARRIER_BUFF)
		{
			if( nWorkPart == WORK_PICK )
			{
// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS];
// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_MID_POS];
// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_BOT_POS];
				dBasicPos = st_motor[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS + npFindWorkPosYXCPB[1]];
				dCompleteMveVal =  dBasicPos;
			}
		}
		else if(nSite == THD_ULD_ALIGN_BUFF)
		{	
			dBasicPos = st_motor[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS];
			dCompleteMveVal =  dBasicPos;
		}		
		break;

	case M_UNLOADER_TRANSFER_X:

		if(nSite == THD_LDULD_CARRIER_BUFF)
		{
			if( nWorkPart == WORK_PICK )
			{
				// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS];
				// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_MID_POS];
				// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_BOT_POS];
				dBasicPos = st_motor[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS + npFindWorkPosYXCPB[1]];
				dCompleteMveVal =  dBasicPos;
			}
		}
		else if(nSite == THD_ULD_ALIGN_BUFF)
		{	
			dBasicPos = st_motor[nMotNum].md_pos[P_UNLOADER_TRANSFER_X_ZIGPLACE_POS];
			dCompleteMveVal =  dBasicPos;
		}
		break;

	case M_HEATSINK_TRANSFER_Z:
		if( nSite == THD_PLACE_CARRIER_DVC)
		{
			if( nWorkPart == TOP )
			{

			}
			else if( nWorkPart == MIDDLE )
			{

			}
			else// if( nWorkPart == BTM )
			{

			}
		}
		break;

	case M_HEATSINK_TRANSFER_X:
		if( nSite == THD_LD_HEATSINK_BUFF)
		{
			SaveHeatSinkBoxPos();
			if( npFindWorkPosYXCPB[4] == 0 )//tray1
			{
				dBasicPos = st_work.d_Heatsink1Xpos[npFindWorkPosYXCPB[0]+(st_recipe.nHsTrayX*npFindWorkPosYXCPB[1])];
			}
			else//tray2
			{
				dBasicPos = st_work.d_Heatsink2Xpos[npFindWorkPosYXCPB[0]+(st_recipe.nHsTrayX*npFindWorkPosYXCPB[1])];
			}
			dCompleteMveVal =  dBasicPos;
		}
		else if( nSite == THD_PLACE_HEATSINK_DVC)
		{
			dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRASNFER_X_TURN_PLACE_POS];
			dCompleteMveVal =  dBasicPos;
		}
		else if( nSite == THD_DISPENSOR_PRBT)
		{
			dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRASNFER_X_TURN_PICK_POS];
			dCompleteMveVal =  dBasicPos;
		}
		else if( nSite == THD_PLACE_CARRIER_DVC)
		{
			if( nWorkPart == TOP )
			{//COMM nuber get teachPos
				dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_X_PLACE_TOPPOS];
				nID = atoi(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[TOP]);
				nID = nID -1;
				if( nID >= 0)
					dBasicPos = st_BoatTeaching[nID].m_dPos_x[TOP];
				else
					dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_X_PLACE_TOPPOS];

				dCompleteMveVal =  dBasicPos;
			}
			else if( nWorkPart == MIDDLE )
			{
				dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_X_PLACE_MIDPOS];
				nID = atoi(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[MIDDLE]);
				nID = nID -1;
				if( nID >= 0)
					dBasicPos = st_BoatTeaching[nID].m_dPos_x[MIDDLE];
				else
					dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_X_PLACE_MIDPOS];

				dCompleteMveVal =  dBasicPos;
			}
			else// if( nWorkPart == BTM )
			{
				dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_X_PLACE_BOTPOS];
				nID = atoi(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[BTM]);
				nID = nID -1;
				if( nID >= 0)
					dBasicPos = st_BoatTeaching[nID].m_dPos_x[BTM];
				else
					dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_X_PLACE_BOTPOS];

				dCompleteMveVal =  dBasicPos;
			}
		}
		break;

	case M_HEATSINK_TRANSFER_Y:
		if( nSite == THD_LD_HEATSINK_BUFF)
		{
			SaveHeatSinkBoxPos();
			if( npFindWorkPosYXCPB[4] == 0 )//tray1
			{
				dBasicPos = st_work.d_Heatsink1Ypos[npFindWorkPosYXCPB[0]+(st_recipe.nHsTrayX*npFindWorkPosYXCPB[1])];
			}
			else//tray2
			{
				dBasicPos = st_work.d_Heatsink2Ypos[npFindWorkPosYXCPB[0]+(st_recipe.nHsTrayX*npFindWorkPosYXCPB[1])];
			}
			dCompleteMveVal =  dBasicPos;
		}
		else if( nSite == THD_PLACE_HEATSINK_DVC)
		{
			dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRASNFER_Y_TURN_PLACE_POS];
			dCompleteMveVal =  dBasicPos;
		}
		else if( nSite == THD_DISPENSOR_PRBT)
		{
			dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRASNFER_Y_TURN_PICK_POS];
			dCompleteMveVal =  dBasicPos;
		}
		else if( nSite == THD_PLACE_CARRIER_DVC)
		{
			if( nWorkPart == TOP )
			{
				dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_TOPPOS];
				nID = atoi(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[TOP]);
				nID = nID -1;
				if( nID >= 0)
					dBasicPos = st_BoatTeaching[nID].m_dPos_y[TOP];
				else
					dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_TOPPOS];

				dCompleteMveVal =  dBasicPos;
			}
			else if( nWorkPart == MIDDLE )
			{
				dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_MIDPOS];
				nID = atoi(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[MIDDLE]);
				nID = nID -1;
				if( nID >= 0)
					dBasicPos = st_BoatTeaching[nID].m_dPos_y[MIDDLE];
				else
					dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_MIDPOS];

				dCompleteMveVal =  dBasicPos;
			}
			else// if( nWorkPart == BTM )
			{
				dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_BOTPOS];
				nID = atoi(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[BTM]);
				nID = nID -1;
				if( nID >= 0)
					dBasicPos = st_BoatTeaching[nID].m_dPos_y[BTM];
				else
					dBasicPos = st_motor[nMotNum].md_pos[P_HEATSINK_TRANSFER_Y_PLACE_BOTPOS];

				dCompleteMveVal =  dBasicPos;
			}
		}
		break;
	}

	//limit 값 셋팅 에러 
	if(dCompleteMveVal < st_motor[nMotNum].d_limit_position[0]) //- LIMIT 
	{//000004
		//000010 0 A "MOTOR SOFTWARE MINUS LIMIT CHECK ERROR -[M_ROBOT_X]."
		//000011 0 A "MOTOR SOFTWARE PULS LIMIT CHECK ERROR -[M_ROBOT_X]."
		m_strAlarmCode.Format(_T("%02d0010"), nMotNum);
		return RET_ERROR;
	}	

	if(dCompleteMveVal > st_motor[nMotNum].d_limit_position[1]) //+ LIMIT 
	{
		m_strAlarmCode.Format(_T("%02d0011"), nMotNum);
		return RET_ERROR;
	} 

	*dpGetTargetPos = dCompleteMveVal;  

	return RET_GOOD;
}


int CPublic_Function::Data_Exchange_PickPlace(int nPickPlace, int nJobCnt, int nRobotSite, int nWorkSite, int *npWorkPos)//Pick & Place시 picker & Tray data exchange 작업
{
	//int nJobCnt =한번에 처리할 data 작업수량, int nRobotSite//로보트사이트정보, int nWorkSite//트레이사이트정보, int *npRobotPos//picker정보, int *npWorkPos//트레이 정보
	//nPickPlace = 0:pick mode, 1:place mode
	CString strTemp;
	if(nPickPlace == PICKER_PICK_MODE) //트레이서 자재를 집는 동작 //TRAY -> robot pick;
	{
		if(nRobotSite == THD_LOAD_WORK_RBT)
		{
			if(nWorkSite == THD_LD_TRAY_PLATE) //트레이에서 자재 집는 동작
			{//피커는 정보를 가지고 가고, 트레이는 정보가 클리어 된다 		

			}			
			else if(nWorkSite == THD_LD_ALIGN_BUFF) //트레이에서 자재 집는 동작
			{//피커는 정보를 가지고 가고, 트레이는 정보가 클리어 된다 

			}

			if(nWorkSite == THD_LD_TRAY_PLATE)
			{
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBinNum;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo		= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strCSerialNo;
				
				//2016.1031
				if( st_handler.bLoaderOnFlag == CTL_YES )
				{
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo = CTL_NO;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = -1;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt = 0;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest = 0;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//현재빈만 사용 
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestSite[st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetestCnt] = -1;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum  = -1;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode = -1;
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN = _T("");
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo = _T("");
				}
				
				//////////////////////
				//data clear
				/////////////////////////
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].nEnable = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nEnable;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nYesNo = CTL_NO;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBin = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nRetestCnt = 0;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nRetest = 0;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				//st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0].nRetestSite[st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].nRetestCnt] = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBdNum  = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nScrCode = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strLotNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strSerialNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strBarcode = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strPartNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strArrNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strPPID = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strPSID = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strWWN = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].strCSerialNo = _T("");
				//test site만 사용 st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].tStart;	
				// jtkim 20160903
// 				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1][npWorkPos[1]].strBarcode1D[0] = _T("");
// 				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1][npWorkPos[1]].strBarcode1D[1] = _T("");
// 				// jtkim 20160929
// 				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1][npWorkPos[1]].strBarcode2D[0]	= _T("");
// 				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1][npWorkPos[1]].strBarcode2D[1]	= _T("");
			}
			else if(nWorkSite ==THD_LD_ALIGN_BUFF )//PICK
			{
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest			    = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo;
				//////////////////////
				//data clear
				/////////////////////////
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo = CTL_NO;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum  = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo = _T("");
			}
		}  //work robot 작업완료
		else if(nRobotSite == THD_UNLOAD_WORK_RBT) //PICKER_PICK_MODE
		{
			if(nWorkSite ==THD_LDULD_CARRIER_BUFF )
			{
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nRetestCnt[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest			    = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nRetest[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBdNum[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nScrCode[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chCSerialNo[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_part_num[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chArrNo[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chPPID[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chPSID[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chWWN[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[npWorkPos[1]]);
				//////////////////////
				//data clear
				/////////////////////////
//				memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE]));
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[npWorkPos[1]] = CTL_NO;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[npWorkPos[1]] = -1;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nRetestCnt[npWorkPos[1]] = 0;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nRetest[npWorkPos[1]] = 0;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[npWorkPos[1]] = -1;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBdNum[npWorkPos[1]] = -1;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nScrCode[npWorkPos[1]] = -1;
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chCSerialNo[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_part_num[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chArrNo[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chPPID[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chPSID[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chWWN[npWorkPos[1]]);
// 				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[npWorkPos[1]]);
			}
		}
		else if(nRobotSite == THD_HEATSINK_PRBT)
		{
			if(nWorkSite == THD_DISPENSOR_PRBT )
			{
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt    = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest       = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum        = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode      = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo      = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo   = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode    = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo     = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo      = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID       = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID       = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[1]].strCSerialNo;
				
				//////////////////////
				//data clear
				/////////////////////////
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo = CTL_NO;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt = 0; //kwlee 2016.1104 test
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum  = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo = _T("");	
			}
		}
		else if(nRobotSite == THD_HEATSINK_RBT) //PICKER_PICK_MODE
		{
			///////////////////////////////	 
			//data exchange
			//////////////////////////////
			if(nWorkSite == THD_LD_HEATSINK_BUFF)
			{
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBinNum;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo			= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo		= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strCSerialNo;

				//////////////////////
				//data clear
				/////////////////////////
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nYesNo = CTL_NO;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBin = -1;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nRetestCnt = 0;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nRetest = 0;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nRetestCnt = -1;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBdNum  = -1;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nScrCode = -1;
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strLotNo = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strSerialNo = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strBarcode = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strPartNo = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strArrNo = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strPPID = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strPSID = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strWWN = _T("");
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].strCSerialNo = _T("");
			}
			else if(nWorkSite == THD_PICK_HEATSINK_DVC)
			{		
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt    = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest       = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum        = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode      = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo      = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo   = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode    = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo     = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo      = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID       = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID       = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[1]].strCSerialNo;
				 
				//////////////////////
				//data clear
				/////////////////////////
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo = CTL_NO;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt = 0; //kwlee 2016.1104 test
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum  = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo = _T("");				 

			}

		}
		else if( nRobotSite == THD_DISPENSOR_PRBT )//reverse picks placeddvc in heatsink pocket
		{
			if( nWorkSite == THD_PLACE_HEATSINK_DVC)
			{
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nBin			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo	= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN;
				st_buffer_info[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo	= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo;
				
				//////////////////////
				//data clear
				/////////////////////////
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo = CTL_NO;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt = 0; //kwlee 2016.1104 test
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum  = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN = _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo = _T("");			
			}

		}
	}
	else if(nPickPlace == PICKER_PLACE_MODE) //자재를 놓는 동작시 사요 , 피커에서 트레이에 놓는다  //robot pick -> TRAY 
	{
		if(nRobotSite == THD_LOAD_WORK_RBT)
		{
			if(nWorkSite == THD_LD_ALIGN_BUFF) //로딩 버퍼에 자재를 놓는다 
			{				 	
			}
			else if(nWorkSite == THD_LDULD_CARRIER_BUFF)//자재를 놓는다 
			{				 	
			}
			///////////////////////////////	 
			//data exchange
			//////////////////////////////
			if(nWorkSite == THD_LD_ALIGN_BUFF) // PLACE
			{
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo	= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo;

				//////////////////////
				//data clear
				/////////////////////////
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo = CTL_NO;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum  = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo = _T("");
			}
			else if(nWorkSite == THD_LDULD_CARRIER_BUFF)
			{
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[npWorkPos[1]]				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[npWorkPos[1]]	= BIN_CDIMM;//			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nRetestCnt[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nRetest[npWorkPos[1]]				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[npWorkPos[1]]				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBdNum[npWorkPos[1]]				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nScrCode[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;

				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_lot_id[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chBarcode[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chSerialNo[nRobotSite], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_part_num[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chArrNo[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chPPID[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chPSID[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chWWN[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].c_chCSerialNo[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo );
				//////////////////////
				//data clear
				/////////////////////////
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo = CTL_NO;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum  = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo = _T("");
			}
		}
		else if(nRobotSite == THD_UNLOAD_WORK_RBT)
		{
			if(nWorkSite == THD_ULD_ALIGN_BUFF )
			{//피커는 정보가 클리어되고, 트레이는 정보를 가진다  	
			}
			///////////////////////////////	 
			//data exchange
			//////////////////////////////
			if(nWorkSite == THD_ULD_ALIGN_BUFF)
			{
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strCSerialNo	= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo;

				//////////////////////
				//data clear
				/////////////////////////
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo = CTL_NO;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum  = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo = _T("");
			}
		}
		else if(nRobotSite == THD_HEATSINK_PRBT)
		{
			if( nWorkSite == THD_PLACE_CARRIER_DVC)
			{		
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].n_exist[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBin[npWorkPos[1]]				= BIN_HEATSINK;// //			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
// 				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nRetestCnt[npWorkPos[1]]		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
// 				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nRetest[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
// 				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
// 				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBdNum[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
// 				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nScrCode[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;
				
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_lot_id[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chSerialNo[nRobotSite], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_part_num[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chArrNo[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chPPID[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chPSID[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chWWN[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN );
// 				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chCSerialNo[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo );
				//////////////////////
				//data clear
				/////////////////////////
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo = CTL_NO;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum  = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo = _T("");				 
				
			}
		}
		else if(nRobotSite == THD_HEATSINK_RBT)
		{
			if( nWorkSite == THD_PLACE_HEATSINK_DVC)
			{
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPPID			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPSID			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strWWN			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[1]].strCSerialNo	= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo;
				
				//////////////////////
				//data clear
				/////////////////////////
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo = CTL_NO;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest = 0;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//현재빈만 사용 
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum  = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode = -1;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN = _T("");
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo = _T("");	
			}
		}
	}
	
	return RET_GOOD;
}


////////////////////////////////////////////////////////////////////////
// 모든 사이트의 트레이 및 자재 정보를 쉬프트 관리한다  
////////////////////////////////////////////////////////////////////////
int CPublic_Function::Handler_Tray_DataInfo_Shift(int nMode, int nDvc_Yes_No, int nSend_SIte, int nRcv_Site)
{
	int x=0, y=0;
	 
	//nMode == 0 : 데이터 온전하게 쉬프만 진행 , send data도 지우지 않는다. 
	//nModw == 1 : 데이터 쉬프트 후 send data는 모두 클리어
	//nMode == 2 : 현재 작업하는 lot의 정보를 넣어준다. 디바이스 정보는 nDvc_Yes_No 변수의 셋팅 값으로 한다 
	//nMode == 3 :  nSend_SIte data 만 모두 클리어

	if(nMode == 0) // nMode == 0 : 데이터 온전하게 쉬프만 진행, send data도 지우지 않는다. 
	{		
		for(y = 0; y < st_recipe.nTrayY; y++)
		{
			for(x=0; x < st_recipe.nTrayX; x++)
			{
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strLotNo = st_tray_info[nSend_SIte].st_pcb_info[y][x].strLotNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strSerialNo = st_tray_info[nSend_SIte].st_pcb_info[y][x].strSerialNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strCSerialNo = st_tray_info[nSend_SIte].st_pcb_info[y][x].strCSerialNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strBarcode = st_tray_info[nSend_SIte].st_pcb_info[y][x].strBarcode;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPartNo = st_tray_info[nSend_SIte].st_pcb_info[y][x].strPartNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strArrNo = st_tray_info[nSend_SIte].st_pcb_info[y][x].strArrNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPPID = st_tray_info[nSend_SIte].st_pcb_info[y][x].strPPID;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPSID = st_tray_info[nSend_SIte].st_pcb_info[y][x].strPSID;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strWWN = st_tray_info[nSend_SIte].st_pcb_info[y][x].strWWN; 

				st_tray_info[nRcv_Site].st_pcb_info[y][x].nYesNo = st_tray_info[nSend_SIte].st_pcb_info[y][x].nYesNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nBin = st_tray_info[nSend_SIte].st_pcb_info[y][x].nBin;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nRetest = st_tray_info[nSend_SIte].st_pcb_info[y][x].nRetest;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nBdNum = st_tray_info[nSend_SIte].st_pcb_info[y][x].nBdNum;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nEnable = st_tray_info[nSend_SIte].st_pcb_info[y][x].nEnable;
				//st_tray_info[THD_MACHINE_INPUT_NETWORK_SITE].st_pcb_info[y][x].nScrCodd =5;
			}
		}

		st_tray_info[nRcv_Site].nNewLotIn = st_tray_info[nSend_SIte].nNewLotIn;
		st_tray_info[nRcv_Site].nLastModule = st_tray_info[nSend_SIte].nLastModule; //마지막 자재인지 체크
		st_tray_info[nRcv_Site].nModuleCnt = st_tray_info[nSend_SIte].nModuleCnt;
		st_tray_info[nRcv_Site].nTrayExist = st_tray_info[nSend_SIte].nTrayExist; //tray 존재 유/무 
		st_tray_info[nRcv_Site].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
		st_tray_info[nRcv_Site].strPartNo = st_tray_info[nSend_SIte].strPartNo;
		st_tray_info[nRcv_Site].strProcess = st_tray_info[nSend_SIte].strProcess;	// process id.....

		/*if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}
	else if (nMode == 1) // nMode == 1 : 데이터 쉬프트 진행, send data 는 지운다 
	{	 
		for(y = 0; y < st_recipe.nTrayY; y++)
		{
			for(x=0; x < st_recipe.nTrayX; x++)
			{
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strLotNo	= st_tray_info[nSend_SIte].st_pcb_info[y][x].strLotNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strSerialNo	= st_tray_info[nSend_SIte].st_pcb_info[y][x].strSerialNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strCSerialNo= st_tray_info[nSend_SIte].st_pcb_info[y][x].strCSerialNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strBarcode	= st_tray_info[nSend_SIte].st_pcb_info[y][x].strBarcode;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPartNo	= st_tray_info[nSend_SIte].st_pcb_info[y][x].strPartNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strArrNo	= st_tray_info[nSend_SIte].st_pcb_info[y][x].strArrNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPPID		= st_tray_info[nSend_SIte].st_pcb_info[y][x].strPPID;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPSID		= st_tray_info[nSend_SIte].st_pcb_info[y][x].strPSID;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strWWN		= st_tray_info[nSend_SIte].st_pcb_info[y][x].strWWN; 

				st_tray_info[nRcv_Site].st_pcb_info[y][x].nYesNo		= st_tray_info[nSend_SIte].st_pcb_info[y][x].nYesNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nBin		= st_tray_info[nSend_SIte].st_pcb_info[y][x].nBin;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nRetest		= st_tray_info[nSend_SIte].st_pcb_info[y][x].nRetest;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nBdNum		= st_tray_info[nSend_SIte].st_pcb_info[y][x].nBdNum;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nEnable		= st_tray_info[nSend_SIte].st_pcb_info[y][x].nEnable;


				st_tray_info[nSend_SIte].st_pcb_info[y][x].strLotNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strSerialNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strCSerialNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strBarcode = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strPartNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strArrNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strPPID = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strPSID = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strWWN = ""; 

				st_tray_info[nSend_SIte].st_pcb_info[y][x].nYesNo = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nBin = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nRetest = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nBdNum = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nEnable = 0;
			}
		}
		st_tray_info[nRcv_Site].nNewLotIn = st_tray_info[nSend_SIte].nNewLotIn;
		st_tray_info[nRcv_Site].nLastModule = st_tray_info[nSend_SIte].nLastModule; //마지막 자재인지 체크
		st_tray_info[nRcv_Site].nModuleCnt = st_tray_info[nSend_SIte].nModuleCnt;
		st_tray_info[nRcv_Site].nTrayExist = st_tray_info[nSend_SIte].nTrayExist; //tray 존재 유/무 
		st_tray_info[nRcv_Site].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
		st_tray_info[nRcv_Site].strPartNo = st_tray_info[nSend_SIte].strPartNo;
		st_tray_info[nRcv_Site].strProcess = st_tray_info[nSend_SIte].strProcess;	// process id.....

		st_tray_info[nSend_SIte].nNewLotIn = 0;
		st_tray_info[nSend_SIte].nLastModule = 0; //마지막 자재인지 체크
		st_tray_info[nSend_SIte].nModuleCnt = 0;
		st_tray_info[nSend_SIte].nTrayExist = 0;
		st_tray_info[nSend_SIte].strLotNo = "";// lot no......
		st_tray_info[nSend_SIte].strPartNo = "";
		st_tray_info[nSend_SIte].strProcess = "";	// process id.....


		/*if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}
	else if(nMode == 2) // 현재 작업하는 lot의 정보를 넣어준다. 디바이스 정보는 nDvc_Yes_No 변수의 셋팅 값으로 한다 
	{	 
		for(y = 0; y < st_recipe.nTrayY; y++)
		{
			for(x=0; x < st_recipe.nTrayX; x++)
			{
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strSerialNo = "";
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strCSerialNo = "";
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strBarcode = "";
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPartNo =  st_tray_info[nSend_SIte].strPartNo;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strArrNo = "";
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPPID = "";
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strPSID = "";
				st_tray_info[nRcv_Site].st_pcb_info[y][x].strWWN = ""; 

				st_tray_info[nRcv_Site].st_pcb_info[y][x].nYesNo = nDvc_Yes_No;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nBin = 0;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nRetest = 0;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nBdNum = 0;
				st_tray_info[nRcv_Site].st_pcb_info[y][x].nEnable = 0; 
			}
		}

		st_tray_info[nRcv_Site].nNewLotIn = st_tray_info[nSend_SIte].nNewLotIn;
		st_tray_info[nRcv_Site].nLastModule = st_tray_info[nSend_SIte].nLastModule; //마지막 자재인지 체크
		st_tray_info[nRcv_Site].nModuleCnt = st_tray_info[nSend_SIte].nModuleCnt;
		st_tray_info[nRcv_Site].nTrayExist = st_tray_info[nSend_SIte].nTrayExist; //tray 존재 유/무 
		st_tray_info[nRcv_Site].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
		st_tray_info[nRcv_Site].strPartNo = st_tray_info[nSend_SIte].strPartNo;
		st_tray_info[nRcv_Site].strProcess = st_tray_info[nSend_SIte].strProcess;	// process id.....


		/*if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}		 

	else //if(n_mode == 3) // 정보를 쉬프트 했으니 모든 필요 동작 완료 후 이 사이트의 정보를 클리어 한다 
	{
		//memset(&st_tray_info[nSend_SIte], 0x00, sizeof(st_tray_info[nSend_SIte]) ); //clear 
		for(y = 0; y < st_recipe.nTrayY; y++)
		{
			for(x=0; x < st_recipe.nTrayX; x++)
			{
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strLotNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strSerialNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strCSerialNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strBarcode = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strPartNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strArrNo = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strPPID = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strPSID = "";
				st_tray_info[nSend_SIte].st_pcb_info[y][x].strWWN = ""; 

				st_tray_info[nSend_SIte].st_pcb_info[y][x].nYesNo = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nBin = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nRetest = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nBdNum = 0;
				st_tray_info[nSend_SIte].st_pcb_info[y][x].nEnable = 0;
				//st_tray_info[THD_MACHINE_INPUT_NETWORK_SITE].st_pcb_info[y][x].nScrCodd =5;
			}
		}

		st_tray_info[nSend_SIte].nNewLotIn = 0;
		st_tray_info[nSend_SIte].nLastModule = 0; //마지막 자재인지 체크
		st_tray_info[nSend_SIte].nModuleCnt = 0;
		st_tray_info[nSend_SIte].nTrayExist = 0;
		st_tray_info[nSend_SIte].strLotNo = "";// lot no......
		st_tray_info[nSend_SIte].strPartNo = "";
		st_tray_info[nSend_SIte].strProcess = "";	// process id.....
		

		/*if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}		 

	return RET_GOOD;
}


int CPublic_Function::DoorOpenCheckSpot()
{
	int Ret = CTLBD_RET_GOOD, n_check = 0, i = 0;
	int nDoorState = CTLBD_RET_PROCEED;
	char Jamcode[10];
	
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck1, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980000");
		alarm.mstr_code = _T(Jamcode); 

		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck1 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck2, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980001");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck2 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck3, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980002");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck3 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck4, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980003");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck4 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck5, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980004");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck5 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck6, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980005");
		alarm.mstr_code = _T(Jamcode); 
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck6 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck7, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980006");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck7 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck8, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980007");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck8 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck9, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980008");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck9 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck10, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980009");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck10 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		return CTLBD_RET_ERROR;
	}
	
	return CTLBD_RET_GOOD;
}

int CPublic_Function::Check_RunAllSafety()
{
	int i = 0, nFuncRet = RET_GOOD;

	for ( i = LDPICKER; i < MAX_SAFETY_POSITION; i++)
	{
		if( st_sync.nCarrierSateyflag[i] != RET_GOOD )
		{
			nFuncRet = RET_ERROR;
			break;
		}
	}

	return nFuncRet;
}

int CPublic_Function::Check_Carrier_MoveUp_Enable( int nMode)
{
	int nFuncRet = RET_ERROR;
	int nRet[10] = {0,};
	nRet[0] = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);	// Device 눌러줌 
	nRet[1] = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_ON);
	nRet[2] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_ON);	// 하단에서 Jig 밀어줌 
	nRet[3] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_ON);
	nRet[4] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);		// 상단에서 Jig 밀어줌 
	nRet[5] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_ON);
	nRet[6] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Up_Check, IO_ON);		// Left z
	nRet[7] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
	nRet[8] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Up_Check, IO_ON);		// Right Z
	nRet[9] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
	nRet[10] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);		// Left 끝단에서 Jig 걸어줌 
	nRet[11] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, IO_ON);
	nRet[12] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_ON);		// Right 끝단에서 Jig 걸어줌 
	nRet[13] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);		
	nRet[14] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);	// 상단 중간에서 Jig Hole에 집어넣어 JIg 잡아줌 
	nRet[15] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_ON);	
	nRet[16] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Forward_Check, IO_ON);		// 상단 중간에서 jig 앞뒤로 눌러서 jig 잡아줌 
	nRet[17] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Backward_Check, IO_ON);

	if ( nMode == 1 )//TOP 이동 조건
	{
		nFuncRet = RET_GOOD;
	}
	else if( nMode == 2 )//BOTTOM 이동조건
	{
		nFuncRet = RET_GOOD;
	}
	else if(nMode == 3)//ZIG 다운 조건
	{
		nFuncRet = RET_GOOD;
	}
	else if( nMode == 4)//ZIP 업 조건
	{
		nFuncRet = RET_GOOD;
	}
	else
	{
		nFuncRet = RET_ERROR;
	}
	return nFuncRet;
}


//////////////////////////////////////////////////////////////////////////
//항상 바코드를 읽고 난뒤이야 한다.
//1 Mode = 0 // 평상시 상태  Left carrier는 감지가 않되는 상태이고 Right는 감지가 되는 상태이다.
//2. Mode = 1 밀고 난뒤에 상태
//3 .Mode = 2 밀고 복귀상태
int CPublic_Function::Check_BeforeMoveUnPressRbt( int nMode )
{
	return RET_GOOD;
	int nFuncRet = RET_ERROR;
	int nRet[40]= {0,};

	//Press_UnPress 실린더
	nRet[0] = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
	nRet[1] = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
	nRet[2] = g_ioMgr.get_out_bit(st_io.o_Press_Up_Sol, IO_ON);
	nRet[3] = g_ioMgr.get_out_bit(st_io.o_Press_Down_Sol, IO_OFF);
	nRet[4] = g_ioMgr.get_in_bit( st_io.i_Press_UpDown_CellIn_Check, IO_OFF);

	//top에서 carrier를 밀때 내려와서 미는 실린더
	nRet[5] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
	nRet[6] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
	nRet[7] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, IO_OFF);//up
	nRet[8] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, IO_OFF);//down

	//양쪽 가이드는 down상태
	nRet[9] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z1_Up_Check, IO_OFF);
	nRet[10] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
	nRet[11] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z1_Up_Sol, IO_OFF);
	nRet[12] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z1_Down_Sol, IO_ON);

	nRet[13] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z2_Up_Check, IO_OFF);
	nRet[14] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
	nRet[15] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z2_Up_Sol, IO_OFF);
	nRet[16] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z2_Down_Sol, IO_ON);

	//Clamp 벌림,닫힘
	nRet[17] = g_ioMgr.get_out_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);
	nRet[18] = g_ioMgr.get_out_bit(st_io.i_Carrier_1_Backward_Check, IO_ON);
	nRet[19] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_1_Forward_Sol, IO_ON);
	nRet[20] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_1_Backward_Sol, IO_ON);

	nRet[21] = g_ioMgr.get_out_bit(st_io.i_Carrier_2_Forward_Check, IO_ON);
	nRet[22] = g_ioMgr.get_out_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);
	nRet[23] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_2_Forward_Sol, IO_ON);
	nRet[24] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_2_Backward_Sol, IO_ON);


	nRet[25] = g_ioMgr.get_in_bit( st_io.i_Carrier_Z_1_Up_Check, IO_OFF); //Carrier가 위에 없어야 한다.
	nRet[26] = g_ioMgr.get_in_bit( st_io.i_Carrier_Z_2_Up_Check, IO_ON); //Carrier가 위에 있어야 한다.

	//비젼위치(5번)에서 지그 고정시키는 실린더
	nRet[27] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_ON);
	nRet[28] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);
	nRet[29] = g_ioMgr.get_out_bit(st_io.o_Camera_Y_Jig_Press_Forward_Sol, IO_ON);
	nRet[30] = g_ioMgr.get_out_bit(st_io.o_Camera_Y_Jig_Press_Backward_Sol, IO_ON);

	//지그(2,3,6) 고정시키는 실린더
	nRet[31] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);
	nRet[32] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_ON);
	nRet[33] = g_ioMgr.get_out_bit(st_io.o_Press_Carrier_Holder_Up_Sol, IO_ON);
	nRet[34] = g_ioMgr.get_out_bit(st_io.o_Press_Carrier_Holder_Down_Sol, IO_ON);

	if( nRet[0]   == IO_ON && nRet[1]  == IO_OFF && nRet[2]   == IO_ON && nRet[3]   == IO_OFF && nRet[4]  == IO_OFF && nRet[5] == IO_OFF && nRet[6] == IO_ON && 
		nRet[7]   == IO_ON && nRet[8]  == IO_OFF && nRet[9] == IO_ON && nRet[10] == IO_OFF && nRet[11] == IO_ON && nRet[12] == IO_OFF && nRet[13] == IO_ON && 
		nRet[14] == IO_OFF && nRet[15] == IO_ON && nRet[16] == IO_OFF && nRet[17] == IO_ON && nRet[18] == IO_OFF && nRet[19] == IO_ON && nRet[20] == IO_OFF && 
		nRet[21] == IO_ON && nRet[22] == IO_OFF && nRet[23] == IO_ON && nRet[24] == IO_OFF && nRet[25] == IO_OFF && nRet[26] == IO_OFF && nRet[27] == IO_OFF &&
		nRet[28] == IO_ON && nRet[29] == IO_OFF && nRet[30] == IO_ON && nRet[31] == IO_ON && nRet[32] == IO_OFF && nRet[33] == IO_ON && nRet[34] == IO_OFF) 
	{
		nFuncRet = RET_GOOD;	 
	}
	else
	{
		if( nMode == 1|| nMode == 2)//밀고 난뒤 이므로 Left Carrier 감지 Right는 감지 하면 않된다.
		{
			if( nRet[25] == IO_OFF)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_1_Up_Check); 
			}
			else if( nRet[26] == IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_2_Up_Check); 
			}
			if( nMode == 1 )//밀때는 실리더가 내려와 있어야 한다
			{
				if( nRet[5] == IO_OFF || nRet[6] == IO_ON || nRet[7] == IO_OFF || nRet[8] == IO_ON)
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check); 
				}
			}
			else
			{
				if( nRet[5] == IO_ON || nRet[6] == IO_OFF || nRet[7] == IO_ON || nRet[8] == IO_ON)
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_X2_Up_Check); 
				}
			}
		}
		else
		{
			if( nRet[25] == IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_1_Up_Check); 
			}
			else if( nRet[26] == IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_2_Up_Check); 
			}
		}
		if( nRet[0] == IO_OFF || nRet[1] == IO_ON || nRet[2] == IO_OFF || nRet[3] == IO_ON)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_UpDown_CellIn_Check); 
		}
		else if( nRet[9] == IO_ON || nRet[10] == IO_OFF || nRet[11] == IO_ON || nRet[12] == IO_OFF)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_Z1_Up_Check); 
		}
		else if( nRet[13] == IO_ON || nRet[14] == IO_OFF || nRet[15] == IO_ON || nRet[16] == IO_ON)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Slide_Guide_Z2_Up_Check); 
		}
		else if( nRet[17] == IO_ON || nRet[18] == IO_OFF || nRet[19] == IO_ON || nRet[20] == IO_OFF)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_1_Forward_Check); 
		}
		else if( nRet[21] == IO_ON || nRet[22] == IO_OFF || nRet[23] == IO_ON || nRet[24] == IO_OFF)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_2_Forward_Check); 
		}
		else if( nRet[27] == IO_ON || nRet[28] == IO_OFF || nRet[29] == IO_ON || nRet[30] == IO_OFF)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Camera_Y_Jig_Press_Forward_Check); 
		}
		else if( nRet[31] == IO_OFF || nRet[32] == IO_ON || nRet[33] == IO_OFF || nRet[34] == IO_ON)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Carrier_Holder_Up_Check); 
		}
		else
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Press_Down_Check); 
		}
		nFuncRet = RET_ERROR;
	}

	return nFuncRet;
}

void CPublic_Function::VppmSet()
{
	int nTorque = 0;

	nTorque = st_recipe.fDispenserVppmA;

	double  fTemp ;

	if(st_basic.n_3874InspectMode == CTL_YES)
		fTemp = (st_recipe.fDispenserVppmA/60);
	else
		fTemp = 0;

	WORD     wBoardToActive=0;
	WORD     wChannelToActive=0;
	//-----  output voltage ----------
	PIODA_CalVoltage(wBoardToActive,wChannelToActive,fTemp);
}

void CPublic_Function::VppmOff()
{
	int nTorque = 0;

	nTorque = st_recipe.fDispenserVppmA;

	double  fTemp ;
	fTemp = (st_recipe.fDispenserVppmA/60);

	WORD     wBoardToActive=0;
	WORD     wChannelToActive=0;
	//-----  output voltage ----------
	PIODA_CalVoltage(wBoardToActive,wChannelToActive,0);
}

void CPublic_Function::SaveHeatSinkBoxPos()
{
	int i = 0 , j = 0;
	double x1, y1, x2, y2;

	for ( i = 0; i < 30   ; i++)
	{
		st_work.d_Heatsink1Xpos[i] = 0.0;
		st_work.d_Heatsink1Ypos[i] = 0.0;
		st_work.d_Heatsink2Xpos[i] = 0.0;
		st_work.d_Heatsink2Ypos[i] = 0.0;
	}

	//X값 거리
	st_work.d_X_Box1Pos = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_2]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayX - 1));
	//Y값 거리
	st_work.d_Y_Box1Pos = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_4]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayY - 1));

	//X기울기
	st_work.d_X_Box1Deg = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_2]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayX - 1));
	//Y기울기
	st_work.d_Y_Box1Deg = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_4]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayY - 1));

// 	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 	{
// 		st_other.str_normal_msg.Format("1.nHeatSinkColCount=%d, nHeatSinkRowCount = %d, d_X_Box1Pos = %f, d_Y_Box1Pos = %f", 
// 			st_recipe.nHsTrayX, st_recipe.nHsTrayY, st_work.d_X_Box1Pos, st_work.d_Y_Box1Pos);
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 	}

	for ( i = 0; i < st_recipe.nHsTrayX ; i++)
	{
		x1 = st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_1] + (st_work.d_X_Box1Pos * i);
		for ( j = 0; j < st_recipe.nHsTrayY; j++)
		{
			y1 = st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_1] + (st_work.d_X_Box1Deg * i);
			st_work.d_Heatsink1Xpos[(j*st_recipe.nHsTrayX) + i] = x1 + (j*st_work.d_Y_Box1Deg);
			st_work.d_Heatsink1Ypos[(j*st_recipe.nHsTrayX) + i] = y1 + (j*st_work.d_Y_Box1Pos);
		}
	}	

	///////20131121
	//X값
	st_work.d_X_Box2Pos = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_2]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayX - 1));
	//Y값
	st_work.d_Y_Box2Pos = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_4]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayY - 1));

	//X기울기
	st_work.d_X_Box2Deg = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_2]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayX - 1));
	//Y기울기
	st_work.d_Y_Box2Deg = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_4]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayY - 1));


	for ( i = 0; i < st_recipe.nHsTrayX   ; i++)
	{
		x2 = st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_1] + (st_work.d_X_Box2Pos * i);
		for ( j = 0; j < st_recipe.nHsTrayY  ; j++)
		{
			{
				y2 = st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_1] + (st_work.d_X_Box2Deg * i);
				st_work.d_Heatsink2Xpos[(j*st_recipe.nHsTrayX) + i] = x2 + (j*st_work.d_Y_Box2Deg);
				st_work.d_Heatsink2Ypos[(j*st_recipe.nHsTrayX) + i] = y2 + (j*st_work.d_Y_Box2Pos);
			}
		}
	}	

// 	MyBasicData.OnRecipe_Data_Save(); 
}
////

//kwlee 2017.0412
void CPublic_Function::OnBcrReader() //input bcr reader
{
	CString strBcrCommand;
	
	// 	st_sync.n_barcode_read_serial_num[0][CTL_LEFT] = 0;
	// 	st_sync.n_barcode_read_serial_num[0][CTL_RIGHT] = 0;
	st_sync.n_barcode_read_serial_num[0][0] = -1;
// 	st_work.strBarcodeRecive = "";//수신 메세지 초기화
	st_msg.mstr_barcode = "";
	
	strBcrCommand.Format("%cLON%c", 0x02, 0x03);//바코드 리더기 읽기 시작 명령
	st_serial.str_snd[BCR_PORT -1] = strBcrCommand;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
	//::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strBcrCommand);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
	
}

BOOL CPublic_Function::SendLocalMsg(RCV_STRUCT msg)
{
	CString	strTmp;
	
	HWND hWnd = ::FindWindow(NULL, "HeatSinkInspection");
	if(hWnd)
	{
		HANDLE handle = AfxGetInstanceHandle(); //자신의 핸들 얻기
		
		m_msg.cbData = sizeof(RCV_STRUCT);
		m_msg.dwData = 1;
		m_msg.lpData = (PVOID)&msg;
		
		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)handle, (LPARAM)&m_msg);
		
		strTmp.Format("[SEND] LotID : %s, CMD : %d, Judge : %d, Detail : %d",msg.sObjectID, msg.nCmdID, msg.nJudge, msg.nDetailInfo);
		On_LogFile_Add( LOG_TOTAL, strTmp);
		return TRUE;
	}
	else 
	{
		//		g_PublicFunction.LogMessage(_T("PatternServer handle find fail !!"),10);
	}
	return FALSE;
}

BOOL CPublic_Function::Send7387MeasureStart()
{
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	strTmp.Format("%s_%04d", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
	msg.nCmdID = HOST_CMD_7387_MEASURE;
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	return SendLocalMsg(msg);
}

BOOL CPublic_Function::SendEpoxyMeasureStart()
{
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	strTmp.Format("%s_%04d", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
	msg.nCmdID = HOST_CMD_EPOXY_MEASURE;
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	
	return SendLocalMsg(msg);
}

BOOL CPublic_Function::SendHeatsinkMeasureStart(int Pos)
{
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	st_work.strLotInputID = "Y099000425";
	st_vision.nHeatSinkVisionCount = 68;
	strTmp.Format("%s_%04d_H", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
// 	if(st_basic.strDeviceName == "TFF" || st_basic.strDeviceName == "SFF") 
// 	{
		if(Pos == 0) msg.nCmdID = HOST_CMD_HEATSINK_MEASURE2;
		else		 msg.nCmdID = HOST_CMD_HEATSINK_MEASURE3;
// 	}
// 	else
// 	{
// 		// 		if(Pos == 0) msg.nCmdID = HOST_CMD_HEATSINK_MEASURE;
// 		// 		else		 msg.nCmdID = HOST_CMD_HEATSINK_MEASURE2;
// 		msg.nCmdID = HOST_CMD_HEATSINK_MEASURE;
// 	}
	////
	
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	
	return SendLocalMsg(msg);
}

BOOL CPublic_Function::SendNobaPosStart()
{
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	strTmp.Format("%s_%04d", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
	msg.nCmdID = HOST_CMD_NOBA_POS;
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	
	return SendLocalMsg(msg);
}

BOOL CPublic_Function::SendHeatSinkPosStart()
{
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	strTmp.Format("%s_%04d", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
	msg.nCmdID = HOST_CMD_HEATSINK_POS;
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	
	return SendLocalMsg(msg);
}

BOOL CPublic_Function::SendInspectOsView()
{
	
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	strTmp.Format("%s_%04d", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
	msg.nCmdID = HOST_CMD_INSPECT_OS_VIEW;
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	return SendLocalMsg(msg);
}

BOOL CPublic_Function::SendInspectOsDisable()
{
	
	CString strTmp;
	RCV_STRUCT msg;
	ZeroMemory(&msg,sizeof(msg));
	
	strTmp.Format("%s_%04d", st_work.strLotInputID, st_vision.nHeatSinkVisionCount);
	
	strcpy(msg.sObjectID, strTmp);
	
	msg.nCmdID = HOST_CMD_INSPECT_OS_DISABLE;
	msg.nJudge = 0;
	msg.nDetailInfo = 0;
	return SendLocalMsg(msg);
}

void CPublic_Function::OnCycleTime(int nMode, CString strLotNo, CString strPartNo, DWORD dwTime1, DWORD dwTime2, DWORD dwTime3)
{
	CString strFileName;				// 마지막으로 생성된 파일 이름 저장 변수 
	CString strCreateFile;				// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString strListName, strTempData;	// 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString strContent;				// 한 라인에 출력할 문자열 정보 저장 변수 
	int nExistence;						// 파일 존재 유무 설정 플래그 
	char chFileName[256];				// 검색할 파일 정보 설정 함수 
	char chMsg[1000];
	FILE  *fp ;							// 파일에 대한 포인터 설정 변수 

	CString strTitleTime, strFileTime;		// 파일에 출력할 타이틀 시간 저장 변수
	int nCurYear, nCurMonth, nCurDay;					// 년, 월, 일 저장 변수
	int nCurHour, nCurMinute, nCurSecond;				// 년, 월, 일 저장 변수
	int nMtbi = 0;

	//	double dAve;

	COleDateTime otCurr;									// 현재 시간 저장 변수
	COleDateTime tNext;
	CTime tCurr;										// 타이틀 시간 저장 변수

	CString strData, strTime;

	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	otCurr = COleDateTime::GetCurrentTime();

	nCurYear		= otCurr.GetYear();  
	nCurMonth		= otCurr.GetMonth();  
	nCurDay			= otCurr.GetDay();  

	nCurHour		= otCurr.GetHour();
	nCurMinute		= otCurr.GetMinute();
	nCurSecond		= otCurr.GetSecond();

	switch (nMode)
	{
	case 0:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("LD_ROBOT\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s CYCLE TIME : [%10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime1 / (DWORD)1000);
		break;

	case 1:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("LD_BUFFER\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : [%10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime1 / (DWORD)1000);
		break;

	case 2:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("UNLD_ROBOT\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : [%10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime1 / (DWORD)1000);
		break;

	case 3:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("UNLD_BUFFER\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : [%10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime1 / (DWORD)1000);
		break;

	case 4:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("TEST_SITE_ROBOT\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : [%10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime1 / (DWORD)1000);
		break;

	case 5:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("TEST_SITE_ROBOT\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : [%10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime1 / (DWORD)1000);
		break;

	case 6:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("LD_TRAY\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : %10d s [%10d(SMEMA) %10d(MOVE) %10d(STOP) %10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime2 / (DWORD)1000,
			dwTime1 / (DWORD)1000,
			(dwTime2 / (DWORD)1000) - (dwTime3 / (DWORD)1000) - (dwTime1 / (DWORD)1000),
			dwTime3 / (DWORD)1000,
			dwTime2 / (DWORD)1000);
		break;

	case 7:
		strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
		if (otCurr.GetHour() == 22 || otCurr.GetHour() == 23 || otCurr.GetHour() == 24)
		{
			if (otCurr.GetHour() == 22 && otCurr.GetSecond() > 0)
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
			else
			{
				tNext = otCurr + COleDateTimeSpan(1, 0, 0, 0);
				strTime.Format(_T("CYCLE_%04d%02d%02d"), tNext.GetYear(), tNext.GetMonth(), tNext.GetDay());
			}
		}

		strCreateFile = _T("C:\\AMT820\\Log\\LOT_DATA\\");
		strCreateFile += _T("UNLD_TRAY\\");
		strCreateFile += strTime;
		strCreateFile += ".TXT";

		strData.Format(_T("[%04d%02d%02d%02d%02d%02d] LotNo : %s PartNo : %s  CYCLE TIME : %10d s [%10d(SMEMA) %10d(MOVE) %10d(STOP) %10d]"), nCurYear,
			nCurMonth,
			nCurDay,
			nCurHour,
			nCurMinute,
			nCurSecond,
			strLotNo,
			strPartNo,
			dwTime2 / (DWORD)1000,
			dwTime1 / (DWORD)1000,
			(dwTime2 / (DWORD)1000) - (dwTime3 / (DWORD)1000) - (dwTime1 / (DWORD)1000),
			dwTime3 / (DWORD)1000,
			dwTime2 / (DWORD)1000);
		break;
	}

	//OnStringToChar(strCreateFile, chFileName);
	sprintf(chFileName, "%s", strCreateFile);

	nExistence = _access(chFileName, 0);

	if (nExistence == -1)  /* 파일 미존재 */
	{
		//		strCreateFile = st_path_info.strPathCycle + strTime;
		//		strCreateFile += ".TXT";
		//OnStringToChar(strCreateFile, chFileName);
		sprintf(chFileName, "%s", strCreateFile);
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	//fopens(&fp, chFileName, "a+");
	fp = fopen(chFileName, "a+");
	if(!fp)
	{
		//		AfxMessageBox(_T("The failure because we open the file."));
		return;
	}
	/* ************************************************************************** */


	strContent.Format(_T("| %-100s |"), strData);
	//OnStringToChar(strContent, chMsg);
	sprintf(chMsg, "%s", strContent);
	fprintf(fp,"%s\r\n", chMsg) ;

	if (ferror(fp))  
	{
		fclose(fp); 

		return ;
	}

	fclose(fp); 
}


void CPublic_Function::OnLotCycleData(CString strLotNo, CString strPartNo, int nCount, DWORD dwTray, DWORD dwLot)
{
	CString strFileName;				// 마지막으로 생성된 파일 이름 저장 변수 
	CString strCreateFile;				// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString strListName, strTempData;	// 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString strContent;				// 한 라인에 출력할 문자열 정보 저장 변수 
	int nExistence;						// 파일 존재 유무 설정 플래그 
	char chFileName[256];				// 검색할 파일 정보 설정 함수 
	char chMsg[1000];
	FILE  *fp ;							// 파일에 대한 포인터 설정 변수 

	CString strTitleTime, strFileTime;		// 파일에 출력할 타이틀 시간 저장 변수
	int nCurYear, nCurMonth, nCurDay;					// 년, 월, 일 저장 변수
	int nCurHour, nCurMinute, nCurSecond;				// 년, 월, 일 저장 변수
	int nMtbi = 0;

	double dAve;

	COleDateTime otCurr;									// 현재 시간 저장 변수
	CTime tCurr;										// 타이틀 시간 저장 변수

	CString strTemp;

	CString strData, strTime;

	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	otCurr = COleDateTime::GetCurrentTime();

	nCurYear		= otCurr.GetYear();  
	nCurMonth		= otCurr.GetMonth();  
	nCurDay			= otCurr.GetDay();  

	nCurHour		= otCurr.GetHour();
	nCurMinute		= otCurr.GetMinute();
	nCurSecond		= otCurr.GetSecond();

	strTime.Format(_T("CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);

	strCreateFile = st_path.strPathCycle + strTime;
	strCreateFile += ".TXT";

	//OnStringToChar(strCreateFile, chFileName);
	sprintf(chFileName, "%s", strCreateFile);
	nExistence = _access(chFileName, 0);

	if (nExistence == -1)  /* 파일 미존재 */
	{
		strCreateFile = st_path.strPathCycle + strTime;
		strCreateFile += ".TXT";
		//OnStringToChar(strCreateFile, chFileName);
		sprintf(chFileName, "%s", strCreateFile);
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	//fopen_s(&fp, chFileName, "a+");
	fp = fopen(chFileName, "a+");
	
	if(!fp)
	{
		//		AfxMessageBox(_T("The failure because we open the file."));
		return;
	}
	/* ************************************************************************** */
	dAve = 0.0f;
	if (nCount > 0) 
	{
		dAve = ((double)dwLot / (double)nCount);
		st_work.dDailyCycle = dAve;
	}
	strTemp.Format(_T("LOTNO : [%s] PARTNO : [%s] Cycle Time [Tray : %d] [Lot : %.2f]"), strLotNo, strPartNo, dwTray, dAve);
	strContent.Format(_T("| %-100s |"), strTemp);
	//OnStringToChar(strContent, chMsg);
	sprintf(chMsg, "%s", strContent);
	fprintf(fp,"%s\r\n", chMsg) ;

	if (ferror(fp))  
	{
		//		Func.MsgLog("파일 저장 실패!..") ;
		fclose(fp); 

		return ;
	}

	fclose(fp); 
}

void CPublic_Function::OnDailyCycleData(CString strLotNo, CString strPartNo, double dTime, CTime tStart, CTime tEnd)
{
	CString strFileName;				// 마지막으로 생성된 파일 이름 저장 변수 
	CString strCreateFile;				// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString strListName, strTempData;	// 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString strContent;				// 한 라인에 출력할 문자열 정보 저장 변수 
	int nExistence;						// 파일 존재 유무 설정 플래그 
	char chFileName[256];				// 검색할 파일 정보 설정 함수 
	char chMsg[1000];
	FILE  *fp ;							// 파일에 대한 포인터 설정 변수 

	CString strTitleTime, strFileTime;		// 파일에 출력할 타이틀 시간 저장 변수
	int nCurYear, nCurMonth, nCurDay;					// 년, 월, 일 저장 변수
	int nCurHour, nCurMinute, nCurSecond;				// 년, 월, 일 저장 변수
	int nMtbi = 0;

	COleDateTime otCurr;									// 현재 시간 저장 변수
	CTime tCurr;										// 타이틀 시간 저장 변수

	CString strTemp;

	CString strData, strTime;

	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	otCurr = COleDateTime::GetCurrentTime();

	nCurYear		= otCurr.GetYear();  
	nCurMonth		= otCurr.GetMonth();  
	nCurDay			= otCurr.GetDay();  

	nCurHour		= otCurr.GetHour();
	nCurMinute		= otCurr.GetMinute();
	nCurSecond		= otCurr.GetSecond();

	strTime.Format(_T("D_CYCLE_%04d%02d%02d"), nCurYear, nCurMonth, nCurDay);
	strCreateFile = st_path.strPathCycle + strTime;
	strCreateFile += ".TXT";

	//OnStringToChar(strCreateFile, chFileName);
	sprintf(chFileName, "%s", strCreateFile);
	nExistence = _access(chFileName, 0);

	if (nExistence == -1)  /* 파일 미존재 */
	{
		strCreateFile = st_path.strPathCycle + strTime;
		strCreateFile += ".TXT";
		//OnStringToChar(strCreateFile, chFileName);
		sprintf(chFileName, "%s", strCreateFile);
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	//fopen_s(&fp, chFileName, "a+");
	fp = fopen( chFileName, "a+");
	if(!fp)
	{
		//		AfxMessageBox(_T("The failure because we open the file."));
		return;
	}
	/* ************************************************************************** */

	strTemp.Format(_T("[%04d%02d%02d %02d:%02d:%02d] : START : [%04d%02d%02d %02d:%02d:%02d] END :[%04d%02d%02d %02d:%02d:%02d] PARTNO : [%s] LOTNO : [%s] Cycle Time [%.2f]"), nCurYear,
		nCurMonth,
		nCurDay,
		nCurHour,
		nCurMinute,
		nCurSecond,
		tStart.GetYear(),
		tStart.GetMonth(),
		tStart.GetDay(),
		tStart.GetHour(),
		tStart.GetMinute(),
		tStart.GetSecond(),
		tEnd.GetYear(),
		tEnd.GetMonth(),
		tEnd.GetDay(),
		tEnd.GetHour(),
		tEnd.GetMinute(),
		tEnd.GetSecond(),
		strPartNo,
		strLotNo, 
		dTime);
	strContent.Format(_T("%s"), strTemp);
	//OnStringToChar(strContent, chMsg);
	sprintf(chMsg, "%s", strContent);
	fprintf(fp,"%s\r\n", chMsg) ;

	if (ferror(fp))  
	{
		//		Func.MsgLog("파일 저장 실패!..") ;
		fclose(fp); 

		return ;
	}

	fclose(fp); 
}

void CPublic_Function::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line, str_temp_data_temp, str_content;
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch (dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";   // +----------------------------------------+----------------------------------+
		break;
		
	case 15:
		str_temp_data_temp.Format("%33s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+----------------------------------+";
		break;
		
	case 24:
		str_temp_data_temp.Format("%24s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 131:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-----------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 51:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 49:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;
		
	case 50:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;

	case 130:
		str_temp_data_temp.Format("%124s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}
