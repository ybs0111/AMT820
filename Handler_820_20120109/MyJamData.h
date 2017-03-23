// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_)
#define AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyJamData  
{
public:
public:
	CMyJamData();
	virtual ~CMyJamData();
	
// Attributes
public:
//	CAlarmListFile mcls_alarm_list;  // �˶� ����Ʈ ���� ���� Ŭ���� ���� ����
	
// Operations
public:
	CString On_Alarm_Display_Info_Load(CString strPath, CString strDataFile, CString strActionFile, CString strCode);  // �߻� �˶� ���� ���� �Լ�
	
	CString On_Alarm_Bmp_Data_Load(CString strPath, CString strDataFile, CString strCode);       // �߻� �˶� ���� ǥ�ÿ� �׸� ���ϸ� ���� �Լ�
	CString On_Alarm_Style_Data_Load(CString strPath, CString strDataFile, CString strCode);     // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	CString On_Alarm_Repair_Data_Load(CString strPath, CString strActionFile, CString strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	
	void On_Alarm_Display();					// �߻��� �˶� ȭ�� ��� �Լ�
	void On_Occured_Alarm_Saving_to_File();		// �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�
	void On_Alarm_Info_Load();					// ���Ͽ� ����� ��� �˶� ���� ���� ������ �����ϴ� �Լ�
	void On_Alarm_Info_Set_to_Variable(CString strErrCode);  // �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�
};
extern CMyJamData			MyJamData;
#endif // !defined(AFX_MYJAMDATA_H__7F7F1DF4_BB1D_46A4_AFA1_76841EFF5980__INCLUDED_)
