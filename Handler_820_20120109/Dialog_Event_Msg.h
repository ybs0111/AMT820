#if !defined(AFX_DIALOG_EVENT_MSG_H__6067D395_1FF7_43C0_8EE6_727BBF0EFEFD__INCLUDED_)
#define AFX_DIALOG_EVENT_MSG_H__6067D395_1FF7_43C0_8EE6_727BBF0EFEFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Event_Msg.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "MacButtons.h"

#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
/////////////////////////////////////////////////////////////////////////////
// CDialog_Event_Msg dialog

class CDialog_Event_Msg : public StandardDialog
{
// Construction
public:
	//CDialog_Event_Msg(CWnd* pParent = NULL);   // standard constructor
	CDialog_Event_Msg(CString str1, CString str2, CString str3, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Event_Msg)
	enum { IDD = IDD_DIALOG_EVENT_MSG };
	CMacButton	m_btn_ok;
	CNewLabel	m_label_event_msg;
	//}}AFX_DATA

// Attributes
private:
	CButton *mp_btn_ok;
// 	CFont* mp_event_font;						// 폰트 설정 변수 선언 
	
public:
	void OnEventMsg_Button_Set();
	void OnEventMsg_Text_Set();
	void OnEventMsg_Label_Set();

	CString mstr_event_msg[3];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Event_Msg)
	public:
	virtual BOOL Create();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Event_Msg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_EVENT_MSG_H__6067D395_1FF7_43C0_8EE6_727BBF0EFEFD__INCLUDED_)
