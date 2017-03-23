// Run_Handler_Check.h: interface for the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_HANDLER_CHECK_H__F68D8FCE_85A6_45B1_9AEB_B63406B372E8__INCLUDED_)
#define AFX_RUN_HANDLER_CHECK_H__F68D8FCE_85A6_45B1_9AEB_B63406B372E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRun_Handler_Check  
{
public:
	int m_n_separator_motor;
	int	Reinstatement[5];
	int i_stop_press_cnt;
	int RejectTrayStep;
	int StartStep;
	int StopStep;
	int LoaderStep;
	int ResetStep;
	int SoundStep;
	int FrontDoorStep;
	
	long on_reject_change_time[3];
	long off_reject_change_time[3];
	long SwitchWaitTime[3];
	long FrontDoorWaitTime[3];

public:
	void ButtonCheck_Loader();
	void ButtonCheck_BuzzerOff();
	void ButtonCheck_Reset();
	void ButtonCheck_Stop();
	void ButtonCheck_Start();
	void OnReject_Tray_Reset();
	void HandlerCheck_RejectTray();
	int Run_Possible_Check();

	CRun_Handler_Check();
	virtual ~CRun_Handler_Check();

};

extern CRun_Handler_Check Run_Handler_Check;
#endif // !defined(AFX_RUN_HANDLER_CHECK_H__F68D8FCE_85A6_45B1_9AEB_B63406B372E8__INCLUDED_)
