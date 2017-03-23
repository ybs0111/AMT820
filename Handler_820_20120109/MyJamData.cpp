// MyJamData.cpp: implementation of the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MyJamData.h"

#include "Public_Function.h"  // ���� ���� �� I/O ���� ���� Ŭ����
#include "io.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyJamData			MyJamData;
CMyJamData::CMyJamData()
{

}

CMyJamData::~CMyJamData()
{

}

// ******************************************************************************
// �߻� �˶� ���� ���� �Լ�                                                      
// -> strPath     : �˶� ���� ���� ���� ���� ����                                
// -> strDataFile : �˶� ���� ����� File Name(File Name+Ȯ����)                       
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CMyJamData::On_Alarm_Display_Info_Load(CString strPath, CString strDataFile, CString strActionFile, CString strCode)
{
	CString str_file_path;  // ���� ���� ����
	CString str_alarm_chk;  // �Լ� ���� ���� ���� ����
	int n_pos;

	str_file_path = strPath;
	if (str_file_path.Right(1) != "\\")  // ���� ��� ������ ���� �˻�
	{
		str_file_path += "\\";
	}

	str_alarm_chk = On_Alarm_Bmp_Data_Load(str_file_path, strDataFile, strCode);  // �߻� �˶� ���� ǥ�ÿ� �׸� File Name ���� �Լ�
	n_pos = str_alarm_chk.Find("Trouble");
	if (n_pos >= 0)  // ���� �߻�
	{
		return str_alarm_chk;
	}

	str_alarm_chk = On_Alarm_Style_Data_Load(str_file_path, strDataFile, strCode);  // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	n_pos = str_alarm_chk.Find("Trouble");
	if (n_pos >= 0)  // ���� �߻�
	{
		return str_alarm_chk;
	}

	str_alarm_chk = On_Alarm_Repair_Data_Load(str_file_path, strActionFile, strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	n_pos = str_alarm_chk.Find("Trouble");
	if (n_pos >= 0)  // ���� �߻�
	{
		return str_alarm_chk;
	}

	return str_alarm_chk;
}
// ******************************************************************************

// ******************************************************************************
// �߻� �˶� ���� ǥ�ÿ� �׸� File Name ���� �Լ�                                
// -> strPath     : �˶� ǥ�� �׸� ���� ���� ����                                
// -> strDataFile : �˶� ǥ�� �׸� File Name(File Name+Ȯ����)                   
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CMyJamData::On_Alarm_Bmp_Data_Load(CString strPath, CString strDataFile, CString strCode)
{
	CString str_return_data;  // �Լ� ���� �÷���
	CString str_file;  // �˶� ǥ�ÿ� �׸��� ���� [����]+[File Name] ���� ����
	CString str_temp;  // �˶� ǥ�ÿ� �׸� File Name �ӽ� ���� ����

	char chr_buf[1000];

	str_return_data = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	str_file = strPath + strDataFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, "DEF", "", chr_buf, 1000, str_file))
	{
		str_return_data = "File Name : "+ str_file;
		str_return_data += "\r\n";
		str_return_data += "[DEF] The information we read. Trouble -> Code : ";
		str_return_data += strCode;

		return str_return_data;
	}

	str_temp.Format("%s", chr_buf);
	st_alarm.str_bmp_file = str_temp;  // �˶� ǥ�ÿ� �׷� File Name ����

	return str_return_data;
}
// ******************************************************************************

