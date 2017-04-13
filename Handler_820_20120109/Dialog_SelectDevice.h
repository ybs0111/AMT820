#if !defined(AFX_DIALOG_SELECTDEVICE_H__7C15983D_BC38_4425_9BDA_C74A83ABE9B8__INCLUDED_)
#define AFX_DIALOG_SELECTDEVICE_H__7C15983D_BC38_4425_9BDA_C74A83ABE9B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_SelectDevice.h : header file
//

#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "PictureEx.h"		// GIF 파일 로딩 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "BtnST.h"
#include "XPGroupBox.h"
/////////////////////////////////////////////////////////////////////////////
// CDialog_SelectDevice dialog

class CDialog_SelectDevice : public StandardDialog
{
// Construction
public:
	CDialog_SelectDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_SelectDevice)
	enum { IDD = IDD_DIALOG_SELECT_DEVICE };
	CPictureEx	m_gif_select;
	CGradientStatic	m_select_message;
	CButtonST	m_chk_action9;
	CButtonST	m_chk_action8;
	CButtonST	m_chk_action7;
	CButtonST	m_chk_action6;
	CButtonST	m_chk_action5;
	CButtonST	m_chk_action4;
	CButtonST	m_chk_action3;
	CButtonST	m_chk_action2;
	CButtonST	m_chk_action10;
	CButtonST	m_chk_action1;
	CXPGroupBox	m_group_picker10;
	CXPGroupBox	m_group_picker9;
	CXPGroupBox	m_group_picker8;
	CXPGroupBox	m_group_picker7;
	CXPGroupBox	m_group_picker6;
	CXPGroupBox	m_group_picker5;
	CXPGroupBox	m_group_picker4;
	CXPGroupBox	m_group_picker3;
	CXPGroupBox	m_group_picker2;
	CXPGroupBox	m_group_picker1;
	//}}AFX_DATA
// Attributes

private:
	CButton *mp_select;
	CFont* mp_select_font;					// 폰트 설정 변수 선언 

	int mn_dialog;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_SelectDevice)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int mn_action[SORTER_PICKER_NUM];
	void OnSelect_Button_Set();
	void OnSelect_Lable_Set();
	void OnSelect_Picture_Set();
	void On_Macro_Action_Change();
	void OnSelect_Action_Change_Set();

protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_SelectDevice)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkAction1();
	afx_msg void OnChkAction2();
	afx_msg void OnChkAction3();
	afx_msg void OnChkAction4();
	afx_msg void OnChkAction5();
	afx_msg void OnChkAction6();
	afx_msg void OnChkAction7();
	afx_msg void OnChkAction8();
	afx_msg void OnChkAction9();
	afx_msg void OnChkAction10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SELECTDEVICE_H__7C15983D_BC38_4425_9BDA_C74A83ABE9B8__INCLUDED_)
