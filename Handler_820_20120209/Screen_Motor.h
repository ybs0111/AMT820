#if !defined(AFX_SCREEN_MOTOR_H__52DCE9AA_3AEE_4C15_B5CA_E5372B940AE0__INCLUDED_)
#define AFX_SCREEN_MOTOR_H__52DCE9AA_3AEE_4C15_B5CA_E5372B940AE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Motor.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "LedButton.h"      // LED 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 생성 클래스 추가 
#include "MacButtons.h"
#include "Digit.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "PictureEx.h"		// GIF 파일을 로딩하기 위한 클래스 추가 
#include "BmpImageST.h"		// 알람 비트맵 로딩 클래스 추가 
//#include "CharSetCtrl.h"
#include "FloatST.h"
//#include "FastechPublic.h"
#include "GridControlAlg.h"
#include "MatrixStatic.h"
#include "Picture.h"
/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor form view
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Motor : public CFormView
{
public:
	int				m_n_part_cnt;
	int				m_n_part_axis_cnt[20];
	int				m_n_part_axis[20][10];
	
	CString			m_str_part_info[20];
	CString			m_str_part_name[20];
	CString			m_str_part_axis_name[20][10];

	TSpread			*m_grid_part;
	TSpread			*m_grid_motor;

	CPicture		m_p_bmp_view;
	GridControlAlg	m_p_grid;
public:
 	CFont* mp_main_font;
public:
	CScreen_Motor();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Motor)

// Form Data
public:
	//{{AFX_DATA(CScreen_Motor)
	enum { IDD = IDD_SCREEN_MOTOR };
	CMacButton	m_btn_motor_speed;
	CXPGroupBox	m_group_part_information;
	CXPGroupBox	m_group_motor_part;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void Init_Motor_Grid();
	void OnMotor_Part_List(int n_col, int n_axis);
	void Init_Group();
	void Init_Part_Grid();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Motor)
	public:
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Motor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Motor)
	afx_msg void OnPaint();
	afx_msg void OnBtnMotorSpeed();
	afx_msg void OnBtnTeachWrite();
	afx_msg void OnBtnTeachRead();
	afx_msg void OnBtnLogTrayMotorPos();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotor_Work_Message(WPARAM wParam,LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MOTOR_H__52DCE9AA_3AEE_4C15_B5CA_E5372B940AE0__INCLUDED_)
