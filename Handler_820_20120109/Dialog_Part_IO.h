#if !defined(AFX_DIALOG_PART_IO_H__0D376FD6_2F3D_4E27_8347_BDB5B857F43E__INCLUDED_)
#define AFX_DIALOG_PART_IO_H__0D376FD6_2F3D_4E27_8347_BDB5B857F43E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Part_IO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Part_IO dialog
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"

class CDialog_Part_IO : public CDialog
{
public:
	CFont			*m_p_font;
	CPoint			m_cp_coord;
	GridControlAlg m_p_grid;

	TSpread			*m_grid_input;
	TSpread			*m_grid_output;

	int				m_n_part;
	int				m_n_in[32];
	int				m_n_in_port[32];
	int				m_n_in_num[32];
	int				m_n_out[32];
	int				m_n_out_port[32];
	int				m_n_out_num[32];
	int				m_n_in_cnt;
	int				m_n_out_cnt;

	CString			m_str_in[32];
	CString			m_str_out[32];
// Construction
public:
	void OnOutputCheck();
	void OnInputCheck();
	void Init_Output();
	void Init_Input();
	void Init_Grid_IO();
	void Init_Group();
	void Init_Button();
	BOOL Create();
	CDialog_Part_IO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Part_IO)
	enum { IDD = IDD_DIALOG_PART_IO };
	CButtonST	m_btn_exit;
	CXPGroupBox	m_group_output;
	CXPGroupBox	m_group_input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Part_IO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Part_IO)
	afx_msg void OnBtnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_PART_IO_H__0D376FD6_2F3D_4E27_8347_BDB5B857F43E__INCLUDED_)
