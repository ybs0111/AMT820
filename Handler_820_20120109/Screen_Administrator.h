#if !defined(AFX_SCREEN_ADMINISTRATOR_H__4AE0269E_4984_454C_B262_1C323D187CE3__INCLUDED_)
#define AFX_SCREEN_ADMINISTRATOR_H__4AE0269E_4984_454C_B262_1C323D187CE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Administrator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_Administrator form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Administrator : public CFormView
{
public:
	CScreen_Administrator();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Administrator)

// Form Data
public:
	//{{AFX_DATA(CScreen_Administrator)
	enum { IDD = IDD_SCREEN_ADMINISTRATOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Administrator)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Administrator();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Administrator)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_ADMINISTRATOR_H__4AE0269E_4984_454C_B262_1C323D187CE3__INCLUDED_)
