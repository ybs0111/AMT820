// HANDLERDoc.h : interface of the CHANDLERDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLERDOC_H__6DC4B7AD_99E2_4BC6_B483_0572DE1C49B1__INCLUDED_)
#define AFX_HANDLERDOC_H__6DC4B7AD_99E2_4BC6_B483_0572DE1C49B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHANDLERDoc : public CDocument
{
protected: // create from serialization only
	CHANDLERDoc();
	DECLARE_DYNCREATE(CHANDLERDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHANDLERDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHANDLERDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHANDLERDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLERDOC_H__6DC4B7AD_99E2_4BC6_B483_0572DE1C49B1__INCLUDED_)
