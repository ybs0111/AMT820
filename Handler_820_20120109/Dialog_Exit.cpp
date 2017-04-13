// Dialog_Exit.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Dialog_Exit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Exit dialog


CDialog_Exit::CDialog_Exit(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Exit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Exit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Program Exit Dialog Box";
}


void CDialog_Exit::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Exit)
	DDX_Control(pDX, IDC_GIF_EXIT, m_gif_exit);
	DDX_Control(pDX, IDC_MSG_EXIT, m_msg_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Exit, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Exit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Exit message handlers

BOOL CDialog_Exit::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_exit_font = NULL;
	mp_exit_font = new CFont;
	mp_exit_font->CreateFont(12,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	OnExit_Button_Set();	// 대화 상자 칼라 버튼 생성 함수
	OnExit_Lable_Set();		// 칼라 텍스트 박스 생성 함수
	OnExit_Picture_Set();	// GIF 그림 파일 로딩 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Exit::OnExit_Button_Set()
{
	mp_btn_ok =		CreateOkButton(IDOK, this);
	mp_btn_cancel = CreateCancelButton(IDCANCEL, this);
}

void CDialog_Exit::OnExit_Lable_Set()
{
	m_msg_exit.SetFont(mp_exit_font);
	m_msg_exit.SetWindowText(_T("AMT820 Program Exit"));
	m_msg_exit.SetCenterText();
	m_msg_exit.SetColor(RGB(0,0,0));
	m_msg_exit.SetGradientColor(RGB(0,0,255));
	m_msg_exit.SetTextColor(RGB(255,255,255));
}

void CDialog_Exit::OnExit_Picture_Set()
{
	if (m_gif_exit.Load(MAKEINTRESOURCE(IDR_GIF_EXIT),_T("GIF")))
		m_gif_exit.Draw();
}

BOOL CDialog_Exit::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_exit_font;
	mp_exit_font = NULL;
	/* ************************************************************************** */

	delete mp_btn_ok;		// OK 버튼에 대한 정보 삭제 
	delete mp_btn_cancel;	// Cancel 버튼에 대한 정보 삭제 
	return StandardDialog::DestroyWindow();
}