// ******************************************************************************
// �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�                               
// -> strPath     : ���� ǥ�� ���� ���� ���� ����                                
// -> strDataFile : ���� ǥ�� ���� ���� File Name(File Name+Ȯ����)              
// -> strCode     : �߻��� �˶� Code                                             
// ******************************************************************************
CString CMyJamData::On_Alarm_Style_Data_Load(CString strPath, CString strDataFile, CString strCode)
{
	CString str_chk_data;		// �Լ� ���� �÷���
	CString str_chk_file;		// �˶� ���� ǥ�� ������ ���� [����]+[File Name] ���� ����
	CString str_chk, str_chk2;  // �˶� ���� ǥ�� ���� �� ��ġ ���� ����
	int start=0, end, i;		// ���� ������ ��ġ ���� ����
	int max=4;					// ���� ������ �ִ� ���� (ȭ��ǥ ������� �ʱ�ȭ)

	char chr_buf[1000];
	char num[20];

	str_chk_data = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	str_chk_file = strPath + strDataFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, "STYLE", "", chr_buf, 1000, str_chk_file))
	{
		str_chk_data = "File Name : "+ str_chk_file;
		str_chk_data += "\r\n";
		str_chk_data += "[STYLE] The information we read. Trouble -> Code : ";
		str_chk_data += strCode;

		return str_chk_data;
	}

	str_chk.Format("%s", chr_buf);

	for(i=0; i<max; i++)
	{
		end = str_chk.Find(';', start);
		if (end == -1)  // ������ ������
		{
			break;
		}
		else            // ������ ����
		{
			str_chk2 = str_chk.Mid(start, end-start);  // ������ ������ �� �� ������ ����
			str_chk2.TrimLeft(' ');
			str_chk2.TrimRight(' ');

			if (i == 0)  // �˶� ���� ǥ�� ����
			{
				st_alarm.str_style = str_chk2;       // �˶� ���� ǥ�� ���� ����
				if (st_alarm.str_style == "CIRCLE")  // ���� ǥ�� ���� 8�� ����
				{
					max = 8;    
				}
				else if (st_alarm.str_style == "NONE")
				{
					max = 1;
				}
			}
			else
			{
				lstrcpy(num, str_chk2);
				if (i == 1)
				{
					st_alarm.n_Cx = atoi(num);         // X�� �ȼ� ��ġ
				}
				else if (i == 2)
				{
					st_alarm.n_Cy = atoi(num);         // Y�� �ȼ� ��ġ
				}
				else if (i == 3)
				{
					st_alarm.n_Offset = atoi(num);     // �� ũ��
				}
				else if (i == 4)
				{
					st_alarm.n_LineWidth = atoi(num);  // ���� �β�
				}
				else if (i == 5)
				{
					st_alarm.n_R = atoi(num);          // RGB �� R ��
				}
				else if (i == 6)
				{
					st_alarm.n_G = atoi(num);          // RGB �� G ��
				}
				else if (i == 7) 
				{
					st_alarm.n_B = atoi(num);          // RGB �� B ��
				}
				// **************************************************************
			}

			start = end + 1;  // ���� ������ ��ġ ����
		}
	}

	if (i != max)  // Data Format ����
	{
		str_chk_data = "File Name : "+ str_chk_file;
		str_chk_data += "\r\n";
		str_chk_data += "[STYLE] Data Format. Trouble -> Code : ";
		str_chk_data += strCode;

		return str_chk_data;
	}

	return str_chk_data;
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶��� ���� ��ġ ���� ���� �Լ�                                        
// -> strPath       : �˶� ��ġ ���� ����� ���� ����                            
// -> strActionFile : �˶� ��ġ ���� ����� File Name(File Name+Ȯ����)          
// -> strCode       : �߻��� �˶� Code                                           
// ******************************************************************************
CString CMyJamData::On_Alarm_Repair_Data_Load(CString strPath, CString strActionFile, CString strCode)
{
	CString str_return_val;		// �Լ� ���� �÷���
	CString str_repair_file;	// ��ġ ���׿� ���� [����]+[File Name] ���� ����
	CString str_act_item;		// ��ġ ���� ���� ��ġ ���� ����
	CString str_act_temp;		// ��ġ ���� �ӽ� ���� ����
	int n_chk_cnt;				// ��ġ ���� ���� ���� ����
	int i; 
	
	char chr_buf[1000];

	str_return_val = _T("OK");  // �Լ� ���� �÷��� �ʱ�ȭ

	str_repair_file = strPath + strActionFile;  // [����]+[File Name] ����

	if (!GetPrivateProfileString(strCode, "COUNT", "0", chr_buf, 10, str_repair_file))
	{
		str_return_val = "File Name : "+ str_repair_file;
		str_return_val += "\r\n";
		str_return_val += "[COUNT] The information we read. Trouble -> Code : ";
		str_return_val += strCode;

		return str_return_val;
	}

	n_chk_cnt = atoi(chr_buf);  // ���� ��ġ ���� ���� ����
	if (n_chk_cnt<1)
	{
		str_return_val = "File Name : "+ str_repair_file;
		str_return_val += "\r\n";
		str_return_val += "[COUNT] Data Format Trouble -> Code : ";
		str_return_val += strCode;

		return str_return_val;
	}
	else
	{
		st_alarm.n_action_cnt = n_chk_cnt;
	}

	(st_alarm.str_repair_msg).Empty();  //  ��ġ ���� ���� ���� �ʱ�ȭ

	for (i=0; i<(st_alarm.n_action_cnt); i++)   
	{
		str_act_item.Format("%d", i+1);  // ��ġ ���� ���� ���� ��ġ ����
		if (!GetPrivateProfileString(strCode, (LPCTSTR)str_act_item, "", chr_buf, 1000, str_repair_file))
		{
			str_return_val = "File Name : "+ str_repair_file;
			str_return_val += "\r\n";
			str_return_val += "An action subject. The information we read. Trouble";

			str_return_val += "\r\n";
			str_return_val += "Code : ";
			str_return_val += strCode;

			str_return_val += "\r\n";
			str_return_val += "Position : ";
			str_return_val += str_act_item;

			return str_return_val;
		}
		// **********************************************************************

		st_alarm.str_repair_msg += "\r\n";			//��ġ ���׿� �� ù���� ����.

		str_act_temp.Format(" %s", chr_buf);  // ��ġ ���� ����
		st_alarm.str_repair_msg += str_act_temp;

		if (i != ((st_alarm.n_action_cnt)-1))  
			st_alarm.str_repair_msg += "\r\n";  // ��ġ ���� �ܶ� ������ ����
	}
	// **************************************************************************
	
	return str_return_val;
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶� ȭ�� ��� �Լ�                                                    
// ******************************************************************************
void CMyJamData::On_Alarm_Display()
{
	CString str_cnt, str_temp;
	char temp_code[10], temp_cnt[10], chr_buf[20]={0,};
	
	CString str_display_time;	// ���� ��¿� �ð� ���� ����
	CString str_alarm_code;		// �߻��� �˶� �ڵ� ���� ����
	CString str_alarm_msg;		// �߻��� �˶� �޽��� ���� ����
	int n_cur_hour;				// ���� �ð� ���� ����
	int n_chk;					// ������ �˶� �ڵ� ���� ����
	long n_chk_time;			// �˶� �߻� �ð� ���� ���� ����
	int n_data_save = 0;		// Jam�� ������ ���̳�?

	COleDateTime cur_time;
	CTime alarm_occured_time;

	// **************************************************************************
	// ��� ���� �� �˶� �߻��ߴ��� �˻��Ѵ�                                     
	// -> �˶� �߻��� ��� �α� ���� �� �ӽ� ���Ͽ� �����ϰ� ȭ�鿡 ����Ѵ�     
	// **************************************************************************
//	alarm.mstr_code = "111111";
	if (st_work.n_run_status == dJAM || st_work.n_run_status == dWARNING || st_work.n_run_status == dLOTEND)  // �˶� �߻�
	{
		if (st_handler.n_alarm_screen == TRUE)	return;

		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_abnormal_msg = "Alarm Code = " + alarm.mstr_code;
			sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}

		// **********************************************************************
		// �˶� ȭ�� ��� �Ұ����� ��� �˻��Ѵ�.                                
		// -> st_handler.n_system_lock : SYSTEM LOCK ���� �÷���                 
		// **********************************************************************
		if (st_handler.n_system_lock != FALSE)  // SYSTEM LOCK
		{
			return;  // ���� ����
		}
		// **********************************************************************

		// **************************************************************************
		// �˶� �߻��� �ð� �����Ѵ�                                                 
		// **************************************************************************
		cur_time = COleDateTime::GetCurrentTime();
		n_cur_hour = cur_time.GetHour();

		alarm_occured_time = CTime::GetCurrentTime();
		str_display_time = alarm_occured_time.Format("%c");
		// **************************************************************************

		// **********************************************************************
		// �˶� ȭ�� ��� ���� �� �˶� ī��Ʈ ���� �˻��Ѵ�                      
		// -> �̹� �˶� ȭ�� ��� ���� ��� �� ����� �ʿ� ������                
		//    �� ��쿡 �߻��ϴ� �˶��� �����Ѵ�                                 
		//  : �˶� ȭ�� ��� �߿� �˶� Code ����Ǹ� ��� �޽��� �޶����� ����   
		// -> st_handler.n_alarm_screen : �˶� ȭ�� ��� ���� �÷���             
		// -> st_alarm.n_cur_state : �˶� ���� ���� �÷���                       
		// **********************************************************************
		if (st_handler.n_alarm_screen != TRUE)  // �˶� ȭ�� �����
		{
			if (st_work.n_run_status == dLOTEND)		
			{
				alarm.mn_reset_status = NO;
			}
			else	alarm.mn_reset_status = YES;

			//�߻��� �˶��� List�� �ִ´�.
			if (alarm.mstr_code != "")
			{
				st_handler.str_last_alarm = alarm.mstr_code;		// ������ �˶��� �����Ѵ�.
				// **************************************************************************
				// ���� �߻��� �˶��� ���� �޽��� ���� �����Ѵ�                              
				// **************************************************************************
				On_Alarm_Info_Set_to_Variable(alarm.mstr_code);  // �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�
				// **************************************************************************
				sprintf(temp_code, "%s", alarm.mstr_code);
				str_cnt.Format("%d", st_alarm.mn_cur_state);
				sprintf(temp_cnt, "%s", str_cnt);

				// **************************************************************************
				// �ٷ� ���� �߻��� �˶� �ڵ�� ���Ͽ� ������ ��� �˶� ī��Ʈ ������      
				// -> �˶� ī��Ʈ �ð� ������ 30�ʷ� �����Ѵ�                                
				// **************************************************************************
				alarm.stl_cur_alarm_time = GetCurrentTime();

				n_chk = atoi(alarm.mstr_code);
				
				if (alarm.stl_prev_code == n_chk)  // �˶� �ڵ� ����
				{
					n_chk_time = alarm.stl_cur_alarm_time - alarm.stl_prev_alarm_time; 
					
					if (n_chk_time < 30000) 
					{
						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
					}
					else
					{
						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
						n_data_save = YES;			// Jam�� ���� �� Count�� �Ѵ�.
					}
				}
				else
				{
					// **********************************************************************
					// �߻��� �˶� ������ ���� ������ ��ü�Ѵ�                               
					// -> ������ �߻��ϴ� �˶��� ���ϴ� ������ ���ȴ�                    
					// **********************************************************************
					alarm.stl_prev_code = n_chk;
					alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
					// **********************************************************************

					n_data_save = YES;				// Jam�� ���� �� Count�� �Ѵ�.
				}
				// **************************************************************************
			}

			if (n_data_save == YES)
			{
//				Dll_JamDataLog(2, temp_code, temp_cnt);
//
				if (st_alarm.mn_cur_state == 1)				// Count�ϴ� Alarm�̸�... 2K4/08/13/ViboX
				{
					alarm.mn_alarm_occured++;
					alarm.mn_alarm_occured_day++;

					str_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
					:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(str_temp), st_path.mstr_basic);
				}

				On_Occured_Alarm_Saving_to_File();  // �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�
			}

			st_handler.n_alarm_screen = TRUE;	// �˶� ȭ�� ��� �÷��� ����
			
			Func.OnSet_IO_Port_Alarm();			// �˶� ���� I/O ��� �Լ�

			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 1);  // ���� �����ӿ� �˶� ȭ�� ��� ��û 

			if (st_handler.cwnd_title != NULL)  // Ÿ��Ʋ �� ��� �˻�
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.n_run_status);  // Ÿ��Ʋ [�˶�] ��� ��û
		}
		// **********************************************************************
	}
	// **************************************************************************
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�                                         
// ******************************************************************************
void CMyJamData::On_Occured_Alarm_Saving_to_File()
{
	CString str_display_time;	// ���� ��¿� �ð� ���� ����
	CString str_alarm_code;		// �߻��� �˶� Code ���� ����
	CString str_alarm_msg;		// �߻��� �˶� �޽��� ���� ����
	int n_cur_hour;				// ���� �ð� ���� ����

	COleDateTime cur_time;
	CTime alarm_occured_time;

	CString str_file_month, str_file_day, str_title_time;
	int n_cur_year, n_cur_month, n_cur_day;		// ��, ��, �� ���� ����

	CString str_month_path, str_day_path;

	FILE *fp;
	CString Load_file = _T("");

	// **************************************************************************
	// ���� �˶� �α� ���� ���� ���� ��� ����                                   
	// -> ���� �� �������� ���� �߻��� �˶� ���� �����ϱ� ���� ���� ������       
	//    �����ϰ� �Ǹ� �α� ���� ���� ���� ���� ����� �� �ִ�                  
	// -> �α� ���� ������ ��Ȯ���� �����ϱ� ���� �� �� �߻��� �˶� ������ ����  
	// -> st_handler.n_calculate : �˶� �α� ������ ���� ���� �÷���             
	// **************************************************************************
// 	if (st_handler.n_calculate != FALSE)
// 	{
// 		return;  // ���� ����
// 	}
	// **************************************************************************

	// **************************************************************************
	// �˶� �߻��� �ð� �����Ѵ�                                                 
	// -> SHIFT ���� �˻� �� �˶� �߻� �ð� �����ϱ� ���Ͽ� �ʿ�                 
	// **************************************************************************
	cur_time = COleDateTime::GetCurrentTime();
	n_cur_hour = cur_time.GetHour();

	alarm_occured_time = CTime::GetCurrentTime();
	str_display_time = alarm_occured_time.Format("%c");
	// **************************************************************************

	// **************************************************************************
	// �˶� Code �� �˶� �޽��� �Լ� ������ �����Ѵ�                             
	// -> ����� �� �Լ� ȣ�� ���� ���ο� �˶��� �߻��ϴ� ��� �˶� Code��       
	//    ��ġ ���� �� ��Ÿ ������ ��ġ���� ���� �� �ִ�                         
	// -> alarm.mstr_code : ���� �߻��� �˶� Code ���� ���� ����                  
	// -> st_alarm.str_cur_msg : �߻��� �˶� �޽��� ���� ����                    
	// **************************************************************************
	str_alarm_code = alarm.mstr_code;
	str_alarm_msg = st_alarm.mstr_cur_msg;
	// **************************************************************************

	// **************************************************************************
	// �Ϻ�, ���� �˶� �߻� Ƚ�� �ӽ� ���Ͽ� �߰��Ѵ�                            
	// **************************************************************************
//	mcls_alarm_list.On_Daily_ListFile_Add(str_alarm_code, str_display_time, str_alarm_msg); // �Ϻ�, ���� �˶� ���� ����Ʈ ���Ͽ� �߰� �Լ�
	// **************************************************************************

	// **************************************************************************
	// �ٷ� ���� �߻��� �˶� Code�� ���Ͽ� ������ ��� �˶� ī��Ʈ ������      
	// -> �˶� ī��Ʈ �ð� ������ 30�ʷ� �����Ѵ�                                
	// **************************************************************************

	// **************************************************************************
	// ���� �̸����� ����� ��¥ �����Ѵ�                                        
	// **************************************************************************
	cur_time = COleDateTime::GetCurrentTime();
	alarm_occured_time = CTime::GetCurrentTime() ;

	n_cur_year = cur_time.GetYear();  
	n_cur_month = cur_time.GetMonth();  
    n_cur_day = cur_time.GetDay();  

	str_file_month.Format("%04d%02d", n_cur_year, n_cur_month);
	str_file_day.Format("%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_title_time = alarm_occured_time.Format("%Y/%m/%d %H:%M:%S");
	// **************************************************************************

	str_month_path = _T("C:\\AMT820\\Alarm\\Monthly\\MT" + str_file_month + ".TXT");
	str_day_path = _T("C:\\AMT820\\Alarm\\Daily\\DY" + str_file_day + ".TXT");

	// �� ����Ÿ ���
	if ((fp = fopen(str_month_path, "a+")) == NULL)  
	{
		AfxMessageBox("The failure because we open the file.");
		return;
	}

	fprintf(fp, "[%s] %s : %s \r\n", str_title_time, str_alarm_code, str_alarm_msg);	
	fclose(fp);
	// �� ����Ÿ ���
	if ((fp = fopen(str_day_path, "a+")) == NULL)  
	{
		AfxMessageBox("The failure because we open the file.");
		return;
	}

	fprintf(fp, "[%s] %s : %s \r\n", str_title_time, str_alarm_code, str_alarm_msg);	
	fclose(fp);

	// **************************************************************************
	// SHIFT�� �˶� �߻� Ƚ�� �ӽ� ���� ���Ͽ� �˶� �߰��Ѵ�                     
	// -> �˶� �߻� Ƚ�� �ӽ� ���� ���� ������ �� ������ �˶� ����Ʈ ���� �̸��� 
	//    �Ϻ� ���� �̸����� ����� ������ ������ �����ϴ�                       
	// **************************************************************************
	if (n_cur_hour>=6 && n_cur_hour<14)  // SHIFT �� DAY
	{
//		mcls_alarm_list.On_Shift_File_Add(1, str_alarm_code, str_display_time);  // SHIFT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
	}
	else if (n_cur_hour >= 14 && n_cur_hour < 22)  // SHIFT �� SWING
	{
//		mcls_alarm_list.On_Shift_File_Add(2, str_alarm_code, str_display_time);  // SHIFT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
	}
	else  // SHIFT �� GY
	{
//		mcls_alarm_list.On_Shift_File_Add(3, str_alarm_code, str_display_time);  // SHIFT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
	}
	// **************************************************************************

	// **************************************************************************
	// LOT�� �˶� �߻� Ƚ�� �ӽ� ���Ͽ� �߰��Ѵ�                                 
	// **************************************************************************
//	mcls_alarm_list.On_Lot_ListFile_Add(str_alarm_code, str_display_time);  // LOT�� �˶� �߻� Ƚ�� ���� �ӽ� ���Ͽ� �߰� �Լ�
	// **************************************************************************
}
// ******************************************************************************

// ******************************************************************************
// ���Ͽ� ����� ��� �˶� ���� ���� ������ �����ϴ� �Լ�                        
// ******************************************************************************
void CMyJamData::On_Alarm_Info_Load()
{
	CString str_load_file;
	CString str_read_data;  // ���Ͽ� ����� �� ���� ���� ���� ����
	CString str_temp_data;
	CString str_comment_data;
	int n_count, n_len;
	int n_pos = 0 ;

	char chr_s[300];
	FILE  *fp ;  // ���� �ڵ� ���� ����

	str_load_file = _T("c:\\AMT820\\Bmp\\Error.txt"); 

	// **************************************************************************
    // �˶� ���� ������ ���� ����ü ��� ���� �ʱ�ȭ                             
    // **************************************************************************
	for (n_count = 0; n_count < 2000; n_count++)  
	{
		(st_alarm.mstr_e_content[n_count]).Empty();  // ���Ͽ��� �ε��� �� ���� ���� ���� ���� �ʱ�ȭ
		(st_alarm.mstr_e_code[n_count]).Empty();     // �˶� Code ���� ���� �ʱ�ȭ
		(st_alarm.mstr_e_state[n_count]).Empty();    // �˶� ���� ���� ���� �ʱ�ȭ
		(st_alarm.mstr_e_part[n_count]).Empty();     // �˶� �߻� ���� ���� ���� �ʱ�ȭ
		(st_alarm.mstr_e_msg[n_count]).Empty();      // �˶� �޽��� ���� ���� �ʱ�ȭ
	}
	// **************************************************************************

	// **************************************************************************
    // �˶� ���� ����� ���� ����                                                
    // **************************************************************************
	if ((fp=fopen(str_load_file,"rt"))==NULL)  
	{
		AfxMessageBox("The failure because we open the file.");
		return ;
	}
	// **************************************************************************

	// **************************************************************************
    // ������ ���� �������� ��� �о� ���� ������ �����Ѵ�                       
    // **************************************************************************
	while(!feof(fp))  // ���� ������ �˻�
	{
		sprintf(chr_s, "");
		
        if (fgets(chr_s,300,fp)==NULL && ferror(fp))
		{   
			AfxMessageBox("The failure because we read the file.");
			return ;
		}
		
		str_read_data.Format("%s",chr_s);  // ���Ͽ��� ���� �� ���� ���� ����

		n_len = lstrlen(str_read_data);		//���� üũ

		if (!str_read_data.IsEmpty() && n_len > 2)
		{
			str_comment_data = str_read_data.Left(2);		//�ּ������� Ȯ���Ѵ�.

			if (str_comment_data != "//")					//�ּ����� �ƴ϶��...
			{
				st_alarm.mstr_e_content[n_pos] = str_read_data ;         // ���Ͽ��� �ε��� �� ���� ���� ����
				st_alarm.mstr_e_code[n_pos] = str_read_data.Left(6) ;    // �˶� Code ����
				st_alarm.mstr_e_state[n_pos] = str_read_data.Mid(7,1) ;  // �˶� ���� ����
				st_alarm.mstr_e_part[n_pos] = str_read_data.Mid(9,1) ;   // �˶� �߻� ���� ����

				str_temp_data = str_read_data.Mid(12,270) ;
				
				if (st_alarm.mstr_e_code[n_pos] != "999999")
				{
					st_alarm.mstr_e_msg[n_pos] = str_temp_data.Left(str_temp_data.GetLength() - 3) ;  // ���� ���� �˶� �޽��� ����
				}
				else
				{
					st_alarm.mstr_e_msg[n_pos] = str_temp_data.Left(str_temp_data.GetLength() - 1) ;  // ���� ���� �˶� �޽��� ����
				}
				n_pos++;
			}
		}
	}
	// **************************************************************************

	fclose(fp) ;  // ���� ����
}
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�                                  
// ******************************************************************************
void CMyJamData::On_Alarm_Info_Set_to_Variable(CString strErrCode)
{
	int n_cnt ;
	int n_cur_num = -1;    // �˶��� ���� �迭 ��ġ ���� ����
	CString str_err_code;  // �˶� Code ���� ����

	// **************************************************************************
    // �Էµ� �˶� Code���� ��/�� ������ �����Ѵ�                                
    // **************************************************************************
	str_err_code = strErrCode;
	str_err_code.TrimLeft(' ');	
	str_err_code.TrimRight(' ');
	// **************************************************************************

	// **************************************************************************
    // �߻��� �˶� Code�� ���� �迭 ��ġ �˻��Ѵ�                                
    // **************************************************************************
	for(n_cnt=0; n_cnt<2000; n_cnt++)
	{  
		if ( (st_alarm.mstr_e_code[n_cnt]).Compare(str_err_code) == 0 )
		{
			n_cur_num = n_cnt ; 
			break;
		}
	}
	// **************************************************************************
	
	if (n_cur_num == -1)  // ������ �˶�
	{
		alarm.mstr_pcode = alarm.mstr_code;
		alarm.mstr_code = "999999";
		str_err_code = "999999";

		for(n_cnt=0; n_cnt<2000; n_cnt++)
		{  
			if ( (st_alarm.mstr_e_code[n_cnt]).Compare(str_err_code) == 0 )
			{
				n_cur_num = n_cnt ; 
				break;
			}
		}
		// **************************************************************************
	}
	
	// **************************************************************************
    // �߻��� �˶��� ���� ���¿� �޽��� ���� ������ �����Ѵ�                     
    // **************************************************************************
	st_alarm.mn_cur_state = atoi(st_alarm.mstr_e_state[n_cur_num]);
	st_alarm.mstr_cur_msg = st_alarm.mstr_e_msg[n_cur_num];
	// **************************************************************************
}
// ******************************************************************************
