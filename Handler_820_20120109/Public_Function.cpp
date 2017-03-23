// Public_Function.cpp: implementation of the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Public_Function.h"
#include "FastechPublic_IO.h"
#include "DataAPI.h"
#include <nb30.h>

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
	int nDoorState = RET_GOOD;
	int nDoor[5], i;

	// 20110314 tae
/*	
	if (st_work.n_run_status == dINIT || st_work.n_run_status == dRUN || st_work.n_run_status == dSTOP)
	{
		nDoor[0] = FAS_IO.Get_In_Bit(st_io.i_left_front_door_chk);
		nDoor[1] = FAS_IO.Get_In_Bit(st_io.i_left_rear_door_chk);
		nDoor[2] = FAS_IO.Get_In_Bit(st_io.i_right_front_door_chk);
		nDoor[3] = FAS_IO.Get_In_Bit(st_io.i_right_rear_door_chk);
		nDoor[4] = FAS_IO.Get_In_Bit(st_io.i_rear_door_chk);

		for(i = 0; i < 5; i++)
		{
			if (nDoor[i] == IO_OFF)
			{
				//jamcode
				alarm.mstr_code.Format("9100%02d", i);
				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING;
				st_work.n_run_status = dWARNING; //종류 설정 (0:경고성    1:진행성    2:장비 정지) //
				
				alarm.n_alarm_assign_section = 200;
				
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("[Safety Error] Handler Door Open!! [%d%d,%d%d,%d]", nDoor[0], nDoor[1], nDoor[2], nDoor[3], nDoor[4]);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				nDoorState = RET_ERROR;
				break;
			}
		}
	}
*/
	return nDoorState;
}

void CPublic_Function::OnSet_IO_Port_Stop()
{
	if ( st_work.m_iRunStatus == dSTOP )
		return;
	
	//if( ( st_work.m_iRunStatus == dRUN || st_work.m_iRunStatus == dWARNING ) && !st_work.m_bInitStatus )
	//{
	//	Handler_Recovery_Data_Write();
	//}

	CString mstr_temp = "";

	st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
	st_timedb.mnSectionBuffer = dSTOP;
	st_timedb.mole_date		= COleDateTime::GetCurrentTime();
//	DB_Write_Time(st_timedb);
	
	st_work.m_iRunStatus = dSTOP;
	COMI.mn_run_status = dSTOP;
	
//	st_handler.mn_status =  CTL_NO;					// 장비가 가동중이냐.. 2K9/12/29/ViboX
	
	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp, IO_ON);
		
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.m_iRunStatus);

	sprintf(st_msg.c_normal_msg, "[Button] STOP.");
	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);

	Func.On_LogFile_Add( LOG_TIME, "[Button] STOP." );

	/*
	if( st_basic.m_iXGemMode == CTL_YES )
	{
		if( g_XGemMgr.IsXGemRdy() )
		{
			// Xgem 보고 해야함.
			g_XGemMgr.SETVAL(2101, st_lamp.mstr_equip_id, true);	// Equip ID
			g_XGemMgr.SETVAL(2102, MC_STOP);						// Machine Status
			g_XGemMgr.SetEVT(c_MachineStatus);
		}
	}

	*/

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
	
//	mstr_temp.Format("%d",st_handler.m_tDMTBI);
//	:: WritePrivateProfileString("BasicData", "Daily_MTBI_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

//	if( st_basic.m_iHISMode == CTL_YES )
//	{
//		g_client_his.SetMachineStatus();
//	}

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
			nPickerStats = FAS_IO.Get_In_Bit(st_io_def.i_main_robot_gripper_on_check_sensor[i]);
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

//	FAS_IO.Set_Out_Bit(st_io_def.o_main_air, IO_ON);

	FAS_IO.Set_Out_Bit(st_io_def.o_start_switch_lamp, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io_def.o_stop_switch_lamp, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io_def.o_reset_switch_lamp, IO_OFF);
//	FAS_IO.Set_Out_Bit(st_io_def.o_buzzer_stop_lamp, IO_OFF);
//	FAS_IO.Set_Out_Bit(st_io_def.o_loader_lamp, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io_def.o_loader_switch_lamp, IO_OFF);
//	FAS_IO.Set_Out_Bit(st_io_def.o_dispenser_airblow_sol, IO_OFF);

	FAS_IO.Set_Out_Bit(st_io_def.o_tower_lamp_blue, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io_def.o_tower_lamp_red, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io_def.o_tower_lamp_green, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io_def.o_tower_lamp_yellow, IO_OFF);

	FAS_IO.Set_Out_Bit(st_io.o_safety_door_free, IO_ON);*/
//	for (i = 0; i < 3; i++)
//	{
//		FAS_IO.Set_Out_Bit(st_io_def.o_towerlamp[i], IO_OFF);
//	}
// jong

	OnSet_IO_Port_Sound(IO_OFF);							// Buzzer를 Off 시킨다.

	FAS_IO.Set_Out_Bit(st_io.o_Start_SwitchLamp, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io.o_Stop_SwitchLamp, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io.o_AlarmClear_SwitchLamp, IO_OFF);
	FAS_IO.Set_Out_Bit(st_io.o_Buzzer1, IO_OFF);

	for (i = 0; i < 3; i++)
	{
		FAS_IO.Set_Out_Bit(st_io.o_tower_lamp[i], IO_OFF);
	}

}

