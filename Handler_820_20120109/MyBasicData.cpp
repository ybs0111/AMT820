// MyBasicData.cpp: implementation of the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MyBasicData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyBasicData::CMyBasicData()
{

}

CMyBasicData::~CMyBasicData()
{

}

void CMyBasicData::OnPassWord_Load(CString str_load_level)
{
	CString str_pass;
	CString str_load_file;
	char chr_data[25];						// 암호는 25글자 이내로 제한

	(st_other.str_password).Empty();		// 암호 저장 변수 초기화

	str_load_file = st_path.mstr_basic + st_basic.mstr_device_name;

	if (str_load_level == "Lock")			// SYSTEM LOCK 암호
		:: GetPrivateProfileString("Password", "SysLock", "M", chr_data, 20, str_load_file);
	else if (str_load_level == "Level1")	// 메인트 암호
		:: GetPrivateProfileString("Password", "Level_1", "M", chr_data, 20, str_load_file);
	else if (str_load_level == "Level2")	// 티칭 암호
		:: GetPrivateProfileString("Password", "Level_2", "T", chr_data, 20, str_load_file);
	else  
		return;

	str_pass = chr_data;
	str_pass.MakeUpper();     // 문자열 대문자로 변경
	str_pass.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	str_pass.TrimRight(' ');  // 우측 문자열 공백 제거

	st_other.str_password = str_pass;  // 암호 전역 변수에 설정
}

void CMyBasicData::OnPassWord_Save(CString str_save_level, CString str_pass_data)
{
	CString str_save_file;
	
	str_save_file = st_path.mstr_basic + st_basic.mstr_device_name;

	if (str_save_level=="Lock")         // SYSTEM LOCK 암호
		:: WritePrivateProfileString("Password", "SysLock", LPCTSTR(str_pass_data), str_save_file);
	else if (str_save_level=="Level1")  // 메인트 암호
		:: WritePrivateProfileString("Password", "Level_1", LPCTSTR(str_pass_data), str_save_file);
	else if (str_save_level=="Level2")  // 티칭 암호
		:: WritePrivateProfileString("Password", "Level_2", LPCTSTR(str_pass_data), str_save_file);
}

CString CMyBasicData::OnStep_File_Index_Load()
{
	CString str_file_index;  // 파일 인덱스 저장 변수
	CString str_load_file;
	char chr_load[20];

	str_load_file = st_path.mstr_basic + st_basic.mstr_device_name;

	:: GetPrivateProfileString("Thread_Step_file", "File_Index", "00", chr_load, 20, str_load_file);
	str_file_index = chr_load;
	str_file_index.TrimLeft(' ');               
	str_file_index.TrimRight(' ');

	return str_file_index;  // 파일 인덱스 리턴
}

void CMyBasicData::OnStep_File_Index_Save(CString str_index)
{
	CString str_save_file;

	str_save_file = st_path.mstr_basic + st_basic.mstr_device_name;

	:: WritePrivateProfileString("Thread_Step_file", "File_Index", LPCTSTR(str_index), str_save_file);
}

CString CMyBasicData::OnStep_File_Name_Load()
{
	CString str_file_name;  // 파일명 저장 변수
	CString str_load_file;
	char chr_load[20];

	str_load_file = st_path.mstr_basic + st_basic.mstr_device_name;

	:: GetPrivateProfileString("Thread_Step_file", "File_Name", "DEFAULT", chr_load, 20, str_load_file);
	str_file_name = chr_load;
	str_file_name.TrimLeft(' ');               
	str_file_name.TrimRight(' ');

	return str_file_name;  // 파일명 리턴
}

void CMyBasicData::OnStep_File_Name_Save(CString str_file)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Name", LPCTSTR(str_file), st_path.mstr_basic);
}

