// APartHandler.h: interface for the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
#define AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ENUM_MACHINE_POSITION
{
	EMPOS_FRONT,
	EMPOS_REAR,
	EMPOS_ALONE,
};

class APartHandler  
{
public:
	APartHandler();
	virtual ~APartHandler();

	void Thread_Run();
	void Run_Move_Main();
	//////////////////////////////////////////////////////////////////////////
	//int mn_run_status;  // 장비 동작 상태 정보 저장 변수 (O:STOP   1:RUN    2:ALARM    3:MAINT    4:INIT)

	void SetMachinePos( ENUM_MACHINE_POSITION empos ) { m_empos = empos; }
	ENUM_MACHINE_POSITION GetMachinePos() { return m_empos; }

	void AddAlarmCnt();
	void AddEpoxyCnt();
	void AddHardnessTime();
	int GetAlarmCnt() { return m_nAlarmCnt; }
	int GetEpoxyCnt() { return m_nEpoxyCnt; }
	int GetHardnessTime() { return m_nHardnessTime; }
	void ClearAlarmCnt();
	void ClearEpoxyCnt();
	void ClearHardnessTime();

	void HsInterfaceChk();
	void Case_Assembly_Able_Check();
	void Case_Assembly_Place_Check();
	void HS_Able_Signal(int OnOff); 
	void Set_Light_Curtain_Muting_On();
	void Set_Light_Curtain_Muting_Off();
	void Set_Interface_HeartBit();
	void Get_Interface_HeartBit();
	int OnLightCurtainCheck();
	int OnEmoCheck();
	void ClearStep();
	int MoveCleanEpoxy();
	int MoveSafetyEpoxy();
	int MoveHardnessOut();
	int MoveHardnessTestOut();
	int MoveEpoxyOut();
	void AlarmEpoxyCleanTime();
	void AlarmHardnessExcessTimeEpoxyCnt();


public:
	int m_nEpoxyMoveStep;
	int m_nEpoxyCleanTimeStep;
	int m_nEpoxySafetyStep;
	int m_nHardnessStep;
	int m_nHardnessTestStep;
	int m_nEpoxyOutStep;
	int m_nHardnessEpoxyStep;
	int nLightCurtainMutingOnStep;
	int nLightCurtainMutingOffStep;
	DWORD lLightCurtainMutingOnTime[3];
	DWORD lLightCurtainMutingOffTime[3];
	DWORD m_dwEpoxyCheckTime[3];

	int nSetInterfaceHeartBitStep;
	int nGetInterfaceHeartBitStep;
	DWORD lSetInterfaceHeartBitTime[3];
	DWORD lGetInterfaceHeartBitTime[3];
	int n_InterfaceHeartBitCount;
	int m_nChkEpoxyYes;


protected:
	ENUM_MACHINE_POSITION m_empos;	
	int	m_nAlarmCnt;
	int m_nEpoxyCnt;
	int m_nHardnessOut;
	int	m_nHardnessTime;
	int	m_nSafetyChk[6];
	int m_safetyStep;
	DWORD m_dwWaitSafetyTime[3];
	DWORD m_dwHardnessWaitTime[3];
	DWORD m_dwEpoxyOutWaitTime[3];
	DWORD m_dwHardnessEpoxyWaitTime[3];
};

extern APartHandler	g_handler;

#endif // !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