void CPublic_Function::OnSet_IO_Port_Sound(int n_status)
{
	switch (n_status)
	{
		case 0:
			FAS_IO.Set_Out_Bit(st_io.o_Buzzer1, IO_OFF);
			FAS_IO.Set_Out_Bit(st_io.o_Buzzer2, IO_OFF);
			FAS_IO.Set_Out_Bit(st_io.o_Buzzer3, IO_OFF);
			FAS_IO.Set_Out_Bit(st_io.o_Buzzer4, IO_OFF);
			break;
			
		case 1:
			if (st_lamp.mn_buzzer_mode == YES)			// Buzzer 사용 모드에서만 켠다.
			{
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer1, IO_ON);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer2, IO_ON);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer3, IO_ON);
				FAS_IO.Set_Out_Bit(st_io.o_Buzzer4, IO_ON);
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
	
	if (st_work.n_run_status != dLOTEND)		
	{
		FAS_IO.Set_Out_Bit(st_io.o_AlarmClear_SwitchLamp, IO_ON);
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
	int i;

	for (i = 0; i < M_MOTOR_COUNT; i++)
	{
		if (COMI.md_spd_vel[i][1] < 1)
		{
			COMI.md_spd_vel[i][1] = 10000;

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg.Format("%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
				sprintf(st_msg.c_abnormal_msg, "%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		
		if (COMI.md_spd_vel[i][2] < 1)
		{
			COMI.md_spd_vel[i][2] = 10000;

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s Motor Dec is under 1 -> 10000 Set up", Get_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}

		COMI.Set_MotSpeed(MOT_SPD_VEL, i, cmSMODE_T, COMI.md_spd_vel[i][0], COMI.md_spd_vel[i][1], COMI.md_spd_vel[i][2]);
		COMI.Set_MotSpeed(MOT_SPD_ORG, i, cmSMODE_T, st_motor[i].md_spd_home, COMI.md_spd_vel[i][1]/10, COMI.md_spd_vel[i][2]/10); //st_motor[i].d_spd_home[0] * 10, st_motor[i].d_spd_home[0] * 10);

		COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		Sleep(100);
	}
}

void CPublic_Function::OnSet_IO_Port_Run()
{	
	if ( st_work.m_iRunStatus == dRUN )
		return;

	/*
	if( st_basic.m_iXGemMode == CTL_YES )
	{
		if( !g_XGemMgr.IsXGemRdy() )
		{
			CString strErr = "Unable to operate equipment. XGem communication DisConnected.";
			st_msg.mstr_event_msg[0] = strErr;
			Func.OnEventMsgAdd( strErr );
			::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			return;		
		}
		
		if( st_basic.m_iRcmdMode == CTL_YES )
		{
			if( st_handler.m_nRemote == CTL_YES )
			{
				// 여기서 Xgem 보고해야함.
				//g_client_cim.SetMachineStatus(MC_RUN);
				g_XGemMgr.SETVAL(2101, st_lamp.mstr_equip_id, true);	// Equip ID
				g_XGemMgr.SETVAL(2102, MC_RUN);							// Machine Status
				g_XGemMgr.SetEVT(c_MachineStatus);
			}
			else
			{
				//CString strErr = "Remote Stop 상태입니다. Recipe가 전달되지 않았거나 서버에서 작업을 허가하지 않습니다.";
				CString strErr = "Remote state stop. Check the recipe.";
				st_msg.mstr_event_msg[0] = strErr;
				Func.OnEventMsgAdd( strErr );
				::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
				return;
			}
		}
		else
		{
			// 여기서 Xgem 보고해야함.
			//g_client_cim.SetMachineStatus(MC_RUN);
			g_XGemMgr.SETVAL(2101, st_lamp.mstr_equip_id, true);	// Equip ID
			g_XGemMgr.SETVAL(2102, MC_RUN);							// Machine Status
			g_XGemMgr.SetEVT(c_MachineStatus);
		}
	}

	*/

	st_work.m_iRunStatus = dRUN;
	COMI.mn_run_status = dRUN;

	st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
	st_timedb.mnSectionBuffer = dRUN;
		
	st_timedb.mole_date		= COleDateTime::GetCurrentTime();
		
//	DB_Write_Time(st_timedb);
	
	
	//레벨 초기화.
	st_handler.n_level_teach = FALSE;
	st_handler.n_level_maint = FALSE;
//	st_handler.n_level_speed = FALSE;
//	st_handler.n_safety = CTL_YES;						// 안전 장치 작동 2K6/06/09/ViboX
	
	//st_handler.n_status =  CTL_YES;					// 장비가 가동중이냐.. 2K9/12/29/ViboX
	//st_handler.m_iConveyorAlarm = CTL_NO;				// Conveyor Alarm Reset 2016/06/03/ViboX
	
	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp,	IO_ON);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp,	IO_OFF);

	// 도어를 잠근다.
	g_ioMgr.set_out_bit( st_io.o_Safety_Door_Lock_Release, IO_ON );
	
//	st_basic.m_bControlPanelEnable = false;

//	if (st_handler.cwnd_main != NULL)
//	{
//		st_handler.cwnd_main->PostMessage(WM_DRAW_UI_MAIN, EDDR_HIDE_CONTROL, 0);
//	}
//	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
//	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.m_iRunStatus);

//	alarm.stl_alarm_start_time = GetCurrentTime();

	sprintf(st_msg.c_normal_msg, "[Button] RUN.");
	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);

	Func.On_LogFile_Add( LOG_TIME, "[Button] RUN." );

	//if( st_basic.m_iHISMode == CTL_YES )
	//{
	//	g_client_his.SetMachineStatus();
	//}
}

void CPublic_Function::OnSet_IO_Port_Init()
{
	st_work.n_run_status = dINIT;	
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
//	FAS_IO.Set_Out_Bit(st_io_def.o_loader_lamp, IO_ON);
//	FAS_IO.Set_Out_Bit(st_io.o_loader, IO_ON);

//	FAS_IO.Set_Out_Bit(st_io_def.o_dispenser_airblow_sol, IO_ON);
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
//	FAS_IO.Set_Out_Bit(st_io_def.o_loader_lamp, IO_OFF);
//	FAS_IO.Set_Out_Bit(st_io.o_loader, IO_OFF);

//j	FAS_IO.Set_Out_Bit(st_io_def.o_dispenser_airblow_sol, IO_OFF);
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
	st_handler.m_tDR = 0;
	st_handler.m_tDS = 0;
	st_handler.m_tDJ = 0;
	st_handler.m_tDM = 0;
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
		FAS_IO.Set_Out_Bit(st_io.o_Safety_Door_Lock_Release, IO_OFF);
//		FAS_IO.Set_Out_Bit(st_io.o_left_door_lock_cyl, IO_OFF);
//		FAS_IO.Set_Out_Bit(st_io.o_right_door_lock_cyl, IO_OFF);
	}
}