void CMyBasicData::OnMotorSpeed_Set_Data_Load()
{
	int  n_chk, i = 0, j = 0;
	CString str_temp, str_part;       // 임시 저장 변수
	CString str_motor_name;
	double d_chk;

	char chr_data[20];

	CString str_msg, str_LoadFile;       // 임시 저장 변수
	
	if( st_basic.n_file_save == CTL_NO)
	{

		str_LoadFile = OnGet_Teach_File_Name("Speed");  // 데이터 저장 파일명 로딩 함수 
		
		for ( i = 0; i < M_MOTOR_COUNT; i++ )		// 새로운 Allow용 2K14/11/04/ViboX
		{
			str_part.Format("[%d]", i);


			:: GetPrivateProfileString("MotorSpeed", "Acc" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_vel[i][1] = 300;
				str_temp.Format( "%3.2f", COMI.md_spd_vel[i][1] );
				:: WritePrivateProfileString("MotorSpeed", "Acc" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_vel[i][1] = d_chk;


			:: GetPrivateProfileString("MotorSpeed", "Dec" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_vel[i][2] = 300;
				str_temp.Format( "%3.2f", COMI.md_spd_vel[i][2] );
				:: WritePrivateProfileString("MotorSpeed", "Dec" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_vel[i][2] = d_chk;


			:: GetPrivateProfileString("MotorSpeed", "Vel" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_vel[i][0] = 50;
				str_temp.Format( "%3.2f", COMI.md_spd_vel[i][0] );
				:: WritePrivateProfileString("MotorSpeed", "Vel" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_vel[i][0] = d_chk;


			:: GetPrivateProfileString("MotorSpeed", "Home" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_home[i] = 10;
				str_temp.Format( "%3.2f", COMI.md_allow_value[i] );
				:: WritePrivateProfileString("MotorSpeed", "Home" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_home[i] = d_chk;
			
			
			:: GetPrivateProfileString("MotorSpeed", "Jog" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_jog[i] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_allow_value[i] );
				:: WritePrivateProfileString("MotorSpeed", "Jog" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_jog[i] = d_chk;
			
			
			:: GetPrivateProfileString("MotorSpeed", "Allow" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_allow_value[i] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_allow_value[i] );
				:: WritePrivateProfileString("MotorSpeed", "Allow" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_allow_value[i] = d_chk;
			
			:: GetPrivateProfileString("MotorSpeed", "M_LIMIT" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_limit_position[i][0] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_limit_position[i][0] );
				:: WritePrivateProfileString("MotorSpeed", "M_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_limit_position[i][0] = d_chk;
			
			:: GetPrivateProfileString("MotorSpeed", "P_LIMIT" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_limit_position[i][1] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_limit_position[i][1] );
				:: WritePrivateProfileString("MotorSpeed", "P_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_limit_position[i][1] = d_chk;
			
		}
	}

	//2016.0327
	if( st_basic.n_file_save == CTL_YES)
	{
		for ( i = 0; i < M_MOTOR_COUNT; i++ )		// 새로운 Allow용 2K14/11/04/ViboX
		{

			str_part.Format("[%d]", i);


			:: GetPrivateProfileString("MotorSpeed", "Acc" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_vel[i][1] = 300;
				str_temp.Format( "%3.2f", COMI.md_spd_vel[i][1] );
				:: WritePrivateProfileString("MotorSpeed", "Acc" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_vel[i][1] = d_chk;


			:: GetPrivateProfileString("MotorSpeed", "Dec" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_vel[i][2] = 300;
				str_temp.Format( "%3.2f", COMI.md_spd_vel[i][2] );
				:: WritePrivateProfileString("MotorSpeed", "Dec" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_vel[i][2] = d_chk;


			:: GetPrivateProfileString("MotorSpeed", "Vel" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_vel[i][0] = 50;
				str_temp.Format( "%3.2f", COMI.md_spd_vel[i][0] );
				:: WritePrivateProfileString("MotorSpeed", "Vel" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_vel[i][0] = d_chk;


			:: GetPrivateProfileString("MotorSpeed", "Home" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_home[i] = 10;
				str_temp.Format( "%3.2f", COMI.md_allow_value[i] );
				:: WritePrivateProfileString("MotorSpeed", "Home" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_home[i] = d_chk;
			
			
			:: GetPrivateProfileString("MotorSpeed", "Jog" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_spd_jog[i] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_allow_value[i] );
				:: WritePrivateProfileString("MotorSpeed", "Jog" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_spd_jog[i] = d_chk;
			
			
			:: GetPrivateProfileString("MotorSpeed", "Allow" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_allow_value[i] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_allow_value[i] );
				:: WritePrivateProfileString("MotorSpeed", "Allow" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_allow_value[i] = d_chk;
			
			:: GetPrivateProfileString("MotorSpeed", "M_LIMIT" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_limit_position[i][0] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_limit_position[i][0] );
				:: WritePrivateProfileString("MotorSpeed", "M_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_limit_position[i][0] = d_chk;
			
			:: GetPrivateProfileString("MotorSpeed", "P_LIMIT" + str_part, "0", chr_data, 10, st_path.mstr_basic);
			d_chk = atof(chr_data);
			
			if ( d_chk <= 0 )
			{
				COMI.md_limit_position[i][1] = 0.1;
				str_temp.Format( "%3.2f", COMI.md_limit_position[i][1] );
				:: WritePrivateProfileString("MotorSpeed", "P_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic);
			}
			else
				COMI.md_limit_position[i][1] = d_chk;
		}
	}

	//Speed Rate
	:: GetPrivateProfileString("Speed_Rate", "Run", "100", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	st_handler.md_run_speed = (double)n_chk;
	COMI.mn_runspeed_rate = st_handler.md_run_speed;

	:: GetPrivateProfileString("Speed_Rate", "Manual", "80", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	st_handler.md_manual_speed = (double)n_chk;
	COMI.mn_manualspeed_rate = st_handler.md_manual_speed;
}

void CMyBasicData::OnMotorSpeed_Set_Data_Save()
{
	int i = 0, j = 0;
	CString str_temp, str_part;       // 임시 저장 변수
	CString str_motor_name;
	char chr_buf[20];
	
	CString str_SaveFile;			// 임시 저장 변수
	
	if(st_basic.n_file_save == CTL_NO)
	{

		str_SaveFile = OnGet_Teach_File_Name( "Speed" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			str_part.Format("[%d]", i);
			str_temp.Format("%3.2f", COMI.md_spd_vel[i][1] );
			:: WritePrivateProfileString( "MotorSpeed", "Acc" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_vel[i][2] );
			:: WritePrivateProfileString( "MotorSpeed", "Dec" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_vel[i][0] );
			:: WritePrivateProfileString( "MotorSpeed", "Vel" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_home[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Home" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_jog[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Jog" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_allow_value[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Allow" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_limit_position[i][0] );
			:: WritePrivateProfileString( "MotorSpeed", "M_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );
			
			str_temp.Format("%3.2f", COMI.md_limit_position[i][1] );
			:: WritePrivateProfileString( "MotorSpeed", "P_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );
		}
		
		str_SaveFile = OnGet_Teach_File_Name("Speed_Back" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			str_part.Format("[%d]", i);
			
			str_temp.Format("%3.2f", COMI.md_spd_vel[i][1] );
			:: WritePrivateProfileString( "MotorSpeed", "Acc" + str_part, LPCTSTR(str_temp), str_SaveFile );
			
			str_temp.Format("%3.2f", COMI.md_spd_vel[i][2] );
			:: WritePrivateProfileString( "MotorSpeed", "Dec" + str_part, LPCTSTR(str_temp), str_SaveFile );
			
			str_temp.Format("%3.2f", COMI.md_spd_vel[i][0] );
			:: WritePrivateProfileString( "MotorSpeed", "Vel" + str_part, LPCTSTR(str_temp), str_SaveFile );
			
			str_temp.Format("%3.2f", COMI.md_spd_home[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Home" + str_part, LPCTSTR(str_temp), str_SaveFile );
			
			str_temp.Format("%3.2f", COMI.md_spd_jog[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Jog" + str_part, LPCTSTR(str_temp), str_SaveFile );
			
			str_temp.Format("%3.2f", COMI.md_allow_value[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Allow" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_limit_position[i][0] );
			:: WritePrivateProfileString( "MotorSpeed", "M_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );
			
			str_temp.Format("%3.2f", COMI.md_limit_position[i][1] );
			:: WritePrivateProfileString( "MotorSpeed", "P_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

		}
	}

	if(st_basic.n_file_save == CTL_YES)
	{
		for( i = 0; i < M_MOTOR_COUNT; i++ )
		{//double md_spd_vel[BD_MAX_MOTOR][3]; 		//[0]:VEL, [1]:accel,[2]:deceel
			str_part.Format("[%d]", i);
			str_temp.Format("%3.2f", COMI.md_spd_vel[i][1] );
			:: WritePrivateProfileString( "MotorSpeed", "Acc" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_vel[i][2] );
			:: WritePrivateProfileString( "MotorSpeed", "Dec" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_vel[i][0] );
			:: WritePrivateProfileString( "MotorSpeed", "Vel" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_home[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Home" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_spd_jog[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Jog" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );

			str_temp.Format("%3.2f", COMI.md_allow_value[i] );
			:: WritePrivateProfileString( "MotorSpeed", "Allow" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );
			
			str_temp.Format("%3.2f", COMI.md_limit_position[i][0] );
			:: WritePrivateProfileString( "MotorSpeed", "M_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );
			
			str_temp.Format("%3.2f", COMI.md_limit_position[i][1] );
			:: WritePrivateProfileString( "MotorSpeed", "P_LIMIT" + str_part, LPCTSTR(str_temp), st_path.mstr_basic );
		}
		
	}

	// Speed Rate
	str_temp = LPCTSTR(_itoa((int)(COMI.mn_runspeed_rate), chr_buf, 10));
	:: WritePrivateProfileString("Speed_Rate", "Run", LPCTSTR(str_temp), st_path.mstr_basic);
	
	str_temp = LPCTSTR(_itoa((int)(COMI.mn_manualspeed_rate), chr_buf, 10));
	:: WritePrivateProfileString("Speed_Rate", "Manual", LPCTSTR(str_temp), st_path.mstr_basic);
}

void CMyBasicData::OnWaitTime_Data_Load()
{
	CString str_temp;       // 임시 저장 변수
	CString str_motor_name;
	
	CString str_msg, str_LoadFile;       // 임시 저장 변수
	
	int i = 0, n_chk = 0;
	CString strPos;
	char chr_data[20];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 기존 DAT -> TXT로 바꾸기 위한 저장용.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

	if( st_basic.n_file_save == CTL_NO)
	{

		str_LoadFile = OnGet_Teach_File_Name( "Time" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
		{
			strPos.Format("%02d_Off", i);
			:: GetPrivateProfileString( "Wait Time", strPos, "-1", chr_data, 20, str_LoadFile );
			n_chk = atoi(chr_data);
			if ( n_chk <= 0 )
			{
				st_wait.nOffWaitTime[i] = 100;
				str_temp.Format("%d", st_wait.nOffWaitTime[i] );
				:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_LoadFile );
			}
			else
				st_wait.nOffWaitTime[i] = n_chk;
			
			strPos.Format("%02d_On", i);
			:: GetPrivateProfileString( "Wait Time", strPos, "-1", chr_data, 20, str_LoadFile );
			n_chk = atoi(chr_data);
			if ( n_chk <= 0 )
			{
				st_wait.nOnWaitTime[i] = 100;
				str_temp.Format("%d", st_wait.nOnWaitTime[i] );
				:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_LoadFile );
			}
			else
				st_wait.nOnWaitTime[i] = n_chk;
			
			strPos.Format("%02d_Limit", i);
			:: GetPrivateProfileString( "Wait Time", strPos, "-1", chr_data, 20, str_LoadFile );
			n_chk = atoi(chr_data);
			if ( n_chk <= 0 )
			{
				st_wait.nLimitWaitTime[i] = 2000;
				str_temp.Format("%d", st_wait.nLimitWaitTime[i] );
				:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_LoadFile );
			}
			else
				st_wait.nLimitWaitTime[i] = n_chk;
		}
	}

	if( st_basic.n_file_save == CTL_YES)
	{

		for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
		{

			strPos.Format("%02d_Off", i);
			:: GetPrivateProfileString( "Wait Time", strPos, "-1", chr_data, 20, st_path.mstr_basic );
			n_chk = atoi(chr_data);
			if ( n_chk <= 0 )
			{
				st_wait.nOffWaitTime[i] = 100;
				str_temp.Format("%d", st_wait.nOffWaitTime[i] );
				:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), st_path.mstr_basic );
			}
			else
				st_wait.nOffWaitTime[i] = n_chk;
			
			strPos.Format("%02d_On", i);
			:: GetPrivateProfileString( "Wait Time", strPos, "-1", chr_data, 20, st_path.mstr_basic );
			n_chk = atoi(chr_data);
			if ( n_chk <= 0 )
			{
				st_wait.nOnWaitTime[i] = 100;
				str_temp.Format("%d", st_wait.nOnWaitTime[i] );
				:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), st_path.mstr_basic );
			}
			else
				st_wait.nOnWaitTime[i] = n_chk;
			
			strPos.Format("%02d_Limit", i);
			:: GetPrivateProfileString( "Wait Time", strPos, "-1", chr_data, 20, st_path.mstr_basic );
			n_chk = atoi(chr_data);
			if ( n_chk <= 0 )
			{
				st_wait.nLimitWaitTime[i] = 2000;
				str_temp.Format("%d", st_wait.nLimitWaitTime[i] );
				:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), st_path.mstr_basic );
			}
			else
				st_wait.nLimitWaitTime[i] = n_chk;
			
		}
	}
}

void CMyBasicData::OnWaitTime_Data_Save()
{
	CString str_temp;       // 임시 저장 변수
	CString str_motor_name;
	
	CString str_SaveFile;			// 임시 저장 변수
	
	int i = 0;
	CString strPos;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 기존 DAT -> TXT로 바꾸기 위한 저장용.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	if( st_basic.n_file_save == CTL_NO)
	{

		str_SaveFile = OnGet_Teach_File_Name( "Time" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
		{
			strPos.Format("%02d_Off", i);
			if( st_wait.nOffWaitTime[i] < 0 )
				str_temp.Format("%d", 300 );
			else
				str_temp.Format("%d", st_wait.nOffWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_SaveFile );
			
			strPos.Format("%02d_On", i);
			if( st_wait.nOnWaitTime[i] < 0 )
				str_temp.Format("%d", 300 );
			else
				str_temp.Format("%d", st_wait.nOnWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_SaveFile );
			
			strPos.Format("%02d_Limit", i);
			if( st_wait.nLimitWaitTime[i] < 0 )
				str_temp.Format("%d", 2000 );
			else
				str_temp.Format("%d", st_wait.nLimitWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_SaveFile );
		}
		
		str_SaveFile = OnGet_Teach_File_Name( "Time_Back" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
		{
			strPos.Format("%02d_Off", i);
			if( st_wait.nOffWaitTime[i] < 0 )
				str_temp.Format("%d", 300 );
			else
				str_temp.Format("%d", st_wait.nOffWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_SaveFile );
			
			strPos.Format("%02d_On", i);
			if( st_wait.nOnWaitTime[i] < 0 )
				str_temp.Format("%d", 300 );
			else
				str_temp.Format("%d", st_wait.nOnWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_SaveFile );
			
			strPos.Format("%02d_Limit", i);
			if( st_wait.nOnWaitTime[i] < 0 )
				str_temp.Format("%d", 2000 );
			else
				str_temp.Format("%d", st_wait.nOnWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), str_SaveFile );
		}
	}

	if(st_basic.n_file_save == CTL_YES)
	{
		for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
		{
			strPos.Format("%02d_Off", i);
			if( st_wait.nOffWaitTime[i] < 0 )
				str_temp.Format("%d", 300 );
			else
				str_temp.Format("%d", st_wait.nOffWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), st_path.mstr_basic );
			
			strPos.Format("%02d_On", i);
			if( st_wait.nOnWaitTime[i] < 0 )
				str_temp.Format("%d", 300 );
			else
				str_temp.Format("%d", st_wait.nOnWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), st_path.mstr_basic );
			
			strPos.Format("%02d_Limit", i);
			if( st_wait.nLimitWaitTime[i] < 0 )
				str_temp.Format("%d", 2000 );
			else
				str_temp.Format("%d", st_wait.nLimitWaitTime[i] );
			:: WritePrivateProfileString( "Wait Time", strPos, LPCTSTR(str_temp), st_path.mstr_basic );
		}		
	}
}

void CMyBasicData::OnMaintenance_Data_Load()
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
	int n_chk=0, i=0;
	CString strLoadFile;

	char chr_data[20], chr_buf[20] ;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 로딩하여 전역 변수에 설정한다                           */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Red", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[0] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[1] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[2] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[7] = n_chk;
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 로딩하여 전역 변수에 설정한다                        */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Yellow", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[0] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[1] = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[2] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 로딩하여 전역 변수에 설정한다                         */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Green", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[0] = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[1] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[2] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_buzzer_mode", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>1)
	{
		st_lamp.mn_buzzer_mode = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_buzzer_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_buzzer_mode = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "str_equip_no", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_no = chr_data;
	(st_lamp.mstr_equip_no).TrimLeft(' ');               
	(st_lamp.mstr_equip_no).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_id = chr_data;
	(st_lamp.mstr_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_equip_id).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_ec_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);	// 20130822 kjh
	st_lamp.mstr_ec_equip_id = chr_data;
	(st_lamp.mstr_ec_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_ec_equip_id).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_equip_area", "AMT", chr_data, 20, st_path.mstr_basic);	
	st_lamp.mstr_equip_area = chr_data;
	(st_lamp.mstr_equip_area).TrimLeft(' ');               
	(st_lamp.mstr_equip_area).TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_on_time_w", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 1)
	{
		st_lamp.mn_lamp_wait_time = 500;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_wait_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_wait_time = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 변경 시간 로딩하여 전역 변수에 설정                              */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_change_time", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 0)
	{
		st_lamp.mn_lamp_change_time = 5;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_change_time = n_chk;
	/* ************************************************************************** */
}

void CMyBasicData::OnMaintenance_Data_Save()
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
	char chr_buf[20] ;
	CString strSaveFile;
	int i=0;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 정보를 파일에 저장한다                                  */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 정보를 파일에 저장한다                               */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 정보를 파일에 저장한다                                */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_buzzer_mode, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: WritePrivateProfileString("TowerLampData", "str_equip_no", LPCTSTR(st_lamp.mstr_equip_no), st_path.mstr_basic);
	:: WritePrivateProfileString("TowerLampData", "str_equip_code", LPCTSTR(st_lamp.mstr_equip_id), st_path.mstr_basic);
	:: WritePrivateProfileString("TowerLampData", "str_ec_equip_code", LPCTSTR(st_lamp.mstr_ec_equip_id), st_path.mstr_basic);	// 20130822 kjh
	:: WritePrivateProfileString("TowerLampData", "str_equip_area", LPCTSTR(st_lamp.mstr_equip_area), st_path.mstr_basic);	// 2014.12.24 - Bredmin.
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);
}

void CMyBasicData::On_Teach_Data_Save()
{
	CString str_temp, str_SaveFile,str_load_file, str_motor_name, str_chk_ext,str_pos;			// 임시 저장 변수
	int i = 0, j = 0, k =0, n_pos = 0;
	
// 	str_SaveFile = OnGet_Teach_File_Name("Teach");		// 데이터 저장 파일명 로딩 함수 
//	int   err = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 기존 DAT -> TXT로 바꾸기 위한 저장용.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString strMotor = "", strPos = "";
	

	if(st_basic.n_file_save == CTL_NO)
	{

		str_SaveFile = OnGet_Teach_File_Name( "Teach" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			strMotor.Format("%02d_Motor", i);
			
			for ( j = 0; j < M_MAX_POS; j++ )
			{
				strPos.Format("%02d_Pos", j);
				
				str_temp.Format("%f", st_motor[i].md_pos[j] );
				:: WritePrivateProfileString( strMotor, strPos, LPCTSTR(str_temp), str_SaveFile );
			}
		}
		
		str_SaveFile = OnGet_Teach_File_Name("Teach_Back" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			strMotor.Format("%02d_Motor", i);
			
			for ( j = 0; j < M_MAX_POS; j++ )
			{
				strPos.Format("%02d_Pos", j);
				
				str_temp.Format("%f", st_motor[i].md_pos[j] );
				:: WritePrivateProfileString( strMotor, strPos, LPCTSTR(str_temp), str_SaveFile );
			}
		}
	}

	if( st_basic.n_file_save == CTL_YES )
	{
		str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
		n_pos = str_load_file.Find(".");  // 확장자 위치 검사
		
		if (n_pos == -1)
			str_load_file += _T(".TXT");  // 확장자 추가
		else 
		{
			str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
			if (str_chk_ext != _T(".TXT"))  
			{
				str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정
				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
		}
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			strMotor.Format("%02d_Motor", i);
			
			for ( j = 0; j < M_MAX_POS; j++ )
			{
				strPos.Format("%02d_Pos", j);
				
				str_temp.Format("%f", st_motor[i].md_pos[j] );
				:: WritePrivateProfileString( strMotor, strPos, LPCTSTR(str_temp), str_load_file );
			}
		}
	}
}

void CMyBasicData::On_Teach_Data_Load()
{
	CString str_temp, str_msg, str_LoadFile, str_load_file, str_chk_ext, str_motor_name, str_pos;       // 임시 저장 변수
	int i = 0, j = 0, n_pos= 0, k = 0;
	double d_chk;
	
	char chr_data[20];
	
// 	str_LoadFile = OnGet_Teach_File_Name("Teach");  // 데이터 저장 파일명 로딩 함수 
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 기존 DAT -> TXT로 바꾸기 위한 로드
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString strMotor = "", strPos = "";
	
	if( st_basic.n_file_save == CTL_NO )
	{
		str_LoadFile = OnGet_Teach_File_Name( "Teach" );  // 데이터 저장 파일명 로딩 함수 
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			strMotor.Format("%02d_Motor", i);
			
			for ( j = 0; j < M_MAX_POS; j++ )
			{
				strPos.Format("%02d_Pos", j);
				
				:: GetPrivateProfileString( strMotor, strPos, "-1", chr_data, 20, str_LoadFile);
				
				d_chk = atof(chr_data);
				
				if ( d_chk <= 0 )
				{
					st_motor[i].md_pos[j] = 1;
					str_temp.Format("%f", st_motor[i].md_pos[j] );
					:: WritePrivateProfileString( strMotor, strPos, LPCTSTR(str_temp), str_LoadFile );
				}
				else
					st_motor[i].md_pos[j] = d_chk;
			}
		}
	}

	if( st_basic.n_file_save == CTL_YES )
	{
		str_LoadFile = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
		n_pos = str_load_file.Find(".");  // 확장자 위치 검사
		
		if (n_pos == -1)
			str_load_file += _T(".TXT");  // 확장자 추가
		else 
		{
			str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
			if (str_chk_ext != _T(".TXT"))  
			{
				str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정
				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
		}
		
		for (i = 0; i < M_MOTOR_COUNT; i++)
		{
			strMotor.Format("%02d_Motor", i);
			
			for ( j = 0; j < M_MAX_POS; j++ )
			{
				strPos.Format("%02d_Pos", j);
				
				:: GetPrivateProfileString( strMotor, strPos, "-1", chr_data, 20, str_LoadFile);
				
				d_chk = atof(chr_data);
				
				if ( d_chk <= 0 )
				{
					st_motor[i].md_pos[j] = 1;
					str_temp.Format("%f", st_motor[i].md_pos[j] );
					:: WritePrivateProfileString( strMotor, strPos, LPCTSTR(str_temp), str_LoadFile );
				}
				else
					st_motor[i].md_pos[j] = d_chk;
			}
		}
	}

}

void CMyBasicData::OnBasic_Data_Load(int nMode)
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part, str_station, str_board;			// 저장할 정보 임시 저장 변수 
	char chr_data[50], chr_buf[20];	
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;
	float mf_chk = 0;
	CString str_Temp;
	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	//kwlee 2017.0403
	if (nMode == 0)
	{	
		:: GetPrivateProfileString("BasicData", "Model_Name", "", chr_data, 20, st_path.mstr_basic);	// 20130822 kjh
		st_basic.mstr_device_name = chr_data;
		(st_basic.mstr_device_name).TrimLeft(' ');               
		(st_basic.mstr_device_name).TrimRight(' ');
	}
	//

	//str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	//kwlee 2017.0403
	str_load_file = st_path.mstr_path_Model + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	n_pos = str_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			//str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정
			//2017.0617
			str_load_file = st_path.mstr_path_Model + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */
	:: GetPrivateProfileString("BASIC", "INTERFACE_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 2)
	{
		st_basic.n_mode_interface = 0;
		mstr_temp.Format("%d", st_basic.n_mode_interface);
		:: WritePrivateProfileString("BASIC", "INTERFACE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_interface = mn_chk;

	:: GetPrivateProfileString("BASIC", "DEVICE_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 2)
	{
		st_basic.n_mode_device = 1;
		mstr_temp.Format("%d", st_basic.n_mode_device);
		:: WritePrivateProfileString("BASIC", "DEVICE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_device = mn_chk;


	:: GetPrivateProfileString("BASIC", "RETEST_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_retest = 0;
		mstr_temp.Format("%d", st_basic.n_mode_retest);
		:: WritePrivateProfileString("BASIC", "RETEST_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_retest = mn_chk;

	:: GetPrivateProfileString("BASIC", "TESTER_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_tester = 0;
		mstr_temp.Format("%d", st_basic.n_mode_tester);
		:: WritePrivateProfileString("BASIC", "TESTER_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_tester = mn_chk;



	:: GetPrivateProfileString("BASIC", "BCR_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_bcr = 0;
		mstr_temp.Format("%d", st_basic.n_mode_bcr);
		:: WritePrivateProfileString("BASIC", "BCR_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_bcr = mn_chk;
	


	:: GetPrivateProfileString("BASIC", "MODULE_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_module = 0;
		mstr_temp.Format("%d", st_basic.n_mode_module);
		:: WritePrivateProfileString("BASIC", "MODULE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_module = mn_chk;

	
	:: GetPrivateProfileString("BASIC", "7387 MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_7387 = 0;
		mstr_temp.Format("%d", st_basic.n_mode_7387);
		:: WritePrivateProfileString("BASIC", "7387 MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_7387 = mn_chk;


	:: GetPrivateProfileString("BASIC", "TRAY TYPE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_tray_type = 0;
		mstr_temp.Format("%d", st_basic.n_mode_tray_type);
		:: WritePrivateProfileString("BASIC", "TRAY TYPE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_tray_type = mn_chk;

	//kwlee 2017.0411
	:: GetPrivateProfileString("BASIC", "3874_ALARMSKIP_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_3874InspectAlarmSkipMode = 0;
		mstr_temp.Format("%d", st_basic.n_3874InspectAlarmSkipMode);
		:: WritePrivateProfileString("BASIC", "3874_ALARMSKIP_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_3874InspectAlarmSkipMode = mn_chk;

	
	:: GetPrivateProfileString("BASIC", "7387_ALARMSKIP_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_7387InspectAlarmSkipMode = 0;
		mstr_temp.Format("%d", st_basic.n_7387InspectAlarmSkipMode);
		:: WritePrivateProfileString("BASIC", "7387_ALARMSKIP_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_7387InspectAlarmSkipMode = mn_chk;


	:: GetPrivateProfileString("BASIC", "3874 MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_3874InspectMode = 0;
		mstr_temp.Format("%d", st_basic.n_3874InspectMode);
		:: WritePrivateProfileString("BASIC", "3874 MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_3874InspectMode = mn_chk;


	:: GetPrivateProfileString("BASIC", "LIGHT CURTAIN_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_Light_Curtain_Mode = 0;
		mstr_temp.Format("%d", st_basic.n_Light_Curtain_Mode);
		:: WritePrivateProfileString("BASIC", "LIGHT CURTAIN_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_Light_Curtain_Mode = mn_chk;
	
	:: GetPrivateProfileString("BASIC", "CASE_ASSEMBLY_MODE", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_case_assembly = 0;
		mstr_temp.Format("%d", st_basic.n_mode_case_assembly);
		:: WritePrivateProfileString("BASIC", "CASE_ASSEMBLY_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_case_assembly = mn_chk;
	
	//Count

	:: GetPrivateProfileString("BASIC", "RETRY_COUNT", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_count_retry = 0;
		mstr_temp.Format("%d",st_basic.n_count_retry);
		:: WritePrivateProfileString("BASIC", "RETRY_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_count_retry = mn_chk;

	:: GetPrivateProfileString("BASIC", "PICK_RETRY_COUNT", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_count_pick_retry = 0;
		mstr_temp.Format("%d",st_basic.n_count_pick_retry);
		:: WritePrivateProfileString("BASIC", "PICK_RETRY_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_count_pick_retry = mn_chk;



	:: GetPrivateProfileString("BASIC", "PARTIAL_COUNT", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_count_partial = 0;
		mstr_temp.Format("%d",st_basic.n_count_partial);
		:: WritePrivateProfileString("BASIC", "PARTIAL_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_count_partial = mn_chk;



	:: GetPrivateProfileString("BASIC", "ELEVATOR_PARTIAL", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_count_elevator_partial = 0;
		mstr_temp.Format("%d",st_basic.n_count_elevator_partial);
		:: WritePrivateProfileString("BASIC", "ELEVATOR_PARTIAL", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_count_elevator_partial = mn_chk;


	:: GetPrivateProfileString("BASIC", "7387ALARM_COUNT", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n7387AlarmCount = 0;
		mstr_temp.Format("%d",st_basic.n7387AlarmCount);
		:: WritePrivateProfileString("BASIC", "7387ALARM_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n7387AlarmCount = mn_chk;

	:: GetPrivateProfileString("BASIC", "RUBB_COUNT", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_rubb_count = 0;
		mstr_temp.Format("%d",st_basic.n_rubb_count);
		:: WritePrivateProfileString("BASIC", "RUBB_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_rubb_count = mn_chk;

	:: GetPrivateProfileString("BASIC", "EPOXY_CLEAN_TIME", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_EPoxyCleanTime = 10;
		mstr_temp.Format("%d",st_basic.n_EPoxyCleanTime);
		:: WritePrivateProfileString("BASIC", "EPOXY_CLEAN_TIME", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_EPoxyCleanTime = mn_chk;

	:: GetPrivateProfileString("BASIC", "HARDNESS_CHANGE_TIME", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_basic.n_HardnessUseTime = 10;
		mstr_temp.Format("%d",st_basic.n_HardnessUseTime);
		:: WritePrivateProfileString("BASIC", "HARDNESS_CHANGE_TIME", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_HardnessUseTime = mn_chk;

	//Offset
	:: GetPrivateProfileString("BASIC", "EPOXY_X_OFFSET", "0", chr_data, 10, str_load_file);
	st_basic.dEpoxyXLineOffSet = atof(chr_data);
	
	:: GetPrivateProfileString("BASIC", "EPOXY_Y_OFFSET", "0", chr_data, 10, str_load_file);
	st_basic.dEpoxyYLineOffSet = atof(chr_data);


	//kwlee 2017.0417
	:: GetPrivateProfileString("RECIPE", "CARRIER_MOVE_OFFSET", "0", chr_data, 10, str_load_file);
	st_recipe.dHSCarrierSpreadMoveOffset = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "CARRIER_MOVE_DISTANCE", "0", chr_data, 10, str_load_file);
	st_recipe.dHSCarrierSpreadMoveDistance = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "CARRIER_MOVE_10_OFFSET", "0", chr_data, 10, str_load_file);
	st_recipe.dHeatSinkCarrierSpreadMove1Offset = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "CARRIER_MOVE_20_OFFSET", "0", chr_data, 10, str_load_file);
	st_recipe.dHeatSinkCarrierSpreadMove2Offset = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "CARRIER_MOVE_30_OFFSET", "0", chr_data, 10, str_load_file);
	st_recipe.dHeatSinkCarrierSpreadMove3Offset = atof(chr_data);
	
	//kwlee 2017.0416
	:: GetPrivateProfileString("RECIPE", "dHeatsinkRubXOffset", "0", chr_data, 10, str_load_file);
	st_recipe.dHeatsinkRubXOffset = atof(chr_data);
	
	:: GetPrivateProfileString("RECIPE", "dHeatsinkRubYOffset", "0", chr_data, 10, str_load_file);
	st_recipe.dHeatsinkRubYOffset = atof(chr_data);
	

	:: GetPrivateProfileString("RECIPE", "nEpoxyUseLimitCont", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nEpoxyUseLimitCont = 0;
		mstr_temp.Format("%d",st_recipe.nEpoxyUseLimitCont);
		:: WritePrivateProfileString("RECIPE", "nEpoxyUseLimitCont", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nEpoxyUseLimitCont = mn_chk;

	//kwlee 2017.0406
	:: GetPrivateProfileString("RECIPE", "dLoaderTransferTrayDeviceGap", "0", chr_data, 10, str_load_file);
	st_recipe.dLoaderTransferTrayDeviceGap = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "dSatbleTime", "0", chr_data, 10, str_load_file);
	st_recipe.dSatbleTime = atol(chr_data);

	:: GetPrivateProfileString("RECIPE", "nEpoxyRunSpeed", "0", chr_data, 10, str_load_file);
	st_recipe.nEpoxyRunSpeed = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Vel", "0", chr_data, 10, str_load_file);
	st_recipe.nEpoxyXYRunSpeed[0] = atof(chr_data);
	
	:: GetPrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Acc", "0", chr_data, 10, str_load_file);
	st_recipe.nEpoxyXYRunSpeed[1] = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Dec", "0", chr_data, 10, str_load_file);
	st_recipe.nEpoxyXYRunSpeed[2] = atof(chr_data);

	:: GetPrivateProfileString("RECIPE", "dEpoxyXOffSet", "0", chr_data, 10, str_load_file);
	st_recipe.dEpoxyXOffSet = atof(chr_data);
	
	:: GetPrivateProfileString("RECIPE", "dEpoxyYOffSet", "0", chr_data, 10, str_load_file);
	st_recipe.dEpoxyYOffSet = atof(chr_data);
	
	:: GetPrivateProfileString("RECIPE", "nRubHSRunSpeed", "0", chr_data, 10, str_load_file);
	st_recipe.nRubHSRunSpeed = atof(chr_data);
	
	:: GetPrivateProfileString("RECIPE", "dTrayPitch_Y", "0", chr_data, 10, str_load_file);
	st_recipe.dTrayPitch_Y = atof(chr_data);
	
	:: GetPrivateProfileString("RECIPE", "dTrayPitch_X", "0", chr_data, 10, str_load_file);
	st_recipe.dTrayPitch_X = atof(chr_data);
	
	//kwlee 2017.0421
	for (i = 0; i<MAX_BCR_CNT; i++)
	{
		for (int j =0; j<MAX_BOAT_CNT; j++)
		{

			str_Temp.Format("Boat_%d_X_Pos_%d",i+1,j+1);
			:: GetPrivateProfileString("CARRIER", str_Temp, "0", chr_data, 10, str_load_file);
			st_BoatTeaching[i].m_dPos_x[j] = atof(chr_data);
			
			str_Temp.Format("Boat_%d_Y_Pos_%d",i+1,j+1);
			:: GetPrivateProfileString("CARRIER", str_Temp, "0", chr_data, 10, str_load_file);
			st_BoatTeaching[i].m_dPos_y[j] = atof(chr_data);
			
			str_Temp.Format("Boat_%d_Z_Pos_%d",i+1,j+1);
			:: GetPrivateProfileString("CARRIER", str_Temp, "0", chr_data, 10, str_load_file);
			st_BoatTeaching[i].m_dPos_z[j] = atof(chr_data);
			
		}
	}

	:: GetPrivateProfileString("RECIPE", "fDispenserVppmA", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.fDispenserVppmA = 0;
		mstr_temp.Format("%d",st_recipe.fDispenserVppmA);
		:: WritePrivateProfileString("RECIPE", "fDispenserVppmA", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.fDispenserVppmA = mn_chk;

	//kwlee 2017.0421
	:: GetPrivateProfileString("RECIPE", "nLdBuffer_Num", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nLdBuffer_Num = 0;
		mstr_temp.Format("%d",st_recipe.nLdBuffer_Num);
		:: WritePrivateProfileString("RECIPE", "nLdBuffer_Num", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nLdBuffer_Num = mn_chk;
	
	:: GetPrivateProfileString("RECIPE", "nTrayNum", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nTrayNum = 0;
		mstr_temp.Format("%d",st_recipe.nTrayNum);
		:: WritePrivateProfileString("RECIPE", "nTrayNum", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nTrayNum = mn_chk;
	
	:: GetPrivateProfileString("RECIPE", "nCarrierBuffer_Num", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nCarrierBuffer_Num = 0;
		mstr_temp.Format("%d",st_recipe.nCarrierBuffer_Num);
		:: WritePrivateProfileString("RECIPE", "nCarrierBuffer_Num", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nCarrierBuffer_Num = mn_chk;

	:: GetPrivateProfileString("RECIPE", "nEpoxyDotScrewCount", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nEpoxyDotScrewCount = 0;
		mstr_temp.Format("%d",st_recipe.nEpoxyDotScrewCount);
		:: WritePrivateProfileString("RECIPE", "nEpoxyDotScrewCount", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nEpoxyDotScrewCount = mn_chk;

	:: GetPrivateProfileString("RECIPE", "nHsTrayY", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nHsTrayY = 0;
		mstr_temp.Format("%d",st_recipe.nHsTrayY);
		:: WritePrivateProfileString("RECIPE", "nHsTrayY", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nHsTrayY = mn_chk;
	
	:: GetPrivateProfileString("RECIPE", "nHsTrayX", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nHsTrayX = 0;
		mstr_temp.Format("%d",st_recipe.nHsTrayX);
		:: WritePrivateProfileString("RECIPE", "nHsTrayX", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nHsTrayX = mn_chk;

	:: GetPrivateProfileString("RECIPE", "nTrayY", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nTrayY = 0;
		mstr_temp.Format("%d",st_recipe.nTrayY);
		:: WritePrivateProfileString("RECIPE", "nTrayY", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nTrayY = mn_chk;

	:: GetPrivateProfileString("RECIPE", "nTrayX", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nTrayX = 0;
		mstr_temp.Format("%d",st_recipe.nTrayX);
		:: WritePrivateProfileString("RECIPE", "nTrayX", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nTrayX = mn_chk;

	:: GetPrivateProfileString("RECIPE", "nEpoxyUseLimitCont", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_recipe.nEpoxyUseLimitCont = 0;
		mstr_temp.Format("%d",st_recipe.nEpoxyUseLimitCont);
		:: WritePrivateProfileString("RECIPE", "nEpoxyUseLimitCont", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_recipe.nEpoxyUseLimitCont = mn_chk;

	/////////////
	
// 	:: GetPrivateProfileString("BasicData", "Device_Mode", "0", chr_data, 10, st_path.mstr_basic);
// 	mn_chk = atoi(chr_data);
// 	if (mn_chk < 0 || mn_chk > 1)
// 	{
// 		st_basic.m_iDeviceMode = 1;
// 		mstr_temp.Format("%d", st_basic.m_iDeviceMode);
// 		:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
// 	}
// 	else  st_basic.m_iDeviceMode = mn_chk;

	:: GetPrivateProfileString("BasicData", "Electric_Check", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 1440)
	{
		st_basic.mn_electrostatic_check_gap = 120;
		mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
		:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_electrostatic_check_gap = mn_chk;

	:: GetPrivateProfileString("BasicData", "Electrostatic_Value", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_electrostatic_value = 120;
		mstr_temp.Format("%d", st_basic.md_electrostatic_value);
		:: WritePrivateProfileString("BasicData", "Electrostatic_Value", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_electrostatic_value = md_chk;

	:: GetPrivateProfileString("BasicData", "Run_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		COMI.mn_runspeed_rate = 100;
		mstr_temp.Format("%d", COMI.mn_runspeed_rate);
		:: WritePrivateProfileString("BasicData", "Run_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  COMI.mn_runspeed_rate = mn_chk;

	:: GetPrivateProfileString("BasicData", "Manual_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		COMI.mn_manualspeed_rate = 60;
		mstr_temp.Format("%d", COMI.mn_manualspeed_rate);
		:: WritePrivateProfileString("BasicData", "Manual_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  COMI.mn_manualspeed_rate = mn_chk;

	// Data Logger에 있는 하루의 시작 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY START TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_start_time = 22;
	}
	else  st_basic.n_day_start_time = mn_chk;
	st_basic.n_day_start_time = 22;

	// Data Logger에 있는 Shift 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY SHIFT TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_shift_time = 8;
	}
	else  st_basic.n_day_shift_time = mn_chk;
	st_basic.n_day_shift_time = 8;

	:: GetPrivateProfileString("BasicData", "Day_Alarm_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		alarm.mn_alarm_occured_day = 0;
		mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  alarm.mn_alarm_occured_day = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Run_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDR = 0;
	}
	else  st_handler.m_tDR = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Run_With_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDRW = 0;
	}
	else  st_handler.m_tDRW = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Stop_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDS = 0;
	}
	else  st_handler.m_tDS = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Alarm_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDJ = 0;
	}
	else  st_handler.m_tDJ = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Maint_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDM = 0;
	}
	else  st_handler.m_tDM = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_MTBI_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDMTBI = 0;
	}
	else  st_handler.m_tDMTBI = mn_chk;
	
}

void CMyBasicData::OnDaily_Count_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20];
	
	mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
	:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	//2015.0715
	mstr_temp.Format("%d",st_handler.m_tDRef);
	:: WritePrivateProfileString("BasicData", "Daily_Ref_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDRdown);
	:: WritePrivateProfileString("BasicData", "Daily_RunDown_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
		
}

void CMyBasicData::OnBasic_Data_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString strTemp;
	CString str_save_file;
	CString str_part, str_chk_ext;
//	char chr_buf[20];
	COleDateTime time_cur;
//	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 시간정보 저장 변수
	int n_pos;
	int i = 0, j = 0; // 반복문 용

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	//str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	//kwlee 2017.0406
	str_save_file = st_path.mstr_path_Model + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			//str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
			//2017.0617
			str_save_file = st_path.mstr_path_Model + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	:: WritePrivateProfileString("BasicData", "Model_Name", LPCTSTR(st_basic.mstr_device_name), st_path.mstr_basic);	//2017.0403
	
	//kwlee 2017.0403
	:: WritePrivateProfileString("BASIC", "MODEL_NAME", LPCTSTR(st_basic.mstr_device_name), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_interface);
	:: WritePrivateProfileString("BASIC", "INTERFACE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_device);
	:: WritePrivateProfileString("BASIC", "DEVICE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_retest);
	:: WritePrivateProfileString("BASIC", "RETEST_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	//kwlee 2017.0405
	mstr_temp.Format("%d", st_basic.n_mode_tester);
	:: WritePrivateProfileString("BASIC", "TESTER_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_bcr);
	:: WritePrivateProfileString("BASIC", "BCR_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_module);
	:: WritePrivateProfileString("BASIC", "MODULE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_7387);
	:: WritePrivateProfileString("BASIC", "7387 MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_tray_type);
	:: WritePrivateProfileString("BASIC", "TRAY TYPE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	//kwlee 2017.0411
	mstr_temp.Format("%d", st_basic.n_3874InspectAlarmSkipMode);
	:: WritePrivateProfileString("BASIC", "3874_ALARMSKIP_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_7387InspectAlarmSkipMode);
	:: WritePrivateProfileString("BASIC", "7387_ALARMSKIP_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_3874InspectMode);
	:: WritePrivateProfileString("BASIC", "3874 MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_Light_Curtain_Mode);
	:: WritePrivateProfileString("BASIC", "LIGHT CURTAIN_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_case_assembly);
	:: WritePrivateProfileString("BASIC", "CASE_ASSEMBLY_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_count_pick_retry);
	:: WritePrivateProfileString("BASIC", "PICK_RETRY_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_count_partial);
	:: WritePrivateProfileString("BASIC", "PARTIAL_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_count_elevator_partial);
	:: WritePrivateProfileString("BASIC", "ELEVATOR_PARTIAL", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n7387AlarmCount);
	:: WritePrivateProfileString("BASIC", "7387ALARM_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_rubb_count);
	:: WritePrivateProfileString("BASIC", "RUBB_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_EPoxyCleanTime);
	:: WritePrivateProfileString("BASIC", "EPOXY_CLEAN_TIME", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_basic.n_HardnessUseTime);
	:: WritePrivateProfileString("BASIC", "HARDNESS_CHANGE_TIME", LPCTSTR(mstr_temp), st_path.mstr_basic);

	
	mstr_temp.Format("%d", st_basic.n_count_retry);
	:: WritePrivateProfileString("BASIC", "RETRY_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);

	
	mstr_temp.Format("%0.2f", st_basic.dEpoxyXLineOffSet);
	:: WritePrivateProfileString("BASIC", "EPOXY_X_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_basic.dEpoxyYLineOffSet);
	:: WritePrivateProfileString("BASIC", "EPOXY_Y_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHSCarrierSpreadMoveOffset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHSCarrierSpreadMoveDistance);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_DISTANCE", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatSinkCarrierSpreadMove1Offset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_10_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatSinkCarrierSpreadMove2Offset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_20_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatSinkCarrierSpreadMove3Offset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_30_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	//kwlee 2017.0416
	mstr_temp.Format("%0.2f", st_recipe.dHeatsinkRubXOffset);
	:: WritePrivateProfileString("RECIPE", "dHeatsinkRubXOffset", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatsinkRubYOffset);
	:: WritePrivateProfileString("RECIPE", "dHeatsinkRubYOffset", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nEpoxyUseLimitCont);
	:: WritePrivateProfileString("RECIPE", "nEpoxyUseLimitCont", LPCTSTR(mstr_temp), str_save_file);

	//kwlee 2017.0406
	mstr_temp.Format("%0.2f", st_recipe.dLoaderTransferTrayDeviceGap);
	:: WritePrivateProfileString("RECIPE", "dLoaderTransferTrayDeviceGap", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%0.2f", (double)st_recipe.dSatbleTime);
	:: WritePrivateProfileString("RECIPE", "dSatbleTime", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%0.2f", st_recipe.nEpoxyRunSpeed);
	:: WritePrivateProfileString("RECIPE", "nEpoxyRunSpeed", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%0.2f", st_recipe.nEpoxyXYRunSpeed[0]);
	:: WritePrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Vel", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%0.2f", st_recipe.nEpoxyXYRunSpeed[1]);
	:: WritePrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Acc", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%0.2f", st_recipe.nEpoxyXYRunSpeed[2]);
	:: WritePrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Dec", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dEpoxyXOffSet);
	:: WritePrivateProfileString("RECIPE", "dEpoxyXOffSet", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dEpoxyYOffSet);
	:: WritePrivateProfileString("RECIPE", "dEpoxyYOffSet", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.nRubHSRunSpeed);
	:: WritePrivateProfileString("RECIPE", "nRubHSRunSpeed", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%0.2f", st_recipe.dTrayPitch_Y);
	:: WritePrivateProfileString("RECIPE", "dTrayPitch_Y", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dTrayPitch_X);
	:: WritePrivateProfileString("RECIPE", "dTrayPitch_X", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%d", st_recipe.fDispenserVppmA);
	:: WritePrivateProfileString("RECIPE", "fDispenserVppmA", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%d", st_recipe.nEpoxyDotScrewCount);
	:: WritePrivateProfileString("RECIPE", "nEpoxyDotScrewCount", LPCTSTR(mstr_temp), str_save_file);
	
	//kwlee 2017.0420
	mstr_temp.Format("%d", st_recipe.nLdBuffer_Num);
	:: WritePrivateProfileString("RECIPE", "nLdBuffer_Num", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nCarrierBuffer_Num);
	:: WritePrivateProfileString("RECIPE", "nCarrierBuffer_Num", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nTrayNum);
	:: WritePrivateProfileString("RECIPE", "nTrayNum", LPCTSTR(mstr_temp), str_save_file);
	//

	mstr_temp.Format("%d", st_recipe.nHsTrayY);
	:: WritePrivateProfileString("RECIPE", "nHsTrayY", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%d", st_recipe.nHsTrayX);
	:: WritePrivateProfileString("RECIPE", "nHsTrayX", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%d", st_recipe.nTrayY);
	:: WritePrivateProfileString("RECIPE", "nTrayY", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%d", st_recipe.nTrayX);
	:: WritePrivateProfileString("RECIPE", "nTrayX", LPCTSTR(mstr_temp), str_save_file);

	//kwlee 2017.0421
	for (i = 0; i<MAX_BCR_CNT; i++)
	{
		for (int j =0; j<MAX_BOAT_CNT; j++)
		{
			strTemp.Format("Boat_%d_X_Pos_%d",i+1,j+1);
			mstr_temp.Format("%0.2f", st_BoatTeaching[i].m_dPos_x[j]);
			:: WritePrivateProfileString("CARRIER", strTemp, LPCTSTR(mstr_temp), str_save_file);

			strTemp.Format("Boat_%d_Y_Pos_%d",i+1,j+1);
			mstr_temp.Format("%0.2f",st_BoatTeaching[i].m_dPos_y[j]);
			:: WritePrivateProfileString("CARRIER", strTemp, LPCTSTR(mstr_temp), str_save_file);

			strTemp.Format("Boat_%d_Z_Pos_%d",i+1,j+1);
			mstr_temp.Format("%0.2f",st_BoatTeaching[i].m_dPos_z[j]);
			:: WritePrivateProfileString("CARRIER", strTemp, LPCTSTR(mstr_temp), str_save_file);
		}
	}

	/* ************************************************************************** */

	/*
	mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
	:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.m_iDeviceMode);
	:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%0.2f", st_basic.md_electrostatic_value);
	:: WritePrivateProfileString("BasicData", "ELECT_VALUE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", COMI.mn_runspeed_rate);
	:: WritePrivateProfileString("BasicData", "Run_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", COMI.mn_manualspeed_rate);
	:: WritePrivateProfileString("BasicData", "Manual_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	:: WritePrivateProfileString("BasicData", "Device_Type", LPCTSTR(st_basic.mstr_device_name), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
	:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

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

	mstr_temp.Format("%d",st_handler.m_tDMTBI);
	:: WritePrivateProfileString("BasicData", "Daily_MTBI_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	*/
}

CString CMyBasicData::OnGet_File_Name()
{
	CString str_temp;       // 임시 저장 변수
	CString str_save_file;  // 저장 파일 임시 설정 변수
	CString str_chk_ext;    // 파일 확장자 저장 변수
	CString str_part, str_part2;
	CString str_new_save_file;
	int n_pos;
	
	// **************************************************************************
    // Socket Contact Count 데이터 저장할 파일 설정한다 [파일 확장자 검사]        
    // **************************************************************************
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		str_save_file += _T(".DAT");  // 확장자 추가
	}
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정

		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				st_other.str_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, st_other.str_abnormal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청
			}
		}
	}
	// **************************************************************************
	
	return str_save_file;  // 파일명 리턴 
}

void CMyBasicData::OnDeviec_Folder_Load()
{

}

void CMyBasicData::OnDevice_Folder_Save()
{

}

void CMyBasicData::OnInterface_Data_Load()
{
	int i;
	CString str_tmp, str_name;
	char ch_data[30];

	for(i=0; i<10; i++)
	{
		str_name.Format("%02d_CLIENT_IP", i);
		:: GetPrivateProfileString("INTERFACE_SCREEN", str_name, "", ch_data, 30, st_path.mstr_file_basic);
		st_client[i].str_ip.Format("%s", ch_data);

		str_name.Format("%02d_CLIENT_PORT", i);
		:: GetPrivateProfileString("INTERFACE_SCREEN", str_name, "", ch_data, 30, st_path.mstr_file_basic);
		st_client[i].n_port = atoi(ch_data);

		str_name.Format("%02d_SERVER_PORT", i);
		:: GetPrivateProfileString("INTERFACE_SCREEN", str_name, "", ch_data, 30, st_path.mstr_file_basic);
		st_server[i].n_port = atoi(ch_data);

		str_name.Format("PORT_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "1", ch_data, 20, st_path.mstr_file_basic);
		rs_232.n_serial_port[i] = atoi(ch_data);
		
		str_name.Format("BAUDRATE_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "9600", ch_data, 20, st_path.mstr_file_basic);
		rs_232.n_serial_baudrate[i] = atoi(ch_data);
		
		str_name.Format("DATA_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "8", ch_data, 20, st_path.mstr_file_basic);
		rs_232.n_serial_data[i] = atoi(ch_data);
		
		str_name.Format("STOP_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "1", ch_data, 20, st_path.mstr_file_basic);
		rs_232.n_serial_stop[i] = atoi(ch_data);
		
		str_name.Format("PARITY_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "0", ch_data, 20, st_path.mstr_file_basic);
		rs_232.n_serial_parity[i] = atoi(ch_data);	
	}

	//kwlee 2017.0414
	:: GetPrivateProfileString("BASIC", "CIP_REAR", "", ch_data, 16, st_path.mstr_basic);
	str_tmp = ch_data;
	str_tmp.MakeUpper();     // 문자열 대문자로 변경
	str_tmp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	str_tmp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_client[CLS_REAR].str_ip = str_tmp;
	
	:: GetPrivateProfileString("BASIC", "CPORT_REAR", "", ch_data, 10, st_path.mstr_basic);
	str_tmp = ch_data;
	str_tmp.MakeUpper();     // 문자열 대문자로 변경
	str_tmp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	str_tmp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_client[CLS_REAR].n_port = atoi(str_tmp);
	//

	:: GetPrivateProfileString("Interface", "EQUIP_ID", "", ch_data, 20, st_path.mstr_basic);
	str_tmp = ch_data;

//	st_work.str_equip_id = str_temp;  // 암호 전역 변수에 설정

	:: GetPrivateProfileString("Interface", "BOARD_TYPE", "", ch_data, 20, st_path.mstr_basic);
	str_tmp = ch_data;
	
//	st_work.str_board_type = str_temp;  // 암호 전역 변수에 설정

	:: GetPrivateProfileString("Interface", "PGM_REVISION", "", ch_data, 20, st_path.mstr_basic);
	str_tmp = ch_data;
	
//	st_work.str_pgm_revision= str_temp;  // 암호 전역 변수에 설정
}

void CMyBasicData::OnInterface_Data_Save()
{
	int i;
	CString str_tmp="", str_name="";

	for(i=0; i<10; i++)
	{
		str_name.Format("%02d_CLIENT_IP", i);
		str_tmp.Format("%s", st_client[i].str_ip);
		:: WritePrivateProfileString("INTERFACE_SCREEN", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
		
		str_name.Format("%02d_CLIENT_PORT", i);
		str_tmp.Format("%d", st_client[i].n_port);
		:: WritePrivateProfileString("INTERFACE_SCREEN", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
		
		str_name.Format("%02d_SERVER_PORT", i);
		str_tmp.Format("%d", st_server[i].n_port);
		:: WritePrivateProfileString("INTERFACE_SCREEN", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);

		str_name.Format("PORT_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_port[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
		
		str_name.Format("BAUDRATE_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_baudrate[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
		
		str_name.Format("DATA_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_data[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
		
		str_name.Format("STOP_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_stop[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
		
		str_name.Format("PARITY_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_parity[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_file_basic);
	}

//	str_temp = st_work.str_equip_id;
	:: WritePrivateProfileString("Interface", "EQUIP_ID", LPCTSTR(str_tmp), st_path.mstr_basic);

//	str_temp = st_work.str_board_type;
	:: WritePrivateProfileString("Interface", "BOARD_TYPE", LPCTSTR(str_tmp), st_path.mstr_basic);

//	str_temp = st_work.str_pgm_revision;
	:: WritePrivateProfileString("Interface", "PGM REVISION", LPCTSTR(str_tmp), st_path.mstr_basic);
}

void CMyBasicData::OnBasic_Data_Save_As(CString str_device)
{
	CString mstr_temp,strTemp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	CString str_pos;
	COleDateTime time_cur;

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	:: WritePrivateProfileString("FILE_NAME", "Device_Type", LPCTSTR(str_device), st_path.mstr_file_basic);

	//str_save_file = /*st_path.mstr_basic*/_T("C:\\AMT820\\motor\\") + str_device;  // 티칭 데이터 저장 파일 설정
	//kwlee 2017.0403
	str_save_file = st_path.mstr_path_Model + str_device;  // 티칭 데이터 저장 파일 설정

	mstr_temp.Format("%d", st_work.n_grid_r[0][0]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_IN_ON_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_g[0][0]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_IN_ON_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_b[0][0]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_IN_ON_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_grid_r[0][1]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_IN_OFF_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_g[0][1]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_IN_OFF_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_b[0][1]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_IN_OFF_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_grid_r[1][0]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_OUT_ON_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_g[1][0]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_OUT_ON_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_b[1][0]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_OUT_ON_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_grid_r[1][1]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_OUT_OFF_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_g[1][1]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_OUT_OFF_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_grid_b[1][1]);
	:: WritePrivateProfileString("IO_COLOR", "GRID_OUT_OFF_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_text_r[0][0]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_IN_ON_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_g[0][0]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_IN_ON_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_b[0][0]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_IN_ON_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_text_r[0][1]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_IN_OFF_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_g[0][1]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_IN_OFF_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_b[0][1]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_IN_OFF_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_text_r[1][0]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_OUT_ON_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_g[1][0]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_OUT_ON_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_b[1][0]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_OUT_ON_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);
	//
	mstr_temp.Format("%d", st_work.n_text_r[1][1]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_OUT_OFF_R", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_g[1][1]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_OUT_OFF_G", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	mstr_temp.Format("%d", st_work.n_text_b[1][1]);
	:: WritePrivateProfileString("IO_COLOR", "TEXT_OUT_OFF_B", LPCTSTR(mstr_temp), st_path.mstr_file_basic);

	//kwlee 2017.0403
	:: WritePrivateProfileString("BASIC", "MODEL_NAME", LPCTSTR(str_device), str_save_file);
	//kwlee 2017.0404
// 	mstr_temp.Format("%d", st_basic.n_mode_work);
// 	:: WritePrivateProfileString("BASIC", "WORK_MODE", LPCTSTR(mstr_temp), str_save_file);
// 
// 	mstr_temp.Format("%d", st_basic.n_mode_turn);
// 	:: WritePrivateProfileString("BASIC", "TURN_MODE", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp.Format("%d", st_basic.n_mode_device);
	:: WritePrivateProfileString("BASIC", "DEVICE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_retest);
	:: WritePrivateProfileString("BASIC", "RETEST_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	//kwlee 2017.0405
	mstr_temp.Format("%d", st_basic.n_mode_tester);
	:: WritePrivateProfileString("BASIC", "TESTER_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_bcr);
	:: WritePrivateProfileString("BASIC", "BCR_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_module);
	:: WritePrivateProfileString("BASIC", "MODULE_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_7387);
	:: WritePrivateProfileString("BASIC", "7387 MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_tray_type);
	:: WritePrivateProfileString("BASIC", "TRAY TYPE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	//kwlee 2017.0411
	mstr_temp.Format("%d", st_basic.n_3874InspectAlarmSkipMode);
	:: WritePrivateProfileString("BASIC", "3874_ALARMSKIP_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_7387InspectAlarmSkipMode);
	:: WritePrivateProfileString("BASIC", "7387_ALARMSKIP_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_3874InspectMode);
	:: WritePrivateProfileString("BASIC", "3874 MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_Light_Curtain_Mode);
	:: WritePrivateProfileString("BASIC", "LIGHT CURTAIN_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_mode_case_assembly);
	:: WritePrivateProfileString("BASIC", "CASE_ASSEMBLY_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_count_pick_retry);
	:: WritePrivateProfileString("BASIC", "PICK_RETRY_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_count_partial);
	:: WritePrivateProfileString("BASIC", "PARTIAL_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_count_elevator_partial);
	:: WritePrivateProfileString("BASIC", "ELEVATOR_PARTIAL", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n7387AlarmCount);
	:: WritePrivateProfileString("BASIC", "7387ALARM_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_rubb_count);
	:: WritePrivateProfileString("BASIC", "RUBB_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_EPoxyCleanTime);
	:: WritePrivateProfileString("BASIC", "EPOXY_CLEAN_TIME", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_basic.n_HardnessUseTime);
	:: WritePrivateProfileString("BASIC", "HARDNESS_CHANGE_TIME", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.n_count_retry);
	:: WritePrivateProfileString("BASIC", "RETRY_COUNT", LPCTSTR(mstr_temp), st_path.mstr_basic);


	mstr_temp.Format("%0.2f", st_basic.dEpoxyXLineOffSet);
	:: WritePrivateProfileString("BASIC", "EPOXY_X_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_basic.dEpoxyYLineOffSet);
	:: WritePrivateProfileString("BASIC", "EPOXY_Y_OFFSET", LPCTSTR(mstr_temp), str_save_file);

	
	//kwlee 2017.0417
	mstr_temp.Format("%0.2f", st_recipe.dHSCarrierSpreadMoveOffset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHSCarrierSpreadMoveDistance);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_DISTANCE", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatSinkCarrierSpreadMove1Offset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_10_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatSinkCarrierSpreadMove2Offset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_20_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatSinkCarrierSpreadMove3Offset);
	:: WritePrivateProfileString("RECIPE", "CARRIER_MOVE_30_OFFSET", LPCTSTR(mstr_temp), str_save_file);
	
	//kwlee 2017.0416
	mstr_temp.Format("%0.2f", st_recipe.dHeatsinkRubXOffset);
	:: WritePrivateProfileString("RECIPE", "dHeatsinkRubXOffset", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dHeatsinkRubYOffset);
	:: WritePrivateProfileString("RECIPE", "dHeatsinkRubYOffset", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nEpoxyUseLimitCont);
	:: WritePrivateProfileString("RECIPE", "nEpoxyUseLimitCont", LPCTSTR(mstr_temp), str_save_file);

	//kwlee 2017.0406
	mstr_temp.Format("%0.2f", st_recipe.dLoaderTransferTrayDeviceGap);
	:: WritePrivateProfileString("RECIPE", "dLoaderTransferTrayDeviceGap", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dSatbleTime);
	:: WritePrivateProfileString("RECIPE", "dSatbleTime", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.nEpoxyRunSpeed);
	:: WritePrivateProfileString("RECIPE", "nEpoxyRunSpeed", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.nEpoxyXYRunSpeed[0]);
	:: WritePrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Vel", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.nEpoxyXYRunSpeed[1]);
	:: WritePrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Acc", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.nEpoxyXYRunSpeed[2]);
	:: WritePrivateProfileString("RECIPE", "nEpoxyXYRunSpeed_Dec", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dEpoxyXOffSet);
	:: WritePrivateProfileString("RECIPE", "dEpoxyXOffSet", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dEpoxyYOffSet);
	:: WritePrivateProfileString("RECIPE", "dEpoxyYOffSet", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.nRubHSRunSpeed);
	:: WritePrivateProfileString("RECIPE", "nRubHSRunSpeed", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dTrayPitch_Y);
	:: WritePrivateProfileString("RECIPE", "dTrayPitch_Y", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%0.2f", st_recipe.dTrayPitch_X);
	:: WritePrivateProfileString("RECIPE", "dTrayPitch_X", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.fDispenserVppmA);
	:: WritePrivateProfileString("RECIPE", "fDispenserVppmA", LPCTSTR(mstr_temp), str_save_file);
	
	//kwlee 2017.0421
	mstr_temp.Format("%d", st_recipe.nLdBuffer_Num);
	:: WritePrivateProfileString("RECIPE", "nLdBuffer_Num", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nCarrierBuffer_Num);
	:: WritePrivateProfileString("RECIPE", "nCarrierBuffer_Num", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nTrayNum);
	:: WritePrivateProfileString("RECIPE", "nTrayNum", LPCTSTR(mstr_temp), str_save_file);
	//

	mstr_temp.Format("%d", st_recipe.nEpoxyDotScrewCount);
	:: WritePrivateProfileString("RECIPE", "nEpoxyDotScrewCount", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nHsTrayY);
	:: WritePrivateProfileString("RECIPE", "nHsTrayY", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nHsTrayX);
	:: WritePrivateProfileString("RECIPE", "nHsTrayX", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nTrayY);
	:: WritePrivateProfileString("RECIPE", "nTrayY", LPCTSTR(mstr_temp), str_save_file);
	
	mstr_temp.Format("%d", st_recipe.nTrayX);
	:: WritePrivateProfileString("RECIPE", "nTrayX", LPCTSTR(mstr_temp), str_save_file);

	//kwlee 2017.0421
	for (int i = 0; i<MAX_BCR_CNT; i++)
	{
		for (int j =0; j<MAX_BOAT_CNT; j++)
		{
			strTemp.Format("Boat_%d_X_Pos_%d",i+1,j+1);
			mstr_temp.Format("%0.2f",st_BoatTeaching[i].m_dPos_x[j]);
			:: WritePrivateProfileString("CARRIER", strTemp, LPCTSTR(mstr_temp), str_save_file);
			
			strTemp.Format("Boat_%d_Y_Pos_%d",i+1,j+1);
			mstr_temp.Format("%0.2f",st_BoatTeaching[i].m_dPos_y[j]);
			:: WritePrivateProfileString("CARRIER", strTemp, LPCTSTR(mstr_temp), str_save_file);
			
			strTemp.Format("Boat_%d_Z_Pos_%d",i+1,j+1);
			mstr_temp.Format("%0.2f",st_BoatTeaching[i].m_dPos_z[j]);
			:: WritePrivateProfileString("CARRIER", strTemp, LPCTSTR(mstr_temp), str_save_file);
		}
	}
}

void CMyBasicData::OnInterface_Data_Save_As(CString str_device)
{

}

void CMyBasicData::OnMaintenance_Data_Save_As(CString str_device)
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
//	char chr_buf[20] ;
	CString str_save_file;;

	// **************************************************************************
	// 타워 램프 RED 상태 정보를 파일에 저장한다                                 
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	
	/*
	str_save_file = st_path.mstr_basic + str_device;  // 티칭 데이터 저장 파일 설정

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_r[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), str_save_file);
	// **************************************************************************


	// **************************************************************************
	// 타워 램프 YELLOW 상태 정보를 파일에 저장한다                              
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_y[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), str_save_file);

	// **************************************************************************

	// **************************************************************************
	// 타워 램프 GREEN 상태 정보를 파일에 저장한다                               
	// ->  STOP			= 0,
// 		   RUN			= 1,
// 		   ALARM		= 2,
// 		   LOTEND		= 3,
// 		   INIT			= 4,
// 		   WARRING		= 5,
// 		   LOCK			= 6,
// 		   SELFCHECK	= 7														 
	// -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                   
	// **************************************************************************
	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), str_save_file);

	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_g[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), str_save_file);

	// **************************************************************************

	// **************************************************************************
	// 부저 사용 모드 로딩하여 전역 변수에 설정                                  
	// -> 0:사용    1:미사용                                                     
	// **************************************************************************
	str_temp = LPCTSTR(_itoa(st_lamp.n_buzzer_mode, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), str_save_file);
	// **************************************************************************

	// **************************************************************************
	// 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                          
	// -> 로딩 정보에서 앞/뒤 공백은 제거한다                                    
	// **************************************************************************
	:: WritePrivateProfileString("TowerLampData", "str_equip_no", LPCTSTR(st_lamp.str_equip_no), str_save_file);
	:: WritePrivateProfileString("TowerLampData", "str_equip_code", LPCTSTR(st_lamp.str_equip_code), str_save_file);
	// **************************************************************************

	// **************************************************************************
	// 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                      
	// **************************************************************************
	str_temp = LPCTSTR(_itoa(st_lamp.n_lamp_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), str_save_file);
	// **************************************************************************
	*/
}

CString CMyBasicData::GetWaitTimeName(int n_mode)
{
	CString str_name;

	str_name = "";

	switch(n_mode)
	{
		case 0:
			str_name = "0";
			break;

		case 1:
			str_name = "1";
			break;

		case 2:
			str_name = "2";
			break;

		case 3:
			str_name = "3";
			break;

		case 4:
			str_name = "4";
			break;

		case 5:
			str_name = "5";
			break;

		case 6:
			str_name = "6";
			break;

		case 7:
			str_name = "7";
			break;

		case 8:
			str_name = "8";
			break;

		case 9:
			str_name = "9";
			break;

		case 10:
			str_name = "10";
			break;

		case 11:
			str_name = "11";
			break;

		case 12:
			str_name = "12";
			break;

		case 13:
			str_name = "13";
			break;

		case 14:
			str_name = "14";
			break;

		case 15:
			str_name = "15";
			break;

		case 16:
			str_name = "16";
			break;

		case 17:
			str_name = "17";
			break;

		case 18:
			str_name = "18";
			break;

		case 19:
			str_name = "19";
			break;
	}

	return str_name;
}

CString CMyBasicData::OnGet_Teach_File_Name( CString strName, int iOpt )
{
	CString str_temp;       // 임시 저장 변수
	CString str_save_file;  // 저장 파일 임시 설정 변수
	CString str_chk_ext;    // 파일 확장자 저장 변수
	CString str_part, str_part2;
	CString str_new_save_file;
	CString str_device_name;
	int n_pos;

	COleDateTime timeCur;
	int iYear, iMonth, iDay, iHour, iMin, iSec;
	CString strTime;
  
	n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
 
	if (n_pos == -1)
	{
		if ( iOpt == 0 )
		{
			st_basic.mstr_device_name += _T(".DAT");  // 확장자 추가
		}
		else
		{
			st_basic.mstr_device_name += _T(".TXT");  // 확장자 추가
		}
		n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	}
  
	str_device_name = st_basic.mstr_device_name.Mid(0, n_pos);
  
	if (strName.Find("Back") != -1)
	{  
		timeCur = COleDateTime::GetCurrentTime();
   
		iYear = timeCur.GetYear();
		iMonth = timeCur.GetMonth();
		iDay = timeCur.GetDay();
   
		iHour = timeCur.GetHour();
		iMin = timeCur.GetMinute();
		iSec = timeCur.GetSecond();
   
		strTime.Format("_%04d%02d%02d%02d%02d%02d", iYear, iMonth, iDay, iHour, iMin, iSec );

		str_save_file = st_path.mstr_path_back + str_device_name + strTime;  // 티칭 데이터 저장 파일 설정
	}
	else
	{
		str_save_file = st_path.mstr_path_dvc + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
  
	if (strName != "")
	{
		str_save_file += "_";
		str_save_file += strName;
	}
  
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
 
	if (n_pos == -1)
	{
		if ( iOpt == 0 )
		{
			str_save_file += _T(".DAT");  // 확장자 추가
		}
		else
		{
			str_save_file += _T(".TXT");  // 확장자 추가
		}
	}
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정

		if ( iOpt == 0 )
		{
			if (str_chk_ext != _T(".DAT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.DAT");  // 티칭 데이터 저장 새로운 파일 설정
    
				if (st_handler.cwnd_list != NULL) // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청
				}
			}
		}
		else
		{
			if (str_chk_ext != _T(".TXT"))  
			{
				str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
    
				if (st_handler.cwnd_list != NULL) // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청			
				}
			}
		}
	}
	// **************************************************************************
  
	return str_save_file;  // 파일명 리턴 
}


