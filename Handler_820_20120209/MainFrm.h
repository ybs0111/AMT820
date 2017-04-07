// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__716C2E1B_366E_43F6_B442_E9A3C17AC4DF__INCLUDED_)
#define AFX_MAINFRM_H__716C2E1B_366E_43F6_B442_E9A3C17AC4DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* ****************************************************************************** */
/* Initial Dialog Bar 관련 클래스 추가 부분                                       */
/* ****************************************************************************** */
#include "Dialog_Title.h"	// 타이틀 정보 출력 대화 상자 클래스 추가 
#include "Dialog_List.h"	// 리스트 정보 출력 대화 상자 클래스 추가 
/* ****************************************************************************** */
#include "TrueColorToolBar.h"
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Dialog_Alarm.h"	// 알람 출력 화면 클래스 추가 
/* ****************************************************************************** */
/* 설정 정보 로딩 및 저장 클래스                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */
#include "SerialComm.h"     // 시리얼 통신 클래스
#include "PPTooltip.h"
#include "Dialog_Event_Msg.h"

#define TMR_FILE_CREATE		1011
#define TMR_MAIN_REFRESH	1012
#define TOOLBAR_DRAW_BUTTON_WIDTH 32

class  CDialog_Alarm;		// 모달리스 알람 화면 대화 상자에 대한 인스턴스 선언
class  CClientSocket;
class  CServerSocket;
class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CServerSocket	*m_p_server[10];
	CClientSocket	*m_p_client[10];

	CPPToolTip		m_p_tooltip;
	SerialComm		m_ports[MAX_PORT];       // 시리얼 포트 클래스 변수
	CString			mstr_received[MAX_PORT];    // 수신 데이터 임시 저장 변수
	CView			*m_pNewActiveView;
	CView			*m_pOldActiveView;
	// **************************************************************************
	// Initial Dialog Bar 관련 클래스 포인터 변수 선언                           
	// **************************************************************************
	CDialog_Title	m_wndTitleBar;  // 타이틀 바 대화 상자에 대한 클래스 변수 선언 
	CDialog_List    m_wndListBar;   // 리스트 정보 대화 상자에 대한 클래스 변수 선언 
	// **************************************************************************

	// **************************************************************************
	// 화면 최소화 관련 변수 선언                                                
	// **************************************************************************
	static	BOOL	IsFlash;
	// **************************************************************************
	int				tHandle;
	int				i_summary_flag;
	CString			m_str_new_device;
	CString			m_str_load_file;

	CMyBasicData    mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	CMyJamData      mcls_frm_alarm;		// 알람 출력 클래스
	CDialog_Alarm*  mp_alarm_dlg;		// 알람 화면에 대한 포인터 변수 선언 
	CDialog_Event_Msg*  mp_msg_dlg;

// Operations
public:
	void OnMain_Thread_Destroy();
	void OnMain_Thread_Creating();
	void OnExitData_Saving();
	void OnMain_Var_Default_Set();
	void OnMain_Path_Set();
	int OnMenu_Change_Checking();
	void OnSwitchToForm(int nForm);
	void OnMainFrame_SelectMessageDisplay();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init_ToolTip();
	void OnMain_Create_Device();
	int OnMain_New_Device_Check();
	void OnDialog_Delete();
	void OnMain_Motor_Setting();
	void OnListError();
	void OnListWait();//kwlee 2017.0406
	void OnListRecipe();//kwlee 2017.0407
	void OnOpen(int nPort); //kwlee 2017.0406
	void OnMain_Port_Close();
	void OnMain_Port_Create(int n_port);
	void Init_View();
	void OnAlarm_Destroy();
	
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CTrueColorToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMain();
	afx_msg void OnBasic();
	afx_msg void OnSetting();
	afx_msg void OnMotor();
	afx_msg void OnList();
	afx_msg void OnIo();
	afx_msg void OnLevel();
	afx_msg void OnSave();
	afx_msg void OnAlarm();
	afx_msg void OnAbout();
	afx_msg void OnAdmin();
	afx_msg void OnReset();
	afx_msg void OnLock();
	afx_msg void OnExit();
	afx_msg void OnClose();
	afx_msg void OnListOperation();
	afx_msg void OnListWork();
	afx_msg void OnListAlarm();
	afx_msg void OnListStep();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnPgmInfo();
	afx_msg void OnMotorSpeed();
	//}}AFX_MSG
	afx_msg LRESULT OnViewChangeMode(WPARAM wParam,LPARAM lParam) ;		// Post Message에 대한 화면 전환 사용자 사용자 정의 메시지 선언 
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT* plRes);	// 텍스트 툴바 드롭다운 메시지 선언 
	afx_msg LRESULT OnMainframe_Work(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnCommand_Client_1(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnCommand_Server_1(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCommunication(UINT, LONG);							// RS-232C 시리얼 포트 제어 메시지
	afx_msg LONG OnCommunicationEvent(UINT, LONG);						// RS-232C 시리얼 포트 이벤트 설정 메시지
	afx_msg LRESULT OnDataSend(WPARAM wParam, LPARAM lParam);			// RS-232C 시리얼 포트 통한 데이터 송신 메시지
	afx_msg LRESULT OnSerialPort(WPARAM wParam,LPARAM lParam) ;			// 시리얼 포트 생성/삭제 메시지 선언 
	afx_msg void OnDataChange(WPARAM wParam,LPARAM lParam) ;	
	afx_msg LRESULT OnMainMessageEvent(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__716C2E1B_366E_43F6_B442_E9A3C17AC4DF__INCLUDED_)
