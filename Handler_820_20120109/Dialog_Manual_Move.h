#if !defined(AFX_DIALOG_MANUAL_MOVE_H__CACD5E57_1C88_4547_A3F9_9455A38B8E2B__INCLUDED_)
#define AFX_DIALOG_MANUAL_MOVE_H__CACD5E57_1C88_4547_A3F9_9455A38B8E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_Move.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move dialog
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"

#define TM_TRAY1			901
#define TM_TRAY2			902
#define TM_TRAY_TRANSFER	903
#define TM_CARRIER_MOVE		904
#define TM_UNPRESS_MOVE		905
#define TM_VISION_MOVE		906
#define TM_EPOXY_MOVE		907
#define TM_HEATSINK_MOVE	908
#define TM_HSVISION_MOVE	909
#define TM_DISPENSOR		910
#define TM_LOADER_MOVE		911
#define TM_LOAD_PLATE		912
#define TM_DVCBUFFER		913
#define TM_ONLYDISPENSOR	920
#define TM_ONLYREVERSE		921
#define TM_ONLYHSPICKPLACE	922
#define TM_VISIONCHK		923

class CDialog_Manual_Move : public CDialog
{
public:
	int				m_n_part;

	CPoint			m_cp_coord;
	CFont			*m_p_font;	

// Construction
public:
	int mn_retry_cnt;
	int m_nRetry;
	int nJobFlag;
	int m_move_step[100];
	int nTrayExist;
	int mn_RunTopFwdStep;
	int m_nTrayDeviceGap;
	int mn_RunDownStep;
	int mn_RunUpStep;
	int mn_RunBtmFwdStep;
	int mn_jig_rotator;
	int mn_move_carrier;
	int mn_BufferPos;
	int mn_SafetyStep;
	int mn_MoveStep;
	int mn_dotPos;
	int m_nCarriorPos;
	int m_nlast_pick;
	BOOL bHeatsinkMeasureCmp;
	int m_nVisCarriorPos;
	int m_nVisTeachPos;
	int m_nWork_Pos;
	int m_LoadBufferPos;
	int m_hs_rub;

	int mn_FirstSecond;
	int mn_StartEnd;

	int ml_motflag[3];
	int m_Thread_Flag[5];

	int nTransferTrayExist;
	int m_nTransfer_WaitPosMove_Flag;
	DWORD m_dwWaitTime[3];
	DWORD m_dwWaitUntil[3];
	DWORD m_dwUpCarrierWaitTime[3];
	CString m_strAlarmCode;
	int nLdPlate_Tray_Supply_Req[30];
	int nWorkTransfer_Req[30][5];
	int m_nTransferJobFlag[30];
	double m_dpTargetPos[30];
	int nCarrierRbt_Dvc_Req[30][5];
	int nCarrierSateyflag[30];
	double m_dTargetPos[100];
	int m_nhsexystop;
	int m_nvisionstop;
	int m_hs_vision;

	int nHeatSinkRbt_Dvc_Req[30][5];
	DWORD m_dwWaitDispenserAirBlow[3];
	int m_npSet_WorkPosYXCPB[5];

	int nLdWorkRbt_Dvc_Req[30][5];
	int m_npFindWorkPosYXCPB[5];
	int nUldWorkRbt_Dvc_Req[30][5];

	DWORD m_dwSatbleWaitTime[3];

	int	m_n_FirstPicker_Num;
	int	m_n_FirstTray_X_Num;
	int	m_n_FirstTray_Y_Num;

	int m_npBuff_Info[4];
	int	m_npBuff_Status[4];
	int	m_npBuff_OutputStatus[4];


	void Init_Data();
	void Init_Show();
	void Init_Group();
	void Init_Button();
	void Init_Timer();
	BOOL Create();
	CDialog_Manual_Move(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Manual_Move)
	enum { IDD = IDD_DIALOG_MANUAL_MOVE };
	CComboBox	m_cbo_vision;
	CComboBox	m_cbo_hs_rub;
	CComboBox	m_cbo_carrier;
	CComboBox	m_cbo_tray_transfer;
	CComboBox	m_cbo_tray2;
	CComboBox	m_cbo_tray1;
	CButtonST	m_btn_exit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_Move)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nTray1;
	int m_nTray2;
	int m_nTrayTransfer;
	void OnButtonControl(bool bflag);
	int Move_TrayElv1();
	int Move_TrayElv2();
	int Move_TrayTransfer();
	int Move_CarrierTransfer();
	int Move_UnPressTransfer();
	int Move_EpoxyTransfer();
    int Move_HeatSinkTransfer();
	int Move_VisionTransfer();
	int Move_Dispensor();
	int Move_LoadPlate();
	int Move_LoadTransfer();
	int Move_BufferTransfer();
	int Move_DvcBuffer();

	int CarrierTopForward( int nMode );
	int CheckCarrierType();
	int CarrierMoveDown();
	int CarrierMoveUp();
	int CarrierBtmForwrad();
	int Check_Carrier_Move_Enable( int nMode);
	int Robot_Move_Safety_Zone( int nMode, int n_site, int n_flag);
	int Move_Billiard_Epoxy( int nMode, int nSite);
	int Move_BuffDispensor();
	int Move_HSPickHeatSinkTransfer( int m_hs_rub_pick );

protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Manual_Move)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnElevatorGo1();
	afx_msg void OnElevatorGo2();
	afx_msg void OnElevatorGo3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReqTrayChange();
	afx_msg void OnBtnEndlessLoop();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnHeatSinkGo();
	afx_msg void OnButton1();
	afx_msg void OnBtnVisionGo();
	afx_msg void OnButton15();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MANUAL_MOVE_H__CACD5E57_1C88_4547_A3F9_9455A38B8E2B__INCLUDED_)
