#if !defined(AFX_DIALOG_SELECT_H__5434031F_5D9B_4F24_B635_7054AA5A324D__INCLUDED_)
#define AFX_DIALOG_SELECT_H__5434031F_5D9B_4F24_B635_7054AA5A324D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Select.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "PictureEx.h"		// GIF 파일 로딩 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
/////////////////////////////////////////////////////////////////////////////
// CDialog_Select dialog

class CDialog_Select : public StandardDialog
{
// Construction
public:
	void OnSelect_Button_Set();
	void OnSelect_Lable_Set();
	void OnSelect_Picture_Set();
	CDialog_Select(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Select)
	enum { IDD = IDD_DIALOG_SELECT };
	CGradientStatic	m_select_message;
	CPictureEx	m_gif_select;
	//}}AFX_DATA

// Attributes
private:
	CButton *mp_select_yes, *mp_select_no;	// 버튼에 대한 포인터 선언
	CFont* mp_select_font;					// 폰트 설정 변수 선언 

	int mn_dialog;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Select)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Select)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SELECT_H__5434031F_5D9B_4F24_B635_7054AA5A324D__INCLUDED_)
