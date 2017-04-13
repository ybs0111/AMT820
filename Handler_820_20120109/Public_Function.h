// Public_Function.h: interface for the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLIC_FUNCTION_H__460DDD39_583E_40D6_B8DC_012DB086B058__INCLUDED_)
#define AFX_PUBLIC_FUNCTION_H__460DDD39_583E_40D6_B8DC_012DB086B058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AXTLIB.h"
#include "AXTDIO.h"
#include "AxtLIBDef.h"		// Added by ClassView
#include "MyInternetSession.h"
#include <afxtempl.h>
#include "Variable.h"		// 전역 변수 정의 클래스
#include "ClientSocket.h"


class CDialog_Part_IO;
class CDialog_Motor_Part;
class CDialog_Manual_Move;

class CPublic_Function  
{
public:
	CClientSocket* m_pclient;
//	CNetWork_Comm    *m_network[NETWORK_CNT];
	CMyInternetSession *m_pInternetSession;
	CFtpConnection *m_pFtpConnection;
	CInternetFile *m_pInternetFile;
	COPYDATASTRUCT		m_msg;
public:
	CPublic_Function();
	virtual ~CPublic_Function();

public:
	char cp_cmd[BUF_SIZE];
	char m_szStatus[1024];
	CString m_strHostName;
	char Jamcode[10];
	bool CYLFlag[MAX_IO];
	long CYLTime[MAX_IO][3];
	bool SiteFlag[MAX_IO];
	long SiteTime[MAX_IO][3];
	bool IOFlag[MAX_IO];
	long IOTime[MAX_IO][3];
	int Mot_HomeStep[MAXMOTOR];
	int Mot_RetryCount;
	int Mot_CurTime[MAXMOTOR][3];
	int Mot_RetryFlag[MAXMOTOR];
	long backuptime[3];
	
	bool IOYFlag[MAX_IO];
	long IOYTime[MAX_IO][3];
	int IOYCount[MAX_IO][3];
	float IOYYield[MAX_IO];

	long ModuleAreaSensor_Time[3];
	long HeatSinkAreaSensor_Time[3];
	int ModuleAreaSensor_Flag;
	int HeatSinkAreaSensor_Flag;

	CString m_strAlarmCode;
	int nLightCurtainMutingOnStep;
	int nLightCurtainMutingOffStep;
	DWORD lLightCurtainMutingOnTime[3];
	DWORD lLightCurtainMutingOffTime[3];

	//////////////////////////////////////2K10/02/09/ViboX////////////////////////////////////

	CDialog_Part_IO		*m_p_io;
	CDialog_Motor_Part	*m_p_motor_part;
	CDialog_Manual_Move	*m_p_move;

