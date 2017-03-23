#if !defined(AFX_TEXTTOOLBAR_H__577C11B3_C22B_4CE6_BA0D_772C35240AED__INCLUDED_)
#define AFX_TEXTTOOLBAR_H__577C11B3_C22B_4CE6_BA0D_772C35240AED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextToolBar window

class CTextToolBar : public CToolBar
{
// Construction
public:
	CTextToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadToolBar(UINT nIDResource);
	virtual ~CTextToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextToolBar)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTTOOLBAR_H__577C11B3_C22B_4CE6_BA0D_772C35240AED__INCLUDED_)
