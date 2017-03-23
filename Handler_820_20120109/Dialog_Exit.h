#if !defined(AFX_DIALOG_EXIT_H__17BF0EAC_CA69_4650_8981_C624963F81C6__INCLUDED_)
#define AFX_DIALOG_EXIT_H__17BF0EAC_CA69_4650_8981_C624963F81C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Exit.h : header file
//

#include "GradientStatic.h"  // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "PictureEx.h"		// GIF 파일을 Loading하기 위한 Class 추가

/////////////////////////////////////////////////////////////////////////////
// CDialog_Exit dialog

class CDialog_Exit : public StandardDialog
{
// Construction
public:
	void OnExit_Picture_Set();
	void OnExit_Lable_Set();
	void OnExit_Button_Set();
	CDialog_Exit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Exit)
	enum { IDD = IDD_DIALOG_EXIT };
	CPictureEx	m_gif_exit;
	CGradientStatic	m_msg_exit;
	//}}AFX_DATA

// Attributes
private:
	CButton *mp_btn_ok, *mp_btn_cancel;  // 버튼에 대한 포인터 선언
	CFont* mp_exit_font;  // 폰트 설정 변수 선언 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Exit)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Exit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_EXIT_H__17BF0EAC_CA69_4650_8981_C624963F81C6__INCLUDED_)
