#if !defined(AFX_SCREEN_IO_MAP_H__7BE1479D_FEC7_4B16_B6E8_1A5044976E85__INCLUDED_)
#define AFX_SCREEN_IO_MAP_H__7BE1479D_FEC7_4B16_B6E8_1A5044976E85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_IO_Map.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO_Map form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Variable.h"			// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"	// 모터 보드 및 I/O 보드 관련 클래스
#include "NewLabel.h"			// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"				// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "LedButton.h"			// LED 버튼 클래스
#include "XpButton.h"			// XP 칼라 버튼 클래스 */
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "TreeOptionsCtrl.h"
#include "FloatST.h"
#include "EXDigitST.h"			// 디지털 카운터 생성 클래스 추가 
#include "BtnST.h"				// 칼라 버튼 생성 클래스 추가 
#include "Digit.h"

#include "IOBoardLibrary.h"		// HSSI IO Board 2K5/06/13/ViboX

#define TMR_IO_READ		940
#define TMR_IO_CONTINUE	941

class CScreen_IO_Map : public CFormView
{
private:
	int			n_conti_wait_time[3];
public:
	CDigit		m_dgt_retry_wait;
	CDigit		m_dgt_retry_cnt;

	CFont*		mp_io_font;    // 폰트 정보 설정 변수

	CString		mstr_io_name;		// Logging할 IO의 이름...
	CString		mstr_io_act;		// Logging할 IO의 동작상태...

	long		WaitTime[3];
	int			mn_action_cnt;
	int			mn_actioncnt;
	int			mn_conti_wait;
	int			mn_conti_io_no;
	int			mn_conti_io_flag;
	int			mn_action_step;
	int			mn_stop_req;
	int			mn_continue;
	int			n_check_slave;
	int			m_nmaster;
	int			m_nport;
	int			m_nslave;
	int			m_nList_Count;

	CString		m_strIo_Model;
	CString		m_strIo_Description[10][8][16];
	CString		m_strIo_Label[10][8][16];
	CString		m_strIo_Module[10][8];

	int			m_nIo_Type[10][8][16];
	int			m_nIo_Num[10][8][16];
	int			m_nIo_Module[10][8];

	int			m_n_output_info[16];
	int			m_n_output_onoff[16];
	
	TSpread		*m_pIoMap;
	TSpread		*m_pIoList;

	GridControlAlg m_p_grid;

	CScreen_IO_Map();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_IO_Map)

// Form Data
public:
	//{{AFX_DATA(CScreen_IO_Map)
	enum { IDD = IDD_SCREEN_IO_MAP };
	CMacButton	m_btn_color;
	CMacButton	m_btn_e_stop;
	CFloatST	m_msg_retry_wait;
	CFloatST	m_msg_retry_cnt;
	CButtonST	m_chk_continue;
	CXPGroupBox	m_group_continue;
	CMacButton	m_btn_slave_enable;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	int OnIO_Safety(int n_io);
	int OnIO_Cylinder_Excution();
	void OnIO_Digital_Count_Set();
	void OnIO_Name_Label_Set();
	void OnIO_Button_Set();
	void OnIO_Controls_Enable(bool b_state);
	void OnIO_Continue_Change();
	void OnIO_GroupBox_Set();
	void OnIO_Slave_Change();
	void OnInputCheck(int port, int slave);
	void OnOutputCheck(int port, int slave);
	void OnOutputData(int port, int slave, int io);
	void Init_List();
	void OnChangeSheet(int port, int num);
	void OnDrawGrid();
	void OnSearch_Data();
	void Init_Map();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_IO_Map)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_IO_Map();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_IO_Map)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSlaveEnable();
	afx_msg void OnChkContinue();
	afx_msg void OnBtnIoEStop();
	afx_msg void OnDgtRetryCnt();
	afx_msg void OnDgtRetryWait();
	afx_msg void OnBtnColor();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_IO_MAP_H__7BE1479D_FEC7_4B16_B6E8_1A5044976E85__INCLUDED_)
