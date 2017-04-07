#if !defined(AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_)
#define AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Alarm.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가
#include "Public_Function.h"

#include "LedButton.h"		// LED 버튼 클래스
#include "BtnST.h"				// 칼라 버튼 생성 클래스 추가 
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "XpButton.h"		// XP 칼라 버튼 클래스 */
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스 
#include "MacButtons.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "BmpImageST.h"		// 알람 비트맵 로딩 클래스 추가 
#include "PictureEx.h"		// GIF 파일 로딩 클래스 추가 
#include "MatrixStatic.h"	// LCD 문자 생성 클래스 추가 
#include "MyJamData.h"		// 알람 출력 정보 로딩 및 알람 메시지 저장 클래스
#include "FloatST.h"
/////////////////////////////////////////////////////////////////////////////
// CDialog_Alarm dialog

class CDialog_Alarm : public CDialog
{
// Construction
public:
	CDialog_Alarm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Alarm)
	enum { IDD = IDD_DIALOG_ALARM };
	CLedButton	m_chk_skip_picker_12;
	CLedButton	m_chk_skip_picker_11;
	CButtonST	m_chk_picker_12;
	CButtonST	m_chk_picker_11;
	CXPGroupBox	m_group_picker_skip;
	CMacButton	m_btn_skip_button;
	CButtonST	m_chk_picker_10;
	CButtonST	m_chk_picker_9;
	CButtonST	m_chk_picker_8;
	CButtonST	m_chk_picker_7;
	CButtonST	m_chk_picker_6;
	CButtonST	m_chk_picker_5;
	CButtonST	m_chk_picker_4;
	CButtonST	m_chk_picker_3;
	CButtonST	m_chk_picker_2;
	CButtonST	m_chk_picker_1;
	CLedButton	m_chk_skip_picker_9;
	CLedButton	m_chk_skip_picker_8;
	CLedButton	m_chk_skip_picker_7;
	CLedButton	m_chk_skip_picker_6;
	CLedButton	m_chk_skip_picker_5;
	CLedButton	m_chk_skip_picker_4;
	CLedButton	m_chk_skip_picker_3;
	CLedButton	m_chk_skip_picker_2;
	CLedButton	m_chk_skip_picker_10;
	CLedButton	m_chk_skip_picker_1;
	CMacButton	m_btn_partial_no1;
	CGradientStatic	m_msg_partial1;
	CMacButton	m_btn_partial_yes1;
	CGradientStatic	m_msg_partial;
	CMatrixStatic	m_msg_lot_end;
	CFloatST	m_msg_alarm_type;
	CFloatST	m_msg_alarm_title;
	CFloatST	m_msg_alarm_repare;
	CFloatST	m_msg_alarm_code;
	CBmpImageST	m_image_alarm;
	CXPGroupBox	m_group_prev_alarm;
	CXPGroupBox	m_group_alarm_info;
	CPictureEx	m_gif_alarm;
	CNewLabel	m_data_alarm_type;
	CNewLabel	m_data_alarm_code;
	CMacButton	m_btn_prev_alarm_10;
	CMacButton	m_btn_prev_alarm_9;
	CMacButton	m_btn_prev_alarm_8;
	CMacButton	m_btn_prev_alarm_7;
	CMacButton	m_btn_prev_alarm_6;
	CMacButton	m_btn_prev_alarm_5;
	CMacButton	m_btn_prev_alarm_4;
	CMacButton	m_btn_prev_alarm_3;
	CMacButton	m_btn_prev_alarm_2;
	CMacButton	m_btn_prev_alarm_1;
	CMacButton	m_btn_buzzer_off;
	CXPButton	m_btn_alarm_ok;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_alarm_font;  // 폰트 설정 변수 선언 

	/* ************************************************************************** */
	/* 알람 출력 유형 및 출력 정보 설정 클래스 변수 선언                          */
	/* ************************************************************************** */
	CMyJamData   mcls_alarm_load;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 칼라 에디트 박스 클래스 변수 선언                                          */
	/* ************************************************************************** */
	CEditEx   m_edit_alarm_title;
	CEditEx   m_edit_alarm_repare;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 알람 출력 형태 저장되어 있는 파일 관련 정보 저장 변수                      */
	/* ************************************************************************** */
	CString mstr_data_file;		// 알람 출력 유형 정보 저장된 파일명 설정 변수 
	CString mstr_action_file;	// 알람 조치 사항 정보 저장된 파일명 설정 변수 
	/* ************************************************************************** */
// Operation
public:
	void OnAlarm_PartialError_Set();
	void OnAlarm_PrevAlarm_Display(int n_alarm_pos);
	CString OnAlarm_PrevAlarm_Check(int n_cnt);
	void OnAlarm_PrevList_Caption_Set(int n_cnt);
	void OnAlarm_PrevButton_List_Set(int n_cnt);
	void OnAlarm_PrevList_Set();
	void OnAlarm_Controls_Show(int n_state);
	void OnAlarm_Data_Display(int n_alarm_state);
	void OnAlarm_Image_Style_Set(CString strPath);
	int OnAlarm_Data_Loading();
	void OnAlarm_Picker_Change();
	void OnAlarm_SkipPickerDisplay();
	void OnAlarm_Default_Data_Load();
	void OnAlarm_SkipPicker_Set();
	void OnAlarm_EditBox_Set();
	void OnAlarm_Button_Set();
	void OnAlarm_LCD_Digital_Set();
	void OnAlarm_Label_Set();
	void OnAlarm_GroupBox_Set();
	void OnAlarm_Picture_Set();

private:
	CString* m_pstrJamCode;
	int n_last_alarm;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Alarm)
	public:
	virtual BOOL Create(CWnd* pParentWnd);				// 모달리스 대화 상자 생성 함수 변경 
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Alarm)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();					// 알람 화면 ESC 키 누른 경우 대비하여 함수 추가 
	afx_msg void OnBtnBuzzerOff();
	afx_msg void OnBtnPrevAlarm1();
	afx_msg void OnBtnPrevAlarm2();
	afx_msg void OnBtnPrevAlarm3();
	afx_msg void OnBtnPrevAlarm4();
	afx_msg void OnBtnPrevAlarm5();
	afx_msg void OnBtnPrevAlarm6();
	afx_msg void OnBtnPrevAlarm7();
	afx_msg void OnBtnPrevAlarm8();
	afx_msg void OnBtnPrevAlarm9();
	afx_msg void OnBtnPrevAlarm10();
	virtual void OnOK();
	afx_msg void OnBtnPartialYes();
	afx_msg void OnBtnPartialNo();
	afx_msg void OnChkPicker1();
	afx_msg void OnChkPicker2();
	afx_msg void OnChkPicker3();
	afx_msg void OnChkPicker4();
	afx_msg void OnChkPicker5();
	afx_msg void OnChkPicker6();
	afx_msg void OnChkPicker7();
	afx_msg void OnChkPicker8();
	afx_msg void OnChkPicker9();
	afx_msg void OnChkPicker10();
	afx_msg void OnBtnPartialYes1();
	afx_msg void OnBtnPartialNo1();
	afx_msg void OnChkPicker11();
	afx_msg void OnChkPicker12();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_)