void CPublic_Function::OnSet_Door_Close()
{
	//도어 락
	FAS_IO.Set_Out_Bit(st_io.o_Safety_Door_Lock_Release, IO_ON);
//	FAS_IO.Set_Out_Bit(st_io.o_left_door_lock_cyl, IO_ON);
//	FAS_IO.Set_Out_Bit(st_io.o_right_door_lock_cyl, IO_ON);
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
	
	char* cpMessage; 
	char* cpFilename;
	
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
		if(nBufferMode == 0)	 //최종적으로 체크할 전역 변수를 기준으로 센서 상태 상태 확인 
		{
			nCurrent_BufferInfo[i] = st_buffer_info[nSite].st_pcb_info[0].nYesNo;
		}
		else if(nBufferMode == 1) //함수 인자(멤버변수) 사용하여 센서 상태 상태 확인  
		{
			nCurrent_BufferInfo[i] = nBufferInfo[i];
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
		nBufferStatus[i] = FAS_IO.get_in_bit(nIO_Num_Info[i],	IO_ON);  
		npBufferOutputStatus[i] = FAS_IO.get_out_bit(nIO_Output_Num_Info[i],	IO_ON);
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

				if(nBufferStatus[i] == IO_OFF || npBufferOutputStatus[i] == IO_ON)//james 2016.0919 CTL_NO)
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
	int nIO_ExistSenChk[MAX_PICKER];
	int nPara_Num;

	int dDvcCheck_Mode = 1;
	if(nSite == THD_LOAD_WORK_RBT) 
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
// 			nIO_ExistSenChk[0][i] = st_io.i_Loader_Transfer_Left_Exist_Check; 
// 			nIO_ExistSenChk[1][i] = st_io.i_Loader_Transfer_Right_Exist_Check;
// 			nIO_Num_Info[0][i] = st_io.i_Loader_Transfer_Clamp_On_Check;  
// 			nIO_Num_Info[1][i] = st_io.i_Loader_Transfer_Clamp_Off_Check;  
// 			nIO_Output_Num_Info[0][i] = st_io.o_Loader_Transfer_Clamp_On_Sol; 
// 			nIO_Output_Num_Info[1][i] = st_io.o_Loader_Transfer_Clamp_Off_Sol; 
			nIO_Num_Info[i] = st_io.i_Loader_Transfer_Left_Exist_Check;  
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
// 			nIO_ExistSenChk[0][i] = st_io.i_Transfer1_Device_Check; 
// 			nIO_ExistSenChk[1][i] = st_io.i_Transfer1_Device_Check;
// 			nIO_Num_Info[0][i] = st_io.i_Transfer1_Clamp_Forward_Check;
// 			nIO_Num_Info[1][i] = st_io.i_Transfer1_Clamp_Backward_Check;
// 			nIO_Output_Num_Info[0][i] = st_io.o_Transfer1_Clamp_Forward_Sol; 
// 			nIO_Output_Num_Info[1][i] = st_io.o_Transfer1_Clamp_Backward_Sol; 
			nIO_Num_Info[i] = st_io.i_Transfer1_Device_Check;
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
	else if(nSite == THD_HSLOAD_WORK_RBT) 
	{
		nPara_Num = PICKER_PARA;
		for(i = 0; i < nPara_Num; i++)
		{
			if(nPickerMode == 0)//pad
			{
				//nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_Pad_Up_Check; 
				//nIO_Output_Num_Info[i] = st_io.o_HeatSink_Trasnfer_Pad_Up_Sol; 
				nIO_Num_Info[i] = st_io.i_HeatSink_Transfer_Vacuum_Check
				nIO_Output_Num_Info[i] = st_io.o_HeatSink_Vacuum_On_Sol
			}
			else//glipper
			{
				nIO_Num_Info[i] = st_io.i_Dispenser_Spray_Forward_Check; 
				nIO_Output_Num_Info[i] = st_io.o_Dispenser_Spread_Forward_Sol; 
			}
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
		npPickerStatus[i] = FAS_IO.get_in_bit(nIO_Num_Info[0][i],	IO_ON);
		npPickerOutputStatus[i] = FAS_IO.get_out_bit(nIO_Output_Num_Info[0][i],	IO_ON);
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
						if(npPickerOutputStatus[i] == IO_OFF)//Backward
						{
							npPickerStatus[i] = IO_ON;
						}
						else
						{
							npPickerStatus[i] = IO_OFF; //open 하고 있는 상태로 문제가 있어 에러처리
						}
					}
				}
				else if(nSite == THD_UNLOAD_WORK_RBT) 
				{
					if(nCurrent_PickerInfo[i] == CTL_YES) 
					{
						if(npPickerOutputStatus[i] == IO_OFF)//Backward
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
					else if(nSite == THD_UNLOAD_WORK_RBT)
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
					if(npPickerStatus[i] == IO_OFF && npPickerOutputStatus[i] == IO_OFF)
					{	//error   //감지 안됨 에러 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_UNLOAD_WORK_RBT)
				{
					if(npPickerStatus[i] == IO_OFF)
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
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_OFF)
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
int CPublicFunction::Find_TrayWork_Pos(int nMode, int nSite, int nDVCYesNO, int *npFindPosYXCPB, int nRobotSite, CString str_LotNo) //first tray Y, first tray X, first picker num, first buff num, 작업 조건에 충족하는 빈 공간 또는 자재 집을 전체 수량
{

	int x, y, i, nFuncRet, nFlag;
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
				if(st_tray_info[nSite].st_pcb_info[y][x].nYesNo == nDVCYesNO && (st_tray_info[nSite].st_pcb_info[y][x].strLotNo == str_LotNo || str_LotNo == "0") )
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
			if(st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO && st_buffer_info[nSite].st_pcb_info[i].nEnable == CTL_YES
				&& (st_buffer_info[nSite].st_pcb_info[i].strLotNo == str_LotNo || str_LotNo == "0") ) //nDVCYesNO == CTL_NO 조건에 충족한 정보만 체크, 사용가능한 소켓에만 작업가능
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

	case THD_UNLD_ALIGN_BUFF:
		nFirstY = 0;  nFirstX = 0;	
		for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
		{
			if(st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO && st_buffer_info[nSite].st_pcb_info[i].nEnable == CTL_YES
				&& (st_buffer_info[nSite].st_pcb_info[i].strLotNo == str_LotNo || str_LotNo == "0") ) //nDVCYesNO == CTL_NO 조건에 충족한 정보만 체크, 사용가능한 소켓에만 작업가능
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
			if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_skip_flag[i] != CTL_YES)
			{
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[i] != CTL_YES )
				{
					if( nDVCYesNO == CTL_NO )//디바이스 Place
					{
						if( nSite == THD_LOAD_WORK_RBT && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist == nDVCYesNO )
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
						if( nSite == THD_UNLOAD_WORK_RBT && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist == nDVCYesNO && /*st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id*/
							st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_bin !=  BIN_LDBUFFERBIN )
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
				}
			}
		}

		break;
	}

	return nFuncRet;
}

int CPublicFunction::Check_Robot_Buffer_Clash( int nRbtTHDSite, int BuffTHDSite, int nBuffStartPos, int nPickerStartPos, int *nErrorInfoStatus) //피커와 버퍼의 작업 위치간 공간의 작업 여부 가능 확인 한다
{//int nBuffStartPos 의 의미는 Buffer, Test site socket의 피커기준 버퍼등의 시작 번호이다 
	int nFuncRet = RET_GOOD;
	int i, nRet[4]={0,};
	int nPara_Num = MAX_SITE_INFO;
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
				if(st_buffer_info[BuffTHDSite].st_pcb_info[i].nYesNo == CTL_YES)  
				{	//에러 , 버퍼의 디바이스와 피커의 자재가 충돌 함
					nErrorInfoStatus[i] = CTL_YES; //에러 정보 셋팅
					m_strAlarmCode.Format(_T("8%d500%d"), IO_ON,st_buffer_info[BuffTHDSite].st_pcb_info[i].nYesNo);//error 
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
			if(BuffTHDSite == THD_UNLD_ALIGN_BUFF)
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
int CPublicFunction::Calculate_MovePos_Find(int nMode, int nMotNum, int nSite, int nWorkPart, int *npFindWorkPosYXCPB, double *dpGetTargetPos)
{
	//n_workpos[2]:[0]:x, [1]:y , n_refnum = 작업 시작할 피커 번호, 작업위치 지정 번호   
	double dBasicPos=0, dVarMveGapVal=0, dCompleteMveVal=0; 
	int nX=0, nY=0, workpoint=0;

	switch (nMotNum)
	{

	case M_LOADER_TRANSFER_Y:
		if(nSite == THD_LD_TRAY_PLATE)
		{
			dBasicPos = st_motor[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PICK_POS];
			dVarMveGapVal = (st_recipe.dTrayPitch_Y * npFindWorkPosYXCPB[0]);
			dCompleteMveVal =  (dBasicPos + dVarMveGapVal);		
		}
		else if(nSite == THD_LD_ALIGN_BUFF)//PLACE
		{
			//PLACE
			if( nWorkPart == WORK_PLACE)
				dBasicPos = st_moto[nMotNum].md_pos[P_LOADER_TRANSFER_Y_ALIGN_POS];
			//PICK
			else
				dBasicPos = st_motor_info[nMotNum].d_pos[P_LOADER_TRANSFER_Y_ALIGN_PICK_POS];
			dVarMveGapVal = (st_recipe.dTrayPitch_Y * npFindWorkPosYXCPB[0]);
			dCompleteMveVal =  (dBasicPos + dVarMveGapVal);		
		}
		else if(nSite == THD_LDULD_CARRIER_BUFF)
		{
			if( nWorkPart == 0 )
				dBasicPos = st_moto[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PLACE_TOP_POS];
			else if( nWorkPart == 1 )
				dBasicPos = st_moto[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PLACE_MID_POS];
			else
				dBasicPos = st_moto[nMotNum].md_pos[P_LOADER_TRANSFER_Y_PLACE_BOT_POS];
			dVarMveGapVal = (st_recipe.dTrayPitch_Y * npFindWorkPosYXCPB[0]);
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
				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS + npFindWorkPosYXCPB[1]];
				dCompleteMveVal =  dBasicPos;
			}
		}
		else if(nSite == THD_ULD_ALIGN_BUFF)
		{	
			dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS];
		
		break;

	case M_UNLOADER_TRANSFER_X:

		if(nSite == THD_LDULD_CARRIER_BUFF)
		{
			if( nWorkPart == WORK_PICK )
			{
				// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS];
				// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_MID_POS];
				// 				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_BOT_POS];
				dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS + npFindWorkPosYXCPB[1]];
				dCompleteMveVal =  dBasicPos;
			}
		}
		else if(nSite == THD_ULD_ALIGN_BUFF)
		{	
			dBasicPos = st_moto[nMotNum].md_pos[P_UNLOADER_TRANSFER_X_ZIGPLACE_POS];
			dCompleteMveVal =  dBasicPos;
		}
		break;
	}

	//limit 값 셋팅 에러 
	if(dCompleteMveVal < st_motor_info[nMotNum].d_limit_position[0]) //- LIMIT 
	{//000004
		//000010 0 A "MOTOR SOFTWARE MINUS LIMIT CHECK ERROR -[M_ROBOT_X]."
		//000011 0 A "MOTOR SOFTWARE PULS LIMIT CHECK ERROR -[M_ROBOT_X]."
		m_strAlarmCode.Format(_T("%02d0010"), nMotNum);
		return RET_ERROR;
	}	

	if(dCompleteMveVal > st_motor_info[nMotNum].d_limit_position[1]) //+ LIMIT 
	{
		m_strAlarmCode.Format(_T("%02d0011"), nMotNum);
		return RET_ERROR;
	} 

	*dpGetTargetPos = dCompleteMveVal;  

	return RET_GOOD;
}


int CPublicFunction::Data_Exchange_PickPlace(int nPickPlace, int nJobCnt, int nRobotSite, int nWorkSite, int *npWorkPos)//Pick & Place시 picker & Tray data exchange 작업
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin				    = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nBinNum;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN				= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo			= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strCSerialNo;
				
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
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nYesNo = CTL_NO;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nBin = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetestCnt = 0;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetest = 0;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nBinNum = -1;//현재빈만 사용 
				//st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetestSite[st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].nRetestCnt] = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nBdNum  = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nScrCode = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strLotNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strSerialNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strBarcode = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strPartNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strArrNo = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strPPID = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strPSID = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strWWN = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strCSerialNo = _T("");
				//test site만 사용 st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].tStart;	
				// jtkim 20160903
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strBarcode1D[0] = _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strBarcode1D[1] = _T("");
				// jtkim 20160929
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strBarcode2D[0]	= _T("");
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].strBarcode2D[1]	= _T("");
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin					= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nRetestCnt[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest			    = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nRetest[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBinNum[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBdNum[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode				= st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nScrCode[npWorkPos[1]];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chCSerialNo[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chBarcode[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_part_num[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chArrNo[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chPPID[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chPSID[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chWWN[npWorkPos[1]]);
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo.Format( "%s", st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id[npWorkPos[1]]);
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
		}
		else if(nRobotSite == THD_LOADHS_WORK_RBT) //PICKER_PICK_MODE
		{
			if(nWorkSite == THD_LD_HSALIGN_BUFF) //테스트 소켓에서 자재 집는 동작
			{//피커는 정보를 가지고 가고, 테스트 소켓은 정보가 클리어 된다 		

			}			
			else if(nWorkSite == THD_LD_HSPICK_BUFF) //테스트 소켓에서 자재 집는 동작
			{//피커는 정보를 가지고 가고, 테스트 소켓은 정보가 클리어 된다 		

			}
			else if(nWorkSite == THD_HS_CARRIER_BUFF) //테스트 소켓에서 자재 집는 동작
			{//피커는 정보를 가지고 가고, 테스트 소켓은 정보가 클리어 된다 		

			}


			///////////////////////////////	 
			//data exchange
			//////////////////////////////
			if(nWorkSite == THD_LD_BUFF || nWorkSite == THD_RETEST_1_BUFF || nWorkSite == THD_RETEST_2_BUFF)
			{
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npWorkPos[0]].nEnable = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nEnable;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt			= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;//현재빈만 사용 
				//st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestSite[st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt]		= st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetestSite[st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nRetestCnt];
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

				// jtkim 20160903
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode1D[0]		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode1D[0];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode1D[1]		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode1D[1];
				// jtkim 20160929
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode2D[0]		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode2D[0];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode2D[1]		= st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode2D[1];
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npRobotPos[3]].tStart = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].tStart;			

				//////////////////////
				//data clear
				/////////////////////////
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npRobotPos[3]].nEnable = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nEnable;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo = CTL_NO;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin = -1;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest = 0;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt = -1;
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

				// jtkim 201609003
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode1D[0]	= _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode1D[1]	= _T("");
				// jtkim 20160929
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode2D[0]	= _T("");
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode2D[1]	= _T("");
				//test site만 사용 st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].tStart;	
			}
			else if(nWorkSite >= THD_TESTSITE_1 && nWorkSite <= THD_TESTSITE_8) //테스트 소켓에서 자재 집는 동작
			{//피커는 정보를 가지고 가고, 테스트 소켓은 정보가 클리어 된다 		
				 
			 
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].nEnable = st_test_site_info[nWorkSite][npWorkPos[0]].st_pcb_info[npWorkPos[1]].nEnable;
				
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo		= st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nYesNo;

				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin		= st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nBin;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt  = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nRetestCnt;
				//kwlee 2017.0203
				if( st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin == BD_DATA_CONTINUE_FAIL )
				{
					if (st_handler_info.cWndList != NULL)
					{
						strTemp.Format(_T("[Data_Exchange] TestNum:%d Picker: %d: CONTINUE_FAIL->DATA_RETEST "),(nWorkSite - 18) +1, npWorkPos[3] +1);
						clsMem.OnNormalMessageWrite(strTemp);
					}
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin = BD_DATA_RETEST;
				}
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest     = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nRetest;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum		= st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nBinNum;//현재빈만 사용 
				//st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestSite[st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt] = st_test_site_info[nWorkSite - THD_LEFT_TEST_SITE][npWorkPos[0]].st_pcb_info[npWorkPos[1]].nRetestSite[st_test_site_info[nWorkSite][npWorkPos[0]].st_pcb_info[npWorkPos[1]].nRetestCnt];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum        = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nBdNum ;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode      = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nScrCode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo      = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strLotNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo    = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strSerialNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode    = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo     = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strPartNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo     = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strArrNo;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID      = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strPPID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID      = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strPSID;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN       = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strWWN;
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo  = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strCSerialNo;

				// jtkim 20160903
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode1D[0]  = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode1D[0];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode1D[1]  = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode1D[1];
				// jtkim 20160929
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode2D[0]	 = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode2D[0];
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode2D[1]	 = st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode2D[1];
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].tStart = st_test_site_info[nWorkSite][npWorkPos[0]].st_pcb_info[npWorkPos[1]].tStart;			
				 
				//////////////////////
				//data clear
				/////////////////////////
				//test site만 사용 st_test_site_info[nWorkSite][npWorkPos[0]].st_pcb_info[npWorkPos[1]].nEnable =  ;
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nTestBdStart = BD_NONE; //2015.0313 추가
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nYesNo = CTL_NO;
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nBin = -1;
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nRetestCnt = 0; //kwlee 2016.1104 test
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nRetest = 0;
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nBinNum = -1;//현재빈만 사용 
				//st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nRetestSite[st_picker[nRobotSite].st_pcb_info[npWorkPos[1]].nRetestCnt] = -1; //kwlee 2017.0117 프로그램 죽어서 막아 놓음.
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nBdNum  = -1;
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].nScrCode = -1;
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strLotNo = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strSerialNo = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strPartNo = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strArrNo = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strPPID = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strPSID = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strWWN = _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strCSerialNo = _T("");
				 
				// jtkim 20160903
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode1D[0]	= _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode1D[1]	= _T("");
				// jtkim 20160929
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode2D[0]	= _T("");
				st_test_site_info[nWorkSite].st_pcb_info[npWorkPos[1]].strBarcode2D[1]	= _T("");
				/* james 2016.0804 
				// jtkim 20150424
				if (nWorkSite == THD_LEFT_TEST_SITE)
				{
					clsInterfaceC.m_nBdInfo[0][npWorkPos[0]][npWorkPos[1]] = NO;
				}
				else
				{
					clsInterfaceC.m_nBdInfo[1][npWorkPos[0]][npWorkPos[1]] = NO;
				}//*/
				
				//test site만 사용 st_test_site_info[nWorkSite][npWorkPos[0]].st_pcb_info[npWorkPos[1]].tStart;	
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo;
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
			else if(nWorkSite == THD_LDULD_CARRIER_BUFF)
			{
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_exist[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBin[npWorkPos[1]]				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nRetestCnt[npWorkPos[1]]		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nRetest[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBinNum[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nBdNum[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].nScrCode[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;

				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_lot_id[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chBarcode[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chSerialNo[nRobotSite], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_part_num[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chArrNo[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chPPID[npWorkPos[1]], "%s", st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPPID );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chPSID[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPSID );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chWWN[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strWWN );
				sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].c_chCSerialNo[npWorkPos[1]], "%s",  st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strCSerialNo );
				//////////////////////
				//data clear
				/////////////////////////
				//test site만 사용 st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].nEnable = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nEnable;
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
		}
		else if(nRobotSite == THD_UNLOAD_WORK_RBT)
		{
			if(nWorkSite == THD_UNLD_ALIGN_BUFF )
			{//피커는 정보가 클리어되고, 트레이는 정보를 가진다  	
			}
			///////////////////////////////	 
			//data exchange
			//////////////////////////////
			if(nWorkSite == THD_UNLD_ALIGN_BUFF)
			{
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nYesNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nYesNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBin				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBin;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetestCnt		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetestCnt;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nRetest			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nRetest;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//현재빈만 사용 
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBdNum		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBdNum ;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nScrCode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nScrCode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strLotNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strLotNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strSerialNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strSerialNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strBarcode		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strBarcode;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strPartNo		= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strPartNo;
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].strArrNo			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].strArrNo;
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
	}
	
	return RET_GOOD;
}


////////////////////////////////////////////////////////////////////////
// 모든 사이트의 트레이 및 자재 정보를 쉬프트 관리한다  
////////////////////////////////////////////////////////////////////////
int CPublicFunction::Handler_Tray_DataInfo_Shift(int nMode, int nDvc_Yes_No, int nSend_SIte, int nRcv_Site)
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
	
// 	//	return CTLBD_RET_GOOD;
// 	
// 	if (st_handler.n_safety == CTL_YES)
// 	{
// 		for (i = 0; i < 9; i++)
// 		{
// 			if (g_ioMgr.get_in_bit(st_io.i_door_chk[i]) != TRUE && n_check == 0)
// 			{
// 				n_check++;
// 				break;
// 			} 
// 		}
// 		
// 		if (st_handler.mn_machine_id == 0)	// 새로 추가된 Door 2K10/10/04/ViboX
// 		{
// 			if (g_ioMgr.get_in_bit(st_io.i_door_chk[9]) != TRUE && n_check == 0)
// 			{
// 				n_check++;
// 			} 
// 		}
// 		
// 		if (n_check > 0)
// 		{
// 			if (st_work.m_iRunStatus == dRUN)
// 			{
// 				alarm.mstr_code.Format("90010%d", i);
// 				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
// 			}
// 			st_msg.mstr_abnormal_msg.Format("[DOOR CHECK] %s is Open", GetDoorName(i));
// 			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			Ret = CTLBD_RET_ERROR;
// 		}
// 	}
	
	return Ret;
}

int CPublicFunction::Check_RunAllSafety()
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

int CPublicFunction::Check_Carrier_MoveUp_Enable( int nMode)
{
	int nFuncRet = RET_ERROR;
	int nRet[10] = {0,};
	nRet[0] = FAS_IO.get_in_bit(st_io.i_Press_Up_Check);	// Device 눌러줌 
	nRet[1] = FAS_IO.get_in_bit(st_io.i_Press_Down_Check);
	nRet[2] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check);	// 하단에서 Jig 밀어줌 
	nRet[3] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check);
	nRet[4] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check);		// 상단에서 Jig 밀어줌 
	nRet[5] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check);
	nRet[6] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_Z1_Up_Check);		// Left z
	nRet[7] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_Z1_Down_Check);
	nRet[8] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_Z2_Up_Check);		// Right Z
	nRet[9] = FAS_IO.get_in_bit(st_io.i_Slide_Guide_Z2_Down_Check);
	nRet[10] = FAS_IO.get_in_bit(st_io.i_Carrier_1_Forward_Check);		// Left 끝단에서 Jig 걸어줌 
	nRet[11] = FAS_IO.get_in_bit(st_io.i_Carrier_1_Backward_Check);
	nRet[12] = FAS_IO.get_in_bit(st_io.i_Carrier_2_Forward_Check);		// Right 끝단에서 Jig 걸어줌 
	nRet[13] = FAS_IO.get_in_bit(st_io.i_Carrier_2_Backward_Check);		
	nRet[14] = FAS_IO.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check);	// 상단 중간에서 Jig Hole에 집어넣어 JIg 잡아줌 
	nRet[15] = FAS_IO.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check);	
	nRet[16] = FAS_IO.get_in_bit(st_io.i_Press_PIN_Guide_Forward_Check);		// 상단 중간에서 jig 앞뒤로 눌러서 jig 잡아줌 
	nRet[17] = FAS_IO.get_in_bit(st_io.i_Press_PIN_Guide_Backward_Check);

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
	int nFuncRet = RET_ERROR;
	int nRet[40]= {0,};

	//Press_UnPress 실린더
	nRet[0] = FAS_IO.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
	nRet[1] = FAS_IO.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
	nRet[2] = FAS_IO.get_out_bit(st_io.o_Press_Up_Sol, IO_ON);
	nRet[3] = FAS_IO.get_out_bit(st_io.o_Press_Down_Sol, IO_OFF);
	nRet[4] = FAS_IO.get_in_bit( st_io.i_Press_UpDown_CellIn_Check, IO_OFF);

	//top에서 carrier를 밀때 내려와서 미는 실린더
	nRet[5] = FAS_IO.get_in_bit( st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
	nRet[6] = FAS_IO.get_in_bit( st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
	nRet[7] = FAS_IO.get_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, IO_OFF);//up
	nRet[8] = FAS_IO.get_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, IO_OFF);//down

	//양쪽 가이드는 down상태
	nRet[9] = FAS_IO.get_in_bit( st_io.i_Slide_Guide_Z1_Up_Check, IO_OFF);
	nRet[10] = FAS_IO.get_in_bit( st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
	nRet[11] = FAS_IO.get_out_bit( st_io.o_Slide_Guide_Z1_Up_Sol, IO_OFF);
	nRet[12] = FAS_IO.get_out_bit( st_io.o_Slide_Guide_Z1_Down_Sol, IO_ON);

	nRet[13] = FAS_IO.get_in_bit( st_io.i_Slide_Guide_Z2_Up_Check, IO_OFF);
	nRet[14] = FAS_IO.get_in_bit( st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
	nRet[15] = FAS_IO.get_out_bit( st_io.o_Slide_Guide_Z2_Up_Sol, IO_OFF);
	nRet[16] = FAS_IO.get_out_bit( st_io.o_Slide_Guide_Z2_Down_Sol, IO_ON);

	//Clamp 벌림,닫힘
	nRet[17] = FAS_IO.get_out_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);
	nRet[18] = FAS_IO.get_out_bit(st_io.i_Carrier_1_Backward_Check, IO_ON);
	nRet[19] = FAS_IO.get_out_bit(st_io.o_Carrier_Clamp_1_Forward_Sol, IO_ON);
	nRet[20] = FAS_IO.get_out_bit(st_io.o_Carrier_Clamp_1_Backward_Sol, IO_ON);

	nRet[21] = FAS_IO.get_out_bit(st_io.i_Carrier_2_Forward_Check, IO_ON);
	nRet[22] = FAS_IO.get_out_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);
	nRet[23] = FAS_IO.get_out_bit(st_io.o_Carrier_Clamp_2_Forward_Sol, IO_ON);
	nRet[24] = FAS_IO.get_out_bit(st_io.o_Carrier_Clamp_2_Backward_Sol, IO_ON);


	nRet[25] = FAS_IO.get_in_bit( st_io.i_Carrier_Z_1_Up_Check, IO_OFF); //Carrier가 위에 없어야 한다.
	nRet[26] = FAS_IO.get_in_bit( st_io.i_Carrier_Z_2_Up_Check, IO_ON); //Carrier가 위에 있어야 한다.

	//비젼위치(5번)에서 지그 고정시키는 실린더
	nRet[27] = FAS_IO.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_ON);
	nRet[28] = FAS_IO.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);
	nRet[29] = FAS_IO.get_out_bit(st_io.o_Camera_Y_Jig_Press_Forward_Sol, IO_ON);
	nRet[30] = FAS_IO.get_out_bit(st_io.o_Camera_Y_Jig_Press_Backward_Sol, IO_ON);

	//지그(2,3,6) 고정시키는 실린더
	nRet[31] = FAS_IO.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);
	nRet[32] = FAS_IO.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_ON);
	nRet[33] = FAS_IO.get_out_bit(st_io.o_Press_Carrier_Holder_Up_Sol, IO_ON);
	nRet[34] = FAS_IO.get_out_bit(st_io.o_Press_Carrier_Holder_Down_Sol, IO_ON);

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
		if( Mode == 1|| Mode == 2)//밀고 난뒤 이므로 Left Carrier 감지 Right는 감지 하면 않된다.
		{
			if( nRet[25] == IO_OFF)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_1_Up_Check); 
			}
			else if( nRet[26] == IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_2_Up_Check); 
			}
			if( Mode == 1 )//밀때는 실리더가 내려와 있어야 한다
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
		else if( nRet[17] == IO_ON || nRet[18] == IO_OFF nRet[19] == IO_ON || nRet[20] == IO_OFF)
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

