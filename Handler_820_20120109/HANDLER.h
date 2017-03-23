// HANDLER.h : main header file for the HANDLER application
//

#if !defined(AFX_HANDLER_H__36513D9D_891B_4C98_A529_05D59494C4A3__INCLUDED_)
#define AFX_HANDLER_H__36513D9D_891B_4C98_A529_05D59494C4A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"			// main symbols
#include "SingleInstanceApp.h"	// Single Instance 생성 클래스 추가 

/////////////////////////////////////////////////////////////////////////////
// CHANDLERApp:
// See HANDLER.cpp for the implementation of this class
//
class CHANDLERApp : public CSingleInstanceApp
{
public:

public:
	CHANDLERApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHANDLERApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHANDLERApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLER_H__36513D9D_891B_4C98_A529_05D59494C4A3__INCLUDED_)
