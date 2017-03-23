// Run_TowerLamp.h: interface for the CRun_TowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_TOWERLAMP_H__DDA03974_99D4_4189_95AF_AC8690043920__INCLUDED_)
#define AFX_RUN_TOWERLAMP_H__DDA03974_99D4_4189_95AF_AC8690043920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRun_TowerLamp  
{
public:
	CRun_TowerLamp();
	virtual ~CRun_TowerLamp();

// Attributes
public:
	bool InitFlag;
	int	 MoveStep;
	int	 mn_lamp_step;

	int	 mn_form_step;
	long ml_form_time[3];

	long ml_lamp_time[3];

	int mn_pcb_exist_check;
	long ml_lamp_change_time[3];

// Operations
public:
	void Run_FormChange();
	void Run_Move();
	void Thread_Run();

};
extern CRun_TowerLamp Run_TowerLamp;
extern CRun_TowerLamp Run_TowerLamp_Manual;

#endif // !defined(AFX_RUN_TOWERLAMP_H__DDA03974_99D4_4189_95AF_AC8690043920__INCLUDED_)