void CPublicFunction::OnCycleTime(int nMode, CString strLotNo, CString strPartNo, DWORD dwTime1, DWORD dwTime2, DWORD dwTime3)
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

	OnStringToChar(strCreateFile, chFileName);
	nExistence = _access(chFileName, 0);

	if (nExistence == -1)  /* 파일 미존재 */
	{
		//		strCreateFile = st_path_info.strPathCycle + strTime;
		//		strCreateFile += ".TXT";
		OnStringToChar(strCreateFile, chFileName);
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	fopen_s(&fp, chFileName, "a+");
	if(!fp)
	{
		//		AfxMessageBox(_T("The failure because we open the file."));
		return;
	}
	/* ************************************************************************** */


	strContent.Format(_T("| %-100s |"), strData);
	OnStringToChar(strContent, chMsg);
	fprintf(fp,"%s\r\n", chMsg) ;

	if (ferror(fp))  
	{
		fclose(fp); 

		return ;
	}

	fclose(fp); 
}


void CPublicFunction::OnLotCycleData(CString strLotNo, CString strPartNo, int nCount, DWORD dwTray, DWORD dwLot)
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

	strCreateFile = st_path_info.strPathCycle + strTime;
	strCreateFile += ".TXT";

	OnStringToChar(strCreateFile, chFileName);
	nExistence = _access(chFileName, 0);

	if (nExistence == -1)  /* 파일 미존재 */
	{
		strCreateFile = st_path_info.strPathCycle + strTime;
		strCreateFile += ".TXT";
		OnStringToChar(strCreateFile, chFileName);
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	fopen_s(&fp, chFileName, "a+");
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
		st_work_info.dDailyCycle = dAve;
	}
	strTemp.Format(_T("LOTNO : [%s] PARTNO : [%s] Cycle Time [Tray : %d] [Lot : %.2f]"), strLotNo, strPartNo, dwTray, dAve);
	strContent.Format(_T("| %-100s |"), strTemp);
	OnStringToChar(strContent, chMsg);
	fprintf(fp,"%s\r\n", chMsg) ;

	if (ferror(fp))  
	{
		//		Func.MsgLog("파일 저장 실패!..") ;
		fclose(fp); 

		return ;
	}

	fclose(fp); 
}

void CPublicFunction::OnDailyCycleData(CString strLotNo, CString strPartNo, double dTime, CTime tStart, CTime tEnd)
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
	strCreateFile = st_path_info.strPathCycle + strTime;
	strCreateFile += ".TXT";

	OnStringToChar(strCreateFile, chFileName);
	nExistence = _access(chFileName, 0);

	if (nExistence == -1)  /* 파일 미존재 */
	{
		strCreateFile = st_path_info.strPathCycle + strTime;
		strCreateFile += ".TXT";
		OnStringToChar(strCreateFile, chFileName);
	}

	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	fopen_s(&fp, chFileName, "a+");
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
	OnStringToChar(strContent, chMsg);
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

