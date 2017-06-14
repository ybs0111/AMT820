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

#include "Variable.h"				// ���� ���� ���� Class

// ******************************************************************************
// ��ȭ ���� Ŭ���� �߰�                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Message.h"
#include "Dialog_Select.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************

#include "math.h"
#include "io.h"			// ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� �ʿ�
#include "FAS_HSSI.h"
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰�
#include "MyBasicData.h"
#include "Cmmsdk.h"
#include "Alg_Mysql.h"
#include "Dialog_Motor_Part.h"
#include "IO_Manager.h"
#include "AMTLotManager.h"
#include "SrcPart/APartHandler.h"

CMyBasicData     mcls_p_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����

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
		/////////��ŸƮ ���۽� ���� ���� ..
		st_handler.n_MutingOn = 1;//�Ŀ����� O
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

		if (OnOff == IO_ON)		// ����̽��� ������  
		{
			if (nPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)  //���� ��Ų ������ ������ �ȴٸ� ���簡 ���ٴ� �ǹ�!!
			{		
				if (OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm�̴�.

				Result[i] = NO; 
				FuncRet = RET_ERROR;
				
				if (Robot == PICKER_LOAD)
				{
					//400200 1 40 "Loading Picker #1 Module On Check Error."
					sprintf(Jamcode, "40020%d",  i); //���簡 ������ �Ǿ��ϴµ� ������ �ȵȴ�!
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
		else if (OnOff == IO_OFF) // ����̽��� ���� �� üũ 
		{					
			if (nPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)	//���ø� ������.. (������ �׸��۰� ������ �ȵȰŴ�)
			{
				if (OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm�̴�.

				Result[i] = NO;
				FuncRet = RET_ERROR;
				
				if (Robot == PICKER_LOAD)
				{
					//400300 1 40 "Unloading Picker #1 Module Off Check Error."
					sprintf(Jamcode, "40030%d",  i); //��Ŀ�� ���ȴµ� ������ ON �Ǿ���..
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
		else					// ��Ŀ�� ���¸� �ľ��Ѵ� 
		{		
			if (nPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
			{	//����̽��� �־�� �ϴµ� ������
				if (OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm�̴�. (door�� ������ �ݾƾ� �Ѵ�!)

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

/*	OnSet_IO_Port_Sound(IO_OFF);							// Buzzer�� Off ��Ų��.

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

	OnSet_IO_Port_Sound(IO_OFF);							// Buzzer�� Off ��Ų��.

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
			if (st_lamp.mn_buzzer_mode == YES)			// Buzzer ��� ��忡���� �Ҵ�.
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
	// ����ġ �� �ε� LOT ���� �÷��� �ʱ�ȭ �κ�                                
	// **************************************************************************
	st_work.b_load_key_flag = false;		// �δ� Ű ON/OFF �÷��� �ʱ�ȭ
//	st_work.b_lot_start_flag = false;		// LOT ���� �÷��� �ʱ�ȭ
//	st_handler.n_lot_flag	= FALSE;
	st_work.b_load_off_flag = false;		// ���� Lot End�� �ƴ�.
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
	OnSet_IO_Port_Sound(IO_ON);											// Buzzer�� ON ��Ų��.
	
	if (st_work.mn_run_status != dLOTEND)		
	{
		g_ioMgr.set_out_bit(st_io.o_AlarmClear_SwitchLamp, IO_ON);
	}
	
	OnSet_Door_Open();
}

void CPublic_Function::On_LogFile_Add(int n_msg_select, CString str_msg)
{
	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // ���� ��, ��, �� ���� ���������� ��ȯ�Ͽ� ������ ���� 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // ���� ��, ��, �� ���� ���� ���� 
	CString mstr_file_name;		// ���������� ������ ���� �̸� ���� ���� 
	CString mstr_create_file;	// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString mstr_list_name, mstr_temp_data;  // �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString mstr_content;		// �� ���ο� ����� ���ڿ� ���� ���� ���� 
	COleDateTime time_cur;		// �˻��� �ð� ���� ���� ���� 
	CTime m_time_current;		// ������ ����� ���� �ð� ���� ���� ����
	char chr_buf[20];
	int mn_existence;			// ���� ���� ���� ���� �÷��� 
	char fileName[256];			// �˻��� ���� ���� ���� �Լ� 
	FILE  *fp ;					// ���Ͽ� ���� ������ ���� ���� 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret;

	if (str_msg.IsEmpty() == TRUE)  
	{
		return;
	}

	// **************************************************************************
	// ���� �̸����� ����� ��¥ ������ ��´�                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // ���� �ð� ������ ��´�. 

	m_time_current = CTime::GetCurrentTime() ;  // ������ ������ ���� �ð� ���� ��´�. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// ��¥ ������ ���������� ��ȯ�Ͽ� ������ �����Ѵ�                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// ���� �ð� ���� ��´�                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// ���� �ð� ������ �����Ѵ�. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// ���� �ð� ������ �����Ѵ�.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// ���� �� ������ �����Ѵ�. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// ���� �� ������ �����Ѵ�. 
	str_display_time = m_time_current.Format("%c");	// ����Ʈ ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ���� 
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
	case 2:							// ��� ���� ����.
		mstr_file_name = "MA" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case 3:							// ��� ����� ����.
		mstr_file_name = "DEBUG" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case 4:							// ��� ����� ����.
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

	case 99:						// ��ü �޼���.
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

		case 2:							// ��� ���� ����.
			mstr_file_name = "MA" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;

		case 3:							// ��� ����� ����.
			mstr_file_name = "DEBUG" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;

		case 4:							// ��� ����� ����.
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

		case 99:						// ��ü �޼���.
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
	// �˶� �߻� Ƚ�� ���� ���� ���Ͽ� �߰� ������ ���� ���Ϸ� ����              
	// **************************************************************************
	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		AfxMessageBox("File open failure!..");
		return;
	}
	// **************************************************************************

	// **************************************************************************
	// �α� ���Ͽ� ���� �߻��� �˶� ���� �����Ѵ�                                
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
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_op_msg = _T("File save failure.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, 7);
		}
		return ;
	}
	// **************************************************************************
	fclose(fp);  // ������ �����Ѵ�.
}

int CPublic_Function::FileSizeCheck(CString FileName, long size, int n_check)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(FileName))
	{	
		if (n_check == YES)			// Size�� üũ�ϴ°��̸�...
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
		else						// Size�� Ȯ���ϴ°��̸�...
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
// 			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
// 			{
// //				st_msg.mstr_abnormal_msg.Format("%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
// 				sprintf(st_msg.c_abnormal_msg, "%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
// 			}
// 		}
// 		
// 		if (COMI.md_spd_vel[i][2] < 1)
// 		{
// 			COMI.md_spd_vel[i][2] = 10000;
// 
// 			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "%s Motor Dec is under 1 -> 10000 Set up", Get_MotorName(i));
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
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
// 	//0�� ����	Motor No., ���� (1ȸ���� �޽� :10000 / 1ȸ���� �̵��Ÿ�)
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
// 	///1�� ���� 
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

	//���� �ʱ�ȭ.
	st_handler.n_level_teach = FALSE;
	st_handler.n_level_maint = FALSE;
	
	g_ioMgr.set_out_bit(st_io.o_Start_SwitchLamp,	IO_ON);
	g_ioMgr.set_out_bit(st_io.o_Stop_SwitchLamp,	IO_OFF);
// 	g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_ON);
// 	g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_Off_Sol, IO_OFF);

	// ��� ��ٴ�.
	g_ioMgr.set_out_bit( st_io.o_Safety_Door_Lock_Release, IO_OFF );
	
// 	sprintf(st_msg.c_normal_msg, "[Button] RUN.");
// 	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);

	Func.On_LogFile_Add( LOG_TOTAL, "[Button] RUN." );

	if(st_handler.cwnd_title != NULL)
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
	}


	st_handler.n_MutingOff = 1;//�Ŀ����� O
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
	/* �ε� ���� �÷��� �����Ѵ�                                                  */
	/* -> st_work.b_load_key_flag : �ε� �۾� ON/OFF ���� �÷���                  */
	/* -> st_work.b_lot_start_flag : LOT ���� �÷���                              */
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
	// �ε� ���� �÷��� �����Ѵ�                                                 
	// -> st_work.b_load_key_flag : �ε� �۾� ON/OFF ���� �÷���                 
	// -> st_work.b_lot_start_flag : LOT ���� �÷���                             
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
	int mn_chk_year, mn_chk_month, mn_chk_day, mn_chk_hour; // Data ���� �ð����� ���� ���� 
	COleDateTime time_cur;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // ���� �ð����� ���� ���� 
	
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
	
	// ���� �ð��� Day Start Time�� �Ǳ� ���̶��...
/*	if (mn_cur_hour < st_basic.n_day_start_time)
	{
		if (mn_chk_day == mn_cur_day)						// ����� ��¥�� ���� ��¥�� ���ٸ�...
		{
			
		}
		else												// ���� �ʴٸ� ���� Data�̴�.
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// ����� �ð��� Day Start �ð����� �����̸�.. Reset�̴�.
			{
				OnDayDataReset();							// Day Data Reset
			}
		}
	}
	else		// ���� �ð��� Day Start Time ���Ķ��...
	{
		if (mn_chk_day == mn_cur_day)						// ����� ��¥�� ���� ��¥�� ���ٸ�...
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// ����� �ð��� Day Start �ð����� �����̸�.. Reset�̴�.
			{
				OnDayDataReset();							// Day Data Reset
			}
		}
		else												// ���� �ʴٸ� ���� Data�̴�.
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
	// �޴��� ����� ��� ����.
	if (st_handler.n_mode_manual == YES)
	{
		//���� ����
		g_ioMgr.set_out_bit(st_io.o_Safety_Door_Lock_Release, IO_ON);
//		g_ioMgr.set_out_bit(st_io.o_left_door_lock_cyl, IO_OFF);
//		g_ioMgr.set_out_bit(st_io.o_right_door_lock_cyl, IO_OFF);
	}
}

void CPublic_Function::OnSet_Door_Close()
{
	//���� ��
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
				if (st_modulemap.ModuleTray[TrayNum][j][i] == YES && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_READY)		// �۾��� ����̽��� ������ 
				{		
					if (tcountmode == COUNT_NO)					// ��ġüũ�̰�, ī���� üũ�� �ƴϸ� 
					{
						if (RowColPos == TRAY_X) Num = j;		
						else Num = i;

						i = st_traybuffer.n_loader_x + 1; j = st_traybuffer.n_loader_y + 1;		// for���� ���� ������ �����̴�!
					}
					else
					{	// ī���� üũ ����̸� 
						if (RowColPos == TRAY_X) Num++;		
						else Num++;
					}
				}
			}
		*/
		// �������. 2K10/08/04/ViboX
//		for (j = 0; j < st_traybuffer.n_loader_y; j++)
/*		for (j = (st_traybuffer.n_loader_y - 1); j >= 0; j--)
		{
			for (i = (st_traybuffer.n_loader_x - 1) ; i >= 0 ; i--)
			{
				if (st_modulemap.ModuleTray[TrayNum][j][i] == YES && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_READY)		// �۾��� ����̽��� ������ 
				{		
					if (tcountmode == COUNT_NO)					// ��ġüũ�̰�, ī���� üũ�� �ƴϸ� 
					{
						if (RowColPos == TRAY_Y) Num = i;		
						else Num = j;
						
						i = -1 ; j = -1; // for���� ���� ������ �����̴�!
					}
					else
					{	// ī���� üũ ����̸� 
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
// 				if (st_modulemap.ModuleTray[TrayNum][j][i] == NO && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_NOTUSE)		// �۾��� ����̽��� ������ 
// 				{		
// 					if (tcountmode == COUNT_NO)					// ��ġüũ�̰�, ī���� üũ�� �ƴϸ� 
// 					{
// 						if (RowColPos == TRAY_X) Num = j;		
// 						else Num = i;
// 
// 						i = st_traybuffer.n_loader_x + 1; j = st_traybuffer.n_loader_y + 1;		// for���� ���� ������ �����̴�!
// 					}
// 					else
// 					{	// ī���� üũ ����̸� 
// 						if (RowColPos == TRAY_X) Num++;		
// 						else Num++;
// 					}
// 				}
// 			}
// 		}

		// �������. 2K10/08/04/ViboX
		for (j = (st_traybuffer.n_loader_y - 1); j >= 0; j--)
		{
			for (i = (st_traybuffer.n_loader_x - 1) ; i >= 0 ; i--)
			{
				if (st_modulemap.ModuleTray[TrayNum][j][i] == NO && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_NOTUSE)		// �۾��� ����̽��� ������ 
				{		
					if (tcountmode == COUNT_NO)					// ��ġüũ�̰�, ī���� üũ�� �ƴϸ� 
					{
						if (RowColPos == TRAY_Y) Num = i;		
						else Num = j;
						
						i = -1 ; j = -1; // for���� ���� ������ �����̴�!
					}
					else
					{	// ī���� üũ ����̸� 
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
				if (st_modulemap.ModuleTray[TrayNum][j][i] == NO && st_modulemap.ModuleTrayAllData[TrayNum][j][i] == MODULE_NOTUSE)		// �۾��� ����̽��� ������ 
				{		
					if (tcountmode == COUNT_NO)					// ��ġüũ�̰�, ī���� üũ�� �ƴϸ� 
					{
						if (RowColPos == TRAY_X) Num = j;		
						else Num = i;
						
						i = st_traybuffer.n_loader_x + 1; j = st_traybuffer.n_loader_y + 1;		// for���� ���� ������ �����̴�!
					}
					else
					{	// ī���� üũ ����̸� 
						if (RowColPos == TRAY_X) Num++;		
						else Num++;
					}
				}
			}
		}
		break;
	}
	
	if (tcountmode == COUNT_NO)  //ī���͸� ���� �ʰ�, ��ġ�� ã�� ����̸� 
	{	
		if (Num < 0)
		{
			FuncRet = RET_NOWHERE;		//�ش� ������ ��ġ�� ã�� ���ߴ�..
		}
		else FuncRet = Num;
	}
	else
	{	// ī���͸� üũ 
		if (Num < 0)
		{
			FuncRet = 0;				//�ش� ������ ���� ������ 0 (����)
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
		// �۾��� ù��° ��Ŀ�� �����ϴ� ��
		for (i = (SORTER_PICKER_NUM - 1) ; i >= 0 ; i--)	// ������. 2K10/08/04/ViboX
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

		if (InfoSetFlag == INFO_NO) break; //row,col ������ ���������� �ѱ��� ����

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
		//�۾��� ù��° ��Ŀ�� �����ϴ� ��
// 		for (i = 0 ; i < SORTER_PICKER_NUM ; i++) 
// 		{
// 			if (PickerInfo[D_EXIST][i] == DVC_YES && PickerInfo[D_BIN][i] == BinNum) // ����̽��� ����������� ��Ŀ��ȣ (�������� ã�°Ŵϱ�..)
// 			{
// 				FirstPicker = i;
// 				st_work.n_FirstPickerNum[UNLOADTRAY] = FirstPicker;
// 				break;
// 			}
// 		}

		for (i = (SORTER_PICKER_NUM - 1) ; i >= 0 ; i--)	// ������. 2K10/08/04/ViboX
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
		
		if (InfoSetFlag == INFO_NO) break; //row,col ������ ���������� �ѱ��� ����
		
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
			if (PickerInfo[D_EXIST][i] == DVC_YES && PickerInfo[D_BIN][i] == BinNum) // ����̽��� ����������� ��Ŀ��ȣ (�������� ã�°Ŵϱ�..)
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
		
		if (InfoSetFlag == INFO_NO) break; //row,col ������ ���������� �ѱ��� ����
		
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
		//�����̶�� �̹� ����Ȱ� �����״� �ϴ� skip����~~
		::WritePrivateProfileString("TempLotInfor", "Load_Device", "0", st_path.str_tempLotInfor);
		
		str_temp_data = st_work.m_lot_end_time.Format("%Y%m%d%H%M%S");
		::WritePrivateProfileString("TempLotInfor", "Finish_Time", str_temp_data, st_path.str_tempLotInfor);
	}
	else if (iMode == 3)
	{
		::WritePrivateProfileString("TempLotInfor", "Load_Device", "0", st_path.str_tempLotInfor);
		
		// ���� Lot�� ���ؼ� �ӽ÷� ����ð��� �����ϱ� ����..
		temp_endtime = CTime::GetCurrentTime();
		
		str_temp_data = temp_endtime.Format("%Y%m%d%H%M%S");
		::WritePrivateProfileString("TempLotInfor", "Finish_Time", str_temp_data, st_path.str_tempLotInfor);
	}
	else if (iMode == 4)
	{
		//�۾��� ������ ������ ���������� �������� �ȵǸ� PGM���� ���۽� ������ �ؾ��Ѵٴ� ���� : Load_Device = 1
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
	
//j	if (st_handler.n_testresult_save != 1)		return;						// 1�� �ƴϸ� ���� �ع�����.
	
	tTime = CTime::GetCurrentTime();
	
	mMessage = tTime.Format("%Y%m%d"); //��,��,��
	
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
	int     nLength;  // ������ �� ���� ���� ���� 
	CString strBody;  // BODY ������ ���� ���� 
	
//	char  cp_cmd[1024];
	
//	memset(&cp_cmd, 0, sizeof(cp_cmd));  // ������ ���� ���� �ʱ�ȭ 
	
	// **************************************************************************
	// ���� ������ �ּ� ���� �˻��Ѵ� [�ּ� 3�ڸ� �̻�]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if (nLength<3)
		return FALSE;
	// **************************************************************************
	
	strBody = strCheckData.Mid(1, nLength-2);  // [���+����] ���� 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	// **************************************************************************
	// ���� �޽��� ���� ���� �˻��Ѵ�                                            
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

		//üũ�ÿ��� �׻� OFF�̴�.
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
			if(nBufferMode == 0)	 //���������� üũ�� ���� ������ �������� ���� ���� ���� Ȯ�� 
			{
				nCurrent_BufferInfo[0] = st_buffer_info[nSite].st_pcb_info[i].nYesNo;
			}
			else if(nBufferMode == 1) //�Լ� ����(�������) ����Ͽ� ���� ���� ���� Ȯ��  
			{
				nCurrent_BufferInfo[0] = nBufferInfo[i];
			}
		}
	}
	else if(nSite == THD_LD_HSALIGN_BUFF) 
	{
		nPara_Num = st_recipe.nLdHsAlignBuffer_Num;

		nIO_Num_Info[0] = st_io.i_HeatSink_Transfer_CellIn_Check; 
		
		nIO_Output_Num_Info[0] = st_io.o_HeatSink_Reverse_180_Sol; //ȸ���� �ȵ��� ���
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

		nIO_Output_Num_Info[0] = st_io.o_HeatSink_Reverse_180_Sol; //����̽��� ���� ������ �������� ��� 
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
	{//�̰����� ���� �ȵ� 
		return RET_ERROR;
	}

	for(i = 0; i < nPara_Num; i++)
	{
		nBufferStatus[i] = g_ioMgr.get_in_bit(nIO_Num_Info[i],	IO_ON);  
		npBufferOutputStatus[i] = g_ioMgr.get_out_bit(nIO_Output_Num_Info[i],	IO_ON);
	}	

	if(st_basic.n_mode_device == WITHOUT_DVC) //with device �� �ƴϸ� 
	{
		for(i = 0; i < nPara_Num; i++)
		{
			if(nBufferMode == 100) //������ ���簡 ��� ����� �Ѵ� 
			{
				if((nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) || st_buffer_info[nSite].st_pcb_info[i].nYesNo == CTL_YES)
				{  				 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
					return nFuncRet;
				}
				continue;
			}

			if(nYesNo == CTL_YES) //����̽��� �ִ� ���¸� üũ�� 
			{
				if(nCurrent_BufferInfo[i] == CTL_YES)
				{
					if(npBufferOutputStatus[i] == IO_OFF)
					{
						nBufferStatus[i] = IO_ON;
					}
					else
					{
						nBufferStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó�� 
						if(nSite == THD_LD_ALIGN_BUFF) //Dispenbuffer(B����)�� �ݴ� 
						{
							if( st_basic.n_mode_device == WITHOUT_DVC )
							{
								if( nYesNo == CTL_YES) nBufferStatus[i] = CTL_YES;
							}
						}

					}
				}
			}
			else //����̽��� ���� ���¸� üũ�� 
			{
				if(nCurrent_BufferInfo[i] == CTL_YES) 
				{
					if(npBufferOutputStatus[i] == IO_ON)//james 2016.0919 �׸��� ���� ���� 
					{
						nBufferStatus[i] = IO_ON; // open�ϰ� �־� ���� ó���� 
					}
					else
					{
						nBufferStatus[i] = IO_OFF; //clamp �ϰ� �ִ� ���·� ������ �־� ����ó�� 
					}
				}
			}
		}
		return RET_GOOD;
	}

	//with device
	for (i = 0; i < nPara_Num; i++) 
	{
		if(nBufferMode == 100) //������ ���簡 ��� ����� �Ѵ� 
		{
			if((nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) || st_buffer_info[nSite].st_pcb_info[i].nYesNo == CTL_YES)
			{  				 
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
				nFuncRet = RET_ERROR;
				return nFuncRet;
			}
			continue;
		}

		if(nYesNo == CTL_YES)	//����̽��� �ִ� ���¸� üũ�� �� 
		{
			if(nCurrent_BufferInfo[i] == CTL_YES) //CTL_YES: ��Ŀ ������ ����̽��� �ִ�, CTL_NO:��Ŀ ������ ����̽��� ����
			{			

				if(nBufferStatus[i] == IO_OFF || npBufferOutputStatus[i] == CTL_NO)
				{	//error   //���� �ȵ� ���� 					
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
				}
			}
			else //if((nCurrent_PickerInfo[i] == CTL_NO) //CTL_YES: ��Ŀ ������ ����̽��� �ִ�, CTL_NO:��Ŀ ������ ����̽��� ����
			{//����̽��� ����� �Ѵ� 
				if(nBufferMode == 0) //���� ������ ���¸� üũ�ϴ� ���۽� ��� 
				{
					if(nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) //james 2016.0918 �߰� nIO_Output_Num_Info[i] 
					{	 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
			}
		}
		else if(nYesNo == CTL_NO)	//������� ����,. ����̽��� ���� ���¸� üũ�� ��,  
		{
			if(nCurrent_BufferInfo[i] == CTL_YES) //CTL_YES: ��Ŀ ������ �۾��ϴ� �����̴�, CTL_NO:��Ŀ ������ �۾����� �ʴ� �����̴�
			{ 
				if(nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) //james 2016.0919 (npPickerStatus[i] == IO_OFF) //james 2016.0918 CTL_NO)
				{	//error   //���� �ȵ� ���� 
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
				}
			}
			else //if((nCurrent_BufferInfo[i] == CTL_NO) //CTL_YES: ��Ŀ ������ ����̽��� �ִ�, CTL_NO:��Ŀ ������ ����̽��� ����
			{//����̽��� ����� �Ѵ� 
				if(nBufferMode == 0)//���� ������ ���¸� üũ�ϴ� ���۽� ��� 
				{
					if(nBufferStatus[i] == IO_ON && npBufferOutputStatus[i] == IO_OFF) //james 2016.0919 �߰� nIO_Output_Num_Info[i] 
					{	//error   //���� �ȵ� ���� 
						//vacuum on/off �� �ٷ� üũ 
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
	if (nStep == st_work.mn_prev_step[nPos]) // step�� �����ϱ�~
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
		case BAR_READ_MODE :  // READ ���
			str_command = "11";
			break;

		case BAR_PERCENT : // �ݺ� READ ���
			/* ********************************************************************** */
			/* ���� ���� READ ���                                                    */
			/* -> �� ��� ���� �� Ư���� ���� �ɼ� ������ �Ǿ�����                    */
			/* ********************************************************************** */
			str_command = "12";
			break;

		case BAR_SETTING_MODE :  // ���� ��� ��ȯ
			str_command = "13";
			break;

		case BAR_SELF_TEST :  // SELF TEST
			str_command = "15";
			break;

		case BAR_ADJUST_MODE :  // SCAN ���� ���� ���� Ȯ��
			str_command = "1J";
			break;

		case BAR_SHOW_LIMIT :  // SCAN ���� �̰��� ���� Ȯ��
			str_command = "1K";
			break;

		case BAR_READ_ON :  // SWITCH ON BAR_FLY_MODE
			str_command = "21";
			break;

		case BAR_READ_OFF :  // SWITCH OFF
			str_command = "22";
			break;

		case BAR_FULL_INFO :  // ���� ������ ���� [���� ������]
			str_command = "3BT2";
			break;

		case BAR_MINI_INFO :  // �ּ� ������ ���� [1�� ������]
			str_command = "3BT3";
			break;

		case BAR_SET_DEFAULT :  // ����Ʈ ����
			str_command = "3DF";
			break;

		case BAR_SET_SWITCH :  // READ �ñ� ����
			/* ********************************************************************** */
			/* TRIGGER READING GATE�� TIME OUT���� �����ϴ� ���                      */
			/* -> SWITCH ON/OFF�� READING �ñ� �����Ѵ�                               */
			/*  : �⺻������ TIME OUT READING GATE �ֱ�� 1000ms�� �����ȴ�           */
			/*  : �߸��� TIME OUT �ֱ� ���� �� �⺻���� �����ǰ� �ȴ�                 */
			/*  : str_command = "3LTM3T050E0C0Z0100A0B4BS49F0L1D0010X0001"            */
			/*  : M = 1 : SENSOR INPUT(ACTIVE HIGH)                                   */
			/*  :     2 : SENSOR INPUT(ACTIVE LOW)                                    */
			/*  :     3 : SERIAL INTERFACE                                            */
			/*  :     4 : FREE RUNNING WITH TIMEOUT                                   */
			/*  :     8 : REFLECTOR POLING                                            */
			/*  : C = 0 : TRIGGER OFF �Ǵ� BARCODE ���������� READ ��� ���          */
			/*  :     1 : TIME OUT �Ⱓ ���� READ ��� ���                           */
			/*  : Z = 0001 ~ 9999 : 1000ms = 1SEC  (TIME OUT �ð� ����, 0001 = 10ms)  */
			/* ********************************************************************** */
			// TIME OUT ���� ��ȣ������ READING GATE ����
			str_command = "3LTM3C0";
			break;

		case BAR_RS232 :  // RS-232C �������̽� ���� ����
			/* ********************************************************************** */
			/* RS-232C �������̽� ���� �����Ѵ�                                     */
			/*  : str_command = "3HSB6D1S1H11Z0E0"                                    */
			/*  : 3HS [ȣ��Ʈ �������̽� ���� ���]                                   */
			/*  : B6  [BAUD RATE ���� : 9600]                                         */
			/*  : D1  [DATA BITS + PARITY ���� : 8bits]                               */
			/*  : S1  [STOP BIT ���� : 1bit]                                          */
			/*  : H11 [��� ��� ���� : 11 (RS232),  01 (RS422/485)]                  */
			/* ********************************************************************** */
			str_command = "3HSH11";
			break;

		case BAR_RS422 :  // 422/485 �������̽� ���� ����
			/* ********************************************************************** */
			/* 422/485 �������̽� ���� �����Ѵ�                                     */
			/*  : str_command = "3HSB6D1S1H11Z0E0"                                    */
			/* ********************************************************************** */
			str_command = "3HSH01";
			break;

		case BAR_VOLUME :  // BEEP ���� ����
			/* ********************************************************************** */
			/* BEEP ������ �����Ѵ�                                                   */
			/* str_command = "3ROA11W0B00X0G00V0L00000000O00000000M00N0P00Q0T10R0S0"  */
			/*  : V  [0:OFF    1:LOW    2:HIGH]                                       */
			/*  : S1 [STOP BIT ���� : 1bit]                                           */
			/* ********************************************************************** */
/*			if (n_option == 1)      // ���� : LOW
				str_command = "3ROV1";
			else if(n_option == 2)  // ���� : HIGH
				str_command = "3ROV2";
			else                    // ���� : OFF
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
		case BAR_CODE_1 :  // CODABAR �ڵ� ��� ����
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
		case BAR_CODE_2 :  // 39 �ڵ� ��� ����
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
		case BAR_CODE_3 :  // UPC ��� ��� ����
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
		case BAR_CODE_4 :  // EAN �ڵ� ��� ����
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
		case BAR_CODE_5 :  // 2/5-Interleaved �ڵ� ��� ����
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
		case BAR_CODE_6 :  // 39 �ڵ� ��� ����
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
		case BAR_CODE_7 :  // 128 �ڵ� ��� ����
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
		case BAR_CODE_8 :  // EAN 128 �ڵ� ��� ����
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
		str_ret.Format("%c%s%c", 0x02, str_command, 0x03);			// ���� : [STX+��ɾ�+ETX] ���·� ��ȯ

		st_serial.str_snd[DEVICE_BARCODE_PORT-1] = str_ret;                    // �۽� ������ ����
		::SendMessage(st_handler.hWnd, WM_DATA_SEND, DEVICE_BARCODE_PORT, 0);  // ������ �۽� ��û
	}
	else 
	{
		st_serial.comm_err[DEVICE_BARCODE_PORT-1] = _T("[�۽� �޽���] ���� ���� ���� �߻�");
	}

	return str_ret;
}

int CPublic_Function::OnBarcode_Receive(int n_picker, int n_command, CString str_rev)
{
	CString str_body;  /* BODY ������ ���� ���� */
	CString str_temp_data_ea;
	int     n_length;  /* ������ �� ���� ���� ���� */
	int     n_start_index, n_end_index;  /* BCR ������ ���� �� ���� ��ġ ���� ���� */
	int		first_carr_position = 0, final_carr_position = 0;
	int     n_receive_data_ea = 0, n_test_no = 0;
	int		n_pos[5];
//	char cp_cmd[BUF_SIZE];  // �˻��� ������ �ӽ� ���� ����

//	memset(cp_cmd, 0, sizeof(cp_cmd));  // ������ ���� ���� �ʱ�ȭ

	/* ************************************************************************** */
	/* ���� ������ �ּ� ���� �˻��Ѵ� [�ּ� 3�ڸ� �̻�]                           */
	/* ************************************************************************** */
	n_length = str_rev.GetLength();
	if(n_length<3)
	{
		return BAR_ERR;
	}

	str_body = str_rev.Mid(1, n_length-2);  // ���+TAIL ���� ������
	str_body.TrimLeft(' ');
	str_body.TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ���� ������ ���� ���� �˻��Ѵ�                                             */
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
	/* ���� ������ ��� �˻��Ѵ�                                                  */
	/* ************************************************************************** */
	if(cp_cmd[0] != 0x02)
	{
		st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
		st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

		return BAR_ERR;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ���� ������ TAIL �˻��Ѵ�                                                  */
	/* ************************************************************************** */
	if(cp_cmd[n_length-1] != 0x03)
	{
		st_serial.n_rec_chk[DEVICE_BARCODE_PORT-1]	= FALSE;
		st_serial.str_rec[DEVICE_BARCODE_PORT-1]	= "";

		return BAR_ERR;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ���� ������ ���� ���� �˻��Ѵ�                                             */
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

			if(n_pos[0] > 0 || n_pos[1] > 0)  // READ�� BCR ������ ����
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
					n_start_index = str_body.Find(10, 0)+1;				// BCR ���� ��ġ ���� [LF]
					n_end_index = str_body.Find(10, n_start_index)-1;	// BCR ���� ��ġ ���� [LF]

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
	if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
	{
		sprintf(st_msg.c_normal_msg, strMsg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
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
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���, CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���, CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���, CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Trasnfer_Pad_Up_Sol; //��¿�� ���� -_-;;
			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���, CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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

			if(nPickerMode == 0)	 //���������� üũ�� ���� ���� Ȯ��, ��ü ��Ŀ ������ ���� ���¸� üũ�Ѵ�  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���,  CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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
			
			if(nPickerMode == 0)	 //���������� üũ�� ���� ���� Ȯ��, ��ü ��Ŀ ������ ���� ���¸� üũ�Ѵ�  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���,  CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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
			
			if(nPickerMode == 0)	 //���������� üũ�� ���� ���� Ȯ��, ��ü ��Ŀ ������ ���� ���¸� üũ�Ѵ�  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���,  CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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

			if(nPickerMode == 0)	 //���������� üũ�� ���� ���� Ȯ��, ��ü ��Ŀ ������ ���� ���¸� üũ�Ѵ�  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���,  CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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
			nIO_Num_Info[i] = st_io.i_Dispenser_Device_Check;//B���� //st_io.i_HeatSink_Transfer_Vacuum_Check;
			nIO_Output_Num_Info[i] = st_io.o_HeatSink_Vacuum_On_Sol;

			if(nPickerMode == 0)
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���,  CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
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

			if(nPickerMode == 0)	 //���������� üũ�� ���� ���� Ȯ��, ��ü ��Ŀ ������ ���� ���¸� üũ�Ѵ�  
			{
				nCurrent_PickerInfo[i] = st_picker[nSite].st_pcb_info[i].nYesNo;
			}
			else //if(nPickerMode == 1) //�Լ� ����(�������) ���,  CTL_YES�� �κи� ��Ũ �������� üũ�Ѵ� 
			{
				nCurrent_PickerInfo[i] = npPickerInfo[i];
			} 
		}  
	}
	else 
	{//�̰����� ���� �ȵ� 
		return RET_ERROR;
	}
	
	for(i = 0; i < nPara_Num; i++)
	{
		npPickerStatus[i]       = g_ioMgr.get_in_bit( nIO_Num_Info[i], IO_ON);
		npPickerOutputStatus[i] = g_ioMgr.get_out_bit(nIO_Output_Num_Info[i], IO_ON);
	}

	if(st_basic.n_mode_device == WITHOUT_DVC) //with device �� �ƴϸ� 
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
						}
					}
				}

			}
			else //����̽��� ���� ���¸� üũ�� 
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
							npPickerStatus[i] = IO_OFF; //clamp �ϰ� �ִ� ���·� ������ �־� ����ó�� 
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
							npPickerStatus[i] = IO_OFF; //clamp �ϰ� �ִ� ���·� ������ �־� ����ó�� 
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
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
							npPickerStatus[i] = IO_OFF; //Vacuum�� OFF�̹Ƿ�  without �̹Ƿ� �׻� B�����̴� �׻� ON
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
							npPickerStatus[i] = IO_OFF; //open �ϰ� �ִ� ���·� ������ �־� ����ó��
						}
					}
				}
			}
		}
		return RET_GOOD;
	}

	for (i = 0; i < nPara_Num; i++) 
	{
		if(nYesNo == CTL_YES)	//����̽��� �ִ� ���¸� üũ�� �� 
		{
			if(nCurrent_PickerInfo[i] == CTL_YES) //CTL_YES: ��Ŀ ������ ����̽��� �ִ�, ��Ŀ�� forwrad ����
			{			
				if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
				{	//error   //���� �ȵ� ���� 	//�Ǹ����� OFF(backward) 					
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
					nFuncRet = RET_ERROR;
				}
			}
			else //if((nCurrent_PickerInfo[i] == CTL_NO) //CTL_YES: ��Ŀ ������ ����̽��� �ִ�, CTL_NO:��Ŀ ������ ����̽��� ����
			{//����̽��� ����� �Ѵ� 
				if(nPickerMode == 0) //���� ������ ���¸� üũ�ϴ� ���۽� ��� 
				{
					if(nSite == THD_LOAD_WORK_RBT )
					{
						if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_HEATSINK_RBT )
					{
						if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_HEATSINK_PRBT )
					{//if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)
						if(npPickerStatus[i] == IO_ON && npPickerOutputStatus[i] == IO_ON)//2017.0420
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_UNLOAD_WORK_RBT)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_LD_HEATSINK_BUFF)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PICK_HEATSINK_DVC)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if( nSite == THD_PLACE_HEATSINK_DVC)
					{
						if(npPickerStatus[i] == IO_ON /*B����*/|| npPickerOutputStatus[i] == IO_ON)
						{	//error   //�����ʵ� ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if( nSite == THD_PLACE_CARRIER_DVC)
					{
						if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
						{	//error   //������ ���� 	//�Ǹ����� ON(forward)
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
				}
			}
		}
		else if(nYesNo == CTL_NO)	// ����̽��� ���� ���¸� üũ�� ��,  
		{
			if(nCurrent_PickerInfo[i] == CTL_YES) //CTL_YES: ��Ŀ ������ �۾��ϴ� �����̴�, CTL_NO:��Ŀ ������ �۾����� �ʴ� �����̴�
			{
				if(nSite == THD_LOAD_WORK_RBT)
				{ 
					if(npPickerStatus[i] == IO_ON || npPickerOutputStatus[i] == IO_ON)
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_HEATSINK_RBT)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_HEATSINK_PRBT)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_UNLOAD_WORK_RBT)
				{
					if(npPickerStatus[i] == IO_ON)
					{	//error   //���� �� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_LD_HEATSINK_BUFF)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if(nSite == THD_PICK_HEATSINK_DVC)
				{
					if(npPickerStatus[i] == IO_OFF)
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if( nSite == THD_PLACE_HEATSINK_DVC)
				{
					if(npPickerStatus[i] == IO_ON)//B����
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}
				else if( nSite == THD_PLACE_CARRIER_DVC)
				{
					if(npPickerStatus[i] == IO_ON)
					{	//error   //���� �ȵ� ���� 
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, nIO_Num_Info[i]); //on check error 
						nFuncRet = RET_ERROR;
					}
				}	
			}
			else //if((nCurrent_PickerInfo[i] == CTL_NO) //CTL_YES: ��Ŀ ������ ����̽��� �ִ�, CTL_NO:��Ŀ ������ ����̽��� ����
			{//����̽��� ����� �Ѵ� 
				if(nPickerMode == 0)//���� ������ ���¸� üũ�ϴ� ���۽� ��� 
				{
					if(nSite == THD_LOAD_WORK_RBT)
					{ 
						if(npPickerStatus[i] == IO_OFF && npPickerOutputStatus[i] == IO_OFF)
						{	//error   //���� �ȵ� ���� 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_UNLOAD_WORK_RBT)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //���� �ȵ� ���� 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_LD_HEATSINK_BUFF)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //���� �ȵ� ���� 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PICK_HEATSINK_DVC)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //���� �ȵ� ���� 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PLACE_HEATSINK_DVC)
					{//npPickerStatus[i] ����̽��� Dispensor buffer�� ���Ҵ� B����
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //���� �ȵ� ���� 
							m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, nIO_Num_Info[i]); //on check error 
							nFuncRet = RET_ERROR;
						}
					}
					else if(nSite == THD_PLACE_CARRIER_DVC)
					{
						if(npPickerStatus[i] == IO_OFF || npPickerOutputStatus[i] == IO_OFF)
						{	//error   //���� �ȵ� ���� 
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
// tray & Buffer �� �۾� ������ ������ ������ ã�´� 
//////////////////////////////////////////////////////////////2016.0814 
int CPublic_Function::Find_TrayWork_Pos(int nMode, int nSite, int nDVCYesNO, int *npFindPosYXCPB, int nRobotSite, CString str_LotNo) //first tray Y, first tray X, first picker num, first buff num, �۾� ���ǿ� �����ϴ� �� ���� �Ǵ� ���� ���� ��ü ����
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
		case THD_LD_TRAY_PLATE: //load olate tray ������ ����
			nFirstY = 0; //st_recipe_info.nTrayY;
			nFirstX = 0;	
			for(y = nFirstY; y < st_recipe.nTrayY; y++) //Y ���� ����
			{
				for(x = nFirstX; x < st_recipe.nTrayX; x++) //X ���� ���� 
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
						npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
					}
				}
			}
			break;

		case THD_LD_ALIGN_BUFF: //�ε� ���۴� test robot�� �۾��ϱ�� ������  test site�� ���� ������ ���� ���� �� ��ġ�� ��� ���ΰ� �����ȴ� 
			nFirstY = 0;  nFirstX = 0;	
			for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
			{
				if(st_buffer_info[nSite].st_pcb_info[i].nYesNo == nDVCYesNO )// && st_buffer_info[nSite].st_pcb_info[i].nEnable == CTL_YES
					//&& (st_buffer_info[nSite].st_pcb_info[i].strLotNo == str_LotNo || str_LotNo == "0") ) //nDVCYesNO == CTL_NO ���ǿ� ������ ������ üũ, ��밡���� ���Ͽ��� �۾�����
				{
					if(nFlag == CTL_NO)
					{
						npFindPosYXCPB[0] = i; npFindPosYXCPB[1] = i;
						nFuncRet = RET_GOOD;
						nFlag = CTL_YES;
					}
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
				}
			}
			break;

		case THD_LDULD_CARRIER_BUFF:

			for ( i = 0 ; i < st_recipe.nCarrierBuffer_Num; i++ )//TOP1 MID 2 BTM 3
			{
				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_skip_flag[i] != CTL_YES)
				{
					//if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE].n_idbuffer[i] != CTL_YES )//���߿� �ٲ���Ѵ�
					//{
						if( nDVCYesNO == CTL_NO )//����̽� Place
						{
							if( nRobotSite == THD_LOAD_WORK_RBT && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == nDVCYesNO )
							{
								if(nFlag == CTL_NO)
								{
									npFindPosYXCPB[0] = 0; npFindPosYXCPB[1] = i;//Y���� ��ġ
									nFuncRet = RET_GOOD;
									nFlag = CTL_YES;
								}
								npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 	
							}
						}
						else//����̽� Pick //if( nSite == THD_UNLOAD_WORK_RBT && nDVCYesNO == CTL_YES )
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
								npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 	
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
				for(y = nFirstY; y < st_recipe.nHsTrayY; y++) //Y ���� ����
				{
					for(x = nFirstX; x < st_recipe.nHsTrayX; x++) //X ���� ���� 
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
							npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
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
					npFindPosYXCPB[2]++; //�۾� ������ count ���� üũ 			
				}
			}
			break;

	}

	return nFuncRet;
}

int CPublic_Function::Check_Robot_Buffer_Clash( int nRbtTHDSite, int BuffTHDSite, int nBuffStartPos, int nPickerStartPos, int *nErrorInfoStatus) //��Ŀ�� ������ �۾� ��ġ�� ������ �۾� ���� ���� Ȯ�� �Ѵ�
{//int nBuffStartPos �� �ǹ̴� Buffer, Test site socket�� ��Ŀ���� ���۵��� ���� ��ȣ�̴� 
	int nFuncRet = RET_GOOD;
	int i, nRet[4]={0,};
	int nPara_Num = st_recipe.nCarrierBuffer_Num;
	int nPos=0;
	int nFisrtPicker=0;

	for(i = 0; i < nPara_Num; i++) nErrorInfoStatus[i] = CTL_NO; //clear 
	 			
	if(nRbtTHDSite == THD_LOAD_WORK_RBT)
	{			
		for(i = 0; i < nPara_Num; i++) //nBuffStartPos�� �������� ��ġ��ȣ ū�ʿ� ���簡 ���������� ��Ŀ ���� �����̴� üũ
		{
			nErrorInfoStatus[i] = CTL_NO; //���� ���� Ŭ���� 				

			if(BuffTHDSite == THD_LD_ALIGN_BUFF) //���縦 only ���� �۾��� �Ѵ�, �۾� ������ 0->1->2->3 ���� �ڿ��� ���� ���´�
			{					
				if(nBuffStartPos > i) continue;
				if(st_buffer_info[BuffTHDSite].st_pcb_info[i].nYesNo == CTL_YES)  
				{						  
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON,st_io.i_Loading_Tr_Jig_Detect_Check);//error 
					//���� , ������ ����̽��� ��Ŀ�� ���簡 �浹 ��
					nErrorInfoStatus[i] = CTL_YES; //���� ���� ����
					nFuncRet = RET_ERROR; 
				} 
			}
			else if(BuffTHDSite == THD_LDULD_CARRIER_BUFF)
			{
				if(nBuffStartPos > i) continue; //���� ��ȣ ������ �۾��� �������� ū ��ġ ������ ���� ������ �浹�ϴ� // �����ؾ� �� 
				if(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == CTL_YES)  
				{	//���� , ������ ����̽��� ��Ŀ�� ���簡 �浹 ��
					nErrorInfoStatus[i] = CTL_YES; //���� ���� ����
					m_strAlarmCode.Format(_T("8%d500%d"), IO_ON,st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i]);//error 
					nFuncRet = RET_ERROR; 
				} 
			}
		}			
	}//üũ �ʿ�
	else if(nRbtTHDSite == THD_UNLOAD_WORK_RBT) //�κ�Ʈ ��Ŀ ����� , ����/�׽�Ʈ ����Ʈ�� ��ȣ ������ �����ϴ� 
	{
		nFisrtPicker=0;
		for(i = 0; i < nPara_Num; i++)
		{
			if(BuffTHDSite == THD_ULD_ALIGN_BUFF)
			{
				if(nBuffStartPos > i) continue; //���� ��ȣ ������ �۾��� �������� ū ��ġ ������ ���� ������ �浹�ϴ� // �����ؾ� �� 
				if(st_buffer_info[BuffTHDSite].st_pcb_info[i].nYesNo == CTL_NO)  
				{	//815100 0 A "UNLOAD_BUFFER_DVC_EXIST_CHECK_ERROR."
					nErrorInfoStatus[i] = CTL_YES; //���� ���� ����
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON,st_io.i_Unloader_Transfer_Device_Check);//error 
					nFuncRet = RET_ERROR; 
				} 
			}
		}//for
	}
	return nFuncRet;
}
////////////////////////////////////////////////////////////
// �κ�Ʈ Pos�� ã�´� 
////////////////////////////////////////////////////////////
int CPublic_Function::Calculate_MovePos_Find(int nMode, int nMotNum, int nSite, int nWorkPart, int *npFindWorkPosYXCPB, double *dpGetTargetPos)
{
	//n_workpos[2]:[0]:x, [1]:y , n_refnum = �۾� ������ ��Ŀ ��ȣ, �۾���ġ ���� ��ȣ   
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

	//limit �� ���� ���� 
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


int CPublic_Function::Data_Exchange_PickPlace(int nPickPlace, int nJobCnt, int nRobotSite, int nWorkSite, int *npWorkPos)//Pick & Place�� picker & Tray data exchange �۾�
{
	//int nJobCnt =�ѹ��� ó���� data �۾�����, int nRobotSite//�κ�Ʈ����Ʈ����, int nWorkSite//Ʈ���̻���Ʈ����, int *npRobotPos//picker����, int *npWorkPos//Ʈ���� ����
	//nPickPlace = 0:pick mode, 1:place mode
	CString strTemp;
	if(nPickPlace == PICKER_PICK_MODE) //Ʈ���̼� ���縦 ���� ���� //TRAY -> robot pick;
	{
		if(nRobotSite == THD_LOAD_WORK_RBT)
		{
			if(nWorkSite == THD_LD_TRAY_PLATE) //Ʈ���̿��� ���� ���� ����
			{//��Ŀ�� ������ ������ ����, Ʈ���̴� ������ Ŭ���� �ȴ� 		

			}			
			else if(nWorkSite == THD_LD_ALIGN_BUFF) //Ʈ���̿��� ���� ���� ����
			{//��Ŀ�� ������ ������ ����, Ʈ���̴� ������ Ŭ���� �ȴ� 

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
					st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//����� ��� 
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
				//test site�� ��� st_picker[nRobotSite].st_pcb_info[npRobotPos[0]].nEnable = st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].nEnable;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nYesNo = CTL_NO;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBin = -1;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nRetestCnt = 0;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nRetest = 0;
				st_tray_info[nWorkSite].st_pcb_info[npWorkPos[1]][npWorkPos[0]].nBinNum = -1;//����� ��� 
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
				//test site�� ��� st_tray_info[nWorkSite].st_pcb_info[npWorkPos[0]][npWorkPos[1]].tStart;	
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//����� ��� 
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
		}  //work robot �۾��Ϸ�
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;//����� ��� 
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum				= st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBinNum;//����� ��� 
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
				st_tray_info[nWorkSite].st_dvc_info[npWorkPos[4]][npWorkPos[1]][npWorkPos[0]].nBinNum = -1;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum		  = st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum;//����� ��� 
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//����� ��� 
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum = -1;//����� ��� 
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
	else if(nPickPlace == PICKER_PLACE_MODE) //���縦 ���� ���۽� ��� , ��Ŀ���� Ʈ���̿� ���´�  //robot pick -> TRAY 
	{
		if(nRobotSite == THD_LOAD_WORK_RBT)
		{
			if(nWorkSite == THD_LD_ALIGN_BUFF) //�ε� ���ۿ� ���縦 ���´� 
			{				 	
			}
			else if(nWorkSite == THD_LDULD_CARRIER_BUFF)//���縦 ���´� 
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//����� ��� 
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
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBinNum[npWorkPos[1]]				= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//����� ��� 
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
			{//��Ŀ�� ������ Ŭ����ǰ�, Ʈ���̴� ������ ������  	
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
				st_buffer_info[nWorkSite].st_pcb_info[npWorkPos[0]].nBinNum			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//����� ��� 
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
// 				st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].nBinNum[npWorkPos[1]]			= st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//����� ��� 
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
				st_picker[nRobotSite].st_pcb_info[npWorkPos[3]].nBinNum = -1;//����� ��� 
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
// ��� ����Ʈ�� Ʈ���� �� ���� ������ ����Ʈ �����Ѵ�  
////////////////////////////////////////////////////////////////////////
int CPublic_Function::Handler_Tray_DataInfo_Shift(int nMode, int nDvc_Yes_No, int nSend_SIte, int nRcv_Site)
{
	int x=0, y=0;
	 
	//nMode == 0 : ������ �����ϰ� ������ ���� , send data�� ������ �ʴ´�. 
	//nModw == 1 : ������ ����Ʈ �� send data�� ��� Ŭ����
	//nMode == 2 : ���� �۾��ϴ� lot�� ������ �־��ش�. ����̽� ������ nDvc_Yes_No ������ ���� ������ �Ѵ� 
	//nMode == 3 :  nSend_SIte data �� ��� Ŭ����

	if(nMode == 0) // nMode == 0 : ������ �����ϰ� ������ ����, send data�� ������ �ʴ´�. 
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
		st_tray_info[nRcv_Site].nLastModule = st_tray_info[nSend_SIte].nLastModule; //������ �������� üũ
		st_tray_info[nRcv_Site].nModuleCnt = st_tray_info[nSend_SIte].nModuleCnt;
		st_tray_info[nRcv_Site].nTrayExist = st_tray_info[nSend_SIte].nTrayExist; //tray ���� ��/�� 
		st_tray_info[nRcv_Site].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
		st_tray_info[nRcv_Site].strPartNo = st_tray_info[nSend_SIte].strPartNo;
		st_tray_info[nRcv_Site].strProcess = st_tray_info[nSend_SIte].strProcess;	// process id.....

		/*if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}
	else if (nMode == 1) // nMode == 1 : ������ ����Ʈ ����, send data �� ����� 
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
		st_tray_info[nRcv_Site].nLastModule = st_tray_info[nSend_SIte].nLastModule; //������ �������� üũ
		st_tray_info[nRcv_Site].nModuleCnt = st_tray_info[nSend_SIte].nModuleCnt;
		st_tray_info[nRcv_Site].nTrayExist = st_tray_info[nSend_SIte].nTrayExist; //tray ���� ��/�� 
		st_tray_info[nRcv_Site].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
		st_tray_info[nRcv_Site].strPartNo = st_tray_info[nSend_SIte].strPartNo;
		st_tray_info[nRcv_Site].strProcess = st_tray_info[nSend_SIte].strProcess;	// process id.....

		st_tray_info[nSend_SIte].nNewLotIn = 0;
		st_tray_info[nSend_SIte].nLastModule = 0; //������ �������� üũ
		st_tray_info[nSend_SIte].nModuleCnt = 0;
		st_tray_info[nSend_SIte].nTrayExist = 0;
		st_tray_info[nSend_SIte].strLotNo = "";// lot no......
		st_tray_info[nSend_SIte].strPartNo = "";
		st_tray_info[nSend_SIte].strProcess = "";	// process id.....


		/*if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}
	else if(nMode == 2) // ���� �۾��ϴ� lot�� ������ �־��ش�. ����̽� ������ nDvc_Yes_No ������ ���� ������ �Ѵ� 
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
		st_tray_info[nRcv_Site].nLastModule = st_tray_info[nSend_SIte].nLastModule; //������ �������� üũ
		st_tray_info[nRcv_Site].nModuleCnt = st_tray_info[nSend_SIte].nModuleCnt;
		st_tray_info[nRcv_Site].nTrayExist = st_tray_info[nSend_SIte].nTrayExist; //tray ���� ��/�� 
		st_tray_info[nRcv_Site].strLotNo = st_tray_info[nSend_SIte].strLotNo;// lot no......
		st_tray_info[nRcv_Site].strPartNo = st_tray_info[nSend_SIte].strPartNo;
		st_tray_info[nRcv_Site].strProcess = st_tray_info[nSend_SIte].strProcess;	// process id.....


		/*if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_normal_msg, "[CLEAR DATA] SHIFT_IDBUFF_OUTPUTREADY_BUFFER DATA CLEAR");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}*/
	}		 

	else //if(n_mode == 3) // ������ ����Ʈ ������ ��� �ʿ� ���� �Ϸ� �� �� ����Ʈ�� ������ Ŭ���� �Ѵ� 
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
		st_tray_info[nSend_SIte].nLastModule = 0; //������ �������� üũ
		st_tray_info[nSend_SIte].nModuleCnt = 0;
		st_tray_info[nSend_SIte].nTrayExist = 0;
		st_tray_info[nSend_SIte].strLotNo = "";// lot no......
		st_tray_info[nSend_SIte].strPartNo = "";
		st_tray_info[nSend_SIte].strProcess = "";	// process id.....
		

		/*if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
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

		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck1 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck2, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980001");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck2 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck3, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980002");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck3 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck4, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980003");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck4 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck5, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980004");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck5 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck6, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980005");
		alarm.mstr_code = _T(Jamcode); 
		
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck6 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck7, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980006");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck7 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck8, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980007");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck8 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck9, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980008");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck9 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		
		return CTLBD_RET_ERROR;
	}
	
	if( g_ioMgr.get_in_bit(st_io.i_DoorCheck10, IO_ON) == IO_OFF )
	{
		sprintf(Jamcode, "980009");
		alarm.mstr_code = _T(Jamcode); 
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] DoorCheck10 open", alarm.mstr_code);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
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
	nRet[0] = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);	// Device ������ 
	nRet[1] = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_ON);
	nRet[2] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Forward_Check, IO_ON);	// �ϴܿ��� Jig �о��� 
	nRet[3] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X1_Backward_Check, IO_ON);
	nRet[4] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, IO_ON);		// ��ܿ��� Jig �о��� 
	nRet[5] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, IO_ON);
	nRet[6] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Up_Check, IO_ON);		// Left z
	nRet[7] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
	nRet[8] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Up_Check, IO_ON);		// Right Z
	nRet[9] = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
	nRet[10] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);		// Left ���ܿ��� Jig �ɾ��� 
	nRet[11] = g_ioMgr.get_in_bit(st_io.i_Carrier_1_Backward_Check, IO_ON);
	nRet[12] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Forward_Check, IO_ON);		// Right ���ܿ��� Jig �ɾ��� 
	nRet[13] = g_ioMgr.get_in_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);		
	nRet[14] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, IO_ON);	// ��� �߰����� Jig Hole�� ����־� JIg ����� 
	nRet[15] = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, IO_ON);	
	nRet[16] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Forward_Check, IO_ON);		// ��� �߰����� jig �յڷ� ������ jig ����� 
	nRet[17] = g_ioMgr.get_in_bit(st_io.i_Press_PIN_Guide_Backward_Check, IO_ON);

	if ( nMode == 1 )//TOP �̵� ����
	{
		nFuncRet = RET_GOOD;
	}
	else if( nMode == 2 )//BOTTOM �̵�����
	{
		nFuncRet = RET_GOOD;
	}
	else if(nMode == 3)//ZIG �ٿ� ����
	{
		nFuncRet = RET_GOOD;
	}
	else if( nMode == 4)//ZIP �� ����
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
//�׻� ���ڵ带 �а� �����̾� �Ѵ�.
//1 Mode = 0 // ���� ����  Left carrier�� ������ �ʵǴ� �����̰� Right�� ������ �Ǵ� �����̴�.
//2. Mode = 1 �а� ���ڿ� ����
//3 .Mode = 2 �а� ���ͻ���
int CPublic_Function::Check_BeforeMoveUnPressRbt( int nMode )
{
	return RET_GOOD;
	int nFuncRet = RET_ERROR;
	int nRet[40]= {0,};

	//Press_UnPress �Ǹ���
	nRet[0] = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, IO_ON);
	nRet[1] = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, IO_OFF);
	nRet[2] = g_ioMgr.get_out_bit(st_io.o_Press_Up_Sol, IO_ON);
	nRet[3] = g_ioMgr.get_out_bit(st_io.o_Press_Down_Sol, IO_OFF);
	nRet[4] = g_ioMgr.get_in_bit( st_io.i_Press_UpDown_CellIn_Check, IO_OFF);

	//top���� carrier�� �ж� �����ͼ� �̴� �Ǹ���
	nRet[5] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Up_Check, IO_ON);
	nRet[6] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Down_Check, IO_OFF);
	nRet[7] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, IO_OFF);//up
	nRet[8] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, IO_OFF);//down

	//���� ���̵�� down����
	nRet[9] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z1_Up_Check, IO_OFF);
	nRet[10] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z1_Down_Check, IO_ON);
	nRet[11] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z1_Up_Sol, IO_OFF);
	nRet[12] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z1_Down_Sol, IO_ON);

	nRet[13] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z2_Up_Check, IO_OFF);
	nRet[14] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_Z2_Down_Check, IO_ON);
	nRet[15] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z2_Up_Sol, IO_OFF);
	nRet[16] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_Z2_Down_Sol, IO_ON);

	//Clamp ����,����
	nRet[17] = g_ioMgr.get_out_bit(st_io.i_Carrier_1_Forward_Check, IO_ON);
	nRet[18] = g_ioMgr.get_out_bit(st_io.i_Carrier_1_Backward_Check, IO_ON);
	nRet[19] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_1_Forward_Sol, IO_ON);
	nRet[20] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_1_Backward_Sol, IO_ON);

	nRet[21] = g_ioMgr.get_out_bit(st_io.i_Carrier_2_Forward_Check, IO_ON);
	nRet[22] = g_ioMgr.get_out_bit(st_io.i_Carrier_2_Backward_Check, IO_ON);
	nRet[23] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_2_Forward_Sol, IO_ON);
	nRet[24] = g_ioMgr.get_out_bit(st_io.o_Carrier_Clamp_2_Backward_Sol, IO_ON);


	nRet[25] = g_ioMgr.get_in_bit( st_io.i_Carrier_Z_1_Up_Check, IO_OFF); //Carrier�� ���� ����� �Ѵ�.
	nRet[26] = g_ioMgr.get_in_bit( st_io.i_Carrier_Z_2_Up_Check, IO_ON); //Carrier�� ���� �־�� �Ѵ�.

	//������ġ(5��)���� ���� ������Ű�� �Ǹ���
	nRet[27] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_ON);
	nRet[28] = g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON);
	nRet[29] = g_ioMgr.get_out_bit(st_io.o_Camera_Y_Jig_Press_Forward_Sol, IO_ON);
	nRet[30] = g_ioMgr.get_out_bit(st_io.o_Camera_Y_Jig_Press_Backward_Sol, IO_ON);

	//����(2,3,6) ������Ű�� �Ǹ���
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
		if( nMode == 1|| nMode == 2)//�а� ���� �̹Ƿ� Left Carrier ���� Right�� ���� �ϸ� �ʵȴ�.
		{
			if( nRet[25] == IO_OFF)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_1_Up_Check); 
			}
			else if( nRet[26] == IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Carrier_Z_2_Up_Check); 
			}
			if( nMode == 1 )//�ж��� �Ǹ����� ������ �־�� �Ѵ�
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

	//X�� �Ÿ�
	st_work.d_X_Box1Pos = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_2]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayX - 1));
	//Y�� �Ÿ�
	st_work.d_Y_Box1Pos = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_4]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayY - 1));

	//X����
	st_work.d_X_Box1Deg = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_2]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayX - 1));
	//Y����
	st_work.d_Y_Box1Deg = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_4]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_1_1]) / (st_recipe.nHsTrayY - 1));

// 	if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
// 	{
// 		st_other.str_normal_msg.Format("1.nHeatSinkColCount=%d, nHeatSinkRowCount = %d, d_X_Box1Pos = %f, d_Y_Box1Pos = %f", 
// 			st_recipe.nHsTrayX, st_recipe.nHsTrayY, st_work.d_X_Box1Pos, st_work.d_Y_Box1Pos);
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
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
	//X��
	st_work.d_X_Box2Pos = ((st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_2]
	- st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRANSFER_X_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayX - 1));
	//Y��
	st_work.d_Y_Box2Pos = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_4]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayY - 1));

	//X����
	st_work.d_X_Box2Deg = ((st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_2]
	- st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_HEATSINK_BOX_2_1]) / (st_recipe.nHsTrayX - 1));
	//Y����
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
// 	st_work.strBarcodeRecive = "";//���� �޼��� �ʱ�ȭ
	st_msg.mstr_barcode = "";
	
	strBcrCommand.Format("%cLON%c", 0x02, 0x03);//���ڵ� ������ �б� ���� ���
	st_serial.str_snd[BCR_PORT -1] = strBcrCommand;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, BCR_PORT, 0);
	//::Sleep(100);
	
	if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
	{
		sprintf(st_msg.c_normal_msg,"[BCR Test] %s",strBcrCommand);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
	}
	
}

BOOL CPublic_Function::SendLocalMsg(RCV_STRUCT msg)
{
	CString	strTmp;
	
	HWND hWnd = ::FindWindow(NULL, "HeatSinkInspection");
	if(hWnd)
	{
		HANDLE handle = AfxGetInstanceHandle(); //�ڽ��� �ڵ� ���
		
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
	CString strFileName;				// ���������� ������ ���� �̸� ���� ���� 
	CString strCreateFile;				// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString strListName, strTempData;	// �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString strContent;				// �� ���ο� ����� ���ڿ� ���� ���� ���� 
	int nExistence;						// ���� ���� ���� ���� �÷��� 
	char chFileName[256];				// �˻��� ���� ���� ���� �Լ� 
	char chMsg[1000];
	FILE  *fp ;							// ���Ͽ� ���� ������ ���� ���� 

	CString strTitleTime, strFileTime;		// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	int nCurYear, nCurMonth, nCurDay;					// ��, ��, �� ���� ����
	int nCurHour, nCurMinute, nCurSecond;				// ��, ��, �� ���� ����
	int nMtbi = 0;

	//	double dAve;

	COleDateTime otCurr;									// ���� �ð� ���� ����
	COleDateTime tNext;
	CTime tCurr;										// Ÿ��Ʋ �ð� ���� ����

	CString strData, strTime;

	/* ************************************************************************** */
	/* ���� �̸����� ����� ��¥ �����Ѵ�                                         */
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

	if (nExistence == -1)  /* ���� ������ */
	{
		//		strCreateFile = st_path_info.strPathCycle + strTime;
		//		strCreateFile += ".TXT";
		//OnStringToChar(strCreateFile, chFileName);
		sprintf(chFileName, "%s", strCreateFile);
	}

	/* ************************************************************************** */
	/* �˶� �߻� Ƚ�� ���� ���� ���Ͽ� �߰� ������ ���� ���Ϸ� ����               */
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
	CString strFileName;				// ���������� ������ ���� �̸� ���� ���� 
	CString strCreateFile;				// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString strListName, strTempData;	// �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString strContent;				// �� ���ο� ����� ���ڿ� ���� ���� ���� 
	int nExistence;						// ���� ���� ���� ���� �÷��� 
	char chFileName[256];				// �˻��� ���� ���� ���� �Լ� 
	char chMsg[1000];
	FILE  *fp ;							// ���Ͽ� ���� ������ ���� ���� 

	CString strTitleTime, strFileTime;		// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	int nCurYear, nCurMonth, nCurDay;					// ��, ��, �� ���� ����
	int nCurHour, nCurMinute, nCurSecond;				// ��, ��, �� ���� ����
	int nMtbi = 0;

	double dAve;

	COleDateTime otCurr;									// ���� �ð� ���� ����
	CTime tCurr;										// Ÿ��Ʋ �ð� ���� ����

	CString strTemp;

	CString strData, strTime;

	/* ************************************************************************** */
	/* ���� �̸����� ����� ��¥ �����Ѵ�                                         */
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

	if (nExistence == -1)  /* ���� ������ */
	{
		strCreateFile = st_path.strPathCycle + strTime;
		strCreateFile += ".TXT";
		//OnStringToChar(strCreateFile, chFileName);
		sprintf(chFileName, "%s", strCreateFile);
	}

	/* ************************************************************************** */
	/* �˶� �߻� Ƚ�� ���� ���� ���Ͽ� �߰� ������ ���� ���Ϸ� ����               */
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
		//		Func.MsgLog("���� ���� ����!..") ;
		fclose(fp); 

		return ;
	}

	fclose(fp); 
}

void CPublic_Function::OnDailyCycleData(CString strLotNo, CString strPartNo, double dTime, CTime tStart, CTime tEnd)
{
	CString strFileName;				// ���������� ������ ���� �̸� ���� ���� 
	CString strCreateFile;				// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString strListName, strTempData;	// �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString strContent;				// �� ���ο� ����� ���ڿ� ���� ���� ���� 
	int nExistence;						// ���� ���� ���� ���� �÷��� 
	char chFileName[256];				// �˻��� ���� ���� ���� �Լ� 
	char chMsg[1000];
	FILE  *fp ;							// ���Ͽ� ���� ������ ���� ���� 

	CString strTitleTime, strFileTime;		// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	int nCurYear, nCurMonth, nCurDay;					// ��, ��, �� ���� ����
	int nCurHour, nCurMinute, nCurSecond;				// ��, ��, �� ���� ����
	int nMtbi = 0;

	COleDateTime otCurr;									// ���� �ð� ���� ����
	CTime tCurr;										// Ÿ��Ʋ �ð� ���� ����

	CString strTemp;

	CString strData, strTime;

	/* ************************************************************************** */
	/* ���� �̸����� ����� ��¥ �����Ѵ�                                         */
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

	if (nExistence == -1)  /* ���� ������ */
	{
		strCreateFile = st_path.strPathCycle + strTime;
		strCreateFile += ".TXT";
		//OnStringToChar(strCreateFile, chFileName);
		sprintf(chFileName, "%s", strCreateFile);
	}

	/* ************************************************************************** */
	/* �˶� �߻� Ƚ�� ���� ���� ���Ͽ� �߰� ������ ���� ���Ϸ� ����               */
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
		//		Func.MsgLog("���� ���� ����!..") ;
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
