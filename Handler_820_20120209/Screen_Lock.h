#if !defined(AFX_SCREEN_LOCK_H__B4D205A2_BA78_440F_A83B_B5DC0564FA22__INCLUDED_)
#define AFX_SCREEN_LOCK_H__B4D205A2_BA78_440F_A83B_B5DC0564FA22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Lock.h : header file
//

#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스 */
#include "PictureEx.h"		// GIF 파일 로딩 클래스 추가 

/////////////////////////////////////////////////////////////////////////////
// CScreen_Lock form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Lock : public CFormView
{
public:
	CScreen_Lock();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Lock)

// Form Data
public:
	//{{AFX_DATA(CScreen_Lock)
	enum { IDD = IDD_SCREEN_LOCK };
	CGradientStatic	m_msg_lock;
	CPictureEx	m_gif_lock;
	CXPButton	m_btn_unlock;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_lock_font;  // 폰트 설정 변수 선언 

// Operations
public:
	void OnLocking_Picture_Set();
	void OnLocking_Lable_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Lock)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Lock();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Lock)
	afx_msg void OnBtnUnlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LOCK_H__B4D205A2_BA78_440F_A83B_B5DC0564FA22__INCLUDED_)
