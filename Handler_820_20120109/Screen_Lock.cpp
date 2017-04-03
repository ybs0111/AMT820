// Screen_Lock.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Screen_Lock.h"

/* ****************************************************************************** */
/* 대화 상자 클래스 추가                                                          */
/* ****************************************************************************** */
#include "Dialog_Pass_Check.h"
/* ****************************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Lock

IMPLEMENT_DYNCREATE(CScreen_Lock, CFormView)

CScreen_Lock::CScreen_Lock()
	: CFormView(CScreen_Lock::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Lock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Lock::~CScreen_Lock()
{
}

void CScreen_Lock::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Lock)
	DDX_Control(pDX, IDC_MSG_LOCK, m_msg_lock);
	DDX_Control(pDX, IDC_GIF_LOCK, m_gif_lock);
	DDX_Control(pDX, IDC_BTN_UNLOCK, m_btn_unlock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Lock, CFormView)
	//{{AFX_MSG_MAP(CScreen_Lock)
	ON_BN_CLICKED(IDC_BTN_UNLOCK, OnBtnUnlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Lock diagnostics

#ifdef _DEBUG
void CScreen_Lock::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Lock::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Lock message handlers

void CScreen_Lock::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_lock_font = NULL;
	mp_lock_font = new CFont;
	mp_lock_font->CreateFont(35,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	st_work.mn_run_status = dLOCK;
	st_handler.n_system_lock = TRUE;	// 시스템 잠금 상태 설정 

	OnLocking_Lable_Set();				// 칼라 텍스트 박스 생성 함수
	OnLocking_Picture_Set();			// GIF 그림 파일 로딩 함수
}


void CScreen_Lock::OnLocking_Lable_Set()
{
	m_msg_lock.SetFont(mp_lock_font);
	m_msg_lock.SetWindowText(_T("System Lock Mode! Impossible of operation!"));
	m_msg_lock.SetCenterText();
	m_msg_lock.SetColor(RGB(255,0,0));
	m_msg_lock.SetGradientColor(RGB(0,0,0));
	m_msg_lock.SetTextColor(RGB(255,255,255));
}

void CScreen_Lock::OnLocking_Picture_Set()
{
	if (m_gif_lock.Load(MAKEINTRESOURCE(IDR_GIF_LOCK),_T("GIF")))
		m_gif_lock.Draw();
}

void CScreen_Lock::OnBtnUnlock() 
{
	int nResponse;								// 대화 상자에 대한 리턴 값 저장 변수

	CDialog_Pass_Check pass_chk_dlg ;			// 암호 검사 대화 상자 클래스 변수 선언 

	st_other.str_pass_level = _T("SystemUnlock");		// 암호 레벨 정보 설정 

	nResponse = pass_chk_dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		st_handler.n_system_lock = FALSE;	// 시스템 잠금 상태 해제

		if (st_handler.cwnd_list != NULL)
		{
			st_other.str_normal_msg = "System Unlocking.";
			sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
	}
	else if (nResponse == IDCANCEL)
	{
	}
}

BOOL CScreen_Lock::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_lock_font;
	mp_lock_font = NULL;
	/* ************************************************************************** */

	st_work.mn_run_status = dSTOP;
	return CFormView::DestroyWindow();
}
