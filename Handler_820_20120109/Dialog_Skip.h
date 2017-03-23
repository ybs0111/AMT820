#if !defined(AFX_DIALOG_SKIP_H__335A27EF_D3AB_4150_A7F7_F25A6C76AB66__INCLUDED_)
#define AFX_DIALOG_SKIP_H__335A27EF_D3AB_4150_A7F7_F25A6C76AB66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Skip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Skip dialog

class CDialog_Skip : public CDialog
{
// Construction
public:
	CDialog_Skip(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Skip)
	enum { IDD = IDD_DIALOG_SKIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Skip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Skip)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SKIP_H__335A27EF_D3AB_4150_A7F7_F25A6C76AB66__INCLUDED_)
