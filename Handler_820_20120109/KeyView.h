#if !defined(AFX_KEYVIEW_H__53490AD7_74F1_42D9_80CC_8CAC933C28E4__INCLUDED_)
#define AFX_KEYVIEW_H__53490AD7_74F1_42D9_80CC_8CAC933C28E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyView view

#include "AfxTempl.h"

int FAR PASCAL HookInit(HWND hMain);
int FAR PASCAL HookOn();
int FAR PASCAL HookOff();
#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4a
#define VK_K	0x4b
#define VK_L	0x4c
#define VK_M	0x4d
#define VK_N	0x4e
#define VK_O	0x4f
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5a
struct KEYDEF
{
	char cNormal;
	char cShifted;
	int cWidth;
};

class CKeyView : public CScrollView
{
protected:
//	CKeyView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CKeyView)

// Attributes
public:
	KEYDEF *key;
	BOOL m_bHangul;
	int pKeyWidth;
	int pKeyHeight;
	int cCurrentKeyState;
	HWND gFocus;
	CTypedPtrList<CPtrList, KEYDEF*> cKeys;
	CKeyView();
	virtual ~CKeyView();

// Operations
public:
	void Close();

	void CalcWidthHeight();
	void SendKey(KEYDEF * keydef);
	void AddKey(char pNormal,char pShifted,int pWidth);
	int DescribeKeyState();
	void DrawKey(CDC *dc, CRect &rc, KEYDEF *key, BOOL cHilight);
	void DrawKeyboard();
	void InitKey();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual ~CKeyView();
	afx_msg long OnHookMsg(WPARAM wParam, LPARAM lParam);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CKeyView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYVIEW_H__53490AD7_74F1_42D9_80CC_8CAC933C28E4__INCLUDED_)
