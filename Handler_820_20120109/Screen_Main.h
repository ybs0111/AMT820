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
#include "GridCtrl.h"
/* ****************************************************************************** */
/* ������ ���� ���� Ŭ���� �߰�                                                   */
/* ****************************************************************************** */
#include "ssdllmfc.h"
/* ****************************************************************************** */



#define WM_CDIMM_UI			 787
#define WM_EPOXY_MOVE_CLEAN  788
#define WM_EPOXY_MOVE_SAFETY 789
#define WM_MAIN_UI			 790
#define WM_HARDNEDD_OUT		 791
#define WM_EPOXY_OUT		 792

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
	CXPGroupBox	m_group_Top_Info;
	CXPGroupBox	m_group_HeatSink;
	CXPGroupBox	m_group_Device;
	CXPGroupBox	m_group_Btm_Info;
	CMacButton	m_btn_hardness_reset;
	CMacButton	m_btn_clean_reset;
	CMacButton	m_btn_hardness_Out;
	CMacButton	m_btn_Tray_Editor;
	CMacButton	m_btn_Epoxy_Safety;
	CMacButton	m_btn_Epoxy_Out;
	CMacButton	m_btn_Epoxy_Out2;
 	CMacButton	m_tray2_unlock;
 	CMacButton	m_tray1_unlock;
 	CMacButton	m_tray1_lock;
 	CMacButton	m_tray2_lock;
	CGradientStatic	m_msg_alarm;
	CGradientStatic	m_msg_mtbi;
	CGradientStatic	m_msg_stop;
	CGradientStatic	m_msg_run;
	CDigit	m_dgt_alarm;
	CDigit	m_dgt_stop;
	CDigit	m_dgt_run;
	CDigit	m_dgt_mtbi;
	CButtonST	m_chkOnlyCarrier;
	CNewLabel	m_ld_buffer;
	CNewLabel	m_disp_place;
	CNewLabel	m_disp_pick;
	CNewLabel	m_uld_buffer;
	CButtonST	m_changeTop_Btm;
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

	//kwlee 2017.0413
	TSpread			*m_grid_Top;
	TSpread			*m_grid_Btm;
	TSpread			*m_grid_Ref;

	GridControlAlg   m_pGrid;
	//kwlee 2017.0424
	CBitmap m_BitmapLock1Check;
	CBitmap m_BitmapUnLock1Check;
	CBitmap m_BitmapLock2Check;
	CBitmap m_BitmapUnLock2Check;

	//kwlee 2017.0425
	TSpread			*m_grid_HeatSink;
	TSpread			*m_grid_Device;
	
	//kwlee 2017.0428
	CGridCtrl        m_grid_Info_Top; 
	CGridCtrl        m_grid_Info_Btm; 
	
	CGridCtrl        m_grid_Info_HeatSink; 
	CGridCtrl        m_grid_Info_Device;

	int m_nRow;
	int m_nCol;
	CString m_strSkipData;
// Operations
public:
	void OnMain_Init_Lot_Info();
	void OnMain_Init_Daily_Yield_info();
	void OnMain_Init_Lot_Yield_info();
	void OnMain_Init_B_Lot_Info();
	void OnMain_Init_Door();
	void OnMain_Display_Door();
	void OnMain_Digital_Count_Set();

	void OnMain_Display_Lot_Info();
	void OnMain_Display_Lot_Yield_info();
	void OnMain_Display_Daily_Yield_info();
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

	//kwlee 2017.0412
	void OnInitTopGrid();
	void OnInitBTMGrid();
	void OnInitGridRef();
	
	//kwlee 2017.0425
	void OnInitHeatSinkGrid();
	void OnInitDeviceGrid();

	void OnMainTop_Loader_Recive_Info();
	void OnMainTop_Input_Loader_Info();
	void OnMainTop_Epoxy_Info();
	void OnMainTop_Wait_Index_Info();
	void OnMainTop_Hs_Vision_Info();
	void OnMainTop_OutSend_Info();
	void OnMainTop_Unloader_Info();
	
	//void OnMainBtmGrid_Info();
	void OnMainBtm_Down_Info();
	void OnMainBtm_DownForward_Info();
	void OnMainBtm_Hs_Down_Info();
	void OnMainBtm_Index_Down_Info();
	void OnMainBtm_Epoxy_Down_Info();
	void OnMainBtm_Input_Down_Info();
	void OnMainBtm_Loader_Down_Info();
	

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
	afx_msg void OnTray2Lock();
	afx_msg void OnTray2Unlock();
	afx_msg void OnTray1Lock();
	afx_msg void OnTray1Unlock();
	afx_msg void OnCheck1();
	afx_msg void OnLabelUnload();
	afx_msg void OnCheckOnlycarrier();
	afx_msg void OnEpoxyCleanOutPosMove();
	afx_msg void OnButtonTrayEditor();
	afx_msg void OnEpoxySafety();
	afx_msg void OnHardnessOut();
	afx_msg void OnEpoxyOut2();
	afx_msg void OnButtonCleanReset();
	afx_msg void OnButtonHardnessReset();
	//}}AFX_MSG
	afx_msg LRESULT OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam);  // �׽�Ʈ ��� ���� ȭ�鿡 ����ϱ� ���� ����� ���� �޽��� �߰� 
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam); //kwlee 2017.0425
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_)
