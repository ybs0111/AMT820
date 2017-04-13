#if !defined(AFX_DIALOG_TITLE_H__2706FE10_A9F4_4353_88E2_99684191F17E__INCLUDED_)
#define AFX_DIALOG_TITLE_H__2706FE10_A9F4_4353_88E2_99684191F17E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Title.h : header file
//
#include "InitDialogBar.h" // Initial Dialog Bar ���� Ŭ���� �߰� 
/////////////////////////////////////////////////////////////////////////////
// CDialog_Title dialog

#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "MenuHead.h"		// ȭ�鿡 ���� ��� ���ǵ� Ŭ���� �߰�
#include "VLabel.h"			// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "PictureEx.h"		// GIF ������ �ε��ϱ� ���� Ŭ���� �߰� 

class CDialog_Title : public CInitDialogBar
{
// Construction
public:
	void OnTitle_Interface_Mode_Display(int ninterface);
	void OnTitle_Press_Mode_Display(int npress);
	void OnTitle_Device_Mode_Display(int n_device);
	void OnTitle_Status_Change(int n_status);
	void OnTitle_Picture_Set();
	void OnTitle_Form_Name_Change(int nViewID);
	void OnTitle_Lable_Set();
	//kwlee 2017.0406
	void OnTitle_7387_Mode_Display(int n_device);
	void OnTitle_3874_Mode_Display(int n_device);
	void OnTitle_Barcode_Mode_Display(int n_device);
	CDialog_Title();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Title)
	enum { IDD = IDD_DIALOG_TITLE };
	CGradientStatic	m_msg_Barcode_mode_info;
	CGradientStatic	m_msg_7387_mode_info;
	CGradientStatic	m_msg_3874_mode_info;
	CGradientStatic	m_msg_model_info;
	CGradientStatic	m_msg_file_info;
	CGradientStatic	m_msg_press_mode_info;
	CGradientStatic	m_msg_interface_mode_info;
	CGradientStatic	m_msg_device_mode_info;
	CGradientStatic	m_msg_status_info;
	CPictureEx	m_gif_title_logo;
	CVLabel	m_msg_form_name;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_title_font;  // ��Ʈ ���� ���� ���� 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Title)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Title)
	virtual BOOL OnInitDialogBar();
	//}}AFX_MSG
	afx_msg  LRESULT OnStatusDataUpdate(WPARAM wParam,LPARAM lParam) ;  // Ÿ��Ʋ �� ȭ������ ���۵� �޽��� ��½�Ű�� ���� ����� ���� �޽��� �߰� 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_TITLE_H__2706FE10_A9F4_4353_88E2_99684191F17E__INCLUDED_)
