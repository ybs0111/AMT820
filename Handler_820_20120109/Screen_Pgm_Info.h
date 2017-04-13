#if !defined(AFX_SCREEN_PGM_INFO_H__860173B3_9510_407B_8132_58D10BF284F4__INCLUDED_)
#define AFX_SCREEN_PGM_INFO_H__860173B3_9510_407B_8132_58D10BF284F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Pgm_Info.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_Pgm_Info form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

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
#include "CheckerCtrl.h"
#include "CharSetCtrl.h"
#include "FloatST.h"
#include "GridControlAlg.h"
#include "Picture.h"

class CScreen_Pgm_Info : public CFormView
{
public:
	CFont			*m_p_font;
	GridControlAlg	m_p_grid;
	TSpread			*m_grid_info;
public:
	CScreen_Pgm_Info();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Pgm_Info)

// Form Data
public:
	//{{AFX_DATA(CScreen_Pgm_Info)
	enum { IDD = IDD_SCREEN_PGM_INFO };
	CXPGroupBox	m_group_pgm_info;
	CXPGroupBox	m_group_time;
	CXPGroupBox	m_group_motor_pos;
	CXPGroupBox	m_group_motor_part;
	CXPGroupBox	m_group_io_part;
	CXPGroupBox	m_group_io;
	CButtonST	m_btn_time_save;
	CButtonST	m_btn_time_open;
	CButtonST	m_btn_motor_pos_save;
	CButtonST	m_btn_motor_pos_open;
	CButtonST	m_btn_motor_part_save;
	CButtonST	m_btn_motor_part_open;
	CButtonST	m_btn_io_save;
	CButtonST	m_btn_io_part_save;
	CButtonST	m_btn_io_part_open;
	CButtonST	m_btn_io_open;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void Init_Button();
	void Init_Group();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Pgm_Info)
	public:
	virtual BOOL DestroyWindow();
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Pgm_Info();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Pgm_Info)
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnIoOpen();
	afx_msg void OnBtnIoPartOpen();
	afx_msg void OnBtnIoPartSave();
	afx_msg void OnBtnIoSave();
	afx_msg void OnBtnMotorPartOpen();
	afx_msg void OnBtnMotorPartSave();
	afx_msg void OnBtnMotorPosOpen();
	afx_msg void OnBtnMotorPosSave();
	afx_msg void OnBtnTimeOpen();
	afx_msg void OnBtnTimeSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_PGM_INFO_H__860173B3_9510_407B_8132_58D10BF284F4__INCLUDED_)
