#if !defined(AFX_DIALOG_LOT_END_H__3C39DB88_C38C_4DBD_9D09_7E678E961D8F__INCLUDED_)
#define AFX_DIALOG_LOT_END_H__3C39DB88_C38C_4DBD_9D09_7E678E961D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Lot_End.h : header file
//
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "Public_Function.h"
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 

/////////////////////////////////////////////////////////////////////////////
// CDialog_Lot_End dialog

class CDialog_Lot_End : public StandardDialog
{
// Construction
public:
	CDialog_Lot_End(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Lot_End)
	enum { IDD = IDD_DIALOG_LOT_END };
	//}}AFX_DATA
	
// Attributes
public:
	CButton *mp_lot_end_ok;			// ��ư�� ���� ������ ����
	CFont* mp_lot_end_font;			// ��Ʈ ���� ���� ����

	int m_nStackerNo;

// Operations
public:
	void OnLotEnd_Init_Lot_Info();
	void OnLotEnd_Button_Set();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Lot_End)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Lot_End)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnLotend();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_LOT_END_H__3C39DB88_C38C_4DBD_9D09_7E678E961D8F__INCLUDED_)
