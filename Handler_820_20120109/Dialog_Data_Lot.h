#if !defined(AFX_DIALOG_DATA_LOT_H__5B5C9A2A_76E5_4E5B_9667_18F04F118689__INCLUDED_)
#define AFX_DIALOG_DATA_LOT_H__5B5C9A2A_76E5_4E5B_9667_18F04F118689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Data_Lot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Lot dialog

class CDialog_Data_Lot : public StandardDialog
{
// Construction
public:
	CDialog_Data_Lot(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Data_Lot)
	enum { IDD = IDD_DIALOG_DATA_LOT };
	CButton	m_btn_update;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Data_Lot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Grid
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridData(UINT nID, int row, int col, CString data);
	void GridFont(UINT nID, int row, int col, int size);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	// Generated message map functions

	void OnInit_Lot();
	void OnInit_LotInfo();

	void OnDisplay_Lot();
	void OnDisplay_LotInfo();
	
protected:
	int m_nSelectedLotIdx;

	//{{AFX_MSG(CDialog_Data_Lot)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCreate();
	afx_msg void OnDestroy();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnUpdate();
	afx_msg void OnBtnNewLotIn();
	afx_msg void OnBtnNetLot();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDraw_Data_Lot(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_DATA_LOT_H__5B5C9A2A_76E5_4E5B_9667_18F04F118689__INCLUDED_)
