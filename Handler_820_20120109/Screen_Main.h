#if !defined(AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_)
#define AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Main.h : header file
//
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "Public_Function.h"

#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 

#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰� 

#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"

#include "LedButton.h"      // LED ��ư Ŭ����

#include "EXDigitST.h"		// ������ ī���� ���� Ŭ���� �߰� 
#include "MacButtons.h"
#include "Digit.h"

#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 

#include "PictureEx.h"		// GIF ������ �ε��ϱ� ���� Ŭ���� �߰� 
#include "BmpImageST.h"		// �˶� ��Ʈ�� �ε� Ŭ���� �߰� 
#include "CheckerCtrl.h"
#include "CharSetCtrl.h"
#include "FloatST.h"
#include "GridControlAlg.h"
#include "MatrixStatic.h"
#include "BitmapSlider.h"
/* ****************************************************************************** */
/* ������ ���� ���� Ŭ���� �߰�                                                   */
/* ****************************************************************************** */
#include "ssdllmfc.h"
/* ****************************************************************************** */



class AModule;
/////////////////////////////////////////////////////////////////////////////
// CScreen_Main form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CClientSocket;
class CScreen_Main : public CFormView
{
public:
	CFont* mp_main_font1;				// ��Ʈ ���� ���� ����

public:
	CScreen_Main();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Main)

// Form Data
public:
	//{{AFX_DATA(CScreen_Main)
	enum { IDD = IDD_SCREEN_MAIN };
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_main_font;			// ��Ʈ ���� ���� ����
	CFont* mp_main_big_font;			// ��Ʈ ���� ���� ����

	/* ************************************************************************** */
	/*  Tray & Buffer Ŭ���� ���� ����                                            */
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ������ ī���� Ŭ���� ���� ����                                             */
	/* ************************************************************************** */
	int mn_alarm_cnt;				// �˶� �߻� Ƚ�� ���� ����
	int mn_alarm_day_cnt;			// Day �˶� �߻� Ƚ�� ���� ����

	CString mstr_lot_total_cnt;

	int mn_discharge_step;
	int mn_time_select;
// Operations
public:
	void OnMain_Display_Lot_Info();
	void OnProgress_Display();
	void OnMain_Button_Set();
	void OnMain_Loader_Display();
	void OnMain_Count();
	void OnMainWork_Loader_Set();
	void OnMain_Workinfo_Display();
	void OnMain_Timeinfo_Display();
	void OnMain_Time_Change(int n_state);
	void OnMain_Time_Display(int iFlag = FALSE);
	void OnMain_Label_Set();
	void OnMain_Status_Display();
	void OnMain_Loader_Set();
	void OnMain_Picture_Set();
	void OnMain_GroupBox_Set();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Main)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Main();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Main)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLotOpen();
	afx_msg void OnBtnDoorOpen();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam);  // �׽�Ʈ ��� ���� ȭ�鿡 ����ϱ� ���� ����� ���� �޽��� �߰� 
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_)
