#if !defined(AFX_RUN_DVCLDBUFFER_H__BA1C40B6_70D4_4215_A1E3_0E768E90054B__INCLUDED_)
#define AFX_RUN_DVCLDBUFFER_H__BA1C40B6_70D4_4215_A1E3_0E768E90054B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_DvcLdBuffer.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_DvcLdBuffer command target

class CRun_DvcLdBuffer : public CObject
{
public:
	DECLARE_SERIAL(CRun_DvcLdBuffer)

	CRun_DvcLdBuffer();           // protected constructor used by dynamic creation
	virtual ~CRun_DvcLdBuffer();

// Attributes
public:
	void		Thread_Run();
	void		OnRunInit();
	void		OnRunMove();

	int			mn_RunStep;
	int			mn_InitStep;

	int			m_npFindWorkPosYXCPB[5]; //[0]first tray Y, [1]first tray X, [2]:작업가능 수량 또는 공간 정보, [3]first picker num, [4]first buff num
	int			m_nFindLotNo_Flag;
	CString		m_strFindLotNo;
	int			m_nWork_Pos; //현재 작업할 위치

	CString		m_strLotNo;
	CString		m_strPartNo;

	CString		m_strAlarmCode;


	DWORD		m_dwWaitTime[3];
	DWORD		m_dwCycleTime[3];

	int			m_npBuff_Info[4];
	int			m_npBuff_Status[4];
	int			m_npBuff_OutputStatus[4];

	bool		m_bClampOnOffFlag;
	DWORD		m_dwClampOnOff[3];

public:
	void Set_Loader_Buffer_Align_OnOff(int OnOff);
	int Chk_Loader_Buffer_Align_OnOff(int OnOff);


protected:

};

/////////////////////////////////////////////////////////////////////////////
extern CRun_DvcLdBuffer Run_DvcLdBuffer;

#endif // !defined(AFX_RUN_DVCLDBUFFER_H__BA1C40B6_70D4_4215_A1E3_0E768E90054B__INCLUDED_)
