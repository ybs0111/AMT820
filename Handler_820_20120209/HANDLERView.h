// HANDLERView.h : interface of the CHANDLERView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLERVIEW_H__692A6CC7_3DA8_4751_9F16_2183C36E2E78__INCLUDED_)
#define AFX_HANDLERVIEW_H__692A6CC7_3DA8_4751_9F16_2183C36E2E78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHANDLERView : public CFormView
{
protected: // create from serialization only
	CHANDLERView();
	DECLARE_DYNCREATE(CHANDLERView)

public:
	//{{AFX_DATA(CHANDLERView)
	enum{ IDD = IDD_HANDLER_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CHANDLERDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHANDLERView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHANDLERView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHANDLERView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HANDLERView.cpp
inline CHANDLERDoc* CHANDLERView::GetDocument()
   { return (CHANDLERDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLERVIEW_H__692A6CC7_3DA8_4751_9F16_2183C36E2E78__INCLUDED_)
