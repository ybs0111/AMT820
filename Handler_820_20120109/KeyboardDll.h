// KeyboardDll.h : main header file for the KEYBOARDDLL DLL
//

#if !defined(AFX_KEYBOARDDLL_H__58AFF752_16D6_44BD_9C70_CB8CFBD06881__INCLUDED_)
#define AFX_KEYBOARDDLL_H__58AFF752_16D6_44BD_9C70_CB8CFBD06881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDllApp
// See KeyboardDll.cpp for the implementation of this class
//
#include "KeyView.h"

class CKeyboardDllApp : public CWinApp
{
public:
	CKeyView	*m_pKeyView;
	CKeyboardDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboardDllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CKeyboardDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



 /////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLL_H__58AFF752_16D6_44BD_9C70_CB8CFBD06881__INCLUDED_)
//extern "C" __declspec(dllexport) void SetKeyboardDialog(CString *str);
extern "C" __declspec(dllexport)LPCTSTR GetEditString(CString strPrevString);
extern "C" __declspec(dllexport)LPCTSTR GetNumEditString_D(double dMin, double dMax, CString strPrevString);
extern "C" __declspec(dllexport)LPCTSTR GetNumEditString_I(int nMin, int nMax, CString strPrevString);
extern "C" __declspec(dllexport)LPCTSTR GetAlphaEditString(CString strPrevString);
