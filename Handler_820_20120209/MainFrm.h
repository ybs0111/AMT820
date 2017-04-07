// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__716C2E1B_366E_43F6_B442_E9A3C17AC4DF__INCLUDED_)
#define AFX_MAINFRM_H__716C2E1B_366E_43F6_B442_E9A3C17AC4DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* ****************************************************************************** */
/* Initial Dialog Bar ���� Ŭ���� �߰� �κ�                                       */
/* ****************************************************************************** */
#include "Dialog_Title.h"	// Ÿ��Ʋ ���� ��� ��ȭ ���� Ŭ���� �߰� 
#include "Dialog_List.h"	// ����Ʈ ���� ��� ��ȭ ���� Ŭ���� �߰� 
/* ****************************************************************************** */
#include "TrueColorToolBar.h"
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "Dialog_Alarm.h"	// �˶� ��� ȭ�� Ŭ���� �߰� 
/* ****************************************************************************** */
/* ���� ���� �ε� �� ���� Ŭ����                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */
#include "SerialComm.h"     // �ø��� ��� Ŭ����
#include "PPTooltip.h"
#include "Dialog_Event_Msg.h"

#define TMR_FILE_CREATE		1011
#define TMR_MAIN_REFRESH	1012
#define TOOLBAR_DRAW_BUTTON_WIDTH 32

class  CDialog_Alarm;		// ��޸��� �˶� ȭ�� ��ȭ ���ڿ� ���� �ν��Ͻ� ����
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
	SerialComm		m_ports[MAX_PORT];       // �ø��� ��Ʈ Ŭ���� ����
	CString			mstr_received[MAX_PORT];    // ���� ������ �ӽ� ���� ����
	CView			*m_pNewActiveView;
	CView			*m_pOldActiveView;
	// **************************************************************************
	// Initial Dialog Bar ���� Ŭ���� ������ ���� ����                           
	// **************************************************************************
	CDialog_Title	m_wndTitleBar;  // Ÿ��Ʋ �� ��ȭ ���ڿ� ���� Ŭ���� ���� ���� 
	CDialog_List    m_wndListBar;   // ����Ʈ ���� ��ȭ ���ڿ� ���� Ŭ���� ���� ���� 
	// **************************************************************************

	// **************************************************************************
	// ȭ�� �ּ�ȭ ���� ���� ����                                                
	// **************************************************************************
	static	BOOL	IsFlash;
	// **************************************************************************
	int				tHandle;
	int				i_summary_flag;
	CString			m_str_new_device;
	CString			m_str_load_file;

	CMyBasicData    mcls_m_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	CMyJamData      mcls_frm_alarm;		// �˶� ��� Ŭ����
	CDialog_Alarm*  mp_alarm_dlg;		// �˶� ȭ�鿡 ���� ������ ���� ���� 
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
	afx_msg LRESULT OnViewChangeMode(WPARAM wParam,LPARAM lParam) ;		// Post Message�� ���� ȭ�� ��ȯ ����� ����� ���� �޽��� ���� 
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT* plRes);	// �ؽ�Ʈ ���� ��Ӵٿ� �޽��� ���� 
	afx_msg LRESULT OnMainframe_Work(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnCommand_Client_1(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnCommand_Server_1(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCommunication(UINT, LONG);							// RS-232C �ø��� ��Ʈ ���� �޽���
	afx_msg LONG OnCommunicationEvent(UINT, LONG);						// RS-232C �ø��� ��Ʈ �̺�Ʈ ���� �޽���
	afx_msg LRESULT OnDataSend(WPARAM wParam, LPARAM lParam);			// RS-232C �ø��� ��Ʈ ���� ������ �۽� �޽���
	afx_msg LRESULT OnSerialPort(WPARAM wParam,LPARAM lParam) ;			// �ø��� ��Ʈ ����/���� �޽��� ���� 
	afx_msg void OnDataChange(WPARAM wParam,LPARAM lParam) ;	
	afx_msg LRESULT OnMainMessageEvent(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__716C2E1B_366E_43F6_B442_E9A3C17AC4DF__INCLUDED_)
