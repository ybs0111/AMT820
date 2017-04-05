#if !defined(AFX_DIALOG_MANUAL_MOVE_H__CACD5E57_1C88_4547_A3F9_9455A38B8E2B__INCLUDED_)
#define AFX_DIALOG_MANUAL_MOVE_H__CACD5E57_1C88_4547_A3F9_9455A38B8E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_Move.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move dialog
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰�  
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EditEx.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EXDigitST.h"		// ������ ī���� Ŭ����
#include "LedButton.h"      // LED ��ư Ŭ����
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "xShadeButton.h"	// ���� ��ư ���� Ŭ���� �߰�
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XpButton.h"		// XP Į�� ��ư Ŭ����
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech ���� ���� Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"

#define TM_CARRIRER_LOOP	3288

class CDialog_Manual_Move : public CDialog
{
public:
	int				m_n_part;

	CPoint			m_cp_coord;
	CFont			*m_p_font;	

// Construction
public:
	void Init_Group();
	void Init_Button();
	BOOL Create();
	CDialog_Manual_Move(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Manual_Move)
	enum { IDD = IDD_DIALOG_MANUAL_MOVE };
	CButtonST	m_btn_exit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_Move)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnButtonControl(bool bflag);
	int Move_Carrier_Loop();

	int m_move_step[25];
	DWORD m_dwBcrWaitTime[3];
	DWORD m_dwWaitTime[25][3];

protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Manual_Move)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnCarrierLoop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnMotorStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MANUAL_MOVE_H__CACD5E57_1C88_4547_A3F9_9455A38B8E2B__INCLUDED_)
