#if !defined(AFX_RUN_DEVICE_CARRIER_ROBOT_H__1DD93297_FD9C_4E69_80E2_4571D0BE9F23__INCLUDED_)
#define AFX_RUN_DEVICE_CARRIER_ROBOT_H__1DD93297_FD9C_4E69_80E2_4571D0BE9F23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Device_Carrier_Robot.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Device_Carrier_Robot command target
enum carrier_buffer_move_info_shift
{
	TOPSHIFT_BUFF_LOADER_RECEIVE = 0,
	TOPSHIFT_BUFF_INPUT_LOADER,			
	TOPSHIFT_BUFF_EPOXY,					
	TOPSHIFT_BUFF_WAIT_INDEX,				
	TOPSHIFT_BUFF_HEATSINK_VISION,			
	TOPSHIFT_BUFF_OUTSEND,					
	TOPSHIFT_BUFF_UNLOADER,					
	BTMSHIFT_BUFF_DOWN,
	BTMSHIFT_BUFF_DOWNFORWARD,
	BTMSHIFT_BUFF_HEATSINK_DOWN,
	BTMSHIFT_BUFF_INDEX_DOWN,
	BTMSHIFT_BUFF_EPOXY_DOWN,
	BTMSHIFT_BUFF_INPUT_DOWN,
	BTMSHIFT_BUFF_LOADER_DOWN,
	TOPSHIFT_BUFF_LOADER_PICKERDATA_RECEIVE,	
	TOPSHIFT_DATA_TEMP_CHECK,
	TOPSHIFT_IDBUFF_SEALING_SITE_ALL_CHK,
	BTMSHIFT_DATA_TEMP_CHECK,
	BTMSHIFT_IDBUFF_SEALING_SITE_ALL_CHK,
};


class CRun_Device_Carrier_Robot : public CObject
{
public:	
	DECLARE_SERIAL(CRun_Device_Carrier_Robot)

	CRun_Device_Carrier_Robot();           // protected constructor used by dynamic creation
	virtual ~CRun_Device_Carrier_Robot();

// Attributes
public:
	int mn_RunMove;
	int mn_RunCarrierStatus;
	int mn_RunTopFwdStep;
	int mn_RunBtmFwdStep;
	int mn_RunUpStep;
	int mn_RunUpDownStep;
	int mn_RunDownStep;
	int mn_RunBcrStep;
	int mn_reinstate_step;
	int mn_InitStep;
	int nBcrNum;

	CString m_strFindLotNo;
	int m_nInspectAxis;
	int m_nEpoxy[4];
	int m_nRobot_Y;
	
	int mn_retry_cnt;
	long ml_retry_wait[3];

	int mn_BcrNum;
	int mn_Count;

	int m_nTransferJobFlag[2];
	//work Ʈ�����ۿ� ������ ��û�Ѵ� , [0]:��û����, [1]:1:Ʈ���̸� ����, 2:Ʈ���̸� ����	
	char cJamcode[10];


	int m_nTransfer_WaitPosMove_Flag;

	double m_dcurr_pos[3];
	int	mn_reinstate_pos[3];

	long m_dwWaitTime[3];
	long m_dwUpCarrierWaitTime[3];

	int m_nFindLotNo_Flag[2][2]; //[0][0] ���� �ε� [0][1] ���� ��ε� 
											//[1][0] �Ķ� �ε� [1][1] �Ķ� ��ε�
	CString m_strLotNo;
	CString m_strPartNo;
	CString m_strAlarmCode;

	double m_dpTargetPos;


	//Press
	bool m_bPressUpDnFlag[2];
	DWORD m_dwPressUpDn[3];

	//Top fwdbwd�� ���� uodown �Ǹ���
	bool m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];

	//BTM fwdbwd�� ���� uodown �Ǹ���
	bool m_bBtmFwdBwdFlag;
	DWORD m_dwBtmFwdBwd[3];


	//Carrier Left Right Clamp���� ���Ǹ�  [0]* : Left [1][*] : Right
	bool m_bCarrierClampFlag[2];  
	DWORD m_dwCarrierClampFwdBwd[2][3];

	//Carrier_Slide_Guide_Z 1,2 UP DOWN
	bool m_bCarrierSlideGuide[2];  
	DWORD m_dwCarrierGuideUpDn[2][3];

	//Carrier pin ����1��° sol�� ����
	//Carrier pin ����2,3,6,��°
	//Carrier vison pin ���� 5��°
	bool m_bCarrierPinFix[3];  
	DWORD m_dwCarrierPinFix[3][3];

	char mc_jamcode[10];

	
	DWORD m_dwBcrWaitTime[3];
	int mn_BufferPos;

	int nRetryCyl[4];

	int m_Thread_Flag[4];

public:
	void Thread_Run();
	void RunInit();
	void RunMove();

	void RunReadBcr();
	void AllBufferClear();

	int CarrierMoveUp();
	int CarrierMoveDown();

	int CarrierTopForward( int nMode );	
	int CarrierBtmForwrad();

	int Check_RunAllSafety();
	int Check_BeforeMoveUnPressRbt( int nMode );//UNPREE�κ��� �������� �Ǹ����� ��ġüũ�Ѵ�.
	int Top_ShiftCarrierDataMoveRbt( int nMode, int nSiftSide );
	int CheckCarrierType();
	int Top_Carrierbuffer_sensor_flag_status_check( int n_mode, int n_check_site );

	int Btm_ShiftCarrierDataMoveRbt( int nMode, int nSiftSide );
	int Btm_Carrierbuffer_sensor_flag_status_check( int n_mode, int n_check_site );

	//PRESS
// 	void Set_Device_Press_UpDn_Cylinder(int OnOff);
// 	int Chk_Device_Press_UpDn_Cylinder( int nOnOff );
// 
	//TOP fwd bwd�� ���� up down �Ǹ���(���Ͱ�  �Ǹ����� �ٿ��Ű�� ���ڿ� �δ�.
// 	void Set_Device_Carrier_Slide_Top_X_UpDown(int OnOff);
// 	int Chk_Device_Carrier_Slide_Top_X_UpDown( int OnOff );

	//BTM fwd bwd
	void Set_Device_Carrier_Slide_Bottom_X_ForBackward(int OnOff);
	int Chk_Device_Carrier_Slide_Bottom_X_ForBackward(int OnOff);

	//Carrier L R Up Down�� ���� clamp����
	void Set_Device_Carrier_Clamp_FwdBwd(int nMode, int OnOff);
	int Chk_Device_Carrier_Clamp_FwdBwd(int nMode, int OnOff );

	//LEFT_RIGHT CARRIER UP DOWN
	void Set_Device_Carrier_Slide_Z_Cylinder_UpDown(int nMode,int OnOff);
	int Chk_Device_Carrier_Slide_Z_Cylinder_UpDown(int nMode, int nOnOff );

	//Carrier pin ����1��° sol�� ����
	//Carrier pin ����2,3,6,��°
	//Carrier vison pin ���� 5��°
	void Set_Device_Carrier_HolderPin_Fix(int nMode, int OnOff);
	int Chk_Device_Carrier_HolderPin_Fix(int nMode, int nOnOff );


	int RunMoveUp();
	int RunMoveDown();
	int Check_Carrier_Move_Enable( int nMode);
	int CheckCarrierStatus( int nCycle);

};

/////////////////////////////////////////////////////////////////////////////
extern CRun_Device_Carrier_Robot Run_Device_Carrier_Robot;
#endif // !defined(AFX_RUN_DEVICE_CARRIER_ROBOT_H__1DD93297_FD9C_4E69_80E2_4571D0BE9F23__INCLUDED_)
