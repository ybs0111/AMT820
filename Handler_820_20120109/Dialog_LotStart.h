#if !defined(AFX_DIALOG_LOTSTART_H__719FE94A_2C5E_4D0B_B5AA_838D56562AAC__INCLUDED_)
#define AFX_DIALOG_LOTSTART_H__719FE94A_2C5E_4D0B_B5AA_838D56562AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_LotStart.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDialog_LotStart dialog

class CDialog_LotStart : public StandardDialog
{
// Construction
public:
	CDialog_LotStart(CWnd* pParent = NULL);   // standard constructor
	int m_nSelectedLotIdx;

// Dialog Data
	//{{AFX_DATA(CDialog_LotStart)
	enum { IDD = IDD_DIALOG_LOT_START };
	CButton	m_btn_lotcancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_LotStart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strCombo;

	// Generated message map functions
	//{{AFX_MSG(CDialog_LotStart)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnLotDisplay();
	afx_msg void OnLotStart();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDraw_Data_LotStart(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void OnInitGridLotInfo();
	void OnDisplay_Lot();

	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridData(UINT nID, int row, int col, CString data);
	void GridFont(UINT nID, int row, int col, int size);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_LOTSTART_H__719FE94A_2C5E_4D0B_B5AA_838D56562AAC__INCLUDED_)
