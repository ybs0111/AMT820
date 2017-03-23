#if !defined(AFX_DIALOG_ABOUT_H__4DBEC1DF_A490_48B1_9D53_2002B4C363C8__INCLUDED_)
#define AFX_DIALOG_ABOUT_H__4DBEC1DF_A490_48B1_9D53_2002B4C363C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_About.h : header file
//

#include "StarWarsCtrl.h"  // 스타워즈 형태의 컨트롤을 생성하기 위한 클래스 추가

/////////////////////////////////////////////////////////////////////////////
// CDialog_About dialog

class CDialog_About : public StandardDialog
{
// Construction
public:
	void OnAbout_StarWars_Control_Set();
	void OnAbout_Button_Set();
	CDialog_About(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_About)
	enum { IDD = IDD_DIALOG_ABOUT };
	CStarWarsCtrl	m_picture_about;
	//}}AFX_DATA

// Attributes
private:
	CButton *mp_about_ok;  // 버튼에 대한 포인터 변수 선언
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_About)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_About)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_ABOUT_H__4DBEC1DF_A490_48B1_9D53_2002B4C363C8__INCLUDED_)
