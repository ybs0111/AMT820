#if !defined(AFX_DIALOG_MENU_H__C5179E92_3334_4DA4_BFC1_8382069FB945__INCLUDED_)
#define AFX_DIALOG_MENU_H__C5179E92_3334_4DA4_BFC1_8382069FB945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Menu.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가 
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "ListBoxST.h"			// 칼라 아이콘 리스트 박스 생성 클래스 추가 
#include "PictureEx.h"			// GIF 파일을 Loading하기 위한 Class 추가
/////////////////////////////////////////////////////////////////////////////
// CDialog_Menu dialog

class CDialog_Menu : public StandardDialog
{
// Construction
public:
	void OnMenu_ID_Sending(CString str_selected_menu);
	void OnCreate_Menu_Image_List();
	void OnCreate_Menu_ListBox();
	void OnMenu_Picture_Set();
	void OnMenu_Lable_Set();
	void OnMenu_Button_Set();
	CDialog_Menu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Menu)
	enum { IDD = IDD_DIALOG_MENU };
	CPictureEx	m_gif_menu;
	CGradientStatic	m_msg_menu;
	CListBoxST	m_list_menu;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_menu_font;			// 폰트 설정 변수 선언 

	CButton *mp_menu_ok;			// 종류 버튼에 대한 포인터 변수 

	CImageList mp_menu_image_list;  // 이미지 리스트 변수 

	CString mp_menu_name;			// 선택된 메뉴 정보 저장 변수 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Menu)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Menu)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListMenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MENU_H__C5179E92_3334_4DA4_BFC1_8382069FB945__INCLUDED_)
