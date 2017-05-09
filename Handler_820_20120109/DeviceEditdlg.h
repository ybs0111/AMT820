//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_DEVICEEDITDLG_H__068416EF_A940_42CE_BD1B_AEB4871C9FC9__INCLUDED_)
#define AFX_DEVICEEDITDLG_H__068416EF_A940_42CE_BD1B_AEB4871C9FC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceEditdlg.h : header file
//

#include "mshflexgrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDeviceEditdlg dialog

class CDeviceEditdlg : public StandardDialog
{
// Construction
public:
	CDeviceEditdlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeviceEditdlg)
	enum { IDD = IDD_DIALOG_TRAY };
	CButtonST	m_btn_dvc_yes;
	CButtonST	m_btn_dvc_none;
	CButtonST	m_btn_dvc_all_Yes;
	CButtonST	m_btn_dvc_all_None;
	CGradientStatic	m_msg_dvc_no;
	CGradientStatic	m_msg_dvc_yes;
	CGradientStatic	m_msg_all_yes;
	CGradientStatic	m_msg_all_none;
	CComboBox	m_cboSelectStage;
	CMSHFlexGrid	m_ctrlTrayModifyDevice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceEditdlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCmdColor(int selchip);
	void LoadTray(int sel);
	void OnTray_Label_Set();
	
	void OnInitButtonSet(); //kwlee 2017.0425
	

public:
	CFont* mp_tray_font;
	int nMouseDown;
	int nSelStage;
	int nPoint[2];
	int nSelChip;
	int m_nHeight;
	int m_nWidth;
	int m_nRow;
	int m_nCol;
	int m_nERow;
	int m_nECol;
	int m_bTrayArray[2][MAX_TRAY_ROW][MAX_TRAY_COL];
	int m_tempTrayArray[2][MAX_TRAY_ROW][MAX_TRAY_COL];

protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceEditdlg)
	virtual void OnOK();
	afx_msg void OnSelchangeCboSelectStage();
	afx_msg void OnClickTraymodifydevice();
	afx_msg void OnMouseDownTraymodifydevice(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpTraymodifydevice(short Button, short Shift, long x, long y);
	afx_msg void OnMouseMoveTraymodifydevice(short Button, short Shift, long x, long y);
	virtual BOOL OnInitDialog();
	afx_msg void OnDvcNone();
	afx_msg void OnDvcAllYes();
	afx_msg void OnDvcYes();
	afx_msg void OnDvcAllNone();
	afx_msg void OnBtnDvcNone();
	afx_msg void OnBtnDvcYes();
	afx_msg void OnBtnDvcAllNone();
	afx_msg void OnBtnDvcAllYes();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void InitShowErrorTray(int Row, int Col);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEEDITDLG_H__068416EF_A940_42CE_BD1B_AEB4871C9FC9__INCLUDED_)
