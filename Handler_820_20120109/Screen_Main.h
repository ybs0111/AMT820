#if !defined(AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_)
#define AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Main.h : header file
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
#include "CheckerCtrl.h"
#include "CharSetCtrl.h"
#include "FloatST.h"
#include "GridControlAlg.h"
#include "MatrixStatic.h"
#include "BitmapSlider.h"
/* ****************************************************************************** */
/* 쓰레드 동작 관련 클래스 추가                                                   */
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
	CFont* mp_main_font1;				// 폰트 정보 설정 변수

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
	CFont* mp_main_font;			// 폰트 정보 설정 변수
	CFont* mp_main_big_font;			// 폰트 정보 설정 변수

	/* ************************************************************************** */
	/*  Tray & Buffer 클래스 변수 선언                                            */
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 디지털 카운터 클래스 변수 선언                                             */
	/* ************************************************************************** */
	int mn_alarm_cnt;				// 알람 발생 횟수 저장 변수
	int mn_alarm_day_cnt;			// Day 알람 발생 횟수 저장 변수

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
	afx_msg LRESULT OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MAIN_H__E288A3BD_D475_476C_95BF_12DEAD7CDBAC__INCLUDED_)
