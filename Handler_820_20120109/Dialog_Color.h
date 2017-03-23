#if !defined(AFX_DIALOG_COLOR_H__41B23817_6E6A_4F73_8312_AE4C0389EBCC__INCLUDED_)
#define AFX_DIALOG_COLOR_H__41B23817_6E6A_4F73_8312_AE4C0389EBCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Color.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Color dialog
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "Public_Function.h"
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰� 
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // Į�� ������ ����Ʈ �ڽ� Ŭ����
#include "xpbutton.h"		// XP Į�� ��ư Ŭ����
#include "EXDigitST.h"		// ������ ī���� Ŭ����
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "FloatST.h"

class CDialog_Color : public CDialog
{
// Construction
public:
	void Init_Group();
	CDialog_Color(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Color)
	enum { IDD = IDD_DIALOG_COLOR };
	CMacButton	m_btn_o_text1;
	CMacButton	m_btn_o_grid1;
	CMacButton	m_btn_i_text1;
	CMacButton	m_btn_i_grid1;
	CMacButton	m_btn_ok;
	CMacButton	m_btn_o_text;
	CMacButton	m_btn_o_grid;
	CMacButton	m_btn_i_text;
	CMacButton	m_btn_i_grid;
	CXPGroupBox	m_group_output;
	CXPGroupBox	m_group_input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Color)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Color)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnIGrid();
	afx_msg void OnBtnIText();
	afx_msg void OnBtnOGrid();
	afx_msg void OnBtnOText();
	afx_msg void OnBtnIGrid2();
	afx_msg void OnBtnIText2();
	afx_msg void OnBtnOGrid2();
	afx_msg void OnBtnOText2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_COLOR_H__41B23817_6E6A_4F73_8312_AE4C0389EBCC__INCLUDED_)