	CString str_step_temp[100];
	CTime  StepChangeTime[100];
public: 
	int OnIO_Safety(int n_io, int n_onoff);
	CString Get_MotorName(int n_axis);
	int OnBarcode_Receive(int n_picker, int n_command, CString str_rev);
	CString OnBarcode_Send(int n_command);
//	void DB_Write_Tray(st_tray_database st_tray);
//	void DB_Write_Lot(st_lot_database st_lot);
//	void DB_Write_Device(st_dvc_database st_dvc);
	void ThreadFunctionStepTrace(int nPos, int nStep);
	int OnBCR_CheckData(CString strCheckData);
	void OnTestResultData_Save(int n_mode, CString cpBindata);
	void OnLotDataTempLoggin(int iMode);
	int Find_WhereToGoRobotTrayPos(int Site, int TrayNum, int PickerInfo[D_INFOSIZE][SORTER_PICKER_NUM], int InfoSetFlag, int BinNum);
	int Find_TrayRowColumns(int Site, int LeftRight, int RowColPos, int BinNum, int DVCYesNo, int tcountmode);
	void OnSet_Door_Open();
	int OnMotMove_SafetyCheck(int n_axis, int n_move);
	BOOL CreateFolder(LPCTSTR szPath);
	void OnDayDataReset();
	void OnDayDataCheck();
	void OnSet_IO_Port_Load_On();
	int On_Encoder_Check(double d_pos);
	int OnMotor_Go_Check(int n_motor, double d_pos);
	CString GetLocalMacAddress();
	void OnSet_IO_Port_Load_Off();
	void OnSet_IO_Port_Init();
	void OnSet_IO_Port_Run();
	void OnMot_Speed_Setting();
	int FileSizeCheck(CString FileName, long size, int n_check);
	void On_LogFile_Add(int n_msg_select, CString str_msg);
	void OnSet_IO_Port_Alarm();
	void On_IOFlagReset();
	void PublicVariableDataInitialize();
	void OnSet_IO_Port_Sound(int n_status);
	void OnSet_IO_Out_Port_Clear();
	int PickerDeviceStatusCheck(int Robot, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM], int Result[PICKER_NUM]);
	void OnSet_IO_Port_Stop();
	void OnSet_Door_Close();
	int DoorOpenCheck();
	int CheckPickBin();
	void MsgLog( LPCTSTR strMsg );
	void DeleteOldFiles( LPCTSTR lpDirPath, int iDays );
	int DoorOpenCheckSpot();
	void TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp);

	int Data_Exchange_PickPlace(int nPickPlace, int nJobCnt, int nRobotSite, int nWorkSite, int *npWorkPos);//Pick & Place시 picker & Tray data exchange 작업
	int Check_PickerStatus(int nPickerMode, int nSite, int nYesNo, int *npPickerInfo, int *npPickerStatus, int *npPickerOutputStatus);
	int Find_TrayWork_Pos(int nMode, int nSite, int nDVCYesNO, int *npFindPosYXCPB, int nRobotSite, CString str_LotNo);
	int Handler_Tray_DataInfo_Shift(int nMode, int nDvc_Yes_No, int nSend_SIte, int nRcv_Site);


	void OnDailyCycleData(CString strLotNo, CString strPartNo, double dTime, CTime tStart, CTime tEnd);
	void OnCycleTime(int nMode, CString strLotNo, CString strPartNo, DWORD dwTime1, DWORD dwTime2, DWORD dwTime3);
	void OnLotCycleData(CString strLotNo, CString strPartNo,int nCount, DWORD dwTray, DWORD dwLot);
	int Check_Robot_Buffer_Clash( int nRbtTHDSite, int BuffTHDSite, int nBuffStartPos, int nPickerStartPos, int *nErrorInfoStatus);
	int Check_BufferStatus(int nBufferMode, int nSite, int nYesNo, int *nBufferInfo, int *nBufferStatus, int *npBufferOutputStatus);
	int Check_Carrier_MoveUp_Enable( int nMode);

	int Check_RunAllSafety();
	int Check_Carrier_Move_Enable( int nMode);
	int Check_BeforeMoveUnPressRbt( int nMode );

	void VppmSet();
	void VppmOff();
	void SaveHeatSinkBoxPos();
	int Calculate_MovePos_Find(int nMode, int nMotNum, int nSite, int nWorkPart, int *npFindWorkPosYXCPB, double *dpGetTargetPos);
	void Set_Light_Curtain_Muting_On();
	void Set_Light_Curtain_Muting_Off();
	void Case_Assembly_Able_Check();
	void Case_Assembly_Place_Check();
	void HS_Able_Signal(int OnOff);
	void OnBcrReader(); //kwlee 2017.0412
	BOOL SendLocalMsg(RCV_STRUCT msg);
	BOOL Send7387MeasureStart();
	BOOL SendEpoxyMeasureStart();
	BOOL SendHeatsinkMeasureStart(int Pos);
	BOOL SendNobaPosStart();
	BOOL SendHeatSinkPosStart();
	BOOL SendInspectOsView();
	BOOL SendInspectOsDisable();

};
extern CPublic_Function  Func;
#endif // !defined(AFX_PUBLIC_FUNCTION_H__460DDD39_583E_40D6_B8DC_012DB086B058__INCLUDED_)
