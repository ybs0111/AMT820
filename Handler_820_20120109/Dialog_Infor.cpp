// Dialog_Infor.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Infor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Infor dialog


CDialog_Infor::CDialog_Infor(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Infor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Infor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "User Select Dialog Box"; 
}

void CDialog_Infor::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Infor)
	DDX_Control(pDX, IDC_INFO_MESSAGE, m_info_message);
	DDX_Control(pDX, IDC_GIF_INFO, m_gif_info);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialog_Infor, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Infor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Infor message handlers

BOOL CDialog_Infor::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_info_font = NULL;
	mp_info_font = new CFont;
	mp_info_font->CreateFont(16,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	OnInfo_Button_Set();	// 대화 상자 칼라 버튼 생성 함수
	OnInfo_Lable_Set();		// 칼라 텍스트 박스 생성 함수
	OnInfo_Picture_Set();	// GIF 그림 파일 로딩 함수

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Infor::OnInfo_Button_Set()
{
	mp_info_ok = CreateOkButton(IDOK, this);
	mp_info_cancel = CreateCancelButton(IDCANCEL, this);
}

void CDialog_Infor::OnInfo_Lable_Set()
{
	m_info_message.SetFont(mp_info_font);
	m_info_message.SetWindowText(_T(st_other.str_confirm_msg));
	m_info_message.SetCenterText();
	m_info_message.SetColor(RGB(0,0,255));
	m_info_message.SetGradientColor(RGB(0,0,0));
	m_info_message.SetTextColor(RGB(255,255,255));
}

void CDialog_Infor::OnInfo_Picture_Set()
{
	if (m_gif_info.Load(MAKEINTRESOURCE(IDR_GIF_CHOICE),_T("GIF")))
		m_gif_info.Draw();
}

BOOL CDialog_Infor::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_info_font;
	mp_info_font = NULL;
	/* ************************************************************************** */

	delete mp_info_ok;      // OK 버튼에 설정된 정보 삭제 
	delete mp_info_cancel;  // Cancel 버튼에 설정된 정보 삭제 
	
	return StandardDialog::DestroyWindow();
}
